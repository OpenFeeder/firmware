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
static char tx_buffer[FILEIO_BUFFER_SIZE];
static char rx_buffer[FILEIO_BUFFER_SIZE];


void listFilesOnUsbDevice( )
{

    FILEIO_SEARCH_RECORD searchRecord;
    int8_t result;

    setLedsStatusColor( LED_USB_ACCESS );

    usbMountDrive( );

    result = FILEIO_Find( "*.*", FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, true );

    if ( FILEIO_RESULT_SUCCESS == result )
    {

        sprintf( tx_buffer, "\t%s", searchRecord.shortFileName );
        write_on_UART1( tx_buffer, strlen( tx_buffer ) );
        sprintf( tx_buffer, " (%ld bytes)\r\n", searchRecord.fileSize );
        write_on_UART1( tx_buffer, strlen( tx_buffer ) );

        while ( FILEIO_RESULT_SUCCESS == FILEIO_Find( "*.*", FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, false ) )
        {

            sprintf( tx_buffer, "\t%s", searchRecord.shortFileName );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );
            sprintf( tx_buffer, " (%ld bytes)\r\n", searchRecord.fileSize );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );

        }

    }
    else
    {

        displayFileErr( ( char * ) searchRecord.shortFileName, false );

    }

    usbUnmountDrive( );

    setLedsStatusColor( LEDS_OFF );

}


void exportFile( const char * filename, bool fileType )
{

    FILEIO_SEARCH_RECORD searchRecord;
    int8_t result;
    FILEIO_OBJECT file;
    size_t num_read;

    memset( tx_buffer, '\0', sizeof (tx_buffer ) );

    result = FILEIO_Find( filename, FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, true );

    if ( FILEIO_RESULT_SUCCESS == result )
    {

        sprintf( tx_buffer, "%c%c%s%c", ack, enq, searchRecord.shortFileName, dc4 );
        write_on_UART1( tx_buffer, strlen( tx_buffer ) );
        sprintf( tx_buffer, "%c%c%c%c", ack, enq, fileType, dc4 );
        write_on_UART1( tx_buffer, strlen( tx_buffer ) );
        sprintf( tx_buffer, "%c%c%ld%c", ack, enq, searchRecord.fileSize, dc4 );
        write_on_UART1( tx_buffer, strlen( tx_buffer ) );

        result = FILEIO_Open( &file, ( char * ) searchRecord.shortFileName, FILEIO_OPEN_READ );

        if ( FILEIO_RESULT_SUCCESS == result )
        {

            sprintf( tx_buffer, "%c%c", ack, enq );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );

            while ( false == FILEIO_Eof( &file ) )
            {

                if ( 'T' == fileType )
                {
                    memset( tx_buffer, '\0', sizeof (tx_buffer ) );
                    FILEIO_Read( tx_buffer, 1, FILEIO_BUFFER_SIZE - 1, &file );
                    write_on_UART1( tx_buffer, strlen( tx_buffer ) );
                }
                else
                {
                    num_read = FILEIO_Read( tx_buffer, 1, FILEIO_BUFFER_SIZE, &file );
                    write_on_UART1( tx_buffer, num_read );
                }

            }

            FILEIO_Close( &file );

            sprintf( tx_buffer, "%c", dc4 );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );

        }
        else
        {

            displayFileErr( ( char * ) searchRecord.shortFileName, true );

        }

        while ( FILEIO_RESULT_SUCCESS == FILEIO_Find( filename, FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, false ) )
        {

            sprintf( tx_buffer, "%c%c%s%c", ack, enq, searchRecord.shortFileName, dc4 );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );

            sprintf( tx_buffer, "%c%c%c%c", ack, enq, fileType, dc4 );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );

            sprintf( tx_buffer, "%c%c%ld%c", ack, enq, searchRecord.fileSize, dc4 );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );

            result = FILEIO_Open( &file, ( char * ) searchRecord.shortFileName, FILEIO_OPEN_READ );

            if ( FILEIO_RESULT_SUCCESS == result )
            {

                sprintf( tx_buffer, "%c%c", ack, enq );
                write_on_UART1( tx_buffer, strlen( tx_buffer ) );

                while ( false == FILEIO_Eof( &file ) )
                {

                    if ( 'T' == fileType )
                    {
                        memset( tx_buffer, '\0', sizeof (tx_buffer ) );
                        FILEIO_Read( tx_buffer, 1, FILEIO_BUFFER_SIZE - 1, &file );
                        write_on_UART1( tx_buffer, strlen( tx_buffer ) );
                    }
                    else
                    {
                        num_read = FILEIO_Read( tx_buffer, 1, FILEIO_BUFFER_SIZE, &file );
                        write_on_UART1( tx_buffer, num_read );
                    }

                }

                FILEIO_Close( &file );

                sprintf( tx_buffer, "%c", dc4 );
                write_on_UART1( tx_buffer, strlen( tx_buffer ) );

            }
            else
            {

                displayFileErr( ( char * ) searchRecord.shortFileName, true );

            }
        }
        sprintf( tx_buffer, "%c", nack );
        write_on_UART1( tx_buffer, strlen( tx_buffer ) );
    }
}


