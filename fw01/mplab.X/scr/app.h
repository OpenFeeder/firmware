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


#define USE_UART1_SERIAL_INTERFACE  // uncomment to display information dsent to UART
#define DISPLAY_CURRENT_STATE       // uncomment to display the current state of main state machine (app.c))

//#define DISPLAY_USB_INFO          // uncomment to display USB information
#define DISPLAY_LOG_INFO 

//#define DISPLAY_ISR_RTCC          // uncomment to display interruption event
//#define DISPLAY_ISR_IR              // uncomment to display interruption event
//#define DISPLAY_ISR_PIR           // uncomment to display interruption event
//#define DISPLAY_ISR_RFID          // uncomment to display interruption event
//#define DISPLAY_ISR_I2C           // uncomment to display interruption event
//#define DISPLAY_USB_ISR_INFO      // in USB_HostInterruptHandler( )

//#define DISPLAY_RFID_STATE        // uncomment to display the current state
//#define DISPLAY_PIT_TAG_INFO 
//#define DEBUG_RFID_WORKING_ON_LED_STATUS // uncomment to display hardware trace in RFID decoding routine

#define DISPLAY_REMOTE_CONTROL_INFO 


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


#include "app_alarm.h"
#include "app_attractive_leds.h"
#include "app_check.h"
#include "app_config.h"
#include "app_data_logger.h"
#include "app_datetime.h"
#if defined (USE_UART1_SERIAL_INTERFACE)
#include "app_debug.h"
#endif
#include "app_error.h"
#include "app_event.h"
#include "app_i2c.h"
#include "app_power.h"
#include "app_remote.h"
#include "app_rfid.h"
#include "app_timers_callback.h"
#include "app_usb.h"

#include "usb_host_local.h"

#define DEFAULT_TIMEOUT_READING_PIT_TAG 30

#define OPENFEEDER_IS_AWAKEN    1
#define OPENFEEDER_IS_SLEEPING  0

#define STANDBY_TIMEOUT_X1000MS_DEFAULT         60
#define PIR_TIMEOUT_X1000MS_DEFAULT             10
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
    APP_STATE_INITIALIZE,

    APP_STATE_CONFIGURE_SYSTEM,

    APP_STATE_IDLE,

    APP_STATE_RFID_READING_PIT_TAG,

    APP_STATE_OPENING_DOOR,
    APP_STATE_WAITING_CATCH_REWARD,
    APP_STATE_CLOSING_DOOR,

    APP_STATE_DATA_LOG,

    APP_STATE_STANDBY,
    APP_STATE_SLEEP,
    APP_STATE_WAKE_UP,

    APP_STATE_REMOTE_CONTROL,

    APP_STATE_LOW_BATTERY,
    APP_STATE_LOW_FOOD_LEVEL,
    APP_STATE_LOW_RFID_FREQUENCY,

    /* Application error state */
    APP_STATE_ERROR

} APP_STATES;


typedef enum
{
    DOOR_IDLE, /* Not in use. */
    DOOR_OPENED,
    DOOR_CLOSED,
    DOOR_OPENING, /* Opening in action. */
    DOOR_CLOSING /* Closing in action. */

} DOOR_STATUS;


typedef enum
{
    RTCC_ALARM_IDLE, /* Not in use. */
    RTCC_ALARM_WAKEUP_OPENFEEDER,
    RTCC_ALARM_SLEEP_OPENFEEDER,
    RTCC_ALARM_OPEN_DOOR,
    RTCC_ALARM_CLOSE_DOOR,
    RTCC_ALARM_SET_ATTRACTIVE_LEDS_ON,
    RTCC_ALARM_SET_ATTRACTIVE_LEDS_OFF,
    RTCC_ALARM_ALT_ATTRACTIVE_LEDS,
    RTCC_BATTERY_LEVEL_CHECK

} RTCC_ALARM_ACTION;


typedef enum
{
    NO_SCENARIO,
    OPEN_BAR,
    LONG_TERM_SPATIAL_MEMORY,
    WORKING_SPATIAL_MEMORY,
    COLOR_ASSOCIATIVE_LEARNING

} SCENARIO;

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
    uint8_t scenario_number;
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
    uint16_t timeout_standby;
    uint16_t timeout_pir;
    uint16_t timeout_taking_reward;

    bool bird_is_taking_reward;

    bool openfeeder_state;
    RTCC_ALARM_ACTION rtcc_alarm_action;

    /* Battery level*/
    uint16_t battery_level;
    uint16_t vbat_level;

    uint16_t rfid_rdyclk;


} APP_DATA;


typedef struct
{
    uint16_t red[2];
    uint16_t green[2];
    uint16_t blue[2];

    struct tm wake_up_time;
    struct tm sleep_time;

    uint8_t alt_delay;
    uint8_t alt_sec_elapsed;

    bool status;

    uint8_t current_color_index;

} APP_DATA_LEDS;


typedef struct
{
    uint16_t open_delay;
    uint16_t close_delay;

    struct tm open_time;
    struct tm close_time;

    uint8_t remain_open;

    DOOR_STATUS reward_door_status;

} APP_DATA_DOOR;


typedef struct
{
    /* Number of PIT tags denied or associated with color A */
    uint16_t numPitTagDeniedOrColorA;
    /* Number of PIT tags accepted or associated with color B */
    uint16_t numPitTagAcceptedOrColorB;
    /* List of PIT tags denied */
    char pit_tags_list[MAX_PIT_TAGS_LIST_NUMBER][11];

    bool isPitTagdeniedOrColorA[MAX_PIT_TAGS_LIST_NUMBER];

    bool didPitTagMatched;

    uint16_t pitTagIndexInList;

    uint8_t number_of_valid_pit_tag;

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
    uint8_t attractive_leds_current_color_index;

    uint8_t door_status_when_bird_arrived;

} APP_DATA_LOG;


typedef struct
{
    struct tm raise_time;

} APP_DATA_EVENT;

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
extern APP_DATA_DOOR appDataDoor;

#define is_bird_sensor_detected( ) appDataLog.bird_pir_sensor_status /* Return the value of global variable. */
#define clear_bird_sensor_detected( ) {appDataLog.bird_pir_sensor_status = 0;} /* Clear the bird detection sensor. */

extern const FILEIO_DRIVE_CONFIG gUSBDrive;

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
