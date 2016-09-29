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
             * (fr) Initialisation de l'application lors du démarrage de l'alimentation principale.
             */
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_INIT\n" );
#endif
                DSCONbits.RELEASE = 0; /* DS30005009C-page 165 */
            }

            /**
             * Set log file name => 20yymmdd.CSV
             * One log file per day.
             */
            if ( false == setLogFileName( ) )
            {
                appData.state = APP_STATE_ERROR;
                break;
            }

            //#if defined (USE_UART1_SERIAL_INTERFACE)
            //            printf( "%s\n", appDataLog.filename );
            //#endif
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
                appData.reward_door_status = DOOR_CLOSING;
                // TODO: Check door position or wainting for timeout closing
                while ( DOOR_IDLE != appData.reward_door_status );
                /* Servomotor power command disable. */
                servomotorPowerDisable( );

                setLedsStatusColor( LEDS_OFF );
                appData.state = APP_STATE_IDLE;
            }
            else
            {
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf( "System not initialized\n" );
#endif
                sprintf( appError.message, "Unable to initialize the system" );
                appError.currentLineNumber = __LINE__;
                sprintf( appError.currentFileName, "%s", __FILE__ );
                appData.state = APP_STATE_ERROR;
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
                setDelayMs( 30000 ); /* waiting 30 sec timeout before going to sleep mode */
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

            // TODO: Update here appData.flags.bit_value.attractive_leds_status
            // from 
            // appDataAttractiveLeds.wake_up_time.tm_hour
            // appDataAttractiveLeds.wake_up_time.tm_min
            // and
            // appDataAttractiveLeds.sleep_time.tm_hour
            // appDataAttractiveLeds.sleep_time.tm_min


            if ( appData.openfeeder_state == OPENFEEDER_IS_AWAKEN && appData.rtcc_alarm_action == GO_TO_SLEEP )
            {
                appData.state = APP_STATE_GO_TO_SLEEP_MODE;
                break;
            }

            if ( appData.openfeeder_state == OPENFEEDER_IS_SLEEPING && appData.rtcc_alarm_action == WAKE_UP )
            {
                appData.state = APP_STATE_WAKE_UP_FROM_SLEEP;
                break;
            }

            /* Check TIMEOUT IDLE MODE endding.
             *  - if false go to APP_STATE_SLEEP
             */
            if ( false == appData.flags.bit_value.attractive_leds_status )
            {
                if ( isDelayMsEnding( ) )
                {
                    appData.state = APP_STATE_SLEEP;
                }
            }
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
            }

            //#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
            //            printf("APP_STATE_SLEEP\n");
            //#endif
            //            appData.previous_state = APP_STATE_SLEEP;
            setLedsStatusColor( LEDS_OFF );

            /* Sequence to enter Deep Sleep mode. */
            //            _DSEN = 1; /* Deep Sleep */
            //            Nop( );
            //            Nop( );
            //            Nop( );
            //            _DSEN = 1; /* Deep Sleep */
            //            Sleep( );

            //            asm("BSET DSCON, #DSEN"); // not work !
            //            asm("mov #8000, w2"); // enable DS
            //            asm("mov w2, DSCON");
            //            asm("mov w2, DSCON");
            Sleep( );

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
            printf( "\n" );
#endif

            //appData.state = APP_STATE_IDLE;
            appData.state = APP_STATE_WAKE_UP_FROM_SLEEP;
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

                if ( appDataPitTag.numPitTagDenied > 0 )
                {
                    appDataLog.is_pit_tag_denied = checkPitTagDenied( );
                }
                else
                {
                    appDataLog.is_pit_tag_denied = false;
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
            //++detect_bird_counter; // do in tmr4.c
            if ( g_timeout_reading_pit_tag == 0 )
            {
                RFID_Disable( );
                clear_bird_sensor_detected( );
                sprintf( appDataLog.bird_pit_tag_str, "0" );
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
                /* Servomotor power command enable. */
                servomotorPowerEnable( );
                appData.reward_door_status = DOOR_OPENING;
                //#if defined (USE_UART1_SERIAL_INTERFACE)
                //                printf( "Opening reward door in action.\n" );
                //#endif
                appDataLog.is_reward_taken = false;
                setDoorLedsColor( );
            }

            // TODO: Check door position or wait for timeout closing
            if ( DOOR_IDLE == appData.reward_door_status )
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
                g_timeout_taking_reward = appData.timeout_taking_reward;
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

            /* low --> coupure de la barrière infra rouge */
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

            /* Timeout endding */
            if ( g_timeout_taking_reward == 0 && BAR_IR1_OUT_GetValue( ) == 0 )
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
                /* Servomotor power command enable. */
                servomotorPowerEnable( );
                appData.reward_door_status = DOOR_CLOSING;
#if defined (USE_UART1_SERIAL_INTERFACE) 
                printf( "Closing reward door in action.\n" );
#endif
            }

            /* Enregistrement de l'heure de fin de détection de l'oiseau. */
            // TODO: Get current Time when the RFID signal 
            while ( !RTCC_TimeGet( &appDataLog.bird_quit_time ) )
            {
                Nop( );
            }

            // TODO: Check door position or wainting for timeout closing
            if ( DOOR_IDLE == appData.reward_door_status )
            {
                /* Servomotor power command enable. */
                servomotorPowerDisable( );
                setAttractiveLedsColor( );
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

        case APP_STATE_GO_TO_SLEEP_MODE:
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_GO_TO_SLEEP_MODE\n" );
#endif
            }

            //            EX_INT0_InterruptDisable( ); /* For deep sleep only */

            rtcc_set_alarm( appDataAlarmWakeup.time.tm_hour, appDataAlarmWakeup.time.tm_min, appDataAlarmWakeup.time.tm_sec );

