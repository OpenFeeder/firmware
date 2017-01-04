/* 
 * File: app_usb.c
 * Author: OpenFeeder Team <https://github.com/orgs/OpenFeeder/people> 
 * Comments:
 * Revision history: 1
 */

#include "app.h"
#include "app_usb.h"


/* Interrupt handler for USB host. */
void __attribute__( ( interrupt, auto_psv ) ) _USB1Interrupt( )
{
    USB_HostInterruptHandler( );
}

/* Declare a FILEIO_DRIVE_CONFIG structure to describe which functions the File I/O library will use to communicate with the media */
const FILEIO_DRIVE_CONFIG gUSBDrive = {
                                       /* Function to initialize the I/O pins used by the driver */
                                       ( FILEIO_DRIVER_IOInitialize ) NULL,
                                       /* Function to detect that the media is inserted */
                                       ( FILEIO_DRIVER_MediaDetect ) USBHostMSDSCSIMediaDetect,
                                       /* Function to initialize the media */
                                       ( FILEIO_DRIVER_MediaInitialize ) USBHostMSDSCSIMediaInitialize,
                                       /* Function to de-initialize the media */
                                       ( FILEIO_DRIVER_MediaDeinitialize ) USBHostMSDSCSIMediaDeinitialize,
                                       /* Function to read a sector from the media */
                                       ( FILEIO_DRIVER_SectorRead ) USBHostMSDSCSISectorRead,
                                       /* Function to write a sector to the media */
                                       ( FILEIO_DRIVER_SectorWrite ) USBHostMSDSCSISectorWrite,
                                       /* Function to determine if the media is write-protected */
                                       ( FILEIO_DRIVER_WriteProtectStateGet ) USBHostMSDSCSIWriteProtectState
};


bool usbMountDrive( void )
{
    uint8_t success;
    FILEIO_ERROR_TYPE errF;
    //    char errMsg[200];

    /* Drive is already not mounted. */
    if ( appDataUsb.usbDriveStatus == USB_DRIVE_MOUNTED )
    {
        return USB_DRIVE_MOUNTED;
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
    printf( "Mounting drive" );
#endif

    if ( USBHostDeviceStatus( appDataUsb.deviceAddress ) == USB_DEVICE_SUSPENDED )
    {
        // http://www.microchip.com/forums/m582058.aspx
        // help_mla_usb.pdf => 1.4.2.1.1.14 USBHostResumeDevice Function            
        success = USBHostResumeDevice( appDataUsb.deviceAddress );
        if ( success != USB_SUCCESS )
        {
            if ( success == USB_UNKNOWN_DEVICE )
                printf( "Device not found\n" );
            else // USB_ILLEGAL_REQUEST
                printf( "Device cannot RESUME unless it is suspended\n" );
            appDataUsb.usbDriveStatus = USB_DRIVE_NOT_MOUNTED;
            return appDataUsb.usbDriveStatus;
        }
    }

    /* Attempt to mount the drive described by gUSBDrive as drive 'A'
     * The deviceAddress parameter describes the USB address of the device;
     * it is initialized by the application in the USB_ApplicationEventHandler
     * function when a new device is detected.
     */
    errF = FILEIO_DriveMount( 'A', &gUSBDrive, &appDataUsb.deviceAddress );
    if ( FILEIO_ERROR_NONE == errF )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
        printf( " - USB drive mounted\n" );
#endif
        appDataUsb.usbDriveStatus = USB_DRIVE_MOUNTED;
    }
    else
    {
        sprintf( appError.message, "Unable to mount drive (%u)", errF );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        appDataUsb.usbDriveStatus = USB_DRIVE_NOT_MOUNTED;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
        printf( " - USB drive not mounted %d\n", errF );
#endif
    }

    return appDataUsb.usbDriveStatus;
}


bool usbUnmountDrive( void )
{
    uint8_t success;

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
    printf( "UsbUnmountDrive" );
#endif

    /* Drive is already not mounted. */
    if ( appDataUsb.usbDriveStatus == USB_DRIVE_NOT_MOUNTED )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
        printf( " drive is already mounted\n" );
#endif
        return appDataUsb.usbDriveStatus;
    }

    /* Unmount the drive since it is no longer in use. */
    if ( FILEIO_RESULT_FAILURE == FILEIO_DriveUnmount( 'A' ) )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
        printf( " unable to unmount drive\n" );
#endif
        strcpy( appError.message, "Unable to unmount drive" );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        return USB_DRIVE_MOUNTED;
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
    printf( " - drive unmounted\n" );
