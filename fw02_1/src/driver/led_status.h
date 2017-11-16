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
    LEDS_OFF,
    LEDS_ON,  
    LED_RED,
    LED_GREEN,
    LED_BLUE,
    LED_PURPLE,
    LED_YELLOW,
    LED_ORANGE,
    LEDS_WAIT_USB_KEY,
    LEDS_OK,
    LEDS_ERROR,
    LEDS_ERROR_CRITICAL_BATTERY,
    LEDS_ERROR_CRITICAL_FOOD,
    LEDS_ERROR_CRITICAL_VBAT,
    LEDS_ERROR_CRITICAL_DOOR,
    LEDS_ERROR_RFID_FREQ

} LED_STATUS;

void setLedsStatusColor( LED_STATUS );
//void doFlashEachLedsStatus( uint16_t flash_duration );
//void doLedsStatusBlink( LED_STATUS, uint8_t, uint16_t, uint16_t );

void LedsStatusBlink( LED_STATUS, LED_STATUS, uint16_t, uint16_t );

void checkLedsStatus( void );

#endif /* APP_LED_STATUS_H */
/*******************************************************************************
 End of File
 */
