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

#ifndef _APP_HEADER_H
#define _APP_HEADER_H

// *****************************************************************************
// *****************************************************************************
// Section: Firmware Configuration
// *****************************************************************************
// *****************************************************************************

#define DELAY_BEFORE_RESET 5000

#define USE_UART1_SERIAL_INTERFACE  // uncomment to display information dsent to UART
#define DISPLAY_CURRENT_STATE       // uncomment to display the current state of main state machine (app.c))

#define DISPLAY_INI_READ_DATA
//#define DISPLAY_USB_INFO          // uncomment to display USB information
//#define DISPLAY_LOG_INFO 
#define DISPLAY_CHECK_INFO 
//#define DISPLAY_EVENT_INFO
//#define DISPLAY_I2C_STATUS
//#define DISPLAY_I2C_SCANNER
//#define DISPLAY_LEDS_STATUS
//#define DISPLAY_PCA9622_STATUS
//#define DISPLAY_ISR_RTCC          // uncomment to display interruption event
//#define DISPLAY_ISR_IR              // uncomment to display interruption event
//#define DISPLAY_ISR_PIR           // uncomment to display interruption event
//#define DISPLAY_ISR_RFID          // uncomment to display interruption event
//#define DISPLAY_ISR_I2C           // uncomment to display interruption event
//#define DISPLAY_USB_ISR_INFO      // in USB_HostInterruptHandler( )

//#define DISPLAY_RFID_STATE        // uncomment to display the current state
//#define DISPLAY_PIT_TAG_INFO 
//#define DEBUG_RFID_WORKING_ON_LED_STATUS // uncomment to display hardware trace in RFID decoding routine

#define DISPLAY_RESET_INFORMATION

#define DISPLAY_REWARD_PROBABILITY

//#define DISPLAY_REMOTE_CONTROL_INFO 

#define DISPLAY_RESET_REGISTERS

#define DISPLAY_SERVO_INFO
//#define DISPLAY_SERVO_POSITION

//#define TEST_RTCC_SLEEP_WAKEUP

#define ENABLE_ERROR_LEDS

#define ENABLE_DEEP_SLEEP

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "mcc.h"
#include "stdio.h"

#include "delay.h"
#include "em4095.h"
#include "mcp23017.h"
#include "pca9622.h"
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
#include "app_fileio.h"
#include "app_scenario.h"
#include "app_rtc_action.h"
#include "app_version.h"
#include "app_stimuli.h"

#include "oc5.h"

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

#define MAX_NUM_RESET 5
#define OPENFEEDER_IS_AWAKEN    1
#define OPENFEEDER_IS_SLEEPING  0
#define MAX_NUM_REWARD_TIMEOUT 5
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
        
    APP_STATE_SERIAL_COMMUNICATION,

    APP_STATE_RFID_READING_PIT_TAG,

    APP_STATE_OPENING_DOOR,
    APP_STATE_WAITING_CATCH_REWARD,
    APP_STATE_CLOSING_DOOR,
    APP_STATE_REOPEN_DOOR,

    APP_STATE_DATA_LOG,

    APP_STATE_STANDBY,
    APP_STATE_SLEEP,
    APP_STATE_WAKE_UP,

    APP_STATE_FLUSH_DATA_TO_USB,
    APP_STATE_REMOVE_USB_DEVICE,

    APP_STATE_FLUSH_DATA_BEFORE_ERROR,
    APP_STATE_TEST_RFID,
        
    /* Application error state */
    APP_STATE_ERROR

} APP_STATES;

typedef enum
{
    NO_SCENARIO,
    OPEN_BAR,
    DOOR_HABITUATION,
    GO_NO_GO,
    LONG_TERM_SPATIAL_MEMORY,
    WORKING_SPATIAL_MEMORY,
    COLOR_ASSOCIATIVE_LEARNING,
    RISK_AVERSION,
    PATCH_PROBABILITY
        
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
    struct ts i2c_current_time;

    union
    {
        uint16_t reg;

        struct
        {
            unsigned num_software_reset : 3;
            unsigned : 13;
        } bit_value;
    } dsgpr0;
    
    union
    {
        uint16_t reg;

        struct
        {
            unsigned : 16;
        } bit_value;
    } dsgpr1;
    
    union
    {
        uint16_t reg;

        struct
        {
            unsigned por : 1;
            unsigned bor : 1;
            unsigned idle : 1;
            unsigned sleep : 1;
            unsigned wdto : 1;
            unsigned swdten : 1;
            unsigned swr : 1;
            unsigned extr : 1;
            unsigned pmslp : 1;
            unsigned cm : 1;
            unsigned dpslp : 1;
            unsigned : 1;
            unsigned reten : 1;
            unsigned : 1;
            unsigned iopuwr : 1;
            unsigned trapr : 1;
        } bit_value;
    } reset_1;
    
    union
    {
        uint16_t reg;

        struct
        {
            unsigned vbat : 1;
            unsigned vbpor : 1;
            unsigned vddpor : 1;
            unsigned vddbor : 1;
            unsigned : 12;
        } bit_value;
    } reset_2;
    
    /* Declaration of FLAGS type. */
    union
    {
        uint8_t reg;

        struct
        {
            unsigned system_init : 1; /* true if configuration success from "CONFIG.INI" */
            unsigned attractive_leds_status : 1; /* true if configuration success from "CONFIG.INI" */
            unsigned remote_control_connected : 1; /* true if Remote Control is connected */
            unsigned new_valid_pit_tag : 1; /* true if a new PIT Tag is validated. */
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
    BUTTON button_pressed;

    /* Servomotor structure */
    uint16_t timeout_standby;
    uint16_t timeout_pir;
    uint16_t timeout_taking_reward;
    uint16_t timeout_guillotine;
        
    bool bird_is_taking_reward;
    
    uint8_t reward_enable;

    bool openfeeder_state;
    RTCC_ALARM_ACTION rtcc_alarm_action;

    /* Battery level*/
    uint16_t battery_level;
    uint16_t vbat_level;
    uint16_t light_level;

    uint16_t rfid_rdyclk;

    bool rfid_signal_detected;

    APP_STATES rc_previous_state;

    uint16_t punishment_delay;
    uint8_t punishment_proba_thresh;
    
    bool need_to_reconfigure;
    
    bool pir_sensor_powered;
    bool servo_powered;
    
    bool test_rfid;
    
    struct
    {
        uint8_t family;
        uint8_t device;
        uint8_t revision;
    } id ;

    struct
    {
        uint32_t words[5];
    } udid ;
    
    bool chk_food_level;
    
    bool secu_bird_reward_reopen;
    bool secu_guillotine;
    uint16_t secu_guillotine_offset;
    
    float ext_temperature;
    
    bool punishment_state;
    
} APP_DATA;

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
extern APP_DATA_STIMULI appDataStimuli; /* Stimuli application data */

extern volatile bool is_bird_detected;

//******************************************************************************
//******************************************************************************
// Function prototypes
//******************************************************************************
//******************************************************************************

//#define is_bird_sensor_detected( ) appDataLog.bird_pir_sensor_status /* Return the value of global variable. */
//#define clear_bird_sensor_detected( ) {appDataLog.bird_pir_sensor_status = 0;} /* Clear the bird detection sensor. */


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

#endif /* _APP_HEADER_H */


/*******************************************************************************
 End of File
 */
