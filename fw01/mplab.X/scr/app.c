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
// dont 0A est interpr�ter comme la valeur suivante !
// --> pour r�soudre cela dans Docklight, il faut appuyer sur la combinaison de touches
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
//       - on d�tecte lorsque l'oiseau n'est plus dans l'ouverure (barri�re IR non coup� + tempo ? ou  d�tection de l'oiseau qui part, lecture PIT Tag)
//       - ou tempo (param�trable depuis fichier CONFIG.INI) d�s que la barri�re IR n'est plus coup�
// TODO: Datalogging with Timestamp: RTCC + ms ( https://github.com/OpenFeeder/project/tree/master/demos/pic24/apps/pic24fj128gb204_rtcc/demo_rtcc_mc3.X )
//       necessite de reboucler la sortie RTCC_OUT sur une entr�e d'interruption externe
// TODO: faire clignotter rapidement la LED status blue lorsque l'on acc�de � la cl� USB
// TODO: When no USB key the Blue LED status set High! Must only have Red LED toggle every 200 ms!
// TODO: Debugging RFID: voie 1 DMOD_OUT, voie 2 INT4 event on LED_STATUS_B, voie 3 read DMOD_OUT event from TMR4 on LED_STATUS_G
// TODO: Si il n'y a pas de cl� USB la machine d'�tat reste bloqu� en APP_STATE_USB_WAITING_FOR_ATTACH
//       apr�s affichage du message de l'APP_STATE_INIT et 9
//       il faudrait mettre plusieurs machines d'�tats en parall�le USB, RFID, DATALOGGER, RTCC, I2C REMOTE CONTROL, ...
//       afin de pouvoir g�rer depuis la machine d'�tats principale les diff�rents modes.
// TODO: Mesure de la p�riode de RDY/CLK, attention TMR3 n'est plus � 10 ms mais � 20 ms --> ajustement de 
// TODO: Repenser la michine d'�tats principale pour avoir seulement les process principaux et faire des machine d'�tats ind�pendante pour les diff�rents modes
// TODO: Definir une convention de nommage comme pour "typedef struct" pour les diff�rentes variables de chaque machine d'�tats
// TODO: Check datasheet "19.1.2.3 VBUS Voltage Generation with External Devices" p269
// TODO: Check the calibrated of the real-time crystal input (see "22.4 Calibration" DS30005009C-page 326)

// FIXME: in USB_HostInterruptHandler( ) occur every time!
//#if defined (DEBUG_UART)
//            printf( "USB_HostInterruptHandler()\n" ); // display Sleeping message
//#endif 

// DONE: Make Power Management (USB powered)
// DONE: Mettre en param�tre dans le fichier de config pour la valeur de g_timeout_taking_reward (waiting timeout in sec)
// DONE: CMD_VDD_APP_V_USB on RB5
// DONE: Mise en place la mise � jour de l'heure par l'interface de debug
// DONE: Servomotor Open and Close serial command do only one step, must reaped until ending position occur!
// DONE: Implement min, max value of servomotor position from CONFIG.INI file ?
// DONE: Activer le boitier t�l�commande lorsque celui-ci est d�tect�
//       Ajouter un test p�riodique du boitier t�l�commande et faire remonter une trace de l'�v�nement lorsque celle ci est branch� ou d�branch�
//       ou lancer la d�tection du boitier t�l�commande lors de l'appuis sur le bouton User
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

