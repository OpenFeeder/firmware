/**
 * @file app_alarm.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#ifndef _APP_ALARM_HEADER_H
#define _APP_ALARM_HEADER_H

#include <xc.h>
#include <time.h>

//#define EVERY_DAY        0xD800
//#define EVERY_HOUR       0xD400
//#define EVERY_10_MINUTES 0xD000
//#define EVERY_MINUTE     0xCC00
//#define EVERY_10_SECONDS 0xC800
//#define EVERY_SECOND     0xC400
#define EVERY_DAY        0b0110
#define EVERY_HOUR       0b0101
#define EVERY_10_MINUTES 0b0100
#define EVERY_MINUTE     0b0011
#define EVERY_10_SECONDS 0b0010
#define EVERY_SECOND     0b0001

typedef enum
{
    RTCC_ALARM_IDLE, /* Not in use. */
    RTCC_ALARM_WAKEUP_OPENFEEDER,
    RTCC_ALARM_SLEEP_OPENFEEDER,
    RTCC_ALARM_OPEN_DOOR,
    RTCC_ALARM_CLOSE_DOOR,
    RTCC_ALARM_SET_ATTRACTIVE_LEDS_ON,
    RTCC_ALARM_SET_ATTRACTIVE_LEDS_OFF,
    RTCC_ALARM_ALT_ATTRACTIVE_LEDS_COLOR,
    RTCC_ALARM_ALT_ATTRACTIVE_LEDS_PATTERN,
    RTCC_BATTERY_LEVEL_CHECK,
    RTCC_FOOD_LEVEL_CHECK,
    RTCC_RFID_FREQ_CHECK,
    RTCC_RTC_CALIBRATION,
    RTCC_DS3231_TEMPERATURE

} RTCC_ALARM_ACTION;

typedef struct
{
    struct tm time;

} APP_DATA_ALARM;

unsigned char ConvertHexToBCD_A( unsigned char );
void rtcc_set_alarm( int, int, int, int );
void rtcc_start_alarm( void );
void rtcc_stop_alarm( void );

#endif /* _APP_ALARM_HEADER_H */


/*******************************************************************************
 End of File
 */
