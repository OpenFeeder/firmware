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
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf( "\tBattery level: " );
#endif
    if ( false == isPowerBatteryGood( ) )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
        printf( "pb\n" );
#endif
        return APP_CHECK_BATTERY_PB;
    }
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf( "ok\n" );
#endif

    /* Check battery level at startup. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf( "\tVBat level: " );
#endif
    if ( false == isPowerVbatGood( ) )
    {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "pb\n" );
#endif
        //        return APP_CHECK_VBAT_PB;
    }
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf( "ok\n" );
#endif

    /* Check food level. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf( "\tFood level: " );
#endif
    if ( false == isEnoughFood( ) )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
        printf( "pb\n" );
#endif
        return APP_CHECK_FOOD_LEVEL_PB;
    }
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf( "ok\n" );
#endif

    /* Check RFID frequency. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf( "\tRFID frequency: " );
#endif
    if ( false == isRfidFreqGood( ) )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
        printf( "pb\n" );
#endif
        return APP_CHECK_RFID_FREQ_PB;
    }
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf( "ok\n" );
#endif

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
    bool flag = false;
    
    if ( CMD_VCC_IR_GetValue( ) == 1 )
    {
        flag = true;
        IRSensorEnable( );
        setDelayMs( 250 ); // TODO: adjust delay according to the datasheet
        while ( false == isDelayMsEnding( ) );
    }
        
    /* Check food level */
    foodLevelOK = ( 1 == BAR_IR2_OUT_GetValue( ) );

    if ( false == foodLevelOK )
    {
        strcpy( appError.message, "Not enough food" );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        appError.number = ERROR_LOW_FOOD;
        appError.ledColor = LED_PURPLE;
    }

    if ( true == flag )
    {
        IRSensorDisable( );
    }
    
    return foodLevelOK;
}

bool isRfidFreqGood( void )
{
    bool rfidFreqOK;

    /* Check food level */
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
