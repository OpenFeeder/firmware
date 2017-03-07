/**
 * @file app_attractive_leds.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 2.0
 * @date 07/03/2017
 */

#include "app.h"
#include "app_attractive_leds.h"
#include "pca9622.h"

/* Initialize LED0 to LED14 output */
void initAttractiveLeds( void )
{
    I2C1_MESSAGE_STATUS i2c_status = I2C1_MESSAGE_COMPLETE; // the status of write data on I2C bus
    uint8_t writeBuffer[5]; // data to transmit

    /* Initialize PCA9622 device in Normal mode. */
    writeBuffer[0] = CTRLREG_MODE1;
    writeBuffer[1] = NORMAL_MODE;
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    //    writeBuffer[0] = CTRLREG_LEDOUT0 || CTRLREG_AUTO_INC_ENABLE;
    //    writeBuffer[1] = 0b10101010; // CTRLREG PWM on all output for LEDOUT0
    //    writeBuffer[2] = 0b10101010; // CTRLREG PWM on all output for LEDOUT1
    //    writeBuffer[3] = 0b10101010; // CTRLREG PWM on all output for LEDOUT2
    //    writeBuffer[4] = 0b00101010; // CTRLREG PWM on all output for LEDOUT3, except LDR15 off
    //    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 5 );

    writeBuffer[0] = CTRLREG_LEDOUT0;
    writeBuffer[1] = 0b10101010; // CTRLREG PWM on all output for LEDOUT0
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_LEDOUT1;
    writeBuffer[1] = 0b10101010; // CTRLREG PWM on all output for LEDOUT1
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_LEDOUT2;
    writeBuffer[3] = 0b10101010; // CTRLREG PWM on all output for LEDOUT2
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_LEDOUT3;
    writeBuffer[4] = 0b00101010; // CTRLREG PWM on all output for LEDOUT3, except LDR15 off
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

#if defined (USE_UART1_SERIAL_INTERFACE)
    if ( I2C1_MESSAGE_COMPLETE != i2c_status )
    {
        printf( "Attractive LEDs initialize failed!\n" );
    }
    else
    {
        printf( "Attractive LEDs initialized.\n" );
    }
#endif
}

void setAttractiveLedsOff( void )
{
    /* Disable PCA9622 device */
    PCA9622_OE_SetHigh( );

    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_OFF;
}

/* Set attractive LEDs color. */
void setAttractiveLedsOn( void )
{
    /* Enable PCA9622 device in Normal mode */
    PCA9622_OE_SetLow( ); // output enable pin is active LOW

    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_ON;
}

/* Set all color for attractive LEDs. */
void setAttractiveLedsColor( void )
{
    /* Set color for red attractive LEDs. */
    //    OC1_SingleCompareValueSet(appDataAttractiveLeds.red[appDataAttractiveLeds.current_color_index]);
    setAttractiveRedLedsColor( appDataAttractiveLeds.red[appDataAttractiveLeds.current_color_index] );

    //    /* Set color for green attractive LEDs. */
    //    OC2_SingleCompareValueSet(appDataAttractiveLeds.green[appDataAttractiveLeds.current_color_index]);
    setAttractiveGreenLedsColor( appDataAttractiveLeds.green[appDataAttractiveLeds.current_color_index] );

    //    /* Set color for blue attractive LEDs. */
    //    OC3_SingleCompareValueSet(appDataAttractiveLeds.blue[appDataAttractiveLeds.current_color_index]);
    setAttractiveBlueLedsColor( appDataAttractiveLeds.blue[appDataAttractiveLeds.current_color_index] );

    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_ON;
}

/* Set color for all red attractive LEDs. */
void setAttractiveRedLedsColor( uint8_t dc_pwm )
{
    I2C1_MESSAGE_STATUS i2c_status = I2C1_MESSAGE_COMPLETE; // the status of write data on I2C bus
    uint8_t writeBuffer[2]; // data to transmit

    //    appDataAttractiveLeds.red[appDataAttractiveLeds.current_color_index] = dc_pwm;
//    dc_pwm = 255 - dc_pwm;

    //writeBuffer[0] = CTRLREG_PWM0 || CTRLREG_AUTO_INC_ENABLE;
    writeBuffer[0] = CTRLREG_PWM0;
    writeBuffer[1] = dc_pwm; // PWM0 Individual Duty Cycle for LED_RGB1_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM3;
//    writeBuffer[1] = dc_pwm; // PWM0 Individual Duty Cycle for LED_RGB2_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM6;
//    writeBuffer[1] = dc_pwm; // PWM0 Individual Duty Cycle for LED_RGB3_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM9;
//    writeBuffer[1] = dc_pwm; // PWM0 Individual Duty Cycle for LED_RGB4_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_ON;
}

/* Set color for all green attractive LEDs. */
void setAttractiveGreenLedsColor( uint8_t dc_pwm )
{
    I2C1_MESSAGE_STATUS i2c_status = I2C1_MESSAGE_COMPLETE; // the status of write data on I2C bus
    uint8_t writeBuffer[2]; // data to transmit

    //    appDataAttractiveLeds.green[appDataAttractiveLeds.current_color_index] = dc_pwm;
//    dc_pwm = 255 - dc_pwm;

    writeBuffer[0] = CTRLREG_PWM1;
    writeBuffer[1] = dc_pwm; // PWM0 Individual Duty Cycle for LED_RGB1_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM4;
//    writeBuffer[1] = dc_pwm; // PWM0 Individual Duty Cycle for LED_RGB2_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM7;
//    writeBuffer[1] = dc_pwm; // PWM0 Individual Duty Cycle for LED_RGB3_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM10;
//    writeBuffer[1] = dc_pwm; // PWM0 Individual Duty Cycle for LED_RGB4_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_ON;
}

/* Set color for all blue attractive LEDs. */
void setAttractiveBlueLedsColor( uint8_t dc_pwm )
{
    I2C1_MESSAGE_STATUS i2c_status = I2C1_MESSAGE_COMPLETE; // the status of write data on I2C bus
    uint8_t writeBuffer[2]; // data to transmit

    //    appDataAttractiveLeds.blue[appDataAttractiveLeds.current_color_index] = dc_pwm;
//    dc_pwm = 255 - dc_pwm;

    writeBuffer[0] = CTRLREG_PWM2;
    writeBuffer[1] = dc_pwm; // PWM0 Individual Duty Cycle for LED_RGB1_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM5;
//    writeBuffer[1] = dc_pwm; // PWM0 Individual Duty Cycle for LED_RGB2_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM8;
//    writeBuffer[1] = dc_pwm; // PWM0 Individual Duty Cycle for LED_RGB3_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM11;
//    writeBuffer[1] = dc_pwm; // PWM0 Individual Duty Cycle for LED_RGB4_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_ON;
}


/*******************************************************************************
 End of File
 */
