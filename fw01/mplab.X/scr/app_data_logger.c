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


static int populateLogBuffer( void )
{
    int flag;
    char line[MAX_CHAR_PER_LINE];

    unsigned long delayS;

    //#if defined (USE_UART1_SERIAL_INTERFACE)
    //    printf(" - Populate buffer\n");
    //#endif

    if ( 0 == strcmp( appDataLog.bird_pit_tag_str, "XXXXXXXXXX" ) )
    {
        appDataLog.is_reward_taken = false;
    }

    if ( true == appDataLog.is_pit_tag_denied )
    {
        appDataLog.is_reward_taken = false;
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

    flag = sprintf( line, "%02d/%02d/%02d%s%02d:%02d:%02d%s"
                    /* siteid               */ "%s"
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
                    /* delayS                        */ "%lu\n",
                    appDataLog.bird_arrived_time.tm_mday,
                    appDataLog.bird_arrived_time.tm_mon,
                    appDataLog.bird_arrived_time.tm_year,
                    appDataLog.separator,
                    appDataLog.bird_arrived_time.tm_hour,
                    appDataLog.bird_arrived_time.tm_min,
                    appDataLog.bird_arrived_time.tm_sec,
                    appDataLog.separator,
                    appData.siteid,
                    appDataLog.separator,
                    appData.scenario_number,
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
                    delayS );

    if ( flag > 0 )
    {
        /* Concatenation de chaines de caracteres dans le buffer. */
        strcat( appDataLog.buffer, line );
    }

    //#if defined (USE_UART1_SERIAL_INTERFACE)
    //    printf("%s", appDataLog.buffer);
    //#endif 

    return flag;
}

// *****************************************************************************
// LogWrite - Ecriture dans le fichier LOG.CSV
// *****************************************************************************


static int writeLogFile( void )
{
    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    size_t numDataWritten;

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf( "Write data to file - " );
#endif 

    if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, appDataLog.filename, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_APPEND ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
        printf( "unable to open log file (%u)", errF );
#endif 
        sprintf( appError.message, "Unable to open log file (%u)", errF );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        return FILEIO_RESULT_FAILURE;
    }

    numDataWritten = FILEIO_Write( appDataLog.buffer, 1, appDataLog.nCharBuffer, &file );

    if ( numDataWritten < appDataLog.nCharBuffer )
    {
        errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
        printf( "unable to write data in log file (%u)", errF );
#endif 
        sprintf( appError.message, "Unable to write data in log file (%u)", errF );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        return FILEIO_RESULT_FAILURE;
    }

    if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
        printf( "unable to close log file (%u)", errF );
#endif 
        sprintf( appError.message, "Unable to close log file (%u)", errF );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        return FILEIO_RESULT_FAILURE;
    }

    clearLogBuffer( );

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf( "data to log file success\n" );
#endif 

    return FILEIO_RESULT_SUCCESS;
}


void clearLogBuffer( void )
{
    /* Vidage du buffer. */
    memset( appDataLog.buffer, '\0', sizeof ( appDataLog.buffer ) );
    appDataLog.nCharBuffer = 0;

}


bool dataLog( void )
{
    unsigned int nChar = 0;

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf( "Data logging - " );
#endif 

    nChar = populateLogBuffer( );

    if ( nChar < 0 )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
        printf( "unable to populate log buffer\n" );
#endif 
        sprintf( appError.message, "Unable to populate log buffer" );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        return false;
    }
    else
    {
        appDataLog.nCharBuffer += nChar;
        appDataLog.numDataStored += 1;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO )
        printf( "data to buffer (%u/%u)\n", appDataLog.numDataStored, MAX_NUM_DATA_TO_STORE );
#endif 
    }

    /* Quantite donnees suffisantes => ecriture dans fichier LOG */
    if ( appDataLog.numDataStored == MAX_NUM_DATA_TO_STORE )
    {

        if ( USB_DRIVE_NOT_MOUNTED == usbMountDrive( ) )
        {
            return false;
        }

        /* Ecriture fichier LOG. */
        if ( FILEIO_RESULT_FAILURE == writeLogFile( ) )
        {
            usbUnmountDrive( );
            CMD_VDD_APP_V_USB_SetLow( );
            return false;
        }
        appDataLog.numDataStored = 0;
        usbUnmountDrive( );
        CMD_VDD_APP_V_USB_SetLow( );
    }

    return true;
}


bool setLogFileName( void )
{

    /* Clear filename buffer */
    memset( appDataLog.filename, 0, sizeof (appDataLog.filename ) );

    /* Get current date */
    while ( !RTCC_TimeGet( &appData.current_time ) )
    {
        Nop( );
    }

    /* Set log file name => 20yymmdd.CSV */
    if ( snprintf( appDataLog.filename, 13, "20%02d%02d%02d.CSV",
                   appData.current_time.tm_year,
                   appData.current_time.tm_mon,
                   appData.current_time.tm_mday ) <= 0 )

    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO )
        printf( "Unable to set log file name\n" );
#endif 
        sprintf( appError.message, "Unable to set log file name" );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        return false;
    }

//#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO )
//    printf( "Log file name: %s\n", appDataLog.filename );
//#endif 

    return true;
}

// Placeholder function to get the timestamp for FILEIO operations


void GetTimestamp( FILEIO_TIMESTAMP * timestamp )
{
    /* help_mla_fileio.pdf 
     * 1.7.1.3.32 FILEIO_TimestampGet Type */

    while ( !RTCC_TimeGet( &appData.current_time ) )
    {
        Nop( );
    }

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
