/**
 * @file app_attractive_leds.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#include "app.h"
#include "app_attractive_leds.h"

void setAttractiveLedsOff( void )
{
    uint16_t value = 0;

    /* Set DC of PWM1 for red LEDs. */
    OC1_SingleCompareValueSet( value );
    /* Set DC of PWM2 for green LEDs. */
    OC2_SingleCompareValueSet( value );
    /* Set DC of PWM3 for blue LEDs. */
    OC3_SingleCompareValueSet( value );
    
    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_OFF;
        
}

/* Set attractive LEDs color. */
void setAttractiveLedsOn( void )
{
    /* Set DC of PWM1 for red LEDs. */
    OC1_SingleCompareValueSet( appDataAttractiveLeds.red[appDataAttractiveLeds.current_color_index] );
    /* Set DC of PWM2 for green LEDs. */
    OC2_SingleCompareValueSet( appDataAttractiveLeds.green[appDataAttractiveLeds.current_color_index] );
    /* Set DC of PWM3 for blue LEDs. */
    OC3_SingleCompareValueSet( appDataAttractiveLeds.blue[appDataAttractiveLeds.current_color_index] );
    
    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_ON;
    
}

// FIXME: setAttractiveLedsColor( ) == setAttractiveLedsOn( )

/* Set all color for attractive LEDs. */
void setAttractiveLedsColor( void )
{
    /* Set color for red attractive LEDs. */
    //setAttractiveRedLedsColor( );
    OC1_SingleCompareValueSet( appDataAttractiveLeds.red[appDataAttractiveLeds.current_color_index] );

    /* Set color for green attractive LEDs. */
    //setAttractiveGreenLedsColor( );
    OC2_SingleCompareValueSet( appDataAttractiveLeds.green[appDataAttractiveLeds.current_color_index] );

    /* Set color for blue attractive LEDs. */
    //setAttractiveBlueLedsColor( );
    OC3_SingleCompareValueSet( appDataAttractiveLeds.blue[appDataAttractiveLeds.current_color_index] );
    
    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_ON;
    
}

///* Set all color for attractive LEDs. */
//void setDoorLedsColor( void )
//{
//    /* Set color for red attractive LEDs. */
//    //setAttractiveRedLedsColor( );
//    OC1_SingleCompareValueSet( appData.open_door_red );
//
//    /* Set color for green attractive LEDs. */
//    //setAttractiveGreenLedsColor( );
//    OC2_SingleCompareValueSet( appData.open_door_green );
//
//    /* Set color for blue attractive LEDs. */
//    //setAttractiveBlueLedsColor( );
//    OC3_SingleCompareValueSet( appData.open_door_blue );
//}

/* Set color for red attractive LEDs. */
//void setAttractiveRedLedsColor( uint8_t red_color )
void setAttractiveRedLedsColor( uint16_t dc_pwm )
{
    appDataAttractiveLeds.red[appDataAttractiveLeds.current_color_index] = dc_pwm;
    /* Set DC of PWM1 for red LEDs. */
    // TODO: appDataAttractiveLeds.red = red_color;
    OC1_SingleCompareValueSet( appDataAttractiveLeds.red[appDataAttractiveLeds.current_color_index] );
    
    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_ON;
    
}

/* Set color for green attractive LEDs. */
void setAttractiveGreenLedsColor( uint16_t dc_pwm )
{
    appDataAttractiveLeds.green[appDataAttractiveLeds.current_color_index] = dc_pwm;
    /* Set DC of PWM2 for green LEDs. */
    OC2_SingleCompareValueSet( appDataAttractiveLeds.green[appDataAttractiveLeds.current_color_index] );
    
    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_ON;
    
}

/* Set color for blue attractive LEDs. */
void setAttractiveBlueLedsColor( uint16_t dc_pwm )
{
    appDataAttractiveLeds.blue[appDataAttractiveLeds.current_color_index] = dc_pwm;
    /* Set DC of PWM3 for blue LEDs. */
    OC3_SingleCompareValueSet( appDataAttractiveLeds.blue[appDataAttractiveLeds.current_color_index] );
    
    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_ON;
    
}


/*******************************************************************************
 End of File
 */
