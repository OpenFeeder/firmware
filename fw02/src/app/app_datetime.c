/**
 * @file app_datetime.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

// https://en.wikipedia.org/wiki/Real-time_clock


#include "app_datetime.h"

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

void printCurrentDate( void )
{
    // Print date and time on serial terminal (PC)
    
    struct tm current_time;

    /* Get current Time. */
    while ( !RTCC_TimeGet( &current_time ) )
    {
        Nop( );
    }

    printf( "%02u/%02u/%02u %02u:%02u:%02u",
            current_time.tm_mday,
            current_time.tm_mon,
            current_time.tm_year,
            current_time.tm_hour,
            current_time.tm_min,
            current_time.tm_sec );
}


/*******************************************************************************
 End of File
 */
