
/**
  RTCC Generated Driver API Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    rtcc.c

  @Summary:
    This is the generated header file for the RTCC driver using MPLAB(c) Code Configurator

  @Description:
    This header file provides APIs for driver for RTCC.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 3.16
        Device            :  PIC24FJ128GB204
        Driver Version    :  0.5
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.26
        MPLAB 	          :  MPLAB X 3.20
 */

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
 Section: Included Files
 */

#include <xc.h>
#include "rtcc.h"
#include "app.h"

/**
 * Section: Function Prototype
 */
//static bool rtccTimeInitialized;
//static bool RTCCTimeInitialized( void );
static uint8_t ConvertHexToBCD( uint8_t hexvalue );
static uint8_t ConvertBCDToHex( uint8_t bcdvalue );


/**
 * Section: Driver Interface Function Definitions
 */

void RTCC_Initialize( void )
{
    // Set the RTCWREN bit
    __builtin_write_RTCWEN( );

    RCFGCALbits.RTCEN = 0;

    RCFGCALbits.RTCOE = 1; // Enable RTCC output
    // RTCOUT Seconds Clock; PWSPRE disabled; RTCLK SOSC; PWCPRE disabled; PWCEN disabled; PWCPOL disabled; 
    RTCPWC = 0x0100;

    // Enable RTCC, clear RTCWREN
    RCFGCALbits.RTCEN = 1;
    RCFGCALbits.RTCWREN = 0;

    // Enable RTCC interrupt
    IEC3bits.RTCIE = 1;
}

/**
 * This function implements RTCC_TimeReset.This function is used to
 * used by application to reset the RTCC value and reinitialize RTCC value.
 */
//void RTCC_TimeReset( bool reset )
//{
//    rtccTimeInitialized = reset;
//}

//static bool RTCCTimeInitialized( void )
//{
//    return (rtccTimeInitialized );
//}


/**
 * This function implements RTCC_TimeGet. It access the 
 * registers of  RTCC and writes to them the values provided 
 * in the function argument currentTime
 */

bool RTCC_TimeGet( struct tm *currentTime )
{
    uint16_t register_value;
    if ( RCFGCALbits.RTCSYNC )
    {
        return false;
    }

    // Set the RTCWREN bit
    __builtin_write_RTCWEN( );

    RCFGCALbits.RTCPTR = 3;
    register_value = RTCVAL;
    currentTime->tm_year = ConvertBCDToHex( register_value & 0x00FF );
    RCFGCALbits.RTCPTR = 2;
    register_value = RTCVAL;
    currentTime->tm_mon = ConvertBCDToHex( ( register_value & 0xFF00 ) >> 8 );
    currentTime->tm_mday = ConvertBCDToHex( register_value & 0x00FF );
    RCFGCALbits.RTCPTR = 1;
    register_value = RTCVAL;
    currentTime->tm_wday = ConvertBCDToHex( ( register_value & 0xFF00 ) >> 8 );
    currentTime->tm_hour = ConvertBCDToHex( register_value & 0x00FF );
    RCFGCALbits.RTCPTR = 0;
    register_value = RTCVAL;
    currentTime->tm_min = ConvertBCDToHex( ( register_value & 0xFF00 ) >> 8 );
    currentTime->tm_sec = ConvertBCDToHex( register_value & 0x00FF );

    RCFGCALbits.RTCWREN = 0;

    return true;
}


/**
 * This function sets the RTCC value and takes the input time in decimal format
 */
void RTCC_TimeSet( struct tm *initialTime )
{
    /* Set the RTCWREN bit. */
    __builtin_write_RTCWEN( );

    RCFGCALbits.RTCEN = 0;

    IFS3bits.RTCIF = false;
    IEC3bits.RTCIE = 0;

    /* Set RTCC initial time. */
    RCFGCALbits.RTCPTR = 3; // start the sequence
    RTCVAL = ConvertHexToBCD( initialTime->tm_year ); // YEAR
    RTCVAL = ( ConvertHexToBCD( initialTime->tm_mon ) << 8 ) | ConvertHexToBCD( initialTime->tm_mday ); // MONTH-1/DAY-1
    RTCVAL = ( ConvertHexToBCD( initialTime->tm_wday ) << 8 ) | ConvertHexToBCD( initialTime->tm_hour ); // WEEKDAY/HOURS
    RTCVAL = ( ConvertHexToBCD( initialTime->tm_min ) << 8 ) | ConvertHexToBCD( initialTime->tm_sec ); // MINUTES/SECONDS

    /* Enable RTCC, clear RTCWREN. */
    RCFGCALbits.RTCEN = 1;
    RCFGCALbits.RTCWREN = 0;

    IEC3bits.RTCIE = 1;
}


/**
 * This function reads the RTCC time and returns the date and time in BCD format
 */
bool RTCC_BCDTimeGet( bcdTime_t *currentTime )
{
    uint16_t register_value;
    if ( RCFGCALbits.RTCSYNC )
    {
        return false;
    }

    __builtin_write_RTCWEN( ); /* Set the RTCWREN bit. */

    RCFGCALbits.RTCPTR = 3;
    register_value = RTCVAL;
    currentTime->tm_year = register_value;
    RCFGCALbits.RTCPTR = 2;
    register_value = RTCVAL;
    currentTime->tm_mon = ( register_value & 0xFF00 ) >> 8;
    currentTime->tm_mday = register_value & 0x00FF;
    RCFGCALbits.RTCPTR = 1;
    register_value = RTCVAL;
    currentTime->tm_wday = ( register_value & 0xFF00 ) >> 8;
    currentTime->tm_hour = register_value & 0x00FF;
    RCFGCALbits.RTCPTR = 0;
    register_value = RTCVAL;
    currentTime->tm_min = ( register_value & 0xFF00 ) >> 8;
    currentTime->tm_sec = register_value & 0x00FF;
    RCFGCALbits.RTCWREN = 0;

    return true;
}


