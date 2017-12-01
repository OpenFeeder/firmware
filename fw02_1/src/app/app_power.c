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
    if ( false == appData.servo_powered && false == appData.pir_sensor_powered )
    {
        // Power on the voltage regulator U7 (MIC39101 - 5.0YM)
        CMD_VDD_ACC_PIR_SERVO_SetHigh( );
    }
    appData.pir_sensor_powered = true;
}


void powerPIRDisable( void )
{
    if ( false == appData.servo_powered )
    {
        // Power off the voltage regulator U7 (MIC39101 - 5.0YM)
        CMD_VDD_ACC_PIR_SERVO_SetLow( );
    }
    appData.pir_sensor_powered = false;
}


/* VDD_APP Enable function */
void powerUsbRfidEnable( void )
{
    // Power on the voltage regulator U5 (MIC39101 - 5.0YM)
    CMD_VDD_APP_V_USB_SetHigh( ); 

    CMD_VDD_USB_SetLow( ); // powered the USB connector
    CMD_VDD_USB_SetDigitalOutput( );
}


/* VDD_APP Disable function */
void powerUsbRfidDisable( void )
{
    CMD_VDD_USB_SetDigitalInput( ); // USB power down, VDD_USB = OFF
    
    // Power off the voltage regulator U5 (MIC39101 - 5.0YM)
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

/*******************************************************************************
End of File
 */
