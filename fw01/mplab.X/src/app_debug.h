/**
 * @file app_debug.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 06/07/2016
 */

#ifndef APP_DEBUG_H
#define APP_DEBUG_H

/**
 * Section: Global Variable Definitions
 */
#define UART1_BUFFER_SIZE  4

void displayBootMessage( void );
void APP_SerialDebugTasks( void );
void printUSBHostDeviceStatus( void );
uint16_t readIntFromUart1( void );


#endif /* APP_DEBUG_H */


/*******************************************************************************
 End of File
 */
