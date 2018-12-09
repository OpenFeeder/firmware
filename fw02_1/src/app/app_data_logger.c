/**
 * @file app_data_logger.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 08/09/2016
 */

#include "app.h"
#include "app_data_logger.h"

#include  "ctype.h"

// *****************************************************************************
// PopulateBuffer - Ajout de donnees dans le buffer
// *****************************************************************************


static bool populateLogBuffer( void )
{
    int nChar;
    char line[MAX_CHAR_PER_LINE];
    unsigned long delayS;

    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
        store_event( OF_POPULATE_LOG_BUFFER );
    }

    if ( 0 == strcmp( appDataLog.bird_pit_tag_str, "XXXXXXXXXX" ) )
    {
        appDataLog.is_reward_taken = false;
        appDataLog.did_door_open = false;
    }

    if ( true == appDataLog.is_pit_tag_denied )
    {
        appDataLog.is_reward_taken = false;
        appDataLog.did_door_open = false;
    }

    /* Durée stationnement oiseau en secondes */
    if ( true == appDataLog.is_reward_taken )
    {
        delayS = ( appDataLog.bird_quit_time.tm_hour - appDataLog.bird_arrived_time.tm_hour )*60 * 60 +
            ( appDataLog.bird_quit_time.tm_min - appDataLog.bird_arrived_time.tm_min )*60 +
            appDataLog.bird_quit_time.tm_sec - appDataLog.bird_arrived_time.tm_sec;
    }
    else
    {
        delayS = 0;
    }

    nChar = sprintf( line, "%02d/%02d/%02d%s%02d:%02d:%02d%s"
                    /* siteid               */ "%c%c"
                    /* separator            */ "%s"
                    /* OF                   */ "OF%c%c"
                    /* separator            */ "%s"
                    /* scenario_number      */ "%u"
                    /* separator            */ "%s"
                    /* bird_pit_tag_str     */ "%s"
                    /* separator            */ "%s"
                    /* is_pit_tag_denied    */ "%u"
                    /* separator            */ "%s"
                    /* is_reward_taken      */ "%u"
                    /* separator            */ "%s"
                    /* attractive LED red   */ "%d"
                    /* separator            */ "%s"
                    /* attractive LED green */ "%d"
                    /* separator            */ "%s"
                    /* attractive LED blue           */ "%d"
                    /* separator                     */ "%s"
                    /* door_status_when_bird_arrived */ "%d"
                    /* separator                     */ "%s"
                    /* did_door_open                 */ "%d"
                    /* separator                     */ "%s"
                    /* delayS                        */ "%lu\n",
                    appDataLog.bird_arrived_time.tm_mday,
                    appDataLog.bird_arrived_time.tm_mon,
                    appDataLog.bird_arrived_time.tm_year,
                    appDataLog.separator,
                    appDataLog.bird_arrived_time.tm_hour,
                    appDataLog.bird_arrived_time.tm_min,
                    appDataLog.bird_arrived_time.tm_sec,
                    appDataLog.separator,
                    appData.siteid[0],
                    appData.siteid[1],
                    appDataLog.separator,
                    appData.siteid[2],
                    appData.siteid[3],
                    appDataLog.separator,
                    getCompletScenarioNumber( ),
                    appDataLog.separator,
                    appDataLog.bird_pit_tag_str,
                    appDataLog.separator,
                    appDataLog.is_pit_tag_denied,
                    appDataLog.separator,
                    appDataLog.is_reward_taken,
                    appDataLog.separator,
                    appDataAttractiveLeds.red[appDataLog.attractive_leds_current_color_index],
                    appDataLog.separator,
                    appDataAttractiveLeds.green[appDataLog.attractive_leds_current_color_index],
                    appDataLog.separator,
                    appDataAttractiveLeds.blue[appDataLog.attractive_leds_current_color_index],
                    appDataLog.separator,
                    appDataLog.door_status_when_bird_arrived,
                    appDataLog.separator,
                    appDataLog.did_door_open,
                    appDataLog.separator,
                    delayS );

    /* If sprintf success*/
    if ( nChar > 0 )
    {
        appDataLog.num_char_buffer += nChar;
        
        /* If data buffer not overflow */
        if ( appDataLog.num_char_buffer < (MAX_CHAR_PER_LINE * MAX_NUM_DATA_TO_STORE - 1 ) )
        {
            /* Concatenation de chaines de caracteres dans le buffer. */
            strcat( appDataLog.buffer, line );
            ++appDataLog.num_data_stored;
        }
        /* If data buffer overflow */
        else
        {
            sprintf( appError.message, "Overflow log buffer" );
            appError.current_line_number = __LINE__;
            sprintf( appError.current_file_name, "%s", __FILE__ );
            appError.number = ERROR_OVERFLOW_DATA_BUFFER;
            
            nChar = -1;
        }
    }
    /* If sprintf failed*/
    else
    {
        sprintf( appError.message, "Unable to populate log buffer" );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        appError.number = ERROR_POPULATE_DATA_BUFFER;
    }

    return nChar > 0;
}

