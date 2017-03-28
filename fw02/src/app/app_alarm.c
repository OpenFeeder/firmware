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
    __builtin_write_RTCC_WRLOCK( ); // Clear WRLOCK to modify RTCC

    //    RTCCON1Lbits.RTCEN = 0;
    //
    //    IFS3bits.RTCIF = false;
    //    IEC3bits.RTCIE = 0;

    RTCCON1Hbits.ALRMEN = 0;

    // set RTCC initial time
    //    ALMDATEH = ( ConvertHexToBCD( initialTime->tm_year ) << 8 ) | ConvertHexToBCD( initialTime->tm_mon ); // YEAR/MONTH-1
    //    ALMDATEL = ( ConvertHexToBCD( initialTime->tm_mday ) << 8 ) | ConvertHexToBCD( initialTime->tm_wday ); // /DAY-1/WEEKDAY
    ALMTIMEH = ( ConvertHexToBCD_A( hour ) << 8 ) | ConvertHexToBCD_A( minute ); // /HOURS/MINUTES
    ALMTIMEL = ( ConvertHexToBCD_A( second ) << 8 ); // SECOND

    //    RTCCON1Hbits.ALRMPTR = 1;
    //    ALRMVAL = 16 * ( hour / 10 ) + hour % 10;
    //    ALRMVAL = ( ( 16 * ( minute / 10 ) + minute % 10 ) << 4 ) + 16 * ( second / 10 ) + second % 10;
    RTCCON1Hbits.AMASK = alarmMask;
    RTCCON1Hbits.CHIME = 1;
    RTCCON1Hbits.ALRMEN = 1;

    //    RTCCON1Lbits.WRLOCK = 0;
    // Clear WRLOCK to modify RTCC as needed
    //__builtin_write_RTCC_WRLOCK( );
    RTCCON1Lbits.WRLOCK = 1; // RTCVALH and RTCVALL registers can be written to by the user
    // RCFGCALbits.RTCWREN = 1; // RTCVALH and RTCVALL registers can be written to by the user --> on 44 pin

    // Enable RTCC, clear RTCWREN         
    //   RTCCON1Lbits.RTCEN = 1;
    //   RTCC_Lock();

    //Enable RTCC interrupt
    //    IEC3bits.RTCIE = 1;
}

void rtcc_stop_alarm( void )
{
    RTCCON1Hbits.ALRMEN = 0;
}


/*******************************************************************************
 End of File
 */