/* Binary to Ascii text converter with simple lookup array */
const char bin2ascii_tab[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };


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

    /* Check the Application State. */
    switch ( appData.state )
    {
        case APP_STATE_INIT:
            /**
             * Initializing the application.
             * (en) Application initialization when starting the main power.
             * (fr) Initialisation de l'application lors du d�marrage de l'alimentation principale.
             */
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_INIT\n" );
#endif
                DSCONbits.RELEASE = 0; /* DS30005009C-page 165 */
            }

            /* Check battery level at startup. */
            if ( false == isPowerBatteryGood( ) )
            {
                appData.state = APP_STATE_BATTERY_LOW;
                break;
            }

            /* Check food level */
            IRSensorEnable( );
            if ( BAR_IR2_OUT_GetValue( ) == 0 )
            {
                printf( "Enough food\n" );
            }
            else
            {
                printf( "Not enough food\n" );
            }
            IRSensorDisable( );

            /* Set log file name => 20yymmdd.CSV (one log file per day). */
            if ( false == setLogFileName( ) )
            {
                appData.state = APP_STATE_ERROR;
                break;
            }

            setLedsStatusColor( LEDS_OFF );
            appData.state = APP_STATE_INIT_MOUNT_USB_KEY;
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_INIT_MOUNT_USB_KEY:
            /**
             * Initializing the application.
             *  - waiting for USB key monting
             *  - if USB key monting go to APP_STATE_INIT_CONFIG_SYS
             */
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_INIT_MOUNT_USB_KEY\n" );
#endif
                appDataUsb.usbDriveStatus = USB_DRIVE_NOT_MOUNTED;
                CMD_VDD_APP_V_USB_SetHigh( ); /* Powering VDD APP for USB and RFID. */
                appDataUsb.key_is_nedded = true;
            }

            if ( appDataUsb.getValidDeviceAdress )
            {
                /* Configure the system if a USB key is plugged. */
                if ( USB_DRIVE_MOUNTED == usbMountDrive( ) )
                {
                    appData.state = APP_STATE_INIT_CONFIG_SYS;
                    break;
                }
            }

#if defined (USE_UART1_SERIAL_INTERFACE)
            APP_SerialDebugTasks( );
            // TODO: Display all event on serial terminal for test accessoire.
#endif
            /* Red status LED blinks until the user plug a USB key */
            LedsStatusBlink( LED_RED, 25, 1975 );
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_INIT_CONFIG_SYS:
            /* APP_STATE_INIT_CONFIG_SYS run only one time. */
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_INIT_CONFIG_SYS\n" );
#endif
            }

            /* Blue status LED blinks as long USB key is acessed. */
            LedsStatusBlink( LED_BLUE, 25, 475 );

            /* System configuration. */
            appData.flags.bit_value.systemInit = config_set( );

            if ( true == appData.flags.bit_value.systemInit )
            {
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf( "System initialized\n" );
#endif
                /* Servomotor power command enable. */
                servomotorPowerEnable( );
                appDataDoor.reward_door_status = DOOR_CLOSING;
                // TODO: Check door position or wainting for timeout closing
                //                while ( DOOR_IDLE != appData.reward_door_status );
                while ( DOOR_CLOSED != appDataDoor.reward_door_status );
                /* Servomotor power command disable. */
                servomotorPowerDisable( );

                setLedsStatusColor( LEDS_OFF );
                //                appData.state = APP_STATE_IDLE;
                appData.state = APP_STATE_RFID_MESURING_RDYCLK;
            }
            else
            {
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf( "System not initialized\n" );
#endif
                //                sprintf( appError.message, "Unable to initialize the system" );
                //                appError.currentLineNumber = __LINE__;
                //                sprintf( appError.currentFileName, "%s", __FILE__ );
                appData.state = APP_STATE_ERROR;
            }

            if ( FILEIO_RESULT_FAILURE == logBatteryLevel( ) )
            {
                appData.state = APP_STATE_ERROR;
                break;
            }

            if ( usbUnmountDrive( ) == USB_DRIVE_MOUNTED )
            {
                appData.state = APP_STATE_ERROR;
            }

            USBHostShutdown( );

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
                    /* Timeout before going to sleep mode */
                    setDelayMs( appData.timeout_standby );
                }
            }

            /* Green status LED blinks in idle mode. */
            LedsStatusBlink( LED_GREEN, 25, 1975 );

#if defined (USE_UART1_SERIAL_INTERFACE)
            /* Get interaction with the serial terminal. */
            APP_SerialDebugTasks( );
