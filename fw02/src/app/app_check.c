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
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "APP_CHECK_BATTERY_PB\n" );
#endif
        return APP_CHECK_BATTERY_PB;
    }

    /* Check battery level at startup. */
    if ( false == isPowerVbatGood( ) )
    {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "APP_CHECK_VBAT_PB\n" );
#endif
        //        return APP_CHECK_VBAT_PB;
    }

    /* Check food level */
    if ( false == isEnoughFood( ) )
    {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "APP_CHECK_FOOD_LEVEL_PB\n" );
#endif
        return APP_CHECK_FOOD_LEVEL_PB;
    }

    /* Check RFID frequency */
    if ( false == isRfidFreqGood( ) )
    {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "APP_CHECK_RFID_FREQ_PB\n" );
#endif
        //        return APP_CHECK_RFID_FREQ_PB;
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
        strcpy( appError.message, "Low battery level" );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );

        appError.number = ERROR_LOW_BATTERY;
        appError.ledColor = LED_PURPLE;
    }

    return (batteryLevelOK );
}

bool isPowerVbatGood( void )
{
    bool vbatLevelOK;
    getVBatLevel( );
    vbatLevelOK = appData.vbat_level > LOW_VBAT_LEVEL;

    if ( false == vbatLevelOK )
    {
        strcpy( appError.message, "Low vbat level" );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        appError.number = ERROR_LOW_VBAT;
        appError.ledColor = LED_PURPLE;
    }

    return (vbatLevelOK );
}

bool isEnoughFood( void )
{
    bool foodLevelOK;

    /* Check food level */
    IRSensorEnable( );
    setDelayMs( 400 );
    while ( false == isDelayMsEnding( ) );

    foodLevelOK = ( 1 == BAR_IR2_OUT_GetValue( ) );
    IRSensorDisable( );

    if ( false == foodLevelOK )
    {
        strcpy( appError.message, "Not enough food" );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        appError.number = ERROR_LOW_FOOD;
        appError.ledColor = LED_PURPLE;
    }

    return foodLevelOK;
}

// E: RFID frequency too low: 13410 (x10Hz))

bool isRfidFreqGood( void )
{
    bool rfidFreqOK;

    /* Check food level */
    measureRfidFreq( );
    setDelayMs( 400 );
    while ( false == isDelayMsEnding( ) );
    measureRfidFreq( );

    rfidFreqOK = appData.rfid_rdyclk > MIN_RDYCLK_FREQ;

    if ( false == rfidFreqOK )
    {
        strcpy( appError.message, "RFID frequency too low" );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        appError.number = ERROR_LOW_RFID_FREQUENCY;
        appError.ledColor = LED_PURPLE;
    }

    return rfidFreqOK;
}


/*******************************************************************************
 End of File
 */
