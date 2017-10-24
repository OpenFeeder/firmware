/**
 * @file led_status.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#include "app.h"
#include "led_status.h"
#include <libpic30.h>

void setLedsStatusColor( LED_STATUS status )
{
#if !defined (DEBUG_RFID_WORKING_ON_LED_STATUS)
    switch ( status )
    {
        case LEDS_OFF:
            /* all leds off */
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetLow( );
            LED_STATUS_B_SetLow( );
            LED_STATUS_Y_SetLow( );
            break;

        case LED_WHITE:
        case LEDS_ON:
            /* all leds on */
            LED_STATUS_R_SetHigh( );
            LED_STATUS_G_SetHigh( );
            LED_STATUS_B_SetHigh( );
            LED_STATUS_Y_SetHigh( );
            break;

        case LED_GREEN:
        case LEDS_OK:
            /* set only green led */
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetHigh( );
            LED_STATUS_B_SetLow( );
            LED_STATUS_Y_SetLow( );
            break;

        case LED_BLUE:
        case LED_PURPLE:
        case LEDS_WAIT_USB_KEY:
            //        case LEDS_INIT:
            /* set only blue led */
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetLow( );
            LED_STATUS_B_SetHigh( );
            LED_STATUS_Y_SetLow( );
            break;
            //            /* set blue color */
            //            LED_STATUS_B_SetHigh( );
            //            break;

        case LED_YELLOW:
            /* set green and red leds, make orange color */
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetLow( );
            LED_STATUS_B_SetLow( );
            LED_STATUS_Y_SetHigh( );
            break;

        case LED_RED:
        case LEDS_ERROR:
            /* set only red led */
            LED_STATUS_R_SetHigh( );
            LED_STATUS_G_SetLow( );
            LED_STATUS_B_SetLow( );
            LED_STATUS_Y_SetLow( );
            break;
    }
#endif
}

//void doFlashEachLedsStatus( uint16_t flash_duration )
//{
//#if !defined (DEBUG_RFID_WORKING_ON_LED_STATUS)
//    /* Test LEDs STATUS, lighting sequence 
//     * executing the sequence of LED rgb
//     */
//    //    LED_STATUS_R_SetLow( );
//    setLedsStatusColor( LED_RED );
//    __delay_ms( flash_duration ); // delay 500 milliseconds
//    //    LED_STATUS_R_SetHigh( );
//    setLedsStatusColor( LEDS_OFF );
//
//    //    LED_STATUS_G_SetLow( );
//    setLedsStatusColor( LED_GREEN );
//    __delay_ms( flash_duration ); // delay 500 milliseconds
//    //    LED_STATUS_G_SetHigh( );
//    setLedsStatusColor( LEDS_OFF );
//
//    //    LED_STATUS_B_SetLow( );
//    setLedsStatusColor( LED_BLUE );
//    __delay_ms( flash_duration ); // delay 500 milliseconds
//    //    LED_STATUS_B_SetHigh( );
//    setLedsStatusColor( LEDS_OFF );
//#endif
//}

//void doLedsStatusBlink( LED_STATUS color, uint8_t blink_number, const uint16_t t_on, const uint16_t t_off )

void doLedsStatusBlink( LED_STATUS color, uint8_t blink_number, uint16_t t_on, uint16_t t_off )
{
#if !defined (DEBUG_RFID_WORKING_ON_LED_STATUS)
    for (; blink_number > 0; --blink_number )
    {
        setLedsStatusColor( color );
        //        __delay_ms( t_on );
        setLedsStatusColor( LEDS_OFF );
        //        __delay_ms( t_off );
    }
#endif
}

void LedsStatusBlink( LED_STATUS color, uint16_t t_on_ms, uint16_t t_off_ms )
{
    static bool next_blink_state = false;

    /* Blink LEDs status for user information about APP state machine. */
    if ( isDelayMsEndingLEDsStatus( ) )
    {
        if ( next_blink_state )
        {
            setDelayMsLEDsStatus( t_on_ms ); /* Reload the delay for LEDs status. */
            setLedsStatusColor( color );
            next_blink_state = false; /* Change state for next time. */
        }
        else
        {
            setDelayMsLEDsStatus( t_off_ms ); /* Reload the delay for LEDs status. */
            setLedsStatusColor( LEDS_OFF );
            next_blink_state = true; /* Change state for next time. */
        }
    }
}


/*******************************************************************************
 End of File
 */
