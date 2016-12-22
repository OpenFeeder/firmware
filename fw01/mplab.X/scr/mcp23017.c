/*
 * File:   mcp23017.c
 * Author: Arnauld Biganzoli
 * Created on 31 octobre 2015
 */

#include "app.h"
#include "mcp23017.h"


I2C1_MESSAGE_STATUS I2C1_MasterWriteMCP23017( const uint16_t addr7bits,
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


I2C1_MESSAGE_STATUS I2C1_MasterReadMCP23017( const uint16_t addr7bits,
                                             const uint8_t register_addr,
                                             uint8_t *p_data_read,
                                             const uint8_t number_of_data_to_read )
{
    I2C1_MESSAGE_STATUS status;
    uint8_t register_value[1];

    register_value[0] = register_addr; // charge register address in write buffer
    I2C1_MasterWrite( register_value, 1, addr7bits, &status ); // write the register address before reading it
    // wait for the message to be sent or status has changed.
    while ( status == I2C1_MESSAGE_PENDING )
    {
        Nop( ); // without pull-up resistor program will be blocked here
    }

    // if I2C1_MESSAGE_COMPLETE then we are ready to read data started at register address send before
    if ( status == I2C1_MESSAGE_COMPLETE )
    {
        // this portion will read data byte from the register address location
        I2C1_MasterRead( p_data_read, number_of_data_to_read, addr7bits, &status );

        // wait for the message to be sent or status has changed.
        while ( status == I2C1_MESSAGE_PENDING )
        {
            Nop( ); // without pull-up resistor program will be blocked here
        }
    }

    return status;
}


/*******************************************************************************
 End of File
 */
