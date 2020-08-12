#ifndef TEMPERATURA_H    /* Guard against multiple inclusion */
#define TEMPERATURA_H

// <editor-fold defaultstate="collapsed" desc="Include Files">

#include <stdint.h>
#include <stdbool.h>

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Constants and Macros">

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Data Types">

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Global Data">


// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Interface Functions">
void TEMP_initialize(void *p_param);
void TEMP_medicion( void* p_param );
void TEMP_UmbralSet( double umbralAFijar );
// </editor-fold>

#endif /* TEMPERATURA_H */
