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
    char buf[200];
    struct tm currentTime;
    int flag;
    size_t numDataWritten;

    getDateTime(&currentTime);

    if (USB_DRIVE_NOT_MOUNTED == usbMountDrive())
    {
        return FILEIO_RESULT_FAILURE;
    }

    if (FILEIO_RESULT_FAILURE == FILEIO_Open(&file, "ERROR.CSV", FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_APPEND))
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

    flag = sprintf(buf, "%c%c,OF%c%c,%u,%02d/%02d/%02d,%02d:%02d:%2d,%u,%s\n",
                   appData.siteid[0],
                   appData.siteid[1],
                   appData.siteid[2],
                   appData.siteid[3],
                   appData.scenario_number,
                   currentTime.tm_mday,
                   currentTime.tm_mon,
                   currentTime.tm_year,
                   currentTime.tm_hour,
                   currentTime.tm_min,
                   currentTime.tm_sec,                   
                   appError.number,
                   appError.message);

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

    if (USB_DRIVE_MOUNTED == usbUnmountDrive())
    {
        return FILEIO_RESULT_FAILURE;
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf("\tError to file success\n");
#endif 

    return FILEIO_RESULT_SUCCESS;
    
    
}

void printError(void)
{
    if (appError.currentLineNumber > 0)
        printf("ERROR %02d: %s\nIn %s (%d)\n", appError.number, appError.message, appError.currentFileName, appError.currentLineNumber);
    else
        printf("ERROR %02d: %s\n", appError.number, appError.message);
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
