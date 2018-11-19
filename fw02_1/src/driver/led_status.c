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
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetLow( );
            LED_STATUS_B_SetLow( );
            LED_STATUS_Y_SetLow( );
            break;
            
        case LEDS_ON:
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
        case LEDS_WAIT_USB_KEY:
        case LED_USB_ACCESS:
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetLow( );
            LED_STATUS_B_SetHigh( );
            LED_STATUS_Y_SetLow( );
            break;

        case LED_PURPLE:
            LED_STATUS_R_SetHigh( );
            LED_STATUS_G_SetLow( );
            LED_STATUS_B_SetHigh( );
            LED_STATUS_Y_SetLow( );
            break;

        case LED_YELLOW:
        case LED_SERVO:
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetLow( );
            LED_STATUS_B_SetLow( );
            LED_STATUS_Y_SetHigh( );
            break;

        case LED_ORANGE:
            LED_STATUS_R_SetHigh( );
            LED_STATUS_G_SetLow( );
            LED_STATUS_B_SetLow( );
            LED_STATUS_Y_SetHigh( );
            break;
            
        case LED_RED:
        case LEDS_ERROR:
            LED_STATUS_R_SetHigh( );
            LED_STATUS_G_SetLow( );
            LED_STATUS_B_SetLow( );
            LED_STATUS_Y_SetLow( );
            break;

        case LEDS_ERROR_CRITICAL_BATTERY:
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetLow( );
            LED_STATUS_B_SetLow( );
            LED_STATUS_Y_SetHigh( );
            break;

        case LEDS_ERROR_CRITICAL_FOOD:
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetLow( );
            LED_STATUS_B_SetHigh( );
            LED_STATUS_Y_SetLow( );
            break;

        case LED_PITTAG_DENIED:
        case LEDS_ERROR_CRITICAL_VBAT:
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetLow( );
            LED_STATUS_B_SetHigh( );
            LED_STATUS_Y_SetHigh( );
            break;

        case LEDS_ERROR_CRITICAL_DOOR:
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetHigh( );
            LED_STATUS_B_SetLow( );
            LED_STATUS_Y_SetHigh( );
            break;

        case LEDS_ERROR_RFID_FREQ:
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetHigh( );
            LED_STATUS_B_SetLow( );
            LED_STATUS_Y_SetLow( );
            break;
        
        case LED_PITTAG_ACCEPTED:
        case LEDS_ERROR_RFID_TIMEOUT:
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetHigh( );
            LED_STATUS_B_SetHigh( );
            LED_STATUS_Y_SetLow( );
            break;
            
        case LEDS_TOO_MANY_SOFTWARE_RESET:
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetHigh( );
            LED_STATUS_B_SetHigh( );
            LED_STATUS_Y_SetHigh( );
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

//void doLedsStatusBlink( LED_STATUS color, uint8_t blink_number, uint16_t t_on, uint16_t t_off )
//{
//#if !defined (DEBUG_RFID_WORKING_ON_LED_STATUS)
//    for (; blink_number > 0; --blink_number )
//    {
//        setLedsStatusColor( color );
//        //        __delay_ms( t_on );
//        setLedsStatusColor( LEDS_OFF );
//        //        __delay_ms( t_off );
//    }
//#endif
//}


void LedsStatusBlink( LED_STATUS color_1, LED_STATUS color_2, uint16_t t_on_ms, uint16_t t_off_ms )
{
    static bool next_blink_state = false;

    /* Blink LEDs status for user information about APP state machine. */
    if ( isDelayMsEndingLEDsStatus( ) )
    {
        if ( next_blink_state )
        {
            setDelayMsLEDsStatus( t_on_ms ); /* Reload the delay for LEDs status. */
            setLedsStatusColor( color_1 );
            next_blink_state = false; /* Change state for next time. */
        }
        else
        {
            setDelayMsLEDsStatus( t_off_ms ); /* Reload the delay for LEDs status. */
            setLedsStatusColor( color_2 );
            next_blink_state = true; /* Change state for next time. */
        }
    }
}


void checkLedsStatus( void )
{
    uint16_t delay_ms = 250;
    
    setLedsStatusColor( LEDS_OFF );

    setLedsStatusColor( LED_GREEN );
    setDelayMsLEDsStatus( delay_ms );
    while ( false == isDelayMsEndingLEDsStatus( ) );
    setLedsStatusColor( LED_BLUE );
    setDelayMsLEDsStatus( delay_ms );
    while ( false == isDelayMsEndingLEDsStatus( ) );
    setLedsStatusColor( LED_YELLOW );
    setDelayMsLEDsStatus( delay_ms );
    while ( false == isDelayMsEndingLEDsStatus( ) );
    setLedsStatusColor( LED_RED );
    setDelayMsLEDsStatus( delay_ms );
    while ( false == isDelayMsEndingLEDsStatus( ) );
    setLedsStatusColor( LED_YELLOW );
    setDelayMsLEDsStatus( delay_ms );
    while ( false == isDelayMsEndingLEDsStatus( ) );
    setLedsStatusColor( LED_BLUE );
    setDelayMsLEDsStatus( delay_ms );
    while ( false == isDelayMsEndingLEDsStatus( ) );
    setLedsStatusColor( LED_GREEN );
    setDelayMsLEDsStatus( delay_ms );
    while ( false == isDelayMsEndingLEDsStatus( ) );

    setLedsStatusColor( LEDS_OFF );
}


/*******************************************************************************
 End of File
 */