// *****************************************************************************
// LogWrite - Ecriture dans le fichier LOG.CSV
// *****************************************************************************


static int writeLogFile( void )
{
    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    size_t numDataWritten;

    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
        store_event( OF_WRITE_DATA_LOG );
    }

    if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, appDataLog.filename, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_APPEND ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
        sprintf( appError.message, "Unable to open log file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_LOG_FILE_OPEN;
        return FILEIO_RESULT_FAILURE;
    }

    numDataWritten = FILEIO_Write( appDataLog.buffer, 1, appDataLog.num_char_buffer, &file );

    if ( numDataWritten < appDataLog.num_char_buffer )
    {
        errF = FILEIO_ErrorGet( 'A' );
        sprintf( appError.message, "Unable to write data in log file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_LOG_FILE_WRITE;
        return FILEIO_RESULT_FAILURE;
    }

    if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
        sprintf( appError.message, "Unable to close log file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_LOG_FILE_CLOSE;
        return FILEIO_RESULT_FAILURE;
    }

    clearLogBuffer( );

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf( "\tWrite data to log file success\n" );
#endif 

    return FILEIO_RESULT_SUCCESS;
}


bool dataLog( bool newData )
{
//    unsigned int nChar = 0;
    bool needToUnmount;

    /* Check if new data need to be added to the log buffer */
    if ( true == newData )
    {

//        nChar = populateLogBuffer( );
//
//        if ( nChar < 0 )
        if ( false == populateLogBuffer( ) )    
        {
//            sprintf( appError.message, "Unable to populate log buffer" );
//            appError.current_line_number = __LINE__;
//            sprintf( appError.current_file_name, "%s", __FILE__ );
//            appError.number = ERROR_POPULATE_DATA_BUFFER;
            return false;
        }
        else
        {
//            appDataLog.num_char_buffer += nChar;
//            appDataLog.num_data_stored += 1;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO )
            printf( "\tPopulate data to buffer (%u/%u)\n", appDataLog.num_data_stored, MAX_NUM_DATA_TO_STORE );
#endif 
        }

    }

    /* If buffer is full then write log file on the USB device */
    if ( appDataLog.num_data_stored == MAX_NUM_DATA_TO_STORE )
    {
        setLedsStatusColor( LED_USB_ACCESS );

        if ( USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status )
        {
            needToUnmount = false;
        }
        else
        {
            if ( USB_DRIVE_NOT_MOUNTED == usbMountDrive( ) )
            {
                return FILEIO_RESULT_FAILURE;
            }
            needToUnmount = true;
        }

        /* Ecriture fichier LOG. */
        if ( FILEIO_RESULT_FAILURE == writeLogFile( ) )
        {
            //            usbUnmountDrive();
            //            CMD_VDD_APP_V_USB_SetLow( );
            return false;
        }

        appDataLog.num_data_stored = 0;

        if ( true == needToUnmount )
        {
            if ( USB_DRIVE_MOUNTED == usbUnmountDrive( ) )
            {
                return FILEIO_RESULT_FAILURE;
            }
        }
        //        CMD_VDD_APP_V_USB_SetLow( );
        setLedsStatusColor( LEDS_OFF );
    }

    return true;
}