/**
 * This function takes the input date and time in BCD format and sets the RTCC
 */
void RTCC_BCDTimeSet( bcdTime_t *initialTime )
{
    // Set the RTCWREN bit
    __builtin_write_RTCWEN( );

    RCFGCALbits.RTCEN = 0;

    IFS3bits.RTCIF = false;
    IEC3bits.RTCIE = 0;

    // set RTCC initial time
    RCFGCALbits.RTCPTR = 3; // start the sequence
    RTCVAL = initialTime->tm_year; // YEAR
    RTCVAL = ( initialTime->tm_mon << 8 ) | initialTime->tm_mday; // MONTH-1/DAY-1
    RTCVAL = ( initialTime->tm_wday << 8 ) | initialTime->tm_hour; // WEEKDAY/HOURS
    RTCVAL = ( initialTime->tm_min << 8 ) | initialTime->tm_sec; // MINUTES/SECONDS

    // Enable RTCC, clear RTCWREN
    RCFGCALbits.RTCEN = 1;
    RCFGCALbits.RTCWREN = 0;

    IEC3bits.RTCIE = 1;
}


static uint8_t ConvertHexToBCD( uint8_t hexvalue )
{
    uint8_t bcdvalue;
    bcdvalue = ( hexvalue / 10 ) << 4;
    bcdvalue = bcdvalue | ( hexvalue % 10 );
    return (bcdvalue );
}


static uint8_t ConvertBCDToHex( uint8_t bcdvalue )
{
    uint8_t hexvalue;
    hexvalue = ( ( ( bcdvalue & 0xF0 ) >> 4 )* 10 ) + ( bcdvalue & 0x0F );
    return hexvalue;
}


/* Function:
  void __attribute__ ( ( interrupt, no_auto_psv ) ) _ISR _RTCCInterrupt( void )

  Summary:
    Interrupt Service Routine for the RTCC Peripheral

  Description:
    This is the interrupt service routine for the RTCC peripheral. Add in code if 
    required in the ISR. 
 */
void __attribute__( ( interrupt, no_auto_psv ) ) _ISR _RTCCInterrupt( void )
{

    if ( true == appData.flags.bit_value.systemInit )
    {

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RTCC)
        printf( "_RTCCInterrupt() " ); // display Sleeping message
#endif 

        appData.rtcc_alarm_action = RTCC_ALARM_IDLE;

        if ( appData.openfeeder_state == OPENFEEDER_IS_SLEEPING )
        {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RTCC)
            printf( "- Wakeup\n" );
#endif 
            appData.rtcc_alarm_action = RTCC_ALARM_WAKEUP_OPENFEEDER;
        }
        else
        {
            while ( !RTCC_TimeGet( &appData.current_time ) )
            {
                Nop( );
            }

            if ( appData.current_time.tm_hour >= appDataAlarmSleep.time.tm_hour &&
                 appData.current_time.tm_min >= appDataAlarmSleep.time.tm_min )
            {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RTCC)
                printf( "- Sleep\n" );
#endif 
                appData.rtcc_alarm_action = RTCC_ALARM_SLEEP_OPENFEEDER;
            }
            else
            {

                /* Attractive LEDs on/off */
                if ( ( appData.current_time.tm_hour * 60 + appData.current_time.tm_min ) >= ( appDataAttractiveLeds.wake_up_time.tm_hour * 60 + appDataAttractiveLeds.wake_up_time.tm_min ) &&
                     ( appData.current_time.tm_hour * 60 + appData.current_time.tm_min )< ( appDataAttractiveLeds.sleep_time.tm_hour * 60 + appDataAttractiveLeds.sleep_time.tm_min ) )

                {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RTCC)
                    printf( "- LEDs on\n" );
#endif 
                    appData.rtcc_alarm_action = RTCC_ALARM_SET_ATTRACTIVE_LEDS_ON;
                }
                else
                {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RTCC)
                    printf( "- LEDs off\n" );
#endif 
                    appData.rtcc_alarm_action = RTCC_ALARM_SET_ATTRACTIVE_LEDS_OFF;
                }

                /* Door open/close */
                if ( 1 == appDataDoor.remain_open )
                {

                    if ( ( appData.current_time.tm_hour * 60 + appData.current_time.tm_min ) >= ( appDataDoor.open_time.tm_hour * 60 + appDataDoor.open_time.tm_min ) &&
                         ( appData.current_time.tm_hour * 60 + appData.current_time.tm_min )< ( appDataDoor.close_time.tm_hour * 60 + appDataDoor.close_time.tm_min ) )

                    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RTCC)
                        printf( "- Door open\n" );
#endif 
                        appData.rtcc_alarm_action = RTCC_ALARM_OPEN_DOOR;
                    }
                    else
                    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RTCC)
                        printf( "- Door close\n" );
#endif 
                        appData.rtcc_alarm_action = RTCC_ALARM_CLOSE_DOOR;
                    }
                }

            }
        }
    }

    IFS3bits.RTCIF = false; /* Clear interrupt flag. */
}


/**
End of File
 */
