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

    /* Check the Application State. */
    switch ( appData.state )
    {
        case APP_STATE_INITIALIZE:
        {
            I2C1_MESSAGE_STATUS i2c_status;

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
            }

            chk = checkImportantParameters( );
            switch ( chk )
            {
                case APP_CHECK_OK:
                    appData.state = APP_STATE_CONFIGURE_SYSTEM;
                    break;
                case APP_CHECK_BATTERY_PB:
                    appData.state = APP_STATE_LOW_BATTERY_LEVEL;
                    break;
                case APP_CHECK_VBAT_PB:
                    appData.state = APP_STATE_LOW_VBAT;
                    break;
                case APP_CHECK_FOOD_LEVEL_PB:
                    appData.state = APP_STATE_LOW_FOOD_LEVEL;
                    break;
                case APP_CHECK_RFID_FREQ_PB:
                    appData.state = APP_STATE_LOW_RFID_FREQUENCY;
                    break;
            }
            
            if (appData.state != APP_STATE_CONFIGURE_SYSTEM)
            {
                break;
            }
            
            /* Power PIR sensor early in the code because of starting delay before usable */
            powerPIREnable( );
            
            /* Reset PCA9622 device */
            i2c_status = I2C1_PCA9622_SoftwareReset( ); 
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
            print_I2C_message_status( i2c_status ); // I2C1_MESSAGE_STATUS
            printf( "\n" );
#endif

            i2c_status = initAttractiveLeds( );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
            if ( i2c_status )
            {
                printf( "\tAttractive LEDs: ok\n" );
            }
            else
            {
                printf( "\tAttractive LEDs: pb\n" );
            }
#endif
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
                powerUsbRfidEnable( );
                appDataUsb.key_is_nedded = true;
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
            }
            else
            {
                /* Blue status LED blinks as long USB key is accessed. */
                LedsStatusBlink( LED_RED, 25, 1975 );
                break;
            }

            /* Blue status LED blinks as long USB key is accessed. */
            LedsStatusBlink( LED_BLUE, 25, 475 );

            /* Set log file name => 20yymmdd.CSV (one log file per day). */
            if ( false == setLogFileName( ) )
            {
                appDataUsb.key_is_nedded = false;
                appData.state = APP_STATE_ERROR;
                break;
            }

            /* System configuration. */
            setLedsStatusColor( LED_BLUE );
            appData.flags.bit_value.systemInit = config_set( );

            if ( true == appData.flags.bit_value.systemInit )
            {
                setLedsStatusColor( LEDS_OFF );

                /* Servomotor power command enable. */
                servomotorPowerEnable( );
                appDataDoor.reward_door_status = DOOR_CLOSING;
                while ( DOOR_CLOSED != appDataDoor.reward_door_status );
                /* Check if door must remain open */
                if ( 1 == appDataDoor.remain_open )
                {
                    appDataDoor.reward_door_status = DOOR_OPENING;
                    while ( DOOR_OPENED != appDataDoor.reward_door_status );
                }
                /* Servomotor power command disable. */
                servomotorPowerDisable( );

                if ( true == appData.flags.bit_value.attractive_leds_status )
                {
                    while ( !RTCC_TimeGet( &appData.current_time ) )
                    {
                        Nop( );
                    }
                    if ( ( appData.current_time.tm_hour * 60 + appData.current_time.tm_min ) >= ( appDataAttractiveLeds.wake_up_time.tm_hour * 60 + appDataAttractiveLeds.wake_up_time.tm_min ) &&
                         ( appData.current_time.tm_hour * 60 + appData.current_time.tm_min )< ( appDataAttractiveLeds.sleep_time.tm_hour * 60 + appDataAttractiveLeds.sleep_time.tm_min ) )
                    {
                        setAttractiveLedsOn( );
                    }   
                }
                else
                {
                    TMR2_Stop( );
                }
                
                rtcc_set_alarm( appDataAlarmWakeup.time.tm_hour, appDataAlarmWakeup.time.tm_min, appDataAlarmWakeup.time.tm_sec, EVERY_SECOND );
                appData.state = APP_STATE_IDLE;
                
            }
            else
            {
                appDataUsb.key_is_nedded = false;
                powerUsbRfidDisable( );
                powerPIRDisable( );
                appData.state = APP_STATE_ERROR;
                break;
            }

            if ( USB_DRIVE_MOUNTED == usbUnmountDrive( ) )
            {
                appData.state = APP_STATE_ERROR;
            }

            appDataUsb.key_is_nedded = false;
            powerUsbRfidDisable( );
            
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_IDLE:
            /**
             * Application idle state.
             *  - waiting for a event during a timeout period
             *  - if user detached USB key go to error state
             *  - event PIR sensor, detecting movement near the system
             *  - after the timeout period go into sleep mode
             */
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_IDLE\n" );
#endif              
                if ( appData.timeout_standby > 0 )
                {
                    /* Timeout before going to standby mode */
                    setDelayMsStandBy( appData.timeout_standby );
                }
                
                rtcc_start_alarm( );
                
                /* Enable PIR sensor interruption for bird detection */
                EX_INT0_InterruptFlagClear( );
                EX_INT0_InterruptEnable( );  
                
            }
           
            /* Check PIR SENSOR detected.
             *  - recording the time of detected of the bird
             *  - if true go to APP_STATE_ERROR
             */
            if ( is_bird_sensor_detected( ) )
            {
                while ( !RTCC_TimeGet( &appDataLog.bird_arrived_time ) )
                {
                    Nop( );
                }
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf( "Bird detected\n" );
#endif
                /* Inizialised global variable datalogging. */
                clear_bird_sensor_detected( );
                appDataLog.is_reward_taken = false;

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
            if ( false == appData.flags.bit_value.attractive_leds_status )
            {
                if ( appData.timeout_standby > 0 && isDelayMsEndingStandBy( ) )
                {
                    appData.state = APP_STATE_STANDBY;
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
                    double t = rand( );

                    if ( ( t / RAND_MAX ) > 0.5 )
                    {
                        appDataAttractiveLeds.current_color_index = 0;
                    }
                    else
                    {
                        appDataAttractiveLeds.current_color_index = 1;
                    }
                    setAttractiveLedsColor( );
                }

                if ( DOOR_OPENED != appDataDoor.reward_door_status && RTCC_ALARM_OPEN_DOOR == appData.rtcc_alarm_action )
                {
                    /* Open reward door */
                    servomotorPowerEnable( );
                    appDataDoor.reward_door_status = DOOR_OPENING;
#if defined (USE_UART1_SERIAL_INTERFACE)
                    printf( "Opening reward door in action.\n" );
#endif
                    while ( DOOR_OPENED != appDataDoor.reward_door_status );
                    servomotorPowerDisable( );
                }

                if ( DOOR_CLOSED != appDataDoor.reward_door_status && RTCC_ALARM_CLOSE_DOOR == appData.rtcc_alarm_action )
                {
                    /* Close reward door */
                    servomotorPowerEnable( );
                    appDataDoor.reward_door_status = DOOR_CLOSING;
#if defined (USE_UART1_SERIAL_INTERFACE)
                    printf( "Closing reward door in action.\n" );
#endif
                    while ( DOOR_CLOSED != appDataDoor.reward_door_status );
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
                        appData.state = APP_STATE_LOW_BATTERY_LEVEL;
                        break;
                    }
                }
                if ( RTCC_FOOD_LEVEL_CHECK == appData.rtcc_alarm_action )
                {
                    flag = isEnoughFood( );
                    if ( false == flag )
                    {
                        appData.state = APP_STATE_LOW_FOOD_LEVEL;
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
                        appData.state = APP_STATE_LOW_RFID_FREQUENCY;
                        break;
                    }
                }

                appData.rtcc_alarm_action = RTCC_ALARM_IDLE;
            }

            /* Green status LED blinks in idle mode. */
            LedsStatusBlink( LED_GREEN, 25, 1975 );

