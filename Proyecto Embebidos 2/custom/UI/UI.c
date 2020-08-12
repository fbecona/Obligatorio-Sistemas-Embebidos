// <editor-fold defaultstate="collapsed" desc="Include Files">

#include "UI.h"
#include "../LEDsRGB/WS2812.h"
#include "../Dispositivo/dispositivo.h"
#include "../../mcc_generated_files/rtcc.h"
#include "../../freeRTOS/include/FreeRTOS.h"
#include "../Temperatura/temperatura_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="File Scope or Global Data">

static uint8_t txt[110];
static uint8_t inputBuffer[50];
static int32_t auxInt;
static double auxTemp;
static uint8_t auxStrUrl[70];   
static uint8_t auxTiempoCadena[30];
static uint8_t auxStrTemp[5];
static uint8_t auxStrId[11];
static log_temp_t * auxLog;
static uint8_t screen_clear[]="\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Local Functions">


// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Interface Functions">

/** 
  @Function
    void UI_menuTask( void* p_param )

  @Summary
 * Tarea que se encarga de manejar la interfaz de usuario
 * Envia y recibe los datos por medio del puerto USB
 */
void UI_menuTask( void* p_param ) {
    uint8_t i;
    UI_MENU_STATES s_state_menuTask = UI_MENU_STATE_MAIN;
    bool dataValid;
    bool logConDatos;
    while( 1 ) {
        if( USB_isConnected() ) {
            switch( s_state_menuTask ) {
                case UI_MENU_STATE_MAIN:
                    vTaskDelay(pdMS_TO_TICKS(2000));
                    USB_sendS("\nMENÚ PRINCIPAL:\n");
                    USB_sendS("1_ Setear id del dispositivo\n");
                    USB_sendS("2_ Setear umbral de temperatura\n");
                    USB_sendS("3_ Setear número para envío de mensajes\n");
                    USB_sendS("4_ Revisar log de datos\n");
                    USB_sendS("5_ Eliminar log de datos\n\n");
                    s_state_menuTask = UI_MENU_STATE_WAIT_INPUT;
                    break;
                case UI_MENU_STATE_WAIT_INPUT:
                    USB_receive(inputBuffer, sizeof (inputBuffer));
                    USB_sendS("\n");
                    switch( inputBuffer[0] ) {
                        
                            // <editor-fold defaultstate="collapsed" desc="Setear id del dispositivo">
                        case '1':
                            do {
                                USB_sendS("El ID actual es : ");
                                sprintf(auxStrId,"%i",DISP_IdGet());                                
                                USB_sendS(auxStrId);
                                USB_sendS("\n");
                                USB_sendS("Ingrese el nuevo id del dispositivo: ");
                                USB_receive(inputBuffer, sizeof (inputBuffer));
                                auxInt = atoi(inputBuffer);
                                dataValid = true;
                                dataValid = dataValid && auxInt > 0;
                                dataValid = dataValid && auxInt < INT32_MAX;
                                USB_sendS("\n");
                            }while(!dataValid);
                            DISP_IdSet(auxInt);
                            USB_sendS(screen_clear);
                            USB_sendS("Id del dispositivo configurado exitosamente\n");
                            s_state_menuTask = UI_MENU_STATE_MAIN;
                            break;
                            // </editor-fold>
                            
                            // <editor-fold defaultstate="collapsed" desc="Setear umbral de temperatura">
                        case '2':
                            do {
                                //Represento la temperatura con 1 solo decimal
                                sprintf(auxStrTemp,"%.1f\n",DISP_UmbralGet());
                                USB_sendS("El Umbral actual es de : ");
                                USB_sendS(auxStrTemp);
                                USB_sendS("\n");
                                USB_sendS("Ingrese umbral de temperatura (en grados Celsius): ");
                                USB_receive(inputBuffer, sizeof (inputBuffer));
                                auxTemp = atof(inputBuffer);
                                
                                dataValid = true;
                                dataValid = dataValid && auxTemp >= 32;
                                dataValid = dataValid && auxTemp <= 42;
                                USB_sendS("\n");                                
                            }while(!dataValid); //Valido los datos sino los pido nuevamente 
                            //Guardo la temperatura
                            DISP_UmbralSet(auxTemp);
                            USB_sendS(screen_clear);
                            USB_sendS("Temperatura umbral configurada exitosamente\n");
                            s_state_menuTask = UI_MENU_STATE_MAIN;
                            break;
                            // </editor-fold> 
                            
                            // <editor-fold defaultstate="collapsed" desc="Setear número para envío de mensajes">
                        case '3':
                            do {                                
                                USB_sendS("El número actual es : ");
                                USB_sendS(DISP_TelefonoGet());
                                USB_sendS("\n");
                                USB_sendS("Ingrese número de teléfono: ");
                                USB_receive(inputBuffer, sizeof (inputBuffer));
                                auxInt = atoi(inputBuffer);
                                
                                dataValid = true;
                                dataValid = dataValid && strlen(inputBuffer) == 9;
                                dataValid = dataValid && *inputBuffer == '0';                                                              
                                dataValid = dataValid && auxInt > 91000000 && auxInt <= 99999999;
                                USB_sendS("\n");                                
                            }while(!dataValid);
                            DISP_TelefonoSet(inputBuffer);
                            USB_sendS(screen_clear);
                            USB_sendS("Número de teléfono configurado exitosamente\n");
                            s_state_menuTask = UI_MENU_STATE_MAIN;
                            break;
                            // </editor-fold>
                            
                            // <editor-fold defaultstate="collapsed" desc="Visualizar log de datos">
                        case '4':                            
                            USB_sendS("Los datos ingresados en el log son : \n\n");
                            for(i = 0; i < LOG_TEMP_QTY; i++) {                                  
                                auxLog = TEMPLOG_LogIndexGet(i);
                                if(!(auxLog->tiempo==0)) { 
                                    logConDatos = true;
                                    sprintf(auxStrTemp,"%.1f\n",auxLog->temperatura);
                                    strftime(auxTiempoCadena, sizeof(auxTiempoCadena), "%d/%m/%Y %H:%M:%S", gmtime(&auxLog->tiempo));
                                    if(auxLog->posicion.latitude != 0 && auxLog->posicion.longitude != 0){
                                        GPS_generateGoogleMaps(auxStrUrl,auxLog->posicion);
                                        sprintf(txt, "%d__ %s %s %s \n", i+1, auxTiempoCadena, auxStrUrl, auxStrTemp);
                                    }
                                    else{
                                        sprintf(txt, "%d__ %s %s \n", i+1, auxTiempoCadena, auxStrTemp);
                                    }
                                    USB_sendS(txt);
                                }
                            }
                            USB_sendS("\n");
                            if(!logConDatos){
                                USB_sendS("No hay datos en el Log \n");
                            }
                            else{
                                logConDatos = false;
                            }
                            s_state_menuTask = UI_MENU_STATE_MAIN;
                            break;
                            // </editor-fold>
                            
                            // <editor-fold defaultstate="collapsed" desc="Eliminar dato de log">
                        case '5':
                            do {
                                USB_sendS("Ingrese número de dato: \n");
                                USB_receive(inputBuffer, sizeof (inputBuffer));
                                auxInt = atoi(inputBuffer)-1;
                                dataValid = true;
                                dataValid = dataValid && auxInt >= 0;
                                dataValid = dataValid && auxInt < LOG_TEMP_QTY;
                                USB_sendS("\n");
                            }while(!dataValid);                            
                            USB_sendS(screen_clear);
                            if(TEMPLOG_deleteLog(auxInt)==true){
                                USB_sendS("El dato se ha eliminado exitosamente\n");
                            }
                            else{
                                USB_sendS("El dato no existe\n");
                            }
                            
                            s_state_menuTask = UI_MENU_STATE_MAIN;
                            break;
                            // </editor-fold>        
                    }
                    break;
                }
            }
            else if( s_state_menuTask != UI_MENU_STATE_MAIN ) {
                s_state_menuTask = UI_MENU_STATE_MAIN;
            }
            else {
            vTaskDelay(pdMS_TO_TICKS(3000));
        }
    }
}

// </editor-fold>


