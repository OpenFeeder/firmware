/** 
 * File: app.c
 * Author: OpenFeeder Team <https://github.com/orgs/OpenFeeder/people> 
 * Comments:
 * Revision history:
 */


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
 */

APP_DATA appData; /* Global application data. */
APP_DATA_PIT_TAG appDataPitTag; /* PIT tags application data */
APP_ERROR appError; /* Errors application data */
APP_DATA_USB appDataUsb; /* USB application data */
APP_DATA_ALARM appDataAlarmSleep; /* Alarm sleep application data */
APP_DATA_ALARM appDataAlarmWakeup; /* Alarm wake-up application data */
APP_DATA_LEDS appDataAttractiveLeds; /* Atractive LEDs application data */
APP_DATA_LOG appDataLog; /* Datalogger application data */
APP_DATA_SERVO appDataServo; /* Servomotor application data */
APP_DATA_RC appDataRc; /* Remote control application data */
APP_DATA_EVENT appDataEvent; /* Events application data */
APP_DATA_DOOR appDataDoor; /* Door application data */


/******************************************************************************
  Function:
    void APP_Tasks( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Tasks( void )
{
    static bool button_user_state;
    static bool previous_button_user_state = BUTTON_NOT_PRESSED;
    APP_CHECK chk;
    int i;
    static bool enter_default_state = false;
    static int num_timeout_reward;

    /* Check the Application State. */
    switch ( appData.state )
    {
        case APP_STATE_INITIALIZE:
        {
            /*
             * Initializing the application.
             * 
             * Process:
             *  - Date and Time calibration
             *  - Enable PIR sensor    
             * 
             * Next state: APP_STATE_CONFIGURE_SYSTEM
             */

            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined ( DISPLAY_CURRENT_STATE )
                printf( "> APP_STATE_INITIALIZE\n" );
#endif
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_STATE_INITIALIZE );
                }

            }

            /* Date & time calibration using external RTC module */
            calibrateDateTime( );

            /* Power PIR sensor early in the code because of starting delay before usable */
            powerPIREnable( );

            /* Go to device configuration state */
            appData.state = APP_STATE_CONFIGURE_SYSTEM;
            break;
        }
            /* -------------------------------------------------------------- */

        case APP_STATE_CONFIGURE_SYSTEM:

            /*
             * System configuration.
             * 
             * Process:
             *  - Set log file name
             *  - Power USB device
             *  - Search for plugged USB device 
             *  - Check status LEDs 
             *  - Check important parameters:
             *      - battery level (mandatory)
             *      - VBat level (mandatory)
             *      - RFID frequency (mandatory)
             *      - Food level 
             *  - Log Unique Device ID (UDID)
             *  - Test servomotor
             *  - Test I2C interface
             *  - Initialize attractive LEDs (if needed)
             *  
             * Next state: 
             *  - if configuration succeed => APP_STATE_IDLE
             *  - if configuration failed => APP_STATE_ERROR
             */

            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined ( DISPLAY_CURRENT_STATE )
                printf( "> APP_STATE_CONFIGURE_SYSTEM\n" );
#endif
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_STATE_CONFIGURE_SYSTEM );
                }

                /* Set log file name => 20yymmdd.CSV (one log file per day). */
                if ( false == setLogFileName( ) )
                {
                    appDataUsb.is_device_needed = false;
                    appData.state = APP_STATE_ERROR;
                    break;
                }

                /* Power USB device */
                powerUsbRfidEnable( );
                /* Ask for the device */
                appDataUsb.is_device_needed = true;

                /* Set timeout if USB device is not found */
                setDelayMs( MAX_DELAY_TO_DETECT_USB_DEVICE );
                
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf( "\tSearching USB device.\n\tThe system will restart if not found after %ldms.\r\n", ( long ) MAX_DELAY_TO_DETECT_USB_DEVICE );
#endif

            }

            /* If USB device is plugged and a valid address is set */
            if ( appDataUsb.is_device_address_available )
            {
#if defined (USE_UART1_SERIAL_INTERFACE)
                        printf( "\tUSB device found.\r\n\r\n" );
#endif   
                /* Mount drive on USB device */
                if ( USB_DRIVE_NOT_MOUNTED == usbMountDrive( ) )
                {
                    /* If mount failed => error */
                    appDataUsb.is_device_needed = false;
                    appData.state = APP_STATE_ERROR;
                    break;
                }
            }
                /* Otherwise wait for USB device connection or for timeout to reach */
            else
            {
                /* If timeout reached => error */
                if ( true == isDelayMsEnding( ) )
                {
                    appDataUsb.is_device_needed = false;
                    sprintf( appError.message, "USB device detection took more than %ldms", ( long ) MAX_DELAY_TO_DETECT_USB_DEVICE );
                    appError.current_line_number = __LINE__;
                    sprintf( appError.current_file_name, "%s", __FILE__ );
                    appError.number = ERROR_USB_DEVICE_NOT_FOUND;
                    appData.state = APP_STATE_ERROR;
                    break;
                }

                /* Blue and yellow status LEDs blink as long USB key is required. */
                LedsStatusBlink( LED_USB_ACCESS, LED_YELLOW, 500, 500 );
                break;
            }

            /* Turn on LEDs status while USB device is accessed */
            setLedsStatusColor( LED_USB_ACCESS );

            /* Set log events file name if required */
            if ( true == appDataLog.log_events )
            {
                /* If set file name failed => error */
                if ( false == setEventFileName( ) )
                {
                    appDataUsb.is_device_needed = false;
                    appData.state = APP_STATE_ERROR;
                    break;
                }
            }

            /* Configure the system with the CONFIG.INI file */
            appData.flags.bit_value.system_init = config_set( );

            /* If system configuration succeed */
            if ( true == appData.flags.bit_value.system_init )
            {

                srand( appData.current_time.tm_mon + appData.current_time.tm_mday + appData.current_time.tm_min + appData.current_time.tm_sec );

                /* Check all status LEDs */
                checkLedsStatus( );

                /* Check mandatory parameters */
                chk = checkImportantParameters( );
                switch ( chk )
                {
                    case APP_CHECK_OK:
                        appData.state = APP_STATE_CONFIGURE_SYSTEM;
                        break;
                    case APP_CHECK_BATTERY_PB:
                    case APP_CHECK_VBAT_PB:
                    case APP_CHECK_FOOD_LEVEL_PB:
                    case APP_CHECK_RFID_FREQ_PB:
                        appData.state = APP_STATE_FLUSH_DATA_BEFORE_ERROR;
                        break;
                }

                /* If one of the mandatory parameter failed => error */
                if ( appData.state != APP_STATE_CONFIGURE_SYSTEM )
                {
                    break;
                }

                /* Flush data on USB device */
                if ( FLUSH_DATA_ON_USB_DEVICE_SUCCESS != flushDataOnUsbDevice( ) )
                {
                    appData.state = APP_STATE_ERROR;
                    break;
                }
                
                /* Log current firmware version */
                logFirmware( );
                    
                /* Log the Unique Devide ID is required */
                if ( true == appDataLog.log_udid )
                {
                    logUdid( );
                }

                /* Turn on LEDs status while servo is moved */
                setLedsStatusColor( LED_SERVO );

                /* Test servomotor by opening and closing the door */
                testServomotor();

                /* Manage attractive LEDs if needed */
                if ( true == appData.flags.bit_value.attractive_leds_status )
                {

                    /* Reset the attractive LEDs driver */                    
                    if ( I2C1_PCA9622_SoftwareReset( ) )
                    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_PCA9622_STATUS)
                        printf( "\tAttractive LEDs driver OK\n" );
#endif
                    }
                    /* If LEDs driver reset failed => error*/
                    else
                    {
                        sprintf( appError.message, "Unable to reset the attractive LEDS driver" );
                        appError.current_line_number = __LINE__;
                        sprintf( appError.current_file_name, "%s", __FILE__ );
                        appError.number = ERROR_ATTRACTIVE_LED_DRIVER_RESET;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }

                    /* If LEDs initialization OK => test LEDs */
                    if ( initAttractiveLeds( ) )
                    {
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined ( DISPLAY_LEDS_STATUS )
                        printf( "\tAttractive LEDs: ok\n" );
#endif
                        /* Turn on attractive LEDs one by one in red */
                        testAttractiveLeds( );

                    }
                    /* If LEDs initialization failed => error */
                    else
                    {
                        sprintf( appError.message, "Unable to initialize attractive LEDS via I2C" );
                        appError.current_line_number = __LINE__;
                        sprintf( appError.current_file_name, "%s", __FILE__ );
                        appError.number = ERROR_ATTRACTIVE_LED_INIT;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }

                    /* Set attractive LEDs order according to file UDIDLEDS.TXT */
                    if ( true == setAttractiveLedsIndex( ) )
                    {
                        /* Turn on attractive LEDs one by one in green */
                        testAttractiveLedsOrder( );
                    }
                    
                    /* Turn on attractive LEDs if required */
                    getDateTime( );
                    
                    if ( ( appData.current_time.tm_hour * 60 + appData.current_time.tm_min ) >= ( appDataAttractiveLeds.wake_up_time.tm_hour * 60 + appDataAttractiveLeds.wake_up_time.tm_min ) &&
                         ( appData.current_time.tm_hour * 60 + appData.current_time.tm_min )< ( appDataAttractiveLeds.sleep_time.tm_hour * 60 + appDataAttractiveLeds.sleep_time.tm_min ) )
                    {
                        setAttractiveLedsOn( );
                    }
                }

                /* Set RTC alarm to raise every second */
                rtcc_set_alarm( appDataAlarmWakeup.time.tm_hour, appDataAlarmWakeup.time.tm_min, appDataAlarmWakeup.time.tm_sec, EVERY_SECOND );

                /* Go to state idle */
                appData.state = APP_STATE_IDLE;
            }
            /* If system configuration failed => error */
            else
            {
                appDataUsb.is_device_needed = false;
                appData.state = APP_STATE_ERROR;
                break;
            }

            /* Unmount drive on USB device */
            if ( USB_DRIVE_MOUNTED == usbUnmountDrive( ) )
            {
                /* If unmount failed => error */
                appData.state = APP_STATE_ERROR;
            }

            appDataUsb.is_device_needed = false;

            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_IDLE:
            
            /*
             * Idle state.
             * 
             * Process:
             *  - Wait for bird detection by PIR sensor  
             *  - Serial communication for debugging
             *  - Manage RTC action
             * 
             * Next state: 
             *  - if bird is detected: APP_STATE_RFID_READING_PIT_TAG
             */
            
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined ( DISPLAY_CURRENT_STATE )
                printf( "> APP_STATE_IDLE\n" );
