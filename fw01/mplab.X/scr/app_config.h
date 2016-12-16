/**
 * @file app_config.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "min_ini.h"

#define sizearray(a)  (sizeof(a) / sizeof((a)[0]))

#define DEFAULT_LOG_SEPARATOR ","

#define INI_NOT_FOUND -1

typedef enum {
    INI_READ_OK = 0,

    INI_PB_SCENARIO_NUM,

    INI_PB_SITEID_ZONE,

    INI_PB_TIME_WAKEUP_HOUR,
    INI_PB_TIME_WAKEUP_MINUTE,

    INI_PB_TIME_SLEEP_HOUR,
    INI_PB_TIME_SLEEP_MINUTE,

    INI_PB_ATTRACTIVE_LEDS_RED_A,
    INI_PB_ATTRACTIVE_LEDS_GREEN_A,
    INI_PB_ATTRACTIVE_LEDS_BLUE_A,

    INI_PB_ATTRACTIVE_LEDS_RED_B,
    INI_PB_ATTRACTIVE_LEDS_GREEN_B,
    INI_PB_ATTRACTIVE_LEDS_BLUE_B,

    INI_PB_ATTRACTIVE_LEDS_ALT_DELAY,

    INI_PB_ATTRACTIVE_LEDS_ON_HOUR,
    INI_PB_ATTRACTIVE_LEDS_ON_MINUTE,

    INI_PB_ATTRACTIVE_LEDS_OFF_HOUR,
    INI_PB_ATTRACTIVE_LEDS_OFF_MINUTE,

    INI_PB_DOOR_TON_MIN,
    INI_PB_DOOR_TON_MAX,
    INI_PB_DOOR_SPEED,

    INI_PB_DOOR_OPEN_DELAY,
    INI_PB_DOOR_CLOSE_DELAY,

    INI_PB_DOOR_REMAIN_OPEN,

    INI_PB_DOOR_OPEN_HOUR,
    INI_PB_DOOR_OPEN_MINUTE,

    INI_PB_DOOR_CLOSE_HOUR,
    INI_PB_DOOR_CLOSE_MINUTE,

    INI_PB_TIMEOUTS_SLEEP,
    INI_PB_TIMEOUTS_PIR,
    INI_PB_TIMEOUTS_REWARD

} INI_READ_STATE;

bool config_set(void);
int config_find_ini(void);
INI_READ_STATE config_read_ini(void);
int8_t config_check_parameters(void);
void config_print(void);
void getIniPbChar(INI_READ_STATE, char *);

#endif /* APP_CONFIG_H */


/*******************************************************************************
 End of File
 */
