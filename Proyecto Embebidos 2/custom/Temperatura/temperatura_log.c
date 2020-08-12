// <editor-fold defaultstate="collapsed" desc="Include Files">

#include "temperatura_log.h"

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="File Scope or Global Data">

static log_temp_t c_temp_log[LOG_TEMP_QTY];
static uint8_t log_temp_index;

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Local Functions">

/** 
  @Function
    static log_temp_t* TEMPLOG_findEmpty( void ))

  @Summary
 * Busca una posicion vacia en el array que representa el LOG de datos
 * 
 */
static log_temp_t* TEMPLOG_findEmpty( void ) {
    uint8_t i;
    for( i = 0; i < LOG_TEMP_QTY; i++ ) {
        if(c_temp_log[i].tiempo == 0) {
            return &c_temp_log[i];
        }
    }
    return NULL;
}

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Interface Functions">

/** 
  @Function
    void TEMPLOG_initialize( void )

  @Summary
 * Inicializa el array de datos 
 * 
 */
void TEMPLOG_initialize( void ) {
    uint8_t i;
    for(i=0;i< LOG_TEMP_QTY;i++){
        c_temp_log[i].tiempo = 0;
    }
}

/** 
  @Function
    log_temp_t * TEMPLOG_LogIndexGet(uint8_t index)

  @Summary
  Retorna un puntero al dato indicado por el indice proporcionado
 
  @Param:  index: El indice del dato a obtener
 */
log_temp_t * TEMPLOG_LogIndexGet(uint8_t index){
    return &c_temp_log[index];
}

/** 
  @Function
    bool TEMPLOG_addLog(time_t timeStamp, GPSPosition_t position, double temperatura)

  @Summary
  Retorna un puntero al dato indicado por el indice proporcionado
  @Param:  timeStamp: La fecha representada en segundos desde 1990
           position: Almacena la longitud y la latitud obtenidas en la trama
           temperatura: La temperatura medida
 */
bool TEMPLOG_addLog(time_t timeStamp, GPSPosition_t position, double temperatura) {    
    log_temp_t* log;
    log = TEMPLOG_findEmpty();
    if( log != NULL ) {
        log->tiempo = timeStamp;
        log->posicion = position;
        log->temperatura = temperatura;
        return true;
    }
    else {
        return false;
    }
}


/** 
  @Function
    bool TEMPLOG_deleteLog( uint8_t indexDel)

  @Summary
  Retorna un puntero al dato indicado por el indice proporcionado
 
  @Param:  indexDel: El indice del dato a borrar
 */
bool TEMPLOG_deleteLog( uint8_t indexDel) {
    if(indexDel < LOG_TEMP_QTY && indexDel >= 0) {
        if(!(c_temp_log[indexDel].tiempo == 0) ) {
            c_temp_log[indexDel].tiempo = 0;
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}
// </editor-fold>


