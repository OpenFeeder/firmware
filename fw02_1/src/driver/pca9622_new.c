/*
 * File:   pca9622.c
 * Author: Arnauld Biganzoli
 * Created 10/02/2017
 */

#include "app.h"
#include "pca9622.h"

//// pose le probleme de l'utilisation des 2 bus en même temps
//#ifdef PCA9622_USE_I2C1
//#define PCA9622_Write I2C1_MasterWrite
//#else
//#define PCA9622_Write I2C2_MasterWrite
//#endif



I2C1_MESSAGE_STATUS I2C1_MasterWritePCA9622( const uint16_t addr7bits,
                                             uint8_t *p_data,
                                             const uint8_t data_len )
{
    I2C1_MESSAGE_STATUS status;

    I2C1_MasterWrite( p_data, data_len, addr7bits, &status );

    // wait for the message to be sent or status has changed.
    while ( status == I2C1_MESSAGE_PENDING )
    {
        Nop( ); // without pull-up resistor program will be blocked here
    }

    return status;
}

I2C1_MESSAGE_STATUS I2C1_PCA9622_Initialize( const uint16_t addr7bits, uint8_t MODE_LED0, uint8_t MODE_LED1, ... MODE_LED15)
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
    writeBuffer[1] = MODE_LED0 || MODE_LED1 || MODE_LED2 || MODE_LED3; // CTRLREG PWM on all output for LEDOUT0
//    writeBuffer[1] = 0b10101010; // CTRLREG PWM on all output for LEDOUT0
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
    writeBuffer[4] = 0b00101010; // CTRLREG PWM on all output for LEDOUT3, except LDR15 off
    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
    if ( I2C1_MESSAGE_COMPLETE != i2c_status )
    {
        return false;
        //printf( "Attractive LEDs initialize failed!\n" );
    }

    return true;
    //printf( "Attractive LEDs initialized.\n" );
}


/******************************* Start App ************************************/
/* Initialize LED0 to LED14 output */
bool initAttractiveLeds( void )
{
    I2C1_MESSAGE_STATUS i2c_status = I2C1_MESSAGE_COMPLETE; // the status of write data on I2C bus
    uint8_t writeBuffer[5]; // data to transmit

    /* Initialize PCA9622 device in Normal mode. */
    i2c_status = I2C1_PCA9622_Initialize( PCA9622_ADDRESS, MODE_LED0, ... MODE_LED15);


    if ( I2C1_MESSAGE_COMPLETE != i2c_status )
    {
        return false;
    }
}
/******************************* End App **************************************/


I2C1_MESSAGE_STATUS I2C1_PCA9622_SoftwareReset( void )
{
    I2C1_MESSAGE_STATUS i2c_status = I2C1_MESSAGE_COMPLETE; // the status of write data on I2C bus
    uint8_t writeBuffer[] = { SWRST_DATA_1, SWRST_DATA_2 }; // data to transmit

    I2C1_MasterWrite( writeBuffer, 2, PCA9622_SWRST_ADR, &i2c_status );

    // wait for the message to be sent or status has changed.
    while ( i2c_status == I2C1_MESSAGE_PENDING )
    {
        Nop( ); // without pull-up resistor program will be blocked here
    }

#if defined (USE_UART1_SERIAL_INTERFACE)
    printf( "\tPCA9622 SWRST.\n" );
#endif

    return i2c_status;
}



/*******************************************************************************
 End of File
 */
