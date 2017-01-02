/**
 * @file app_config.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

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

    INI_PB_DOOR_OPEN_HOUR, // 24
    INI_PB_DOOR_OPEN_MINUTE, // 25

    INI_PB_DOOR_CLOSE_HOUR, // 26
    INI_PB_DOOR_CLOSE_MINUTE, // 27

    INI_PB_TIMEOUTS_SLEEP, // 28
    INI_PB_TIMEOUTS_PIR, // 29
    INI_PB_TIMEOUTS_REWARD, // 30

    INI_PB_PIT_TAG_DENIED, // 31
    INI_PB_PIT_TAG_ACCEPTED // 32

} INI_READ_STATE;

bool config_set( void );
FILEIO_RESULT config_find_ini( void );
INI_READ_STATE config_read_ini( void );
void config_print( void );
void getIniPbChar( INI_READ_STATE, char * );
FILEIO_RESULT read_PIT_tags( void );

#endif /* APP_CONFIG_H */


/*******************************************************************************
 End of File
 */