void clearLogBuffer( void )
{
    /* Vidage du buffer. */
    memset( appDataLog.buffer, '\0', sizeof ( appDataLog.buffer ) );
    appDataLog.num_char_buffer = 0;

}


void clearRfidFreqBuffer( void )
{
    appDataLog.num_rfid_freq_stored = 0;
}


void clearExtTemperatureBuffer( void )
{
    appDataLog.num_ds3231_temp_stored = 0;
}


void clearCalibrationBuffer( void )
{
    appDataLog.num_time_calib_stored = 0;
}


void clearBatteryBuffer( void )
{
    appDataLog.num_battery_level_stored = 0;
}


bool setLogFileName( void )
{

    /* Clear filename buffer */
    memset( appDataLog.filename, 0, sizeof (appDataLog.filename ) );

    /* Get current date */
    if ( true == getDateTime( ) )
    {
        /* Set log file name => 20yymmdd.CSV */
        if ( snprintf( appDataLog.filename, 13, "%04d%02d%02d.CSV",
                       2000 + appData.current_time.tm_year,
                       appData.current_time.tm_mon,
                       appData.current_time.tm_mday ) < 0 )
        {
    #if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO )
            printf( "Unable to set log file name\n" );
    #endif 
            sprintf( appError.message, "Unable to set log file name" );
            appError.current_line_number = __LINE__;
            sprintf( appError.current_file_name, "%s", __FILE__ );
            appError.number = ERROR_LOG_FILE_SET_NAME;
            return false;
        }
    }
    else
    {
    #if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO )
            printf( "Unable to set log file name\n" );
    #endif 
        sprintf( appError.message, "Unable to set log file name" );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        appError.number = ERROR_LOG_FILE_SET_NAME;
        return false;
    }

    return true;
}


FILEIO_RESULT logBatteryLevel( void )
{
    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    char buf[35];
    int flag, i;
    size_t numDataWritten;
    bool needToUnmount;

    getDateTime( );

    if ( USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status )
    {
        needToUnmount = false;
    }
    else
    {
        if ( USB_DRIVE_NOT_MOUNTED == usbMountDrive( ) )
        {
            return FILEIO_RESULT_FAILURE;
        }
        needToUnmount = true;
    }

    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
        store_event( OF_WRITE_BATTERY_LOG );
    }

    if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, BATTERY_LOG_FILE, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_APPEND ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
        sprintf( appError.message, "Unable to open battery log file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_BATTERY_FILE_OPEN;
        return FILEIO_RESULT_FAILURE;
    }

    memset( buf, '\0', sizeof ( buf ) );

    for ( i = 0; i < appDataLog.num_battery_level_stored; i++ )
    {
        flag = sprintf( buf, "%c%c%sOF%c%c%s%u%s%02d/%02d/%02d%s%02d:00%s%2.3f\n",
                        appData.siteid[0],
                        appData.siteid[1],
                        appDataLog.separator,
                        appData.siteid[2],
                        appData.siteid[3],
                        appDataLog.separator,
                        getCompletScenarioNumber( ),
                        appDataLog.separator,
                        appData.current_time.tm_mday,
                        appData.current_time.tm_mon,
                        appData.current_time.tm_year,
                        appDataLog.separator,
                        appDataLog.battery_level[i][0],
                        appDataLog.separator,
                        appDataLog.battery_level[i][1] * BATTERY_VOLTAGE_FACTOR );

        if ( flag > 0 )
        {
            numDataWritten = FILEIO_Write( buf, 1, flag, &file );

            if ( numDataWritten < flag )
            {
                errF = FILEIO_ErrorGet( 'A' );
                sprintf( appError.message, "Unable to write battery level in log file (%u)", errF );
                appError.current_line_number = __LINE__;
                sprintf( appError.current_file_name, "%s", __FILE__ );
                FILEIO_ErrorClear( 'A' );
                appError.number = ERROR_BATTERY_FILE_WRITE;
                return FILEIO_RESULT_FAILURE;
            }
        }
    }

    if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
        sprintf( appError.message, "Unable to close battery log file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_BATTERY_FILE_CLOSE;
        return FILEIO_RESULT_FAILURE;
    }

    if ( true == needToUnmount )
    {
        if ( USB_DRIVE_MOUNTED == usbUnmountDrive( ) )
        {
            return FILEIO_RESULT_FAILURE;
        }
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf( "\tWrite battery level to file success\n" );
#endif 

    clearBatteryBuffer( );

    return FILEIO_RESULT_SUCCESS;
}


