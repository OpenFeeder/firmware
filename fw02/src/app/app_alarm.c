/**
 * @file app_alarm.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#include "app_alarm.h"
//#include sdtint

unsigned char ConvertHexToBCD_A( unsigned char hexvalue )
{
    unsigned char bcdvalue;
    bcdvalue = ( hexvalue / 10 ) << 4;
    bcdvalue = bcdvalue | ( hexvalue % 10 );
    return ( bcdvalue );
}

void rtcc_set_alarm( int hour, int minute, int second, int alarmMask )
{
    RTCCON1Lbits.WRLOCK = 1; // RTCVALH and RTCVALL registers can be written to by the user
    //RCFGCALbits.RTCWREN = 1; // RTCVALH and RTCVALL registers can be written to by the user --> on 44 pin


    //    RTCCON1Hbits.ALRMEN = 0;
    //    RTCCON1Hbits.ALRMPTR = 1;
    //    ALRMVAL = 16 * ( hour / 10 ) + hour % 10;
    //    ALRMVAL = ( ( 16 * ( minute / 10 ) + minute % 10 ) << 4 ) + 16 * ( second / 10 ) + second % 10;
    //    ALCFGRPT = alarmMask;

    RTCCON1Hbits.ALRMEN = 0;

    // set RTCC initial time
    //   DATEH = (ConvertHexToBCD(initialTime->tm_year) << 8) | ConvertHexToBCD(initialTime->tm_mon) ;  // YEAR/MONTH-1
    //   DATEL = (ConvertHexToBCD(initialTime->tm_mday) << 8) | ConvertHexToBCD(initialTime->tm_wday) ;  // /DAY-1/WEEKDAY
    TIMEH = ( ConvertHexToBCD_A( hour ) << 8 ) | ConvertHexToBCD_A( minute ); // /HOURS/MINUTES
    TIMEL = ( ConvertHexToBCD_A( second ) << 8 ); // SECOND

    //    RTCCON1Hbits.ALRMPTR = 1;
    //    ALRMVAL = 16 * ( hour / 10 ) + hour % 10;
    //    ALRMVAL = ( ( 16 * ( minute / 10 ) + minute % 10 ) << 4 ) + 16 * ( second / 10 ) + second % 10;
    RTCCON1Hbits.AMASK = alarmMask;
    RTCCON1Hbits.CHIME = 1;
    RTCCON1Hbits.ALRMEN = 1;

    RTCCON1Lbits.WRLOCK = 0;
}

void rtcc_stop_alarm( void )
{
    RTCCON1Hbits.ALRMEN = 0;
}

/*******************************************************************************
 End of File
 */
