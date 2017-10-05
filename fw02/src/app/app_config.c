/**
 * @file app_config.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#include "app.h"
#include "app_config.h"


bool config_set( void )
{

    INI_READ_STATE read_ini_status;
    char buf[50];

    /* Search for the CONFIG.INI file. */
    if ( FILEIO_RESULT_FAILURE == config_find_ini( ) )
    {
        strcpy( appError.message, "File not found (CONFIG.INI)" );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        return false;
    }

    /* Read the CONFIG.INI file. */
    read_ini_status = config_read_ini( );

    if ( INI_READ_OK != read_ini_status )
    {
        getIniPbChar( read_ini_status, buf );

        sprintf( appError.message, "Wrong parameters in CONFIG.INI: %s (%d)", buf, read_ini_status );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        appError.number = ERROR_INI_FILE_READ;
        return false;
    }

    if ( appDataPitTag.numPitTagDeniedOrColorA > 0 || appDataPitTag.numPitTagAcceptedOrColorB > 0 )
    {
        if ( FILEIO_RESULT_FAILURE == read_PIT_tags( ) )
        {
            return false;
        }
    }

    return true;
}


FILEIO_RESULT read_PIT_tags( void )
{

    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    uint16_t i;

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf( "Read PIT tags files\n" );
#endif 

    if ( appDataPitTag.numPitTagDeniedOrColorA > 0 )
    {

        if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, "PTDENIED.TXT", FILEIO_OPEN_READ ) )
        {
            errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
            printf( "unable to open PIT tags denied file (%u)", errF );
#endif 
            sprintf( appError.message, "Unable to open PIT tags denied file (%u)", errF );
            appError.currentLineNumber = __LINE__;
            sprintf( appError.currentFileName, "%s", __FILE__ );
            FILEIO_ErrorClear( 'A' );
            appError.number = ERROR_PIT_TAGS_DENIED_FILE_OPEN;
            return FILEIO_RESULT_FAILURE;
        }

        for ( i = 0; i < appDataPitTag.numPitTagDeniedOrColorA; i++ )
        {
            FILEIO_Read( appDataPitTag.pit_tags_list[i], 1, 10, &file );
            appDataPitTag.pit_tags_list[i][11] = '\0';
        }

        if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
        {
            errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
            printf( "unable to close PIT tags denied file (%u)", errF );
#endif 
            sprintf( appError.message, "Unable to close PIT tags denied file (%u)", errF );
            appError.currentLineNumber = __LINE__;
            sprintf( appError.currentFileName, "%s", __FILE__ );
            FILEIO_ErrorClear( 'A' );
            appError.number = ERROR_PIT_TAGS_DENIED_FILE_CLOSE;
            return FILEIO_RESULT_FAILURE;
        }

    }

    if ( appDataPitTag.numPitTagAcceptedOrColorB > 0 )
    {

        if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, "PTACCEPT.TXT", FILEIO_OPEN_READ ) )
        {
            errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
            printf( "unable to open PIT tags accepted file (%u)", errF );
#endif 
            sprintf( appError.message, "Unable to open PIT tags accepted file (%u)", errF );
            appError.currentLineNumber = __LINE__;
            sprintf( appError.currentFileName, "%s", __FILE__ );
            FILEIO_ErrorClear( 'A' );
            appError.number = ERROR_PIT_TAGS_ACCEPTED_FILE_OPEN;
            return FILEIO_RESULT_FAILURE;
        }

        for ( i = 0; i < appDataPitTag.numPitTagAcceptedOrColorB; i++ )
        {
            FILEIO_Read( appDataPitTag.pit_tags_list[i + appDataPitTag.numPitTagDeniedOrColorA], 1, 10, &file );
            appDataPitTag.pit_tags_list[i + appDataPitTag.numPitTagDeniedOrColorA][11] = '\0';
        }

        if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
        {
            errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
            printf( "unable to close PIT tags denied file (%u)", errF );
#endif 
            sprintf( appError.message, "Unable to close PIT tags accepted file (%u)", errF );
            appError.currentLineNumber = __LINE__;
            sprintf( appError.currentFileName, "%s", __FILE__ );
            FILEIO_ErrorClear( 'A' );
            appError.number = ERROR_PIT_TAGS_ACCEPTED_FILE_CLOSE;
            return FILEIO_RESULT_FAILURE;
        }

    }

    return FILEIO_RESULT_SUCCESS;

}


