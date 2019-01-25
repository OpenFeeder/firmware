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
    /* Log event if required */
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
    /* Log event if required */
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
    /* Log event if required */
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
    /* Log event if required */
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

    appData.vbat_level = ADC1_ConversionResultGet( );
}


void printVBatLevel( void )
{
#if defined (USE_UART1_SERIAL_INTERFACE) 
    printf( "\tVBat level: %2.3f V (%u)\n", 2.0 * appData.vbat_level * VBAT_VOLTAGE_FACTOR, appData.vbat_level );
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
    printf( "\tBattery level: %2.3f V (%u)\n", appData.battery_level * BATTERY_VOLTAGE_FACTOR, appData.battery_level );
#endif 

}

void manageResetSituation( void )
{
    
    /* "dsPIC33/PIC24 Family Reference Manual", "Reset" (DS39712)
     * 7.13 REGISTERS AND STATUS BIT VALUES, page 13 */
    
#if defined (USE_UART1_SERIAL_INTERFACE) 
    printf( "\n\tWait %dms before starting the application...\n", DELAY_AFTER_POWERUP);
#endif 
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf( "\t=========================================\n" );
#endif 
    setDelayMs( DELAY_AFTER_POWERUP );
    while ( 0 == isDelayMsEnding( ) );
    
    /* Trap Event Reset */
    if ( appData.reset_1.bit_value.trapr )
    {
        store_event(OF_RESET_TRAP_EVENT);
        return;
    }
    
    /* Illegal Opcode / Uninitialized WREG */
    if ( appData.reset_1.bit_value.iopuwr )
    {
        store_event(OF_RESET_ILLEGAL_OPCODE);
        return;
    }
    
    /* MCLR */
    if ( appData.reset_1.bit_value.extr)
    {
        /* MCLR during Idle Mode */
        if ( appData.reset_1.bit_value.idle )
        {
            store_event(OF_RESET_MCLR_IDLE_MODE);
            return;
        }
        /* MCLR during Sleep Mode */
        else if ( appData.reset_1.bit_value.sleep )
        {
            store_event(OF_RESET_MCLR_SLEEP_MODE);
            return;
        }
        /* MCLR during Deep Sleep Mode */
        else if ( appData.reset_1.bit_value.dpslp )
        {
            store_event(OF_RESET_MCLR_DEEP_SLEEP_MODE);
            return;
        }
        /* MCLR during Run Mode */
        else
        {
            store_event(OF_RESET_MCLR_RUN_MODE);
            return; 
        }
    }
    
    /* SWR */
    if ( appData.reset_1.bit_value.swr )
    {
        store_event(OF_RESET_SWR);
        return;
    }
    
    /* WDTO */
    if ( appData.reset_1.bit_value.wdto )
    {
        /* WDT Time-out Reset during Idle Mode */
        if ( appData.reset_1.bit_value.idle )
        {
            store_event(OF_RESET_WDT_IDLE_MODE);
            return;
        }
        /* WDT Time-out Wake-up during Sleep Mode */
        else if ( appData.reset_1.bit_value.sleep )
        {
            store_event(OF_RESET_WDT_SLEEP_MODE);
            return;
        }
        /* WDT Time-out Reset during Run Mode */
        else
        {
            store_event(OF_RESET_WDT_RUN_MODE);
            return;
        }
    }
    
    /* Configuration Word Mismatch Reset */
    if ( appData.reset_1.bit_value.cm )
    {
        store_event(OF_RESET_CONF_WORD_MISMATCH);
        return;
    }
    
    /* Deep Sleep Mode  */
    if ( appData.reset_1.bit_value.dpslp )
    {
        store_event(OF_RESET_DEEP_SLEEP_MODE);
        return;
    }
    
    /* VBAT */
    if ( appData.reset_2.bit_value.vbat )
    {
        /* Wake from VBAT mode with Discontinuous VBAT source */
        if ( appData.reset_2.bit_value.vbpor )
        {
            store_event(OF_RESET_VBAT_DISCONT);
            return;
        }
        /* Wake from Battery Backup with Continuous VBAT */
        else
        {
            store_event(OF_RESET_VBAT_CONT);
            return;
        }
    }
    
    /* POR */
    if ( appData.reset_1.bit_value.bor && appData.reset_1.bit_value.por && appData.reset_2.bit_value.vddpor && appData.reset_2.bit_value.vddbor )
    {
        /* Power-on Reset */
        if ( appData.reset_2.bit_value.vbpor )
        {
            store_event(OF_RESET_POR);
            return;
        }
        /* Power-on Reset on VDD */
        else
        {
            store_event(OF_RESET_POR_VDD);
            return;
        }
    }
    
    /* Brown-out Reset */
    if ( appData.reset_1.bit_value.bor && appData.reset_2.bit_value.vddbor )
    {
        store_event(OF_RESET_BROWN_OUT);
        return;
    }

    /* Default */
    store_event(OF_RESET_OTHER);
    
}
    
