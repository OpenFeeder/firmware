/**
 * @file app_attractive_leds.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 2.0
 * @date 07/03/2017
 */

#ifndef _APP_ATTRACTIVE_LEDS_HEADER_H
#define _APP_ATTRACTIVE_LEDS_HEADER_H

#define ATTRACTIVE_LEDS_ON 1
#define ATTRACTIVE_LEDS_OFF 0

#define ATTRACTIVE_LEDS_COLOR_A 0
#define ATTRACTIVE_LEDS_COLOR_B 1

typedef struct
{
    uint16_t red[2];
    uint16_t green[2];
    uint16_t blue[2];

    struct tm wake_up_time;
    struct tm sleep_time;

    uint8_t alt_delay;
    uint8_t alt_sec_elapsed;

    bool status;

    uint8_t current_color_index;

} APP_DATA_LEDS;

bool initAttractiveLeds( void );
void setAttractiveLedsOff( void );
void setAttractiveLedsOn( void );
void setAttractiveLedsColor( void );
void setAttractiveLedsNoColor( void );
void setAttractiveRedLedsColor( uint8_t );
void setAttractiveGreenLedsColor( uint8_t );
void setAttractiveBlueLedsColor( uint8_t );

#endif /* _APP_ATTRACTIVE_LEDS_HEADER_H */


/*******************************************************************************
 End of File
 */
