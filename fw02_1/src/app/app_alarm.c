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

    // Disable RTCC alarm
    RTCCON1Hbits.ALRMEN = 0;

    // set RTCC initial time
    //    ALMDATEH = ( ConvertHexToBCD( initialTime->tm_year ) << 8 ) | ConvertHexToBCD( initialTime->tm_mon ); // YEAR/MONTH-1
    //    ALMDATEL = ( ConvertHexToBCD( initialTime->tm_mday ) << 8 ) | ConvertHexToBCD( initialTime->tm_wday ); // /DAY-1/WEEKDAY
    ALMTIMEH = ( ConvertHexToBCD_A( hour ) << 8 ) | ConvertHexToBCD_A( minute ); // /HOURS/MINUTES
    ALMTIMEL = ( ConvertHexToBCD_A( second ) << 8 ); // SECOND

    RTCCON1Hbits.AMASK = alarmMask;
    
    /* Indefinite repetition of the alarm  */       
    RTCCON1Hbits.CHIME = 1;
    
    // Enable RTCC alarm
    RTCCON1Hbits.ALRMEN = 1;

}

void rtcc_start_alarm( void )
{
    // Enable RTCC alarm
    RTCCON1Hbits.ALRMEN = 1;
}

void rtcc_stop_alarm( void )
{
    // Disable RTCC alarm
    RTCCON1Hbits.ALRMEN = 0;
}


/*******************************************************************************
 End of File
 */
