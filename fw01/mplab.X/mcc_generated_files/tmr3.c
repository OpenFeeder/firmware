
/**
  TMR3 Generated Driver API Source File 

  @Company
    Microchip Technology Inc.

  @File Name
    tmr3.c

  @Summary
    This is the generated source file for the TMR3 driver using MPLAB(c) Code Configurator

  @Description
    This source file provides APIs for driver for TMR3. 
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
#include "tmr3.h"
#include "app.h"
//#include "ext_int.h"
//#include "servomotor.h"

/**
  Section: Data Type Definitions
 */
// FIXME: add to appData ?
volatile uint16_t rdyclk_count_in_10ms; /* variable to display the number of positive edge counter on RDY/CLK in (10 ms ! --> 20 ms) */
volatile bool g_new_value_of_em4095_rdyclk_measurement; // new value of a frequency measurement RDY/CLK of the EM4095 signal available
volatile uint16_t g_timeout_x20ms;
volatile uint16_t g_timeout_leds_status_x20ms;
//volatile uint8_t g_timeout_x100ms;
volatile uint8_t g_timeout_taking_reward;

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

static TMR_OBJ tmr3_obj;

/**
  Section: Driver Interface
 */

void TMR3_Initialize( void )
{
    //TMR3 0; 
    TMR3 = 0x0000;
    //    //Period = 0.01 s; Frequency = 8000000 Hz; PR3 10000; 
    //    PR3 = 0x2710;
    //Period = 0.02 s; Frequency = 8000000 Hz; PR3 20000; 
    PR3 = 0x4E20;
    //TCKPS 1:8; TON enabled; TSIDL disabled; TCS FOSC/2; TECS SOSC; TGATE disabled; 
    T3CON = 0x8010;


    IFS0bits.T3IF = false;
    IEC0bits.T3IE = true;

    tmr3_obj.timerElapsed = false;
}

void __attribute__( ( interrupt, no_auto_psv ) ) _T3Interrupt( )
{
    /* Check if the Timer Interrupt/Status is set */

    //***User Area Begin
    static uint8_t counter_100ms = 0; // variable for capture the number of positive edge counter in 10 ms
    static uint8_t count_call_scanning_push_button = 0; // variable for ScanningPushButton
    static uint16_t capture_positive_edge_counter_in_10ms = 0; // variable for capture the number of positive edge counter in 10 ms
    static uint16_t previous_positive_edge_counter_in_10ms = 0; // and this to check if there is a new value

    capture_positive_edge_counter_in_10ms = counter_positive_edge_rdyclk; // MikroElektronika "RFid Reader Board": 0x0483 <=> 1155 (dec) --> 115,5 kHz
    counter_positive_edge_rdyclk = 0; // reset RDY/CLK pulse counter

    /* Check if we can update new value. */
    if ( g_new_value_of_em4095_rdyclk_measurement == false )
    {
        /* and if there is a new value to display */
        if ( capture_positive_edge_counter_in_10ms != previous_positive_edge_counter_in_10ms )
        {
            previous_positive_edge_counter_in_10ms = capture_positive_edge_counter_in_10ms;
            rdyclk_count_in_10ms = capture_positive_edge_counter_in_10ms;
            g_new_value_of_em4095_rdyclk_measurement = true;
        }
    }

    switch ( appData.reward_door_status )
    {
        case DOOR_OPENING:
            /* Opening in action. */
            //if ( servomotorOpenTheDoor( &appDataServo ) )
            if ( servomotorOpenTheDoor( ) )
            {
                appData.reward_door_status = DOOR_IDLE;
            }
            break;

        case DOOR_CLOSING:
            /* Closing in action. */
            if ( servomotorCloseTheDoor( ) )
            {
                appData.reward_door_status = DOOR_IDLE;
            }
            break;

        case DOOR_IDLE:
        default:
            /* if nothing else matches, do the default. */
            break;
    }

    /* This is for delay routine in app.c file
     * if timeout x20ms counter == 0 do nothing.
     */
    if ( g_timeout_x20ms )
    {
        --g_timeout_x20ms;
    }
    
    if ( g_timeout_leds_status_x20ms )
    {
        --g_timeout_leds_status_x20ms;
    }

    /* Timeout x100 ms here. */
    if ( ++counter_100ms == 5 )
    {
        counter_100ms = 0; /* reset counter_100ms */
        if ( g_timeout_taking_reward != 0 )
        {
            --g_timeout_taking_reward;
        }
    }

    /* 200 ms here */
    if ( ++count_call_scanning_push_button == 10 )
    {
        /* Call Scanning Push Button, either every 200 ms. */
        count_call_scanning_push_button = 0; /* reset ticker counter */

        if ( appData.flags.bit_value.RemoteControlConnected )
        {
            APP_ScanningPushButtonTasks( ); /* calling task for reading button */
            blinkDigitState = !blinkDigitState; /* change display state for blinking digit */
        }
    }

    // ticker function call;
    // ticker is 1 -> Callback function gets called everytime this ISR executes
    //TMR3_CallBack( );

    //***User Area End

    tmr3_obj.count++;
    tmr3_obj.timerElapsed = true;
    IFS0bits.T3IF = false;
}

void TMR3_Period16BitSet( uint16_t value )
{
    /* Update the counter values */
    PR3 = value;
    /* Reset the status information */
    tmr3_obj.timerElapsed = false;
}

uint16_t TMR3_Period16BitGet( void )
{
    return ( PR3 );
}

void TMR3_Counter16BitSet( uint16_t value )
{
    /* Update the counter values */
    TMR3 = value;
    /* Reset the status information */
    tmr3_obj.timerElapsed = false;
}

uint16_t TMR3_Counter16BitGet( void )
{
    return ( TMR3 );
}

//void __attribute__( ( weak ) ) TMR3_CallBack( void )
//{
//    // Add your custom callback code here
//}

void TMR3_Start( void )
{
    /* Reset the status information */
    tmr3_obj.timerElapsed = false;

    /* Enable the interrupt */
    IFS0bits.T3IF = false;
    IEC0bits.T3IE = true;

    /* Start the Timer */
    T3CONbits.TON = 1;
}

void TMR3_Stop( void )
{
    /* Stop the Timer */
    T3CONbits.TON = false;

    /*Disable the interrupt*/
    IEC0bits.T3IE = false;
}

bool TMR3_GetElapsedThenClear( void )
{
    bool status;

    status = tmr3_obj.timerElapsed;

    if ( status == true )
    {
        tmr3_obj.timerElapsed = false;
    }
    return status;
}

int TMR3_SoftwareCounterGet( void )
{
    return tmr3_obj.count;
}

void TMR3_SoftwareCounterClear( void )
{
    tmr3_obj.count = 0;
}

/**
 End of File
 */
