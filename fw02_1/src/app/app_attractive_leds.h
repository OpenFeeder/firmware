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
    uint8_t red[2]; // FIXME : uint16_t pass to uint8_t
    uint8_t green[2];
    uint8_t blue[2];

    struct tm wake_up_time;
    struct tm sleep_time;

    uint8_t alt_delay;
    uint8_t alt_sec_elapsed;

    bool status;

    uint8_t current_color_index;
    
    uint8_t pattern_number;
    bool pattern_idx;    
    bool pattern[4];    
    uint8_t leds_index[4];
//    uint8_t pattern_one_led_num; // number in config.ini
//    uint8_t pattern_one_led_idx; // index on the OF
    uint8_t pattern_one_led_current; // index on the OF
    uint8_t pattern_one_led_groups[3];
    
    double pattern_percent;

} APP_DATA_LEDS;

typedef enum
{
    ALL_LEDS,
    LEFT_RIGHT_LEDS,
    TOP_BOTTOM_LEDS,
    ONE_LED
        
} LEDS_PATTERN;

bool initAttractiveLeds( void );
void setAttractiveLedsOff( void );
void setAttractiveLedsOn( void );
void setAttractiveLedsColor( void );
void setAttractiveLedsNoColor( void );
void setAttractiveRedLedsColor( uint8_t );
void setAttractiveGreenLedsColor( uint8_t );
void setAttractiveBlueLedsColor( uint8_t );

void setOneAttractiveLedColor( uint8_t, uint8_t, uint8_t, uint8_t );

void setAttractiveLedsPattern( void );

void testAttractiveLeds( void );

void setAttractiveLedsIndex( void );

#endif /* _APP_ATTRACTIVE_LEDS_HEADER_H */


/*******************************************************************************
 End of File
 */
