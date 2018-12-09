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

    if ( day < 1 || day > 31 || 
        month < 1 || month > 12 ||
        year < MIN_ADMISSIBLE_YEAR || year > MAX_ADMISSIBLE_YEAR ||
        hour < 0 || hour > 23 ||
        minute < 0 || minute > 59 ||
        second < 0 || second > 59)
    {
        /* Log event if required */
        if ( true == appDataLog.log_events )
        {
           store_event(OF_SET_DATE_TIME_FAIL); 
        }
        return false;
    }
    
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
    
    if ( appData.current_time.tm_mday < 1 || appData.current_time.tm_mday > 31 || 
        appData.current_time.tm_mon < 1 || appData.current_time.tm_mon > 12 ||
        appData.current_time.tm_year < MIN_ADMISSIBLE_YEAR || appData.current_time.tm_year > MAX_ADMISSIBLE_YEAR ||
        appData.current_time.tm_hour < 0 || appData.current_time.tm_hour > 23 ||
        appData.current_time.tm_min < 0 || appData.current_time.tm_min > 59 ||
        appData.current_time.tm_sec < 0 || appData.current_time.tm_sec > 59)
    {
        return false;
    }
    else
    {
        return true;
    }
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
            if ( appData.i2c_current_time.mday < 1 || appData.i2c_current_time.mday > 31 || 
                appData.i2c_current_time.mon < 1 || appData.i2c_current_time.mon > 12 ||
                appData.i2c_current_time.year_s < MIN_ADMISSIBLE_YEAR || appData.i2c_current_time.year_s > MAX_ADMISSIBLE_YEAR ||
                appData.i2c_current_time.hour < 0 || appData.i2c_current_time.hour > 23 ||
                appData.i2c_current_time.min < 0 || appData.i2c_current_time.min > 59 ||
                appData.i2c_current_time.sec < 0 || appData.i2c_current_time.sec > 59)
            {
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_CALIBRATE_TIME_WRONG); 
                }
            }
            else 
            {
                
                if ( true == setDateTime( appData.i2c_current_time.year_s, 
                             appData.i2c_current_time.mon, 
                             appData.i2c_current_time.mday, 
                             appData.i2c_current_time.hour, 
                             appData.i2c_current_time.min, 
                             appData.i2c_current_time.sec ) )
                {
                
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

                        if ( appDataLog.num_time_calib_stored < NUM_TIME_CALIBRATION_TO_LOG )
                        {
                            appDataLog.time_calibration[appDataLog.num_time_calib_stored][0] = appData.i2c_current_time.hour;
                            appDataLog.time_calibration[appDataLog.num_time_calib_stored][1] = appData.i2c_current_time.min;
                            appDataLog.time_calibration[appDataLog.num_time_calib_stored][2] = difftime(t1,t2);
                            appDataLog.num_time_calib_stored++;
                        }
                        else
                        {
                            /* Log event if required */
                            if ( true == appDataLog.log_events )
                            {
                               store_event(OF_CALIBRATE_TIME_OVERFLOW); 
                            }
                        }

                    }

                    /* Log event if required */
                    if ( true == appDataLog.log_events )
                    {
                       store_event(OF_CALIBRATE_TIME); 
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
    
        printf( "%02u/%02u/%04u %02u:%02u:%02u",
            time.tm_mday,
            time.tm_mon,
            2000 + time.tm_year,
            time.tm_hour,
            time.tm_min,
            time.tm_sec );

}

/*******************************************************************************
 End of File
 */
