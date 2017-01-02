#ifndef APP_LED_STATUS_H
#define APP_LED_STATUS_H

#include "mcc_generated_files/mcc.h"
//#include "mcc_generated_files/pin_manager.h"

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
void doFlashEachLedsStatus( uint16_t flash_duration );
void doLedsStatusBlink( LED_STATUS color, uint8_t blink_number, uint16_t t_on, uint16_t t_off );

#endif /* APP_LED_STATUS_H */
