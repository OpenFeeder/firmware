/**
 * @file app_error.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 06/07/2016
 * @revision
 */

#include "app.h"
#include "app_error.h"

FILEIO_RESULT logError(void)
{
    
    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    char buf[250];
    int flag;
    size_t numDataWritten;
    bool needToUnmount;
    
    getDateTime( );

    if ( USB_DRIVE_MOUNTED == appDataUsb.usbDriveStatus )
    {
        needToUnmount = false;
    }
    else
    {
        if (USB_DRIVE_NOT_MOUNTED == usbMountDrive())
        {
            return FILEIO_RESULT_FAILURE;
        }
        needToUnmount = true;
    }

    if ( true == appDataLog.log_events )
    {
       store_event(OF_WRITE_ERRORS_LOG); 
    }
    
    if (FILEIO_RESULT_FAILURE == FILEIO_Open(&file, ERRORS_LOG_FILE, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_APPEND))
    {
        errF = FILEIO_ErrorGet('A');
        sprintf(appError.message, "Unable to open error log file (%u)", errF);
        appError.currentLineNumber = __LINE__;
        sprintf(appError.currentFileName, "%s", __FILE__);
        FILEIO_ErrorClear('A');
        appError.number = ERROR_ERROR_FILE_OPEN;
        return FILEIO_RESULT_FAILURE;
    }

    memset(buf, '\0', sizeof ( buf));
    
    flag = sprintf(buf, "%c%c%sOF%c%c%s%u%s%02d/%02d/%02d%s%02d:%02d:%02d%s%u%s\"%s\"%s%s%s%u\n",
                    appData.siteid[0],
                    appData.siteid[1],
                    appDataLog.separator,
                    appData.siteid[2],
                    appData.siteid[3],
                    appDataLog.separator,
                    getCompletScenarioNumber(),
                    appDataLog.separator,
                    appData.current_time.tm_mday,
                    appData.current_time.tm_mon,
                    appData.current_time.tm_year,
                    appDataLog.separator,
                    appData.current_time.tm_hour,
                    appData.current_time.tm_min,
                    appData.current_time.tm_sec,
                    appDataLog.separator,                   
                    appError.number,
                    appDataLog.separator,
                    appError.message,
                    appDataLog.separator,
                    appError.currentFileName,
                    appDataLog.separator,
                    appError.currentLineNumber);

    if (flag > 0)
    {
        numDataWritten = FILEIO_Write(buf, 1, flag, &file);
    }

    if (numDataWritten < flag)
    {
        errF = FILEIO_ErrorGet('A');
        sprintf(appError.message, "Unable to write error in log file (%u)", errF);
        appError.currentLineNumber = __LINE__;
        sprintf(appError.currentFileName, "%s", __FILE__);
        FILEIO_ErrorClear('A');
        appError.number = ERROR_ERROR_FILE_WRITE;
        return FILEIO_RESULT_FAILURE;
    }

    if (FILEIO_RESULT_FAILURE == FILEIO_Close(&file))
    {
        errF = FILEIO_ErrorGet('A');
        sprintf(appError.message, "Unable to close error file (%u)", errF);
        appError.currentLineNumber = __LINE__;
        sprintf(appError.currentFileName, "%s", __FILE__);
        FILEIO_ErrorClear('A');
        appError.number = ERROR_ERROR_FILE_CLOSE;
        return FILEIO_RESULT_FAILURE;
    }

    if (true == needToUnmount)
    {
        if (USB_DRIVE_MOUNTED == usbUnmountDrive())
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
    
    if ( appError.currentLineNumber > 0 )
    {
        printf( "\tERROR %03d\n\t%s\n\tIn %s at line %d\n", appError.number, appError.message, appError.currentFileName, appError.currentLineNumber);
    }
    else
    {
        printf( "\tERROR %03d\n%s\n", appError.number, appError.message);
    }
}


void clearError(void)
{
    memset(appError.message, '\0', sizeof ( appError.message));
    memset(appError.currentFileName, '\0', sizeof ( appError.currentFileName));
    appError.currentLineNumber = 0;
}


/*******************************************************************************
 End of File
 */
