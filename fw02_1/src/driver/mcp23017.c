/*
 * File:   mcp23017.c
 * Author: Arnauld Biganzoli
 * Created on 31 octobre 2015
 */

#include "app.h"
#include "mcp23017.h"
// C Routine for Hex to ASCII with simple lookup array :
const char hex2digit[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

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

void i2cScanner( void )
{
    uint16_t addr7bits = 0; // variable 7-bits slave address for scan I2C bus
    uint8_t device_found = 0; // number of I2C device found on the bus
//    uint8_t device_lost = 0; // count number of I2C device not found on the bus
    I2C1_MESSAGE_STATUS status; // generic I2C status return of MCC functions
    uint8_t writeBuffer[2];

    printf("\tI2C bus scan started. 7 bit address used.\n");
        
    // main loop to scan address from 0x00, 0x02 ... to 0x7F
    while ( addr7bits < 0x80 )
    {
        status = I2C1_MESSAGE_PENDING;

        // write only address byte on bus (0 is the count of data byte to write, here there is no data to transfer)
        I2C1_MasterWrite( writeBuffer, 0, addr7bits, &status ); // write address on I2C bus

        // wait for the message to be sent or status has changed
        while ( status == I2C1_MESSAGE_PENDING );

        if ( status == I2C1_MESSAGE_COMPLETE )
        {
            uint8_t addr8bits = addr7bits << 1; // convert 7-bits address to 8-bits format (Byte format)
            
            // print address in hexa format :
            printf("\t\tSlave found. 7 bit address = 0x%02X; 8 bit address = 0x%02X\n", addr7bits, addr8bits);

            ++device_found; // inc. device_found +1
        }
//        else
//        {
////            printf( "." );
//            ++device_lost;
//        }
        ++addr7bits; // inc. device addr +1
    }

    printf("\tI2C bus scan finished\n");
//    printf( "\nDone!\n\n" );

//    if ( device_found )
//    {
//        printf( "\t%d I2C device found, and %d not match.\n\n", device_found, device_lost );
//    }
//    else
//    {
//        printf( "\tNo I2C device found!\n\n" );
//    }
}


/*******************************************************************************
 End of File
 */
