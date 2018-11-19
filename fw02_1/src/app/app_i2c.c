/**
 * @file app_i2c.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 06/11/2015
 */

#include "app.h"
#include <string.h>
#include "app_i2c.h"

extern const char bin2ascii_tab[];

void print_I2C_message_status( I2C1_MESSAGE_STATUS status )
{
#if defined (USE_UART1_SERIAL_INTERFACE)
    switch ( status )
    {
        case I2C1_MESSAGE_FAIL:
            printf( "\tI2C1_MESSAGE_FAIL" );
            break;

        case I2C1_MESSAGE_PENDING:
            printf( "\tI2C1_MESSAGE_PENDING" );
            break;

        case I2C1_MESSAGE_COMPLETE:
            printf( "\tI2C1_MESSAGE_COMPLETE" );
            break;

        case I2C1_STUCK_START:
            printf( "\tI2C1_STUCK_START" );
            break;

        case I2C1_MESSAGE_ADDRESS_NO_ACK:
            printf( "\tI2C1_MESSAGE_ADDRESS_NO_ACK" );
            break;

        case I2C1_DATA_NO_ACK:
            printf( "\tI2C1_DATA_NO_ACK" );
            break;

        case I2C1_LOST_STATE:
            printf( "\tI2C1_LOST_STATE" );
            break;

        default:
            // if nothing else matches, do the default
            printf( "\tUnknown I2C status" );
            break;
    }
#endif
}

/* Research slaves components present on the I2C bus. */
uint8_t APP_I2CMasterSeeksSlaveDevice( uint16_t addr7bits_start, uint16_t addr7bits_stop )
{
    I2C1_MESSAGE_STATUS status;
    uint16_t addr7bits = addr7bits_start; // 7-bits slave address scan
    uint8_t writeBuffer[] = { 0 }; // no data to transmit
    uint8_t device_found = 0;
    uint8_t device_lost = 0;

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_I2C_SCANNER)
    printf( "Scan for 128 I2C bus address from 0x00, 0x02 ... to 0x7F\n" );
#endif
    memset( appData.i2c_add_found, 0, MAX_OF_UNKNOWN_I2C_8_BIT_SLAVE_ADD ); // clear i2c_add_found tab

    while ( addr7bits <= addr7bits_stop )
    {
        // write only address byte on bus (0 is the count of data byte to write), from 0x00, 0x02 ... to 0x7F (7-bit addr 0x7F --> 0xFE 8-bit addr)
        I2C1_MasterWrite( writeBuffer, 0, addr7bits, &status );
        // wait for the message to be sent or status has changed.
        while ( status == I2C1_MESSAGE_PENDING )
        {
            Nop( ); // without pull-up resistor program will be blocked here
        }

        /* If one device found, then save and display is address. */
        if ( status == I2C1_MESSAGE_COMPLETE )
        {
            uint8_t addr8bits = addr7bits << 1; // convert 7-bits address to 8-bits format (Byte format)
            appData.i2c_add_found[device_found] = addr8bits;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_I2C_SCANNER)
            printf( "\r\nFound 8 bit Add: 0x%c%c (hex)", bin2ascii_tab[addr8bits >> 4], bin2ascii_tab[addr8bits & 0x0F] ); // print address in hexa format
#endif
            ++device_found; // inc. device_found +1
        }
        else
        {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_I2C_SCANNER)
            //printf( "." ); // or
            putchar( '.' );
#endif
            ++device_lost;
        }
        ++addr7bits; // inc. device addr +1
    }
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_I2C_SCANNER)
    printf( "\r\nSearch done.\r\n\n" );

    if ( device_found )
    {
        I2C_ADD first_i2c_device;
        printf( "%d I2C device found, and %d not match.\r\n", device_found, device_lost );
        first_i2c_device.add8bit = appData.i2c_add_found[0];
        printf( "First 7-bit i2c address found: 0x%c%c\r\n", bin2ascii_tab[first_i2c_device.add7bit >> 4], bin2ascii_tab[first_i2c_device.add7bit & 0x0F] );
    }
    else
    {
        printf( "No I2C device found!\r\n\n" );
    }
