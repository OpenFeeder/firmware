/**
 * @file app_error.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 06/07/2016
 * @revision
 */

#include "app.h"
#include "app_event.h"

void store_event(APP_EVENT ev) 
{
    struct tm currentTime;
    getDateTime(&currentTime);
   
    appDataEvent.numbers[appDataEvent.num_events_stored] = ev;    
    appDataEvent.hours[appDataEvent.num_events_stored] = currentTime.tm_hour;
    appDataEvent.minutes[appDataEvent.num_events_stored] = currentTime.tm_min;
    appDataEvent.seconds[appDataEvent.num_events_stored] = currentTime.tm_sec;
    
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_EVENT_INFO )
    printf("\t\tEvent %03d - %02d:%02d:%02d %03d\n", appDataEvent.num_events_stored, 
       appDataEvent.hours[appDataEvent.num_events_stored], 
       appDataEvent.minutes[appDataEvent.num_events_stored], 
       appDataEvent.seconds[appDataEvent.num_events_stored], 
       appDataEvent.numbers[appDataEvent.num_events_stored]);
#endif
    
    appDataEvent.num_events_stored +=1;
    
}

FILEIO_RESULT logEvents(void)
{
    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    char buf[35];
    uint8_t ibuf[4];
    struct tm currentTime;
    int flag, i;
    size_t numDataWritten;
    bool needToUnmount;
    
    getDateTime(&currentTime);

    if ( true == appDataLog.log_events )
    {
       store_event(OF_WRITE_EVENTS_LOG); 
    }
    
    if ( USB_DRIVE_MOUNTED == appDataUsb.usbDriveStatus )
    {
        needToUnmount = false;
        if ( true == appDataLog.log_events )
        {
           store_event(OF_ALREADY_MOUNTED_USB_DRIVE); 
        }
    }
    else
    {
        if (USB_DRIVE_NOT_MOUNTED == usbMountDrive())
        {
            return FILEIO_RESULT_FAILURE;
        }
        needToUnmount = true;
    }

    if (EVENT_FILE_TEXT == appDataEvent.file_type || EVENT_FILE_BINARY_AND_TEXT == appDataEvent.file_type )
    {
        if (FILEIO_RESULT_FAILURE == FILEIO_Open(&file, appDataEvent.filename, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_APPEND))
        {
            errF = FILEIO_ErrorGet('A');
            sprintf(appError.message, "Unable to open event CSV log file (%u)", errF);
            appError.currentLineNumber = __LINE__;
            sprintf(appError.currentFileName, "%s", __FILE__);
            FILEIO_ErrorClear('A');
            appError.number = ERROR_EVENT_CSV_FILE_OPEN;
            return FILEIO_RESULT_FAILURE;
        }

        memset(buf, '\0', sizeof ( buf));

        for (i = 0; i < appDataEvent.num_events_stored; i++)
        {
            flag = sprintf(buf, "%02d:%02d:%02d,%03d\n", 
                           appDataEvent.hours[i], 
                           appDataEvent.minutes[i], 
                           appDataEvent.seconds[i], 
                           appDataEvent.numbers[i]);

            if (flag > 0)
            {
                numDataWritten = FILEIO_Write(buf, 1, flag, &file);

                if (numDataWritten < flag)
                {
                    errF = FILEIO_ErrorGet('A');
                    sprintf(appError.message, "Unable to write event in CSV log file (%u)", errF);
                    appError.currentLineNumber = __LINE__;
                    sprintf(appError.currentFileName, "%s", __FILE__);
                    FILEIO_ErrorClear('A');
                    appError.number = ERROR_EVENT_CSV_FILE_WRITE;
                    return FILEIO_RESULT_FAILURE;
                }
            }
        }

        if (FILEIO_RESULT_FAILURE == FILEIO_Close(&file))
        {
            errF = FILEIO_ErrorGet('A');
            sprintf(appError.message, "Unable to close event CSV log file (%u)", errF);
            appError.currentLineNumber = __LINE__;
            sprintf(appError.currentFileName, "%s", __FILE__);
            FILEIO_ErrorClear('A');
            appError.number = ERROR_EVENT_CSV_FILE_CLOSE;
            return FILEIO_RESULT_FAILURE;
        }
    }
    
    if (EVENT_FILE_BINARY == appDataEvent.file_type || EVENT_FILE_BINARY_AND_TEXT == appDataEvent.file_type )
    {
        if (FILEIO_RESULT_FAILURE == FILEIO_Open(&file, appDataEvent.binfilename, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_APPEND))
        {
            errF = FILEIO_ErrorGet('A');
            sprintf(appError.message, "Unable to open event BIN log file (%u)", errF);
            appError.currentLineNumber = __LINE__;
            sprintf(appError.currentFileName, "%s", __FILE__);
            FILEIO_ErrorClear('A');
            appError.number = ERROR_EVENT_BIN_FILE_OPEN;
            return FILEIO_RESULT_FAILURE;
        }

        memset(ibuf, 0, sizeof ( ibuf));

        for (i = 0; i < appDataEvent.num_events_stored; i++)
        {
            buf[0] = appDataEvent.hours[i]; 
            buf[1] = appDataEvent.minutes[i]; 
            buf[2] = appDataEvent.seconds[i]; 
            buf[3] = appDataEvent.numbers[i];
            
            numDataWritten = FILEIO_Write(buf, 1, 4, &file);

            if (numDataWritten < 4)
            {
                errF = FILEIO_ErrorGet('A');
                sprintf(appError.message, "Unable to write event in BIN log file (%u)", errF);
                appError.currentLineNumber = __LINE__;
                sprintf(appError.currentFileName, "%s", __FILE__);
                FILEIO_ErrorClear('A');
                appError.number = ERROR_EVENT_BIN_FILE_WRITE;
                return FILEIO_RESULT_FAILURE;
            }
        }

        if (FILEIO_RESULT_FAILURE == FILEIO_Close(&file))
        {
            errF = FILEIO_ErrorGet('A');
            sprintf(appError.message, "Unable to close event BIN log file (%u)", errF);
            appError.currentLineNumber = __LINE__;
            sprintf(appError.currentFileName, "%s", __FILE__);
            FILEIO_ErrorClear('A');
            appError.number = ERROR_EVENT_BIN_FILE_CLOSE;
            return FILEIO_RESULT_FAILURE;
        }
    }
    
    /* Do not move the next line below unmount action because unmount events wont be recorded */
    clear_events_buffers( );
    
    if (true == needToUnmount)
    {
        if (USB_DRIVE_MOUNTED == usbUnmountDrive())
        {
            return FILEIO_RESULT_FAILURE;
        }
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf("\tWrite events to file success\n");
#endif 

    return FILEIO_RESULT_SUCCESS;
}

bool setEventFileName(void)
{

    /* Clear filename buffer */
    memset(appDataEvent.filename, 0, sizeof (appDataEvent.filename));
    memset(appDataEvent.binfilename, 0, sizeof (appDataEvent.binfilename));

    /* Get current date */
    while (!RTCC_TimeGet(&appData.current_time))
    {
        Nop();
    }

    if (EVENT_FILE_TEXT == appDataEvent.file_type || EVENT_FILE_BINARY_AND_TEXT == appDataEvent.file_type )
    {
        
        /* Set event log file name => EVyymmdd.CSV */
        if (snprintf(appDataEvent.filename, 13, "EV%02d%02d%02d.CSV",
                     appData.current_time.tm_year,
                     appData.current_time.tm_mon,
                     appData.current_time.tm_mday) <= 0)

        {
    #if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_EVENT_INFO )
            printf("Unable to set event file name\n");
    #endif 
            sprintf(appError.message, "Unable to set event file CSV name");
            appError.currentLineNumber = __LINE__;
            sprintf(appError.currentFileName, "%s", __FILE__);
            appError.number = ERROR_EVENT_CSV_FILE_SET_NAME;
            return false;
        }
    #if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_EVENT_INFO )
            printf("\tTXT event file name: %s\n", appDataEvent.filename);
    #endif      
    }
    
    if (EVENT_FILE_BINARY == appDataEvent.file_type || EVENT_FILE_BINARY_AND_TEXT == appDataEvent.file_type )
    {
        /* Set event log file name => EVyymmdd.BIN */
        if (snprintf(appDataEvent.binfilename, 13, "EV%02d%02d%02d.BIN",
                     appData.current_time.tm_year,
                     appData.current_time.tm_mon,
                     appData.current_time.tm_mday) <= 0)

        {
    #if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_EVENT_INFO )
            printf("Unable to set event file name\n");
    #endif 
            sprintf(appError.message, "Unable to set event file BIN name");
            appError.currentLineNumber = __LINE__;
            sprintf(appError.currentFileName, "%s", __FILE__);
            appError.number = ERROR_EVENT_BIN_FILE_SET_NAME;
            return false;
        }
    #if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_EVENT_INFO )
            printf("\tBIN event file name: %s\n", appDataEvent.binfilename);
    #endif 
    }
    return true;
}

void display_events( void )
{
    int i;
    
    if (0 == appDataEvent.num_events_stored)
    {
       printf("\tNo event\n"); 
    }
    else
    {
        printf("\tEvents\n");

        for (i=0;i<appDataEvent.num_events_stored;i++)
        {
            printf("\t\t#%03d - %02d:%02d:%02d %03d\n", i, appDataEvent.hours[i], appDataEvent.minutes[i], appDataEvent.seconds[i], appDataEvent.numbers[i]);
        }
    }
}

void clear_events_buffers( void )
{
    int i;
    for (i=0;i<MAX_NUMBER_OF_EVENT;i++)
    {
        appDataEvent.numbers[i] = 0;
        appDataEvent.hours[i] = 0;
        appDataEvent.minutes[i] = 0;
        appDataEvent.seconds[i] = 0; 
    }
    appDataEvent.num_events_stored = 0;
}

/*******************************************************************************
 End of File
 */
