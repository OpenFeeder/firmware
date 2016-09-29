#include "app_rtcc.h"

void APP_SetDateTime( uint8_t mday, uint8_t mon, uint8_t year, uint8_t hour, uint8_t min, uint8_t sec )
{
    struct tm date_time; // https://www-s.acm.illinois.edu/webmonkeys/book/c_guide/2.15.html

    /* RTCC value change from HEX to DEC with new MC3! */
    /* Static configuration of date to 04/08/2016 */
    //    date_time.tm_mday = 4;
    //    date_time.tm_mon = 8;
    //    date_time.tm_year = 16;
    //    /* and time to 20:59:30 */
    //    date_time.tm_hour = 20;
    //    date_time.tm_min = 59;
    //    date_time.tm_sec = 30;
    /* also...
     * currentTime.tm_yday = 303;
     * currentTime.tm_wday = 5; // 0 to 6 --> sunday to saturday, https://fr.wikipedia.org/wiki/Time.h
     * currentTime.tm_isdst = 1;
     */

    date_time.tm_mday = mday;
    date_time.tm_mon = mon;
    date_time.tm_year = year;
    date_time.tm_hour = hour;
    date_time.tm_min = min;
    date_time.tm_sec = sec;

    RTCC_TimeSet( &date_time );

    //    // Get current Time
    //    while ( !RTCC_TimeGet( &date_time ) )
    //    {
    //        Nop( );
    //    }
    //    printf( "Time initialise at date %02u/%02u/%02u, ", date_time.tm_mday, date_time.tm_mon, date_time.tm_year );
    //    printf( "Time %02u:%02u:%02u\n\n", date_time.tm_hour, date_time.tm_min, date_time.tm_sec );
}

void APP_GetDateTime( struct tm *currentTime )
{
    /* Get Date and Time. */
    while ( !RTCC_TimeGet( currentTime ) )
    {
        Nop( );
    }
}

void APP_PrintCurrentDateTime( void )
{
    static struct tm currentTime;

    /* Get current Time. */
    while ( !RTCC_TimeGet( &currentTime ) )
    {
        Nop( );
    }

    /* Print date and time on serial terminal (PC). */
    printf( "%02u/%02u/%02u %02u:%02u:%02u",
            currentTime.tm_mday,
            currentTime.tm_mon,
            currentTime.tm_year,
            currentTime.tm_hour,
            currentTime.tm_min,
            currentTime.tm_sec );
}