void exportAllFiles( )
{

    setLedsStatusColor( LED_USB_ACCESS );

    usbMountDrive( );

    sprintf( tx_buffer, "%c", stx );
    write_on_UART1( tx_buffer, strlen( tx_buffer ) );

    exportFile( "20??????.CSV", 'T' );
    exportFile( "EV??????.BIN", 'B' );
    exportFile( "CONFIG.INI", 'T' );
    exportFile( FIRMWARE_LOG_FILE, 'T' );
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

    sprintf( tx_buffer, "%c", etx );
    write_on_UART1( tx_buffer, strlen( tx_buffer ) );

    usbUnmountDrive( );

    setLedsStatusColor( LEDS_OFF );
}

void deleteCsvFiles( )
{
    setLedsStatusColor( LED_USB_ACCESS );

    usbMountDrive( );
    
    deleteFile( "20??????.CSV" );

    usbUnmountDrive( );

    setLedsStatusColor( LEDS_OFF );
}

void deleteLogFiles( )
{
    setLedsStatusColor( LED_USB_ACCESS );

    usbMountDrive( );
    
    deleteFile( FIRMWARE_LOG_FILE );
    deleteFile( BATTERY_LOG_FILE );
    deleteFile( ERRORS_LOG_FILE );
    deleteFile( RFID_LOG_FILE );
    deleteFile( CALIBRATION_LOG_FILE );
    deleteFile( UDID_LOG_FILE );
    deleteFile( EXT_TEMP_LOG_FILE );

    usbUnmountDrive( );

    setLedsStatusColor( LEDS_OFF );
}

void deleteEventFiles( )
{
    setLedsStatusColor( LED_USB_ACCESS );

    usbMountDrive( );
    
    deleteFile( "EV??????.BIN" );

    usbUnmountDrive( );

    setLedsStatusColor( LEDS_OFF );
}

void deleteAllFiles( )
{
    setLedsStatusColor( LED_USB_ACCESS );

    usbMountDrive( );
    
    deleteFile( "20??????.CSV" );
    deleteFile( "EV??????.BIN" );
    deleteFile( "CONFIG.INI" );
    deleteFile( FIRMWARE_LOG_FILE );
    deleteFile( BATTERY_LOG_FILE );
    deleteFile( ERRORS_LOG_FILE );
    deleteFile( RFID_LOG_FILE );
    deleteFile( CALIBRATION_LOG_FILE );
    deleteFile( UDID_LOG_FILE );
    deleteFile( EXT_TEMP_LOG_FILE );
    deleteFile( "UDIDLEDS.TXT" );
    deleteFile( "PTLEFT.TXT" );
    deleteFile( "PTRIGHT.TXT" );
    deleteFile( "PTTOP.TXT" );
    deleteFile( "PTBOTTOM.TXT" );
    deleteFile( "PTONE1.TXT" );
    deleteFile( "PTONE2.TXT" );
    deleteFile( "PTONE3.TXT" );
    deleteFile( "PTONE4.TXT" );
    deleteFile( "PTACCEPT.TXT" );
    deleteFile( "PTDENIED.TXT" );
    deleteFile( "PTCOLORA.TXT" );
    deleteFile( "PTCOLORB.TXT" );
    deleteFile( "PROBA.TXT" );

    usbUnmountDrive( );

    setLedsStatusColor( LEDS_OFF );
}

