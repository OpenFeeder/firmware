#ifndef APP_RFID_H
#define APP_RFID_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

typedef enum
{
    /* In this state, the application opens the driver and then go in IDLE state after completion */
    DISPLAY_RFID_INIT,

    RFID_IDLE,

    RFID_DETECT_COMPLET_DATASTREAM,

    RFID_DETECT_FALSE_DATASTREAM,

    /* Application error state */
    DISPLAY_RFID_ERROR

} RFID_STATES;

extern RFID_STATES g_rfid_reading_status; // for serial display of state machine process of EM4095
extern volatile uint8_t g_timeout_reading_pit_tag;

void APP_Rfid_Init( void );
bool APP_Rfid_Task( void );
bool checkPitTagDenied(void);
void clearPitTagBuffers( void );

#endif /* APP_RFID_H */
/*******************************************************************************
 End of File
 */
