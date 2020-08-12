// <editor-fold defaultstate="collapsed" desc="Include Files">

#include "../../freeRTOS/include/FreeRTOS.h"
#include "../../freeRTOS/include/semphr.h"
#include "../../mcc_generated_files/adc1.h"
#include "../../mcc_generated_files/pin_manager.h"

#include "temperatura.h"
#include "../Dispositivo/dispositivo.h"
#include "../GPS/GPS.h"
#include "../LEDsRGB/WS2812.h"
#include "../SIM808/SIM808.h"

#include <stdio.h>
#include <time.h>
#include <string.h>

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="File Scope or Global Data">
static SemaphoreHandle_t medicionTemp;
static BaseType_t xHigherPriorityTaskWoken;

static bool antiBouncer;
static bool anuladorMedida;
static bool anularMedida;

static struct tm fecha;
static GPSPosition_t posicion;
static double medida;

static uint8_t trama[100];
static uint8_t txt[100];

static uint8_t tiempoCadena[30];
static uint8_t url[60];
static uint8_t medidaCadena[5];

static double medidas;
static int i;
static uint8_t casos_positivos;
// </editor-fold>    

// <editor-fold defaultstate="collapsed" desc="Local Functions">

/** 
  @Function
    void habilitarMedicionTemperatura()

  @Summary
 * Rutina de atencion a la interrupcion del boton
 * Otorga un permiso al semaforo para que se pueda tomar una medida
 */
void habilitarMedicionTemperatura() {
    if(!antiBouncer){
        if(!anuladorMedida){
            xHigherPriorityTaskWoken = pdFALSE;
            xSemaphoreGiveFromISR(medicionTemp,&xHigherPriorityTaskWoken);
            anuladorMedida = true;
        }else{
            anularMedida = true;
        }
        antiBouncer = true;
    }
}

/** 
  @Function
    void obtenerPosicionGPS()

  @Summary
 * Permite obtener una trama del GPS 
 */
void obtenerPosicionGPS(){
    //if( c_semGPSIsReady != NULL ){
        //if( xSemaphoreTake(c_semGPSIsReady, portMAX_DELAY ) == pdTRUE)
        //{
            //SIM808_getNMEA(trama);    
            //Uso una trama para simular el GPS
            //Trama valida
            strcpy(trama, "+CGNSINF: 1,1,20200715213000.000,-32.370193,-54.172768,117.100");             
            //Trama invalida
            //strcpy(trama, "+CGNSINF: 1,0,20200715213000.000,-32.370193,-54.172768,117.100"); 
            
            
            //Si la trama es valida
            if (SIM808_validateNMEAFrame(trama))
            {
                //Obtengo la posicion
                GPS_getPosition(&posicion, trama);                
            }
            else{
                //Si la trama no es valida lo represento seteando logitud y latitud en 0
                posicion.latitude=0;
                posicion.longitude=0;
            }
            //xSemaphoreGive(c_semGPSIsReady);
       //}
    //}
}


/** 
  @Function
    void iniciarRTC()

  @Summary
 * Toma una trama valida del GPS y guarda la fecha en el RTC
 */
void iniciarRTC(){
    //if( c_semGPSIsReady != NULL ){
        //if( xSemaphoreTake(c_semGPSIsReady, portMAX_DELAY ) == pdTRUE)
        //{
            //SIM808_getNMEA(trama);    
            //Uso una trama fija para simular el GPS
            strcpy(trama, "+CGNSINF: 1,1,20200715213000.000,-32.370193,-54.172768,117.100"); 
            //Si la trama es valida
            if (SIM808_validateNMEAFrame(trama)) 
            {                
                //Obtengo la fecha
                GPS_getUTC(&fecha, trama);
                //Seteo la fecha en el RTC
                RTCC_TimeSet(&fecha);                
            }
            //xSemaphoreGive(c_semGPSIsReady);
       //}
    //}
}


/** 
  @Function
    void iniciarRTC()

  @Summary
 * Hace las tareas relacionadas con los datos de la medida tomada 
 * Se guarda el dato en el Log de datos
 * Se genera la cadena de texto que sera enviada por mensaje en caso de ser necesario
 */
void procesarDato(){
    //Obtengo la fecha desde el RTC
    RTCC_TimeGet(&fecha);    
    time_t tiempo = mktime(&fecha);
    TEMPLOG_addLog(tiempo, posicion, medida);
    sprintf(medidaCadena,"%.1f",medida);
    strftime(tiempoCadena, sizeof(tiempoCadena), "%d/%m/%Y %H:%M:%S", gmtime(&tiempo));
    //Si la posicion es distinta de 0
    if(posicion.latitude != 0 && posicion.longitude != 0){
        GPS_generateGoogleMaps(url,posicion);    
        sprintf(txt, "%i %s %s %s \n", DISP_IdGet(), tiempoCadena, url, medidaCadena);     
    }
    else{
        sprintf(txt, "%i %s %s \n", DISP_IdGet(), tiempoCadena, medidaCadena);     
    }
}