void deleteConfigurationFiles( )
{
    setLedsStatusColor( LED_USB_ACCESS );

    usbMountDrive( );

    deleteFile( "CONFIG.INI" );
    deleteFile( "UDIDLEDS.TXT" );
    deleteFile( "PTLEFT.TXT" );
    deleteFile( "PTRIGHT.TXT" );
    deleteFile( "PTTOP.TXT" );
    deleteFile( "PTBOTTOM.TXT" );
    deleteFile( "PTONE1.TXT" );
    deleteFile( "PTONE2.TXT" );
    deleteFile( "PTONE3.TXT" );
    deleteFile( "PTONE4.TXT" );
    deleteFile( "PTACCEPT.TXT" );
    deleteFile( "PTDENIED.TXT" );
    deleteFile( "PTCOLORA.TXT" );
    deleteFile( "PTCOLORB.TXT" );
    deleteFile( "PROBA.TXT" );

    usbUnmountDrive( );

    setLedsStatusColor( LEDS_OFF );
}

void deleteFile( const char * filename )
{

    FILEIO_SEARCH_RECORD searchRecord;

    if ( FILEIO_RESULT_SUCCESS == FILEIO_Find( filename, FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, true ) )
    {
        if ( FILEIO_RESULT_SUCCESS != FILEIO_Remove(( char * ) searchRecord.shortFileName) )
        {
            displayFileErr( ( char * ) searchRecord.shortFileName, false );
        }
        else
        {
            sprintf( tx_buffer, "\t%s deleted.\r\n", searchRecord.shortFileName );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );
        }

        while ( FILEIO_RESULT_SUCCESS == FILEIO_Find( filename, FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, false ) )
        {

            if ( FILEIO_RESULT_SUCCESS != FILEIO_Remove(( char * ) searchRecord.shortFileName) )
            {
                displayFileErr( ( char * ) searchRecord.shortFileName, false );
            }
            else
            {
                sprintf( tx_buffer, "\t%s deleted.\r\n", searchRecord.shortFileName );
                write_on_UART1( tx_buffer, strlen( tx_buffer ) );
            }
        }
    }
}