#if defined (USE_UART1_SERIAL_INTERFACE)
            /* Get interaction with the serial terminal. */
            APP_SerialDebugTasks( );
#endif

            /* Check USER BUTTON detected. */
            button_user_state = USER_BUTTON_GetValue( );

            if ( button_user_state != previous_button_user_state )
            {
                previous_button_user_state = button_user_state;
                if ( BUTTON_PRESSED == button_user_state )
                {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_REMOTE_CONTROL_INFO)
                    printf( "User button pressed - " );
#endif
                    if ( APP_isRemoteControlConnected( ) )
                    {
                        appData.flags.bit_value.RemoteControlConnected = true; // FIXME: same of appData.mcp23017.status_bit.found
                        //appData.mcp23017.status_bit.found = true;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_REMOTE_CONTROL_INFO )
                        printf( "Remote control found.\n" );
#endif
                        appData.rc_previous_state = APP_STATE_IDLE;
                        appData.state = APP_STATE_REMOTE_CONTROL;
                        break;
                    }
                    else
                    {
                        appData.mcp23017.status_bit.found = false;
                        appData.mcp23017.status_bit.initialized = false;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_REMOTE_CONTROL_INFO )
                        printf( "Remote control not found.\n" );
#endif
                        appData.state = APP_STATE_FLUSH_DATA_TO_USB;
                    }
                }
                else
                {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_REMOTE_CONTROL_INFO)
                    printf( "User button released\n" );
