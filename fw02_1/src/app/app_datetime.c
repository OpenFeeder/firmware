/**
 * @file app_datetime.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

// https://en.wikipedia.org/wiki/Real-time_clock


#include "app_datetime.h"
#include "app.h"

void setDateTime( int year, int month, int day, int hour, int minute, int second )
{
    struct tm date_time;

    date_time.tm_mday = day;
    date_time.tm_mon = month;
    date_time.tm_year = year;

    date_time.tm_hour = hour;
    date_time.tm_min = minute;
    date_time.tm_sec = second;

    RTCC_TimeSet( &date_time );
}

void getDateTime( struct tm *currentTime )
{
    /* Get Date and Time. */
    while ( !RTCC_TimeGet( currentTime ) )
    {
        Nop( );
    }
}

void getCurrentDate( void )
{
    
    while ( !RTCC_TimeGet( &appData.current_time ) )
    {
        Nop( );
    }
    
}

void calibrateCurrentDate( void )
{
   
    if ( APP_I2CRTC_DateTime_get( ) )
    {
        setDateTime( appData.i2c_current_time.year_s, 
                     appData.i2c_current_time.mon, 
                     appData.i2c_current_time.mday, 
                     appData.i2c_current_time.hour, 
                     appData.i2c_current_time.min, 
                     appData.i2c_current_time.sec );
    }
}

void printCurrentDate( void )
{
    // Print date and time on serial terminal (PC)
    
        printf( "%02u/%02u/20%02u %02u:%02u:%02u",
            appData.current_time.tm_mday,
            appData.current_time.tm_mon,
            appData.current_time.tm_year,
            appData.current_time.tm_hour,
            appData.current_time.tm_min,
            appData.current_time.tm_sec );
        
//    struct tm current_time;
//
//    /* Get current Time. */
//    while ( !RTCC_TimeGet( &current_time ) )
//    {
//        Nop( );
//    }
//
//    printf( "%02u/%02u/20%02u %02u:%02u:%02u",
//            current_time.tm_mday,
//            current_time.tm_mon,
//            current_time.tm_year,
//            current_time.tm_hour,
//            current_time.tm_min,
//            current_time.tm_sec );
}

/*******************************************************************************
 End of File
 */
