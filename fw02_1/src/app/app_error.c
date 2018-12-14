/**
 * @file app_error.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 06/07/2016
 * @revision
 */

#include "app.h"
#include "app_error.h"

FILEIO_RESULT logError( void )
{
    
    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    char buf[250];
    int flag;
    size_t numDataWritten;
    bool needToUnmount;
    
    getDateTime( );

    if ( USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status )
    {
        needToUnmount = false;
    }
    else
    {
        if (USB_DRIVE_NOT_MOUNTED == usbMountDrive( ) )
        {
            return FILEIO_RESULT_FAILURE;
        }
        needToUnmount = true;
    }

    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
       store_event( OF_WRITE_ERRORS_LOG ); 
    }
    
    if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, ERRORS_LOG_FILE, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_APPEND ) )
    {
        errF = FILEIO_ErrorGet( 'A' );
        sprintf( appError.message, "Unable to open error log file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_ERROR_FILE_OPEN;
        return FILEIO_RESULT_FAILURE;
    }

    memset( buf, '\0', sizeof( buf ) );
    
    flag = sprintf( buf, "%c%c,OF%c%c,%u,%02d/%02d/%04d,%02d:%02d:%02d,%u,\"%s\",%s,%u\n",
                    appData.siteid[0],
                    appData.siteid[1],
                    appData.siteid[2],
                    appData.siteid[3],
                    getCompletScenarioNumber(),
                    appData.current_time.tm_mday,
                    appData.current_time.tm_mon,
                    2000 + appData.current_time.tm_year,
                    appData.current_time.tm_hour,
                    appData.current_time.tm_min,
                    appData.current_time.tm_sec,                  
                    appError.number,
                    appError.message,
                    appError.current_file_name,
                    appError.current_line_number );

    if ( flag > 0 )
    {
        numDataWritten = FILEIO_Write( buf, 1, flag, &file );
    }

    if ( numDataWritten < flag )
    {
        errF = FILEIO_ErrorGet( 'A' );
        sprintf( appError.message, "Unable to write error in log file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear('A');
        appError.number = ERROR_ERROR_FILE_WRITE;
        return FILEIO_RESULT_FAILURE;
    }

    if (FILEIO_RESULT_FAILURE == FILEIO_Close(&file))
    {
        errF = FILEIO_ErrorGet('A');
        sprintf( appError.message, "Unable to close error file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_ERROR_FILE_CLOSE;
        return FILEIO_RESULT_FAILURE;
    }

    if (true == needToUnmount)
    {
        if (USB_DRIVE_MOUNTED == usbUnmountDrive( ) )
        {
            return FILEIO_RESULT_FAILURE;
        }
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf("\tError to file success\n");
#endif 

    return FILEIO_RESULT_SUCCESS;
    
    
}

void printError(void)
{
    printf( "\t%02u/%02u/20%02u %02u:%02u:%02u ",
            appError.time.tm_mday,
            appError.time.tm_mon,
            appError.time.tm_year,
            appError.time.tm_hour,
            appError.time.tm_min,
            appError.time.tm_sec );
    
    if ( appError.current_line_number > 0 )
    {
        printf( "\tERROR %03d\n\t%s\n\tIn %s at line %d\n", appError.number, appError.message, appError.current_file_name, appError.current_line_number );
    }
    else
    {
        printf( "\tERROR %03d\n%s\n", appError.number, appError.message );
    }
}


void clearError(void)
{
    memset( appError.message, '\0', sizeof( appError.message ) );
    memset( appError.current_file_name, '\0', sizeof( appError.current_file_name ) );
    appError.current_line_number = 0;
}


/*******************************************************************************
 End of File
 */
