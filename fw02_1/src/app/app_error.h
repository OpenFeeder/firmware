/**
 * @file app_error.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 */

#ifndef _APP_ERROR_HEADER_H
#define _APP_ERROR_HEADER_H


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
    /* Critical errors: the system stops if errors below occurred */
    ERROR_LOW_BATTERY,
    ERROR_LOW_FOOD,
    ERROR_LOW_VBAT,
    ERROR_DOOR_CANT_CLOSE,
    ERROR_CRITICAL,
    /* Non critical errors: the system resets if errors below occurred */
    ERROR_USB,
    ERROR_USB_DEVICE_NOT_FOUND,
    ERROR_USB_MOUNT_DRIVE,
    ERROR_USB_UNMOUNT_DRIVE,
    ERROR_USB_RESUME_DEVICE,
    ERROR_USB_SUSPEND_DEVICE, 
    ERROR_INI_FILE_NOT_FOUND,
    ERROR_INI_FILE_READ,
    ERROR_PIT_TAGS_PTONE1_FILE_OPEN,
    ERROR_PIT_TAGS_PTONE1_FILE_CLOSE,
    ERROR_PIT_TAGS_PTONE2_FILE_OPEN,
    ERROR_PIT_TAGS_PTONE2_FILE_CLOSE,
    ERROR_PIT_TAGS_PTONE3_FILE_OPEN,
    ERROR_PIT_TAGS_PTONE3_FILE_CLOSE,
    ERROR_PIT_TAGS_PTONE4_FILE_OPEN,
    ERROR_PIT_TAGS_PTONE4_FILE_CLOSE,
    ERROR_PIT_TAGS_LEFT_FILE_OPEN,
    ERROR_PIT_TAGS_LEFT_FILE_CLOSE,
    ERROR_PIT_TAGS_RIGHT_FILE_OPEN,
    ERROR_PIT_TAGS_RIGHT_FILE_CLOSE,
    ERROR_PIT_TAGS_TOP_FILE_OPEN,
    ERROR_PIT_TAGS_TOP_FILE_CLOSE,
    ERROR_PIT_TAGS_BOTTOM_FILE_OPEN,
    ERROR_PIT_TAGS_BOTTOM_FILE_CLOSE,
    ERROR_PIT_TAGS_ACCEPTED_FILE_OPEN,
    ERROR_PIT_TAGS_ACCEPTED_FILE_READ,
    ERROR_PIT_TAGS_ACCEPTED_FILE_CLOSE,
    ERROR_PIT_TAGS_DENIED_FILE_OPEN,
    ERROR_PIT_TAGS_DENIED_FILE_READ,
    ERROR_PIT_TAGS_DENIED_FILE_CLOSE,
    ERROR_PIT_TAGS_COLOR_A_FILE_OPEN,
    ERROR_PIT_TAGS_COLOR_A_FILE_CLOSE,
    ERROR_PIT_TAGS_COLOR_B_FILE_OPEN,
    ERROR_PIT_TAGS_COLOR_B_FILE_CLOSE,
    ERROR_DOOR,
    ERROR_LOW_RFID_FREQUENCY,
    ERROR_TIMEOUT_RFID_FREQUENCY,
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
    ERROR_RFID_FILE_CLOSE,
    ERROR_UDID_FILE_OPEN,
    ERROR_UDID_FILE_WRITE,
    ERROR_UDID_FILE_CLOSE,
    ERROR_ERROR_FILE_OPEN,
    ERROR_ERROR_FILE_WRITE,
    ERROR_ERROR_FILE_CLOSE,
    ERROR_EVENT_CSV_FILE_OPEN,
    ERROR_EVENT_CSV_FILE_WRITE,
    ERROR_EVENT_CSV_FILE_CLOSE,
    ERROR_EVENT_CSV_FILE_SET_NAME,
    ERROR_EVENT_BIN_FILE_OPEN,
    ERROR_EVENT_BIN_FILE_WRITE,
    ERROR_EVENT_BIN_FILE_CLOSE,
    ERROR_EVENT_BIN_FILE_SET_NAME,
    ERROR_ATTRACTIVE_LED_INIT,
    ERROR_REWARD_PROBABILITIES_FILE_OPEN,
    ERROR_REWARD_PROBABILITIES_FILE_CLOSE,
        
    ERROR_TOO_MANY_SOFTWARE_RESET = 100

} ERROR_NUMBER;

typedef struct
{
    char message[200]; // Error message buffer
    char currentFileName[200];
    uint16_t currentLineNumber;

    LED_STATUS ledColor_1;
    LED_STATUS ledColor_2;

    ERROR_NUMBER number;
    
    struct tm time;

} APP_ERROR;

FILEIO_RESULT logError(void);
void printError( void );
void clearError( void );

#endif /* _APP_ERROR_HEADER_H */


/*******************************************************************************
 End of File
 */
