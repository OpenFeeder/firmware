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

    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
       store_event(OF_ATTRACTIVE_LEDS_OFF); 
    }
    
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

bool setAttractiveLedsIndex( void )
{
    FILEIO_SEARCH_RECORD sR;
    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    char buf[20];
    char subbuf[5];
    char udid[5];
    bool udidfound = false;
    int i;
    
    appDataAttractiveLeds.leds_index[0] = 1;
    appDataAttractiveLeds.leds_index[1] = 2;
    appDataAttractiveLeds.leds_index[2] = 3;
    appDataAttractiveLeds.leds_index[3] = 4;
        
    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
        store_event(OF_ATTRACTIVE_LEDS_SET_INDEX);
    }
    
    if (USB_DRIVE_NOT_MOUNTED == usbMountDrive())
    {
#if defined (USE_UART1_SERIAL_INTERFACE)
            printf( "\tWarning:  unable to mount drive. Use default order for attractive LEDs\n");
#endif 
        return false;
    }
    
    if ( true == appDataLog.log_events )
    {
        store_event(OF_FIND_UDID_LEDS);
    } 
    
    if ( FILEIO_RESULT_FAILURE == FILEIO_Find( "UDIDLEDS.TXT", FILEIO_ATTRIBUTE_ARCHIVE, &sR, true ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE)
            printf( "\tWarning:  file UDIDLEDS.TXT not found (%u). Use default order for attractive LEDs\n", errF);
#endif 
        return false;
    }
    
    sprintf(udid, "%02lX%02lX", appData.udid.words[3] & 0xFF, appData.udid.words[4] & 0xFF);

    udid[4] = '\0';
    
    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
        store_event(OF_READ_UDID_LEDS);
    } 
    if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, "UDIDLEDS.TXT", FILEIO_OPEN_READ ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "\tWarning:  unable to open UDIDLEDS.TXT (%u). Use default order for attractive LEDs\n", errF);
#endif 
        return false;
    }

    while ( 14 == FILEIO_Read( buf, 1, 14, &file ) )
    {
        memcpy(subbuf, buf, 4);
        subbuf[4] = '\0';
        
        if ( 0 == strcmp( subbuf, udid ) )
        {          
            
            for (i=0;i<4;i++)
            {
                appDataAttractiveLeds.leds_order[i] = buf[2*i+5] - '0';
                appDataAttractiveLeds.leds_index[appDataAttractiveLeds.leds_order[i]-1] = i+1;                     
            }

           udidfound = true;
           break;
        }             
    }
    
    if (false == udidfound)
    {
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "\tWarning:  unable to match UDID %s in UDIDLEDS.TXT. Use default order for attractive LEDs\n", udid);
#endif 
    }
//    else
//    {
//#if defined (USE_UART1_SERIAL_INTERFACE)
//        printf( "\tAttractive LEDs order: %u %u %u %u\n", 
//               appDataAttractiveLeds.leds_order[0], appDataAttractiveLeds.leds_order[1], 
//               appDataAttractiveLeds.leds_order[2], appDataAttractiveLeds.leds_order[3]);
//#endif   
//    }
    
    if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) ) 
    {
        errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE)
        printf( "\tWarning:  unable to close UDIDLEDS.TXT (%u).\n", errF);
#endif 
    }
    
    if (USB_DRIVE_MOUNTED == usbUnmountDrive())
    {
#if defined (USE_UART1_SERIAL_INTERFACE)
            printf( "\tWarning:  unable to unmount drive\n");
#endif 
    }
       
    return udidfound;
}

void setAttractiveLedsOff( void )
{
    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
       store_event(OF_ATTRACTIVE_LEDS_OFF); 
    }
    
    /* Disable PCA9622 device */
    PCA9622_OE_SetHigh( );

    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_OFF;
}

/* Set attractive LEDs color. */
void setAttractiveLedsOn( void )
{
    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
       store_event(OF_ATTRACTIVE_LEDS_ON); 
    }
    
    /* Enable PCA9622 device in Normal mode */
    PCA9622_OE_SetLow( ); // output enable pin is active LOW

    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_ON;
}

