/**
 * @file app_check.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 */

#ifndef APP_CHECK_H
#define APP_CHECK_H

#define MAX_MEASURE_ATTEMPTS 3
#define MIN_RDYCLK_FREQ 12500 // (x10Hz)


typedef enum
{
    APP_CHECK_OK,

    APP_CHECK_BATTERY_PB,
    APP_CHECK_VBAT_PB,
    APP_CHECK_FOOD_LEVEL_PB,
    APP_CHECK_RFID_FREQ_PB

} APP_CHECK;

APP_CHECK checkImportantParameters( void );
bool isPowerBatteryGood( void );
bool isPowerVbatGood( void );
bool isEnoughFood( void );
bool isRfidFreqGood( void );

#endif /* APP_CHECK_H */


/*******************************************************************************
 End of File
 */
