/*
 * File:   app_fileio.c
 * Author: jerome
 *
 * Created on 7 novembre 2018, 09:23
 */


#include "app.h"

static uint8_t stx = 0xFA;
static uint8_t etx = 0xFB;
static uint8_t enq = 0xFC;
static uint8_t ack = 0xFD;
static uint8_t dc4 = 0xFE;
static uint8_t nack = 0xFF;
static char ch[FILEIO_EXPORT_BUFFER_SIZE];

void listFilesOnUsbDevice() {
    
    FILEIO_SEARCH_RECORD searchRecord;
    int8_t result;
                
    setLedsStatusColor( LED_USB_ACCESS );
   
    usbMountDrive( );

    result = FILEIO_Find( "*.*", FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, true );

    if (FILEIO_RESULT_SUCCESS == result) {

        sprintf(ch, "\t%s", searchRecord.shortFileName);
        daves_putU1(ch, strlen(ch));
        sprintf(ch, " (%ld bytes)\r\n", searchRecord.fileSize);
        daves_putU1(ch, strlen(ch));
        
        while (FILEIO_RESULT_SUCCESS == FILEIO_Find( "*.*", FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, false )) {

            sprintf(ch, "\t%s", searchRecord.shortFileName);
            daves_putU1(ch, strlen(ch));
            sprintf(ch, " (%ld bytes)\r\n", searchRecord.fileSize);
            daves_putU1(ch, strlen(ch));
            
        }

    }
    else {
        
        displayFileErr( searchRecord.shortFileName, false );
        
    }

    usbUnmountDrive( );
    
}

void exportFile(const char * filename, bool fileType)
{
    
    FILEIO_SEARCH_RECORD searchRecord;
    int8_t result;
    FILEIO_OBJECT file;
    size_t num_read;

    memset(ch, '\0', sizeof (ch));

    result = FILEIO_Find( filename, FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, true );

    if (FILEIO_RESULT_SUCCESS == result) {                            
        
        sprintf(ch, "%c%c%s%c", ack, enq, searchRecord.shortFileName, dc4);
        daves_putU1(ch, strlen(ch));
        sprintf(ch, "%c%c%c%c", ack, enq, 'T', dc4);
        daves_putU1(ch, strlen(ch));
        sprintf(ch, "%c%c%ld%c", ack, enq, searchRecord.fileSize, dc4);
        daves_putU1(ch, strlen(ch));

        result = FILEIO_Open(&file, searchRecord.shortFileName, FILEIO_OPEN_READ);

        if (FILEIO_RESULT_SUCCESS == result) {

            sprintf(ch, "%c%c", ack, enq);
            daves_putU1(ch, strlen(ch));
            
            while (false == FILEIO_Eof(&file)) {

                if ('T' == fileType )
                {
                    memset(ch, '\0', sizeof (ch));
                    FILEIO_Read(ch, 1, FILEIO_EXPORT_BUFFER_SIZE-1, &file);                
                    daves_putU1(ch, strlen(ch));
                }
                else
                {
                    num_read = FILEIO_Read(ch, 1, FILEIO_EXPORT_BUFFER_SIZE, &file);
                    daves_putU1(ch, num_read);
                }
   
            }
            
            FILEIO_Close(&file);
            
            sprintf(ch, "%c", dc4);
            daves_putU1(ch, strlen(ch));
            
        }
        else {
            
            displayFileErr( searchRecord.shortFileName, true );
            
        }

        while (FILEIO_RESULT_SUCCESS == FILEIO_Find( filename, FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, false )) {
            
            sprintf(ch, "%c%c%s%c", ack, enq, searchRecord.shortFileName, dc4);
            daves_putU1(ch, strlen(ch));

            sprintf(ch, "%c%c%c%c", ack, enq, 'T', dc4);
            daves_putU1(ch, strlen(ch));
            
            sprintf(ch, "%c%c%ld%c", ack, enq, searchRecord.fileSize, dc4);
            daves_putU1(ch, strlen(ch));

            result = FILEIO_Open(&file, searchRecord.shortFileName, FILEIO_OPEN_READ);

            if (FILEIO_RESULT_SUCCESS == result) {

                sprintf(ch, "%c%c", ack, enq);
                daves_putU1(ch, strlen(ch));
            
                while (false == FILEIO_Eof(&file)) {

                    if ('T' == fileType )
                    {
                        memset(ch, '\0', sizeof (ch));
                        FILEIO_Read(ch, 1, FILEIO_EXPORT_BUFFER_SIZE-1, &file);                
                        daves_putU1(ch, strlen(ch));
                    }
                    else
                    {
                        num_read = FILEIO_Read(ch, 1, FILEIO_EXPORT_BUFFER_SIZE, &file);
                        daves_putU1(ch, num_read);
                    }

                }
                
                FILEIO_Close(&file); 

                sprintf(ch, "%c", dc4);
                daves_putU1(ch, strlen(ch));
                
            }
            else {

                displayFileErr( searchRecord.shortFileName, true );
                
            }
        }
        sprintf(ch, "%c", nack);
        daves_putU1(ch, strlen(ch));
    }
}