FILEIO_RESULT importFiles( )
{
    
    /* MLA FILEIO filename question 
     https://www.microchip.com/forums/m824637.aspx 
     * https://www.microchip.com/forums/FindPost/824688
     */
    
    int i;
    int num_files = 0;
    int filename_size = 0;
    int num_bytes_send = 0;
    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    char myfile[13];
    const char *mypoint = &myfile[0];
    
    memset( rx_buffer, '\0', sizeof (rx_buffer ) );
    read_on_UART1(rx_buffer, 1);
    
    num_files = rx_buffer[0];

    setLedsStatusColor( LED_USB_ACCESS );

    usbMountDrive( );
    
    for (i = 0; i < num_files ; i++)
    {

        /* Read file name size */
        memset( rx_buffer, '\0', sizeof (rx_buffer ) );
        read_on_UART1(rx_buffer, 1);

        filename_size = rx_buffer[0];

        /* Read file name */
        memset( rx_buffer, '\0', sizeof (rx_buffer ) );
        read_on_UART1(rx_buffer, filename_size);

        sprintf( tx_buffer, "\t%c %s\r\n", filename_size, rx_buffer );
        write_on_UART1( tx_buffer, strlen( tx_buffer ) );
            
        strcpy(myfile, rx_buffer);
        
        memset( rx_buffer, '\0', sizeof (rx_buffer ) );
        read_on_UART1(rx_buffer, 1);
        
        if ( 1 == rx_buffer[0] )
        {

            if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, mypoint, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE ) )
            {
                errF = FILEIO_ErrorGet( 'A' );
                sprintf( appError.message, "Unable to open file (%u)", errF );
                appError.current_line_number = __LINE__;
                sprintf( appError.current_file_name, "%s", __FILE__ );
                FILEIO_ErrorClear( 'A' );
                appError.number = ERROR_LOG_FILE_OPEN;
                return FILEIO_RESULT_FAILURE;
            }
 
            /* Read number of char send */
            memset( rx_buffer, '\0', sizeof (rx_buffer ) );
            read_on_UART1(rx_buffer, 1);

            num_bytes_send = rx_buffer[0];

            while ( 0 != num_bytes_send )
            {
                memset( rx_buffer, '\0', sizeof (rx_buffer ) );
                read_on_UART1(rx_buffer, num_bytes_send);

                FILEIO_Write( rx_buffer, 1, num_bytes_send, &file );

                sprintf( tx_buffer, "%d", 1);
                write_on_UART1( tx_buffer, strlen( tx_buffer ) );

                /* Read number of char send */
                memset( rx_buffer, '\0', sizeof (rx_buffer ) );
                read_on_UART1(rx_buffer, 1);

                num_bytes_send = rx_buffer[0];
            }
            
            if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
            {
                errF = FILEIO_ErrorGet( 'A' );
                sprintf( appError.message, "Unable to close file (%u)", errF );
                appError.current_line_number = __LINE__;
                sprintf( appError.current_file_name, "%s", __FILE__ );
                FILEIO_ErrorClear( 'A' );
                appError.number = ERROR_LOG_FILE_CLOSE;
                return FILEIO_RESULT_FAILURE;
            }   
        
            sprintf( tx_buffer, "%d", 1);
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );
        }
        else
        {
            break;
        }


 

    }
    
    usbUnmountDrive( );

    setLedsStatusColor( LEDS_OFF );
    
    return FILEIO_RESULT_SUCCESS;
    
}

void updateConfigFiles( void )
{
    deleteFile( "CONFIG.INI" );
    deleteFile( "UDIDLEDS.CSV" );
    deleteFile( "PTLEFT.TXT" );
    deleteFile( "PTRIGHT.TXT" );
    deleteFile( "PTTOP.TXT" );
    deleteFile( "PTBOTTOM.TXT" );
    deleteFile( "PTONE1.TXT" );
    deleteFile( "PTONE2.TXT" );
    deleteFile( "PTONE3.TXT" );
    deleteFile( "PTONE4.TXT" );
    deleteFile( "PTACCEPT.TXT" );
    deleteFile( "PTDENIED.TXT" );
    deleteFile( "PTCOLORA.TXT" );
    deleteFile( "PTCOLORB.TXT" );
    deleteFile( "PROBA.TXT" );
}


void displayFileErr( const char * filename, bool export )
{

    FILEIO_ERROR_TYPE errF;

    if ( true == export )
    {
        sprintf( tx_buffer, "%c%c\t%s - ", nack, enq, filename );
        write_on_UART1( tx_buffer, strlen( tx_buffer ) );
    }

    errF = FILEIO_ErrorGet( 'A' );

    switch ( errF )
    {

        case FILEIO_ERROR_INVALID_ARGUMENT:
            sprintf( tx_buffer, "The path could not be resolved.\r\n" );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );
            break;
            
        case FILEIO_ERROR_WRITE_PROTECTED:
            sprintf( tx_buffer, "The device is write-protected.\r\n" );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );
            break;
            
        case FILEIO_ERROR_DELETE_DIR:
            sprintf( tx_buffer, "The file being deleted is actually a directory.\r\n" );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );
            break;
            
        case FILEIO_ERROR_ERASE_FAIL:
            sprintf( tx_buffer, "The erase operation failed.\r\n" );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );
            break;
            
        case FILEIO_ERROR_FILE_NOT_FOUND:
            sprintf( tx_buffer, "The file entries for this file are invalid or have already been erased.\r\n" );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );
            break;
            
        case FILEIO_ERROR_WRITE:
            sprintf( tx_buffer, "The updated file data and entry could not be written to the device.\r\n" );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );
            break;
            
        case FILEIO_ERROR_BAD_SECTOR_READ:
            sprintf( tx_buffer, "The directory entry could not be cached.\r\n" );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );
            break;

        case FILEIO_ERROR_INVALID_FILENAME:
            sprintf( tx_buffer, "The file name is invalid.\r\n" );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );
            break;

        case FILEIO_ERROR_BAD_CACHE_READ:
            sprintf( tx_buffer, "There was an error searching directory entries.\r\n" );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );
            break;

        case FILEIO_ERROR_DONE:
            sprintf( tx_buffer, "File not found.\r\n" );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );
            break;

        default:
            sprintf( tx_buffer, "Error (%d).\r\n", errF );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );
            break;

    }

    if ( true == export )
    {
        sprintf( tx_buffer, "%c", dc4 );
        write_on_UART1( tx_buffer, strlen( tx_buffer ) );
    }

}


