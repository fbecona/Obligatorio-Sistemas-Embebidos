

#ifndef UI_H    /* Guard against multiple inclusion */
#define UI_H

// <editor-fold defaultstate="collapsed" desc="Include Files">


// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Constants and Macros">


// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Data Types">

typedef enum {
    UI_MENU_STATE_MAIN,
    UI_MENU_STATE_WAIT_INPUT,
} UI_MENU_STATES;

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Global Data">


// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Interface Functions">

void UI_menuTask( void* p_param );

// </editor-fold>

#endif /* UI_H */