#endif              
#if defined ( USE_UART1_SERIAL_INTERFACE )
                printf( "\tUser button pressed briefly => flush data and eject USB device\n" );
                printf( "\tUser button pressed longer => activate serial communication mode\n" );
#endif   
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_STATE_IDLE );
                }

                /* Enable RTC alarm */
                rtcc_start_alarm( );

                /* Enable PIR sensor interruption for bird detection */
                EX_INT0_InterruptFlagClear( );
                EX_INT0_InterruptEnable( );

                clearPitTagBuffers( );
                clearPitTagSringBuffers( );

                appData.test_rfid = false;
                is_bird_detected = false;

                if ( false == appData.punishment_state && true == appData.flags.bit_value.attractive_leds_status )
                {
                    if ( DOOR_HABITUATION == appData.scenario_number ||
                         LONG_TERM_SPATIAL_MEMORY == appData.scenario_number ||
                         WORKING_SPATIAL_MEMORY == appData.scenario_number ||
                         RISK_AVERSION == appData.scenario_number ||
                         PATCH_PROBABILITY == appData.scenario_number )
                    {
                        appDataAttractiveLeds.current_color_index = ATTRACTIVE_LEDS_COLOR_A;
                        setAttractiveLedsColor( );
                    }
                    else if ( GO_NO_GO == appData.scenario_number || COLOR_ASSOCIATIVE_LEARNING == appData.scenario_number )
                    {
                        initializeLedsScenario( );
                    }
                }

            }

            /* If the user ask for reconfiguration via serial communication */
            if ( appData.need_to_reconfigure )
            {
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_RECONFIGURE_SYSTEM );
                }

                appData.need_to_reconfigure = false;
                for ( i = 0; i < MAX_PIT_TAGS_LIST_NUMBER; i++ )
                {
                    appDataPitTag.is_pit_tag_denied_or_color_A[i] = false;
                }
                appData.state = APP_STATE_CONFIGURE_SYSTEM;
                break;
            }

//#if defined ( USE_UART1_SERIAL_INTERFACE )
//            /* Get interaction with the serial terminal. */
//            APP_SerialDebugTasks( );
//#endif

            /* If punishment timeout reached => remove punishment state */
            if ( true == appData.punishment_state && true == isDelayPunishmentMsEnding( ) )
            {
#if defined ( USE_UART1_SERIAL_INTERFACE )
                printf( "\tPunishment timeout reached\n" );
#endif
                appData.punishment_state = false;

                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_PUNISHMENT_STOP );
                }
            }

            /* If event buffer is full => flush events to USB device */
            if ( true == appDataLog.log_events && appDataEvent.num_events_stored >= MAX_NUM_EVENT_BEFORE_SAVE )
            {
                if ( FILEIO_RESULT_FAILURE == logEvents( ) )
                {
                    appDataUsb.is_device_needed = false;
                    appData.state = APP_STATE_ERROR;
                    break;
                }
            }

            if ( RTCC_ALARM_IDLE != appData.rtcc_alarm_action )
            {
                manageRtcAction( );
                
                if ( APP_STATE_IDLE != appData.state )
                {
                    break;
                }
            }

            /* Check USER BUTTON detected. */
            button_user_state = USER_BUTTON_GetValue( );

            if ( button_user_state != previous_button_user_state )
            {
                previous_button_user_state = button_user_state;
                if ( BUTTON_PRESSED == button_user_state )
                {
                    setLedsStatusColor( LEDS_ON );
                    setDelayMs( 2000 );
                    while ( 0 == isDelayMsEnding( ) );

                    button_user_state = USER_BUTTON_GetValue( );

                    setLedsStatusColor( LEDS_OFF );

                    if ( BUTTON_PRESSED == button_user_state )
                    {
//                        appData.state = APP_STATE_TEST_RFID;
                        appData.state = APP_STATE_SERIAL_COMMUNICATION;
                        break;
                    }
                    else
                    {
                        appData.state = APP_STATE_FLUSH_DATA_TO_USB;
//                        appData.state = APP_STATE_SERIAL_COMMUNICATION;
                        break;
                    }
                }
            }  
            
            /* Green status LED blinks in idle mode. */
            LedsStatusBlink( LED_GREEN, LEDS_OFF, 25, 4975 );

            /* If a new bird is detected by the PIR sensor */
            if ( true == is_bird_detected )
            {

                /* Disable PIR sensor interruption */
                EX_INT0_InterruptFlagClear( );
                EX_INT0_InterruptDisable( );

                /* Get landing time */
                while ( !RTCC_TimeGet( &appDataLog.bird_arrived_time ) )
                {
                    Nop( );
                }
                                            
                /* Initialised global variable datalogging. */
                is_bird_detected = false;
                appDataLog.is_reward_taken = false;
                appDataLog.did_door_open = false;
                clearPitTagBuffers( );

                appDataLog.attractive_leds_current_color_index = appDataAttractiveLeds.current_color_index;

                /* Get door position at landing time */
                if ( DOOR_OPENED == appDataDoor.reward_door_status )
                {
                    appDataLog.door_status_when_bird_arrived = 1;
                }
                else
                {
                    appDataLog.door_status_when_bird_arrived = 0;
                }

                /* Go to RFID reading state */
                appData.state = APP_STATE_RFID_READING_PIT_TAG;
                break;
            }
            
            //#if defined (TEST_RTCC_SLEEP_WAKEUP)
            //            /* Next line for debugging sleep/wakeup only */
            //            /* Should be commented in normal mode */
            //            /* Modify time value according to sleep values in the CONFIG.INI file */
            //            setDateTime( 17, 9, 21, 22, 59, 55 );
            //#endif   
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_SERIAL_COMMUNICATION:
                
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf( "> APP_STATE_SERIAL_COMMUNICATION\n" );
#endif

                /* Disable PIR interruption */
                EX_INT0_InterruptDisable( );
                
                /* Disable RTC alarm */
                rtcc_stop_alarm( );
                
                if ( true == appData.flags.bit_value.attractive_leds_status )
                {
                    setAttractiveLedsNoColor( );
                }
                    
                displayKeyMapping( );
                
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf( "\n\t/!\\ The system will reset after 60s of inactivity.\n" );
#endif   
                
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_STATE_SERIAL_COMMUNICATION );
                }
                
                /* Empty UART RX buffer to avoid garbage value */
                while ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) )
                {
                    UART1_Read( );                    
                }
                
                setDelayMsStandBy( 60000 );

            }

            /* Green status LED blinks in idle mode. */
            LedsStatusBlink( LED_SERIAL_COMMUNICATION, LEDS_OFF, 100, 4900 );
            
            /* Get interaction with the serial terminal. */
            APP_SerialDebugTasks( );

            /* If more than 60s of inactivity => reset the system */
            if ( true == isDelayMsEndingStandBy( ) )
            {

#if defined ( USE_UART1_SERIAL_INTERFACE ) 
                printf( "\tMore than 60s of inactivity.\n\tThe system will reset in 3s.\n" );
#endif
                for ( i = 0; i < 3; i++ )
                {
                    setLedsStatusColor( LEDS_ON );
                    setDelayMs( 500 );
                    while ( 0 == isDelayMsEnding( ) );
                    setLedsStatusColor( LEDS_OFF );
                    setDelayMs( 500 );
                    while ( 0 == isDelayMsEnding( ) );
                }
                
                /* Set peripherals Off */
                setAttractiveLedsOff( );
                powerPIRDisable( );
                EX_INT0_InterruptDisable( );
                RFID_Disable( );
                IRSensorDisable( );
                servomotorPowerDisable( );

                /* Unmount drive on USB device before power it off. */
                if ( USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status )
                {
                    usbUnmountDrive( );
                }
                USBHostShutdown( );
                powerUsbRfidDisable( );
                
                __asm__ volatile ( "reset" );
                
            }
            
            break;
            /* -------------------------------------------------------------- */
                        
        case APP_STATE_RFID_READING_PIT_TAG:
                        
            /*
             * RFID reading.
             * 
             * Process:
             * 
             * Next state: 
             *  - if PIT tag read: APP_STATE_OPENING_DOOR
             */
            
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf( "> APP_STATE_RFID_READING_PIT_TAG\n" );
#endif
#if defined ( USE_UART1_SERIAL_INTERFACE )
                printf( "\tBird detected\n" );