FILEIO_RESULT config_find_ini( void )
{

    FILEIO_SEARCH_RECORD searchRecord;

    return FILEIO_Find( "CONFIG.INI", FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, true );
}


INI_READ_STATE config_read_ini( void )
{

    int32_t read_parameter;
    int s;
    char str[20];
    
    /* Scenario number */
    read_parameter = ini_getl( "scenario", "num", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_SCENARIO_NUM;
    }
    else
    {
        appData.scenario_number = ( uint8_t ) read_parameter;
    }

    /* Site identification. */
    ini_gets( "siteid", "zone", "XXXX", appData.siteid, sizearray( appData.siteid ), "CONFIG.INI" );

    /* Wake up time. */
    read_parameter = ini_getl( "time", "wakeup_hour", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_TIME_WAKEUP_HOUR;
    }
    else
    {
        appDataAlarmWakeup.time.tm_hour = ( int ) read_parameter;
    }
    read_parameter = ini_getl( "time", "wakeup_minute", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_TIME_WAKEUP_MINUTE;
    }
    else
    {
        appDataAlarmWakeup.time.tm_min = ( int ) read_parameter;
    }
    appDataAlarmWakeup.time.tm_sec = 0;

    /* Sleep time. */
    read_parameter = ini_getl( "time", "sleep_hour", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_TIME_SLEEP_HOUR;
    }
    else
    {
        appDataAlarmSleep.time.tm_hour = ( int ) read_parameter;
    }
    read_parameter = ini_getl( "time", "sleep_minute", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_TIME_SLEEP_MINUTE;
    }
    else
    {
        appDataAlarmSleep.time.tm_min = ( int ) read_parameter;
    }
    appDataAlarmSleep.time.tm_sec = 0;

    /* Attractive LEDs Color. */
    appData.flags.bit_value.attractive_leds_status = 0;
    /* Check if "attractiveleds" is present in the INI file */ 
    for (s = 0; ini_getsection(s, str, 20, "CONFIG.INI") > 0; s++)
    {
        if ( 0 == strcmp( str, "attractiveleds" ) )
        {
            appData.flags.bit_value.attractive_leds_status = true;
        }
    }
        
    if (true == appData.flags.bit_value.attractive_leds_status)
    {
        read_parameter = ini_getl( "attractiveleds", "red_a", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_ATTRACTIVE_LEDS_RED_A;
        }
        else
        {
            appDataAttractiveLeds.red[0] = ( uint8_t ) read_parameter;
        }
        read_parameter = ini_getl( "attractiveleds", "green_a", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_ATTRACTIVE_LEDS_GREEN_A;
        }
        else
        {
            appDataAttractiveLeds.green[0] = ( uint8_t ) read_parameter;
        }
        read_parameter = ini_getl( "attractiveleds", "blue_a", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_ATTRACTIVE_LEDS_BLUE_A;
        }
        else
        {
            appDataAttractiveLeds.blue[0] = ( uint8_t ) read_parameter;
        }
        read_parameter = ini_getl( "attractiveleds", "red_b", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_ATTRACTIVE_LEDS_RED_B;
        }
        else
        {
            appDataAttractiveLeds.red[1] = ( uint8_t ) read_parameter;
        }
        read_parameter = ini_getl( "attractiveleds", "green_b", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_ATTRACTIVE_LEDS_GREEN_B;
        }
        else
        {
            appDataAttractiveLeds.green[1] = ( uint8_t ) read_parameter;
        }
        read_parameter = ini_getl( "attractiveleds", "blue_b", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_ATTRACTIVE_LEDS_BLUE_B;
        }
        else
        {
            appDataAttractiveLeds.blue[1] = ( uint8_t ) read_parameter;
        }
        /* Attractive LEDs alternate delay. */
        read_parameter = ini_getl( "attractiveleds", "alt_delay", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_ATTRACTIVE_LEDS_ALT_DELAY;
        }
        else
        {
            appDataAttractiveLeds.alt_delay = ( uint8_t ) read_parameter;
        }
        /* Attractive LEDs wake up time. */
        read_parameter = ini_getl( "attractiveleds", "on_hour", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_ATTRACTIVE_LEDS_ON_HOUR;
        }
        else
        {
            appDataAttractiveLeds.wake_up_time.tm_hour = ( int ) read_parameter;
        }
        read_parameter = ini_getl( "attractiveleds", "on_minute", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_ATTRACTIVE_LEDS_ON_MINUTE;
        }
        else
        {
            appDataAttractiveLeds.wake_up_time.tm_min = ( int ) read_parameter;
        }

        appDataAttractiveLeds.wake_up_time.tm_sec = 0;

        /* Attractive LEDs sleep time. */
        read_parameter = ini_getl( "attractiveleds", "off_hour", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_ATTRACTIVE_LEDS_OFF_HOUR;
        }
        else
        {
            appDataAttractiveLeds.sleep_time.tm_hour = ( int ) read_parameter;
        }
        read_parameter = ini_getl( "attractiveleds", "off_minute", -1, "CONFIG.INI" );
        if ( read_parameter == -1 )
        {
            return INI_PB_ATTRACTIVE_LEDS_OFF_MINUTE;
        }
        else
        {
            appDataAttractiveLeds.sleep_time.tm_min = ( int ) read_parameter;
        }
        appDataAttractiveLeds.sleep_time.tm_sec = 0;
    }

    /* PIT Tags denied or associated with color A. */
    read_parameter = ini_getl( "pittagsdenied", "num", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_PIT_TAG_DENIED;
    }
    else
    {
        appDataPitTag.numPitTagDeniedOrColorA = ( uint16_t ) read_parameter;
    }
    /* PIT Tags accepted or associated with color B. */
    read_parameter = ini_getl( "pittagsaccepted", "num", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_PIT_TAG_ACCEPTED;
    }
    else
    {
        appDataPitTag.numPitTagAcceptedOrColorB = ( uint16_t ) read_parameter;
    }

    /* Door/servomotor configuration. */
    read_parameter = ini_getl( "door", "ton_min", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_TON_MIN;
    }
    else
    {
        if ( read_parameter >= SERVO_POSITION_MIN_DEFAULT )
        {
            appDataServo.ton_min = ( uint16_t ) read_parameter;
        }
        else
        {
            appDataServo.ton_min = SERVO_POSITION_MIN_DEFAULT;
        }
    }

    read_parameter = ini_getl( "door", "ton_max", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_TON_MAX;
    }
    else
    {
        if ( read_parameter <= SERVO_POSITION_MAX_DEFAULT )
        {
            appDataServo.ton_max = ( uint16_t ) read_parameter;
        }
        else
        {
            appDataServo.ton_max = SERVO_POSITION_MAX_DEFAULT;
        }
    }

    read_parameter = ini_getl( "door", "speed", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_SPEED;
    }
    else
    {
        appDataServo.speed = ( uint8_t ) read_parameter;
    }
    /* Door open/closee delays. */
    read_parameter = ini_getl( "door", "open_delay", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_OPEN_DELAY;
    }
    else
    {
        appDataDoor.open_delay = ( uint16_t ) read_parameter * 1000;
    }
    read_parameter = ini_getl( "door", "close_delay", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_CLOSE_DELAY;
    }
    else
    {
        appDataDoor.close_delay = ( uint16_t ) read_parameter * 1000;
    }
    /* Door habituation */
    if (DOOR_HABITUATION == appData.scenario_number)
    {
        read_parameter = ini_getl( "door", "habituation", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_DOOR_CLOSE_DELAY;
        }
        else
        {
            appDataDoor.habituation_percent = ( uint8_t ) read_parameter;
            appDataServo.ton_max = (appDataServo.ton_max-appDataServo.ton_min)/100*appDataDoor.habituation_percent+appDataServo.ton_min;
        }
    }
    /* Door remain open */
    read_parameter = ini_getl( "door", "remain_open", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_REMAIN_OPEN;
    }
    else
    {
        appDataDoor.remain_open = ( uint8_t ) read_parameter;
    }
    /* Door reward probability */
//    read_parameter = ini_getf( "door", "reward_probability", -1, "CONFIG.INI" );
    ini_gets( "door", "reward_probability", "1.0", str, sizearray( str ), "CONFIG.INI" );
    appDataDoor.reward_probability = atof(str);

    /* Door open time. */
    read_parameter = ini_getl( "door", "open_hour", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_OPEN_HOUR;
    }
    else
    {
        appDataDoor.open_time.tm_hour = ( int ) read_parameter;
    }
    read_parameter = ini_getl( "door", "open_minute", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_OPEN_MINUTE;
    }
    else
    {
        appDataDoor.open_time.tm_min = ( int ) read_parameter;
    }
    appDataDoor.open_time.tm_sec = 0;

    /* Door close time. */
    read_parameter = ini_getl( "door", "close_hour", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_CLOSE_HOUR;
    }
    else
    {
        appDataDoor.close_time.tm_hour = ( int ) read_parameter;
    }
    read_parameter = ini_getl( "door", "close_minute", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_CLOSE_MINUTE;
    }
    else
    {
        appDataDoor.close_time.tm_min = ( int ) read_parameter;
    }
    appDataDoor.close_time.tm_sec = 0;

    /* Data separator in the log file. */
    ini_gets( "logfile", "separator", DEFAULT_LOG_SEPARATOR, appDataLog.separator, sizearray( appDataLog.separator ), "CONFIG.INI" );

    /* Timeout before standby. */
    read_parameter = ini_getl( "timeouts", "sleep", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_TIMEOUTS_SLEEP;
    }
    else
    {
        appData.timeout_standby = ( uint16_t ) read_parameter * 1000;
    }
    /* Timeout before pir. */
    read_parameter = ini_getl( "timeouts", "pir", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_TIMEOUTS_PIR;
    }
    else
    {
        appData.timeout_pir = ( uint16_t ) read_parameter * 1000;
    }
    /* Timeout taking reward. */
    read_parameter = ini_getl( "timeouts", "reward", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_TIMEOUTS_REWARD;
    }
    else
    {
        appData.timeout_taking_reward = ( uint16_t ) read_parameter * 1000;
    }
    /* Punishment delay. */
    read_parameter = ini_getl( "punishment", "delay", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_PUNISHMENT_DELAY;
    }
    else
    {
        appData.punishment_delay = ( uint16_t ) read_parameter * 1000;
    }

    return INI_READ_OK;
}


