/**
 * @file app_check.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 12/20/2016
 * @revision
 */

#include "app.h"
#include "app_check.h"


APP_CHECK checkImportantParameters(void)
{
    /* Check battery level at startup. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf("\n\tBattery level: ");
#endif
    if (false == isPowerBatteryGood())
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
        printf("pb\n");
#endif
        return APP_CHECK_BATTERY_PB;
    }
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf("ok\n");
#endif

    /* Check battery level at startup. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf("\tVBat level: ");
#endif
    if (false == isPowerVbatGood())
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
        printf("pb\n");
#endif
        return APP_CHECK_VBAT_PB;
    }
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf("ok\n");
#endif

    /* Check food level. */    
    if ( true == appData.chk_food_level )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf("\tFood level: ");
#endif
        if (false == isEnoughFood())
        {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
        printf("pb\n");
#endif
            return APP_CHECK_FOOD_LEVEL_PB;
        }
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf("ok\n");
#endif
    }
    
    /* Check RFID frequency. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf("\tRFID frequency: ");
#endif
    if (false == isRfidFreqGood())
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
        printf("pb\n");
#endif
        return APP_CHECK_RFID_FREQ_PB;
    }
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf("ok\n");
#endif

    return APP_CHECK_OK;
}


bool isPowerBatteryGood( void )
{
    bool battery_level_ok;
    
    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
       store_event( OF_CHECK_BATTERY ); 
    }

    getBatteryLevel( );
    battery_level_ok = appData.battery_level > LOW_BATTERY_LEVEL;

    if (false == battery_level_ok)
    {
        strcpy( appError.message, "Low battery level" );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        appError.number = ERROR_LOW_BATTERY;
    }

    return (battery_level_ok);
}


bool isPowerVbatGood(void)
{
    bool vbat_level_ok;
    
    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
       store_event( OF_CHECK_VBAT ); 
    }

    getVBatLevel( );
    vbat_level_ok = appData.vbat_level > ( LOW_VBAT_LEVEL / 2 );

    if (false == vbat_level_ok)
    {
        strcpy( appError.message, "Low vbat level" );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        appError.number = ERROR_LOW_VBAT;
    }

    return (vbat_level_ok);
}


bool isEnoughFood(void)
{
    bool food_level_ok;
    bool flag = false;
    
    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
       store_event( OF_CHECK_FOOD ); 
    }

    if ( CMD_VCC_IR_GetValue( ) == 1 )
    {
        flag = true;
        IRSensorEnable( );
        setDelayMs( DELAY_MS_BEFORE_IR_ENABLE );
        while ( false == isDelayMsEnding( ) );
    }

    /* Check food level */
    food_level_ok = ( 1 == BAR_IR2_OUT_GetValue( ) );

    if (false == food_level_ok)
    {
        strcpy( appError.message, "Not enough food" );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        appError.number = ERROR_LOW_FOOD;
    }

    if ( true == flag )
    {
        IRSensorDisable( );
    }

    return food_level_ok;
}


bool isRfidFreqGood( void )
{
    bool flag;

    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
       store_event( OF_CHECK_RFID ); 
    }

    /* Check RFID frequency */
    flag = measureRfidFreq( );

    /* RFID measure took too much time */
    if ( false == flag )
    {
        strcpy( appError.message, "Timeout during RFID frequency measure" );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        appError.number = ERROR_TIMEOUT_RFID_FREQUENCY;
        return false;
    }

    /* RFID frequency too low */
    if ( appData.rfid_rdyclk < MIN_RDYCLK_FREQ )
    {
        strcpy( appError.message, "RFID frequency too low" );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        appError.number = ERROR_LOW_RFID_FREQUENCY;
        return false;
    }

    return true;

}


/*******************************************************************************
 End of File
 */