#endif    
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_STATE_RFID_READING_PIT_TAG );
                }

                /* Stop PIR interruption */
                EX_INT0_InterruptFlagClear( );
                EX_INT0_InterruptDisable( );

                /* Stop alarm interruption */
                rtcc_stop_alarm( );

                /* Initialize RFID */
                APP_Rfid_Init( );

            }

            APP_Rfid_Task( );

            /* Enable interruption see case RFID_DETECT_COMPLET_DATASTREAM
             * in APP_Rfid_Task */
            EX_INT4_InterruptEnable( );

            if ( appData.flags.bit_value.new_valid_pit_tag )
            {

                /* Manage PIT tag according to current scenario */
                manageScenario( );

                /* Disable RFID to preserve battery */
                RFID_Disable( );

                is_bird_detected = false;

                if ( PATCH_PROBABILITY == appData.scenario_number && false == isItANewPitTag( ) )
                {
                    appData.rfid_signal_detected = false;
                    appDataLog.is_reward_taken = false;
                    appDataLog.did_door_open = false;
                    appDataLog.is_pit_tag_denied = false;
                    clearPitTagBuffers( );
                    appData.state = APP_STATE_IDLE;
                    break;
                }

                /* If PIT tag read is considered as denied */
                if ( true == appDataLog.is_pit_tag_denied )
                {
                    /* If the PIT tag is in the list => simply denied */
                    if ( true == appDataPitTag.pit_tag_found_in_list )
                    {
                        /* Log event if required */
                        if ( true == appDataLog.log_events )
                        {
                            store_event( OF_PIT_TAG_DENIED );
                        }
#if defined ( USE_UART1_SERIAL_INTERFACE ) 
                        printf( "\tPIT tag %s denied.\n", appDataLog.bird_pit_tag_str );
#endif
                    }
                    /* If the PIT tag is not in the list => not listed and denied */
                    else
                    {
                        /* Log event if required */
                        if ( true == appDataLog.log_events )
                        {
                            store_event( OF_PIT_TAG_NOT_LISTED_DENIED );
                        }
#if defined ( USE_UART1_SERIAL_INTERFACE ) 
                        printf( "\tPIT tag %s not listed => consider as denied.\n", appDataLog.bird_pit_tag_str );
#endif
                    }

                    /* If PIT tag denied and scenario 3 or 6 => activate punishment state */
                    if ( COLOR_ASSOCIATIVE_LEARNING == appData.scenario_number || GO_NO_GO == appData.scenario_number )
                    {
                        /* Log event if required */
                        if ( true == appDataLog.log_events )
                        {
                            store_event( OF_ATTRACTIVE_LEDS_PUNISHMENT );
                        }

                        /* Turn off attractive LEDs */
                        setAttractiveLedsNoColor( );

                        /* Set punishment timeout */
                        setDelayPunishmentMs( appData.punishment_delay );

                        /* If punishmen is not yet activated => activate */
                        if ( false == appData.punishment_state )
                        {
                            appData.punishment_state = true;
                            /* Log event if required */
                            if ( true == appDataLog.log_events )
                            {
                                store_event( OF_PUNISHMENT_START );
                            }
#if defined ( USE_UART1_SERIAL_INTERFACE )
                            printf( "\tPunishment start\n" );
#endif
                        }
                        /* If punishment is yet activated => re-activate */
                        else
                        {
                            /* Log event if required */
                            if ( true == appDataLog.log_events )
                            {
                                store_event( OF_PUNISHMENT_RESTART );
                            }
#if defined ( USE_UART1_SERIAL_INTERFACE )
                            printf( "\tPunishment re-start\n" );
#endif
                        }

                    }
                    appData.state = APP_STATE_DATA_LOG;
                }
                /* Otherwise, PIT tag is accepted => Open the door*/
                else
                {
#if defined ( USE_UART1_SERIAL_INTERFACE )
                    printf( "\tPIT tag %s accepted.\n", appDataLog.bird_pit_tag_str );
#endif
                    appData.state = APP_STATE_OPENING_DOOR;
                }
                break;
            }

            /* If RFID timeout is reached => disable RFID */
            if ( 0 == g_timeout_reading_pit_tag )
            {
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_RFID_TIMEOUT );
                }

                /* Disable RFID to preserve battery */
                RFID_Disable( );

                is_bird_detected = false;

                /* If RFID signal but no tag read => set tag to XXXXXXXXXX */
                if ( false == appData.rfid_signal_detected )
                {
                    snprintf( appDataLog.bird_pit_tag_str, 11, "XXXXXXXXXX" );

                    /* Log event if required */
                    if ( true == appDataLog.log_events )
                    {
                        store_event( OF_PIT_TAG_X );
                    }
                }
                    /* If RFID signal and tag read but not decoded => set tag to ?????????? */
                else
                {
                    snprintf( appDataLog.bird_pit_tag_str, 11, "??????????" );

                    /* Log event if required */
                    if ( true == appDataLog.log_events )
                    {
                        store_event( OF_PIT_TAG_U );
                    }
                }

                /* Clear variables */
                appData.rfid_signal_detected = false;
                appDataLog.is_reward_taken = false;
                appDataLog.did_door_open = false;
                appDataLog.is_pit_tag_denied = false;
                clearPitTagBuffers( );

                appData.state = APP_STATE_DATA_LOG;
#if defined ( USE_UART1_SERIAL_INTERFACE )
                printf( "\tRFID timeout reached\n" );