/** 
  @Function
    void enviarSMS(void *p_param)

  @Summary
 * Tarea encargada de enviar un mensaje de texto
 * Deshabilita la accion del boton y enciende el LED que indica el envio del mensaje
 * al finalizar activa nuevamente le boton y apaga el LED que representa el envio
 */
 
void enviarSMS(){
   //Deshabilito el boton cambiando la ISR
   BTN1_SetInterruptHandler(&BTN1_CallBack);
   //Enciendo la luz mientras se envia el mensaje
   LEDA_SetHigh();
   if( xSemaphoreTake( c_semGSMIsReady, portMAX_DELAY ) == pdTRUE )
   {
       SIM808_sendSMS(DISP_TelefonoGet(), txt);
       xSemaphoreGive( c_semGSMIsReady );
   }    
   //Habilito el boton cambiando la ISR
   BTN1_SetInterruptHandler(&habilitarMedicionTemperatura);
   //Apago la luz
   LEDA_SetLow();   
}


/** 
  @Function
    void atencionResultado()

  @Summary
 * Esta funcion se encarga de procesar el resultado de la temperatura tomada
 * Encendiende el anillo representando el resultado 
 * Llama a la funcion de envio de mensaje y proceso de datos  
 */
void atencionResultado(){
    obtenerPosicionGPS();
    procesarDato();
    if(medida >= DISP_UmbralGet()){
        anilloColor(RED);
        vTaskDelay(pdMS_TO_TICKS(2000));
        anilloColor(BLACK);        
        //xTaskCreate(enviarSMS, "envioSMS", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
        enviarSMS();
        casos_positivos++;
    }else{
        anilloColor(GREEN);
        vTaskDelay(pdMS_TO_TICKS(2000));
        anilloColor(BLACK);
    }
    antiBouncer = false;
    anuladorMedida = false;
    anularMedida = false;
    
}

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Interface Functions">

/** 
  @Function
    void TEMP_initialize(void *p_param)

  @Summary
 * Tarea que se encarga de inicializar las variables y el RTC
 * Indica el estado por medio del anillo de LEDS
 */
void TEMP_initialize(void *p_param){
    while( 1 ){                 
        //Inicializar variables
        antiBouncer = false;
        anuladorMedida = false;
        anularMedida = false;
        medicionTemp = xSemaphoreCreateBinary();
        //Prendo el anillo en blanco para representar la carga
        anilloColor(WHITE);
        //Simulo un tiempo para que el GPS obtenga la trama
        vTaskDelay(pdMS_TO_TICKS(20000));        
        //Obtengo la trama
        iniciarRTC();

        //Prendo el anillo en verde para indicar que ya se pueden tomar temperaturas    
        anilloColor(GREEN);
        vTaskDelay(pdMS_TO_TICKS(2000));        
        //Apago el anillo RGB
        anilloColor(BLACK);   
        //Inicializar LOG
        TEMPLOG_initialize();    
        //Seteo la ISR del Boton S2
        BTN1_SetInterruptHandler(&habilitarMedicionTemperatura);
        vTaskDelete(NULL);
    }
}


/** 
  @Function
    void TEMP_medicion(void *p_param)

  @Summary
 * Tarea que se encarga de tomar una medida
 * Lee las medidas del ADC
 * Se utiliza un semaforo que espera por la interrupcion del boton que dispara las medidas
 * 
 */
void TEMP_medicion(void *p_param) {
    while(1){
        medidas = 0;
        medida = 32;
        if(medicionTemp != NULL)
        {
            if(xSemaphoreTake( medicionTemp, portMAX_DELAY ) == pdTRUE)
            {
                vTaskDelay(pdMS_TO_TICKS(300)); //delay anti-bouncer, menos de esto da problema(dijo bruno))
                antiBouncer = false;
                for(i = 0 ; i<10; i++) { 
                    if(anularMedida){
                        anilloColor(BLACK);
                        vTaskDelay(pdMS_TO_TICKS(300)); //delay anti-bouncer para la cancelaci?n de medida, menos de esto da problema(dijo bruno))
                        antiBouncer = false;
                        break;
                    }
                    if(i%2==0){
                        anilloColor(BLUE); 
                    }
                    else{
                        anilloColor(BLACK); 
                    }
                    ADC1_ChannelSelect(POT);
                    ADC1_SoftwareTriggerEnable();
                    vTaskDelay(pdMS_TO_TICKS(250));
                    ADC1_SoftwareTriggerDisable();
                    while(!ADC1_IsConversionComplete(POT));
                    medidas += ADC1_ConversionResultGet(POT);
                }
                if(anularMedida){
                    anuladorMedida = false;
                    anularMedida = false;
                    continue;
                }
                medida += medidas/1023;
                atencionResultado();
            }
        }
    }
}


/** 
  @Function
    uint8_t TEMP_casosPositivosGet()

  @Summary
 * Retorna la cantidas de casos positivos
 * 
 */
uint8_t TEMP_casosPositivosGet(){
    return casos_positivos;
}

// </editor-fold>