#endif

            if ( false == isPowerBatteryGood( ) )
            {
                appData.state = APP_STATE_BATTERY_LOW;
                break;
            }

            /* If VDD_APP_V_USB power is ON, check if USB key is detached by user.
             *  - if true go to APP_STATE_ERROR
             */
            //            if ( true == CMD_VDD_APP_V_USB_GetValue( ) )
            //            {
            //            if (USB_DEVICE_DETACHED == USBHostDeviceStatus(appDataUsb.deviceAddress))
            //            {
            //                appData.state = APP_STATE_ERROR;
            //                break;
            //            }
            //            }

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

                appData.state = APP_STATE_RFID_READING_PIT_TAG;
                break;
            }

            /* Check USER BUTTON detected.
             *  - if true go to ...
             */
            button_user_state = USER_BUTTON_GetValue( );

            if ( button_user_state != previous_button_user_state )
            {
                previous_button_user_state = button_user_state;
                if ( BUTTON_PRESSED == button_user_state )
                {
                    //#if defined (USE_UART1_SERIAL_INTERFACE)
                    //                    printf("User button pressed - ");
                    //#endif
                    if ( APP_isRemoteControlConnected( ) )
                    {
                        appData.flags.bit_value.RemoteControlConnected = true; // FIXME: same of appData.mcp23017.status_bit.found
                        //appData.mcp23017.status_bit.found = true;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_REMOTE_CONTROL_INFO )
                        printf( "Remote control found.\n" );
#endif
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
                    }
                }
                //                else
                //                {
                //#if defined (USE_UART1_SERIAL_INTERFACE)
                //                    printf("User button released\n");
                //#endif
                //                }
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
                    setAttractiveLedsOn( );
                }
                if ( DOOR_OPENED != appDataDoor.reward_door_status && RTCC_ALARM_OPEN_DOOR == appData.rtcc_alarm_action )
                {
                    /* Open reward door */
                    servomotorPowerEnable( );
                    appDataDoor.reward_door_status = DOOR_OPENING;
                    printf( "Opening reward door in action.\n" );
                    while ( DOOR_OPENED != appDataDoor.reward_door_status );
                    servomotorPowerDisable( );
                }
                if ( DOOR_CLOSED != appDataDoor.reward_door_status && RTCC_ALARM_CLOSE_DOOR == appData.rtcc_alarm_action )
                {
                    /* Close reward door */
                    servomotorPowerEnable( );
                    appDataDoor.reward_door_status = DOOR_CLOSING;
                    printf( "Closing reward door in action.\n" );
                    while ( DOOR_CLOSED != appDataDoor.reward_door_status );
                    servomotorPowerDisable( );
                }

                appData.rtcc_alarm_action = RTCC_ALARM_IDLE;

            }

            /* Check TIMEOUT IDLE MODE endding.
             *  - if false go to APP_STATE_STANDBY
             */
            if ( false == appData.flags.bit_value.attractive_leds_status )
            {
                if ( appData.timeout_standby > 0 && isDelayMsEnding( ) )
                {
                    appData.state = APP_STATE_STANDBY;
                }
            }
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
                APP_Rfid_Init( );

            }

            APP_Rfid_Task( );

            if ( appData.flags.bit_value.NewValidPitTag )
            {

                /* Check PIT tag read is in the PIT tags list */
                findPitTagInList( );

                if ( true == appDataPitTag.didPitTagMatched || OPEN_BAR == appData.scenario_number )
                {

                    switch ( appData.scenario_number )
                    {
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
                            /* TODO */
                            appDataLog.is_pit_tag_denied = true;
                            if ( appDataPitTag.pitTagIndexInList <= ( appDataPitTag.numPitTagAcceptedOrColorB + appDataPitTag.numPitTagDeniedOrColorA + 1 ) / 2 && appDataLog.attractive_leds_current_color_index == 0 )
                            {
                                appDataLog.is_pit_tag_denied = false;
                            }
                            if ( appDataPitTag.pitTagIndexInList > ( appDataPitTag.numPitTagAcceptedOrColorB + appDataPitTag.numPitTagDeniedOrColorA + 1 ) / 2 && appDataLog.attractive_leds_current_color_index == 1 )
                            {
                                appDataLog.is_pit_tag_denied = false;
                            }
                            break;
                    }
                }
                else
                {
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
                    printf( "PIT tag not matched - Consider as denied\n" );
#endif
                    /* TODO */
                    appDataLog.is_pit_tag_denied = true;
                }

                RFID_Disable( );
                clear_bird_sensor_detected( );

                if ( true == appDataLog.is_pit_tag_denied )
                {
#if defined (USE_UART1_SERIAL_INTERFACE)
                    printf( "PIT Tag denied\n" );
#endif
                    appData.state = APP_STATE_DATA_LOG;
                }
                else
                {
                    appData.state = APP_STATE_OPENING_REWARD_DOOR;
                }
                break;
            }

            /* Test if delay detect PIT Tags in ending. (20x 160 ms) */
            if ( g_timeout_reading_pit_tag == 0 )
            {
                RFID_Disable( );
                clear_bird_sensor_detected( );
                sprintf( appDataLog.bird_pit_tag_str, "XXXXXXXXXX" );
                appDataLog.is_reward_taken = false;
                appDataLog.is_pit_tag_denied = false;
                clearPitTagBuffers( );
                appData.state = APP_STATE_DATA_LOG;
            }
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_OPENING_REWARD_DOOR:
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_OPENING_REWARD_DOOR\n" );
#endif

                appDataLog.is_reward_taken = false;

                /* Check if door is already open */
                if ( 1 == appDataDoor.remain_open )
                {
                    appData.state = APP_STATE_WAITING_CATCH_REWARD;
                    break;
                }

                /* Delay before door open */
                setDelayMs( appDataDoor.open_delay );
                while ( false == isDelayMsEnding( ) )
                {
                    Nop( );
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

                IRSensorEnable( );
                /* Timeout before door closing if reward is not taken */
                setDelayMs( appData.timeout_taking_reward );
                //                g_timeout_taking_reward = appData.timeout_taking_reward;
                appData.bird_is_taking_reward = false;
            }

            if ( ( g_flag_ir1_sensor == true ) && ( appData.bird_is_taking_reward == false ) )
            {
                /* REWARD_IR_SENSOR true. */
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf( "Taking reward detected.\n" );
#endif
                g_flag_ir1_sensor = false;
                appData.bird_is_taking_reward = true;
                break;
            }

            /* low --> coupure de la barri�re infra rouge */
            if ( ( BAR_IR1_OUT_GetValue( ) == 0 ) && ( appData.bird_is_taking_reward == true ) )
            {
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf( "Reward taken.\n" );
#endif
                IRSensorDisable( );
                appData.bird_is_taking_reward = false;
                appDataLog.is_reward_taken = true;
                appData.state = APP_STATE_CLOSING_REWARD_DOOR;
                break;
            }

            /* Timeout elapsed and reward is not taken */
            if ( true == isDelayMsEnding( ) && BAR_IR1_OUT_GetValue( ) == 0 )
                //            if ( g_timeout_taking_reward == 0 && BAR_IR1_OUT_GetValue( ) == 0 )
            {
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf( "Reward timeout.\n" );
#endif
                IRSensorDisable( );
                appData.state = APP_STATE_CLOSING_REWARD_DOOR;
            }
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_CLOSING_REWARD_DOOR:
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_CLOSING_REWARD_DOOR\n" );
#endif

                /* Enregistrement de l'heure de fin de d�tection de l'oiseau. */
                while ( !RTCC_TimeGet( &appDataLog.bird_quit_time ) )
                {
                    Nop( );
                }

                /* Check if door must remain open */
                if ( 1 == appDataDoor.remain_open )
                {
                    appData.state = APP_STATE_DATA_LOG;
                    break;
                }

                /* Delay before door close */
                setDelayMs( appDataDoor.close_delay );
                while ( false == isDelayMsEnding( ) )
                {
                    Nop( );
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

        case APP_STATE_DATA_LOG:
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_DATA_LOG\n" );
#endif
            }

            LedsStatusBlink( LED_BLUE, 50, 250 );

            if ( false == dataLog( ) )
            {
                appData.state = APP_STATE_ERROR;
            }
            else
            {
                appData.state = APP_STATE_IDLE;
            }
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
             *  - mise en sommeil simple du syst�me
             *    OPERATION DURING SLEEP MODES - PIR sensor and alarm deep sleep
             *  - lors de la sortie du mode SLEEP, remettre en service les fonctions de l'OpenFeeder
             *  - passer � l'�tat APP_STATE_IDLE
             */

            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_SLEEP\n" );
