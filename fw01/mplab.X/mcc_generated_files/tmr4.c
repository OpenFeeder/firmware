
/**
  TMR4 Generated Driver API Source File 

  @Company
    Microchip Technology Inc.

  @File Name
    tmr4.c

  @Summary
    This is the generated source file for the TMR4 driver using MPLAB(c) Code Configurator

  @Description
    This source file provides APIs for driver for TMR4. 
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
#include <stdbool.h>
#include "tmr4.h"
//#include "em4095.h"
#include "app.h"

/**
  Section: Data Type Definitions
 */

/** TMR Driver Hardware Instance Object

  @Summary
    Defines the object required for the maintainence of the hardware instance.

  @Description
    This defines the object required for the maintainence of the hardware
    instance. This object exists once per hardware instance of the peripheral.

  Remarks:
    None.
 */

typedef struct _TMR_OBJ_STRUCT
{
    /* Timer Elapsed */
    bool timerElapsed;
    /*Software Counter value*/
    uint8_t count;

} TMR_OBJ;

static TMR_OBJ tmr4_obj;

/**
  Section: Driver Interface
 */


void TMR4_Initialize( void )
{
    //TMR4 0; 
    TMR4 = 0x0000;
    //Period = 0.000026125 s; Frequency = 8000000 Hz; PR4 209; 
    PR4 = 0x00D1;
    //TCKPS 1:1; T32 16 Bit; TON enabled; TSIDL disabled; TCS FOSC/2; TECS SOSC; TGATE disabled; 
    T4CON = 0x8000;


    IFS1bits.T4IF = false;
    IEC1bits.T4IE = true;

    tmr4_obj.timerElapsed = false;

}

void __attribute__( ( interrupt, no_auto_psv ) ) _T4Interrupt( )
{
    /* Check if the Timer Interrupt/Status is set */

    //***User Area Begin
    static volatile uint16_t CountCallBack = 0;
    //static volatile uint16_t CountCallBack_160ms = 0;

    // callback function - called every 15th pass 
    //    if ( ++CountCallBack >= TMR4_INTERRUPT_TICKER_FACTOR )
    // with 15 next interrupt occur at 442 us
    // old : with 17 next interrupt occur at 495 us
    // counter_delay_read_bit
    if ( g_counter_delay_read_bit++ > DECODING_RFID_INTERRUPT_TICKER_FACTOR )
    {
        // EM4095 DEMOD_OUT interrupt on timer even every 400 us :
#if defined (DEBUG_RFID_WORKING_ON_LED_STATUS)
        LED_STATUS_G_SetHigh( );
#endif
        g_datastream_read_bit = EM4095_DEMOD_OUT_GetValue( );
        ++g_counter_delay_overflow; // increment every 440 us

        // reset ticker counter
        g_counter_delay_read_bit = 0;
    }

    // callback function - called every 6124th pass
    //if ( ++CountCallBack_160ms >= 6124 )
    if ( ++CountCallBack >= TMR4_INTERRUPT_TICKER_FACTOR )
    {
        // ticker function call
        //        TMR4_CallBack( );
        if ( g_rfid_activate )
        {
            g_timeout_reading_pit_tag = (g_timeout_reading_pit_tag != 0) ? g_timeout_reading_pit_tag-1 : 0;
            EM4095_SHD_Toggle( ); /* EM4095 active when SHD is low state, EM4095 enable. */
        }
        else
        {
           // EM4095_SHD_SetHigh( ); /* !!! for disable EM4095 set LOW. */
            EM4095_SHD_DISABLE();
        }

        // reset ticker counter
        CountCallBack = 0;
        //CountCallBack_160ms = 0;
    }

    //***User Area End

    tmr4_obj.count++;
    tmr4_obj.timerElapsed = true;
    IFS1bits.T4IF = false;
}

void TMR4_Period16BitSet( uint16_t value )
{
    /* Update the counter values */
    PR4 = value;
    /* Reset the status information */
    tmr4_obj.timerElapsed = false;
}

uint16_t TMR4_Period16BitGet( void )
{
    return ( PR4 );
}

void TMR4_Counter16BitSet( uint16_t value )
{
    /* Update the counter values */
    TMR4 = value;
    /* Reset the status information */
    tmr4_obj.timerElapsed = false;
}

uint16_t TMR4_Counter16BitGet( void )
{
    return ( TMR4 );
}

void __attribute__( ( weak ) ) TMR4_CallBack( void )
{
    // Add your custom callback code here
}

void TMR4_Start( void )
{
    /* Reset the status information */
    tmr4_obj.timerElapsed = false;

    /*Enable the interrupt*/
    IEC1bits.T4IE = true;

    /* Start the Timer */
    T4CONbits.TON = 1;
}

void TMR4_Stop( void )
{
    /* Stop the Timer */
    T4CONbits.TON = false;

    /*Disable the interrupt*/
    IEC1bits.T4IE = false;
}

bool TMR4_GetElapsedThenClear( void )
{
    bool status;

    status = tmr4_obj.timerElapsed;

    if ( status == true )
    {
        tmr4_obj.timerElapsed = false;
    }
    return status;
}

int TMR4_SoftwareCounterGet( void )
{
    return tmr4_obj.count;
}

void TMR4_SoftwareCounterClear( void )
{
    tmr4_obj.count = 0;
}

/**
 End of File
 */