void exportAllFiles(){
    
    setLedsStatusColor( LED_USB_ACCESS );

    usbMountDrive( );

    sprintf(ch, "%c", stx);
    daves_putU1(ch, strlen(ch));

    exportFile( "20??????.CSV", 'T' );
    exportFile( "EV??????.BIN", 'B' );
    exportFile( "CONFIG.INI", 'T' );
    exportFile( BATTERY_LOG_FILE, 'T' );
    exportFile( ERRORS_LOG_FILE, 'T' );
    exportFile( RFID_LOG_FILE, 'T' );
    exportFile( CALIBRATION_LOG_FILE, 'T' );
    exportFile( UDID_LOG_FILE, 'T' );
    exportFile( EXT_TEMP_LOG_FILE, 'T' );    
    exportFile( "UDIDLEDS.CSV", 'T' );
    exportFile( "PTLEFT.TXT", 'T' );
    exportFile( "PTRIGHT.TXT", 'T' );
    exportFile( "PTTOP.TXT", 'T' );
    exportFile( "PTBOTTOM.TXT", 'T' );
    exportFile( "PTONE1.TXT", 'T' );
    exportFile( "PTONE2.TXT", 'T' );
    exportFile( "PTONE3.TXT", 'T' );
    exportFile( "PTONE4.TXT", 'T' );
    exportFile( "PTACCEPT.TXT", 'T' );
    exportFile( "PTDENIED.TXT", 'T' );
    exportFile( "PTCOLORA.TXT", 'T' );
    exportFile( "PTCOLORB.TXT", 'T' );
    exportFile( "PROBA.TXT", 'T' );

    sprintf(ch, "%c", etx);
    daves_putU1(ch, strlen(ch));
    
    usbUnmountDrive( );
}

void displayFileErr(const char * filename, bool export) {

    FILEIO_ERROR_TYPE errF; 
    
    if (true == export)
    {
        sprintf(ch, "%c%c\t%s - ", nack, enq, filename);
        daves_putU1(ch, strlen(ch));
    }
            
    errF = FILEIO_ErrorGet('A');

    switch ( errF ) {

        case FILEIO_ERROR_INVALID_ARGUMENT:
            sprintf(ch, "The path could not be resolved.\r\n");
            daves_putU1(ch, strlen(ch));
            break;

        case FILEIO_ERROR_INVALID_FILENAME:
            sprintf(ch, "The file name is invalid.\r\n");
            daves_putU1(ch, strlen(ch));
            break;

        case FILEIO_ERROR_BAD_CACHE_READ:
            sprintf(ch, "There was an error searching directory entries.\r\n");
            daves_putU1(ch, strlen(ch));
            break;

        case FILEIO_ERROR_DONE:
            sprintf(ch, "File not found.\r\n");
            daves_putU1(ch, strlen(ch));
            break;

        default:
            sprintf(ch, "Error (%d).\r\n", errF);
            daves_putU1(ch, strlen(ch));
            break;
    
    }
    
    if (true == export)
    {
        sprintf(ch, "%c", dc4);
        daves_putU1(ch, strlen(ch));
    }
        
}

