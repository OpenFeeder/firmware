/**
 * @file app_power.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 * @revision history 1
 * @dependency 
 */

#include "app.h"
#include "app_power.h"


bool isPowerServoEnable( void )
{
    return appDataServo.cmd_vcc_servo_state;
}

void powerPIREnable( void )
{
    CMD_VDD_ACC_PIR_SERVO_SetHigh( ); // CMD_ACC_PIR = EN pin of U7 (5V regulator for PIR sensor and accesoir "VDD_ACC": servomotor with "VDD_SERVO", )
}


void powerPIRDisable( void )
{
    CMD_VDD_ACC_PIR_SERVO_SetLow( );
}


void powerUsbRfidEnable( void )
{
    CMD_VDD_APP_V_USB_SetHigh( );
}


void powerUsbRfidDisable( void )
{
    CMD_VDD_APP_V_USB_SetLow( );
}


uint16_t getADC1value( ADC1_CHANNEL channel )
{
    uint16_t i;

    ADC1_ChannelSelect( channel );

    ADC1_Start( );
    for ( i = 0; i < 1000; i++ ); /* Provide Delay */
    ADC1_Stop( );

    while ( !ADC1_IsConversionComplete( ) )
    {
        ADC1_Tasks( );
    }

    return ADC1_ConversionResultGet( );
}

// FIXME : Can be replace by getADC1value()


void getVBatLevel( void )
{
    uint16_t i;

    ADC1_ChannelSelect( ADC1_CHANNEL_VBAT_2 );

    ADC1_Start( );
    for ( i = 0; i < 1000; i++ ); /* Provide Delay */
    ADC1_Stop( );

    while ( !ADC1_IsConversionComplete( ) )
    {
        ADC1_Tasks( );
    }

    appData.vbat_level = ADC1_ConversionResultGet( ) * 2;
}


void printVBatLevel( void )
{
#if defined (USE_UART1_SERIAL_INTERFACE) 
    printf( "VBat level: %2.3f V (%u)\n", appData.vbat_level * VBAT_VOLTAGE_FACTOR, appData.vbat_level );
#endif 
}

// FIXME : Can be replace by getADC1value()


void getBatteryLevel( void )
{
    uint16_t i;

    ADC1_ChannelSelect( ADC1_CHANNEL_MA_12V );

    ADC1_Start( );
    for ( i = 0; i < 1000; i++ ); /* Provide Delay */
    ADC1_Stop( );

    while ( !ADC1_IsConversionComplete( ) )
    {
        ADC1_Tasks( );
    }

    appData.battery_level = ADC1_ConversionResultGet( );
}


void printBatteryLevel( void )
{
#if defined (USE_UART1_SERIAL_INTERFACE) 
    printf( "Battery level: %2.3f V (%u)\n", appData.battery_level * BATTERY_VOLTAGE_FACTOR, appData.battery_level );
#endif 

}


FILEIO_RESULT logBatteryLevel( void )
{
    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    char buf[100];
    struct tm currentTime;
    int flag, i;
    size_t numDataWritten;

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf( "Write battery level to file - " );
#endif

    getBatteryLevel( );
    getDateTime( &currentTime );

    if ( USB_DRIVE_NOT_MOUNTED == usbMountDrive( ) )
    {
        return FILEIO_RESULT_FAILURE;
    }

    if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, "BATTERY.TXT", FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_APPEND ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
        printf( "unable to open battery log file (%u)", errF );
#endif 
        sprintf( appError.message, "Unable to open battery log file (%u)", errF );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_BATTERY_FILE_OPEN;
        return FILEIO_RESULT_FAILURE;
    }

    flag = sprintf( buf, "%02d/%02d/%02d"
                    /* separator        */ "%s"
                    /* site id          */ "%s"
                    /* separator        */ "%s"
                    /* scenario         */ "%u",
                    currentTime.tm_mday,
                    currentTime.tm_mon,
                    currentTime.tm_year,
                    appDataLog.separator,
                    appData.siteid,
                    appDataLog.separator,
                    appData.scenario_number );

    if ( flag > 0 )
    {
        numDataWritten = FILEIO_Write( buf, 1, flag, &file );
    }

    if ( numDataWritten < flag )
    {
        errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
        printf( "unable to write battery level in log file (%u)", errF );
#endif 
        sprintf( appError.message, "Unable to write battery level in log file (%u)", errF );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_BATTERY_FILE_WRITE;
        return FILEIO_RESULT_FAILURE;
    }

    memset( buf, '\0', sizeof ( buf ) );

    for ( i = 0; i < 24; i++ )
    {
        flag = sprintf( buf, "%s%d%s%d",
                        appDataLog.separator,
                        appDataLog.battery_level[i][0],
                        appDataLog.separator,
                        appDataLog.battery_level[i][1] );

        if ( flag > 0 )
        {
            numDataWritten = FILEIO_Write( buf, 1, flag, &file );
        }

        if ( numDataWritten < flag )
        {
            errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
            printf( "unable to write battery level in log file (%u)", errF );
#endif 
            sprintf( appError.message, "Unable to write battery level in log file (%u)", errF );
            appError.currentLineNumber = __LINE__;
            sprintf( appError.currentFileName, "%s", __FILE__ );
            FILEIO_ErrorClear( 'A' );
            appError.number = ERROR_BATTERY_FILE_WRITE;
            return FILEIO_RESULT_FAILURE;
        }
    }

    if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
        printf( "unable to close battery log file (%u)", errF );
#endif 
        sprintf( appError.message, "Unable to close battery log file (%u)", errF );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_BATTERY_FILE_CLOSE;
        return FILEIO_RESULT_FAILURE;
    }

    if ( USB_DRIVE_MOUNTED == usbUnmountDrive( ) )
    {
        return FILEIO_RESULT_FAILURE;
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf( "success\n" );
#endif 

    return FILEIO_RESULT_SUCCESS;
}

/*******************************************************************************
End of File
 */
