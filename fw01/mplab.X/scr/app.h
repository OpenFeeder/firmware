/*******************************************************************************
  Sample Application Header

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
 ******************************************************************************/

#ifndef _APP_H
#define _APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Firmware Configuration
// *****************************************************************************
// *****************************************************************************

//#define DEBUG_RFID_WORKING_ON_LED_STATUS // uncomment to display hardware trace in RFID decoding routine
#define USE_UART1_SERIAL_INTERFACE       // uncomment to display information during Finite State Machine (FSM) of main application
//#define DISPLAY_USB_INFO               // uncomment to display USB information
#define DISPLAY_LOG_INFO 
//#define DISPLAY_ISR                    // uncomment to display interruption event
//#define DISPLAY_ISR_IR                 // uncomment to display interruption event
//#define DISPLAY_ISR_I2C                // uncomment to display interruption event
#define DISPLAY_CURRENT_STATE            // uncomment to display the current state
#define DISPLAY_RFID_STATE            // uncomment to display the current state
#define DISPLAY_LOG_BUFFER_INFO 
#define DISPLAY_PIT_TAG_INFO 
#define DISPLAY_REMOTE_CONTROL_INFO 
//#define DISPLAY_USB_ISR_INFO  // in USB_HostInterruptHandler( )


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "mcc_generated_files/mcc.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "fileio_config.h"
#include "usb_host_msd_scsi.h"
#include "delay.h"
#include "em4095.h"
#include "mcp23017.h"
#include "digits.h"
#include "buttons.h"
#include "led_status.h"
#include "ir_sensor.h"
#include "servomotor.h"
#include "app_attractive_leds.h"

#include "app_power.h"
#include "app_usb.h"
#include "usb_host_local.h"
#include "app_i2c.h"
#include "app_config.h"
#include "app_alarm.h"
#include "app_datetime.h"
#include "app_error.h"
#include "app_event.h"
#include "app_data_logger.h"
#include "app_remote.h"
#if defined (USE_UART1_SERIAL_INTERFACE)
#include "app_debug.h"
#endif

#define MAX_PIT_TAGS_DENIED_NUMBER 6

#define DEFAULT_TIMEOUT_READING_PIT_TAG 30

#define OPENFEEDER_IS_AWAKEN 1
#define OPENFEEDER_IS_SLEEPING 0
#define WAKE_UP 1
#define GO_TO_SLEEP 0

#define SLEEP_TIMEOUT_X1000MS_DEFAULT   60
#define PIR_TIMEOUT_X1000MS_DEFAULT   10
#define TAKING_REWARD_TIMEOUT_X1000MS_DEFAULT   10

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

/******************************************************************************/

/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
 */

typedef enum
{
    /* In this state, the application opens the driver */
    APP_STATE_INIT,

    APP_STATE_INIT_MOUNT_USB_KEY,

    APP_STATE_INIT_CONFIG_SYS,

    /* In this state, application is in IDLE state after completion. */
    APP_STATE_IDLE,

    APP_STATE_SLEEP,

    /* APP_STATE_RFID */
    APP_STATE_RFID_MESURING_RDYCLK,
    APP_STATE_RFID_READING_PIT_TAG,

    APP_STATE_OPENING_REWARD_DOOR,
    APP_STATE_WAITING_CATCH_REWARD,
    APP_STATE_CLOSING_REWARD_DOOR,

    APP_STATE_DATA_LOG,

    /* APP_STATE_SLEEP */
    APP_STATE_GO_TO_SLEEP_MODE,
    APP_STATE_WAKE_UP_FROM_SLEEP,

    APP_STATE_REMOTE_CONTROL,

    APP_STATE_BATTERY_LOW,
        
    /* Application error state */
    APP_STATE_ERROR

} APP_STATES;

typedef enum
{
    DOOR_IDLE, /* Not in use. */
    DOOR_OPENING, /* Opening in action. */
    DOOR_CLOSING /* Closing in action. */

} DOOR_STATUS;


// *****************************************************************************