#endif
            }
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_OPENING_DOOR:
                        
            /*
             * Door opening.
             * 
             * Process:
             * 
             * Next state: 
             */
            
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf( "> APP_STATE_OPENING_REWARD_DOOR\n" );
#endif
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_STATE_OPENING_DOOR );
                }

                /* Reward probability */
                if ( 0 == appDataDoor.reward_probability )
                {
                    setAttractiveLedsNoColor( );
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined(DISPLAY_REWARD_PROBABILITY)
                    printf( "\tReward probability is 0%% => denied.\n" );
#endif
                    /* Delay before reactivate attractiveLEDs */
                    setDelayMs( appData.punishment_delay );
                    while ( false == isDelayMsEnding( ) )
                    {
                        Nop( );
                    }
                    appDataLog.is_pit_tag_denied = true;
                    appData.state = APP_STATE_DATA_LOG;
                    break;
                }
                else if ( 100 == appDataDoor.reward_probability )
                {
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined(DISPLAY_REWARD_PROBABILITY)
                    printf( "\tReward probability is 100%% => accepted.\n" );
#endif
                }
                else
                {
                    int randomInteger = rand( );

                    if ( randomInteger > ( RAND_MAX / 100 * appDataDoor.reward_probability ) )
                    {
                        if ( PATCH_PROBABILITY != appData.scenario_number )
                        {
                            setAttractiveLedsNoColor( );
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined(DISPLAY_REWARD_PROBABILITY)
                            printf( "\t%.0f > %u (reward prob.) => denied.\n", 100 * ( ( double ) randomInteger / ( double ) RAND_MAX ), appDataDoor.reward_probability );
#endif
                            /* Delay before reactivate attractiveLEDs */
                            setDelayMs( appData.punishment_delay );
                            while ( false == isDelayMsEnding( ) )
                            {
                                Nop( );
                            }
                            appDataLog.is_pit_tag_denied = true;
                            appData.state = APP_STATE_DATA_LOG;
                            break;
                        }
                        else if ( appDataDoor.reward_probability < appData.punishment_proba_thresh )
                        {
                            setAttractiveLedsNoColor( );
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined(DISPLAY_REWARD_PROBABILITY)
                            printf( "\t%.0f > %u (reward prob.) => denied.\n", 100 * ( ( double ) randomInteger / ( double ) RAND_MAX ), appDataDoor.reward_probability );
#endif
                            /* Delay before reactivate attractiveLEDs */
                            setDelayMs( appData.punishment_delay );
                            while ( false == isDelayMsEnding( ) )
                            {
                                Nop( );
                            }
                            appDataLog.is_pit_tag_denied = true;
                            appData.state = APP_STATE_DATA_LOG;
                            break;
                        }
                        else
                        {
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined(DISPLAY_REWARD_PROBABILITY)
                            printf( "\t%.0f > %u (reward prob.) => denied\n\tBut %u (reward prob.) > %u (prob. thresh.) => no punishment.\n", 100 * ( ( double ) randomInteger / ( double ) RAND_MAX ), appDataDoor.reward_probability, appDataDoor.reward_probability, appData.punishment_proba_thresh );
#endif 
                            appDataLog.is_pit_tag_denied = true;
                            appData.state = APP_STATE_DATA_LOG;
                            break;
                        }

                    }
                    else
                    {
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined(DISPLAY_REWARD_PROBABILITY)
                        printf( "\t%.0f < %u (reward prob.) => accepted.\n", 100 * ( ( double ) randomInteger / ( double ) RAND_MAX ), appDataDoor.reward_probability );
#endif 
                    }

                }

                /* If reward monitoring required => enable IR barrier */
                if ( 1 == appData.reward_enable )
                {
                    /* Power IR sensor here because start delay */
                    IRSensorEnable( );
                    /* Enable IR 1 sensor interruption (reward)*/
                    EX_INT1_InterruptDisable( );
                    EX_INT1_PositiveEdgeSet( );
                    EX_INT1_InterruptFlagClear( );
                    EX_INT1_InterruptEnable( );
                }

                appDataLog.is_reward_taken = false;

                /* Skip opening door if the "remain open" flag is set */
                if ( 1 == appDataDoor.remain_open )
                {
                    appDataLog.did_door_open = true;
                    appData.state = APP_STATE_WAITING_CATCH_REWARD;
                    break;
                }

                /* Optional delay before opening the door */
                if ( appDataDoor.open_delay > 0 )
                {
                    setDelayMs( appDataDoor.open_delay );
                    while ( false == isDelayMsEnding( ) )
                    {
                        Nop( );
                    }
                }

            }

            /* Servomotor power command enable. */
            servomotorPowerEnable( );

            appDataServo.ton_cmd = servomotorGetDoorPosition( );
            appDataServo.ton_goal = appDataServo.ton_max;

            if ( appDataServo.ton_cmd != appDataServo.ton_goal )
            {
                appDataDoor.reward_door_status = DOOR_MOVING;
                while ( DOOR_MOVED != appDataDoor.reward_door_status );
            }
            appDataDoor.reward_door_status = DOOR_OPENED;

#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_SERVO_POSITION)
            servomotorGetDoorPosition( );
#endif  
            /* Servomotor power command disable. */
            servomotorPowerDisable( );

            appDataLog.did_door_open = true;

#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_SERVO_INFO)
            printf( "\tDoor opened\n" );
#endif
            appData.state = APP_STATE_WAITING_CATCH_REWARD;
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_WAITING_CATCH_REWARD:
            
            /*
             * Waiting for catch reward.
             * 
             * Process:
             * 
             * Next state: 
             */
            
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf( "> APP_STATE_WAITING_CATCH_REWARD\n" );
#endif           
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_STATE_WAITING_CATCH_REWARD );
                }

                num_timeout_reward = 0;

                /* Timeout before door closing if reward is not taken */
                if ( appData.timeout_taking_reward > 0 )
                {
                    setDelayMs( appData.timeout_taking_reward );
                }

                appData.bird_is_taking_reward = false;

                clear_ir1_sensor( );

                if ( 0 == appData.reward_enable )
                {
                    if ( appData.timeout_taking_reward > 0 )
                    {
#if defined ( USE_UART1_SERIAL_INTERFACE )
                        printf( "\tReward disabled, waiting reward timeout to end.\n" );
#endif 
                        while ( false == isDelayMsEnding( ) );
                    }
                    else
                    {
#if defined ( USE_UART1_SERIAL_INTERFACE )
                        printf( "\tReward disabled. No timeout reward.\n" );
#endif 
                    }

                    EX_INT1_InterruptDisable( );
                    EX_INT1_PositiveEdgeSet( );
                    EX_INT1_InterruptFlagClear( );
                    clear_ir1_sensor( );

                    appData.state = APP_STATE_CLOSING_DOOR;
                    break;
                }
            }

            if ( ( true == g_flag_ir1_sensor ) && ( false == appData.bird_is_taking_reward ) )
            {
                EX_INT1_InterruptDisable( );
                EX_INT1_NegativeEdgeSet( );
                EX_INT1_InterruptFlagClear( );
                EX_INT1_InterruptEnable( );
                clear_ir1_sensor( );

                /* REWARD_IR_SENSOR true. */
#if defined ( USE_UART1_SERIAL_INTERFACE )
                printf( "\tTaking reward detected.\n" );
#endif
                appData.bird_is_taking_reward = true;
                break;
            }

            /* low --> Breaking of the infrared barrier */
            if ( ( true == g_flag_ir1_sensor ) && ( true == appData.bird_is_taking_reward ) )
            {
#if defined ( USE_UART1_SERIAL_INTERFACE )
                printf( "\tReward taken.\n" );
#endif
                EX_INT1_InterruptDisable( );
                EX_INT1_PositiveEdgeSet( );
                EX_INT1_InterruptFlagClear( );
                clear_ir1_sensor( );

                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_REWARD_TAKEN );
                }

                appData.bird_is_taking_reward = false;
                appDataLog.is_reward_taken = true;
                appData.state = APP_STATE_CLOSING_DOOR;
                break;
            }

            /* Timeout elapsed and reward is not taken */
            if ( true == isDelayMsEnding( ) )
            {

                if ( false == appData.bird_is_taking_reward )
                {
                    /* Log event if required */
                    if ( true == appDataLog.log_events )
                    {
                        store_event( OF_REWARD_TIMEOUT );
                    }

#if defined ( USE_UART1_SERIAL_INTERFACE )
                    printf( "\tReward timeout.\n" );
#endif
                    EX_INT1_InterruptDisable( );
                    EX_INT1_PositiveEdgeSet( );
                    EX_INT1_InterruptFlagClear( );
                    clear_ir1_sensor( );

                    appData.state = APP_STATE_CLOSING_DOOR;
                }
                else
                {
                    ++num_timeout_reward;

                    /* Log event if required */
                    if ( true == appDataLog.log_events )
                    {
                        store_event( OF_REWARD_TIMEOUT_WRONG );
                    }

#if defined ( USE_UART1_SERIAL_INTERFACE )
                    printf( "\tReward timeout but something wrong (%d/%d).\n", num_timeout_reward, MAX_NUM_REWARD_TIMEOUT );
#endif  
                    if ( num_timeout_reward >= MAX_NUM_REWARD_TIMEOUT )
                    {
                        appData.state = APP_STATE_CLOSING_DOOR;
                        EX_INT1_InterruptDisable( );
                        EX_INT1_PositiveEdgeSet( );
                        EX_INT1_InterruptFlagClear( );
                        clear_ir1_sensor( );
                    }
                    else
                    {
                        setDelayMs( appData.timeout_taking_reward );
                    }
                }
            }
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_CLOSING_DOOR:
            
            /*
             * Closing door.
             * 
             * Process:
             * 
             * Next state: 
             */
            
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf( "> APP_STATE_CLOSING_REWARD_DOOR\n" );
#endif
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_STATE_CLOSING_DOOR );
                }

                /* Enregistrement de l'heure de fin de détection de l'oiseau. */
                while ( !RTCC_TimeGet( &appDataLog.bird_quit_time ) )
                {
                    Nop( );
                }

                /* Skip closing door if the "remain open" flag is set */
                if ( 1 == appDataDoor.remain_open )
                {
                    IRSensorDisable( );
                    EX_INT1_InterruptDisable( );
                    EX_INT1_PositiveEdgeSet( );
                    EX_INT1_InterruptFlagClear( );
                    clear_ir1_sensor( );
                    // appDataUsb.getValidDeviceAdress = false;
                    appData.state = APP_STATE_DATA_LOG;
                    break;
                }

                /* Optional delay before closing the door */
                if ( appDataDoor.close_delay > 0 )
                {
                    setDelayMs( appDataDoor.close_delay );
                    while ( false == isDelayMsEnding( ) )
                    {
                        Nop( );
                    }
                }

                setDelayMsStandBy( appData.timeout_guillotine );

            }

            /* Servomotor power command enable. */
            servomotorPowerEnable( );

            appDataServo.ton_cmd = servomotorGetDoorPosition( );
            appDataServo.ton_goal = appDataServo.ton_min;

            if ( appDataServo.ton_cmd != appDataServo.ton_goal )
            {
                appDataDoor.reward_door_status = DOOR_MOVING;
                while ( DOOR_MOVED != appDataDoor.reward_door_status )
                {
                    /* Check if the bird puts its head in during door close */
                    if ( PATCH_PROBABILITY != appData.scenario_number &&
                         1 == appData.reward_enable && 1 == BAR_IR1_OUT_GetValue( ) )
                    {
                        if ( true == appData.secu_bird_reward_reopen )
                        {
                            appDataDoor.reward_door_status = DOOR_MOVED;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_SERVO_INFO)
                            printf( "\tBird puts its head in during door close => reopen\n" );
#endif  
                            /* Log event if required */
                            if ( true == appDataLog.log_events )
                            {
                                store_event( OF_BIRD_X_REWARD_REOPEN );
                            }
                            appData.state = APP_STATE_REOPEN_DOOR;
                            break;
                        }
                        else
                        {
                            /* Log event if required */
                            if ( true == appDataLog.log_events )
                            {
                                store_event( OF_BIRD_X_REWARD_NO_REOPEN );
                            }
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_SERVO_INFO)
                            printf( "\tBird puts its head in during door close but reopen security disable\n" );
#endif  
                        }
                    }
                    /* Check if door take too much time to close */
                    if ( true == isDelayMsEndingStandBy( ) )
                    {
                        /* Log event if required */
                        if ( true == appDataLog.log_events )
                        {
                            store_event( OF_DOOR_CLOSE_SLOW_REOPEN );
                        }
                        appDataDoor.reward_door_status = DOOR_MOVED;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_SERVO_INFO)
                        printf( "\tDoor take too much time to close => reopen\n" );
