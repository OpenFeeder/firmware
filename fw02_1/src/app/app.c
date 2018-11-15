/** 
 * File: app.c
 * Author: OpenFeeder Team <https://github.com/orgs/OpenFeeder/people> 
 * Comments:
 * Revision history:
 */

/* Software */
// Docklight COM connections (RS232 terminal to communicate with USB Serial Port)
// Downloads: http://docklight.de/
// Lors de l'appuis dans Docklight sur la touche [Enter] le PC envoie 0D 0A soit 2 octets,
// dont 0A est interpréter comme la valeur suivante !
// --> pour résoudre cela dans Docklight, il faut appuyer sur la combinaison de touches
//     [Ctrl]+[Enter] pour n'envoyer que la valeur 0D (Carriage Return <CR>)
//     https://en.wikipedia.org/wiki/Control_character

// TODO: Documentation of the microcontroller program to achieve with the documentation generation tool "Doxygen"
/**
 * @file name.ext
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date dd/mm/yyyy
 */
/**
 * @brief Titre function
 * @param name usage
 */

// TODO: Pour la fermeture de la porte, deux cas sont possible:
//       - on détecte lorsque l'oiseau n'est plus dans l'ouverure (barrière IR non coupé + tempo ? ou  détection de l'oiseau qui part, lecture PIT Tag)
//       - ou tempo (paramétrable depuis fichier CONFIG.INI) dès que la barrière IR n'est plus coupé
// TODO: Datalogging with Timestamp: RTCC + ms ( https://github.com/OpenFeeder/project/tree/master/demos/pic24/apps/pic24fj128gb204_rtcc/demo_rtcc_mc3.X )
//       necessite de reboucler la sortie RTCC_OUT sur une entrée d'interruption externe
// TODO: faire clignotter rapidement la LED status blue lorsque l'on accède à la clé USB
// TODO: When no USB key the Blue LED status set High! Must only have Red LED toggle every 200 ms!
// TODO: Debugging RFID: voie 1 DMOD_OUT, voie 2 INT4 event on LED_STATUS_B, voie 3 read DMOD_OUT event from TMR4 on LED_STATUS_G
// TODO: Si il n'y a pas de clé USB la machine d'état reste bloqué en APP_STATE_USB_WAITING_FOR_ATTACH
//       après affichage du message de l'APP_STATE_INIT et 9
//       il faudrait mettre plusieurs machines d'états en parallèle USB, RFID, DATALOGGER, RTCC, I2C REMOTE CONTROL, ...
//       afin de pouvoir gérer depuis la machine d'états principale les différents modes.
// TODO: Mesure de la période de RDY/CLK, attention TMR3 n'est plus à 10 ms mais à 20 ms --> ajustement de 
// TODO: Repenser la michine d'états principale pour avoir seulement les process principaux et faire des machine d'états indépendante pour les différents modes
// TODO: Definir une convention de nommage comme pour "typedef struct" pour les différentes variables de chaque machine d'états
// TODO: Check datasheet "19.1.2.3 VBUS Voltage Generation with External Devices" p269
// TODO: Check the calibrated of the real-time crystal input (see "22.4 Calibration" DS30005009C-page 326)

// FIXME: in USB_HostInterruptHandler( ) occur every time!
//#if defined (DEBUG_UART)
//            printf( "USB_HostInterruptHandler()\n" ); // display Sleeping message
//#endif 

// DONE: Make Power Management (USB powered)
// DONE: Mettre en paramètre dans le fichier de config pour la valeur de g_timeout_taking_reward (waiting timeout in sec)
// DONE: CMD_VDD_APP_V_USB on RB5
// DONE: Mise en place la mise à jour de l'heure par l'interface de debug
// DONE: Servomotor Open and Close serial command do only one step, must reaped until ending position occur!
// DONE: Implement min, max value of servomotor position from CONFIG.INI file ?
// DONE: Activer le boitier télécommande lorsque celui-ci est détecté
//       Ajouter un test périodique du boitier télécommande et faire remonter une trace de l'évènement lorsque celle ci est branché ou débranché
//       ou lancer la détection du boitier télécommande lors de l'appuis sur le bouton User
// DONE: "UsbMountDrive\n" display first when pgm boot!


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
APP_DATA_PIT_TAG appDataPitTag;
APP_ERROR appError;
APP_DATA_USB appDataUsb;
APP_DATA_ALARM appDataAlarmSleep;
APP_DATA_ALARM appDataAlarmWakeup;
APP_DATA_LEDS appDataAttractiveLeds;
APP_DATA_LOG appDataLog;
APP_DATA_SERVO appDataServo; /* Servomotor application data. */
APP_DATA_RC appDataRc;
APP_DATA_EVENT appDataEvent;
APP_DATA_DOOR appDataDoor;

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
    bool flag;
    I2C1_MESSAGE_STATUS i2c_status;
    int i;
    static bool enter_default_state = false;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
    FILEIO_DRIVE_PROPERTIES drive_properties;
#endif
    
    static int num_timeout_reward;
    
    /* Check the Application State. */
    switch ( appData.state )
    {
        case APP_STATE_INITIALIZE:
        {
            /**
             * Initializing the application.
             * (en) Application initialization when starting the main power.
             * (fr) Initialisation de l'application lors du démarrage de l'alimentation principale.
             */
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_INITIALIZE\n" );
#endif
                if ( true == appDataLog.log_events )
                {
                    store_event(OF_STATE_INITIALIZE);
                }
            
            }

            calibrateDateTime( );
            
            /* Power PIR sensor early in the code because of starting delay before usable */
            powerPIREnable( );

            appData.state = APP_STATE_CONFIGURE_SYSTEM;
            break;
        }
            /* -------------------------------------------------------------- */

        case APP_STATE_CONFIGURE_SYSTEM:
            /* APP_STATE_CONFIGURE_SYSTEM run only one time. */
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_CONFIGURE_SYSTEM\n" );
#endif
                if ( true == appDataLog.log_events )
                {
                    store_event(OF_STATE_CONFIGURE_SYSTEM);
                }
                
                /* Set log file name => 20yymmdd.CSV (one log file per day). */
                if ( false == setLogFileName( ) )
                {
                    appDataUsb.key_is_nedded = false;
                    appData.state = APP_STATE_ERROR;
                    break;
                }
                
                powerUsbRfidEnable( );
                appDataUsb.key_is_nedded = true;

                setDelayMs(MAX_DELAY_TO_DETECT_USB_DEVICE);
                    
            }

            if ( appDataUsb.getValidDeviceAdress )
            {
                /* Configure the system if a USB key is plugged. */
                if ( USB_DRIVE_NOT_MOUNTED == usbMountDrive( ) )
                {
                    appDataUsb.key_is_nedded = false;
                    appData.state = APP_STATE_ERROR;
                    break;
                }
                
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
                drive_properties.new_request = true;
                do
                {
                    FILEIO_DrivePropertiesGet(&drive_properties, 'A');
                } while (drive_properties.properties_status == FILEIO_GET_PROPERTIES_STILL_WORKING);
                
                if (FILEIO_GET_PROPERTIES_NO_ERRORS == drive_properties.properties_status)
                {
                    printf("\tUSB device properties\n");
                    if (1 == drive_properties.results.disk_format)
                    {
                        printf("\t\tDrive format: FAT12\n");
                    }
                    else if (2 == drive_properties.results.disk_format)
                    {
                        printf("\t\tDrive format: FAT16\n");
                    }                        
                    else if (3 == drive_properties.results.disk_format)
                    {
                        printf("\t\tDrive format: FAT32\n");
                    }
                    else
                    {
                        printf("\t\tDrive format: unknown (%d)\n", drive_properties.results.disk_format);
                    }
                    printf("\t\tSector size: %u\n", drive_properties.results.sector_size);
                    printf("\t\tSector per cluster: %u\n", drive_properties.results.sectors_per_cluster);
                    printf("\t\tTotal clusters: %lu\n", drive_properties.results.total_clusters);
                    printf("\t\tFree clusters: %lu\n", drive_properties.results.free_clusters);                    
                    printf("\t\tTotal space: %lu MB\n", drive_properties.results.total_clusters*drive_properties.results.sectors_per_cluster*drive_properties.results.sector_size/1024/1024);
                    printf("\t\tFree space: %lu MB\n\n", drive_properties.results.free_clusters*drive_properties.results.sectors_per_cluster*drive_properties.results.sector_size/1024/1024);
                }
                else
                {
                    printf("\tUSB device properties\n\t\tGet properties failed (%d)\n\n", drive_properties.properties_status);
                }
#endif 
                
            }
            else
            {
                if ( true == isDelayMsEnding() )
                {
                    appDataUsb.key_is_nedded = false;
                    sprintf(appError.message, "USB device detection took more than %ld ms", (long)MAX_DELAY_TO_DETECT_USB_DEVICE);
                    appError.currentLineNumber = __LINE__;
                    sprintf(appError.currentFileName, "%s", __FILE__);
                    appError.number = ERROR_USB_DEVICE_NOT_FOUND;
                    appData.state = APP_STATE_ERROR;
                    break; 
                }
                /* Blue and yellow status LEDs blink as long USB key is required. */
                LedsStatusBlink( LED_USB_ACCESS, LED_YELLOW, 500, 500 );
                break;
            }

            setLedsStatusColor( LED_USB_ACCESS );

            if ( true == appDataLog.log_events )
            {
                if ( false == setEventFileName( ) )
                {
                    appDataUsb.key_is_nedded = false;
                    appData.state = APP_STATE_ERROR;
                    break;
                }
            }
            
            /* System configuration. */
            appData.flags.bit_value.systemInit = config_set( );

            if ( true == appData.flags.bit_value.systemInit )
            {
                
                srand(appData.current_time.tm_mon+appData.current_time.tm_mday+appData.current_time.tm_min+appData.current_time.tm_sec);
                
                /* Check all status LEDs */
                checkLedsStatus( );

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

                if ( appData.state != APP_STATE_CONFIGURE_SYSTEM )
                {
                    break;
                }

                if ( true == appDataLog.log_udid )
                {
                    logUdid();
                }
                
                setLedsStatusColor( LED_YELLOW );

                /* Servomotor power command enable. */
                servomotorPowerEnable( );

                appDataServo.ton_cmd = servomotorGetDoorPosition( );
                if ( true == appDataLog.log_events )
                {
                    store_event(OF_OPEN_DOOR);
                }

                /* Open the door */
                appDataServo.ton_goal = appDataServo.ton_max;
                if ( appDataServo.ton_cmd != appDataServo.ton_goal )
                {
                    appDataDoor.reward_door_status = DOOR_MOVING;
                    while ( DOOR_MOVED != appDataDoor.reward_door_status );
                }
                appDataDoor.reward_door_status = DOOR_OPENED;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_SERVO_POSITION)
                servomotorGetDoorPosition( );
