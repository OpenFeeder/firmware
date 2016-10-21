/**
 * @file app_error.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 06/07/2016
 * @revision
 */

#include "app.h"
#include "app_event.h"


FILEIO_RESULT logBatteryLevel(void)
{

    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    char buf[100];
    struct tm currentTime;
    int flag;
    size_t numDataWritten;

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf("Write battery level to file - ");
#endif

    getBatteryLevel();
    getDateTime(&currentTime);

    if (USB_DRIVE_NOT_MOUNTED == usbMountDrive())
    {
        return FILEIO_RESULT_FAILURE;
    }

    if (FILEIO_RESULT_FAILURE == FILEIO_Open(&file, "BATTERY.LOG", FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_APPEND))
    {
        errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
        printf("unable to open battery log file (%u)", errF);
#endif 
        sprintf(appError.message, "Unable to open battery log file (%u)", errF);
        appError.currentLineNumber = __LINE__;
        sprintf(appError.currentFileName, "%s", __FILE__);
        FILEIO_ErrorClear('A');
        return FILEIO_RESULT_FAILURE;
    }

    flag = sprintf(buf, "%02d/%02d/%02d%s%02d:%02d:%02u%s"
                   /* battery_level  */ "%04d\n",
                   currentTime.tm_mday,
                   currentTime.tm_mon,
                   currentTime.tm_year,
                   appDataLog.separator,
                   currentTime.tm_hour,
                   currentTime.tm_min,
                   currentTime.tm_sec,
                   appDataLog.separator,
                   appData.battery_level);

    if (flag > 0)
    {
        numDataWritten = FILEIO_Write(buf, 1, flag, &file);
    }

    if (numDataWritten < appDataLog.nCharBuffer)
    {
        errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
        printf("unable to write battery level in log file (%u)", errF);
#endif 
        sprintf(appError.message, "Unable to write battery level in log file (%u)", errF);
        appError.currentLineNumber = __LINE__;
        sprintf(appError.currentFileName, "%s", __FILE__);
        FILEIO_ErrorClear('A');
        return FILEIO_RESULT_FAILURE;
    }

    if (FILEIO_RESULT_FAILURE == FILEIO_Close(&file))
    {
        errF = FILEIO_ErrorGet('A');
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
        printf("unable to close battery log file (%u)", errF);
#endif 
        sprintf(appError.message, "Unable to close battery log file (%u)", errF);
        appError.currentLineNumber = __LINE__;
        sprintf(appError.currentFileName, "%s", __FILE__);
        FILEIO_ErrorClear('A');
        return FILEIO_RESULT_FAILURE;
    }



#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf("success\n");
#endif 

    return FILEIO_RESULT_SUCCESS;

}


/*******************************************************************************
 End of File
 */
