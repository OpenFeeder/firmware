/*
  DS3231 library for the PIC24.

  This library implements the following features:

   - read/write of current time
   - TODO: both of the alarms, control/status registers, aging register
   - TODO: read of the temperature register, and of any address from the chip.

  Author:          Arnauld Biganzoli <arnauld.biganzoli@gmail.com>
  Available from:  https://github.com/OpenFeeder/firmwares
 
  The DS3231 is a low-cost, extremely accurate I2C real-time clock 
  (RTC) with an integrated temperature-compensated crystal oscillator 
  (TCXO) and crystal.
  https://www.maximintegrated.com/en/products/digital/real-time-clocks/DS3231.html

 */

#include <xc.h>
#include <stdio.h>
#include "app.h"
#include "ds3231.h"

/* control register 0Eh/8Eh
bit7 EOSC   Enable Oscillator (1 if oscillator must be stopped when on battery)
bit6 BBSQW  Battery Backed Square Wave
bit5 CONV   Convert temperature (1 forces a conversion NOW)
bit4 RS2    Rate select - frequency of square wave output
bit3 RS1    Rate select
bit2 INTCN  Interrupt control (1 for use of the alarms and to disable square wave)
bit1 A2IE   Alarm2 interrupt enable (1 to enable)
bit0 A1IE   Alarm1 interrupt enable (1 to enable)
 */

I2C1_MESSAGE_STATUS DS3231_time_get( struct ts *t )
{
    I2C1_MESSAGE_STATUS status;
    uint8_t register_value[7];
    uint8_t *p_data_read;
    uint8_t TimeDate[7]; // second, minute, hour, dow, day, month, year
    uint8_t century = 0;
    uint8_t i, n;
    uint16_t year_full;

    // write the register address before reading it
    I2C1_MasterWrite( DS3231_TIME_CAL_ADDR, 1, DS3231_I2C_ADDR, &status );
    // wait for the message to be sent or status has changed.
    while ( status == I2C1_MESSAGE_PENDING )
    {
        Nop( ); // without pull-up resistor program will be blocked here
    }

    // if I2C1_MESSAGE_COMPLETE then we are ready to read data started at register address send before
    if ( status == I2C1_MESSAGE_COMPLETE )
    {
        p_data_read = register_value;
        // this portion will read data byte from the register address location
        I2C1_MasterRead( p_data_read, 7, DS3231_I2C_ADDR, &status );

        // wait for the message to be sent or status has changed.
        while ( status == I2C1_MESSAGE_PENDING )
        {
            Nop( ); // without pull-up resistor program will be blocked here
        }

        if (status != I2C1_MESSAGE_FAIL)
        {
            for ( i = 0; i <= 6; i++ )
            {
                n = register_value[i];
                if ( i == 5 )
                {
                    TimeDate[5] = bcdtodec( n & 0x1F );
                    century = ( n & 0x80 ) >> 7;
                }
                else
                {
                    TimeDate[i] = bcdtodec( n );
                }
            }

            if ( century == 1 )
            {
                year_full = 2000 + TimeDate[6];
            }
            else
            {
                year_full = 1900 + TimeDate[6];
            }

            t->sec = TimeDate[0];
            t->min = TimeDate[1];
            t->hour = TimeDate[2];
            t->mday = TimeDate[4];
            t->mon = TimeDate[5];
            t->year = year_full;
            t->wday = TimeDate[3];
            t->year_s = TimeDate[6];
        }
    }

    return status;
} /* End of DS3231_time_get( ) */