#endif
                }
            }
//            
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
                
                EX_INT0_InterruptFlagClear( );
                EX_INT0_InterruptDisable( );
                rtcc_stop_alarm( );
                
                APP_Rfid_Init( );
            }

            APP_Rfid_Task( );

            if ( appData.flags.bit_value.NewValidPitTag )
            {
                switch ( appData.scenario_number )
                {
                    case NO_SCENARIO:
                    case OPEN_BAR:
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
                        appDataLog.is_pit_tag_denied = true;
                        if ( ( appDataPitTag.pitTagIndexInList <= ( appDataPitTag.numPitTagAcceptedOrColorB + appDataPitTag.numPitTagDeniedOrColorA + 1 ) / 2 ) && ( appDataLog.attractive_leds_current_color_index == 0 ) )
                        {
                            appDataLog.is_pit_tag_denied = false;
                        }
                        if ( ( appDataPitTag.pitTagIndexInList > ( appDataPitTag.numPitTagAcceptedOrColorB + appDataPitTag.numPitTagDeniedOrColorA + 1 ) / 2 ) && ( appDataLog.attractive_leds_current_color_index == 1 ) )
                        {
                            appDataLog.is_pit_tag_denied = false;
                        }
                        break;
                }

                RFID_Disable( );
                powerUsbRfidDisable( );
                clear_bird_sensor_detected( );
                
                if ( true == appDataLog.is_pit_tag_denied )
                {
#if defined (USE_UART1_SERIAL_INTERFACE) 
                    printf( "\tPIT tag %s denied.\n", appDataLog.bird_pit_tag_str );
#endif
                    if ( COLOR_ASSOCIATIVE_LEARNING == appData.scenario_number )
                    {
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
                RFID_Disable( );
                powerUsbRfidDisable( );
                clear_bird_sensor_detected( );

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
                appDataLog.is_pit_tag_denied = false;
                clearPitTagBuffers( );
                appData.state = APP_STATE_DATA_LOG;
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf( "RFID timeout.\n" );
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
                
                /* Power IR sensor here because start delay */
                IRSensorEnable( );
                /* Enable IR 1 sensor interruption (reward)*/
                EX_INT1_InterruptDisable();                    
                EX_INT1_PositiveEdgeSet( );  
                EX_INT1_InterruptFlagClear( );
                EX_INT1_InterruptEnable( );
                    
                appDataLog.is_reward_taken = false;

                /* Skip opening door if the "remain open" flag is set */
                if ( 1 == appDataDoor.remain_open )
                {
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

                /* Servomotor power command enable. */
                servomotorPowerEnable( );
                appDataDoor.reward_door_status = DOOR_OPENING;
                
            }

            if ( DOOR_OPENED == appDataDoor.reward_door_status )
            {
                /* Servomotor power command enable. */
                servomotorPowerDisable( );
                appData.state = APP_STATE_WAITING_CATCH_REWARD;
            }
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

                /* Timeout before door closing if reward is not taken */
                if (appData.timeout_taking_reward > 0)
                {
                    setDelayMs( appData.timeout_taking_reward );
                }
                
                appData.bird_is_taking_reward = false;

                clear_ir1_sensor( );
                
            }

#if !defined (PATH_HARDWARE_IR_SENSOR_DISABLE)
            if ( ( true == g_flag_ir1_sensor ) && ( false == appData.bird_is_taking_reward ) )
            {
                EX_INT1_InterruptDisable();                    
                EX_INT1_NegativeEdgeSet( );  
                EX_INT1_InterruptFlagClear( );
                EX_INT1_InterruptEnable(); 
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
                IRSensorDisable( );
                EX_INT1_InterruptDisable();                    
                EX_INT1_PositiveEdgeSet( );  
                EX_INT1_InterruptFlagClear( );
                clear_ir1_sensor( );

                appData.bird_is_taking_reward = false;
                appDataLog.is_reward_taken = true;
                appData.state = APP_STATE_CLOSING_DOOR;
                break;
            }
#endif

            /* Timeout elapsed and reward is not taken */
#if defined (PATH_HARDWARE_IR_SENSOR_DISABLE)
#warning "PATH_HARDWARE_IR_SENSOR_DISABLE is defined! See in fw02\src\app\app.h file for normal use of OpenFeeder"
            if ( true == isDelayMsEnding( ) )
#else
//            if ( true == isDelayMsEnding( ) && 0 == BAR_IR1_OUT_GetValue( ) )
            if ( true == isDelayMsEnding( ) && false == appData.bird_is_taking_reward )
#endif
            {
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf( "\tReward timeout.\n" );
#endif
                
                IRSensorDisable( );
                EX_INT1_InterruptDisable();                    
                EX_INT1_PositiveEdgeSet( );  
                EX_INT1_InterruptFlagClear( );
                clear_ir1_sensor( );

                appData.state = APP_STATE_CLOSING_DOOR;
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
                /* Enregistrement de l'heure de fin de détection de l'oiseau. */
                while ( !RTCC_TimeGet( &appDataLog.bird_quit_time ) )
                {
                    Nop( );
                }
                
                /* Skip closing door if the "remain open" flag is set */
                if ( 1 == appDataDoor.remain_open )
                {
                    // appDataUsb.getValidDeviceAdress = false;
                    appData.state = APP_STATE_DATA_LOG;
                    break;
                }

                /* Optional delay before closing the door */
                if (appDataDoor.close_delay > 0) 
                {
                    setDelayMs( appDataDoor.close_delay );
                    while ( false == isDelayMsEnding( ) )
                    {
                        Nop( );
                    }
                }
                
                /* Servomotor power command enable. */
                servomotorPowerEnable( );
                appDataDoor.reward_door_status = DOOR_CLOSING;
#if defined (USE_UART1_SERIAL_INTERFACE) 
                printf( "Closing reward door in action.\n" );
#endif
            }

            if ( DOOR_CLOSED == appDataDoor.reward_door_status )
            {
                /* Servomotor power command enable. */
                servomotorPowerDisable( );
                appData.state = APP_STATE_DATA_LOG;
            }
            break;
            /* -------------------------------------------------------------- */

            //        case APP_STATE_ATTACH_KEY:
            //            if (appData.state != appData.previous_state)
            //            {
            //                appData.previous_state = appData.state;
            //#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
            //                printf("> APP_STATE_ATTACH_KEY\n");
            //#endif
            //                powerUsbRfidEnable();
            //            }
            //
            //            if (appDataUsb.getValidDeviceAdress)
            //            {
            //                appData.state = APP_STATE_DATA_LOG;
            //            }
            //            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_DATA_LOG:
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_DATA_LOG\n" );
#endif
                //                if ((appDataLog.numDataStored >= (MAX_NUM_DATA_TO_STORE - 1)) && (false == appDataUsb.getValidDeviceAdress))
                //                {
                //                    appDataUsb.key_is_nedded = true;
                //                }
                //                else
                //                {
                //                    appDataUsb.key_is_nedded = false;
                //                }
            }

            //            LedsStatusBlink( LED_BLUE, 50, 250 );

            //            if ((true == appDataUsb.key_is_nedded) && (false == appDataUsb.getValidDeviceAdress))
            //            {
            //                appData.state = APP_STATE_ATTACH_KEY;
            //                break;
            //            }

            if ( false == dataLog( true ) )
            {
                appData.state = APP_STATE_ERROR;
            }
            else
            {
                appData.state = APP_STATE_IDLE;
            }

            //            if ((true == appDataUsb.key_is_nedded) && (true == appDataUsb.getValidDeviceAdress))
            //            {
            //                appDataUsb.getValidDeviceAdress = false;
            //                appDataUsb.key_is_nedded = false;
            //                powerUsbRfidDisable();
            //            }
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_FLUSH_DATA_TO_USB:
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_FLUSH_DATA_TO_USB\n" );
#endif
                /* Log data on USB device */
                appDataUsb.key_is_nedded = true;
                powerUsbRfidEnable( );
            }

            LedsStatusBlink( LED_BLUE, 50, 250 );

            if ( appDataUsb.getValidDeviceAdress )
            {
                if ( appDataLog.numDataStored > 0 )
                {
                    setLedsStatusColor( LED_BLUE );
                    /* Force data to be written on the USB device */
                    appDataLog.numDataStored = MAX_NUM_DATA_TO_STORE;
                    if ( false == dataLog( false ) )
                    {
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }

                if ( appDataLog.numBatteryLevelStored > 0 )
                {
                    setLedsStatusColor( LED_BLUE );
                    if ( FILEIO_RESULT_FAILURE == logBatteryLevel( ) )
                    {
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }

                if ( appDataLog.numRfidFreqStored > 0 )
                {
                    setLedsStatusColor( LED_BLUE );
                    if ( FILEIO_RESULT_FAILURE == logRfidFreq( ) )
                    {
                        appData.state = APP_STATE_ERROR;
                        break;
                    }
                }

            }
            else
            {
                break;
            }

            appData.state = APP_STATE_IDLE;
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_STANDBY:
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_STANDBY\n" );
#endif
            }

            Sleep( );

            appData.state = APP_STATE_IDLE;
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
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_SLEEP\n" );
#endif
                
                /* Stop RTCC interuption during shutdown process */
                rtcc_stop_alarm();
                
                /* Close the door if it is opened */
                if ( DOOR_CLOSED != appDataDoor.reward_door_status )
                {
                    /* Close reward door */
                    servomotorPowerEnable( );
                    appDataDoor.reward_door_status = DOOR_CLOSING;
#if defined (USE_UART1_SERIAL_INTERFACE)
                    printf( "Closing reward door in action.\n" );
#endif
                    while ( DOOR_CLOSED != appDataDoor.reward_door_status );
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
                    if ( appDataLog.numDataStored > 0 )
                    {
                        setLedsStatusColor( LED_BLUE );
                        /* Force data to be written on the USB device */
                        appDataLog.numDataStored = MAX_NUM_DATA_TO_STORE;
                        if ( false == dataLog( false ) )
                        {
                            appData.state = APP_STATE_ERROR;
                            break;
                        }
                    }

                    if ( appDataLog.numBatteryLevelStored > 0 )
                    {
                        setLedsStatusColor( LED_BLUE );
                        if ( FILEIO_RESULT_FAILURE == logBatteryLevel( ) )
                        {
                            appData.state = APP_STATE_ERROR;
                            break;
                        }
                    }

                    if ( appDataLog.numRfidFreqStored > 0 )
                    {
                        setLedsStatusColor( LED_BLUE );
                        if ( FILEIO_RESULT_FAILURE == logRfidFreq( ) )
                        {
                            appData.state = APP_STATE_ERROR;
                            break;
                        }
                    }

                    powerUsbRfidDisable( );
                    CMD_VDD_APP_V_USB_SetLow( ); // --> powerUsbRfidDisable( );
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
            setDateTime( 17, 9, 21, 5, 59, 30 );
#endif
            
            /* Set alarm for wake up time */
            rtcc_set_alarm( appDataAlarmWakeup.time.tm_hour, appDataAlarmWakeup.time.tm_min, appDataAlarmWakeup.time.tm_sec, EVERY_DAY );

            rtcc_start_alarm();
            
            setDelayMs( 500 );
            while ( false == isDelayMsEnding( ) )
            {
                Nop( );
            }
                    
            Sleep( );

//#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
//            printf( "\n" );
//#endif
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
            }
            
#if defined (TEST_RTCC_SLEEP_WAKEUP)
            /* Next line for debugging sleep/wakeup only */
            /* Should be commented in normal mode */
            /* Modify time value according to sleep values in the CONFIG.INI file */
            setDateTime( 17, 9, 21, 22, 59, 0 );
#endif
            
            rtcc_set_alarm( appDataAlarmWakeup.time.tm_hour, appDataAlarmWakeup.time.tm_min, appDataAlarmWakeup.time.tm_sec, EVERY_SECOND );
            
            APP_Initialize( );
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_REMOTE_CONTROL:
            /**
             * Application idle state.
             *  - 
             */
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_REMOTE_CONTROL\n" );
#endif
                APP_remoteControlInitialize( );
            }

            /* Blue status LED blinks when the remote control is plugged. */
            LedsStatusBlink( LED_BLUE, 500, 500 );

            /* Check USER BUTTON detected.
             *  - if true RemoteControlConnected = false and go to APP_STATE_IDLE
             */
            button_user_state = USER_BUTTON_GetValue( );

            if ( button_user_state != previous_button_user_state )
            {
                previous_button_user_state = button_user_state;
                if ( BUTTON_PRESSED == button_user_state )
                {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_REMOTE_CONTROL_INFO )
                    printf( "USER BUTTON PRESSED\n" );
#endif
                    clearRemoteControlDisplay( );
                    appData.flags.bit_value.RemoteControlConnected = false;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_REMOTE_CONTROL_INFO )
                    printf( "Remote control disconnected\n" );
