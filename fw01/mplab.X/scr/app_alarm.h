/**
 * @file app_alarm.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#ifndef APP_ALARM_H
#define APP_ALARM_H

#include <xc.h>
#include <stdint.h>
#include <time.h>

#define EVERY_DAY        0xD800
#define EVERY_HOUR       0xD400
#define EVERY_10_MINUTES 0xD000
#define EVERY_MINUTE     0xCC00
#define EVERY_10_SECONDS 0xC800
#define EVERY_SECOND     0xC400

typedef struct
{
    struct tm time;

} APP_DATA_ALARM;


void rtcc_set_alarm( int, int, int, int );


#endif /* APP_ALARM_H */


/*******************************************************************************
 End of File
 */
