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
