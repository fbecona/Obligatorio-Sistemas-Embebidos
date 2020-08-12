
#include "dispositivo.h"
#include <string.h>

// <editor-fold defaultstate="collapsed" desc="Include Files">
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="File Scope or Global Data">

static dispositivo_t esteDispositivo;

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Local Functions">
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Interface Functions">

/** 
  @Function
    void DISP_initialize()

  @Summary
 * Incializa los valores del dispositivo
 */
void DISP_initialize(){
    esteDispositivo.idDispositivo = 0;
    strcpy(esteDispositivo.numeroTelefono,"\"099647682\"");
    esteDispositivo.umbralTemperatura = 37.6;
    strcpy(esteDispositivo.comando,"Casos");
}

/** 
  @Function
    int32_t DISP_IdGet()

  @Summary
 * Retorna el valor del Id del dispositivo
 */
int32_t DISP_IdGet(){
    return esteDispositivo.idDispositivo;
}

/** 
  @Function
    double DISP_UmbralGet()

  @Summary
 * Retorna el valor de la temperatura umbral configurada en el dispositivo
 */
double DISP_UmbralGet(){
    return esteDispositivo.umbralTemperatura;
}

/** 
  @Function
    uint8_t * DISP_TelefonoGet()

  @Summary
 * Retorna el valor del telefono configurada en el dispositivo
 */
uint8_t * DISP_TelefonoGet(){
    return esteDispositivo.numeroTelefono;
}

/** 
  @Function
    uint8_t * DISP_ComandoGet()

  @Summary
 * Retorna el valor del comando configurado en el dispositivo
 */
uint8_t * DISP_ComandoGet(){
    return esteDispositivo.comando;
}

/** 
  @Function
    void DISP_IdSet( int32_t idASetear )

  @Summary
  Permite setear el id del dispositivo
 
  @Param:  idASetear: El id a ser seteado
 */
void DISP_IdSet( int32_t idASetear ) {    
    esteDispositivo.idDispositivo = idASetear;
}

/** 
  @Function
    DISP_UmbralSet( double umbralAFijar )

  @Summary
  Permite setear el umbral del dispositivo
 
  @Param:  umbralAFijar: La temperatura umbral a fijar
 */
void DISP_UmbralSet( double umbralAFijar ) {
    esteDispositivo.umbralTemperatura = umbralAFijar;    
}

/** 
  @Function
    DISP_UmbralSet( double umbralAFijar )

  @Summary
  Permite setear el telefono asociado al dispositivo
 
  @Param:  numeroAFijar: El telefono a fijar
 */
void DISP_TelefonoSet( uint8_t * numeroAFijar ) {
    uint8_t strTemp[10];
    strcpy(strTemp,"\"");
    strcpy(esteDispositivo.numeroTelefono,numeroAFijar);
    strcat(strTemp,esteDispositivo.numeroTelefono);    
    strcat(strTemp, "\"");
    strcpy(esteDispositivo.numeroTelefono,strTemp);    
}

// </editor-fold>