#endif    
    appDataUsb.usbDriveStatus = USB_DRIVE_NOT_MOUNTED;

    // http://www.microchip.com/forums/m582058.aspx
    // help_mla_usb.pdf => 1.4.2.1.1.18 USBHostSuspendDevice Function            
    success = USBHostSuspendDevice( appDataUsb.deviceAddress ); /* now no interrupt occur */
    if ( success != USB_SUCCESS )
    {
        // TODO: Gestion des erreurs
        if ( success == USB_UNKNOWN_DEVICE )
        {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "Unable to suspend drive: USB unknown device\n" );
#endif    
            strcpy( appError.message, "Unable to suspend drive: USB unknown device" );
            appError.currentLineNumber = __LINE__;
            sprintf( appError.currentFileName, "%s", __FILE__ );
        }
        else /* USB_ILLEGAL_REQUEST */
        {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "Cannot suspend unless device is in normal run mode\n" );
#endif 
            strcpy( appError.message, "Cannot suspend unless device is in normal run mode" );
            appError.currentLineNumber = __LINE__;
            sprintf( appError.currentFileName, "%s", __FILE__ );
        }
        appDataUsb.usbDriveStatus = USB_DRIVE_MOUNTED;
    }

    return appDataUsb.usbDriveStatus;
}


bool USB_ApplicationEventHandler( uint8_t address, USB_EVENT event, void *data, uint32_t size )
{
    switch ( ( int ) event )
    {

        case EVENT_HUB_ATTACH:
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: EVENT_HUB_ATTACH\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_STALL:
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: EVENT_STALL\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_VBUS_SES_REQUEST:
            /* This means that the device was removed. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: EVENT_VBUS_SES_REQUEST\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_VBUS_OVERCURRENT:
            /* This means that the device was removed. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: EVENT_VBUS_OVERCURRENT\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_VBUS_REQUEST_POWER:
            /* The data pointer points to a byte that represents the amount of power
             * requested in mA, divided by two.  If the device wants too much power,
             * we reject it.
             */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: EVENT_VBUS_REQUEST_POWER - Port: %u - Current: %u (x2mA)\n", ( ( USB_VBUS_POWER_EVENT_DATA* ) data )->port, ( ( USB_VBUS_POWER_EVENT_DATA* ) data )->current );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_VBUS_RELEASE_POWER:
            /* This means that the device was removed. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: EVENT_VBUS_RELEASE_POWER - Port: %u\n", ( ( USB_VBUS_POWER_EVENT_DATA* ) data )->port );
#endif
            CMD_VDD_APP_V_USB_SetLow( );
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_VBUS_POWER_AVAILABLE:
            /* This means that the device was removed. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: EVENT_VBUS_POWER_AVAILABLE\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_UNSUPPORTED_DEVICE:
            /* Here are various other events that a user might want to handle
             * or be aware of.  In this demo we are not handling them so we
             * will just return true to allow the stack to move on from the error.
             */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: EVENT_UNSUPPORTED_DEVICE\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_CANNOT_ENUMERATE:
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: EVENT_CANNOT_ENUMERATE\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_CLIENT_INIT_ERROR:
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: EVENT_CLIENT_INIT_ERROR\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_OUT_OF_MEMORY:
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: EVENT_OUT_OF_MEMORY\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_UNSPECIFIED_ERROR:
            /* This should never be generated. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: EVENT_UNSPECIFIED_ERROR\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_OVERRIDE_CLIENT_DRIVER_SELECTION:
            /* This should never be generated. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: EVENT_OVERRIDE_CLIENT_DRIVER_SELECTION\n" );
#endif
            //            return false;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_1MS:
            /* This should never be generated. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: EVENT_1MS\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_ALT_INTERFACE:
            /* This should never be generated. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: EVENT_ALT_INTERFACE\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_HOLD_BEFORE_CONFIGURATION:
            /* This should never be generated. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: EVENT_HOLD_BEFORE_CONFIGURATION\n" );
#endif
            //            return false;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_MSD_ATTACH:
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: EVENT_MSD_ATTACH\n" );
#endif

            if ( false == appDataUsb.key_is_nedded )
            {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
                printf( "USB host suspend device\n" );
#endif
                USBHostSuspendDevice( appDataUsb.deviceAddress );
            }

            else
            {
                appDataUsb.getValidDeviceAdress = true;
                appDataUsb.deviceAddress = address;
            }
            //            USBHostShutdown();
            //            UsbMountDrive();
            break;
            /* -------------------------------------------------------------- */

        default:
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "USB event: ??????? (%d)\n", ( int ) event );
#endif
            break;
    }

    return false;
}


/*******************************************************************************
 End of File
 */