#endif
                }
            }

            if ( appData.flags.bit_value.RemoteControlConnected )
            {
                APP_remoteControlTask( );
            }
            else
            {
                appData.state = appData.rc_previous_state;
            }
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_LOW_BATTERY_LEVEL:
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_LOW_BATTERY\n" );
#endif
                appDataUsb.key_is_nedded = false;
                /* Log data on USB device */
                powerUsbRfidEnable( );
                appDataUsb.key_is_nedded = true;
            }

            if ( true == appDataUsb.key_is_nedded )
            {
                if ( appDataUsb.getValidDeviceAdress )
                {
                    if ( appDataLog.numDataStored > 0 )
                    {
                        /* Force data to be written on the USB device */
                        appDataLog.numDataStored = MAX_NUM_DATA_TO_STORE;
                        if ( false == dataLog( true ) )
                        {
                            appData.state = APP_STATE_ERROR;
                            break;
                        }
                    }

                    if ( appDataLog.numBatteryLevelStored > 0 )
                    {
                        if ( FILEIO_RESULT_FAILURE == logBatteryLevel( ) )
                        {
                            appData.state = APP_STATE_ERROR;
                            break;
                        }
                    }
                }
                else
                {
                    break;
                }
            }

            appData.state = APP_STATE_ERROR;
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_LOW_VBAT:
        case APP_STATE_LOW_FOOD_LEVEL:
        case APP_STATE_LOW_RFID_FREQUENCY:
        case APP_STATE_ERROR:
            /* Step in APP_Tasks( ) failed.
             * TODO: APP_STATE_ERROR - This state should not be blocking!
             */
            if ( appData.state != appData.previous_state )
            {

                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                if ( appData.state == APP_STATE_ERROR )
                {
                    printf( "> APP_STATE_ERROR\n" );
                }

                if ( appData.state == APP_STATE_LOW_VBAT )
                {
                    printf( "> APP_STATE_LOW_VBAT\n" );
                }

                if ( appData.state == APP_STATE_LOW_FOOD_LEVEL )
                {
                    printf( "> APP_STATE_LOW_FOOD_LEVEL\n" );
                }

                if ( appData.state == APP_STATE_LOW_RFID_FREQUENCY )
                {
                    printf( "> APP_STATE_LOW_RFID_FREQUENCY\n" );
                }
#endif
#if defined (USE_UART1_SERIAL_INTERFACE)
                printCurrentDate( );
                printf(" ");
                printError( );
#endif
                rtcc_stop_alarm( );

                /* Close the door if it is opened */
                if ( DOOR_CLOSED != appDataDoor.reward_door_status )
                {
                    /* Close reward door */
                    servomotorPowerEnable( );
                    appDataDoor.reward_door_status = DOOR_CLOSING;
#if defined (USE_UART1_SERIAL_INTERFACE)
                    printf( "Closing reward door in action.\n" );
#endif
                    while ( DOOR_CLOSED != appDataDoor.reward_door_status );
                    servomotorPowerDisable( );
                }

                /* Set peripherals Off. */
                setAttractiveLedsOff( );
                powerPIRDisable( );
                EX_INT0_InterruptDisable( );
                RFID_Disable( );
                USBHostShutdown( );
                powerUsbRfidDisable( );
                IRSensorDisable( );
                TMR3_Start( );
                setDelayMs( 5000 );

#if defined (USE_UART1_SERIAL_INTERFACE) 
                if ( appError.number > ERROR_LOW_VBAT )
                {
                    printf( "%d - Need to reset\n", T3CONbits.TON );
                }
#endif
            }

            /* FIXME : Reset every 5 sec */
            /* Reset the system if non critical error occurred */
            if ( appError.number > ERROR_LOW_VBAT )
            {
                if ( isDelayMsEnding( ) )
                {
#if defined (USE_UART1_SERIAL_INTERFACE)               
                    printf( "RESET...\n" );
#endif
                    __asm__ volatile ( "reset" ); /* reboot the board!!! */
                }
            }

            /* Red status LED blinks */
            LedsStatusBlink( appError.ledColor, 50, 450 );