#endif  
                if ( 0 == appDataDoor.remain_open )
                {
                    if ( true == appDataLog.log_events )
                    {
                        store_event(OF_CLOSE_DOOR);
                    }

                    /* Close the door */
                    appDataServo.ton_cmd = servomotorGetDoorPosition( );
                    /* Close the door */
                    appDataServo.ton_goal = appDataServo.ton_min;
                    if ( appDataServo.ton_cmd != appDataServo.ton_goal )
                    {
                        appDataDoor.reward_door_status = DOOR_MOVING;
                        while ( DOOR_MOVED != appDataDoor.reward_door_status );
                    }
                    appDataDoor.reward_door_status = DOOR_CLOSED;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_SERVO_POSITION)
                    servomotorGetDoorPosition( );
#endif  
                }

                /* Servomotor power command disable. */
                servomotorPowerDisable( );
         
                /* Initialize I2C device */
                /* Reset PCA9622 device */
                i2c_status = I2C1_PCA9622_SoftwareReset( );
                
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_I2C_STATUS)
                    print_I2C_message_status( i2c_status ); // I2C1_MESSAGE_STATUS
                    printf( "\n" );
#endif
                    
                if ( true == appData.flags.bit_value.attractive_leds_status )
                {
                    if ( initAttractiveLeds( ) )
                    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LEDS_STATUS)
                        printf( "\tAttractive LEDs: ok\n" ); 
#endif
                        setAttractiveLedsOn();
                        setAttractiveLedsNoColor();
                        testAttractiveLeds();
                        setAttractiveLedsNoColor();
                        setAttractiveLedsOff();

                    }
                    else
                    {
                        sprintf( appError.message, "Unable to initialize attractive LEDS via I2C" );
                        appError.currentLineNumber = __LINE__;
                        sprintf( appError.currentFileName, "%s", __FILE__ );
                        appError.number = ERROR_ATTRACTIVE_LED_INIT;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
          
                    if ( true == setAttractiveLedsIndex( ))
                    {
                        setAttractiveLedsOn();
                        setAttractiveLedsNoColor();
                        
                        if ( true == appDataLog.log_events )
                        {
                            store_event(OF_ATTRACTIVE_LEDS_CHECK_INDEX);
                        }
                        
                        for (i=0;i<4;i++)
                        {
                            setOneAttractiveLedColor( appDataAttractiveLeds.leds_index[i], 0, 35, 0);
                            setDelayMs( 1000 );
                            while ( 0 == isDelayMsEnding( ) );
                        }
                        setAttractiveLedsNoColor();
                        setAttractiveLedsOff();
                    }
//                    else
//                    {
//                        
//                    }

                    if ( GO_NO_GO == appData.scenario_number || COLOR_ASSOCIATIVE_LEARNING == appData.scenario_number)
                    {
                        getDateTime( );                    
                    }                

                    if ( GO_NO_GO == appData.scenario_number )
                    {
                        if ( ALL_LEDS == appDataAttractiveLeds.pattern_number )
                        {
                            appDataAttractiveLeds.pattern[0] = 0;
                            appDataAttractiveLeds.pattern[1] = 0;
                            appDataAttractiveLeds.pattern[2] = 0;
                            appDataAttractiveLeds.pattern[3] = 0;
                            if ( true == appDataLog.log_events )
                            {
                                store_event(OF_GO_NO_GO_ALL_ON);
                            }
                        }
                        else if ( LEFT_RIGHT_LEDS == appDataAttractiveLeds.pattern_number )
                        {
                            appDataAttractiveLeds.pattern[0] = 1;
                            appDataAttractiveLeds.pattern[1] = 0;
                            appDataAttractiveLeds.pattern[2] = 0;
                            appDataAttractiveLeds.pattern[3] = 1; 
                            if ( true == appDataLog.log_events )
                            {
                                store_event(OF_GO_NO_GO_LR_L);
                            }
                        }
                        else if ( TOP_BOTTOM_LEDS == appDataAttractiveLeds.pattern_number )
                        {
                            appDataAttractiveLeds.pattern[0] = 1;
                            appDataAttractiveLeds.pattern[1] = 1;
                            appDataAttractiveLeds.pattern[2] = 0;
                            appDataAttractiveLeds.pattern[3] = 0; 
                            if ( true == appDataLog.log_events )
                            {
                                store_event(OF_GO_NO_GO_TB_T);
                            }
                        }
                        else // ONE_LED
                        {
                            
                            appDataAttractiveLeds.pattern[0] = 1;
                            appDataAttractiveLeds.pattern[1] = 1;
                            appDataAttractiveLeds.pattern[2] = 1;
                            appDataAttractiveLeds.pattern[3] = 1;

                            if ( true == appDataLog.log_events )
                            {
                                store_event(OF_GO_NO_GO_ONE_NONE);
                            }
                            
                        }

                        appDataAttractiveLeds.pattern_idx = 0;     
                    }
                    else
                    {
                        appDataAttractiveLeds.current_color_index = ATTRACTIVE_LEDS_COLOR_A;
                        if ( true == appDataLog.log_events )
                        {
                            store_event(OF_CAL_A);
                        }
                    }
                    
                    getDateTime( );

                    if ( ( appData.current_time.tm_hour * 60 + appData.current_time.tm_min ) >= ( appDataAttractiveLeds.wake_up_time.tm_hour * 60 + appDataAttractiveLeds.wake_up_time.tm_min ) &&
                         ( appData.current_time.tm_hour * 60 + appData.current_time.tm_min )< ( appDataAttractiveLeds.sleep_time.tm_hour * 60 + appDataAttractiveLeds.sleep_time.tm_min ) )
                    {
                        setAttractiveLedsOn( );
                    }
                }

                rtcc_set_alarm( appDataAlarmWakeup.time.tm_hour, appDataAlarmWakeup.time.tm_min, appDataAlarmWakeup.time.tm_sec, EVERY_SECOND );
                appData.state = APP_STATE_IDLE;
            }
            else
            {
                appDataUsb.key_is_nedded = false;
                appData.state = APP_STATE_ERROR;
                break;
            }
  
            if ( USB_DRIVE_MOUNTED == usbUnmountDrive( ) )
            {
                appData.state = APP_STATE_ERROR;
            }

            appDataUsb.key_is_nedded = false;

            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_IDLE:
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_IDLE\n" );
#endif              
//                if ( appData.timeout_standby > 0 )
//                {
//                    /* Timeout before going to standby mode */
//                    setDelayMsStandBy( appData.timeout_standby );
//                }
                
                if ( true == appDataLog.log_events )
                {
                    store_event(OF_STATE_IDLE);
                }
                
                rtcc_start_alarm( );

                /* Enable PIR sensor interruption for bird detection */
                EX_INT0_InterruptFlagClear( );
                EX_INT0_InterruptEnable( );

                clearPitTagBuffers( );
                clearPitTagSringBuffers( );
                
                appData.test_rfid = false;