#endif

    return device_found; // sizeof (appData.i2c_add_found) --> 255, soit MAX_OF_UNKNOWN_I2C_8_BIT_SLAVE_ADD

} /* End of APP_I2CMasterSeeksSlaveDevice() */

/* Test Remote Control present on the I2C bus. */
bool APP_isRemoteControlConnected( void )
{
    I2C1_MESSAGE_STATUS status;
    uint8_t writeBuffer[] = { 0 }; // no data to transmit

    //#if defined (DEBUG_UART)
    //    printf( "Scan I2C bus for Remote Control present.\n" );
    //#endif
    /* Write only address byte on bus (0 is the count of data byte to write) */
    I2C1_MasterWrite( writeBuffer, 0, MCP23017_ADDRESS, &status );
    /* Wait for the message to be sent or status has changed. */
    while ( status == I2C1_MESSAGE_PENDING )
    {
        Nop( ); // without pull-up resistor program will be blocked here
    }

    /* If MCP23017 device found, then return true. */
    if ( status == I2C1_MESSAGE_COMPLETE )
    {
        //#if defined (DEBUG_UART)
        //    printf( "Remote Control found.\n" );
        //#endif
        return true;
    }

    //#if defined (DEBUG_UART)
    //    printf( "Remote Control not found!\n" );
    //#endif
    return false;

} /* End of APP_isRemoteControlConnected() */