#endif  
                        appData.state = APP_STATE_REOPEN_DOOR;
                        break;
                    }
                }
            }

            if ( APP_STATE_REOPEN_DOOR == appData.state )
            {
                break;
            }

            appDataDoor.reward_door_status = DOOR_CLOSED;

            uint16_t pos = servomotorGetDoorPosition( );

            if ( pos > appDataServo.ton_goal && ( pos - appDataServo.ton_goal ) > 150 )
            {
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_SERVO_FAR_CLOSE_POS_REOPEN );
                }
                appDataDoor.reward_door_status = DOOR_MOVED;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_SERVO_INFO)
                printf( "\tServo too far from goal position (%u to %u) => reopen\n", pos, appDataServo.ton_goal );
#endif  
                appData.state = APP_STATE_REOPEN_DOOR;
                break;
            }

            /* Servomotor power command disable. */
            servomotorPowerDisable( );

#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_SERVO_INFO)
            printf( "\tDoor closed\n" );
#endif
            if ( 1 == appData.reward_enable )
            {
                IRSensorDisable( );
                EX_INT1_InterruptDisable( );
                EX_INT1_PositiveEdgeSet( );
                EX_INT1_InterruptFlagClear( );
                clear_ir1_sensor( );
            }

            appDataDoor.num_reopen_attempt = 0;

            appData.state = APP_STATE_DATA_LOG;
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_REOPEN_DOOR:
            
            /*
             * Reopen door.
             * 
             * Process:
             * 
             * Next state: 
             */
            
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf( "> APP_STATE_REOPEN_DOOR\n" );
#endif          
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_STATE_REOPEN_DOOR );
                }

                OC5R = appDataServo.ton_max;
                setDelayMs( 2000 );
                while ( false == isDelayMsEnding( ) )
                {
                    Nop( );
                }

                appDataDoor.reward_door_status = DOOR_MOVED;

                appDataDoor.num_reopen_attempt += 1;

                /* Timeout before going to error. */
                setDelayMs( 10000 );

            }

            if ( true == isDelayMsEnding( ) )
            {
                sprintf( appError.message, "Unable to close the door after %d seconds", 10 );
                appError.current_line_number = __LINE__;
                sprintf( appError.current_file_name, "%s", __FILE__ );
                appError.number = ERROR_DOOR_CANT_CLOSE;
                appData.state = APP_STATE_ERROR;
                break;
            }

            if ( DOOR_MOVED == appDataDoor.reward_door_status )
            {
                if ( MAX_NUM_DOOR_REOPEN_ATTEMPT < appDataDoor.num_reopen_attempt )
                {
                    sprintf( appError.message, "Unable to close the door after %d attempts", MAX_NUM_DOOR_REOPEN_ATTEMPT );
                    appError.current_line_number = __LINE__;
                    sprintf( appError.current_file_name, "%s", __FILE__ );
                    appError.number = ERROR_DOOR_CANT_CLOSE;
                    appData.state = APP_STATE_ERROR;
                }
                else
                {
                    if ( ( 0 == appData.reward_enable ) || ( 0 == BAR_IR1_OUT_GetValue( ) ) )
                    {
                        appData.state = APP_STATE_CLOSING_DOOR;
                    }
                }
            }
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_DATA_LOG:
            
            /*
             * Datalogger.
             * 
             * Process:
             * 
             * Next state: 
             */
            
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf( "> APP_STATE_DATA_LOG\n" );
#endif
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_STATE_DATA_LOG );
                }
            }

            if ( false == dataLog( true ) )
            {
                appData.state = APP_STATE_ERROR;
            }
            else
            {
                appData.state = APP_STATE_IDLE;
            }
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_REMOVE_USB_DEVICE:
            
            /*
             * Remove USB device.
             * 
             * Process:
             * 
             * Next state: 
             */
            
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;

#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf( "> APP_STATE_REMOVE_USB_DEVICE\n" );
#endif
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf( "\t/!\\ Don't remove the USB device yet.\n" );
#endif
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_STATE_REMOVE_USB_DEVICE );
                }

                /* Unmount drive on USB device before power it off. */
                if ( USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status )
                {
                    usbUnmountDrive( );
                }

                USBHostShutdown( );

                setDelayMs( 2000 );
                while ( 0 == isDelayMsEnding( ) );

                powerUsbRfidDisable( );

                setLedsStatusColor( LED_GREEN );
#if defined ( USE_UART1_SERIAL_INTERFACE )
                printf( "\tUSB device can be safely removed\r\n" );
                printf( "\tThe system will automatically restart in 60s\r\n" );
                printf( "\tThe serial communication is off until the system reset\r\n" );
#endif
                setDelayMsStandBy( 60000 );

            }

            if ( true == isDelayMsEndingStandBy( ) )
            {
                appData.dsgpr0.bit_value.num_software_reset = 0;
                DSGPR0 = appData.dsgpr0.reg;
                DSGPR0 = appData.dsgpr0.reg;

                __asm__ volatile ( "reset" );
            }

            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_FLUSH_DATA_TO_USB:
            
            /*
             * Flush data to USB device.
             * 
             * Process:
             * 
             * Next state: 
             */
            
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf( "> APP_STATE_FLUSH_DATA_TO_USB\n" );
#endif
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_STATE_FLUSH_DATA_TO_USB );
                }
                
                /* Disable PIR interruption */
                EX_INT0_InterruptDisable( );
                
                /* Disable RTC alarm */
                rtcc_stop_alarm( );
                
                setLedsStatusColor( LED_USB_ACCESS );

                if ( appDataLog.num_data_stored > 0 ||
                     appDataLog.num_battery_level_stored > 0 ||
                     appDataLog.num_rfid_freq_stored > 0 ||
                     appDataEvent.num_events_stored > 0 ||
                     appDataLog.num_ds3231_temp_stored > 0 )
                {
                    /* Log data on USB device */
                    appDataUsb.is_device_needed = true;
                    powerUsbRfidEnable( );
                }
                else
                {
#if defined ( USE_UART1_SERIAL_INTERFACE )  
                    printf( "\tNo data to flush.\n" );
                    appData.state = APP_STATE_REMOVE_USB_DEVICE;
                    break;
#endif
                }

                setDelayMs( MAX_DELAY_TO_DETECT_USB_DEVICE );

            }

            if ( appDataUsb.is_device_address_available )
            {
                if ( FLUSH_DATA_ON_USB_DEVICE_SUCCESS != flushDataOnUsbDevice( ) )
                {
                    appData.state = APP_STATE_ERROR;
                    break;
                }
            }
            else
            {
                if ( true == isDelayMsEnding( ) )
                {
                    appDataUsb.is_device_needed = false;
                    sprintf( appError.message, "USB device detection took more than %ld ms", ( long ) MAX_DELAY_TO_DETECT_USB_DEVICE );
                    appError.current_line_number = __LINE__;
                    sprintf( appError.current_file_name, "%s", __FILE__ );
                    appError.number = ERROR_USB_DEVICE_NOT_FOUND;
                    appData.state = APP_STATE_ERROR;
                    break;
                }
                /* Blue and yellow status LEDs blink as long USB key is required. */
                LedsStatusBlink( LED_USB_ACCESS, LED_YELLOW, 500, 500 );
                break;
            }

            appDataUsb.is_device_needed = false;
            appData.state = APP_STATE_REMOVE_USB_DEVICE;
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_TEST_RFID:
            
            /*
             * Test RFID.
             * 
             * Process:
             * 
             * Next state: 
             */

            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf( "> APP_STATE_TEST_RFID\n" );