#if defined (USE_UART1_SERIAL_INTERFACE)
            printf( "Go into sleep mode...\n" );
#endif 
            appData.openfeeder_state = OPENFEEDER_IS_SLEEPING;
            appData.state = APP_STATE_SLEEP;
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_WAKE_UP_FROM_SLEEP:
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_WAKE_UP_FROM_SLEEP\n" );
#endif
            }

            rtcc_set_alarm( appDataAlarmSleep.time.tm_hour, appDataAlarmSleep.time.tm_min, appDataAlarmSleep.time.tm_sec );

#if defined (USE_UART1_SERIAL_INTERFACE)
            printf( "Awaken from sleep mode!\n" );
#endif 
            appData.openfeeder_state = OPENFEEDER_IS_AWAKEN;
            //            EX_INT0_InterruptEnable( );
            //            CMD_VDD_APP_V_USB_SetHigh( ); /* Enable the 5 V regulator. */

            // le delais de 10 sec ne change pas l'erreur: ERROR: In (0)
            //            setDelayMs( 10000 );
            //            while ( false == isDelayMsEnding( ) );

            //                        while (  )
            //            {
            //
            //            }

            appData.state = APP_STATE_IDLE;
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

            if ( USBHostDeviceStatus( appDataUsb.deviceAddress ) == USB_DEVICE_DETACHED )
            {
                setLedsStatusColor( LEDS_OFF );
                appData.state = APP_STATE_INIT_MOUNT_USB_KEY;
            }

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
    /* Status LED */
    setLedsStatusColor( LED_RED );

    /* Attractive LEDs initialize */
    setAttractiveLedsOff( );

    /* APP state task initialize */
    appData.state = APP_STATE_INIT;
    appData.previous_state = APP_STATE_ERROR;

    appData.openfeeder_state = OPENFEEDER_IS_AWAKEN;
    appData.rtcc_alarm_action = WAKE_UP;

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

    appData.bird_is_taking_reward = false;

    /* USB host */
    appDataUsb.getValidDeviceAdress = false;
    appDataUsb.key_is_nedded = false;
}


/*******************************************************************************
 End of File
 */

//        case APP_STATE_RFID_MESURING_RDYCLK:
//            if ( appData.state != appData.previous_state )
//            {
//                appData.previous_state = appData.state;
//#if defined (DEBUG_UART) && defined (DISPLAY_CURRENT_STATE)
//                printf( "APP_STATE_RFID_MESURING_RDYCLK\n" );
//#endif
//            }
//            /* New frequency value to display on PC interface
//             * read 1860 à 123.9 kHz
//             */
//            if ( EM4095_SHD_GetValue( ) == true )
//            {
//                //LED_STATUS_R_SetLow( ); // erasing red LED status during frequency measurement of the RDY/CLK signal
//
//                // New frequency value to display on PC interface
//                if ( g_new_value_of_em4095_rdyclk_measurement == true )
//                {
//                    printf( "%u\n", rdyclk_count_in_10ms );
//                    g_new_value_of_em4095_rdyclk_measurement = false;
//                }
//
//                // Checking the validity of the frequency interval (in Hz x100) of RDY/CLK signal for 125 kHz
//                if ( ( rdyclk_count_in_10ms > 1245 ) && ( rdyclk_count_in_10ms < 1255 ) )
//                {
//                    // LED_STATUS_G_SetHigh( ); // light on green status LED if correct frequency
//                    setLedsStatusColor( LED_GREEN );
//                }
//                else
//                {
//                    // LED_STATUS_G_SetLow( ); // light off green status LED if frequency is out of range
//                    setLedsStatusColor( LEDS_OFF );
//                }
//            }
//            else
//            {
//                appData.state = APP_STATE_IDLE;
//                //                /* Only light on red status LED during idle state. */
//                //                setLedsStatusColor( LED_RED );
//            }
//            break;