void displayCsvFiles() {
    
    FILEIO_SEARCH_RECORD searchRecord;
    int8_t result;
    FILEIO_OBJECT file;
    
    memset(ch, '\0', sizeof (ch));
    
    setLedsStatusColor( LED_USB_ACCESS );
                        
    usbMountDrive( );

    result = FILEIO_Find( "20??????.CSV", FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, true );

    if (FILEIO_RESULT_SUCCESS == result) {

        sprintf(ch, "\r\n");
        daves_putU1(ch, strlen(ch));
        sprintf(ch, "\r\nContent of %s\r\n", searchRecord.shortFileName);
        daves_putU1(ch, strlen(ch));

        result = FILEIO_Open(&file, searchRecord.shortFileName, FILEIO_OPEN_READ);

        if (FILEIO_RESULT_SUCCESS == result) {

            while (false == FILEIO_Eof(&file)) {

                memset(ch, '\0', sizeof (ch));
                FILEIO_Read(ch, 1, FILEIO_EXPORT_BUFFER_SIZE-1, &file);
                daves_putU1(ch, strlen(ch));

            }
            FILEIO_Close(&file);
        }
        else {
            
            displayFileErr( searchRecord.shortFileName, false );
            
        }

        while (FILEIO_RESULT_SUCCESS == FILEIO_Find( "20??????.CSV", FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, false )) {

                sprintf(ch, "\r\n");
                daves_putU1(ch, strlen(ch));
                sprintf(ch, "\r\nContent of %s\r\n", searchRecord.shortFileName);
                daves_putU1(ch, strlen(ch));

            result = FILEIO_Open(&file, searchRecord.shortFileName, FILEIO_OPEN_READ);

            if (FILEIO_RESULT_SUCCESS == result) {

                while (false == FILEIO_Eof(&file)) {

                    memset(ch, '\0', sizeof (ch));
                    FILEIO_Read(ch, 1, FILEIO_EXPORT_BUFFER_SIZE-1, &file);
                    daves_putU1(ch, strlen(ch));
                    
                }
                FILEIO_Close(&file);
            }
            else {

                displayFileErr( searchRecord.shortFileName, false );

            }   

        }

    }
    else {

        displayFileErr( "20??????.CSV", false );
        
    }

    usbUnmountDrive( );
}

void displayIniFile() {
    
    FILEIO_SEARCH_RECORD searchRecord;
    int8_t result;
    FILEIO_OBJECT file;
    
    memset(ch, '\0', sizeof (ch));
    
    setLedsStatusColor( LED_USB_ACCESS );
                        
    usbMountDrive( );

    result = FILEIO_Find( "CONFIG.INI", FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, true );

    if (FILEIO_RESULT_SUCCESS == result) {

        sprintf(ch, "\r\n");
        daves_putU1(ch, strlen(ch));
        sprintf(ch, "\r\nContent of %s\r\n", searchRecord.shortFileName);
        daves_putU1(ch, strlen(ch));
                
        result = FILEIO_Open(&file, searchRecord.shortFileName, FILEIO_OPEN_READ);

        if (FILEIO_RESULT_SUCCESS == result) {

            while (false == FILEIO_Eof(&file)) {

                FILEIO_Read(ch, 1, 64, &file);

                daves_putU1(ch, strlen(ch));

                memset(ch, '\0', sizeof (ch));
            }
            
            FILEIO_Close(&file);
            
        }
        else {
            
            displayFileErr( searchRecord.shortFileName, false );
            
        }

    }
    else {
        
        displayFileErr( "CONFIG.INI", false );        
        
    }

    usbUnmountDrive( );
                        
}


void displayErrorsFile() {
    
    FILEIO_SEARCH_RECORD searchRecord;
    int8_t result;
    FILEIO_OBJECT file;
    
    memset(ch, '\0', sizeof (ch));
    
    setLedsStatusColor( LED_USB_ACCESS );
                        
    usbMountDrive( );

    result = FILEIO_Find( ERRORS_LOG_FILE, FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, true );

    if (FILEIO_RESULT_SUCCESS == result) {

        sprintf(ch, "\r\n");
        daves_putU1(ch, strlen(ch));
        sprintf(ch, "\r\nContent of %s\r\n", searchRecord.shortFileName);
        daves_putU1(ch, strlen(ch));

        result = FILEIO_Open(&file, searchRecord.shortFileName, FILEIO_OPEN_READ);

        if (FILEIO_RESULT_SUCCESS == result) {

            while (false == FILEIO_Eof(&file)) {

                FILEIO_Read(ch, 1, 64, &file);

                daves_putU1(ch, strlen(ch));

                memset(ch, '\0', sizeof (ch));
            }
            
            FILEIO_Close(&file);
            
        }
        else {
            
            displayFileErr( searchRecord.shortFileName, false );
            
        }

    }
    else {
        
        displayFileErr( ERRORS_LOG_FILE, false );
        
    }

    usbUnmountDrive( );
    
}


void daves_putU1(const char *buf, unsigned int num_to_transmit)
{
    /* https://www.microchip.com/forums/m1063767.aspx */
    /* https://www.microchip.com/forums/FindPost/1063897 */
    UART1_TRANSFER_STATUS status;
    unsigned int numBytes = 0;
     
    while (numBytes < num_to_transmit) {
        status = UART1_TransferStatusGet();
        if (status & UART1_TRANSFER_STATUS_TX_EMPTY) {
            numBytes += UART1_WriteBuffer((uint8_t *)(buf + numBytes), num_to_transmit - numBytes);
            if (numBytes < num_to_transmit) {
                continue;
            } else {
                break;
            }
        } else {
            continue;
        }
    }

} // End of daves_putU1