/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    //uint8_t siteidzone;
    char siteid[5];
    
    /* Application current state */
    APP_STATES state; /* current state */
    APP_STATES previous_state; /* save previous state */

    /* DateTime structure */
    struct tm current_time;

    /* Declaration of FLAGS type. */
    union
    {
        uint8_t reg;

        struct
        {
            unsigned systemInit : 1; /* true if configuration success from "CONFIG.INI" */
            unsigned attractive_leds_status : 1; /* true if configuration success from "CONFIG.INI" */
            unsigned RemoteControlConnected : 1; /* true if Remote Control is connected */
            unsigned NewValidPitTag : 1; /* true if a new PIT Tag is validated. */
            unsigned : 4;
        } bit_value;
    } flags;

    /* I2C - Slave Device found */
    uint8_t i2c_add_found[MAX_OF_UNKNOWN_I2C_8_BIT_SLAVE_ADD]; // FIXME: Do not place this here !

    /* I2C - Status of MCP23017 */
    union
    {
        uint8_t status_reg;

        struct
        {
            unsigned cmd_digits : 4; /* cmd individual 7 segments digits 1 to 4: 0b0001; 0b0010; 0b0100; 0b1000; */
            unsigned : 2;
            unsigned found : 1; /* true if MCP23017 is detected when USER BUTTON PRESSED. */
            unsigned initialized : 1; /* FALSE if MCP23017 is not connected, TRUE means that PORTs have been initialized. */
        } status_bit;
    } mcp23017; // FIXME: Do not place this here !

    /* I2C - 4x 7 segments digits LEDs on MCP23017 GPIO */
    uint8_t digit[4]; /* binary value to display on each digit */

    /* I2C - Status of Buttons */
    BUTTON buttonPressed;

    /* Servomotor structure */
    DOOR_STATUS reward_door_status;
    uint16_t timeout_sleep;
    uint16_t timeout_pir;
    uint16_t timeout_taking_reward;

    uint16_t open_door_red;
    uint16_t open_door_green;
    uint16_t open_door_blue;

    uint16_t dooropendelay;
    uint16_t doorclosedelay;
    
    bool bird_is_taking_reward;

    bool openfeeder_state;
    bool rtcc_alarm_action;

    /* Battery level*/
    uint16_t battery_level;
    uint16_t vbat_level;
    

} APP_DATA;

typedef struct
{
    uint16_t red[2];
    uint16_t green[2];
    uint16_t blue[2];

    struct tm wake_up_time;
    struct tm sleep_time;
    
    uint16_t alt_delay;
    
    uint8_t current_color_index;

} APP_DATA_LEDS;

typedef struct
{
    /* Number of PIT tags denied */
    uint8_t numPitTagDenied;
    /* List of PIT tags denied */
    char pit_tags_denied[MAX_PIT_TAGS_DENIED_NUMBER][11];

} APP_DATA_PIT_TAG;

typedef struct
{
    /* Log file name - 8.3 convention - Upper case only */
    char filename[13];
    /* Data separator in log file */
    char separator[2];

    /* Character buffer to store data before export to the log file */
    char buffer[MAX_CHAR_PER_LINE * MAX_NUM_DATA_TO_STORE];
    /* Number of written characters in the buffer */
    unsigned int nCharBuffer;
    /* Number of line stored in the buffer */
    uint8_t numDataStored;

    /* Bird Data */
    struct tm bird_arrived_time;
    struct tm bird_quit_time;
    char bird_pit_tag_str[11];
    bool bird_pir_sensor_status;
    bool is_pit_tag_denied;
    bool is_reward_taken;
    
    /* Attractive LEDs color*/
    uint16_t attractive_leds_rgb[3];
    
} APP_DATA_LOG;

typedef struct
{
    struct tm raise_time;

} APP_DATA_EVENT;

//typedef struct
//{
//    /* Declaration of FLAGS type. */
//    union
//    {
//        uint8_t reg;
//
//        struct
//        {
//            unsigned pir_sensor : 1; /* true if ... */
//            unsigned ir1_sensor : 1; /* true if ...*/
//            unsigned ir2_sensor : 1; /* true if ... */
//            unsigned : 5;
//        } bits;
//    } flags;
//} APP_DATA_CAPTEURS;


// *****************************************************************************
// *****************************************************************************
// Section: extern declarations
// *****************************************************************************
// *****************************************************************************

extern APP_DATA appData; /* Main application data. */
extern APP_DATA_PIT_TAG appDataPitTag; /* Pit tag application data. */
extern APP_ERROR appError; /* Error application data. */
extern APP_DATA_USB appDataUsb; /* USB host application data. */
extern APP_DATA_ALARM appDataAlarmSleep;
extern APP_DATA_ALARM appDataAlarmWakeup;
extern APP_DATA_LEDS appDataAttractiveLeds; /* Error application data. */
extern APP_DATA_LOG appDataLog;
extern APP_DATA_SERVO appDataServo; /* Servomotor application data. */
extern APP_DATA_RC appDataRc; /* Remote control application data. */
extern APP_DATA_EVENT appDataEvent;

#define is_bird_sensor_detected( ) appDataLog.bird_pir_sensor_status /* Return the value of global variable. */
#define clear_bird_sensor_detected( ) {appDataLog.bird_pir_sensor_status = 0;} /* Clear the bird detection sensor. */

extern const char bin2ascii_tab[];

extern const FILEIO_DRIVE_CONFIG gUSBDrive;
extern volatile uint16_t counter_positive_edge_rdyclk;
extern volatile uint16_t rdyclk_count_in_10ms;
extern volatile bool g_new_value_of_em4095_rdyclk_measurement;
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
 */


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     Sample application's initialization routine

  Description:
    This routine initializes sample application's state machine.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
 */

void APP_Initialize( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    Sample application tasks function

  Description:
    This routine is the sample application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

// (en) State machine of the application.
// (fr) Machine à états de l'application.
void APP_Tasks( void );
void APP_USB_MSDTasks( void );


#endif /* _APP_H */
/*******************************************************************************
 End of File
 */
