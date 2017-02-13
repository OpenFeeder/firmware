/**
 * @file led_status.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 09/09/2016
 */

#ifndef APP_LED_STATUS_H
#define APP_LED_STATUS_H

typedef enum
{
    /* all leds off */
    LEDS_OFF,

    /* all leds on */
    LED_WHITE,
    LEDS_ON,

    /* set only red led */
    LED_RED,
    LEDS_ERROR,

    /* set only green led */
    LED_GREEN,
    LEDS_OK,

    /* set only blue led */
    LED_BLUE,
//    LEDS_INIT,

    /* set blue and red leds, make purple color */
    LED_PURPLE,
    LEDS_WAIT_USB_KEY,

    /* set leds red and green, make yellow color */
    LED_YELLOW

} LED_STATUS;

void setLedsStatusColor( LED_STATUS );
//void doFlashEachLedsStatus( uint16_t flash_duration );
void doLedsStatusBlink( LED_STATUS color, uint8_t blink_number, uint16_t t_on, uint16_t t_off );

void LedsStatusBlink( LED_STATUS color, uint16_t t_on_ms, uint16_t t_off_ms );


#endif /* APP_LED_STATUS_H */
/*******************************************************************************
 End of File
 */
