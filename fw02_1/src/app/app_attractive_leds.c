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
bool initAttractiveLeds( void )
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
    //    writeBuffer[4] = 0b10101010; // CTRLREG PWM on all output for LEDOUT3
    //    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 5 );

    writeBuffer[0] = CTRLREG_LEDOUT0;
    writeBuffer[1] = 0b10101010; // CTRLREG PWM on all output for LEDOUT0
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
    if ( I2C1_MESSAGE_COMPLETE != i2c_status )
    {
        return false;
    }

    writeBuffer[0] = CTRLREG_LEDOUT1;
    writeBuffer[1] = 0b10101010; // CTRLREG PWM on all output for LEDOUT1
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
    if ( I2C1_MESSAGE_COMPLETE != i2c_status )
    {
        return false;
    }

    writeBuffer[0] = CTRLREG_LEDOUT2;
    writeBuffer[3] = 0b10101010; // CTRLREG PWM on all output for LEDOUT2
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
    if ( I2C1_MESSAGE_COMPLETE != i2c_status )
    {
        return false;
    }

    writeBuffer[0] = CTRLREG_LEDOUT3;
    writeBuffer[4] = 0b10101010; // CTRLREG PWM on all output for LEDOUT3
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
    if ( I2C1_MESSAGE_COMPLETE != i2c_status )
    {
        return false;
    }

    return true;
}

void setAttractiveLedsIndex( void )
{
    FILEIO_SEARCH_RECORD searchRecord;
    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    char buf[20];
    char subbuf[5];
    char udid[5];
    
    appDataAttractiveLeds.leds_index[0] = 1;
    appDataAttractiveLeds.leds_index[1] = 2;
    appDataAttractiveLeds.leds_index[2] = 3;
    appDataAttractiveLeds.leds_index[3] = 4;
        
    if ( FILEIO_RESULT_FAILURE == FILEIO_Find( "UDIDLEDS.TXT", FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, true ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE)
            printf( "Warning:  file UDIDLEDS.TXT not found (%u). Use default numbers for attractive LEDs\n", errF);
#endif 
        return;
    }
    
    sprintf(udid, "%02lX%02lX", appData.udid.words[3] & 0xFF, appData.udid.words[4] & 0xFF);

    udid[4] = '\0';
    
    if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, "UDIDLEDS.TXT", FILEIO_OPEN_READ ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "Warning:  unable to open UDIDLEDS.TXT (%u). Use default numbers for attractive LEDs\n", errF);
#endif 
        return;
    }

    while ( 14 == FILEIO_Read( buf, 1, 14, &file ) )
    {
        memcpy(subbuf, buf, 4);
        subbuf[4] = '\0';
        
        if ( 0 == strcmp( subbuf, udid ) )
        {           
           appDataAttractiveLeds.leds_index[0] = buf[5] - '0';
           appDataAttractiveLeds.leds_index[1] = buf[7] - '0';
           appDataAttractiveLeds.leds_index[2] = buf[9] - '0';
           appDataAttractiveLeds.leds_index[3] = buf[11] - '0';
        
           break;
        }             
    }
    
    if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) ) 
    {
        errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "Warning:  unable to close UDIDLEDS.TXT (%u).\n", errF);
#endif 
    }
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
void setAttractiveLedsNoColor( void )
{
    /* Set color for red attractive LEDs. */
    setAttractiveRedLedsColor( 0 );

    /* Set color for green attractive LEDs. */
    setAttractiveGreenLedsColor( 0 );

    /* Set color for blue attractive LEDs. */
    setAttractiveBlueLedsColor( 0 );

    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_ON;
}

/* Set all color for attractive LEDs. */
void setAttractiveLedsColor( void )
{
    /* Set color for red attractive LEDs. */
    setAttractiveRedLedsColor( appDataAttractiveLeds.red[appDataAttractiveLeds.current_color_index] );

    /* Set color for green attractive LEDs. */
    setAttractiveGreenLedsColor( appDataAttractiveLeds.green[appDataAttractiveLeds.current_color_index] );

    /* Set color for blue attractive LEDs. */
    setAttractiveBlueLedsColor( appDataAttractiveLeds.blue[appDataAttractiveLeds.current_color_index] );

    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_ON;
}


