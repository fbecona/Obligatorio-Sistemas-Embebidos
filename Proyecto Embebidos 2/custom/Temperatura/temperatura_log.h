#ifndef LOG_TEMPERATURA_H    /* Guard against multiple inclusion */
#define LOG_TEMPERATURA_H

// <editor-fold defaultstate="collapsed" desc="Include Files">
#include <stdint.h>
#include <stdbool.h>
#include "../GPS/GPS.h"
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Constants and Macros">

#define LOG_TEMP_QTY   200
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Data Types">

typedef struct {
    time_t tiempo;
    GPSPosition_t posicion;
    double temperatura;
} log_temp_t;

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Global Data">
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Interface Functions">
log_temp_t * TEMPLOG_LogIndexGet(uint8_t index);
bool TEMPLOG_addLog(time_t timeStamp, GPSPosition_t position, double temperatura);
bool TEMPLOG_deleteLog( uint8_t indexDel);
// </editor-fold>

#endif /* LOG_TEMPERATURA_H */