void displayCsvFiles( )
{

    FILEIO_SEARCH_RECORD searchRecord;
    int8_t result;
    FILEIO_OBJECT file;

    memset( tx_buffer, '\0', sizeof (tx_buffer ) );

    setLedsStatusColor( LED_USB_ACCESS );

    usbMountDrive( );

    result = FILEIO_Find( "20??????.CSV", FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, true );

    if ( FILEIO_RESULT_SUCCESS == result )
    {

        sprintf( tx_buffer, "\r\n" );
        write_on_UART1( tx_buffer, strlen( tx_buffer ) );
        sprintf( tx_buffer, "\r\nContent of %s\r\n", searchRecord.shortFileName );
        write_on_UART1( tx_buffer, strlen( tx_buffer ) );

        result = FILEIO_Open( &file, ( char * ) searchRecord.shortFileName, FILEIO_OPEN_READ );

        if ( FILEIO_RESULT_SUCCESS == result )
        {

            while ( false == FILEIO_Eof( &file ) )
            {

                memset( tx_buffer, '\0', sizeof (tx_buffer ) );
                FILEIO_Read( tx_buffer, 1, FILEIO_BUFFER_SIZE - 1, &file );
                write_on_UART1( tx_buffer, strlen( tx_buffer ) );

            }
            FILEIO_Close( &file );
            sprintf( tx_buffer, "\r\n" );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );
        }
        else
        {

            displayFileErr( ( char * ) searchRecord.shortFileName, false );

        }

        while ( FILEIO_RESULT_SUCCESS == FILEIO_Find( "20??????.CSV", FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, false ) )
        {

            sprintf( tx_buffer, "\r\n" );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );
            sprintf( tx_buffer, "\r\nContent of %s\r\n", searchRecord.shortFileName );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );

            result = FILEIO_Open( &file, ( char * ) searchRecord.shortFileName, FILEIO_OPEN_READ );

            if ( FILEIO_RESULT_SUCCESS == result )
            {

                while ( false == FILEIO_Eof( &file ) )
                {

                    memset( tx_buffer, '\0', sizeof (tx_buffer ) );
                    FILEIO_Read( tx_buffer, 1, FILEIO_BUFFER_SIZE - 1, &file );
                    write_on_UART1( tx_buffer, strlen( tx_buffer ) );

                }
                FILEIO_Close( &file );
                sprintf( tx_buffer, "\r\n" );
                write_on_UART1( tx_buffer, strlen( tx_buffer ) );
            }
            else
            {

                displayFileErr( ( char * ) searchRecord.shortFileName, false );

            }

        }

    }
    else
    {

        displayFileErr( "20??????.CSV", false );

    }

    usbUnmountDrive( );

    setLedsStatusColor( LEDS_OFF );
}


