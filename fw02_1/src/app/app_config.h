/**
 * @file app_config.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 */

#ifndef _APP_CONFIG_HEADER_H
#define _APP_CONFIG_HEADER_H

#include "min_ini.h"

#define sizearray(a)  (sizeof(a) / sizeof((a)[0]))

#define DEFAULT_LOG_SEPARATOR ","

typedef enum
{
    INI_READ_OK = 0,

    INI_PB_SCENARIO_NUM, // 1

    INI_PB_SITEID_ZONE, // 2

    INI_PB_TIME_WAKEUP_HOUR, // 3
    INI_PB_TIME_WAKEUP_MINUTE, // 4

    INI_PB_TIME_SLEEP_HOUR, // 5
    INI_PB_TIME_SLEEP_MINUTE, // 6

    INI_PB_ATTRACTIVE_LEDS_RED_A, // 7
    INI_PB_ATTRACTIVE_LEDS_GREEN_A, // 8
    INI_PB_ATTRACTIVE_LEDS_BLUE_A, // 9

    INI_PB_ATTRACTIVE_LEDS_RED_B, // 10
    INI_PB_ATTRACTIVE_LEDS_GREEN_B, // 11
    INI_PB_ATTRACTIVE_LEDS_BLUE_B, // 12

    INI_PB_ATTRACTIVE_LEDS_ALT_DELAY, // 13

    INI_PB_ATTRACTIVE_LEDS_ON_HOUR, // 14
    INI_PB_ATTRACTIVE_LEDS_ON_MINUTE, // 15

    INI_PB_ATTRACTIVE_LEDS_OFF_HOUR, // 16
    INI_PB_ATTRACTIVE_LEDS_OFF_MINUTE, // 17

    INI_PB_ATTRACTIVE_LEDS_PATTERN, // 18
        
    INI_PB_ATTRACTIVE_LEDS_PATTERN_PERCENT,
            
    INI_PB_DOOR_CLOSE_POSITION, // 20
    INI_PB_DOOR_OPEN_POSITION, // 21
    INI_PB_DOOR_CLOSING_SPEED, // 22
    INI_PB_DOOR_OPENING_SPEED, // 23

    INI_PB_DOOR_OPEN_DELAY, // 24
    INI_PB_DOOR_CLOSE_DELAY, // 25

    INI_PB_DOOR_REMAIN_OPEN, // 26

    INI_PB_DOOR_REWARD_PROBABILITY, // 27

    INI_PB_DOOR_OPEN_HOUR, // 28
    INI_PB_DOOR_OPEN_MINUTE, // 29

    INI_PB_DOOR_CLOSE_HOUR, // 30
    INI_PB_DOOR_CLOSE_MINUTE, // 31

    INI_PB_REWARD_ENABLE, // 32
    INI_PB_REWARD_TIMEOUT,  
    INI_PB_REWARD_PROBABILITY,
           
    INI_PB_TIMEOUTS_SLEEP, // 33
    INI_PB_TIMEOUTS_PIR, // 34
    INI_PB_TIMEOUTS_UNIQUE_VISIT,
        
    INI_PB_PIT_TAG_DENIED, // 37
    INI_PB_PIT_TAG_ACCEPTED, // 38

    INI_PB_PIT_TAG_COLOR_A,
    INI_PB_PIT_TAG_COLOR_B,
        
    INI_PB_PIT_TAG_LEFT,
    INI_PB_PIT_TAG_RIGHT,
    INI_PB_PIT_TAG_TOP,
    INI_PB_PIT_TAG_BOTTOM,
            
    INI_PB_PIT_TAG_LED_1,
    INI_PB_PIT_TAG_LED_2,
    INI_PB_PIT_TAG_LED_3,
    INI_PB_PIT_TAG_LED_4,
            
    INI_PB_PUNISHMENT_DELAY, // 39
    INI_PB_PUNISHMENT_PROBA_THRESH,
        
    INI_PB_DOOR_HABITUATION, // 40
       
    INI_PB_LOGS_BIRDS,
    INI_PB_LOGS_UDID,
    INI_PB_LOGS_EVENTS,
    INI_PB_LOGS_ERRORS,
    INI_PB_LOGS_BATTERY,
    INI_PB_LOGS_RFID,
        
    INI_PB_CHECK_FOOD_LEVEL,
        
    INI_PB_SECURITY_BIRD_REWARD_REOPEN
            
} INI_READ_STATE;

bool config_set( void );
FILEIO_RESULT config_find_ini( void );
INI_READ_STATE config_read_ini( void );
void config_print( void );
void getIniPbChar( INI_READ_STATE, char *, uint8_t );
FILEIO_RESULT read_PIT_tags( void );
FILEIO_RESULT read_reward_probabilities( void );

#endif /* _APP_CONFIG_HEADER_H */


/*******************************************************************************
 End of File
 */
