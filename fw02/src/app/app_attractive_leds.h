/**
 * @file app_attractive_leds.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#ifndef APP_ATTRACT_LEDS_H
#define APP_ATTRACT_LEDS_H

#define ATTRACTIVE_LEDS_ON 1
#define ATTRACTIVE_LEDS_OFF 0

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

void setAttractiveLedsOff(void);
void setAttractiveLedsOn(void);
void setAttractiveLedsColor(void);
void setAttractiveRedLedsColor(uint16_t);
void setAttractiveGreenLedsColor(uint16_t);
void setAttractiveBlueLedsColor(uint16_t);

#endif /* APP_ATTRACT_LEDS_H */


/*******************************************************************************
 End of File
 */