FILEIO_RESULT logUdid( void )
{
    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    char buf[35];
    size_t numDataWritten;
    int flag;
    bool needToUnmount;

    if ( USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status )
    {
        needToUnmount = false;
    }
    else
    {
        if ( USB_DRIVE_NOT_MOUNTED == usbMountDrive( ) )
        {
            return FILEIO_RESULT_FAILURE;
        }
        needToUnmount = true;
    }

    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
        store_event( OF_WRITE_UDID_LOG );
    }

    if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, UDID_LOG_FILE, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_APPEND ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
        sprintf( appError.message, "Unable to open UDID log file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_UDID_FILE_OPEN;
        return FILEIO_RESULT_FAILURE;
    }

    flag = sprintf( buf, "%06lX%s%06lX%s%06lX%s%06lX%s%06lX\n",
                    appData.udid.words[0],
                    appDataLog.separator,
                    appData.udid.words[1],
                    appDataLog.separator,
                    appData.udid.words[2],
                    appDataLog.separator,
                    appData.udid.words[3],
                    appDataLog.separator,
                    appData.udid.words[4] );

    if ( flag > 0 )
    {
        numDataWritten = FILEIO_Write( buf, 1, flag, &file );

        if ( numDataWritten < flag )
        {
            errF = FILEIO_ErrorGet( 'A' );
            sprintf( appError.message, "Unable to write UDID frequency in log file (%u)", errF );
            appError.current_line_number = __LINE__;
            sprintf( appError.current_file_name, "%s", __FILE__ );
            FILEIO_ErrorClear( 'A' );
            appError.number = ERROR_UDID_FILE_WRITE;
            return FILEIO_RESULT_FAILURE;
        }
    }

    if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
        sprintf( appError.message, "Unable to close UDID log file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_UDID_FILE_CLOSE;
        return FILEIO_RESULT_FAILURE;
    }

    if ( true == needToUnmount )
    {
        if ( USB_DRIVE_MOUNTED == usbUnmountDrive( ) )
        {
            return FILEIO_RESULT_FAILURE;
        }
    }

    return FILEIO_RESULT_SUCCESS;

}