//                clear_bird_sensor_detected( );
                is_bird_detected = false;

                if ( DOOR_HABITUATION == appData.scenario_number || 
                     LONG_TERM_SPATIAL_MEMORY == appData.scenario_number || 
                     WORKING_SPATIAL_MEMORY == appData.scenario_number ||
                     RISK_AVERSION == appData.scenario_number ||
                     PATCH_PROBABILITY == appData.scenario_number)
                {
                    appDataAttractiveLeds.current_color_index = ATTRACTIVE_LEDS_COLOR_A;
                    setAttractiveLedsColor( );
                }
                else if ( GO_NO_GO == appData.scenario_number)
                {
                    setAttractiveLedsPattern( );
                }

            }

            if ( appData.need_to_reconfigure )
            {
                if ( true == appDataLog.log_events )
                {
                    store_event(OF_RECONFIGURE_SYSTEM);
                }
                appData.need_to_reconfigure = false;
                for ( i = 0; i < MAX_PIT_TAGS_LIST_NUMBER; i++ )
                {
                    appDataPitTag.isPitTagdeniedOrColorA[i] = false;
                }
                appData.state = APP_STATE_CONFIGURE_SYSTEM;
                break;
            }

            /* Green status LED blinks in idle mode. */
            LedsStatusBlink( LED_GREEN, LEDS_OFF, 25, 4975 );

            /* Check PIR SENSOR detected.
             *  - recording the time of detected of the bird
             *  - if true go to APP_STATE_ERROR
             */
//            if ( is_bird_sensor_detected( ) )
            if ( true == is_bird_detected )
            {

                /* Disable PIR sensor interruption for bird detection */
                EX_INT0_InterruptFlagClear( );
                EX_INT0_InterruptDisable( );

                while ( !RTCC_TimeGet( &appDataLog.bird_arrived_time ) )
                {
                    Nop( );
                }
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf( "\tBird detected\n" );
#endif
                /* Initialised global variable datalogging. */
//                clear_bird_sensor_detected( );
                is_bird_detected = false;
                appDataLog.is_reward_taken = false;
                appDataLog.did_door_open = false;

                appDataLog.attractive_leds_current_color_index = appDataAttractiveLeds.current_color_index;

                if ( DOOR_OPENED == appDataDoor.reward_door_status )
                {
                    appDataLog.door_status_when_bird_arrived = 1;
                }
                else
                {
                    appDataLog.door_status_when_bird_arrived = 0;
                }

                clearPitTagBuffers( );
                appData.state = APP_STATE_RFID_READING_PIT_TAG;
                break;
            }

            /* Check TIMEOUT IDLE MODE ending.
             *  - if false go to APP_STATE_STANDBY
             */
//            if ( false == appData.flags.bit_value.attractive_leds_status )
//            {
//                if ( appData.timeout_standby > 0 && isDelayMsEndingStandBy( ) )
//                {
//                    appData.state = APP_STATE_STANDBY;
//                    break;
//                }
//            }
 
            if ( true == appDataLog.log_events && appDataEvent.num_events_stored >= MAX_NUM_EVENT_BEFORE_SAVE )
            {
                if ( FILEIO_RESULT_FAILURE == logEvents( ) )
                {
                    appDataUsb.key_is_nedded = false;
                    appData.state = APP_STATE_ERROR;
                    break;
                }
            }
 
            if ( RTCC_ALARM_IDLE != appData.rtcc_alarm_action )
            {
                if ( OPENFEEDER_IS_AWAKEN == appData.openfeeder_state && RTCC_ALARM_SLEEP_OPENFEEDER == appData.rtcc_alarm_action )
                {
                    appData.state = APP_STATE_SLEEP;
                    break;
                }

                if ( OPENFEEDER_IS_SLEEPING == appData.openfeeder_state && RTCC_ALARM_WAKEUP_OPENFEEDER == appData.rtcc_alarm_action )
                {
                    appData.state = APP_STATE_WAKE_UP;
                    break;
                }
                
                if ( RTCC_RTC_CALIBRATION == appData.rtcc_alarm_action )
                {
                    calibrateDateTime( );
                }
                
                if ( RTCC_DS3231_TEMPERATURE == appData.rtcc_alarm_action )
                {
                    if (0 < APP_I2CMasterSeeksSlaveDevice(DS3231_I2C_ADDR, DS3231_I2C_ADDR))
                    {
                        if ( true == appDataLog.log_events )
                        {
                           store_event(OF_DS3231_GET_TEMPERATURE); 
                        }
                                
                        getDateTime( );
                        DS3231_temperature_get( );
                        appDataLog.ds3231_temp[appDataLog.numDs3231TempStored][0] = (float)appData.current_time.tm_hour;
                        appDataLog.ds3231_temp[appDataLog.numDs3231TempStored][1] = (float)appData.current_time.tm_min;
                        appDataLog.ds3231_temp[appDataLog.numDs3231TempStored][2] = appData.ext_temperature;
                        ++appDataLog.numDs3231TempStored;
                    }
                }
                
                if ( RTCC_ALARM_SET_ATTRACTIVE_LEDS_OFF == appData.rtcc_alarm_action )
                {
                    if ( ATTRACTIVE_LEDS_ON == appDataAttractiveLeds.status )
                    {
                        setAttractiveLedsOff( );
                    }
                }

                if ( RTCC_ALARM_SET_ATTRACTIVE_LEDS_ON == appData.rtcc_alarm_action )
                {
                    if ( ATTRACTIVE_LEDS_OFF == appDataAttractiveLeds.status )
                    {
                        setAttractiveLedsOn( );
                    }
                }

                if ( RTCC_ALARM_ALT_ATTRACTIVE_LEDS == appData.rtcc_alarm_action )
                {
                    if ( rand( ) > (RAND_MAX/2) ) // t / RAND_MAX ) > 0.5
                    {
                        appDataAttractiveLeds.current_color_index = !appDataAttractiveLeds.current_color_index;
                        setAttractiveLedsColor( );
                        if ( true == appDataLog.log_events )
                        {
                            if ( 0 == appDataAttractiveLeds.current_color_index )
                            {
                                store_event(OF_CAL_A);
                            }
                            else
                            {
                                store_event(OF_CAL_B);
                            }
                        }  
                    }                                
                }
                if ( RTCC_ALARM_ALT_ATTRACTIVE_LEDS_PATTERN == appData.rtcc_alarm_action )
                {
                    int randomInteger = rand( );
                    
                    if (ONE_LED == appDataAttractiveLeds.pattern_number)
                    {
                        for (i=0;i<4;i++)
                        {
                           appDataAttractiveLeds.pattern[i] = 1; 
                        }
                        
                        if ( randomInteger > (RAND_MAX/4*3) )
                        {
                            appDataAttractiveLeds.pattern_one_led_current = 0;
                            if ( true == appDataLog.log_events )
                            {
                                store_event( OF_GO_NO_GO_ONE_1 );
                            }
                        }                            
                        else if ( randomInteger > (RAND_MAX/2) )
                        {
                            appDataAttractiveLeds.pattern_one_led_current = 1;
                            if ( true == appDataLog.log_events )
                            {
                                store_event( OF_GO_NO_GO_ONE_2 );
                            }
                        }
                        else if ( randomInteger > (RAND_MAX/4) )
                        {
                            appDataAttractiveLeds.pattern_one_led_current = 2; 
                            if ( true == appDataLog.log_events )
                            {
                                store_event( OF_GO_NO_GO_ONE_3 );
                            }
                        }
                        else
                        {
                            appDataAttractiveLeds.pattern_one_led_current = 3;
                            if ( true == appDataLog.log_events )
                            {
                                store_event( OF_GO_NO_GO_ONE_4 );
                            }
                        }
                        
                        appDataAttractiveLeds.pattern[appDataAttractiveLeds.pattern_one_led_current] = 0;      

                       setAttractiveLedsPattern( );
                    }
                    else if (ALL_LEDS == appDataAttractiveLeds.pattern_number)
                    {
                        if ( randomInteger > (RAND_MAX/100*appDataAttractiveLeds.pattern_percent) )
                        {
                            for (i=0;i<4;i++)
                            {
                               appDataAttractiveLeds.pattern[i] = !appDataAttractiveLeds.pattern[i]; 
                            }
                            appDataAttractiveLeds.pattern_idx = !appDataAttractiveLeds.pattern_idx;
                            setAttractiveLedsPattern( );
                            
                            appDataAttractiveLeds.pattern_percent = 100-appDataAttractiveLeds.pattern_percent;
                            
                            if ( true == appDataLog.log_events )
                            {
                                if ( 0 == appDataAttractiveLeds.pattern_idx )
                                {
                                    store_event( OF_GO_NO_GO_ALL_ON );
                                }
                                else
                                {
                                    store_event( OF_GO_NO_GO_ALL_OFF );
                                }
                            }                                
                        }
                    }
                    else
                    {
                        if ( randomInteger > (RAND_MAX/2) )
                        {
                            for (i=0;i<4;i++)
                            {
                               appDataAttractiveLeds.pattern[i] = !appDataAttractiveLeds.pattern[i]; 
                            }
                            appDataAttractiveLeds.pattern_idx = !appDataAttractiveLeds.pattern_idx;
                            setAttractiveLedsPattern( );   
                            
                            if ( true == appDataLog.log_events )
                            {
                                if ( 0 == appDataAttractiveLeds.pattern_idx )
                                {
                                    if ( LEFT_RIGHT_LEDS == appDataAttractiveLeds.pattern_number )
                                    {
                                        store_event( OF_GO_NO_GO_LR_L );
                                    }
                                    else
                                    {
                                        store_event( OF_GO_NO_GO_TB_T );
                                    }
                                }
                                else
                                {
                                    if ( LEFT_RIGHT_LEDS == appDataAttractiveLeds.pattern_number )
                                    {
                                        store_event( OF_GO_NO_GO_LR_R );
                                    }
                                    else
                                    {
                                        store_event( OF_GO_NO_GO_TB_B );
                                    }
                                }
                            }  
                        }
                    }
                    
                }
                if ( DOOR_OPENED != appDataDoor.reward_door_status && RTCC_ALARM_OPEN_DOOR == appData.rtcc_alarm_action )
                {
                    /* Open reward door */
                    servomotorPowerEnable( );

                    appDataServo.ton_cmd = servomotorGetDoorPosition( );
                    appDataServo.ton_goal = appDataServo.ton_max;

                    if ( appDataServo.ton_cmd != appDataServo.ton_goal )
                    {
                        appDataDoor.reward_door_status = DOOR_MOVING;
                        while ( DOOR_MOVED != appDataDoor.reward_door_status );
                    }
                    appDataDoor.reward_door_status = DOOR_OPENED;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_SERVO_POSITION)
                    servomotorGetDoorPosition( );
#endif  
                    servomotorPowerDisable( );
                }

                if ( DOOR_CLOSED != appDataDoor.reward_door_status && RTCC_ALARM_CLOSE_DOOR == appData.rtcc_alarm_action )
                {
                    /* Close reward door */
                    servomotorPowerEnable( );

                    appDataServo.ton_cmd = servomotorGetDoorPosition( );
                    appDataServo.ton_goal = appDataServo.ton_min;

                    if ( appDataServo.ton_cmd != appDataServo.ton_goal )
                    {
                        appDataDoor.reward_door_status = DOOR_MOVING;
                        while ( DOOR_MOVED != appDataDoor.reward_door_status );
                    }
                    appDataDoor.reward_door_status = DOOR_CLOSED;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_SERVO_POSITION)
                    servomotorGetDoorPosition( );