/* Call every 3 ms by _T2Interrupt( ). */
I2C1_MESSAGE_STATUS APP_MultiplexingLEDsTasks( void )
{
    I2C1_MESSAGE_STATUS i2c_status = I2C1_MESSAGE_COMPLETE; // the status of write data on I2C bus
    uint8_t writeBuffer[3]; // data to transmit
    //    static uint8_t indice = 0; // digit indice

    //    LED_STATUS_B_SetHigh();
    // Considering that this is the first communication with the MCP23017
    // CONTROL REGISTER IOCON.BANK = 0 by default
    // --> testing the communication with the MCP23017, if ok then initializes
    //     the direction of PORT before lighting the LEDs on the digit 1
    if ( appData.mcp23017.status_bit.initialized )
    {
        // Load LEDs register in MCP23017
        writeBuffer[0] = MCP23017_OLATA; // MCP23017 OLATA Register

        // FIXME: Pb - on observe l'affichage du prochain digit éclairé faiblement !!!
        //        writeBuffer[1] = 0b11111111; // ceci ne permet pas de régler le pb d'affichage !
        //        writeBuffer[2] = ~appData.mcp23017.status_bit.cmd_digits; // MCP23017 OLATB Register
        //        i2c_status = I2C1_MasterWriteMCP23017( MCP23017_ADDRESS, writeBuffer, 3 ); // clear data on PORTA
        // Change command Digit to switch betwin the 4 digits to display
        appData.mcp23017.status_bit.cmd_digits <<= 1;
        //if (( appData.mcp23017.status_bit.cmd_digits != DIGIT_1 ) && ( appData.mcp23017.status_bit.cmd_digits != DIGIT_2 ) && ( appData.mcp23017.status_bit.cmd_digits != DIGIT_3 ) && ( appData.mcp23017.status_bit.cmd_digits != DIGIT_4 )) // do not solve multiplexing pb !
        if ( appData.mcp23017.status_bit.cmd_digits == 0 )
        {
            appData.mcp23017.status_bit.cmd_digits = DIGIT_1;
        }
        // putchar( appData.mcp23017.status_bit.cmd_digits ); // display: 01 02 04 08 01 02 04 08 ...

        //        // Change command Digit to switch betwin the 4 digits to display
        //        appData.mcp23017.status_bit.cmd_digits <<= 1;
        //        //if (( appData.mcp23017.status_bit.cmd_digits != DIGIT_1 ) && ( appData.mcp23017.status_bit.cmd_digits != DIGIT_2 ) && ( appData.mcp23017.status_bit.cmd_digits != DIGIT_3 ) && ( appData.mcp23017.status_bit.cmd_digits != DIGIT_4 )) // do not solve multiplexing pb !
        //        if ( appData.mcp23017.status_bit.cmd_digits == 0 )
        //        {
        //            appData.mcp23017.status_bit.cmd_digits = DIGIT_1;
        //        }
        // appData.mcp23017.status_bit.cmd_digits = ( appData.mcp23017.status_bit.cmd_digits == DIGIT_4 ) ? DIGIT_1 : appData.mcp23017.status_bit.cmd_digits << 1; // do not work (all digit off), maybe appData.mcp23017.status_bit.cmd_digits == 0 !

        // Load 7 segment data from memory in writeBuffer[1] before trasmit on I2C
        switch ( appData.mcp23017.status_bit.cmd_digits )
        {
                //            case DIGIT_1:
            case DIGIT_4:
                if ( ( blinkDigitEnable == true ) && ( blinkDigitState == OFF ) && ( blinkDigit == 0 ) )
                {
                    writeBuffer[1] = 0b11111111; // clear digit if blink
                }
                else
                {
                    writeBuffer[1] = appData.digit[0]; // get data to display from memory
                }
                break;

                //            case DIGIT_2:
            case DIGIT_3:
                if ( ( blinkDigitEnable == true ) && ( blinkDigitState == OFF ) && ( blinkDigit == 1 ) )
                {
                    writeBuffer[1] = 0b11111111; // clear digit if blink
                }
                else
                {
                    writeBuffer[1] = appData.digit[1]; // get data to display from memory
                }
                break;

                //            case DIGIT_3:
            case DIGIT_2:
                if ( ( blinkDigitEnable == true ) && ( blinkDigitState == OFF ) && ( blinkDigit == 2 ) )
                {
                    writeBuffer[1] = 0b11111111; // clear digit if blink
                }
                else
                {
                    writeBuffer[1] = appData.digit[2]; // get data to display from memory
                }
                break;

                //            case DIGIT_4:
            case DIGIT_1:
                if ( ( blinkDigitEnable == true ) && ( blinkDigitState == OFF ) && ( blinkDigit == 3 ) )
                {
                    writeBuffer[1] = 0b11111111; // clear digit if blink
                }
                else
                {
                    writeBuffer[1] = appData.digit[3]; // get data to display from memory
                }
                break;

            default:
                // if nothing else matches, do the default
                // default is optional
                printf( "DIGIT_?\n" );
                break;
        }

        // Load digits command for display only one digit at a time
        // Command individual 7 segments digits 1 to 4 :
        //  with Common Cathode (CC) and MOSFET N: 0b0001; 0b0010; 0b0100; 0b1000;
        // or with Common Anode (CA) and MOSFET P: 0b1110; 0b1101; 0b1011; 0b0111;
        writeBuffer[2] = ~appData.mcp23017.status_bit.cmd_digits; // MCP23017 OLATB Register
        // '~' inversion des bits du multiplexage pour la commande des afficheurs en Common Cathode (CC)
        // try other way for identifi multiplexing pb, with no result :        
        //        if ( appData.mcp23017.status_bit.cmd_digits == DIGIT_4 )
        //        {
        //            writeBuffer[2] = 0b11110111;
        //        }
        //        if ( appData.mcp23017.status_bit.cmd_digits == DIGIT_3 )
        //        {
        //            writeBuffer[2] = 0b11111011;
        //        }
        //        if ( appData.mcp23017.status_bit.cmd_digits == DIGIT_2 )
        //        {
        //            writeBuffer[2] = 0b11111101;
        //        }
        //        if ( appData.mcp23017.status_bit.cmd_digits == DIGIT_1 )
        //        {
        //            writeBuffer[2] = 0b11111110;
        //        }

        i2c_status = I2C1_MasterWriteMCP23017( MCP23017_ADDRESS, writeBuffer, 3 ); // write data on PORTA
    }
    else
    {
        //        printf( "\nSend initialize to MCP23017 device...\r\n" ); // FIXME: suppression de ce message car trop d'info sur le terminal
        i2c_status = I2C1_MasterWriteMCP23017( MCP23017_ADDRESS, writeBuffer, 0 );
        if ( i2c_status == I2C1_MESSAGE_COMPLETE )
        {
            // Set all PORTA direction as OUTPUT
            writeBuffer[0] = MCP23017_IODIRA; // initialize pointer register with IODIRA
            writeBuffer[1] = 0b00000000; // configure PORTA pin direction in IODIRA register
            // Set PORTB direction as PORTB<7:4> = INPUT and PORTB<3:0> = OUTPUT
            writeBuffer[2] = 0b11110000; // configure PORTB pin direction in IODIRB register
            i2c_status = I2C1_MasterWriteMCP23017( MCP23017_ADDRESS, writeBuffer, 3 );
            appData.mcp23017.status_bit.initialized = 1; // set MCP23017 initialized bit status
            //            printf( "done\r\n\n" );
        }
    }

    return i2c_status;

} /* End of APP_MultiplexingLEDsTasks() */

