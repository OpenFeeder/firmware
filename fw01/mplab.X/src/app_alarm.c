/**
 * @file app_alarm.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#include "app_alarm.h"


void rtcc_set_alarm( int hour, int minute, int second, int alarmMask )
{
    RCFGCALbits.RTCWREN = 1; // RTCVALH and RTCVALL registers can be written to by the user

    ALCFGRPTbits.ALRMEN = 0;
    ALCFGRPTbits.ALRMPTR = 1;
    ALRMVAL = 16 * ( hour / 10 ) + hour % 10;
    ALRMVAL = ( ( 16 * ( minute / 10 ) + minute % 10 ) << 4 ) + 16 * ( second / 10 ) + second % 10;
    ALCFGRPT = alarmMask;

    RCFGCALbits.RTCWREN = 0;
}


/*******************************************************************************
 End of File
 */