FILEIO_RESULT logRfidFreq( void )
{
    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    char buf[35];
    int flag, i;
    size_t numDataWritten;
    bool needToUnmount;

    getDateTime( );

    if ( USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status )
    {
        needToUnmount = false;
    }
    else
    {
        if ( USB_DRIVE_NOT_MOUNTED == usbMountDrive( ) )
        {
            return FILEIO_RESULT_FAILURE;
        }
        needToUnmount = true;
    }

    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
        store_event( OF_WRITE_RFID_LOG );
    }

    if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, RFID_LOG_FILE, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_APPEND ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
        sprintf( appError.message, "Unable to open RFID log file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_RFID_FILE_OPEN;
        return FILEIO_RESULT_FAILURE;
    }

    memset( buf, '\0', sizeof ( buf ) );

    for ( i = 0; i < appDataLog.num_rfid_freq_stored; i++ )
    {
        flag = sprintf( buf, "%c%c%sOF%c%c%s%u%s%02d/%02d/%02d%s%02d:%02d%s%ld\n",
                        appData.siteid[0],
                        appData.siteid[1],
                        appDataLog.separator,
                        appData.siteid[2],
                        appData.siteid[3],
                        appDataLog.separator,
                        getCompletScenarioNumber( ),
                        appDataLog.separator,
                        appData.current_time.tm_mday,
                        appData.current_time.tm_mon,
                        appData.current_time.tm_year,
                        appDataLog.separator,
                        appDataLog.rfid_freq[i][0],
                        appDataLog.rfid_freq[i][1],
                        appDataLog.separator,
                        ( long ) appDataLog.rfid_freq[i][2]*10 );


        if ( flag > 0 )
        {
            numDataWritten = FILEIO_Write( buf, 1, flag, &file );

            if ( numDataWritten < flag )
            {
                errF = FILEIO_ErrorGet( 'A' );
                sprintf( appError.message, "Unable to write RFID frequency in log file (%u)", errF );
                appError.current_line_number = __LINE__;
                sprintf( appError.current_file_name, "%s", __FILE__ );
                FILEIO_ErrorClear( 'A' );
                appError.number = ERROR_RFID_FILE_WRITE;
                return FILEIO_RESULT_FAILURE;
            }
        }
    }

    if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
        sprintf( appError.message, "Unable to close RFID log file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_RFID_FILE_CLOSE;
        return FILEIO_RESULT_FAILURE;
    }

    if ( true == needToUnmount )
    {
        if ( USB_DRIVE_MOUNTED == usbUnmountDrive( ) )
        {
            return FILEIO_RESULT_FAILURE;
        }
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf( "\tWrite RFID frequency to file success\n" );
#endif 

    clearRfidFreqBuffer( );

    return FILEIO_RESULT_SUCCESS;
}


FILEIO_RESULT logDs3231Temp( void )
{
    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    char buf[35];
    int flag, i;
    size_t numDataWritten;
    bool needToUnmount;

    getDateTime( );

    if ( USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status )
    {
        needToUnmount = false;
    }
    else
    {
        if ( USB_DRIVE_NOT_MOUNTED == usbMountDrive( ) )
        {
            return FILEIO_RESULT_FAILURE;
        }
        needToUnmount = true;
    }

    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
        store_event( OF_WRITE_EXT_TEMP_LOG );
    }

    if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, EXT_TEMP_LOG_FILE, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_APPEND ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
        sprintf( appError.message, "Unable to open external temperature log file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_EXT_TEMP_FILE_OPEN;
        return FILEIO_RESULT_FAILURE;
    }

    memset( buf, '\0', sizeof ( buf ) );

    for ( i = 0; i < appDataLog.num_ds3231_temp_stored; i++ )
    {
        flag = sprintf( buf, "%c%c%sOF%c%c%s%u%s%02d/%02d/%02d%s%02d:%02d%s%5.2f\n",
                        appData.siteid[0],
                        appData.siteid[1],
                        appDataLog.separator,
                        appData.siteid[2],
                        appData.siteid[3],
                        appDataLog.separator,
                        getCompletScenarioNumber( ),
                        appDataLog.separator,
                        appData.current_time.tm_mday,
                        appData.current_time.tm_mon,
                        appData.current_time.tm_year,
                        appDataLog.separator,
                        ( int ) appDataLog.ds3231_temp[i][0],
                        ( int ) appDataLog.ds3231_temp[i][1],
                        appDataLog.separator,
                        ( double ) appDataLog.ds3231_temp[i][2] );

        if ( flag > 0 )
        {
            numDataWritten = FILEIO_Write( buf, 1, flag, &file );

            if ( numDataWritten < flag )
            {
                errF = FILEIO_ErrorGet( 'A' );
                sprintf( appError.message, "Unable to write external temperature in log file (%u)", errF );
                appError.current_line_number = __LINE__;
                sprintf( appError.current_file_name, "%s", __FILE__ );
                FILEIO_ErrorClear( 'A' );
                appError.number = ERROR_EXT_TEMP_FILE_WRITE;
                return FILEIO_RESULT_FAILURE;
            }
        }
    }

    if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
        sprintf( appError.message, "Unable to close external temperature file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_EXT_TEMP_FILE_CLOSE;
        return FILEIO_RESULT_FAILURE;
    }

    if ( true == needToUnmount )
    {
        if ( USB_DRIVE_MOUNTED == usbUnmountDrive( ) )
        {
            return FILEIO_RESULT_FAILURE;
        }
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf( "\tWrite external temperature to file success\n" );
#endif 

    clearExtTemperatureBuffer( );

    return FILEIO_RESULT_SUCCESS;
}