/**
 * Call this function every 100 ms minimum to refresh the buttons status periodically
 * Update status of push buton with BUTTON_PRESSED state if pressed,
 * the user must clear the state after reading it whith BUTTON_NOT_PRESSED state
 */
I2C1_MESSAGE_STATUS APP_ScanningPushButtonTasks( void )
{
    I2C1_MESSAGE_STATUS status = I2C1_MESSAGE_COMPLETE;
    static I2C1_MESSAGE_STATUS last_status = I2C1_MESSAGE_COMPLETE;
    uint8_t readBuffer[1]; // data buffer

    if ( ( appData.button_pressed == BUTTON_NONE ) || ( appData.button_pressed == BUTTON_READ ) )
    {
        status = I2C1_MasterReadMCP23017( MCP23017_ADDRESS, MCP23017_GPIOB, readBuffer, 1 );

        if ( status == I2C1_MESSAGE_COMPLETE )
        {
            last_status = status;
            appData.button_pressed = BUTTON_NONE;

            /* BUTTON_DOWN == BUTTON_PRESSED ? */
            if ( ( readBuffer[0] & BUTTON_DOWN_MASK ) == BUTTON_PRESSED )
            {
                appData.button_pressed = BUTTON_DOWN;
            }

            /* BUTTON_UP == BUTTON_PRESSED ? */
            if ( ( readBuffer[0] & BUTTON_UP_MASK ) == BUTTON_PRESSED )
            {
                appData.button_pressed = BUTTON_UP;
            }

            /* BUTTON_RIGHT == BUTTON_PRESSED ? */
            if ( ( readBuffer[0] & BUTTON_RIGHT_MASK ) == BUTTON_PRESSED )
            {
                appData.button_pressed = BUTTON_RIGHT;
            }

            /* BUTTON_LEFT == BUTTON_PRESSED ? */
            if ( ( readBuffer[0] & BUTTON_LEFT_MASK ) == BUTTON_PRESSED )
            {
                appData.button_pressed = BUTTON_LEFT;
            }
        }
        else
        {
            if ( last_status != status )
            {
                last_status = status;
                printf( "\nError reading I2C interface !\n\n" );
            }
        }
    }

    return status;
} /* End of APP_ScanningPushButtonTasks() */