void config_print( void )
{
    int i;

    printf( "Configuration parameters\n" );

    printf( "\tScenario\n\t\tNumber: %u\n\t\tTitle:",
            appData.scenario_number );
    switch ( appData.scenario_number )
    {
        case NO_SCENARIO:
            printf( " no scenario\n" );
            break;
        case OPEN_BAR:
            printf( " open-bar\n" );
            break;
        case LONG_TERM_SPATIAL_MEMORY:
            printf( " long term spatial memory\n" );
            break;
        case WORKING_SPATIAL_MEMORY:
            printf( " working spatial memory\n" );
            break;
        case COLOR_ASSOCIATIVE_LEARNING:
            printf( " color associative learning\n" );
            break;
        case DOOR_HABITUATION:
            printf( " door habituation\n" );
            break;
    }

    printf( "\tSite ID\n\t\tZone: %s\n",
            appData.siteid );

    printf( "\tTime\n" );
    printf( "\t\tWake up: %02d:%02d\n",
            appDataAlarmWakeup.time.tm_hour,
            appDataAlarmWakeup.time.tm_min );
    printf( "\t\tSleep: %02d:%02d\n",
            appDataAlarmSleep.time.tm_hour,
            appDataAlarmSleep.time.tm_min );

    if (true == appData.flags.bit_value.attractive_leds_status)
    {
        printf( "\tAttractive LEDs\n" );
        printf( "\t\tColor A: RGB(%d, %d, %d)\n",
                appDataAttractiveLeds.red[0],
                appDataAttractiveLeds.green[0],
                appDataAttractiveLeds.blue[0] );
        printf( "\t\tColor B: RGB(%d, %d, %d)\n",
                appDataAttractiveLeds.red[1],
                appDataAttractiveLeds.green[1],
                appDataAttractiveLeds.blue[1] );
        printf( "\t\tAlternate delay: %us\n", appDataAttractiveLeds.alt_delay );
        printf( "\t\tOn time: %02d:%02d\n",
                appDataAttractiveLeds.wake_up_time.tm_hour,
                appDataAttractiveLeds.wake_up_time.tm_min );
        printf( "\t\tOff time: %02d:%02d\n",
                appDataAttractiveLeds.sleep_time.tm_hour,
                appDataAttractiveLeds.sleep_time.tm_min );
    }

    printf( "\tDoor\n" );
    printf( "\t\tServomotor position min: %d\n", appDataServo.ton_min );
    
    if (DOOR_HABITUATION != appData.scenario_number)
    {           
        printf( "\t\tServomotor position max: %d\n", appDataServo.ton_max );
    }
    else
    {
        printf( "\t\tServomotor position max: %d (%d%% opened)\n", appDataServo.ton_max, appDataDoor.habituation_percent );
    }
    printf( "\t\tServomotor increment position: %d\n", appDataServo.speed ); /* Increment pace of the servomotor position. */
    printf( "\t\tOpen delay: %ds\n\t\tClose delay: %ds\n",
            appDataDoor.open_delay / 1000,
            appDataDoor.close_delay / 1000 );
    if ( 0 == appDataDoor.remain_open )
    {
        printf( "\t\tRemain open: no\n" );
    }
    else
    {
        printf( "\t\tRemain open: yes\n" );
    }
    printf( "\t\tOpen time: %02d:%02d\n",
            appDataDoor.open_time.tm_hour,
            appDataDoor.open_time.tm_min );
    printf( "\t\tClose time: %02d:%02d\n",
            appDataDoor.close_time.tm_hour,
            appDataDoor.close_time.tm_min );

    printf( "\tTimeouts\n" );
    printf( "\t\tSleep: %us\n", appData.timeout_standby / 1000 );
    printf( "\t\tPIR: %us\n", appData.timeout_pir / 1000 );
    printf( "\t\tTaking reward: %us\n", appData.timeout_taking_reward / 1000 );
    
    if (true == appData.flags.bit_value.attractive_leds_status)
    {
        printf( "\tPunishment\n" );
        printf( "\t\tDelay: %us\n", appData.punishment_delay / 1000 );
    }

    printf( "\tData logger\n" );
    printf( "\t\tFile name: %s\n", appDataLog.filename );
    printf( "\t\tData separator: %s\n", appDataLog.separator );

    printf( "\tPIT Tags denied or associated with color A\n" );
    if ( appDataPitTag.numPitTagDeniedOrColorA > 0 )
    {
        for ( i = 0; i < appDataPitTag.numPitTagDeniedOrColorA; ++i )
        {
            printf( "\t\tSN%d: %s\n", i + 1, appDataPitTag.pit_tags_list[i] );
        }
    }
    else
    {
        printf( "\t\tNone\n" );
    }
    printf( "\tPIT Tags accepted or associated with color B\n" );
    if ( appDataPitTag.numPitTagAcceptedOrColorB > 0 )
    {
        for ( i = appDataPitTag.numPitTagDeniedOrColorA; i < ( appDataPitTag.numPitTagDeniedOrColorA + appDataPitTag.numPitTagAcceptedOrColorB ); ++i )
        {
            printf( "\t\tSN%d: %s\n", i + 1 - appDataPitTag.numPitTagDeniedOrColorA, appDataPitTag.pit_tags_list[i] );
        }
    }
    else
    {
        printf( "\t\tNone\n" );
    }

}


