/**
 * @file app_datetime.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

// https://en.wikipedia.org/wiki/Real-time_clock


#include "app_datetime.h"
#include "app.h"

uint16_t current_millis;
// current_millis = TMR1_Counter16BitGet( ); // lecture des milliseconds


bool setDateTime( int year, int month, int day, int hour, int minute, int second )
{
    struct tm date_time;

    date_time.tm_mday = day;
    date_time.tm_mon = month;
    date_time.tm_year = year;

    date_time.tm_hour = hour;
    date_time.tm_min = minute;
    date_time.tm_sec = second;

    RTCC_TimeSet( &date_time );
    
    return true;
}

bool getDateTime( void )
{
    while ( !RTCC_TimeGet( &appData.current_time ) )
    {
        Nop( );
    }
    
    return true;
}

void calibrateDateTime( void )
{
    
    if (0 == APP_I2CMasterSeeksSlaveDevice(DS3231_I2C_ADDR, DS3231_I2C_ADDR))
    {
        if ( true == appDataLog.log_events )
        {
           store_event(OF_DS3231_NOT_FOUND); 
        }
        
        #if defined (USE_UART1_SERIAL_INTERFACE)
            printf( "\tDS3231 (external RTC) not found.\n");
        #endif
    }
    else 
    {
        if ( getExtDateTime( ) )
        {
            if (0==appData.i2c_current_time.year_s && 1==appData.i2c_current_time.mon && 1==appData.i2c_current_time.mday) {
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_CALIBRATE_TIME_FAIL); 
                }
            }
            else {
                setDateTime( appData.i2c_current_time.year_s, 
                             appData.i2c_current_time.mon, 
                             appData.i2c_current_time.mday, 
                             appData.i2c_current_time.hour, 
                             appData.i2c_current_time.min, 
                             appData.i2c_current_time.sec );

                if ( true == appDataLog.log_events )
                {
                   store_event(OF_CALIBRATE_TIME); 
                }
            }       
        }
        else
        {
            if ( true == appDataLog.log_events )
            {
               store_event(OF_CALIBRATE_TIME_FAIL); 
            }
        }
    }
}

void printDateTime( void )
{
    // Print date and time on serial terminal (PC)
    
        printf( "%02u/%02u/20%02u %02u:%02u:%02u",
            appData.current_time.tm_mday,
            appData.current_time.tm_mon,
            appData.current_time.tm_year,
            appData.current_time.tm_hour,
            appData.current_time.tm_min,
            appData.current_time.tm_sec );

}

/*******************************************************************************
 End of File
 */
