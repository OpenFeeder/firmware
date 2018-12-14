/**
 * @file app_debug.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 06/07/2016
 */

#ifndef _APP_DEBUG_HEADER_H
#define _APP_DEBUG_HEADER_H

/**
 * Section: Global Variable Definitions
 */
#define UART1_BUFFER_SIZE  4
#define MAX_READ_FROM_UART_DELAY 3000

void displayBootMessage( void );
void displayResetRegisters( void );
void APP_SerialDebugTasks( void );
void printUSBHostDeviceStatus( void );
int16_t readIntFromUart1( void );
void getDeviceId( void );
void displayDeviceId( void );
void getUniqueDeviceId( void );
void displayUniqueDeviceId( void );
uint8_t getCompletScenarioNumber( void );
void displayKeyMapping( void );
void displayBuildDateTime( void );
void displayFirmwareVersion( void );
    
#endif /* _APP_DEBUG_HEADER_H */


/*******************************************************************************
 End of File
 */