#if defined (USE_UART1_SERIAL_INTERFACE)
            /* Get interaction with the serial terminal. */
            APP_SerialDebugTasks( );
#endif
            /* Check USER BUTTON detected. */
            button_user_state = USER_BUTTON_GetValue( );

            if ( button_user_state != previous_button_user_state )
            {
                previous_button_user_state = button_user_state;
                if ( BUTTON_PRESSED == button_user_state )
                {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_REMOTE_CONTROL_INFO)
                    printf( "User button pressed - " );
#endif
                    if ( APP_isRemoteControlConnected( ) )
                    {
                        appData.flags.bit_value.RemoteControlConnected = true; // FIXME: same of appData.mcp23017.status_bit.found
                        //appData.mcp23017.status_bit.found = true;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_REMOTE_CONTROL_INFO )
                        printf( "Remote control found.\n" );
#endif
                        appData.rc_previous_state = APP_STATE_ERROR;
                        appData.state = APP_STATE_REMOTE_CONTROL;
                        break;
                    }
                    //                    else
                    //                    {
                    //                        appData.mcp23017.status_bit.found = false;
                    //                        appData.mcp23017.status_bit.initialized = false;
                    //#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_REMOTE_CONTROL_INFO )
                    //                        printf( "Remote control not found.\n" );
                    //#endif
                    //                        appData.state = APP_STATE_FLUSH_DATA_TO_USB;
                    //                    }
                }
                else
                {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_REMOTE_CONTROL_INFO)
                    printf( "User button released\n" );