#endif
#if defined ( USE_UART1_SERIAL_INTERFACE ) 
                printf( "\t/!\\ Debug not available in RFID test mode.\n\tRFID test is launched for 60s.\n\tPress the user button to stop the test.\n" );
#endif

                initAttractiveLeds( );
                setAttractiveLedsOn( );

                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_STATE_TEST_RFID );
                }

                setDelayMsStandBy( 60000 );

                /* Debounce à la noix */
                setDelayMs( 1000 );
                while ( false == isDelayMsEnding( ) );

                button_user_state = BUTTON_NOT_PRESSED;

                //                for (i=0;i<3;i++)
                //                {
                //                    setLedsStatusColor( LEDS_ON );
                //                    setDelayMs( 500 );
                //                    while ( 0 == isDelayMsEnding( ) ); 
                //                    setLedsStatusColor( LEDS_OFF );
                //                    setDelayMs( 500 );
                //                    while ( 0 == isDelayMsEnding( ) ); 
                //                }

                appData.test_rfid = true;

                APP_Rfid_Init( );

            }

            APP_Rfid_Task( );

            /* Enable interruption see case RFID_DETECT_COMPLET_DATASTREAM
             * in APP_Rfid_Task */
            EX_INT4_InterruptEnable( );

            if ( appData.rfid_signal_detected )
            {
                if ( appData.flags.bit_value.new_valid_pit_tag )
                {
                    switch ( appData.scenario_number )
                    {
                        case NO_SCENARIO:
                        case OPEN_BAR:
                            /* No PIT tag denied */
                            appDataLog.is_pit_tag_denied = false;
                            break;

                        case DOOR_HABITUATION:
                            /* No PIT tag denied */
                            appDataLog.is_pit_tag_denied = false;
                            break;

                        case LONG_TERM_SPATIAL_MEMORY:
                            /* Check if PIT tag is denied */
                            appDataLog.is_pit_tag_denied = isPitTagDenied( );
                            break;

                        case WORKING_SPATIAL_MEMORY:
                            /* Check if PIT tag is denied */
                            appDataLog.is_pit_tag_denied = isPitTagDenied( );
                            /* Bird is allowed only one time */
                            appDataPitTag.is_pit_tag_denied_or_color_A[appDataPitTag.pit_tag_index_in_list] = true;
                            break;

                        case COLOR_ASSOCIATIVE_LEARNING:
                            appDataLog.is_pit_tag_denied = false;
                            /* Check if PIT tag is denied */
                            if ( appDataPitTag.pit_tag_index_in_list >= appDataPitTag.num_pit_tag_denied_or_color_A && appDataLog.attractive_leds_current_color_index == ATTRACTIVE_LEDS_COLOR_A )
                            {
                                appDataLog.is_pit_tag_denied = true;
                            }
                            if ( appDataPitTag.pit_tag_index_in_list < appDataPitTag.num_pit_tag_denied_or_color_A && appDataLog.attractive_leds_current_color_index == ATTRACTIVE_LEDS_COLOR_B )
                            {
                                appDataLog.is_pit_tag_denied = true;
                            }
                            break;

                        case RISK_AVERSION:
                            /* Check if PIT tag is denied */
                            appDataLog.is_pit_tag_denied = isPitTagDenied( );
                            break;
                    }

                    if ( true == appDataLog.is_pit_tag_denied )
                    {
                        // LED 1 off
                        setOneAttractiveLedColor( 1, 0, 0, 0 );
                        // LED 2 off red
                        setOneAttractiveLedColor( 2, 35, 0, 0 );
                    }
                    else
                    {
                        // LED 1 off
                        setOneAttractiveLedColor( 1, 0, 0, 0 );
                        // LED 2 green
                        setOneAttractiveLedColor( 2, 0, 35, 0 );
                    }
                }
                else
                {
                    // LED 1 blue
                    setOneAttractiveLedColor( 1, 0, 0, 35 );
                    // LED 2 off
                    setOneAttractiveLedColor( 2, 0, 0, 0 );
                }

                appData.rfid_signal_detected = false;

            }
            else
            {
                // LED 1 off
                setOneAttractiveLedColor( 1, 0, 0, 0 );
                // LED 2 off
                setOneAttractiveLedColor( 2, 0, 0, 0 );
            }

            button_user_state = USER_BUTTON_GetValue( );

            if ( BUTTON_PRESSED == button_user_state || true == isDelayMsEndingStandBy( ) )
            {

#if defined ( USE_UART1_SERIAL_INTERFACE ) 
                printf( "\tQuit RFID test mode\n\tBack to serial communication mode in 3s.\n" );
#endif
                RFID_Disable( );

                appData.test_rfid = false;

                for ( i = 0; i < 3; i++ )
                {
                    setLedsStatusColor( LEDS_ON );
                    setDelayMs( 500 );
                    while ( 0 == isDelayMsEnding( ) );
                    setLedsStatusColor( LEDS_OFF );
                    setDelayMs( 500 );
                    while ( 0 == isDelayMsEnding( ) );
                }
//                appData.state = APP_STATE_IDLE;
                appData.state = APP_STATE_SERIAL_COMMUNICATION;
            }

            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_SLEEP:
            /**
             * Application sleep state.
             *  - mise hors service de l'ensemble des fonctions hormis le capteur PIR
             *  - mise en sommeil simple du système
             *    OPERATION DURING SLEEP MODES - PIR sensor and alarm deep sleep
             *  - lors de la sortie du mode SLEEP, remettre en service les fonctions de l'OpenFeeder
             *  - passer à l'état APP_STATE_IDLE
             */
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined ( DISPLAY_CURRENT_STATE )
                printf( "> APP_STATE_SLEEP\n" );
#endif
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_STATE_SLEEP );
                }

                /* Stop RTCC interuption during shutdown process */
                rtcc_stop_alarm( );

                /* Close the door if it is opened */
                if ( DOOR_CLOSED_AT_NIGHT != appDataDoor.reward_door_status )
                {
                    /* Servomotor power command enable. */
                    servomotorPowerEnable( );

                    appDataServo.ton_cmd = servomotorGetDoorPosition( );
                    appDataServo.ton_goal = appDataServo.ton_min_night;

                    if ( appDataServo.ton_cmd != appDataServo.ton_goal )
                    {
                        appDataDoor.reward_door_status = DOOR_MOVING;
                        while ( DOOR_MOVED != appDataDoor.reward_door_status );
                    }

                    appDataDoor.reward_door_status = DOOR_CLOSED_AT_NIGHT;

                    /* Servomotor power command disable. */
                    servomotorPowerDisable( );
                }

                /* Set peripherals Off. */
                setAttractiveLedsOff( );
                powerPIRDisable( );
                EX_INT0_InterruptDisable( );
                RFID_Disable( );
                IRSensorDisable( );
                appDataUsb.is_device_needed = false;
                /* Log data on USB device */
                powerUsbRfidEnable( );
                appDataUsb.is_device_needed = true;

            }

            if ( true == appDataUsb.is_device_needed )
            {
                if ( appDataUsb.is_device_address_available )
                {
//                    if ( USB_DRIVE_NOT_MOUNTED == usbMountDrive( ) )
//                    {
//                        appDataUsb.is_device_needed = false;
//                        appData.state = APP_STATE_ERROR;
//                        break;
//                    }
                    

//                    /* Log data if required */
//                    if ( appDataLog.num_data_stored > 0 )
//                    {
//                        setLedsStatusColor( LED_USB_ACCESS );
//                        /* Force data to be written on the USB device */
//                        appDataLog.num_data_stored = MAX_NUM_DATA_TO_STORE;
//                        if ( false == dataLog( false ) )
//                        {
//                            appDataUsb.is_device_needed = false;
//                            appData.state = APP_STATE_ERROR;
//                            break;
//                        }
//                    }
//
//                    /* Log battery level if required */
//                    if ( true == appDataLog.log_battery && appDataLog.num_battery_level_stored > 0 )
//                    {
//                        setLedsStatusColor( LED_USB_ACCESS );
//                        if ( FILEIO_RESULT_FAILURE == logBatteryLevel( ) )
//                        {
//                            appDataUsb.is_device_needed = false;
//                            appData.state = APP_STATE_ERROR;
//                            break;
//                        }
//                    }
//
//                    /* Log RFID frequency if required */
//                    if ( true == appDataLog.log_rfid && appDataLog.num_rfid_freq_stored > 0 )
//                    {
//                        setLedsStatusColor( LED_USB_ACCESS );
//                        if ( FILEIO_RESULT_FAILURE == logRfidFreq( ) )
//                        {
//                            appDataUsb.is_device_needed = false;
//                            appData.state = APP_STATE_ERROR;
//                            break;
//                        }
//                    }
//
//                    /* Log event if required */
//                    if ( true == appDataLog.log_events && appDataEvent.num_events_stored > 0 )
//                    {
//                        setLedsStatusColor( LED_USB_ACCESS );
//                        if ( FILEIO_RESULT_FAILURE == logEvents( ) )
//                        {
//                            appDataUsb.is_device_needed = false;
//                            appData.state = APP_STATE_ERROR;
//                            break;
//                        }
//                    }

//                    /* Unmount drive on USB device before power it off. */
//                    if ( USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status )
//                    {
//                        usbUnmountDrive( );
//                    }
                    
                    if ( FLUSH_DATA_ON_USB_DEVICE_SUCCESS != flushDataOnUsbDevice( ) )
                    {
                        /* Unmount drive on USB device before power it off. */
                        if ( USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status )
                        {
                            usbUnmountDrive( );
                        }
                    }

                    USBHostShutdown( );
                    powerUsbRfidDisable( );
                }
                else
                {
                    break;
                }
            }

            /* Turn status LED off */
            setLedsStatusColor( LEDS_OFF );

