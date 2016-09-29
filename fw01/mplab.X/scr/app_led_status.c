#include "app_led_status.h"
//#include "usb_hal_local.h"

void setLedsStatusColor( LED_STATUS status )
{
    switch ( status )
    {
        case LEDS_OFF:
            /* all leds off */
            LED_STATUS_R_SetHigh( );
            LED_STATUS_G_SetHigh( );
            LED_STATUS_B_SetHigh( );
            break;

        case LED_WHITE:
        case LEDS_ON:
            /* all leds on */
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetLow( );
            LED_STATUS_B_SetLow( );
            break;

        case LED_GREEN:
        case LEDS_OK:
            /* set only green led */
            LED_STATUS_R_SetHigh( );
            LED_STATUS_G_SetLow( );
            LED_STATUS_B_SetHigh( );
            break;

        case LED_BLUE:
//        case LEDS_INIT:
            /* set only blue led */
            LED_STATUS_R_SetHigh( );
            LED_STATUS_G_SetHigh( );
            LED_STATUS_B_SetLow( );
            break;

        case LED_YELLOW:
//        case LEDS_IDLE:
            /* set green and red leds, make orange color */
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetLow( );
            LED_STATUS_B_SetHigh( );
            break;

        case LED_PURPLE:
        case LEDS_WAIT_USB_KEY:
            /* set blue and red leds, make purple color */
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetHigh( );
            LED_STATUS_B_SetLow( );
            break;

        case LED_RED:
        case LEDS_ERROR:
            /* set only red led */
            LED_STATUS_R_SetLow( );
            LED_STATUS_G_SetHigh( );
            LED_STATUS_B_SetHigh( );
            break;
    }
}

void doFlashEachLedsStatus( uint16_t flash_duration )
{
    /* Test LEDs STATUS, lighting sequence 
     * executing the sequence of LED rgb
     */
    LED_STATUS_R_SetLow( );
    __delay_ms( flash_duration ); // delay 500 milliseconds
    LED_STATUS_R_SetHigh( );

    LED_STATUS_G_SetLow( );
    __delay_ms( flash_duration ); // delay 500 milliseconds
    LED_STATUS_G_SetHigh( );

    LED_STATUS_B_SetLow( );
    __delay_ms( flash_duration ); // delay 500 milliseconds
    LED_STATUS_B_SetHigh( );
}

void doLedsStatusBlink( LED_STATUS color, uint8_t blink_number, uint16_t t_on, uint16_t t_off )
{
    for (; blink_number > 0; --blink_number )
    {
        setLedsStatusColor( color );
        __delay_ms( t_on );
        setLedsStatusColor( LEDS_OFF );
        __delay_ms( t_off );
    }
}
