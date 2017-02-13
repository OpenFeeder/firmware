/*
 * File:   pca9622.c
 * Author: Arnauld Biganzoli
 * Created 10/02/2017
 */

#include "app.h"
#include "pca9622.h"

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


/*******************************************************************************
 End of File
 */
