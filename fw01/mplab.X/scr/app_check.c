/**
 * @file app_check.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 12/20/2016
 * @revision
 */

#include "app.h"
#include "app_check.h"

APP_CHECK checkImportantParameters( void )
{

    /* Check battery level at startup. */
    if ( false == isPowerBatteryGood( ) )
    {
        return APP_CHECK_BATTERY_PB;
    }

    /* Check food level */
    if ( false == isEnoughFood( ) )
    {
        return APP_CHECK_FOOD_LEVEL_PB;
    }

    /* Check RFID frequency */
    if ( false == isRfidFreqGood( ) )
    {
        return APP_CHECK_RFID_FREQ_PB;
    }

    return APP_CHECK_OK;
}


bool isPowerBatteryGood( void )
{

    bool batteryLevelOK;
    getBatteryLevel( );
    batteryLevelOK = appData.battery_level > LOW_BATTERY_LEVEL;

    if ( false == batteryLevelOK )
    {
        strcpy( appError.message, "Lowbattery level" );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );

        appError.ledColor = LED_PURPLE;
    }

    return (batteryLevelOK );

}


bool isEnoughFood( void )
{
    bool foodLevelOK;

    /* Check food level */
    IRSensorEnable( );
    foodLevelOK = ( BAR_IR2_OUT_GetValue( ) == 0 );
    IRSensorDisable( );

    if ( false == foodLevelOK )
    {
        strcpy( appError.message, "Not enough food" );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        //        appError.ledColor = LED_YELLOW;
        appError.ledColor = LED_PURPLE;
    }

    return foodLevelOK;

}


bool isRfidFreqGood( void )
{
    bool rfidFreqOK;

    /* Check food level */
    measureRfidFreq( );
    measureRfidFreq( );

    rfidFreqOK = appData.rfid_rdyclk > MIN_RDYCLK_FREQ;

    if ( false == rfidFreqOK )
    {
        strcpy( appError.message, "Rfid frequency too low" );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        //        appError.ledColor = LED_YELLOW;
        appError.ledColor = LED_PURPLE;
    }

    return rfidFreqOK;

}

/*******************************************************************************
 End of File
 */