#endif  
                    servomotorPowerDisable( );
                }

                if ( RTCC_BATTERY_LEVEL_CHECK == appData.rtcc_alarm_action )
                {
                    flag = isPowerBatteryGood( );
                    appDataLog.battery_level[appDataLog.numBatteryLevelStored][0] = appData.current_time.tm_hour;
                    appDataLog.battery_level[appDataLog.numBatteryLevelStored][1] = appData.battery_level;
                    ++appDataLog.numBatteryLevelStored;

                    if ( false == flag )
                    {
                        appData.state = APP_STATE_FLUSH_DATA_BEFORE_ERROR;
                        break;
                    }
                }
                if ( RTCC_FOOD_LEVEL_CHECK == appData.rtcc_alarm_action && true == appData.chk_food_level )
                {
                    flag = isEnoughFood( );
                    if ( false == flag )
                    {
                        appData.state = APP_STATE_FLUSH_DATA_BEFORE_ERROR;
                        break;
                    }
                }
                if ( RTCC_RFID_FREQ_CHECK == appData.rtcc_alarm_action )
                {
                    flag = isRfidFreqGood( );
                    appDataLog.rfid_freq[appDataLog.numRfidFreqStored][0] = appData.current_time.tm_hour;
                    appDataLog.rfid_freq[appDataLog.numRfidFreqStored][1] = appData.current_time.tm_min;
                    appDataLog.rfid_freq[appDataLog.numRfidFreqStored][2] = appData.rfid_rdyclk;
                    ++appDataLog.numRfidFreqStored;

                    if ( false == flag )
                    {
                        appData.state = APP_STATE_FLUSH_DATA_BEFORE_ERROR;
                        break;
                    }
                }

                appData.rtcc_alarm_action = RTCC_ALARM_IDLE;
            }