void printResetSituation( void )
{
    /* "dsPIC33/PIC24 Family Reference Manual", "Reset" (DS39712)
     * 7.13 REGISTERS AND STATUS BIT VALUES, page 13 */
    
    /* Trap Event Reset */
    if ( appData.reset_1.bit_value.trapr )
    {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "Trap Event Reset\n");
#endif 
        return;
    }
    
    /* Illegal Opcode / Uninitialized WREG */
    if ( appData.reset_1.bit_value.iopuwr )
    {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "Illegal Opcode / Uninitialized WREG\n");
#endif 
        return;
    }
    
    /* MCLR */
    if ( appData.reset_1.bit_value.extr)
    {
        /* MCLR during Idle Mode */
        if ( appData.reset_1.bit_value.idle )
        {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "MCLR during Idle Mode\n");
#endif 
            return;
        }
        /* MCLR during Sleep Mode */
        else if ( appData.reset_1.bit_value.sleep )
        {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "MCLR during Sleep Mode\n");
#endif 
            return;
        }
        /* MCLR during Deep Sleep Mode */
        else if ( appData.reset_1.bit_value.dpslp )
        {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "MCLR during Deep Sleep Mode\n");
#endif 
            return;
        }
        /* MCLR during Run Mode */
        else
        {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "MCLR during Run Mode\n");
#endif 
            return; 
        }
    }
    
    /* SWR */
    if ( appData.reset_1.bit_value.swr )
    {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "RESET Instruction\n");
#endif 
        return;
    }
    
    /* WDTO */
    if ( appData.reset_1.bit_value.wdto )
    {
        /* WDT Time-out Reset during Idle Mode */
        if ( appData.reset_1.bit_value.idle )
        {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "WDT Time-out Reset during Idle Mode\n");
#endif 
            return;
        }
        /* WDT Time-out Wake-up during Sleep Mode */
        else if ( appData.reset_1.bit_value.sleep )
        {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "WDT Time-out Wake-up during Sleep Mode\n");
#endif 
            return;
        }
        /* WDT Time-out Reset during Run Mode */
        else
        {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "WDT Time-out Reset during Run Mode\n");
#endif 
            return;
        }
    }
    
    /* Configuration Word Mismatch Reset */
    if ( appData.reset_1.bit_value.cm )
    {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "Configuration Word Mismatch Reset\n");
#endif 
        return;
    }
    
    /* Deep Sleep Mode  */
    if ( appData.reset_1.bit_value.dpslp )
    {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "Deep Sleep Mode\n");
#endif 
        return;
    }
    
    /* VBAT */
    if ( appData.reset_2.bit_value.vbat )
    {
        /* Wake from VBAT mode with Discontinuous VBAT source */
        if ( appData.reset_2.bit_value.vbpor )
        {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "Wake from VBAT mode with Discontinuous VBAT source\n");
#endif 
            return;
        }
        /* Wake from Battery Backup with Continuous VBAT */
        else
        {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "Wake from Battery Backup with Continuous VBAT\n");
#endif 
            return;
        }
    }
    
    /* POR */
    if ( appData.reset_1.bit_value.bor && appData.reset_1.bit_value.por && appData.reset_2.bit_value.vddpor && appData.reset_2.bit_value.vddbor )
    {
        /* Power-on Reset */
        if ( appData.reset_2.bit_value.vbpor )
        {
#if defined (USE_UART1_SERIAL_INTERFACE)
            printf( "Power-on Reset\n");
#endif 
            return;
        }
        /* Power-on Reset on VDD */
        else
        {
#if defined (USE_UART1_SERIAL_INTERFACE)
            printf( "Power-on Reset on VDD \n");
#endif 
            return;
        }
    }
    
    /* Brown-out Reset */
    if ( appData.reset_1.bit_value.bor && appData.reset_2.bit_value.vddbor )
    {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "Brown-out Reset\n");
#endif 
        return;
    }

    /* Default */
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "Other Reset\n");
#endif 
}
/*******************************************************************************
End of File
 */
