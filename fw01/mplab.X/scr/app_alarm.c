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
    // PIC24FJ256GB406    
    //    RTCCON1Lbits.WRLOCK = 0; // RTCC registers may be written by the user

    ALCFGRPTbits.ALRMEN = 0;
    ALCFGRPTbits.ALRMPTR = 1;
    ALRMVAL = 16 * ( hour / 10 ) + hour % 10;
    ALRMVAL = ( ( 16 * ( minute / 10 ) + minute % 10 ) << 4 ) + 16 * ( second / 10 ) + second % 10;

    /* 
     * You set alarm at 8:30 and a flag "EVERY_HOUR" then the alarm will raise every hour:
     * 00:30, 01:30, 02:30, and so on... 
     * But if you set the flag "EVERY_DAY" instead, your alarm will be raised only at 8:30 of each day */

    //TODO: set the alarm to EVERY_DAY in code production
//        ALCFGRPT = EVERY_10_SECONDS;
//         ALCFGRPT = EVERY_MINUTE;
    ALCFGRPT = alarmMask;

    RCFGCALbits.RTCWREN = 0;
}


/*******************************************************************************
 End of File
 */