#endif
            }

            rtcc_set_alarm( appDataAlarmWakeup.time.tm_hour, appDataAlarmWakeup.time.tm_min, appDataAlarmWakeup.time.tm_sec, EVERY_DAY );

            /* Close the door if it is opened */
            if ( DOOR_CLOSED != appDataDoor.reward_door_status )
            {
                /* Close reward door */
                servomotorPowerEnable( );
                appDataDoor.reward_door_status = DOOR_CLOSING;
                printf( "Closing reward door in action.\n" );
                while ( DOOR_CLOSED != appDataDoor.reward_door_status );
                servomotorPowerDisable( );
            }

            /* Turn attractive LEDs off */
            if ( ATTRACTIVE_LEDS_ON == appDataAttractiveLeds.status )
            {
                setAttractiveLedsOff( );
            }

            /* Turn status LED off */
            setLedsStatusColor( LEDS_OFF );

            Sleep( );

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
            printf( "\n" );
#endif

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

            rtcc_set_alarm( appDataAlarmWakeup.time.tm_hour, appDataAlarmWakeup.time.tm_min, appDataAlarmWakeup.time.tm_sec, EVERY_SECOND );

#if defined (USE_UART1_SERIAL_INTERFACE)
            printf( "Awaken from sleep mode!\n" );
