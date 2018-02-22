/**
 * @file app_event.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 */

#ifndef _APP_EVENT_HEADER_H
#define _APP_EVENT_HEADER_H

#define MAX_NUMBER_OF_EVENT 100
#define MAX_NUM_EVENT_BEFORE_SAVE 50

typedef enum
{
    OF_EVENT_NONE,
    OF_RESET_SWR,
    OF_RESET_MCLR_RUN_MODE,
    OF_RESET_POR,
    OF_RESET_VBAT,
    OF_RESET_MCLR_IDLE_MODE,
    OF_RESET_MCLR_SLEEP_MODE,
    OF_RESET_OTHER,
        
    OF_ENTER_NORMAL_MODE,
    OF_ENTER_DEBUG_MODE,
        
    OF_STATE_INITIALIZE,
    OF_STATE_CONFIGURE_SYSTEM,
    OF_STATE_IDLE,
    OF_STATE_RFID_READING_PIT_TAG,
    OF_STATE_OPENING_DOOR,
    OF_STATE_WAITING_CATCH_REWARD,
    OF_STATE_CLOSING_DOOR,
    OF_STATE_REOPEN_DOOR,
    OF_STATE_DATA_LOG,
    OF_STATE_STANDBY,
    OF_STATE_SLEEP,
    OF_STATE_WAKE_UP,
    OF_STATE_FLUSH_DATA_TO_USB,
    OF_STATE_REMOVE_USB_DEVICE,
    OF_STATE_FLUSH_DATA_BEFORE_ERROR,
    OF_STATE_TEST_RFID,
    OF_STATE_ERROR,
        
    OF_SYSTEM_RESET,

    OF_CHECK_BATTERY,
    OF_CHECK_VBAT,
    OF_CHECK_FOOD,
    OF_CHECK_RFID, 
        
    OF_CALIBRATE_TIME,
        
    OF_POPULATE_LOG_BUFFER,
    OF_WRITE_DATA_LOG,
    OF_WRITE_BATTERY_LOG,
    OF_WRITE_UDID_LOG,
    OF_WRITE_RFID_LOG,
    OF_WRITE_EVENT_LOG,
        
    OF_POWER_PIR_ON,
    OF_POWER_PIR_OFF,
    OF_POWER_USB_RFID_ON,
    OF_POWER_USB_RFID_OFF,
    OF_POWER_SERVO_ON,
    OF_POWER_SERVO_OFF,
        
    OF_OPEN_DOOR,
    OF_CLOSE_DOOR,
        
    OF_RECONFIGURE_SYSTEM,
        
    OF_MOUNT_USB_DRIVE,
    OF_UNMOUNT_USB_DRIVE,
    OF_SUSPEND_USB_DRIVE,
    OF_RESUME_USB_DRIVE
        
        
} APP_EVENT;

typedef struct
{
    /* Log file name - 8.3 convention - Upper case only */
    char filename[13];
    char binfilename[13];
    /* Data separator in log file */
    char separator[2];
    
    uint8_t hours[MAX_NUMBER_OF_EVENT];
    uint8_t minutes[MAX_NUMBER_OF_EVENT];
    uint8_t seconds[MAX_NUMBER_OF_EVENT];
    uint8_t numbers[MAX_NUMBER_OF_EVENT];
    uint8_t num_events_stored;

} APP_DATA_EVENT;


void store_event( APP_EVENT ); 
bool setEventFileName( void );
void display_events( void );
void clear_events_buffers( void );
FILEIO_RESULT logEvents(void);

#endif /* _APP_EVENT_HEADER_H */


/*******************************************************************************
 End of File
 */