/* Set all color for attractive LEDs. */
void setAttractiveLedsNoColor( void )
{
    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
       store_event(OF_ATTRACTIVE_LEDS_NO_COLOR); 
    }
    
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

//void storeAttractiveLedsPattern( void )
//{
//    struct tm currentTime;
//    getDateTime(&currentTime);
//   
//    appDataAttractiveLeds.patterns[4*appDataAttractiveLeds.num_patterns_stored] = appDataAttractiveLeds.pattern[0];    
//    appDataAttractiveLeds.patterns[4*appDataAttractiveLeds.num_patterns_stored+1] = appDataAttractiveLeds.pattern[1];
//    appDataAttractiveLeds.patterns[4*appDataAttractiveLeds.num_patterns_stored+2] = appDataAttractiveLeds.pattern[2];
//    appDataAttractiveLeds.patterns[4*appDataAttractiveLeds.num_patterns_stored+3] = appDataAttractiveLeds.pattern[3];
//    appDataAttractiveLeds.hours[appDataAttractiveLeds.num_patterns_stored] = currentTime.tm_hour;
//    appDataAttractiveLeds.minutes[appDataAttractiveLeds.num_patterns_stored] = currentTime.tm_min;
//    appDataAttractiveLeds.seconds[appDataAttractiveLeds.num_patterns_stored] = currentTime.tm_sec;
//    
//    appDataAttractiveLeds.num_patterns_stored +=1;
//}

void testAttractiveLeds( void )
{
    I2C1_MESSAGE_STATUS i2c_status = I2C1_MESSAGE_COMPLETE; // the status of write data on I2C bus
    uint8_t writeBuffer[2]; // data to transmit
    uint16_t delay = 500;

    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
        store_event(OF_ATTRACTIVE_LEDS_TEST);
    }
    
#if defined ( USE_UART1_SERIAL_INTERFACE )
    printf( "\tTest attractive LEDs\n" );
#endif
                        
    setAttractiveLedsOn( );
    setAttractiveLedsNoColor( );
    
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
    writeBuffer[1] = 55; // PWM0 Individual Duty Cycle for LED_RGB2_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
    
    setDelayMs( delay );
    while ( 0 == isDelayMsEnding( ) );
    
    writeBuffer[0] = CTRLREG_PWM3;
    writeBuffer[1] = 0; // PWM0 Individual Duty Cycle for LED_RGB2_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
    
    writeBuffer[0] = CTRLREG_PWM6;
    writeBuffer[1] = 55; // PWM0 Individual Duty Cycle for LED_RGB3_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
    
    setDelayMs( delay );
    while ( 0 == isDelayMsEnding( ) );
    
    writeBuffer[0] = CTRLREG_PWM6;
    writeBuffer[1] = 0; // PWM0 Individual Duty Cycle for LED_RGB3_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
    
    writeBuffer[0] = CTRLREG_PWM9;
    writeBuffer[1] = 55; // PWM0 Individual Duty Cycle for LED_RGB4_R
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
    
    setDelayMs( delay );
    while ( 0 == isDelayMsEnding( ) );
    
    appDataAttractiveLeds.status = ATTRACTIVE_LEDS_OFF;
    
    setAttractiveLedsNoColor( );
    setAttractiveLedsOff( );
    
    
}

void testAttractiveLedsOrder( void )
{
    int i;
    
    setAttractiveLedsOn( );
    setAttractiveLedsNoColor( );

    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
        store_event( OF_ATTRACTIVE_LEDS_CHECK_INDEX );
    }

#if defined ( USE_UART1_SERIAL_INTERFACE )
    printf( "\tTest attractive LEDs order\n" );
#endif
    
    for ( i = 0; i < 4; i++ )
    {
        setOneAttractiveLedColor( appDataAttractiveLeds.leds_index[i], 0, 35, 0 );
        setDelayMs( 1000 );
        while ( 0 == isDelayMsEnding( ) );
    }
    setAttractiveLedsNoColor( );
    setAttractiveLedsOff( );
}
/*******************************************************************************
 End of File
 */
