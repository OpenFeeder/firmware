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

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************


typedef enum
{
    ERROR_NONE,
    ERROR_LOW_BATTERY,
    ERROR_LOW_VBAT,
    ERROR_USB,
    ERROR_INI_FILE_READ,
    ERROR_PIT_TAGS_ACCEPTED_FILE_OPEN,
    ERROR_PIT_TAGS_ACCEPTED_FILE_READ,
    ERROR_PIT_TAGS_ACCEPTED_FILE_CLOSE,
    ERROR_PIT_TAGS_DENIED_FILE_OPEN,
    ERROR_PIT_TAGS_DENIED_FILE_READ,
    ERROR_PIT_TAGS_DENIED_FILE_CLOSE,
    ERROR_DOOR,
    ERROR_LOW_FOOD,
    ERROR_LOW_RFID_FREQUENCY,
    ERROR_POPULATE_DATA_BUFFER,
    ERROR_LOG_FILE_OPEN,
    ERROR_LOG_FILE_WRITE,
    ERROR_LOG_FILE_CLOSE,
    ERROR_LOG_FILE_SET_NAME,
    ERROR_BATTERY_FILE_OPEN,
    ERROR_BATTERY_FILE_WRITE,
    ERROR_BATTERY_FILE_CLOSE,
    ERROR_RFID_FILE_OPEN,
    ERROR_RFID_FILE_WRITE,
    ERROR_RFID_FILE_CLOSE

} ERROR_NUMBER;


typedef struct
{
    char message[200]; // Error message buffer
    char currentFileName[200];
    int16_t currentLineNumber;
    //    uint8_t priority; // Message priority: 0 => high priority
    //                      //                   1 => low priority 

    LED_STATUS ledColor;

    ERROR_NUMBER number;

} APP_ERROR;


void printError( void );
void clearError( void );


#endif /* APP_ERROR_H */


/*******************************************************************************
 End of File
 */
