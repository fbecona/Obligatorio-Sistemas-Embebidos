

#ifndef DISPOSITIVO_H    /* Guard against multiple inclusion */
#define DISPOSITIVO_H

// <editor-fold defaultstate="collapsed" desc="Include Files">

#include <stdint.h>
#include <stdbool.h>

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Constants and Macros">


// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Data Types">

typedef struct {
    int32_t idDispositivo;
    double umbralTemperatura;
    uint8_t numeroTelefono[12];
    uint8_t comando[6];
} dispositivo_t;

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Global Data">


// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Interface Functions">

void DISP_initialize();
int32_t DISP_IdGet();
double DISP_UmbralGet();
uint8_t * DISP_TelefonoGet();
uint8_t * DISP_ComandoGet();
void DISP_IdSet( int32_t idASetear );
void DISP_UmbralSet( double umbralAFijar );
void DISP_TelefonoSet( uint8_t * numeroAFijar );

// </editor-fold>

#endif /* DISPOSITIVO_H */