//#if defined (USE_UART1_SERIAL_INTERFACE)
//            /* Get interaction with the serial terminal. */
//            APP_SerialDebugTasks( );
//#endif
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
                        appData.state = APP_STATE_TEST_RFID;
                    }
                    else
                    {
                        appData.state = APP_STATE_FLUSH_DATA_TO_USB;
                    }
                }
            }

            //#if defined (TEST_RTCC_SLEEP_WAKEUP)
            //            /* Next line for debugging sleep/wakeup only */
            //            /* Should be commented in normal mode */
            //            /* Modify time value according to sleep values in the CONFIG.INI file */
            //            setDateTime( 17, 9, 21, 22, 59, 55 );
            //#endif   
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_RFID_READING_PIT_TAG:
            /* 
             * TODO: comment APP_STATE_RFID_READING_PIT_TAG
             */
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_RFID_READING_PIT_TAG\n" );
#endif
                                
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_STATE_RFID_READING_PIT_TAG); 
                }
                
                EX_INT0_InterruptFlagClear( );
                EX_INT0_InterruptDisable( );
                rtcc_stop_alarm( );

                APP_Rfid_Init( );

            }

            APP_Rfid_Task( );

            /* Enable interruption see case RFID_DETECT_COMPLET_DATASTREAM
             * in APP_Rfid_Task */
            EX_INT4_InterruptEnable();
            
            if ( appData.flags.bit_value.NewValidPitTag )
            {

                switch ( appData.scenario_number )
                {
                    case NO_SCENARIO:
                    case OPEN_BAR:
                        /* No PIT tag denied */
                        appDataPitTag.didPitTagMatched = true;
                        appDataLog.is_pit_tag_denied = false;
                        break;
                        
                    case DOOR_HABITUATION:
                        /* No PIT tag denied */
                        appDataPitTag.didPitTagMatched = true;
                        appDataLog.is_pit_tag_denied = false;
                        break;
                        
                    case GO_NO_GO:

                        appDataLog.is_pit_tag_denied = false;
                        
                        if ( ALL_LEDS == appDataAttractiveLeds.pattern_number )
                        {
                            appDataPitTag.didPitTagMatched = true;
                            
                            if ( 1 == appDataAttractiveLeds.pattern_idx )
                            {                                
                                appDataLog.is_pit_tag_denied = true;
                            }
                        }
                        else if ( ONE_LED == appDataAttractiveLeds.pattern_number )
                        {
                            appDataLog.is_pit_tag_denied = false;
                            
                            findPitTagInList( );
                            if ( false == appDataPitTag.didPitTagMatched )
                            {
                                appDataLog.is_pit_tag_denied = true;
                            }
                            else
                            {        
                                if (0 == appDataPitTag.numPitTagGroup[appDataAttractiveLeds.pattern_one_led_current])
                                {
                                   appDataLog.is_pit_tag_denied = true; 
                                }
                                else
                                {
                                    if ( appDataPitTag.pitTagIndexInList < appDataAttractiveLeds.pattern_one_led_groups[0] )
                                    {
                                        if ( 0 != appDataAttractiveLeds.pattern_one_led_current )
                                        {
                                            appDataLog.is_pit_tag_denied = true;
                                        }                                        
                                    }
                                    else if ( appDataPitTag.pitTagIndexInList < appDataAttractiveLeds.pattern_one_led_groups[1] )
                                    {
                                        if ( 1 != appDataAttractiveLeds.pattern_one_led_current )
                                        {
                                            appDataLog.is_pit_tag_denied = true;
                                        }   
                                    }
                                    else if ( appDataPitTag.pitTagIndexInList < appDataAttractiveLeds.pattern_one_led_groups[2] )
                                    {
                                        if ( 2 != appDataAttractiveLeds.pattern_one_led_current )
                                        {
                                            appDataLog.is_pit_tag_denied = true;
                                        }     
                                    }
                                    else
                                    {
                                        if ( 3 != appDataAttractiveLeds.pattern_one_led_current )
                                        {
                                            appDataLog.is_pit_tag_denied = true;
                                        }    
                                    }
                                }
                            } 
                        }
                        else
                        {
                            findPitTagInList( );
                            if ( false == appDataPitTag.didPitTagMatched )
                            {
                                appDataLog.is_pit_tag_denied = true;
                            }
                            else
                            {
                                if ( ( appDataPitTag.pitTagIndexInList >= appDataPitTag.numPitTagDeniedOrColorA ) && ( 1 == appDataAttractiveLeds.pattern_idx ) )
                                {
                                    appDataLog.is_pit_tag_denied = true;
                                }
                                if ( ( appDataPitTag.pitTagIndexInList < appDataPitTag.numPitTagDeniedOrColorA ) && ( 0 == appDataAttractiveLeds.pattern_idx ) )
                                {
                                    appDataLog.is_pit_tag_denied = true;
                                }
                            }
                        }

                        break;
                        
                    case LONG_TERM_SPATIAL_MEMORY:
                        
                        findPitTagInList( );
                        if ( false == appDataPitTag.didPitTagMatched )
                        {
                            appDataLog.is_pit_tag_denied = true;
                        }
                        else
                        {                                                
                            /* Check if PIT tag is denied */
                            appDataLog.is_pit_tag_denied = isPitTagDenied( );
                        }
                        break;

                    case WORKING_SPATIAL_MEMORY:
                        
                        findPitTagInList( );
                        if ( false == appDataPitTag.didPitTagMatched )
                        {
                            appDataLog.is_pit_tag_denied = true;
                        }
                        else
                        {                         
                            /* Check if PIT tag is denied */
                            appDataLog.is_pit_tag_denied = isPitTagDenied( );
                            /* Bird is allowed only one time */
                            appDataPitTag.isPitTagdeniedOrColorA[appDataPitTag.pitTagIndexInList] = true;
                        }
                        break;

                    case COLOR_ASSOCIATIVE_LEARNING:
                        
                        findPitTagInList( );
                        if ( false == appDataPitTag.didPitTagMatched )
                        {
                            appDataLog.is_pit_tag_denied = true;
                        }
                        else
                        {                        
                            appDataLog.is_pit_tag_denied = false;
                            /* Check if PIT tag is denied */
                            if ( ( appDataPitTag.pitTagIndexInList >= appDataPitTag.numPitTagDeniedOrColorA ) && ( appDataLog.attractive_leds_current_color_index == ATTRACTIVE_LEDS_COLOR_A ) )
                            {
                                appDataLog.is_pit_tag_denied = true;
                            }
                            if ( ( appDataPitTag.pitTagIndexInList < appDataPitTag.numPitTagDeniedOrColorA )  && ( appDataLog.attractive_leds_current_color_index == ATTRACTIVE_LEDS_COLOR_B ) )
                            {
                                appDataLog.is_pit_tag_denied = true;
                            }
                        }
                        break;
                    
                    case RISK_AVERSION:
                        
                        findPitTagInList( );
                        if ( false == appDataPitTag.didPitTagMatched )
                        {
                            appDataLog.is_pit_tag_denied = true;
                        }
                        else
                        {                                                
                            /* Check if PIT tag is denied */
                            appDataLog.is_pit_tag_denied = isPitTagDenied( );
                        }
                        break;  
                        
                    case PATCH_PROBABILITY:

                        findPitTagInList( );
                        if ( false == appDataPitTag.didPitTagMatched )
                        {
                            appDataLog.is_pit_tag_denied = true;
                        }
                        else
                        {                                                
                            /* Check if PIT tag is denied */
                            appDataLog.is_pit_tag_denied = isPitTagDenied( );
                        }                        
                        if ( false == appDataLog.is_pit_tag_denied )
                        {
                            appDataDoor.reward_probability = appDataPitTag.reward_probability[appDataPitTag.pitTagIndexInList];
                        }                        
                        break; 
                        
                }
                
                RFID_Disable( );

//                clear_bird_sensor_detected( );
                is_bird_detected = false;

                if ( PATCH_PROBABILITY == appData.scenario_number && false == isItANewPitTag())
                {
                    appData.rfid_signal_detected = false;
                    appDataLog.is_reward_taken = false;
                    appDataLog.did_door_open = false;
                    appDataLog.is_pit_tag_denied = false;
                    clearPitTagBuffers( );
                    appData.state = APP_STATE_IDLE;
                    break;
                }
                
                if ( true == appDataLog.is_pit_tag_denied )
                {
                    if ( true == appDataPitTag.didPitTagMatched )
                    {
                        if ( true == appDataLog.log_events )
                        {
                           store_event(OF_PIT_TAG_DENIED); 
                        }
#if defined (USE_UART1_SERIAL_INTERFACE) 
                        printf( "\tPIT tag %s denied.\n", appDataLog.bird_pit_tag_str );
#endif
                    }
                    else
                    {
                        if ( true == appDataLog.log_events )
                        {
                           store_event(OF_PIT_TAG_NOT_LISTED_DENIED); 
                        }
#if defined (USE_UART1_SERIAL_INTERFACE) 
                        printf( "\tPIT tag %s not listed => consider as denied.\n", appDataLog.bird_pit_tag_str ); 
#endif
                    }
                    if ( COLOR_ASSOCIATIVE_LEARNING == appData.scenario_number || GO_NO_GO == appData.scenario_number )
                    {
                        if ( true == appDataLog.log_events )
                        {
                           store_event(OF_ATTRACTIVE_LEDS_PUNISHMENT); 
                        }
                        setAttractiveLedsNoColor( );
                        /* Delay before reactivate attractiveLEDs */
                        setDelayMs( appData.punishment_delay );
                        while ( false == isDelayMsEnding( ) )
                        {
                            Nop( );
                        }
                    }
                    appData.state = APP_STATE_DATA_LOG;
                }
                else
                {
#if defined (USE_UART1_SERIAL_INTERFACE)
                    printf( "\tPIT tag %s accepted.\n", appDataLog.bird_pit_tag_str );
#endif
                    appData.state = APP_STATE_OPENING_DOOR;
                }
                break;
            }

            /* Test if delay detect PIT Tags in ending. (Xx 160 ms) */
            if ( 0 == g_timeout_reading_pit_tag ) // && (Timeout_Detecting_RFID_Tag != 0)
            {
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_RFID_TIMEOUT); 
                }
                
                RFID_Disable( );

//                clear_bird_sensor_detected( );
                is_bird_detected = false;

                if ( false == appData.rfid_signal_detected )
                {
                    snprintf( appDataLog.bird_pit_tag_str, 11, "XXXXXXXXXX" );
                }
                else
                {
                    snprintf( appDataLog.bird_pit_tag_str, 11, "??????????" );
                }

                appData.rfid_signal_detected = false;
                appDataLog.is_reward_taken = false;
                appDataLog.did_door_open = false;
                appDataLog.is_pit_tag_denied = false;
                clearPitTagBuffers( );
                appData.state = APP_STATE_DATA_LOG;
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf( "\tRFID timeout.\n" );
#endif
            }
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_OPENING_DOOR:
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_OPENING_REWARD_DOOR\n" );
#endif
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_STATE_OPENING_DOOR); 
                }
                                
                /* Reward probability */
                if ( 0 == appDataDoor.reward_probability )
                {
                    setAttractiveLedsNoColor( );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_REWARD_PROBABILITY)
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
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_REWARD_PROBABILITY)
                        printf( "\tReward probability is 100%% => accepted.\n" );
#endif
                    }
                else 
                {
                    int randomInteger = rand( );

                    if ( randomInteger > (RAND_MAX/100*appDataDoor.reward_probability) )
                    {
                        if ( PATCH_PROBABILITY != appData.scenario_number )
                        {
                            setAttractiveLedsNoColor( );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_REWARD_PROBABILITY)
                            printf( "\t%.0f > %u (reward prob.) => denied.\n", 100*((double)randomInteger/(double)RAND_MAX), appDataDoor.reward_probability);
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
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_REWARD_PROBABILITY)
                            printf( "\t%.0f > %u (reward prob.) => denied.\n", 100*((double)randomInteger/(double)RAND_MAX), appDataDoor.reward_probability);
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
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_REWARD_PROBABILITY)
                            printf( "\t%.0f > %u (reward prob.) => denied\n\tBut %u (reward prob.) > %u (prob. thresh.) => no punishment.\n", 100*((double)randomInteger/(double)RAND_MAX), appDataDoor.reward_probability, appDataDoor.reward_probability, appData.punishment_proba_thresh);
#endif 
                            appDataLog.is_pit_tag_denied = true; 
                            appData.state = APP_STATE_DATA_LOG;
                            break;
                        }

                    }
                    else
                    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_REWARD_PROBABILITY)
                        printf( "\t%.0f < %u (reward prob.) => accepted.\n", 100*((double)randomInteger/(double)RAND_MAX), appDataDoor.reward_probability);
#endif 
                    }
                    
                }
                
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

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_SERVO_POSITION)
            servomotorGetDoorPosition( );
#endif  
            /* Servomotor power command disable. */
            servomotorPowerDisable( );

            appDataLog.did_door_open = true;
            
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_SERVO_INFO)
            printf( "\tDoor opened\n");
#endif
            appData.state = APP_STATE_WAITING_CATCH_REWARD;
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_WAITING_CATCH_REWARD:
            /* Waiting for the catch of a reward. */
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_WAITING_CATCH_REWARD\n" );
#endif           
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_STATE_WAITING_CATCH_REWARD); 
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
#if defined (USE_UART1_SERIAL_INTERFACE)
                        printf("\tReward disabled, waiting reward timeout to end.\n");
