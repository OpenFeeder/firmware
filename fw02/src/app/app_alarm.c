/**
 * @file app_alarm.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#include "app_alarm.h"


void rtcc_set_alarm( int hour, int minute, int second, int alarmMask )
{
    RTCCON1Lbits.WRLOCK = 1; // RTCVALH and RTCVALL registers can be written to by the user
    //RCFGCALbits.RTCWREN = 1; // RTCVALH and RTCVALL registers can be written to by the user


//    RTCCON1Hbits.ALRMEN = 0;
//    RTCCON1Hbits.ALRMPTR = 1;
//    ALRMVAL = 16 * ( hour / 10 ) + hour % 10;
//    ALRMVAL = ( ( 16 * ( minute / 10 ) + minute % 10 ) << 4 ) + 16 * ( second / 10 ) + second % 10;
//    ALCFGRPT = alarmMask;

    RTCCON1Lbits.WRLOCK = 0;
}


void rtcc_stop_alarm( void )
{
    RTCCON1Hbits.ALRMEN = 0;
}

/*******************************************************************************
 End of File
 */