#if defined ( TEST_RTCC_SLEEP_WAKEUP )
            /* Next line for debugging sleep/wakeup only */
            /* Should be commented in normal mode */
            /* Modify time value according to wake up values in the CONFIG.INI file */
            setDateTime( 17, 9, 21, 5, 59, 50 );
#endif
            /* Set alarm for wake up time */
            rtcc_set_alarm( appDataAlarmWakeup.time.tm_hour, appDataAlarmWakeup.time.tm_min, appDataAlarmWakeup.time.tm_sec, EVERY_DAY );

            rtcc_start_alarm( );

            setDelayMs( 500 );
            while ( false == isDelayMsEnding( ) )
            {
                Nop( );
            }

            appData.dsgpr0.bit_value.num_software_reset += 1;

            /* The repeat sequence (repeating
            the instruction twice) is required to write to
            any of the Deep Sleep registers (DSCON,
            DSWAKE, DSGPR0, DSGPR1).
            PIC24FJ256GB406 datasheet page 200 */
            DSGPR0 = appData.dsgpr0.reg;
            DSGPR0 = appData.dsgpr0.reg;
            DSGPR1 = appData.dsgpr1.reg;
            DSGPR1 = appData.dsgpr1.reg;

#if defined ( ENABLE_DEEP_SLEEP )
            DSCONbits.DSEN = 1;
            DSCONbits.DSEN = 1;
#endif
            Sleep( );

            appData.state = APP_STATE_WAKE_UP;
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_WAKE_UP:
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined ( DISPLAY_CURRENT_STATE )
                printf( "> APP_STATE_WAKE_UP_FROM_SLEEP\n" );
#endif
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_STATE_WAKE_UP );
                }
            }

#if defined ( TEST_RTCC_SLEEP_WAKEUP )
            /* Next line for debugging sleep/wakeup only */
            /* Should be commented in normal mode */
            /* Modify time value according to sleep values in the CONFIG.INI file */
            setDateTime( 17, 9, 21, 22, 59, 50 );
#endif

            rtcc_set_alarm( appDataAlarmWakeup.time.tm_hour, appDataAlarmWakeup.time.tm_min, appDataAlarmWakeup.time.tm_sec, EVERY_SECOND );

            APP_Initialize( );
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_FLUSH_DATA_BEFORE_ERROR:
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf( "> APP_STATE_FLUSH_DATA_BEFORE_ERROR\n" );
#endif
                appError.is_data_flush_before_error = true;

                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_STATE_FLUSH_DATA_BEFORE_ERROR );
                }

//                if ( appDataLog.num_data_stored == 0 &&
//                     appDataLog.num_battery_level_stored == 0 &&
//                     appDataLog.num_rfid_freq_stored == 0 &&
//                     appDataEvent.num_events_stored == 0 )
//                {
//#if defined ( USE_UART1_SERIAL_INTERFACE )
//                    printf( "\t No data stored.\n" );
//#endif
//                    appData.state = APP_STATE_ERROR;
//                    break;
//                }

                appDataUsb.is_device_needed = true;
                /* Log data on USB device */
                powerUsbRfidEnable( );

                setDelayMs( MAX_DELAY_TO_DETECT_USB_DEVICE );
            }

            if ( appDataUsb.is_device_address_available )
            {
                if ( USB_DRIVE_NOT_MOUNTED == usbMountDrive( ) )
                {
                    appDataUsb.is_device_needed = false;
                    appData.state = APP_STATE_ERROR;
                    break;
                }

                if ( appDataLog.num_data_stored > 0 && true == appDataLog.is_file_name_set )
                {
                    setLedsStatusColor( LED_USB_ACCESS );
                    /* Force data to be written on the USB device */
                    appDataLog.num_data_stored = MAX_NUM_DATA_TO_STORE;
                    if ( false == dataLog( false ) )
                    {
                        appDataUsb.is_device_needed = false;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }

//                if ( true == appDataLog.log_battery && appDataLog.num_battery_level_stored > 0 )
                if ( appDataLog.num_battery_level_stored > 0 )
                {
                    setLedsStatusColor( LED_USB_ACCESS );
                    if ( FILEIO_RESULT_FAILURE == logBatteryLevel( ) )
                    {
                        appDataUsb.is_device_needed = false;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }

//                if ( true == appDataLog.log_rfid && appDataLog.num_rfid_freq_stored > 0 )
                if ( appDataLog.num_rfid_freq_stored > 0 )
                {
                    setLedsStatusColor( LED_USB_ACCESS );
                    if ( FILEIO_RESULT_FAILURE == logRfidFreq( ) )
                    {
                        appDataUsb.is_device_needed = false;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }

//                if ( true == appDataLog.log_errors )
                if ( ERROR_NONE < appError.number )
                {
                    setLedsStatusColor( LED_USB_ACCESS );
                    if ( FILEIO_RESULT_FAILURE == logError( ) )
                    {
                        appDataUsb.is_device_needed = false;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }

                /* Log event if required */
//                if ( true == appDataLog.log_events && appDataEvent.num_events_stored > 0 )
                if ( appDataEvent.num_events_stored > 0 )
                {
                    if ( true == appDataEvent.is_bin_file_name_set  || true == appDataEvent.is_txt_file_name_set )
                    {
                        setLedsStatusColor( LED_USB_ACCESS );
                        if ( FILEIO_RESULT_FAILURE == logEvents( ) )
                        {
                            appDataUsb.is_device_needed = false;
                            appData.state = APP_STATE_ERROR;
                            break;
                        }                        
                    }                    
                }

                if ( USB_DRIVE_MOUNTED == usbUnmountDrive( ) )
                {
                    appData.state = APP_STATE_ERROR;
                }

            }
            else
            {
                if ( true == isDelayMsEnding( ) )
                {
                    appDataUsb.is_device_needed = false;
                    sprintf( appError.message, "USB device detection took more than %ldms", ( long ) MAX_DELAY_TO_DETECT_USB_DEVICE );
                    appError.current_line_number = __LINE__;
                    sprintf( appError.current_file_name, "%s", __FILE__ );
                    appError.number = ERROR_USB_DEVICE_NOT_FOUND;
                    appData.state = APP_STATE_ERROR;
                    break;
                }
                /* Blue and yellow status LEDs blink as long USB key is required. */
                LedsStatusBlink( LED_USB_ACCESS, LED_YELLOW, 500, 500 );
                break;
            }

            setLedsStatusColor( LEDS_OFF );
            appData.state = APP_STATE_ERROR;
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_ERROR:
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf( "> APP_STATE_ERROR\n" );
#endif
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_STATE_ERROR );
                }

                /* If the current error si not related to the USB device, try to log the error */
                if ( appError.number < ERROR_USB || appError.number > ERROR_USB_SUSPEND_DEVICE )
                {
                    /* Flush data on USB device if necessary */
                    if ( false == appError.is_data_flush_before_error )
                    {
                        appData.state = APP_STATE_FLUSH_DATA_BEFORE_ERROR;
                        break;
                    }
                }

                while ( !RTCC_TimeGet( &appError.time ) )
                {
                    Nop( );
                }

                switch ( appError.number )
                {
                    case ERROR_LOW_BATTERY:
                        appError.led_color_2 = LEDS_ERROR_CRITICAL_BATTERY;
                        break;
                    case ERROR_LOW_FOOD:
                        appError.led_color_2 = LEDS_ERROR_CRITICAL_FOOD;
                        break;
                    case ERROR_LOW_VBAT:
                        appError.led_color_2 = LEDS_ERROR_CRITICAL_VBAT;
                        break;
                    case ERROR_DOOR_CANT_CLOSE:
                        appError.led_color_2 = LEDS_ERROR_CRITICAL_DOOR;
                        break;
                    case ERROR_LOW_RFID_FREQUENCY:
                        appError.led_color_2 = LEDS_ERROR_RFID_FREQ;
                        break;
                    case ERROR_TIMEOUT_RFID_FREQUENCY:
                        appError.led_color_2 = LEDS_ERROR_RFID_TIMEOUT;
                        break;
                    default:
                        appError.led_color_2 = LEDS_OFF;
                        break;
                }

                if ( appError.number > ERROR_TOO_MANY_SOFTWARE_RESET )
                {
                    appError.led_color_2 = LEDS_TOO_MANY_SOFTWARE_RESET;
                }

#if defined ( USE_UART1_SERIAL_INTERFACE )
                printError( );
#endif
                rtcc_stop_alarm( );

                /* Close the door if it is opened */
                if ( ERROR_DOOR_CANT_CLOSE != appError.number && true == appData.flags.bit_value.system_init )
                {
                    /* Servomotor power command enable. */
                    servomotorPowerEnable( );

                    appDataServo.ton_cmd = servomotorGetDoorPosition( );
                    appDataServo.ton_goal = appDataServo.ton_min_night;

                    if ( appDataServo.ton_cmd != appDataServo.ton_goal )
                    {
                        appDataDoor.reward_door_status = DOOR_MOVING;
                        while ( DOOR_MOVED != appDataDoor.reward_door_status );
                    }

                    appDataDoor.reward_door_status = DOOR_CLOSED_AT_NIGHT;

                    /* Servomotor power command disable. */
                    servomotorPowerDisable( );
                }

                /* Set peripherals Off */
                setAttractiveLedsOff( );
                powerPIRDisable( );
                EX_INT0_InterruptDisable( );
                RFID_Disable( );
                IRSensorDisable( );
                servomotorPowerDisable( );

                /* Unmount drive on USB device before power it off. */
                if ( USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status )
                {
                    usbUnmountDrive( );
                }
                USBHostShutdown( );
                powerUsbRfidDisable( );

                /* Reset the system after DELAY_BEFORE_RESET milli-seconds if non critical error occurred */
                if ( appError.number > ERROR_CRITICAL && appError.number < ERROR_TOO_MANY_SOFTWARE_RESET )
                {
                    if ( MAX_NUM_RESET <= appData.dsgpr0.bit_value.num_software_reset )
                    {
#if defined ( USE_UART1_SERIAL_INTERFACE ) 
                        printf( "\t/!\\ The system reset %u times => Critical error\n", MAX_NUM_RESET );
#endif                       
                        appError.number = appError.number + ERROR_TOO_MANY_SOFTWARE_RESET;
                        appData.previous_state = APP_STATE_IDLE;
                        break;
                    }

                    TMR3_Start( );
                    setDelayMs( DELAY_BEFORE_RESET );
#if defined ( USE_UART1_SERIAL_INTERFACE ) 
                    printf( "\t/!\\ The system will reset in %ums\n", DELAY_BEFORE_RESET );
#endif
                }
                else
                {
                    rtcc_set_alarm( 0, 0, 0, EVERY_10_SECONDS );
                    rtcc_start_alarm( );
                }
            }

            if ( appError.number > ERROR_CRITICAL && appError.number < ERROR_TOO_MANY_SOFTWARE_RESET )
            {
                /* A non critical error occurred                      */
                /* Wait DELAY_BEFORE_RESET milli-seconds and reset    */
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined (ENABLE_ERROR_LEDS)
                LedsStatusBlink( appError.led_color_1, appError.led_color_2, 150, 150 );
#endif
                if ( isDelayMsEnding( ) )
                {

                    appData.dsgpr0.bit_value.num_software_reset += 1;
                    DSGPR0 = appData.dsgpr0.reg;
                    DSGPR0 = appData.dsgpr0.reg;

                    TMR3_Stop( );

                    __asm__ volatile ( "reset" );

                }
            }
            else
            {
                /* A critical error occured */
                /* Stop the openfeeder to save battery */
                /* Make status LEDs blink at long interval */

                Sleep( );

#if defined ( USE_UART1_SERIAL_INTERFACE )
                printError( );
#endif

#if defined (ENABLE_ERROR_LEDS)
                /* Status LED blinks */
                TMR3_Start( );
                setLedsStatusColor( appError.led_color_1 );
                setDelayMs( 150 );
                while ( 0 == isDelayMsEnding( ) );
                setLedsStatusColor( appError.led_color_2 );
                setDelayMs( 150 );
                while ( 0 == isDelayMsEnding( ) );
                setLedsStatusColor( LEDS_OFF );
                TMR3_Stop( );
#endif
            }
            break;
            /* -------------------------------------------------------------- */

        default:
            if ( false == enter_default_state )
            {
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf( "> DEFAULT_STATE\n" );
#endif
#if defined ( USE_UART1_SERIAL_INTERFACE ) 
                getDateTime( );
                printDateTime( appData.current_time );
#endif

                enter_default_state = true;

            }

            setLedsStatusColor( LED_RED );
            break;
    }
}


