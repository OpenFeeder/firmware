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
    if ( true == appDataLog.log_events )
    {
       store_event(OF_POWER_PIR_ON); 
    }
    
    if ( false == appData.servo_powered && false == appData.pir_sensor_powered )
    {
        // Power on the voltage regulator U7 (MIC39101 - 5.0YM)
        CMD_VDD_ACC_PIR_SERVO_SetHigh( );
    }
    appData.pir_sensor_powered = true;
}


void powerPIRDisable( void )
{
    if ( true == appDataLog.log_events )
    {
       store_event(OF_POWER_PIR_OFF); 
    }

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
    if ( true == appDataLog.log_events )
    {
       store_event(OF_POWER_USB_RFID_ON); 
    }

    // Power on the voltage regulator U5 (MIC39101 - 5.0YM)
    CMD_VDD_APP_V_USB_SetHigh( ); 

    CMD_VDD_USB_SetLow( ); // powered the USB connector
    CMD_VDD_USB_SetDigitalOutput( );
}


/* VDD_APP Disable function */
void powerUsbRfidDisable( void )
{
    if ( true == appDataLog.log_events )
    {
       store_event(OF_POWER_USB_RFID_OFF); 
    }

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

void manageResetSituation( void )
{
    switch ( appData.reset_status_num)
    {
        case 1024: // Software reset
            appData.dsgpr0.reg = DSGPR0;
            store_event(OF_RESET_SWR);
            break;
            
        case 2048:
            store_event(OF_RESET_MCLR_RUN_MODE);
            break;
            
        case 2101:
            store_event(OF_RESET_POR);
//#if defined (USE_UART1_SERIAL_INTERFACE) 
//    printf( "\tWait %dms before stating application", DELAY_AFTER_POWERUP);
//#endif 
            setDelayMs( DELAY_AFTER_POWERUP );
            while ( 0 == isDelayMsEnding( ) ); 
            break;
            
        case 2107:
            store_event(OF_RESET_VBAT);
//#if defined (USE_UART1_SERIAL_INTERFACE) 
//    printf( "\tWait %dms before stating application", DELAY_AFTER_POWERUP);
//#endif 
            setDelayMs( DELAY_AFTER_POWERUP );
            while ( 0 == isDelayMsEnding( ) ); 
            break;
            
        case 2176:
            store_event(OF_RESET_MCLR_IDLE_MODE);
            break;
            
        case 2304:
            store_event(OF_RESET_MCLR_SLEEP_MODE);
            break;
            
        default:
            store_event(OF_RESET_OTHER);
            break;
    }
    
}
    
void printResetSituation( void )
{
    
//       printf("\n%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u\n", 
//           appData.reset_1.bit_value.trapr,
//           appData.reset_1.bit_value.iopuwr,
//           appData.reset_1.bit_value.dpslp,
//           appData.reset_1.bit_value.extr,
//           appData.reset_1.bit_value.swr,
//           appData.reset_1.bit_value.wdto,
//           appData.reset_1.bit_value.sleep,
//           appData.reset_1.bit_value.idle,
//           appData.reset_1.bit_value.cm, 
//           appData.reset_1.bit_value.bor,
//           appData.reset_1.bit_value.por,
//           appData.reset_2.bit_value.vbat,
//           appData.reset_2.bit_value.vbpor,
//           appData.reset_2.bit_value.vddpor,
//           appData.reset_2.bit_value.vddbor);
       
//    printf("\nreset_status_num: %u\n", appData.reset_status_num);
    
    switch ( appData.reset_status_num)
    {
        case 1024:
            printf( "\tSWR reset\n");
            break;
            
        case 2048:
            printf( "\tMCLR during Run Mode\n");
            break;
            
        case 2101:
            printf( "\tPOR reset\n");
            break;
            
        case 2107:
            printf( "\tVBAT reset\n");
            break;
            
        case 2176:
            printf( "\tMCLR during Idle Mode\n");
            break;
            
        case 2304:
            printf( "\tMCLR during Sleep Mode\n");
            break;
            
        default:
            printf("\tReset code not listed: %u\n", appData.reset_status_num);
    }
    
}
/*******************************************************************************
End of File
 */
