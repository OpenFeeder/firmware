/**
 * @file app_error.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 */

#ifndef APP_ERROR_H
#define APP_ERROR_H


#define ERROR_HIGH_PRIORITY 0
#define ERROR_LOW_PRIORITY 1

#include <stdio.h>
#include <stdint.h>

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

typedef struct
{
    char message[200]; // Error message buffer
    char currentFileName[200];
    int16_t currentLineNumber;
    //    uint8_t priority; // Message priority: 0 => high priority
    //                      //                   1 => low priority 

} APP_ERROR;


void printError( void );
void clearError( void );


#endif /* APP_ERROR_H */


/*******************************************************************************
 End of File
 */