I2C1_MESSAGE_STATUS DS3231_time_set( struct ts *t )
{
    I2C1_MESSAGE_STATUS status;
    uint8_t century;
    uint8_t TimeDate[8];

    TimeDate[0] = DS3231_TIME_CAL_ADDR;
    TimeDate[1] = dectobcd( t->sec );
    TimeDate[2] = dectobcd( t->min );
    TimeDate[3] = dectobcd( t->hour );
    TimeDate[4] = dectobcd( t->wday );
    TimeDate[5] = dectobcd( t->mday );
    century = 0x80; // t->year >= 2000
    TimeDate[6] = dectobcd( t->mon ) + century;
    t->year_s = t->year;
    TimeDate[7] = dectobcd( t->year_s );

    I2C1_MasterWrite( TimeDate, 8, DS3231_I2C_ADDR, &status );
    // wait for the message to be sent or status has changed.
    while ( status == I2C1_MESSAGE_PENDING )
    {
        Nop( ); // without pull-up resistor program will be blocked here
    }

    return status;
} /* End of DS3231_time_set( ) */

I2C1_MESSAGE_STATUS getDS3231Temperature( )
{
    I2C1_MESSAGE_STATUS status;
    uint8_t write_buffer[2] = {0, 0};
    uint8_t register_value[7] = {0, 0, 0, 0, 0, 0, 0};
    uint8_t *p_data_read;
        
    // write the register address before reading it
    write_buffer[0] = DS3231_CONTROL_ADDR;
    I2C1_MasterWrite( write_buffer, 1, DS3231_I2C_ADDR, &status );    
    // wait for the message to be sent or status has changed.
    while ( status == I2C1_MESSAGE_PENDING )
    {
        Nop( ); // without pull-up resistor program will be blocked here
    }
    if ( status != I2C1_MESSAGE_COMPLETE )
    {
        return status;
    }
    
    p_data_read = register_value;
    I2C1_MasterRead( p_data_read, 1, DS3231_I2C_ADDR, &status );
    // wait for the message to be sent or status has changed.
    while ( status == I2C1_MESSAGE_PENDING )
    {
        Nop( ); // without pull-up resistor program will be blocked here
    }
    if ( status != I2C1_MESSAGE_COMPLETE )
    {
        return status;
    }
    
    write_buffer[0] = DS3231_CONTROL_ADDR;
    write_buffer[1] = register_value[0] | 0x01<<5;
    
    I2C1_MasterWrite( write_buffer, 2, DS3231_I2C_ADDR, &status );
    // wait for the message to be sent or status has changed.
    while ( status == I2C1_MESSAGE_PENDING )
    {
        Nop( ); // without pull-up resistor program will be blocked here
    }
    if ( status != I2C1_MESSAGE_COMPLETE )
    {
        return status;
    }
    
    write_buffer[0] = DS3231_TEMPERATURE_ADDR;
    I2C1_MasterWrite( write_buffer, 1, DS3231_I2C_ADDR, &status );
    // wait for the message to be sent or status has changed.
    while ( status == I2C1_MESSAGE_PENDING )
    {
        Nop( ); // without pull-up resistor program will be blocked here
    }
    if ( status != I2C1_MESSAGE_COMPLETE )
    {
        return status;
    }
    
    memset(register_value, 0, 7);
    
    p_data_read = register_value;
    I2C1_MasterRead( p_data_read, 2, DS3231_I2C_ADDR, &status );
    // wait for the message to be sent or status has changed.
    while ( status == I2C1_MESSAGE_PENDING )
    {
        Nop( ); // without pull-up resistor program will be blocked here
    }
    if ( status != I2C1_MESSAGE_COMPLETE )
    {
        return status;
    }
   
    if (register_value[0]>>7)
    {
        appData.ext_temperature = -1.0*((float)((register_value[0]-1)^255)-(float)(register_value[1]>>7)*0.5-(float)(register_value[1]>>6 & 0x01)*0.25);
    }
    else
    {
        appData.ext_temperature = (float)register_value[0] + (float)(register_value[1]>>7)*0.5 + (float)(register_value[1]>>6 & 0x01)*0.25;
    }

    return status;
    
} /* End of DS3231_temperature_get( ) */

/* Helpers */

uint8_t dectobcd( const uint8_t val )
{
    return (( val / 10 * 16 ) + ( val % 10 ) );
}

uint8_t bcdtodec( const uint8_t val )
{
    return (( val / 16 * 10 ) + ( val % 16 ) );
}


/*******************************************************************************
 End of File
 */
