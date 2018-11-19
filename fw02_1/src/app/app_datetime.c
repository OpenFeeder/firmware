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
    time_t t1, t2;
    struct tm tm1;
    struct tm tm2;
    
    if (0 == APP_I2CMasterSeeksSlaveDevice(DS3231_I2C_ADDR, DS3231_I2C_ADDR))
    {
        /* Log event if required */
        if ( true == appDataLog.log_events )
        {
           store_event(OF_DS3231_NOT_FOUND); 
        }
    }
    else 
    {
        if ( true == appDataLog.log_calibration)
        {
            getDateTime( );
            tm1 = appData.current_time;
        }
        
        if ( getExtDateTime( ) )
        {
            if ( 18 > appData.i2c_current_time.year_s ) {
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_CALIBRATE_TIME_WRONG); 
                }
            }
            else {
                
                setDateTime( appData.i2c_current_time.year_s, 
                             appData.i2c_current_time.mon, 
                             appData.i2c_current_time.mday, 
                             appData.i2c_current_time.hour, 
                             appData.i2c_current_time.min, 
                             appData.i2c_current_time.sec );
                
                if ( true == appDataLog.log_calibration)
                {
                    
                    tm1.tm_year += 100;
                    tm1.tm_mon -= 1;
                    tm2.tm_year = appData.i2c_current_time.year_s; 
                    tm2.tm_mon = appData.i2c_current_time.mon; 
                    tm2.tm_mday = appData.i2c_current_time.mday; 
                    tm2.tm_hour = appData.i2c_current_time.hour; 
                    tm2.tm_min = appData.i2c_current_time.min;
                    tm2.tm_sec = appData.i2c_current_time.sec;                  
                    tm2.tm_year += 100;
                    tm2.tm_mon -= 1;

                    t1 = mktime(&tm1);
                    t2 = mktime(&tm2);

                    appDataLog.time_calibration[appDataLog.num_time_calib_stored][0] = appData.i2c_current_time.hour;
                    appDataLog.time_calibration[appDataLog.num_time_calib_stored][1] = appData.i2c_current_time.min;
                    appDataLog.time_calibration[appDataLog.num_time_calib_stored][2] = difftime(t1,t2);
                    appDataLog.num_time_calib_stored++;
       
                }
                
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_CALIBRATE_TIME); 
                }
            }       
        }
        else
        {
            /* Log event if required */
            if ( true == appDataLog.log_events )
            {
               store_event(OF_CALIBRATE_TIME_FAIL); 
            }
        }
    }
}

void printDateTime( struct tm time )
{
    // Print date and time on serial terminal (PC)
    
        printf( "%02u/%02u/20%02u %02u:%02u:%02u",
            time.tm_mday,
            time.tm_mon,
            time.tm_year,
            time.tm_hour,
            time.tm_min,
            time.tm_sec );

}

/*******************************************************************************
 End of File
 */