#endif 
                        while (false == isDelayMsEnding( ));
                    }
                    else
                    {
#if defined (USE_UART1_SERIAL_INTERFACE)
                        printf("\tReward disabled. No timeout reward.\n");
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
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf( "\tTaking reward detected.\n" );
#endif
                appData.bird_is_taking_reward = true;
                break;
            }

            /* low --> Breaking of the infrared barrier */
            if ( ( true == g_flag_ir1_sensor ) && ( true == appData.bird_is_taking_reward ) )
            {
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf( "\tReward taken.\n" );
#endif
                EX_INT1_InterruptDisable( );
                EX_INT1_PositiveEdgeSet( );
                EX_INT1_InterruptFlagClear( );
                clear_ir1_sensor( );

                if ( true == appDataLog.log_events )
                {
                   store_event(OF_REWARD_TAKEN); 
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
                    if ( true == appDataLog.log_events )
                    {
                       store_event(OF_REWARD_TIMEOUT); 
                    }
                    
#if defined (USE_UART1_SERIAL_INTERFACE)
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
                    
                    if ( true == appDataLog.log_events )
                    {
                       store_event(OF_REWARD_TIMEOUT_WRONG); 
                    }
                    
#if defined (USE_UART1_SERIAL_INTERFACE)
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
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_CLOSING_REWARD_DOOR\n" );
#endif
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_STATE_CLOSING_DOOR); 
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
                            1 == appData.reward_enable && 1 == BAR_IR1_OUT_GetValue( ))
                    {
                        if (true == appData.secu_bird_reward_reopen)
                        {
                            appDataDoor.reward_door_status = DOOR_MOVED;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_SERVO_INFO)
            printf("\tBird puts its head in during door close => reopen\n");
#endif  
                            if ( true == appDataLog.log_events )
                            {
                               store_event(OF_BIRD_X_REWARD_REOPEN); 
                            }
                            appData.state = APP_STATE_REOPEN_DOOR;
                            break;
                        }
                        else
                        {
                            if ( true == appDataLog.log_events )
                            {
                               store_event(OF_BIRD_X_REWARD_NO_REOPEN); 
                            }
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_SERVO_INFO)
            printf("\tBird puts its head in during door close but reopen security disable\n");
#endif  
                        }
                    }
                    /* Check if door take too much time to close */
                    if ( true == isDelayMsEndingStandBy() )
                    {
                        if ( true == appDataLog.log_events )
                        {
                           store_event(OF_DOOR_CLOSE_SLOW_REOPEN); 
                        }
                        appDataDoor.reward_door_status = DOOR_MOVED;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_SERVO_INFO)
            printf("\tDoor take too much time to close => reopen\n");
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

            uint16_t pos = servomotorGetDoorPosition();

            if ( pos > appDataServo.ton_goal && (pos - appDataServo.ton_goal) > 150)
            {
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_SERVO_FAR_CLOSE_POS_REOPEN); 
                }
                appDataDoor.reward_door_status = DOOR_MOVED;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_SERVO_INFO)
            printf("\tServo too far from goal position (%u to %u) => reopen\n", pos, appDataServo.ton_goal);