bool getExtDateTime( void )
{
    I2C1_MESSAGE_STATUS status;
    int seconds;
    
    if (0 == APP_I2CMasterSeeksSlaveDevice(DS3231_I2C_ADDR, DS3231_I2C_ADDR))
    {
        appData.i2c_current_time.year = 0;
        appData.i2c_current_time.year_s = 0;
        appData.i2c_current_time.mon = 0;
        appData.i2c_current_time.mday = 0;
        appData.i2c_current_time.hour = 0;
        appData.i2c_current_time.min = 0;
        appData.i2c_current_time.sec = 0;

        return false;
    }
    
    status = DS3231_time_get( &appData.i2c_current_time );
    
    if ( status == I2C1_MESSAGE_FAIL )
    {
        appData.i2c_current_time.year = 0;
        appData.i2c_current_time.year_s = 0;
        appData.i2c_current_time.mon = 0;
        appData.i2c_current_time.mday = 0;
        appData.i2c_current_time.hour = 0;
        appData.i2c_current_time.min = 0;
        appData.i2c_current_time.sec = 0;

        return false;
    }
    
    seconds = appData.i2c_current_time.sec;
    
    while ( seconds == appData.i2c_current_time.sec )
    {
        status = DS3231_time_get( &appData.i2c_current_time ); 
        if ( status == I2C1_MESSAGE_FAIL )
        {
            appData.i2c_current_time.year = 0;
            appData.i2c_current_time.year_s = 0;
            appData.i2c_current_time.mon = 0;
            appData.i2c_current_time.mday = 0;
            appData.i2c_current_time.hour = 0;
            appData.i2c_current_time.min = 0;
            appData.i2c_current_time.sec = 0;

            return false;
        }
    }

    if ( status == I2C1_MESSAGE_COMPLETE )
    {
        return true;
    }
    else
    {
        appData.i2c_current_time.year = 0;
        appData.i2c_current_time.year_s = 0;
        appData.i2c_current_time.mon = 0;
        appData.i2c_current_time.mday = 0;
        appData.i2c_current_time.hour = 0;
        appData.i2c_current_time.min = 0;
        appData.i2c_current_time.sec = 0;

        return false;
    }
}

/* Dynamic configuration date, example 22/08/2016 and time to 15:59:30 */
// setDateTime( 16, 8, 22, 15, 59, 30 ); /* Set date and time. */

bool setExtDateTime( uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second ) /* Set date and time. */
{
    struct ts t;

    if (0 == APP_I2CMasterSeeksSlaveDevice(DS3231_I2C_ADDR, DS3231_I2C_ADDR))
    {
        appData.i2c_current_time.year = 0;
        appData.i2c_current_time.year_s = 0;
        appData.i2c_current_time.mon = 0;
        appData.i2c_current_time.mday = 0;
        appData.i2c_current_time.hour = 0;
        appData.i2c_current_time.min = 0;
        appData.i2c_current_time.sec = 0;

        return false;
    }
    
    t.year = year;
    t.mon = month;
    t.mday = day;
    t.hour = hour;
    t.min = minute;
    t.sec = second;

    if ( I2C1_MESSAGE_COMPLETE == DS3231_time_set( &t ) )
    {
        getExtDateTime( );
        return true;
    }
    else
    {
      return false;  
    }
   
}

void printExtDateTime( void )
{
    
    if (0 == APP_I2CMasterSeeksSlaveDevice(DS3231_I2C_ADDR, DS3231_I2C_ADDR))
    {
        printf( "--/--/---- --:--:--" );
    }
    else if (0 == appData.i2c_current_time.mday && 0 == appData.i2c_current_time.mon && 0 == appData.i2c_current_time.year_s)
    {
        printf( "--/--/---- --:--:--" );
    }
    else
    {
        printf( "%02u/%02u/20%02u %02u:%02u:%02u", 
           appData.i2c_current_time.mday, 
           appData.i2c_current_time.mon, 
           appData.i2c_current_time.year_s, 
           appData.i2c_current_time.hour, 
           appData.i2c_current_time.min, 
           appData.i2c_current_time.sec ); // I2C RTC
    }

}

/*******************************************************************************
 End of File
 */