void setOneAttractiveLedColor( uint8_t n, uint8_t red, uint8_t green, uint8_t blue )
{
    
    I2C1_MESSAGE_STATUS i2c_status = I2C1_MESSAGE_COMPLETE; // the status of write data on I2C bus
    static uint8_t writeBuffer[2]; // data to transmit
    
    switch (n)
    {
        case 1:
            writeBuffer[0] = CTRLREG_PWM0;
            writeBuffer[1] = red; // PWM0 Individual Duty Cycle for LED_RGB1_R
            i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);

            writeBuffer[0] = CTRLREG_PWM1;
            writeBuffer[1] = green; // PWM1 Individual Duty Cycle for LED_RGB1_G
            i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);

            writeBuffer[0] = CTRLREG_PWM2;
            writeBuffer[1] = blue; // PWM2 Individual Duty Cycle for LED_RGB1_B
            i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);

            break;
            
        case 2:
            writeBuffer[0] = CTRLREG_PWM3;
            writeBuffer[1] = red; // PWM3 Individual Duty Cycle for LED_RGB2_R
            i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);

            writeBuffer[0] = CTRLREG_PWM4;
            writeBuffer[1] = green; // PWM4 Individual Duty Cycle for LED_RGB2_G
            i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);

            writeBuffer[0] = CTRLREG_PWM5;
            writeBuffer[1] = blue; // PWM5 Individual Duty Cycle for LED_RGB2_B
            i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);

            break;
            
        case 3:
            writeBuffer[0] = CTRLREG_PWM6;
            writeBuffer[1] = red; // PWM6 Individual Duty Cycle for LED_RGB3_R
            i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);

            writeBuffer[0] = CTRLREG_PWM7;
            writeBuffer[1] = green; // PWM7 Individual Duty Cycle for LED_RGB3_G
            i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);

            writeBuffer[0] = CTRLREG_PWM8;
            writeBuffer[1] = blue; // PWM8 Individual Duty Cycle for LED_RGB3_B
            i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);

            break;
            
        case 4:
            writeBuffer[0] = CTRLREG_PWM9;
            writeBuffer[1] = red; // PWM9 Individual Duty Cycle for LED_RGB4_R
            i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);

            writeBuffer[0] = CTRLREG_PWM10;
            writeBuffer[1] = green; // PWM10 Individual Duty Cycle for LED_RGB4_G
            i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);

            writeBuffer[0] = CTRLREG_PWM11;
            writeBuffer[1] = blue; // PWM11 Individual Duty Cycle for LED_RGB4_B
            i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);

            break;
    }

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
    //    writeBuffer[1] = dc_pwm; // PWM3 Individual Duty Cycle for LED_RGB2_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM6;
    //    writeBuffer[1] = dc_pwm; // PWM6 Individual Duty Cycle for LED_RGB3_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM9;
    //    writeBuffer[1] = dc_pwm; // PWM9 Individual Duty Cycle for LED_RGB4_R
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
    writeBuffer[1] = dc_pwm; // PWM1 Individual Duty Cycle for LED_RGB1_G
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM4;
    //    writeBuffer[1] = dc_pwm; // PWM4 Individual Duty Cycle for LED_RGB2_G
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM7;
    //    writeBuffer[1] = dc_pwm; // PWM7 Individual Duty Cycle for LED_RGB3_G
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM10;
    //    writeBuffer[1] = dc_pwm; // PWM10 Individual Duty Cycle for LED_RGB4_G
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
    writeBuffer[1] = dc_pwm; // PWM2 Individual Duty Cycle for LED_RGB1_B
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM5;
    //    writeBuffer[1] = dc_pwm; // PWM5 Individual Duty Cycle for LED_RGB2_B
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM8;
    //    writeBuffer[1] = dc_pwm; // PWM8 Individual Duty Cycle for LED_RGB3_B
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM11;
    //    writeBuffer[1] = dc_pwm; // PWM11 Individual Duty Cycle for LED_RGB4_B
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_ON;
}

void setAttractiveLedsPattern( void )
{

    uint8_t i;
    
    for (i=0;i<4;i++)
    {
        
        setOneAttractiveLedColor( appDataAttractiveLeds.leds_index[i], 
                                 appDataAttractiveLeds.red[appDataAttractiveLeds.pattern[i]], 
                                 appDataAttractiveLeds.green[appDataAttractiveLeds.pattern[i]], 
                                 appDataAttractiveLeds.blue[appDataAttractiveLeds.pattern[i]] );
        Nop();
    }
    
}

void testAttractiveLeds( void )
{
    I2C1_MESSAGE_STATUS i2c_status = I2C1_MESSAGE_COMPLETE; // the status of write data on I2C bus
    uint8_t writeBuffer[2]; // data to transmit
    uint16_t delay = 500;

    writeBuffer[0] = CTRLREG_PWM0;
    writeBuffer[1] = 55; // PWM0 Individual Duty Cycle for LED_RGB1_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
                
    writeBuffer[0] = CTRLREG_PWM3;
    writeBuffer[1] = 0; // PWM3 Individual Duty Cycle for LED_RGB2_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM6;
    writeBuffer[1] = 0; // PWM6 Individual Duty Cycle for LED_RGB3_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    writeBuffer[0] = CTRLREG_PWM9;
    writeBuffer[1] = 0; // PWM9 Individual Duty Cycle for LED_RGB4_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_ON;
    
    setDelayMs( delay );
    while ( 0 == isDelayMsEnding( ) );
    
    writeBuffer[0] = CTRLREG_PWM0;
    writeBuffer[1] = 0; // PWM0 Individual Duty Cycle for LED_RGB1_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
    
    writeBuffer[0] = CTRLREG_PWM3;
    writeBuffer[1] = 55; // PWM0 Individual Duty Cycle for LED_RGB1_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
    
    setDelayMs( delay );
    while ( 0 == isDelayMsEnding( ) );
    
    writeBuffer[0] = CTRLREG_PWM3;
    writeBuffer[1] = 0; // PWM0 Individual Duty Cycle for LED_RGB1_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
    
    writeBuffer[0] = CTRLREG_PWM6;
    writeBuffer[1] = 55; // PWM0 Individual Duty Cycle for LED_RGB1_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
    
    setDelayMs( delay );
    while ( 0 == isDelayMsEnding( ) );
    
    writeBuffer[0] = CTRLREG_PWM6;
    writeBuffer[1] = 0; // PWM0 Individual Duty Cycle for LED_RGB1_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
    
    writeBuffer[0] = CTRLREG_PWM9;
    writeBuffer[1] = 55; // PWM0 Individual Duty Cycle for LED_RGB1_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
    
    setDelayMs( delay );
    while ( 0 == isDelayMsEnding( ) );
    
    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_OFF;
    
}
/*******************************************************************************
 End of File
 */