void getIniPbChar( INI_READ_STATE state, char buf[] )
{

    switch ( state )
    {

        case INI_PB_SCENARIO_NUM:
            snprintf( buf, sizearray( buf ), "Scenario: number" );
            break;
        case INI_PB_SITEID_ZONE:
            snprintf( buf, sizearray( buf ), "Site ID: zone" );
            break;
        case INI_PB_TIME_WAKEUP_HOUR:
            snprintf( buf, sizearray( buf ), "Wake-up: hour" );
            break;
        case INI_PB_TIME_WAKEUP_MINUTE:
            snprintf( buf, sizearray( buf ), "Wake-up: minute" );
            break;
        case INI_PB_TIME_SLEEP_HOUR:
            snprintf( buf, sizearray( buf ), "Sleep: hour" );
            break;
        case INI_PB_TIME_SLEEP_MINUTE:
            snprintf( buf, sizearray( buf ), "Sleep: minute" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_RED_A:
            snprintf( buf, sizearray( buf ), "Attractive LEDs: red A" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_GREEN_A:
            snprintf( buf, sizearray( buf ), "Attractive LEDs: green A" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_BLUE_A:
            snprintf( buf, sizearray( buf ), "Attractive LEDs: blue A" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_RED_B:
            snprintf( buf, sizearray( buf ), "Attractive LEDs: red B" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_GREEN_B:
            snprintf( buf, sizearray( buf ), "Attractive LEDs: green B" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_BLUE_B:
            snprintf( buf, sizearray( buf ), "Attractive LEDs: blue B" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_ALT_DELAY:
            snprintf( buf, sizearray( buf ), "Attractive LEDs: alternate delay" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_ON_HOUR:
            snprintf( buf, sizearray( buf ), "Attractive LEDs: off minute" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_ON_MINUTE:
            snprintf( buf, sizearray( buf ), "Attractive LEDs: on minute" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_OFF_HOUR:
            snprintf( buf, sizearray( buf ), "Attractive LEDs: off hour" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_OFF_MINUTE:
            snprintf( buf, sizearray( buf ), "Attractive LEDs: off minute" );
            break;
        case INI_PB_DOOR_TON_MIN:
            snprintf( buf, sizearray( buf ), "Door: position min" );
            break;
        case INI_PB_DOOR_TON_MAX:
            snprintf( buf, sizearray( buf ), "Door: position max" );
            break;
        case INI_PB_DOOR_SPEED:
            snprintf( buf, sizearray( buf ), "Door: speed" );
            break;
        case INI_PB_DOOR_OPEN_DELAY:
            snprintf( buf, sizearray( buf ), "Door: open delay" );
            break;
        case INI_PB_DOOR_CLOSE_DELAY:
            snprintf( buf, sizearray( buf ), "Door: close delay" );
            break;
        case INI_PB_DOOR_REMAIN_OPEN:
            snprintf( buf, sizearray( buf ), "Door: remain open" );
            break;
        case INI_PB_DOOR_OPEN_HOUR:
            snprintf( buf, sizearray( buf ), "Door: open hour" );
            break;
        case INI_PB_DOOR_OPEN_MINUTE:
            snprintf( buf, sizearray( buf ), "Door: open minute" );
            break;
        case INI_PB_DOOR_CLOSE_HOUR:
            snprintf( buf, sizearray( buf ), "Door: close hour" );
            break;
        case INI_PB_DOOR_CLOSE_MINUTE:
            snprintf( buf, sizearray( buf ), "Door: close minute" );
            break;
        case INI_PB_TIMEOUTS_SLEEP:
            snprintf( buf, sizearray( buf ), "Timeouts: sleep" );
            break;
        case INI_PB_TIMEOUTS_PIR:
            snprintf( buf, sizearray( buf ), "Timeouts: pir" );
            break;
        case INI_PB_TIMEOUTS_REWARD:
            snprintf( buf, sizearray( buf ), "Timeouts: reward" );
            break;
        case INI_PB_PUNISHMENT_DELAY:
            snprintf( buf, sizearray( buf ), "New bird: delay" );
            break;
        default:
            snprintf( buf, sizearray( buf ), "Error not listed" );
            break;
    }


}

/*******************************************************************************
 End of File
 */
