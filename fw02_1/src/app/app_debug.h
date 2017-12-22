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

void displayBootMessage( void );
void displayResetRegisters( void );
void APP_SerialDebugTasks( void );
void printUSBHostDeviceStatus( void );
uint16_t readIntFromUart1( void );

///* Declaration of RCON1 flag. */
//typedef union
//{
//    uint16_t status_reg;
//
//    struct
//    {
//        unsigned POR : 1; /* */
//        unsigned BOR : 1; /* */
//        unsigned IDLE : 1; /* */
//        unsigned SLEEP : 1; /* */
//        unsigned WDTO : 1; /* */
//        unsigned SWDTEN : 1; /* */
//        unsigned SWR : 1; /* */
//        unsigned EXTR : 1; /* */
//        unsigned PMSLP : 1; /* */
//        unsigned CM : 1; /* */
//        unsigned DPSLP : 1; /* */
//        unsigned : 1; /* Unimplemented: */
//        unsigned RETEN : 1; /* */
//        unsigned : 1; /* Unimplemented: */
//        unsigned IOPUWR : 1; /* */
//        unsigned TRAPR : 1; /* */
//
//
//    } status_bit;
//} RESET_SYSTEM_CONTROL_REGISTER_1_t;
//
///* Declaration of RCON2 flag. */
//typedef union
//{
//    uint16_t status_reg;
//
//    struct
//    {
//        unsigned VBAT : 1; /* VBAT Flag bit */
//        unsigned VBPOR : 1; /* VBPOR Flag bit */
//        unsigned VDDPOR : 1; /* VDD Power-on Reset Flag bit */
//        unsigned VDDBOR : 1; /* VDD Brown-out Reset Flag bit */
//        unsigned : 4; /* Reserved */
//        unsigned : 8; /* Unimplemented */
//    } status_bit;
//} RESET_SYSTEM_CONTROL_REGISTER_2_t;

#endif /* _APP_DEBUG_HEADER_H */


/*******************************************************************************
 End of File
 */