#endif 
            appData.openfeeder_state = OPENFEEDER_IS_AWAKEN;
            appData.state = APP_STATE_IDLE;
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
                printf( "> APP_STATE_REMOTE_CONTROL\n" ); // GREEN LED for AB proto
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
                    //#if defined (USE_UART1_SERIAL_INTERFACE)
                    //                    printf("USER BUTTON PRESSED\n");
                    //#endif
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
                appData.state = APP_STATE_IDLE;
            }
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_BATTERY_LOW:
            /* APP_STATE_BATTERY_LOW run only one time. */
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_BATTERY_LOW\n" );
#endif
                /* Set peripherals Off. */
                setLedsStatusColor( LEDS_OFF );
                setAttractiveLedsOff( );
                EX_INT0_InterruptDisable( );
                RFID_Disable( );
                CMD_VDD_APP_V_USB_SetLow( ); /* Shut down VDD APP for USB and RFID. */
                IRSensorDisable( );
            }

            Sleep( );
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_RFID_MESURING_RDYCLK:

            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_RFID_MESURING_RDYCLK\n" );
#endif
            }

            CMD_VDD_APP_V_USB_SetHigh( ); /* Power up VDD APP for USB and RFID. */

            EM4095_SHD_ENABLE( );
            /* Set-up time after a sleep period - Tset: 35ms */
            /* Tableau page 5 - datasheet EM4095*/
            setDelayMsEM4095( EM4095_TSET_DELAY_MS );
            while ( isDelayMsEndingEM4095( ) == false )
            {
                Nop( );
            }

            while ( EM4095_SHD_GetValue( ) == false )
            {
                Nop( );
            }
            EX_INT3_InterruptEnable( );
            while ( g_new_value_of_em4095_rdyclk_measurement == false )
            {
                Nop( );
            }
            EX_INT3_InterruptDisable( );

