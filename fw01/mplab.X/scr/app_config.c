/**
 * @file app_config_ini.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#include "app.h"
#include "app_config.h"


bool config_set( void )
{
    /* Search for the CONFIG.INI file. */
    if ( CONFIG_INI_NOT_FOUND == config_find_ini( ) )
    {
        //#if defined (USE_UART1_SERIAL_INTERFACE)
        strcpy( appError.message, "File not found (CONFIG.INI)" );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        //        printf( "CONFIG.INI - File not found" );
        //#endif 
        return false;
    }

    //#if defined (USE_UART1_SERIAL_INTERFACE)
    //    printf( "CONFIG.INI - Localisation Ok\n" );
    //#endif 

    /* Try to read the CONFIG.INI file. */
    int read_ini_status;
    read_ini_status = config_read_ini( );

    if ( CONFIG_INI_READ_OK != read_ini_status )
    {
#if defined (USE_UART1_SERIAL_INTERFACE)
        config_print( );
#endif
        //#if defined (USE_UART1_SERIAL_INTERFACE)
        sprintf( appError.message, "Wrong parameters in CONFIG.INI (%d)", read_ini_status );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        //        strcpy( appError.message, "CONFIG.INI - Wrong parameters read (%d)" );
        //        printf( "CONFIG.INI - Wrong parameters read" );
        //        config_print( );
        //#endif  
        return false;
    }
    //
    //#if defined (USE_UART1_SERIAL_INTERFACE)
    //    config_print( );
    //#endif

    config_set_parameters( );

    return true;
}


int config_find_ini( void )
{
    FILEIO_SEARCH_RECORD searchRecord;

    return FILEIO_Find( "CONFIG.INI", FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, true );
}