void displayIniFile( )
{

    FILEIO_SEARCH_RECORD searchRecord;
    int8_t result;
    FILEIO_OBJECT file;

    memset( tx_buffer, '\0', sizeof (tx_buffer ) );

    setLedsStatusColor( LED_USB_ACCESS );

    usbMountDrive( );

    result = FILEIO_Find( "CONFIG.INI", FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, true );

    if ( FILEIO_RESULT_SUCCESS == result )
    {

        sprintf( tx_buffer, "\r\n" );
        write_on_UART1( tx_buffer, strlen( tx_buffer ) );
        sprintf( tx_buffer, "\r\nContent of %s\r\n", searchRecord.shortFileName );
        write_on_UART1( tx_buffer, strlen( tx_buffer ) );

        result = FILEIO_Open( &file, ( char * ) searchRecord.shortFileName, FILEIO_OPEN_READ );

        if ( FILEIO_RESULT_SUCCESS == result )
        {

            while ( false == FILEIO_Eof( &file ) )
            {

                FILEIO_Read( tx_buffer, 1, 64, &file );

                write_on_UART1( tx_buffer, strlen( tx_buffer ) );

                memset( tx_buffer, '\0', sizeof (tx_buffer ) );
            }

            FILEIO_Close( &file );
            sprintf( tx_buffer, "\r\n" );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );

        }
        else
        {

            displayFileErr( ( char * ) searchRecord.shortFileName, false );

        }

    }
    else
    {

        displayFileErr( "CONFIG.INI", false );

    }

    usbUnmountDrive( );

    setLedsStatusColor( LEDS_OFF );

}


void displayErrorsFile( )
{

    FILEIO_SEARCH_RECORD searchRecord;
    int8_t result;
    FILEIO_OBJECT file;

    memset( tx_buffer, '\0', sizeof (tx_buffer ) );

    setLedsStatusColor( LED_USB_ACCESS );

    usbMountDrive( );

    result = FILEIO_Find( ERRORS_LOG_FILE, FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, true );

    if ( FILEIO_RESULT_SUCCESS == result )
    {

        sprintf( tx_buffer, "\r\n" );
        write_on_UART1( tx_buffer, strlen( tx_buffer ) );
        sprintf( tx_buffer, "\r\nContent of %s\r\n", searchRecord.shortFileName );
        write_on_UART1( tx_buffer, strlen( tx_buffer ) );

        result = FILEIO_Open( &file, ( char * ) searchRecord.shortFileName, FILEIO_OPEN_READ );

        if ( FILEIO_RESULT_SUCCESS == result )
        {

            while ( false == FILEIO_Eof( &file ) )
            {

                FILEIO_Read( tx_buffer, 1, 64, &file );

                write_on_UART1( tx_buffer, strlen( tx_buffer ) );

                memset( tx_buffer, '\0', sizeof (tx_buffer ) );
            }

            FILEIO_Close( &file );
            sprintf( tx_buffer, "\r\n" );
            write_on_UART1( tx_buffer, strlen( tx_buffer ) );

        }
        else
        {

            displayFileErr( ( char * ) searchRecord.shortFileName, false );

        }

    }
    else
    {

        displayFileErr( ERRORS_LOG_FILE, false );

    }

    usbUnmountDrive( );

    setLedsStatusColor( LEDS_OFF );

}


void write_on_UART1( const char *buf, unsigned int num_to_transmit )
{
    /* https://www.microchip.com/forums/m1063767.aspx */
    /* https://www.microchip.com/forums/FindPost/1063897 */
    UART1_TRANSFER_STATUS status;
    unsigned int numBytes = 0;

    while ( numBytes < num_to_transmit )
    {
        status = UART1_TransferStatusGet( );
        if ( status & UART1_TRANSFER_STATUS_TX_EMPTY )
        {
            numBytes += UART1_WriteBuffer( ( uint8_t * ) ( buf + numBytes ), num_to_transmit - numBytes );
            if ( numBytes < num_to_transmit )
            {
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            continue;
        }
    }

} // End of write_on_UART1

void read_on_UART1( char *buf, unsigned int num_to_read )
{
    UART1_TRANSFER_STATUS status;
    unsigned int numBytes = 0;

    while( numBytes < num_to_read)
    {
        status = UART1_TransferStatusGet ( ) ;
        if (status & UART1_TRANSFER_STATUS_RX_DATA_PRESENT)
//        if (status & UART1_TRANSFER_STATUS_RX_FULL)
        {
            numBytes += UART1_ReadBuffer( ( uint8_t * ) ( buf + numBytes ), num_to_read - numBytes )  ;
            if(numBytes < num_to_read)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            continue;
        }

        // Do something else...
    }

} // End of write_on_UART1

