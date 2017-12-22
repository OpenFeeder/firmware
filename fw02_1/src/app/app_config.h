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

    INI_PB_DOOR_TON_MIN, // 18
    INI_PB_DOOR_TON_MAX, // 19
    INI_PB_DOOR_SPEED, // 20

    INI_PB_DOOR_OPEN_DELAY, // 21
    INI_PB_DOOR_CLOSE_DELAY, // 22

    INI_PB_DOOR_REMAIN_OPEN, // 23

    INI_PB_DOOR_REWARD_PROBABILITY, // 24

    INI_PB_DOOR_OPEN_HOUR, // 25
    INI_PB_DOOR_OPEN_MINUTE, // 26

    INI_PB_DOOR_CLOSE_HOUR, // 27
    INI_PB_DOOR_CLOSE_MINUTE, // 28

    INI_PB_TIMEOUTS_SLEEP, // 29
    INI_PB_TIMEOUTS_PIR, // 30
    INI_PB_TIMEOUTS_REWARD, // 31

    INI_PB_PIT_TAG_DENIED, // 32
    INI_PB_PIT_TAG_ACCEPTED, // 33

    INI_PB_PUNISHMENT_DELAY, // 34

    INI_PB_DOOR_HABITUATION // 35
        
} INI_READ_STATE;

bool config_set( void );
FILEIO_RESULT config_find_ini( void );
INI_READ_STATE config_read_ini( void );
void config_print( void );
void getIniPbChar( INI_READ_STATE, char *, uint8_t );
FILEIO_RESULT read_PIT_tags( void );

#endif /* _APP_CONFIG_HEADER_H */


/*******************************************************************************
 End of File
 */
