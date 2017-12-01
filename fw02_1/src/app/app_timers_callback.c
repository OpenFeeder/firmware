/**
 * @file app_timers_callback.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 12/19/2016
 * @revision
 */

#include "app.h"
#include "app_timers_callback.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

volatile uint16_t g_rdyclk_count_in_10ms; /* variable to display the number of positive edge counter on RDY/CLK in (10 ms ! --> 20 ms) */ // FIXME: add to appData ?
volatile uint16_t g_timeout_x20ms;
volatile uint16_t g_timeout_standby_x20ms;
volatile uint16_t g_timeout_em4095_x20ms;
volatile uint16_t g_timeout_leds_status_x20ms;
volatile uint8_t g_timeout_taking_reward;


//******************************************************************************
//******************************************************************************
//  Function Implementation
//******************************************************************************
//******************************************************************************

void TMR2_CallBack( void )
{
    static volatile uint8_t CountCmdMultiplex = 0;

    /* Multiplexing LEDs, either every 3 ms, measuring = 3.744 ms */
    // ticker is 0.003/0.000031875= 94 -> Callback function gets called everytime this ISR executes
    if ( appData.flags.bit_value.RemoteControlConnected )
    {
        //        LED_STATUS_B_Toggle( ); // FIXME: Debug display
        //        if ( appData.mcp23017.status_bit.found )
        //        {
        if ( ++CountCmdMultiplex > 94 )
        {
            CountCmdMultiplex = 0; /* reset ticker counter */
            if ( APP_isRemoteControlConnected( ) )
            {
                APP_MultiplexingLEDsTasks( ); /* Multiplexing LEDs on I2C Control Device board. (duration: 474 us) */
            }
            else
            {
                appData.flags.bit_value.RemoteControlConnected = false;
                //                    appData.mcp23017.status_bit.found = false;
                appData.mcp23017.status_bit.initialized = false;
            }
        }
        //        }
    }
}

void TMR3_CallBack( void )
{
    static volatile uint8_t counter_100ms = 0; // variable for capture the number of positive edge counter in 10 ms
//    static volatile uint8_t count_call_scanning_push_button = 0; // variable for ScanningPushButton
    static volatile uint16_t capture_positive_edge_counter_in_10ms = 0; // variable for capture the number of positive edge counter in 10 ms
    static volatile uint16_t previous_positive_edge_counter_in_10ms = 0; // and this to check if there is a new value

    capture_positive_edge_counter_in_10ms = counter_positive_edge_rdyclk; // MikroElektronika "RFid Reader Board": 0x0483 <=> 1155 (dec) --> 115,5 kHz
    counter_positive_edge_rdyclk = 0; // reset RDY/CLK pulse counter

    /* Check if we can update new value. */
    if ( g_new_value_of_em4095_rdyclk_measurement == false )
    {
        /* and if there is a new value to display */
        if ( capture_positive_edge_counter_in_10ms != previous_positive_edge_counter_in_10ms )
        {
            previous_positive_edge_counter_in_10ms = capture_positive_edge_counter_in_10ms;
            g_rdyclk_count_in_10ms = capture_positive_edge_counter_in_10ms;
            g_new_value_of_em4095_rdyclk_measurement = true;
        }
    }

    switch ( appDataDoor.reward_door_status )
    {            
        case DOOR_MOVING:
            /* Moving in action. */
            if ( servomotorMoveTheDoor( ) )
            {
                appDataDoor.reward_door_status = DOOR_MOVED;
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

    if ( g_timeout_em4095_x20ms )
    {
        --g_timeout_em4095_x20ms;
    }
    
    if ( g_timeout_standby_x20ms )
    {
        --g_timeout_standby_x20ms;
    }

    if ( g_timeout_leds_status_x20ms )
    {
        --g_timeout_leds_status_x20ms;
    }

    /* Timeout x100 ms here = 5x 20 ms */
    if ( ++counter_100ms == 5 )
    {
        counter_100ms = 0; /* reset counter every 100 ms */
        if ( g_timeout_taking_reward != 0 )
        {
            --g_timeout_taking_reward;
        }
    }

    /* 200 ms here */
//    if ( ++count_call_scanning_push_button == 10 )
//    {
//        /* Call Scanning Push Button, either every 200 ms. */
//        count_call_scanning_push_button = 0; /* reset ticker counter */
//
//        if ( appData.flags.bit_value.RemoteControlConnected )
//        {
//            APP_ScanningPushButtonTasks( ); /* calling task for reading button */
//            blinkDigitState = !blinkDigitState; /* change display state for blinking digit */
//        }
//    }

}

void TMR4_CallBack( void )
{
    static volatile uint16_t counter_timeout_160ms = 0;

//#if defined (DEBUG_RFID_WORKING_ON_LED_STATUS)
//    LED_STATUS_G_SetHigh( );
//#endif

    // callback function - called every 15th pass 
    //    if ( ++CountCallBack >= TMR4_INTERRUPT_TICKER_FACTOR )
    // with 15 next interrupt occur at 442 us
    // old : with 17 next interrupt occur at 495 us
    // counter_delay_read_bit
    if ( ++g_counter_delay_read_bit > DECODING_RFID_THREE_QUARTER_PERIODE_READING )
    {
        // EM4095 DEMOD_OUT interrupt on timer even every 400 us :
        //#if defined (DEBUG_RFID_WORKING_ON_LED_STATUS)
        //        LED_STATUS_G_SetHigh( );
        //#endif
        g_datastream_read_bit = EM4095_DEMOD_OUT_GetValue( );
        ++g_counter_delay_overflow; // increment every 440 us

        // reset ticker counter
        g_counter_delay_read_bit = 0;
    }

    /* Test timeout for 160 ms */
    if ( ++counter_timeout_160ms >= TIMER_ISR_TICKER_FACTOR_FOR_160MS )
    {
        // ticker function call
        if ( g_rfid_activate )
        {
//#if defined (DEBUG_RFID_WORKING_ON_LED_STATUS)
//            LED_STATUS_R_SetHigh( );
//#endif               
            g_timeout_reading_pit_tag = ( g_timeout_reading_pit_tag > 0 ) ? g_timeout_reading_pit_tag - 1 : 0;
            //            EM4095_SHD_Toggle( ); /* EM4095 active when SHD is low state, EM4095 enable. */
        }
        else
        {
            EM4095_SHD_DISABLE( );
        }

        // reset ticker counter
        counter_timeout_160ms = 0;
    }
}


/*******************************************************************************
 End of File
 */