int8_t config_read_ini( void )
{
    int8_t error_id = 0;
    int32_t read_parameter;
    bool pitTagDeniedFoundInConfigIni = false;
    bool pitTagAcceptedFoundInConfigIni = false;

    /* Site identification. */
    // TODO check site ID validity 
    ini_gets( "siteid", "zone", "XXXX", appData.siteid, sizearray( appData.siteid ), "CONFIG.INI" );

    /* Wake up time. */
    read_parameter = ini_getl( "time", "wakeup_hour", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataAlarmWakeup.time.tm_hour = ( int ) read_parameter;
    }
    read_parameter = ini_getl( "time", "wakeup_minute", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataAlarmWakeup.time.tm_min = ( int ) read_parameter;
    }

    appDataAlarmWakeup.time.tm_sec = 0;

    /* Sleep time. */
    read_parameter = ini_getl( "time", "sleep_hour", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataAlarmSleep.time.tm_hour = ( int ) read_parameter;
    }
    read_parameter = ini_getl( "time", "sleep_minute", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataAlarmSleep.time.tm_min = ( int ) read_parameter;
    }

    appDataAlarmSleep.time.tm_sec = 0;

    /* Attractive LEDs Color. */
    /* (fr) Lecture de la couleur des LED d'attraction. */
    read_parameter = ini_getl( "attractive_leds", "red_a", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataAttractiveLeds.red[0] = ( uint8_t ) read_parameter;
    }
    read_parameter = ini_getl( "attractive_leds", "green_a", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataAttractiveLeds.green[0] = ( uint8_t ) read_parameter;
    }
    read_parameter = ini_getl( "attractive_leds", "blue_a", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataAttractiveLeds.blue[0] = ( uint8_t ) read_parameter;
    }
    read_parameter = ini_getl( "attractive_leds", "red_b", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataAttractiveLeds.red[1] = ( uint8_t ) read_parameter;
    }
    read_parameter = ini_getl( "attractive_leds", "green_b", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataAttractiveLeds.green[1] = ( uint8_t ) read_parameter;
    }
    read_parameter = ini_getl( "attractive_leds", "blue_b", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataAttractiveLeds.blue[1] = ( uint8_t ) read_parameter;
    }
    /* Attractive LEDs alternate delay. */
    read_parameter = ini_getl( "attractive_leds", "alt_delay", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataAttractiveLeds.alt_delay = ( uint16_t ) read_parameter * 1000;
    }
    /* Attractive LEDs wake up time. */
    read_parameter = ini_getl( "attractive_leds", "on_hour", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataAttractiveLeds.wake_up_time.tm_hour = ( int ) read_parameter;
    }
    read_parameter = ini_getl( "attractive_leds", "on_minute", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataAttractiveLeds.wake_up_time.tm_min = ( int ) read_parameter;
    }

    appDataAttractiveLeds.wake_up_time.tm_sec = 0;

    /* Attractive LEDs sleep time. */
    read_parameter = ini_getl( "attractive_leds", "off_hour", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataAttractiveLeds.sleep_time.tm_hour = ( int ) read_parameter;
    }
    read_parameter = ini_getl( "attractive_leds", "off_minute", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataAttractiveLeds.sleep_time.tm_min = ( int ) read_parameter;
    }

    appDataAttractiveLeds.sleep_time.tm_sec = 0;

    /* Door/servomotor configuration. */
    read_parameter = ini_getl( "door", "ton_min", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        appDataServo.ton_min = SERVO_POSITION_MIN_DEFAULT;
        return error_id;
    }
    else
    {
        //appDataServo.ton_min = (uint16_t) read_parameter;
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
    --error_id;
    if ( read_parameter == -1 )
    {
        appDataServo.ton_max = SERVO_POSITION_MAX_DEFAULT;
        return error_id;
    }
    else
    {
        //appDataServo.ton_max = ( uint16_t ) read_parameter;
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
    --error_id;
    if ( read_parameter == -1 )
    {
        appDataServo.speed = SERVO_SPEED_INC_DEFAULT;
        return error_id;
    }
    else
    {
        appDataServo.speed = ( uint8_t ) read_parameter;
    }

    /* Door open/closee delays. */
    read_parameter = ini_getl( "door", "opendelay", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataDoor.open_delay = ( uint16_t ) read_parameter * 1000;
    }
    read_parameter = ini_getl( "door", "closedelay", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataDoor.close_delay = ( uint16_t ) read_parameter * 1000;
    }
    /* Door remain open */
    read_parameter = ini_getl( "door", "remainopen", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataDoor.remain_open = ( uint8_t ) read_parameter;
    }
    /* Door open time. */
    read_parameter = ini_getl( "door", "open_hour", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataDoor.open_time.tm_hour = ( int ) read_parameter;
    }
    read_parameter = ini_getl( "door", "open_minute", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataDoor.open_time.tm_min = ( int ) read_parameter;
    }

    appDataDoor.open_time.tm_sec = 0;

    /* Door close time. */
    read_parameter = ini_getl( "door", "close_hour", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataDoor.close_time.tm_hour = ( int ) read_parameter;
    }
    read_parameter = ini_getl( "door", "close_minute", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        return error_id;
    }
    else
    {
        appDataDoor.close_time.tm_min = ( int ) read_parameter;
    }

    appDataDoor.close_time.tm_sec = 0;

    /* PIT Tags denied or associated with color A. */
    int s;
    char name[30];
    for ( s = 0; ini_getsection( s, name, 20, "CONFIG.INI" ) > 0; ++s )
    {
        if ( strcmp( name, "pittagsdenied" ) == 0 )
        {
            pitTagDeniedFoundInConfigIni = true;
        }
    }

    appDataPitTag.numPitTagDeniedOrColorA = 0;
    if ( true == pitTagDeniedFoundInConfigIni )
    {
        int k;
        char name[20];
        for ( k = 0; ini_getkey( "pittagsdenied", k, name, 20, "CONFIG.INI" ) > 0; k++ )
        {
            if ( k == MAX_PIT_TAGS_LIST_NUMBER )
            {
                break;
            }
            ++appDataPitTag.numPitTagDeniedOrColorA;
            ini_gets( "pittagsdenied", name, "XXXXXXXXXX", appDataPitTag.pit_tags_list[k], sizearray( appDataPitTag.pit_tags_list[0] ), "CONFIG.INI" );
            appDataPitTag.isPitTagdeniedOrColorA[k] = true;
        }
    }
    
    /* PIT Tags accepted or associated with color B. */
    for ( s = 0; ini_getsection( s, name, 20, "CONFIG.INI" ) > 0; ++s )
    {
        if ( strcmp( name, "pittagsaccepted" ) == 0 )
        {
            pitTagAcceptedFoundInConfigIni = true;
        }
    }

    appDataPitTag.numPitTagAcceptedOrColorB = 0;
    if ( true == pitTagAcceptedFoundInConfigIni )
    {
        int k;
        char name[20];
        for ( k = 0; ini_getkey( "pittagsaccepted", k, name, 20, "CONFIG.INI" ) > 0; k++ )
        {
            if ( k == MAX_PIT_TAGS_LIST_NUMBER )
            {
                break;
            }
            ++appDataPitTag.numPitTagAcceptedOrColorB;
            ini_gets( "pittagsaccepted", name, "XXXXXXXXXX", appDataPitTag.pit_tags_list[k+appDataPitTag.numPitTagDeniedOrColorA], sizearray( appDataPitTag.pit_tags_list[0] ), "CONFIG.INI" );
            appDataPitTag.isPitTagdeniedOrColorA[k+appDataPitTag.numPitTagDeniedOrColorA] = false;
        }
    }

    /* Data separator in the log file. */
    ini_gets( "logfile", "separator", DEFAULT_LOG_SEPARATOR, appDataLog.separator, sizearray( appDataLog.separator ), "CONFIG.INI" );

    /* Timeout before sleep. */
    read_parameter = ini_getl( "timeouts", "sleep", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        appData.timeout_standby = STANDBY_TIMEOUT_X1000MS_DEFAULT * 1000;
        return error_id;
    }
    else
    {
        appData.timeout_standby = ( uint16_t ) read_parameter * 1000;
    }
    /* Timeout before pir. */
    read_parameter = ini_getl( "timeouts", "pir", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        appData.timeout_pir = PIR_TIMEOUT_X1000MS_DEFAULT * 1000;
        return error_id;
    }
    else
    {
        appData.timeout_pir = ( uint16_t ) read_parameter * 1000;
    }
    /* Timeout taking reward. */
    read_parameter = ini_getl( "timeouts", "reward", -1, "CONFIG.INI" );
    --error_id;
    if ( read_parameter == -1 )
    {
        appData.timeout_taking_reward = TAKING_REWARD_TIMEOUT_X1000MS_DEFAULT * 1000;
        return error_id;
    }
    else
    {
        appData.timeout_taking_reward = ( uint16_t ) read_parameter * 1000;
    }

    //    /* Door LEDs Color. */
    //    read_parameter = ini_getl( "doorledscolor", "red", -1, "CONFIG.INI" );
    //    --error_id;
    //    if ( read_parameter == -1 )
    //    {
    //        return error_id;
    //    }
    //    else
    //    {
    //        appData.open_door_red = ( uint8_t ) read_parameter;
    //    }
    //    read_parameter = ini_getl( "doorledscolor", "green", -1, "CONFIG.INI" );
    //    --error_id;
    //    if ( read_parameter == -1 )
    //    {
    //        return error_id;
    //    }
    //    else
    //    {
    //        appData.open_door_green = ( uint8_t ) read_parameter;
    //    }
    //    read_parameter = ini_getl( "doorledscolor", "blue", -1, "CONFIG.INI" );
    //    --error_id;
    //    if ( read_parameter == -1 )
    //    {
    //        return error_id;
    //    }
    //    else
    //    {
    //        appData.open_door_blue = ( uint8_t ) read_parameter;
    //    }
    return CONFIG_INI_READ_OK;
}


void config_set_parameters( void )
{
    //#if defined (USE_UART1_SERIAL_INTERFACE)
    //    printf( "Configuration settings:\n" );
    //#endif 

    /* Set RTCC alarm */
    //    rtcc_set_alarm( appDataAlarmWakeup.time.tm_hour, appDataAlarmWakeup.time.tm_min, appDataAlarmWakeup.time.tm_sec, EVERY_DAY );
    rtcc_set_alarm( appDataAlarmWakeup.time.tm_hour, appDataAlarmWakeup.time.tm_min, appDataAlarmWakeup.time.tm_sec, EVERY_SECOND );
    //#if defined (USE_UART1_SERIAL_INTERFACE)
    //    printf( "\tAlarm: set\n" );
    //#endif 

    /* Set attractive LEDs color */
    //    setAttractiveLedsOn( );
    //#if defined (USE_UART1_SERIAL_INTERFACE)
    //    printf( "\tAttractive LEDs color: set\n" );
    //#endif 

    /* Servomotor configuration. */
    // No need to affect
}


void config_print( void )
{
    int i; /* For loop indice */

    printf( "Configuration parameters:\n" );

    printf( "\tSite ID\n\t\tzone: %s\n",
            appData.siteid );

    printf( "\tTime\n" );
    printf( "\t\tWake up: %02d:%02d\n",
            appDataAlarmWakeup.time.tm_hour,
            appDataAlarmWakeup.time.tm_min );
    printf( "\t\tSleep: %02d:%02d\n",
            appDataAlarmSleep.time.tm_hour,
            appDataAlarmSleep.time.tm_min );

    printf( "\tAttractive LEDs\n" );
    printf( "\t\tRGB(%d, %d, %d)\n",
            appDataAttractiveLeds.red[0],
            appDataAttractiveLeds.green[0],
            appDataAttractiveLeds.blue[0] );
    printf( "\t\tRGB(%d, %d, %d)\n",
            appDataAttractiveLeds.red[1],
            appDataAttractiveLeds.green[1],
            appDataAttractiveLeds.blue[1] );
    printf( "\t\tAlt. delay: %us\n", appDataAttractiveLeds.alt_delay / 1000 );
    printf( "\t\tOn time: %02d:%02d\n",
            appDataAttractiveLeds.wake_up_time.tm_hour,
            appDataAttractiveLeds.wake_up_time.tm_min );
    printf( "\t\tOff time: %02d:%02d\n",
            appDataAttractiveLeds.sleep_time.tm_hour,
            appDataAttractiveLeds.sleep_time.tm_min );

    printf( "\tDoor\n" );
    printf( "\t\tServomotor position min: %d\n", appDataServo.ton_min );
    printf( "\t\tServomotor position max: %d\n", appDataServo.ton_max );
    printf( "\t\tServomotor increment position: %d\n", appDataServo.speed ); /* Increment pace of the servomotor position. */
    printf( "\t\tOpen delay: %ds\n\t\tClose dealy: %ds\n",
            appDataDoor.open_delay / 1000,
            appDataDoor.close_delay / 1000 );
    printf( "\t\tRemain open: %u\n", appDataDoor.remain_open );
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

    //    printf( "\tDoor LEDs\n" );
    //    printf( "\t\tRGB(%d, %d, %d)\n",
    //            appData.open_door_red,
    //            appData.open_door_green,
    //            appData.open_door_blue );

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
            printf( "\t\tSN%d: %s\n", i + 1, appDataPitTag.pit_tags_list[i] );
        }
    }
    else
    {
        printf( "\t\tNone\n" );
    }

    printf( "\tData Logger file\n" ); /* data_logger */
    printf( "\t\tdata separator (CSV file): '%s'\n", appDataLog.separator );
}


/*******************************************************************************
 End of File
 */
