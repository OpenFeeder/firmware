/**
 * @file app_remote.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#ifndef APP_REMOTE_H
#define APP_REMOTE_H

#define START_MENU_DELAY 2000


typedef enum
{
    /* In this state, the application opens the driver */
    APP_STATE_RC_INIT,

    /* In this state, application is in IDLE state after completion */
    APP_STATE_RC_IDLE,

    APP_STATE_RC_BATTERY_LEVEL_INIT,

    APP_STATE_RC_WAIT_ENTER_MENU,

    APP_STATE_RC_MANIPULATE_DIGIT

} APP_STATES_RC;


typedef enum
{
    NO = 0,
    N1_MENU,
    N2_SYSTEM,
    N3_BATTERY,
    N4_BATTERY_STATE,
    N3_PILE,
    N4_PILE_STATE,
    N3_OPERATING_TIME,
    N4_NUMBER_DAYS,
    N3_ERRORS,
    N4_ERRORS_LIST,
    N2_TIME_CLOCK,
    N3_DATE,
    N4_DATE_YEAR,
    N4_DATE_MONTH_DAY,
    N3_TIME,
    N4_TIME_HOUR_MIN,
    N3_START,
    N4_START_HOUR_MIN,
    N3_STOP,
    N4_STOP_HOUR_MIN,
    N2_TEST,
    N3_PIR,
    N4_PIR_DETECT,
    N3_RFID,
    N4_RFID_DETECT,
    N3_DOOR,
    N4_DOOR_STATUS,
    N3_IR,
    N4_IR_DETECT,
    N3_LEDS,
    N4_LEDS_R,
    N4_LEDS_G,
    N4_LEDS_B,
    N2_SITE,
    N3_SITE_ID

} MENUS_LEVELS;


typedef enum
{
    NONE = 0,
    READ,
    WRITE,
    READ_WRITE

} MENUS_MODES;


typedef enum
{
    NO_TYPE = 0,
    YEAR,
    MONTH_DAY,
    HOUR_MINUTE,
    RGB

} MENUS_VALUE_TYPES;


typedef struct
{
    MENUS_LEVELS currentMenu;
    MENUS_LEVELS parentMenu;
    MENUS_LEVELS firstChild;
    MENUS_LEVELS nextMenu;
    MENUS_LEVELS previousMenu;
    char name[6];
    unsigned int level;
    MENUS_MODES mode;
    MENUS_VALUE_TYPES valueType;

} MENU;


typedef struct
{
    APP_STATES_RC state;
    APP_STATES_RC previous_state;

    char digitBuffer[6];

} APP_DATA_RC;

extern MENU menu;

void print7Segments( const char * );
void setCurrentMenu( MENUS_LEVELS );
void printCurrentMenuValue( void );
void printCurrentMenu( void );
void getCurrentMenuValue( void );
void setCurrentMenuValue( void );
void modifyCurrentDigit( uint8_t, uint8_t );
//void displayDigitBlink( void );
//void displayDigitFixe( void );
void storeCurrentMenu( void );
void clearRemoteControlDisplay( void );

void APP_remoteControlInitialize( void );
void APP_remoteControlTask( void );


#endif /* APP_REMOTE_H */


/*******************************************************************************
 End of File
 */