FILEIO_RESULT logCalibration( void )
{
    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    char buf[35];
    int flag, i;
    size_t numDataWritten;
    bool needToUnmount;

    getDateTime( );

    if ( USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status )
    {
        needToUnmount = false;
    }
    else
    {
        if ( USB_DRIVE_NOT_MOUNTED == usbMountDrive( ) )
        {
            return FILEIO_RESULT_FAILURE;
        }
        needToUnmount = true;
    }

    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
        store_event( OF_WRITE_CALIB_LOG );
    }

    if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, CALIBRATION_LOG_FILE, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_APPEND ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
        sprintf( appError.message, "Unable to open calibration log file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_CALIB_FILE_OPEN;
        return FILEIO_RESULT_FAILURE;
    }

    memset( buf, '\0', sizeof ( buf ) );

    for ( i = 0; i < appDataLog.num_time_calib_stored; i++ )
    {
        flag = sprintf( buf, "%c%c%sOF%c%c%s%u%s%02d/%02d/20%02d%s%02d:%02d%s%.0f\n",
                        appData.siteid[0],
                        appData.siteid[1],
                        appDataLog.separator,
                        appData.siteid[2],
                        appData.siteid[3],
                        appDataLog.separator,
                        getCompletScenarioNumber( ),
                        appDataLog.separator,
                        appData.current_time.tm_mday,
                        appData.current_time.tm_mon,
                        appData.current_time.tm_year,
                        appDataLog.separator,
                        ( int ) appDataLog.time_calibration[i][0],
                        ( int ) appDataLog.time_calibration[i][1],
                        appDataLog.separator,
                        appDataLog.time_calibration[i][2] );

        if ( flag > 0 )
        {
            numDataWritten = FILEIO_Write( buf, 1, flag, &file );

            if ( numDataWritten < flag )
            {
                errF = FILEIO_ErrorGet( 'A' );
                sprintf( appError.message, "Unable to write calibration in log file (%u)", errF );
                appError.current_line_number = __LINE__;
                sprintf( appError.current_file_name, "%s", __FILE__ );
                FILEIO_ErrorClear( 'A' );
                appError.number = ERROR_CALIB_FILE_WRITE;
                return FILEIO_RESULT_FAILURE;
            }
        }
    }

    if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
        sprintf( appError.message, "Unable to close calibration file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_CALIB_FILE_CLOSE;
        return FILEIO_RESULT_FAILURE;
    }

    if ( true == needToUnmount )
    {
        if ( USB_DRIVE_MOUNTED == usbUnmountDrive( ) )
        {
            return FILEIO_RESULT_FAILURE;
        }
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf( "\tWrite calibration to file success\n" );
#endif 

    clearCalibrationBuffer( );

    return FILEIO_RESULT_SUCCESS;
}


int flushDataOnUsbDevice( )
{
    setLedsStatusColor( LED_USB_ACCESS );

    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
        store_event( OF_FLUSH_DATA_ON_USB_DEVICE );
    }
    
    if ( USB_DRIVE_NOT_MOUNTED == usbMountDrive( ) )
    {
        appDataUsb.is_device_needed = false;
        return FLUSH_DATA_ON_USB_DEVICE_FAIL;
    }

    if ( appDataLog.num_data_stored > 0 )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) 
        printf( "\tFlush birds data.\n" );
