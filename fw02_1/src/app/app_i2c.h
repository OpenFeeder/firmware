/**
 * @file app_i2c.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 06/11/2015
 */

#ifndef _APP_I2C_HEADER_H
#define	_APP_I2C_HEADER_H

#include "mcp23017.h" // remote control
#include "pca9622.h"  // led driver
#include "ds3231.h"   // external real-time clock (RTC)


#define MAX_OF_UNKNOWN_I2C_8_BIT_SLAVE_ADD 255

typedef struct i2c_add_t
{

    union
    {
        uint8_t add8bit;

        struct
        {
            unsigned rw : 1;
            unsigned add7bit : 7;
        };
    };
} I2C_ADD;

void print_I2C_message_status( I2C1_MESSAGE_STATUS status );

/* Research slaves components present on the I2C bus. */
uint8_t APP_I2CMasterSeeksSlaveDevice( uint16_t addr7bits_start, uint16_t addr7bits_stop );

/* Test Remote Control present on the I2C bus. */
bool APP_isRemoteControlConnected( void );

/* Call every 3 ms by _T2Interrupt( ). */
I2C1_MESSAGE_STATUS APP_MultiplexingLEDsTasks( void );

/**
 * Call this function every 100 ms minimum to refresh the buttons status periodically
 * Update status of push buton with BUTTON_PRESSED state if pressed,
 * the user must clear the state after reading it whith BUTTON_NOT_PRESSED state
 */
I2C1_MESSAGE_STATUS APP_ScanningPushButtonTasks( void );

bool getExtDateTime( void );
bool setExtDateTime( uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second );
void printExtDateTime( void );

#endif	/* _APP_I2C_HEADER_H */


/*******************************************************************************
 End of File
 */
