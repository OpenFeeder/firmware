/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */

/**
 * @brief Development of a test program with PIC24FJ256GB406 MCU on OpenFeeder prototype board.
 * @see [OpenFeeder Board v2_0](https://docs.google.com/document/d/1EUuRprU-K5NVF4FKyMJyoSdlVwXycWjznXlICHy2UsM/edit?usp=sharing)
 * Author: OpenFeeder Team <https://github.com/orgs/OpenFeeder/people> 
 * @version fw02
 * @revision 
 * @date 13/02/2017
 */


#include "mcc.h"
#include "app.h"

/*
                         Main application
 */

int main( void )
{
    I2C1_MESSAGE_STATUS i2c_status;

    int i;
    
    appData.reset_1.reg = RCON;
    appData.reset_2.reg = RCON2;
 
#if defined (ENABLE_DEEP_SLEEP)   
    if ( RCONbits.DPSLP )
    {
        /* This means that keeping the SOSC running after waking up requires 
         * the SOSCEN bit to be set before clearing RELEASE. 
         PIC24FJ256GB406 datasheet page 202 */
        OSCCONbits.SOSCEN = 1;

        appData.dsgpr0.reg = DSGPR0;
        appData.dsgpr1.reg = DSGPR1;
        
        DSCONbits.RELEASE = 0;
        DSCONbits.RELEASE = 0;

        RCONbits.DPSLP = 0;
 
    }
#endif    
    
    /*Clear the RESET CONTROL registers */
    RCON = 0;
    RCON2 = 0;
    
    appData.dsgpr0.reg = 0;
    appData.dsgpr1.reg = 0;
        
    clear_events_buffers( );
    
    /* Initialize the device. */
    SYSTEM_Initialize( );

    getDeviceId();
    getUniqueDeviceId();

    /* "dsPIC33/PIC24 Family Reference Manual", "Reset" (DS39712)
     * 7.13 REGISTERS AND STATUS BIT VALUES, page 13 */
    appData.reset_status_num = appData.reset_2.bit_value.vddbor +
        (appData.reset_2.bit_value.vddpor<<1) +
        (appData.reset_2.bit_value.vbpor<<2) +
        (appData.reset_2.bit_value.vbat<<3) +
        (appData.reset_1.bit_value.por<<4) +
        (appData.reset_1.bit_value.bor<<5) +
        (appData.reset_1.bit_value.cm<<6) +        
        (appData.reset_1.bit_value.idle<<7) +
        (appData.reset_1.bit_value.sleep<<8) +
        (appData.reset_1.bit_value.wdto<<9) +
        (appData.reset_1.bit_value.swr<<10) +
        (appData.reset_1.bit_value.extr<<11) +
        (appData.reset_1.bit_value.dpslp<<12) +
        (appData.reset_1.bit_value.iopuwr<<13) +
        (appData.reset_1.bit_value.trapr<<14);

    manageResetSituation( );
    
    /* Initialize peripheral driver. */
    RFID_Initialize( );
    SERVO_Initialize( );

    /* Initialize the file IO system. */
    FILEIO_Initialize( );
    FILEIO_RegisterTimestampGet( GetTimestamp );

    /* Initialize the USB Host API. */
    if ( !USBHostInit( 0 ) )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
        printf( "\nFailure to initialize USB Host API!\n" );
#endif
        LedsStatusBlink( LEDS_ERROR, LEDS_OFF, 83, 250 );
    }

    /* Initialize the application. */
    APP_Initialize( );
      
    /* If the user button is not pressed => enter normal mode */
    if ( BUTTON_NOT_PRESSED == USER_BUTTON_GetValue( ) )
    {
        
        store_event(OF_ENTER_NORMAL_MODE);
        
#if defined (USE_UART1_SERIAL_INTERFACE)
        /* Display boot message */
        displayBootMessage( );
#endif

        /* Blink status LEDs if reset ??? */
        /* TODO */
        if ( 2101 == appData.reset_status_num || 2107 == appData.reset_status_num )
        {          
            setLedsStatusColor( LEDS_ON );
            setDelayMs( 5000 );
            while ( 0 == isDelayMsEnding( ) ); 
            setLedsStatusColor( LEDS_OFF );
        }

        /* Main loop. */
        while ( 1 )
        {            
            /* Maintain Device Drivers. */
            USBTasks( );

            /* Maintain the application's state machine. */
            APP_Tasks( ); /* application specific tasks */  
        }
    }
    /* If the user button is pressed => enter debug mode */
    else
    {
        
        store_event(OF_ENTER_DEBUG_MODE);
        
        printf( "\n\n/!\\ ...DEBUG MODE... /!\\\n\n" );

 #if defined (USE_UART1_SERIAL_INTERFACE)
        /* Display boot message */
        displayBootMessage( );
#endif
        printf( "\n\n/!\\ ...DEBUG MODE... /!\\\n\n" );
        
        /* Status LED blinks */
        TMR3_Start( );                
        for (i=0;i<3;i++)
        {
            setLedsStatusColor( LEDS_ON );
            setDelayMs( 500 );
            while ( 0 == isDelayMsEnding( ) ); 
            setLedsStatusColor( LEDS_OFF );
            setDelayMs( 500 );
            while ( 0 == isDelayMsEnding( ) ); 
        }
        TMR3_Stop( );

        OC4_Stop( );
        OC5_Stop( );
        TMR4_Stop( );
        TMR2_Stop( );

        i2c_status = initAttractiveLeds( );

        if ( i2c_status )
        {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
            printf( "\tAttractive LEDs: ok\n" );
#endif
        }
        else
        {
            sprintf( appError.message, "Unable to initialize attractive LEDS via I2C" );
            appError.current_line_number = __LINE__;
            sprintf( appError.current_file_name, "%s", __FILE__ );
            appError.number = ERROR_ATTRACTIVE_LED_INIT;
            appData.state = APP_STATE_ERROR;
        }
        
        TMR3_Start( ); 
        printf( "\tTimer 3 started\n" );
        printf( "\tReady\n" );
        
        while ( 1 )
        {
            /* Maintain Device Drivers. */
            USBTasks( );
            
            /* Maintain the Serial Debug Tasks state machine. */
            APP_SerialDebugTasks( );
        }
    }

    /* Execution should not come here during normal operation. */
    setLedsStatusColor( LED_RED );

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
 */