#endif
        /* Force data to be written on the USB device */
        appDataLog.num_data_stored = MAX_NUM_DATA_TO_STORE;
        if ( false == dataLog( false ) )
        {
            appDataUsb.is_device_needed = false;
            return FLUSH_DATA_ON_USB_DEVICE_FAIL;
        }
    }

    setLedsStatusColor( LED_USB_ACCESS );

    if ( true == appDataLog.log_battery && appDataLog.num_battery_level_stored > 0 )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) 
        printf( "\tFlush battery data.\n" );
#endif
        if ( FILEIO_RESULT_FAILURE == logBatteryLevel( ) )
        {
            appDataUsb.is_device_needed = false;
            return FLUSH_DATA_ON_USB_DEVICE_FAIL;
        }
    }

    setLedsStatusColor( LED_USB_ACCESS );

    if ( true == appDataLog.log_rfid && appDataLog.num_rfid_freq_stored > 0 )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) 
        printf( "\tFlush RFID data.\n" );
#endif
        if ( FILEIO_RESULT_FAILURE == logRfidFreq( ) )
        {
            appDataUsb.is_device_needed = false;
            return FLUSH_DATA_ON_USB_DEVICE_FAIL;
        }
    }

    setLedsStatusColor( LED_USB_ACCESS );

    if ( true == appDataLog.log_calibration && appDataLog.num_time_calib_stored > 0 )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) 
        printf( "\tFlush calibration data.\n" );
#endif
        if ( FILEIO_RESULT_FAILURE == logCalibration( ) )
        {
            appDataUsb.is_device_needed = false;
            return FLUSH_DATA_ON_USB_DEVICE_FAIL;
        }
    }

    setLedsStatusColor( LED_USB_ACCESS );

    if ( true == appDataLog.log_temp && appDataLog.num_ds3231_temp_stored > 0 )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) 
        printf( "\tFlush temperature data.\n" );
#endif
        if ( FILEIO_RESULT_FAILURE == logDs3231Temp( ) )
        {
            appDataUsb.is_device_needed = false;
            return FLUSH_DATA_ON_USB_DEVICE_FAIL;
        }
    }

    setLedsStatusColor( LED_USB_ACCESS );

    /* Log event if required */
    if ( true == appDataLog.log_events && appDataEvent.num_events_stored > 0 )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) 
        printf( "\tFlush events data.\n" );
#endif
        if ( FILEIO_RESULT_FAILURE == logEvents( ) )
        {
            appDataUsb.is_device_needed = false;
            return FLUSH_DATA_ON_USB_DEVICE_FAIL;
        }
    }

    if ( USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status )
    {
        usbUnmountDrive( );
    }

    setLedsStatusColor( LEDS_OFF );

    return FLUSH_DATA_ON_USB_DEVICE_SUCCESS;

}


// Placeholder function to get the timestamp for FILEIO operations


void GetTimestamp( FILEIO_TIMESTAMP * timestamp )
{
    /* help_mla_fileio.pdf 
     * 1.7.1.3.32 FILEIO_TimestampGet Type */

    getDateTime( );

    timestamp->date.bitfield.day = appData.current_time.tm_mday;
    timestamp->date.bitfield.month = appData.current_time.tm_mon;
    timestamp->date.bitfield.year = appData.current_time.tm_year + 20; // 2000-1980 = 20
    timestamp->time.bitfield.hours = appData.current_time.tm_hour;
    timestamp->time.bitfield.secondsDiv2 = appData.current_time.tm_sec / 2;
    timestamp->time.bitfield.minutes = appData.current_time.tm_min;
    timestamp->timeMs = 0;
}


/*******************************************************************************
 End of File
 */