#if defined (USE_UART1_SERIAL_INTERFACE)
            printf( "RDY/CLK signal frequency: %u (x10Hz)\n", rdyclk_count_in_10ms * 5 );
#endif    
            g_new_value_of_em4095_rdyclk_measurement = false;
            //            RFID_Disable();
            CMD_VDD_APP_V_USB_SetLow( ); /* Shut down VDD APP for USB and RFID. */

            appData.state = APP_STATE_IDLE;

            break;

        case APP_STATE_ERROR:
            /* Step in APP_Tasks( ) failed.
             * TODO: APP_STATE_ERROR - This state should not be blocking!
             */
            if ( appData.state != appData.previous_state )
            {


                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_ERROR\n" );
#endif
#if defined (USE_UART1_SERIAL_INTERFACE) 
                printError( );
#endif
                clearError( );
            }

            /* Red status LED blinks until the user plug a USB key */
            LedsStatusBlink( LED_RED, 50, 450 );

            //            if ( USBHostDeviceStatus( appDataUsb.deviceAddress ) == USB_DEVICE_DETACHED )
            //            {
            //                setLedsStatusColor( LEDS_OFF );
            //                appData.state = APP_STATE_INIT_MOUNT_USB_KEY;
            //            }

            break;
            /* -------------------------------------------------------------- */

        default:
            Nop( );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
            printf( "> APP_STATE_DEFAULT\n" );
#endif
            setLedsStatusColor( LED_RED );
            break;
    }
}


void APP_Initialize( void )
{
    int i;

    /* Attractive LEDs initialize */
    setAttractiveLedsOff( );
    appDataAttractiveLeds.current_color_index = 0;
    appDataAttractiveLeds.alt_sec_elapsed = 0;

    /* APP state task initialize */
    appData.state = APP_STATE_INIT;
    appData.previous_state = APP_STATE_ERROR;

    appData.openfeeder_state = OPENFEEDER_IS_AWAKEN;
    appData.rtcc_alarm_action = RTCC_ALARM_WAKEUP_OPENFEEDER;

    /* Initialize all flags */
    //appData.flags.bit_value.systemInit = false;
    appData.flags.reg = 0;

    /* I2C */
    memset( appData.i2c_add_found, 0, MAX_OF_UNKNOWN_I2C_8_BIT_SLAVE_ADD ); // clear tab i2c_add_found
    appData.mcp23017.status_reg = 0;
    /* Set all digit Off. */
    appData.digit[0] = 0xFF;
    appData.digit[1] = 0xFF;
    appData.digit[2] = 0xFF;
    appData.digit[3] = 0xFF;

    appData.buttonPressed = BUTTON_READ; /* initilized button status */

    /* Servomotor */
    /* See globale variable in servomotor.c */

    /* Data logger */
    appDataLog.nCharBuffer = 0;
    appDataLog.numDataStored = 0;
    appDataLog.attractive_leds_current_color_index = 0;

    appData.bird_is_taking_reward = false;

    /* USB host */
    appDataUsb.getValidDeviceAdress = false;
    appDataUsb.key_is_nedded = false;

    memset( appData.siteid, '\0', 5 );

    rtcc_set_alarm( appDataAlarmWakeup.time.tm_hour, appDataAlarmWakeup.time.tm_min, appDataAlarmWakeup.time.tm_sec, EVERY_SECOND );

    /* PIT tag data */
    appDataPitTag.didPitTagMatched = false;
    appDataPitTag. numPitTagDeniedOrColorA = 0;
    appDataPitTag.numPitTagAcceptedOrColorB = 0;
    appDataPitTag.pitTagIndexInList = 0;
    for ( i = 0; i < MAX_PIT_TAGS_LIST_NUMBER; i++ )
    {
        appDataPitTag.isPitTagdeniedOrColorA[i] = false;
    }

}


/*******************************************************************************
End of File
 */