#endif  
                appData.state = APP_STATE_REOPEN_DOOR;
                break;
            }

            /* Servomotor power command disable. */
            servomotorPowerDisable( );

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_SERVO_INFO)
            printf( "\tDoor closed\n");
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
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_REOPEN_DOOR\n" );
#endif          
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_STATE_REOPEN_DOOR); 
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
                appError.currentLineNumber = __LINE__;
                sprintf( appError.currentFileName, "%s", __FILE__ );
                appError.number = ERROR_DOOR_CANT_CLOSE;
                appData.state = APP_STATE_ERROR;
                break;
            }

            if ( DOOR_MOVED == appDataDoor.reward_door_status )
            {
                if ( MAX_NUM_DOOR_REOPEN_ATTEMPT < appDataDoor.num_reopen_attempt )
                {
                    sprintf( appError.message, "Unable to close the door after %d attempts", MAX_NUM_DOOR_REOPEN_ATTEMPT );
                    appError.currentLineNumber = __LINE__;
                    sprintf( appError.currentFileName, "%s", __FILE__ );
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
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_DATA_LOG\n" );
#endif
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_STATE_DATA_LOG); 
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
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;

                if ( true == appDataLog.log_events )
                {
                   store_event(OF_STATE_REMOVE_USB_DEVICE); 
                }
                
                /* Unmount drive on USB device before power it off. */
                if ( USB_DRIVE_MOUNTED == appDataUsb.usbDriveStatus )
                {
                    usbUnmountDrive();
                }
                
                USBHostShutdown( );
                
                setDelayMs( 2000 );
                while ( 0 == isDelayMsEnding( ) ); 
        
                powerUsbRfidDisable( );
                
                setLedsStatusColor( LED_GREEN);
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_REMOVE_USB_DEVICE\n" );
#endif
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf( "\tUSB device can be safely removed\n");
#endif
                
                setDelayMsStandBy(60000);
                
            }

            if ( true == isDelayMsEndingStandBy( ) )
            {
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf( "\tSystem will automatically restart\n");
#endif
                appData.dsgpr0.bit_value.num_software_reset = 0;
                DSGPR0 = appData.dsgpr0.reg;
                DSGPR0 = appData.dsgpr0.reg;
                    
                __asm__ volatile ( "reset" );
            }
                
            break;
            /* -------------------------------------------------------------- */
            
        case APP_STATE_FLUSH_DATA_TO_USB:
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_FLUSH_DATA_TO_USB\n" );
#endif
                
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_STATE_FLUSH_DATA_TO_USB); 
                }
                
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "\t/!\\ Don't remove the USB device yet.\n" );
#endif
                
                setLedsStatusColor( LED_USB_ACCESS );
                
                if ( appDataLog.numDataStored > 0 || 
                     appDataLog.numBatteryLevelStored > 0 || 
                     appDataLog.numRfidFreqStored > 0  ||
                     appDataEvent.num_events_stored > 0 ||
                     appDataLog.numDs3231TempStored > 0)
                {
                    /* Log data on USB device */
                    appDataUsb.key_is_nedded = true;
                    powerUsbRfidEnable( );
                }
                else
                {
#if defined (USE_UART1_SERIAL_INTERFACE)  
                    printf("\tNo data stored.\n");
                    appData.state = APP_STATE_REMOVE_USB_DEVICE;
                    break;
#endif
                }

                setDelayMs(MAX_DELAY_TO_DETECT_USB_DEVICE);
            
            }

            if ( appDataUsb.getValidDeviceAdress )
            {

                setLedsStatusColor( LED_USB_ACCESS );
                
                if ( USB_DRIVE_NOT_MOUNTED == usbMountDrive( ) )
                {
                    appDataUsb.key_is_nedded = false;
                    appData.state = APP_STATE_ERROR;
                    break;
                }
                
                if ( appDataLog.numDataStored > 0 )
                {
#if defined (USE_UART1_SERIAL_INTERFACE) 
                    printf("\tFlush birds data.\n");
#endif
                    /* Force data to be written on the USB device */
                    appDataLog.numDataStored = MAX_NUM_DATA_TO_STORE;
                    if ( false == dataLog( false ) )
                    {
                        appDataUsb.key_is_nedded = false;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }
                
                setLedsStatusColor( LED_USB_ACCESS );

                if ( true == appDataLog.log_battery && appDataLog.numBatteryLevelStored > 0 )
                {
#if defined (USE_UART1_SERIAL_INTERFACE) 
                    printf("\tFlush battery data.\n");
#endif
                    if ( FILEIO_RESULT_FAILURE == logBatteryLevel( ) )
                    {
                        appDataUsb.key_is_nedded = false;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }

                setLedsStatusColor( LED_USB_ACCESS );
                
                if ( true == appDataLog.log_rfid && appDataLog.numRfidFreqStored > 0 )
                {
#if defined (USE_UART1_SERIAL_INTERFACE) 
                    printf("\tFlush RFID data.\n");
#endif
                    if ( FILEIO_RESULT_FAILURE == logRfidFreq( ) )
                    {
                        appDataUsb.key_is_nedded = false;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }
                
                setLedsStatusColor( LED_USB_ACCESS );
                
                if ( true == appDataLog.log_calibration && appDataLog.numTimeCalibStored > 0 )
                {
#if defined (USE_UART1_SERIAL_INTERFACE) 
                    printf("\tFlush calibration data.\n");
#endif
                    if ( FILEIO_RESULT_FAILURE == logCalibration( ) )
                    {
                        appDataUsb.key_is_nedded = false;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }
                
                setLedsStatusColor( LED_USB_ACCESS );
                
                if ( true == appDataLog.log_temp && appDataLog.numDs3231TempStored > 0 )
                {
#if defined (USE_UART1_SERIAL_INTERFACE) 
                    printf("\tFlush temperature data.\n");
#endif
                    if ( FILEIO_RESULT_FAILURE == logDs3231Temp( ) )
                    {
                        appDataUsb.key_is_nedded = false;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }

                setLedsStatusColor( LED_USB_ACCESS );
                
                if ( true == appDataLog.log_events && appDataEvent.num_events_stored > 0 )
                {
#if defined (USE_UART1_SERIAL_INTERFACE) 
                    printf("\tFlush events data.\n");
#endif
                    if ( FILEIO_RESULT_FAILURE == logEvents( ) )
                    {
                        appDataUsb.key_is_nedded = false;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }
                
                if ( USB_DRIVE_MOUNTED == appDataUsb.usbDriveStatus )
                {
                    usbUnmountDrive();
                }

            }
            else
            {
                if ( true == isDelayMsEnding() )
                {
                    appDataUsb.key_is_nedded = false;
                    sprintf(appError.message, "USB device detection took more than %ld ms", (long)MAX_DELAY_TO_DETECT_USB_DEVICE);
                    appError.currentLineNumber = __LINE__;
                    sprintf(appError.currentFileName, "%s", __FILE__);
                    appError.number = ERROR_USB_DEVICE_NOT_FOUND;
                    appData.state = APP_STATE_ERROR;
                    break; 
                }
                /* Blue and yellow status LEDs blink as long USB key is required. */
                LedsStatusBlink( LED_USB_ACCESS, LED_YELLOW, 500, 500 );
                break;
            }

            appDataUsb.key_is_nedded = false;
            appData.state = APP_STATE_REMOVE_USB_DEVICE;
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_TEST_RFID:

            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_TEST_RFID\n" );
#endif
#if defined (USE_UART1_SERIAL_INTERFACE) 
                printf( "\t/!\\ Debug not available in RFID test mode.\n\tRFID test is launched for 60s.\n\tPress the user button to stop the test.\n" );
#endif
                
                initAttractiveLeds();
                setAttractiveLedsOn();
                    
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_STATE_TEST_RFID); 
                }
                
                setDelayMsStandBy(60000);
                
                /* Debounce à la noix */
                setDelayMs(1000);
                while (false == isDelayMsEnding());
                
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
                
                APP_Rfid_Init();
                
            }

            APP_Rfid_Task( );

            /* Enable interruption see case RFID_DETECT_COMPLET_DATASTREAM
             * in APP_Rfid_Task */
            EX_INT4_InterruptEnable();
            
            if ( appData.rfid_signal_detected  )
            {
                if ( appData.flags.bit_value.NewValidPitTag )
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
                            appDataPitTag.isPitTagdeniedOrColorA[appDataPitTag.pitTagIndexInList] = true;
                            break;

                        case COLOR_ASSOCIATIVE_LEARNING:
                            appDataLog.is_pit_tag_denied = false;
                            /* Check if PIT tag is denied */
                            if ( ( appDataPitTag.pitTagIndexInList >= appDataPitTag.numPitTagDeniedOrColorA ) && ( appDataLog.attractive_leds_current_color_index == ATTRACTIVE_LEDS_COLOR_A ) )
                            {
                                appDataLog.is_pit_tag_denied = true;
                            }
                            if ( ( appDataPitTag.pitTagIndexInList < appDataPitTag.numPitTagDeniedOrColorA )  && ( appDataLog.attractive_leds_current_color_index == ATTRACTIVE_LEDS_COLOR_B ) )
                            {
                                appDataLog.is_pit_tag_denied = true;
                            }
                            break;
                            
                        case RISK_AVERSION:
                            /* Check if PIT tag is denied */
                            appDataLog.is_pit_tag_denied = isPitTagDenied( );
                            break;
                    }
                    
                    if (true == appDataLog.is_pit_tag_denied)
                    {
//                        setLedsStatusColor( LED_PITTAG_DENIED );
                        setOneAttractiveLedColor( 1, 0, 0, 0 );
                        setOneAttractiveLedColor( 2, 35, 0, 0 );
                    }
                    else
                    {
//                        setLedsStatusColor( LED_PITTAG_ACCEPTED );
                        setOneAttractiveLedColor( 1, 0, 0, 0 );
                        setOneAttractiveLedColor( 2, 0, 35, 0 );
                    }
                }
                else
                {
//                    setLedsStatusColor(LED_BLUE);
                    setOneAttractiveLedColor( 1, 0, 0, 35 );
                    setOneAttractiveLedColor( 2, 0, 0, 0 );
                }

                appData.rfid_signal_detected = false;
                
            }
            else
            {
//               setLedsStatusColor( LEDS_OFF ); 
               setOneAttractiveLedColor( 1, 0, 0, 0 );
               setOneAttractiveLedColor( 2, 0, 0, 0 );
            }
            
            button_user_state = USER_BUTTON_GetValue( );

            if ( BUTTON_PRESSED == button_user_state || true == isDelayMsEndingStandBy() )
            {
                
#if defined (USE_UART1_SERIAL_INTERFACE) 
                printf( "\tQuit RFID test mode => back to normal mode in 3s.\n" );
#endif
                RFID_Disable( );
                
                appData.test_rfid = false;
                    
                for (i=0;i<3;i++)
                {
                    setLedsStatusColor( LEDS_ON );
                    setDelayMs( 500 );
                    while ( 0 == isDelayMsEnding( ) ); 
                    setLedsStatusColor( LEDS_OFF );
                    setDelayMs( 500 );
                    while ( 0 == isDelayMsEnding( ) ); 
                }              
                appData.state = APP_STATE_IDLE;
            }

            break;
            /* -------------------------------------------------------------- */
            
//        case APP_STATE_STANDBY:
//            if ( appData.state != appData.previous_state )
//            {
//                appData.previous_state = appData.state;
//#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
//                printf( "> APP_STATE_STANDBY\n" );
//#endif
//            }
//
////            Sleep( );
//
//            appData.state = APP_STATE_IDLE;
//            break;
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
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_SLEEP\n" );
#endif
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_STATE_SLEEP); 
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
                appDataUsb.key_is_nedded = false;
                /* Log data on USB device */
                powerUsbRfidEnable( );
                appDataUsb.key_is_nedded = true;

            }

            if ( true == appDataUsb.key_is_nedded )
            {
                if ( appDataUsb.getValidDeviceAdress )
                {
                    if ( USB_DRIVE_NOT_MOUNTED == usbMountDrive( ) )
                    {
                        appDataUsb.key_is_nedded = false;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                                    
                    if ( appDataLog.numDataStored > 0 )
                    {
                        setLedsStatusColor( LED_USB_ACCESS );
                        /* Force data to be written on the USB device */
                        appDataLog.numDataStored = MAX_NUM_DATA_TO_STORE;
                        if ( false == dataLog( false ) )
                        {
                            appDataUsb.key_is_nedded = false;
                            appData.state = APP_STATE_ERROR;
                            break;
                        }
                    }

                    if ( true == appDataLog.log_battery && appDataLog.numBatteryLevelStored > 0 )
                    {
                        setLedsStatusColor( LED_USB_ACCESS );
                        if ( FILEIO_RESULT_FAILURE == logBatteryLevel( ) )
                        {
                            appDataUsb.key_is_nedded = false;
                            appData.state = APP_STATE_ERROR;
                            break;
                        }
                    }

                    if ( true == appDataLog.log_rfid && appDataLog.numRfidFreqStored > 0 )
                    {
                        setLedsStatusColor( LED_USB_ACCESS );
                        if ( FILEIO_RESULT_FAILURE == logRfidFreq( ) )
                        {
                            appDataUsb.key_is_nedded = false;
                            appData.state = APP_STATE_ERROR;
                            break;
                        }
                    }
                    
                    if ( true == appDataLog.log_events && appDataEvent.num_events_stored > 0 )
                    {
                        setLedsStatusColor( LED_USB_ACCESS );
                        if ( FILEIO_RESULT_FAILURE == logEvents( ) )
                        {
                            appDataUsb.key_is_nedded = false;
                            appData.state = APP_STATE_ERROR;
                            break;
                        }
                    }
                    
                    /* Unmount drive on USB device before power it off. */
                    if ( USB_DRIVE_MOUNTED == appDataUsb.usbDriveStatus )
                    {
                        usbUnmountDrive();
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

#if defined (TEST_RTCC_SLEEP_WAKEUP)
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
                
#if defined (ENABLE_DEEP_SLEEP)
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
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_WAKE_UP_FROM_SLEEP\n" );
#endif
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_STATE_WAKE_UP); 
                }
            }

#if defined (TEST_RTCC_SLEEP_WAKEUP)
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
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_FLUSH_DATA_BEFORE_ERROR\n" );
#endif
                appDataLog.data_flush_before_error = true;
                
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_STATE_FLUSH_DATA_BEFORE_ERROR); 
                }
                
                if ( appDataLog.numDataStored == 0 && 
                     appDataLog.numBatteryLevelStored == 0 && 
                     appDataLog.numRfidFreqStored == 0 &&
                     appDataEvent.num_events_stored == 0 )
                {
#if defined (USE_UART1_SERIAL_INTERFACE)
                    printf("\t No data stored.\n");
#endif
                    appData.state = APP_STATE_ERROR;
                    break;
                }

                appDataUsb.key_is_nedded = true;
                /* Log data on USB device */
                powerUsbRfidEnable( ); 
                
                setDelayMs(MAX_DELAY_TO_DETECT_USB_DEVICE);
            }

            if ( appDataUsb.getValidDeviceAdress )
            {        
                if ( USB_DRIVE_NOT_MOUNTED == usbMountDrive( ) )
                {
                    appDataUsb.key_is_nedded = false;
                    appData.state = APP_STATE_ERROR;
                    break;
                }
                
                if ( appDataLog.numDataStored > 0 )
                {
                    setLedsStatusColor( LED_USB_ACCESS );
                    /* Force data to be written on the USB device */
                    appDataLog.numDataStored = MAX_NUM_DATA_TO_STORE;
                    if ( false == dataLog( false ) )
                    {
                        appDataUsb.key_is_nedded = false;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }

                if ( true == appDataLog.log_battery && appDataLog.numBatteryLevelStored > 0 )
                {
                    setLedsStatusColor( LED_USB_ACCESS );
                    if ( FILEIO_RESULT_FAILURE == logBatteryLevel( ) )
                    {
                        appDataUsb.key_is_nedded = false;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }
                
                if ( true == appDataLog.log_rfid && appDataLog.numRfidFreqStored > 0 )
                {
                    setLedsStatusColor( LED_USB_ACCESS );
                    if ( FILEIO_RESULT_FAILURE == logRfidFreq( ) )
                    {
                        appDataUsb.key_is_nedded = false;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }                
                
                if ( true == appDataLog.log_errors )
                {
                    setLedsStatusColor( LED_USB_ACCESS );
                    if ( FILEIO_RESULT_FAILURE == logError() )
                    {
                        appDataUsb.key_is_nedded = false;
                        appData.state = APP_STATE_ERROR;
                        break; 
                    }
                }
                
                if ( true == appDataLog.log_events && appDataEvent.num_events_stored > 0 )
                {
                    setLedsStatusColor( LED_USB_ACCESS );
                    if ( FILEIO_RESULT_FAILURE == logEvents( ) )
                    {
                        appDataUsb.key_is_nedded = false;
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }
                
                if ( USB_DRIVE_MOUNTED == usbUnmountDrive( ) )
                {
                    appData.state = APP_STATE_ERROR;
                }
                
            }
            else
            {
                if ( true == isDelayMsEnding() )
                {
                    appDataUsb.key_is_nedded = false;
                    sprintf(appError.message, "USB device detection took more than %ld ms", (long)MAX_DELAY_TO_DETECT_USB_DEVICE);
                    appError.currentLineNumber = __LINE__;
                    sprintf(appError.currentFileName, "%s", __FILE__);
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
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_ERROR\n" );
#endif
                if ( true == appDataLog.log_events )
                {
                   store_event(OF_STATE_ERROR); 
                }
                
                if (appError.number<ERROR_USB || appError.number>ERROR_USB_SUSPEND_DEVICE)
                {
                    if ( false == appDataLog.data_flush_before_error )
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
                        appError.ledColor_2 = LEDS_ERROR_CRITICAL_BATTERY;
                        break;
                    case ERROR_LOW_FOOD:
                        appError.ledColor_2 = LEDS_ERROR_CRITICAL_FOOD;
                        break;
                    case ERROR_LOW_VBAT:
                        appError.ledColor_2 = LEDS_ERROR_CRITICAL_VBAT;
                        break;
                    case ERROR_DOOR_CANT_CLOSE:
                        appError.ledColor_2 = LEDS_ERROR_CRITICAL_DOOR;
                        break;
                    case ERROR_LOW_RFID_FREQUENCY:
                        appError.ledColor_2 = LEDS_ERROR_RFID_FREQ;
                        break;
                    case ERROR_TIMEOUT_RFID_FREQUENCY:
                        appError.ledColor_2 = LEDS_ERROR_RFID_TIMEOUT;
                        break;
                    default:
                        appError.ledColor_2 = LEDS_OFF;
                        break;
                }
                
                if ( appError.number > ERROR_TOO_MANY_SOFTWARE_RESET )
                {
                    appError.ledColor_2 = LEDS_TOO_MANY_SOFTWARE_RESET;
                }

#if defined (USE_UART1_SERIAL_INTERFACE)
                printError( );
#endif
                rtcc_stop_alarm( );

                /* Close the door if it is opened */
                if ( ERROR_DOOR_CANT_CLOSE != appError.number && true == appData.flags.bit_value.systemInit )
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
                if ( USB_DRIVE_MOUNTED == appDataUsb.usbDriveStatus )
                {
                    usbUnmountDrive();
                }
                USBHostShutdown( );                
                powerUsbRfidDisable( );

                /* Reset the system after DELAY_BEFORE_RESET milli-seconds if non critical error occurred */
                if ( appError.number > ERROR_CRITICAL && appError.number < ERROR_TOO_MANY_SOFTWARE_RESET)
                {
                    if ( MAX_NUM_RESET <= appData.dsgpr0.bit_value.num_software_reset )
                    {
#if defined (USE_UART1_SERIAL_INTERFACE) 
                        printf( "\t/!\\ The system reset %u times => Critical error\n", MAX_NUM_RESET );   
 #endif                       
                        appError.number = appError.number + ERROR_TOO_MANY_SOFTWARE_RESET;
                        appData.previous_state = APP_STATE_IDLE;
                        break;
                    }
                    
                    TMR3_Start( );
                    setDelayMs( DELAY_BEFORE_RESET );
#if defined (USE_UART1_SERIAL_INTERFACE) 
                    printf( "\t/!\\ The system will reset in %u milli-seconds\n", DELAY_BEFORE_RESET );
#endif
                }
                else
                {
                    rtcc_set_alarm( 0, 0, 0, EVERY_10_SECONDS );
                    rtcc_start_alarm( );
                }
            }

            if ( appError.number > ERROR_CRITICAL && appError.number < ERROR_TOO_MANY_SOFTWARE_RESET)
            {
                /* A non critical error occurred                      */
                /* Wait DELAY_BEFORE_RESET milli-seconds and reset    */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (ENABLE_ERROR_LEDS)
                LedsStatusBlink( appError.ledColor_1, appError.ledColor_2, 150, 150 );
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

#if defined (USE_UART1_SERIAL_INTERFACE)
                printError( );            
#endif
                
#if defined (ENABLE_ERROR_LEDS)
                /* Status LED blinks */
                TMR3_Start( );
                setLedsStatusColor( appError.ledColor_1 );
                setDelayMs( 150 );
                while ( 0 == isDelayMsEnding( ) );
                setLedsStatusColor( appError.ledColor_2 );
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
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> DEFAULT_STATE\n" );
#endif
#if defined (USE_UART1_SERIAL_INTERFACE) 
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

    appData.buttonPressed = BUTTON_READ; /* initialized button status */

    /* Data logger */
    appDataLog.nCharBuffer = 0;
    appDataLog.numDataStored = 0;
    appDataLog.attractive_leds_current_color_index = 0;

    appDataLog.data_flush_before_error = false;
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
    appDataUsb.usbDriveStatus = USB_DRIVE_NOT_MOUNTED;
    appDataUsb.getValidDeviceAdress = false;
    appDataUsb.key_is_nedded = false;

    /* PIT tag data */
    appDataPitTag.didPitTagMatched = false;
    appDataPitTag. numPitTagDeniedOrColorA = 0;
    appDataPitTag.numPitTagAcceptedOrColorB = 0;
    appDataPitTag.pitTagIndexInList = 0;
    clearPitTagBuffers( );
    clearPitTagSringBuffers( );

    for ( i = 0; i < MAX_PIT_TAGS_LIST_NUMBER; ++i )
    {
        appDataPitTag.isPitTagdeniedOrColorA[i] = false;
    }

    appDataPitTag.timeout_unique_visit = 0;
        
    appDataLog.numBatteryLevelStored = 0;
    appDataLog.numRfidFreqStored = 0;
    appDataLog.numDs3231TempStored = 0;
    appDataLog.numTimeCalibStored = 0;
        
    appError.ledColor_1 = LEDS_ERROR;
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
    
    for (i=0;i<4;i++)
    {
        appData.siteid[i] = 'X';
    }
    appData.siteid[4] = '\0';
    
    appData.scenario_number = 0;
    
//    clear_bird_sensor_detected( );
    is_bird_detected = false;
    
    appDataEvent.file_type = EVENT_FILE_BINARY;
    
    appData.chk_food_level = false;
    
    appData.ext_temperature = 0.0;
    
}


/*******************************************************************************
End of File
 */