void APP_Initialize( void )
{
    int i;

    OC4_Stop( );
    OC5_Stop( );
    TMR4_Stop( );
    TMR2_Stop( );

    /* Attractive LEDs */
    setAttractiveLedsOff( );
    appDataAttractiveLeds.current_color_index = 0;
    appDataAttractiveLeds.alt_sec_elapsed = 0;
    appDataAttractiveLeds.red[0] = 0;
    appDataAttractiveLeds.green[0] = 0;
    appDataAttractiveLeds.blue[0] = 0;
    appDataAttractiveLeds.red[1] = 0;
    appDataAttractiveLeds.green[1] = 0;
    appDataAttractiveLeds.blue[1] = 0;

    /* APP state task */
    appData.state = APP_STATE_INITIALIZE;
    appData.previous_state = APP_STATE_ERROR;

    appData.need_to_reconfigure = false;

    appData.openfeeder_state = OPENFEEDER_IS_AWAKEN;
    appData.rtcc_alarm_action = RTCC_ALARM_WAKEUP_OPENFEEDER;

    /* Initialize all flags */
    appData.flags.reg = 0;

    /* I2C */
    memset( appData.i2c_add_found, 0, MAX_OF_UNKNOWN_I2C_8_BIT_SLAVE_ADD ); // clear tab i2c_add_found
    appData.mcp23017.status_reg = 0;
    /* Set all digit Off. */
    appData.digit[0] = 0xFF;
    appData.digit[1] = 0xFF;
    appData.digit[2] = 0xFF;
    appData.digit[3] = 0xFF;

    appData.button_pressed = BUTTON_READ; /* initialized button status */

    /* Data logger */
    appDataLog.is_file_name_set = false;
    appDataLog.num_char_buffer = 0;
    appDataLog.num_data_stored = 0;
    appDataLog.attractive_leds_current_color_index = 0;

    appDataLog.log_birds = true;
    appDataLog.log_udid = false;
    appDataLog.log_events = true;
    appDataLog.log_errors = false;
    appDataLog.log_battery = false;
    appDataLog.log_rfid = false;
    appDataLog.log_temp = false;

    appDataLog.did_door_open = false;

    appData.bird_is_taking_reward = false;

    /* USB host */
    appDataUsb.usb_drive_status = USB_DRIVE_NOT_MOUNTED;
    appDataUsb.is_device_address_available = false;
    appDataUsb.is_device_needed = false;

    /* PIT tag data */
    appDataPitTag.pit_tag_found_in_list = false;
    appDataPitTag. num_pit_tag_denied_or_color_A = 0;
    appDataPitTag.num_pit_tag_accepted_or_color_B = 0;
    appDataPitTag.pit_tag_index_in_list = 0;
    clearPitTagBuffers( );
    clearPitTagSringBuffers( );

    for ( i = 0; i < MAX_PIT_TAGS_LIST_NUMBER; ++i )
    {
        appDataPitTag.is_pit_tag_denied_or_color_A[i] = false;
    }

    appDataPitTag.timeout_unique_visit = 0;

    appDataLog.num_battery_level_stored = 0;
    appDataLog.num_rfid_freq_stored = 0;
    appDataLog.num_ds3231_temp_stored = 0;
    appDataLog.num_time_calib_stored = 0;

    appError.led_color_1 = LEDS_ERROR;
    appError.number = ERROR_NONE;

    appData.punishment_delay = 0;

    appData.rfid_signal_detected = false;

    appDataDoor.reward_door_status = DOOR_CLOSED_AT_NIGHT;
    appDataDoor.reward_probability = 1.0;

    CMD_3V3_RF_SetLow( );

    appData.servo_powered = false;
    appData.pir_sensor_powered = false;

    appDataServo.num_empty_step = 5;

    appData.test_rfid = false;

    for ( i = 0; i < 4; i++ )
    {
        appData.siteid[i] = 'X';
    }
    appData.siteid[4] = '\0';

    appData.scenario_number = 0;

    is_bird_detected = false;

    appDataEvent.file_type = EVENT_FILE_BINARY;

    appData.chk_food_level = false;

    appData.ext_temperature = 0.0;

    appData.punishment_state = false;

    appError.led_color_1 = LEDS_OFF;
    appError.led_color_2 = LEDS_OFF;
    appError.number = ERROR_NONE;
    appError.current_line_number = 0;
    appError.is_data_flush_before_error = false;
    
    appDataEvent.is_txt_file_name_set = false;
    appDataEvent.is_bin_file_name_set = false;
        
}


/*******************************************************************************
End of File
 */