#endif
                }
            }
            break;
            /* -------------------------------------------------------------- */

        default:
            //            Nop( );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
            printf( "> APP_STATE_DEFAULT\n" );
#endif
            setLedsStatusColor( LED_RED );
            break;
    }
}


void APP_Initialize( void )
{
    int i, j;

    TMR4_Stop( );
    
    /* Attractive LEDs initialize */
    setAttractiveLedsOff( );
    appDataAttractiveLeds.current_color_index = 0;
    appDataAttractiveLeds.alt_sec_elapsed = 0;

    /* Initialize color */
    appDataAttractiveLeds.red[0] = 0;
    appDataAttractiveLeds.red[1] = 0;

    appDataAttractiveLeds.green[0] = 0;
    appDataAttractiveLeds.green[1] = 0;

    appDataAttractiveLeds.blue[0] = 0;
    appDataAttractiveLeds.blue[1] = 0;

    /* APP state task initialize */
    appData.state = APP_STATE_INITIALIZE;
    appData.previous_state = APP_STATE_ERROR;

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

    appDataLog.numBatteryLevelStored = 0;

    appData.bird_is_taking_reward = false;

    /* USB host */
    appDataUsb.usbDriveStatus = USB_DRIVE_NOT_MOUNTED;
    appDataUsb.getValidDeviceAdress = false;
    appDataUsb.key_is_nedded = false;

    memset( appData.siteid, '\0', 5 );

    /* PIT tag data */
    appDataPitTag.didPitTagMatched = false;
    appDataPitTag. numPitTagDeniedOrColorA = 0;
    appDataPitTag.numPitTagAcceptedOrColorB = 0;
    appDataPitTag.pitTagIndexInList = 0;

    for ( i = 0; i < MAX_PIT_TAGS_LIST_NUMBER; i++ )
    {
        appDataPitTag.isPitTagdeniedOrColorA[i] = false;
    }

    for ( i = 0; i < 24; ++i )
    {
        for ( j = 0; j < 2; ++j )
        {
            appDataLog.battery_level[i][j] = 0;
        }
    }
    appDataLog.numBatteryLevelStored = 0;

    for ( i = 0; i < 96; ++i )
    {
        for ( j = 0; j < 3; ++j )
        {
            appDataLog.rfid_freq[i][j] = 0;
        }
    }
    appDataLog.numRfidFreqStored = 0;

    appError.ledColor = LED_RED;
    appError.number = ERROR_NONE;

    appData.punishment_delay = 0;

    appData.rfid_signal_detected = false;

}


/*******************************************************************************
End of File
 */
