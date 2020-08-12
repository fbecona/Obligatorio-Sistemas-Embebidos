

#ifndef USB_H    /* Guard against multiple inclusion */
#define USB_H

// <editor-fold defaultstate="collapsed" desc="Include Files">

#include <stdint.h>
#include <stdbool.h>

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Constants and Macros">


// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Data Types">

typedef struct {
    uint8_t* ptr;
    uint8_t size;
    uint8_t bytesReceived;
} usb_rxDest_t;

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Global Data">


// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Interface Functions">

//void UI_menuTask( void* p_param );

void USB_initialize( void );

void USB_taskCheck( void* p_param );

void USB_send( uint8_t* p_src, uint8_t p_length );

uint8_t USB_receive( uint8_t* p_dst, uint8_t p_size );

bool USB_isConnected( void );

// </editor-fold>

#endif /* USB_H */
