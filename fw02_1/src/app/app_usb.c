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
             
    /* Drive is already mounted. */
    if ( USB_DRIVE_MOUNTED == appDataUsb.usbDriveStatus )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
        printf( "\tUSB drive already mounted\n" );
#endif
        if ( true == appDataLog.log_events )
        {
           store_event(OF_ALREADY_MOUNTED_USB_DRIVE); 
        }
        
        return USB_DRIVE_MOUNTED;
    }

    if ( USB_DEVICE_SUSPENDED == USBHostDeviceStatus( appDataUsb.deviceAddress ) )
    {
        // http://www.microchip.com/forums/m582058.aspx
        // help_mla_usb.pdf => 1.4.2.1.1.14 USBHostResumeDevice Function            
        success = USBHostResumeDevice( appDataUsb.deviceAddress );
        if ( USB_SUCCESS != success )
        {
            if ( USB_UNKNOWN_DEVICE == success )
            {
                sprintf( appError.message, "Device not found" );
            }
            else // USB_ILLEGAL_REQUEST
            {
                sprintf( appError.message, "Device cannot RESUME unless it is suspended" );
            }
            appError.currentLineNumber = __LINE__;
            sprintf( appError.currentFileName, "%s", __FILE__ );
            appError.number = ERROR_USB_MOUNT_DRIVE;

            appDataUsb.usbDriveStatus = ERROR_USB_RESUME_DEVICE;            
            return appDataUsb.usbDriveStatus;
        }
        else
        {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB device resumed\n" );
#endif 
            if ( true == appDataLog.log_events )
            {
               store_event(OF_RESUME_USB_DRIVE); 
            }  
        }
    }
    
    /* Attempt to mount the drive described by gUSBDrive as drive 'A'
     * The deviceAddress parameter describes the USB address of the device;
     * it is initialized by the application in the USB_ApplicationEventHandler
     * function when a new device is detected.
     */
    errF = FILEIO_DriveMount( 'A', &gUSBDrive, &appDataUsb.deviceAddress );
    Nop( );
    Nop( );
    Nop( );
    if ( FILEIO_ERROR_NONE != errF )
    {
        sprintf( appError.message, "Unable to mount drive (%u)", errF );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        appError.number = ERROR_USB_MOUNT_DRIVE;
        
        appDataUsb.usbDriveStatus = USB_DRIVE_NOT_MOUNTED;        
        return appDataUsb.usbDriveStatus;
    }
    
    appDataUsb.usbDriveStatus = USB_DRIVE_MOUNTED;
    
    if ( true == appDataLog.log_events )
    {
       store_event(OF_MOUNT_USB_DRIVE); 
    }
    
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
    printf( "\tUSB drive mounted\n" );        
#endif
        
    return appDataUsb.usbDriveStatus;
}

bool usbUnmountDrive( void )
{
    uint8_t success;
    
    /* Drive is already not mounted. */
    if ( USB_DRIVE_NOT_MOUNTED == appDataUsb.usbDriveStatus )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
        printf( "\tUSB drive already unmounted\n" );
#endif
        if ( true == appDataLog.log_events )
        {
           store_event(OF_ALREADY_UNMOUNTED_USB_DRIVE); 
        }
        
        return appDataUsb.usbDriveStatus;
    }

    /* Unmount the drive since it is no longer in use. */
    if ( FILEIO_RESULT_FAILURE == FILEIO_DriveUnmount( 'A' ) )
    {
        strcpy( appError.message, "Unable to unmount drive" );
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        appError.number = ERROR_USB_UNMOUNT_DRIVE;
        
        appDataUsb.usbDriveStatus = USB_DRIVE_MOUNTED;
        return appDataUsb.usbDriveStatus;
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
    printf( "\tUSB drive unmounted\n" );
#endif    
    appDataUsb.usbDriveStatus = USB_DRIVE_NOT_MOUNTED;

    if ( true == appDataLog.log_events )
    {
       store_event(OF_UNMOUNT_USB_DRIVE); 
    }
    
    // http://www.microchip.com/forums/m582058.aspx
    // help_mla_usb.pdf => 1.4.2.1.1.18 USBHostSuspendDevice Function            
    success = USBHostSuspendDevice( appDataUsb.deviceAddress ); /* now no interrupt occur */
    if ( USB_SUCCESS != success )
    {
        if ( USB_UNKNOWN_DEVICE == success )
        {
            strcpy( appError.message, "Unable to suspend drive: USB unknown device" );
        }
        else /* USB_ILLEGAL_REQUEST */
        {
            strcpy( appError.message, "Cannot suspend unless device is in normal run mode" );
        }
        appError.currentLineNumber = __LINE__;
        sprintf( appError.currentFileName, "%s", __FILE__ );
        appError.number = ERROR_USB_SUSPEND_DEVICE;
        appDataUsb.usbDriveStatus = USB_DRIVE_MOUNTED;
    }
    else
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
        printf( "\tUSB device suspended\n" );
#endif 
        if ( true == appDataLog.log_events )
        {
           store_event(OF_SUSPEND_USB_DRIVE); 
        } 
    }

    return appDataUsb.usbDriveStatus;
}

bool USB_ApplicationEventHandler( uint8_t address, USB_EVENT event, void *data, uint32_t size )
{
    
    uint8_t success;
    
    switch ( ( int ) event )
    {

        case EVENT_DETACH:
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_DETACH\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_RESUME:
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_RESUME\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */
        case EVENT_SUSPEND:
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_SUSPEND\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */
            
        case EVENT_HUB_ATTACH:
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_HUB_ATTACH\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_STALL:
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_STALL\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_VBUS_SES_REQUEST:
            /* This means that the device was removed. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_VBUS_SES_REQUEST\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_VBUS_OVERCURRENT:
            /* This means that the device was removed. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_VBUS_OVERCURRENT\n" );
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
            printf( "\tUSB: EVENT_VBUS_REQUEST_POWER - Port: %u - Current: %u (x2mA)\n", ( ( USB_VBUS_POWER_EVENT_DATA* ) data )->port, ( ( USB_VBUS_POWER_EVENT_DATA* ) data )->current );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_VBUS_RELEASE_POWER:
            /* This means that the device was removed. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_VBUS_RELEASE_POWER - Port: %u\n", ( ( USB_VBUS_POWER_EVENT_DATA* ) data )->port );
#endif
//            CMD_VDD_APP_V_USB_SetLow( ); // FIXME: EVENT_VBUS_RELEASE_POWER must change USB power to OFF
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_VBUS_POWER_AVAILABLE:
            /* This means that the device was removed. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_VBUS_POWER_AVAILABLE\n" );
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
            printf( "\tUSB: EVENT_UNSUPPORTED_DEVICE\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_CANNOT_ENUMERATE:
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_CANNOT_ENUMERATE\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_CLIENT_INIT_ERROR:
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_CLIENT_INIT_ERROR\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_OUT_OF_MEMORY:
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_OUT_OF_MEMORY\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_UNSPECIFIED_ERROR:
            /* This should never be generated. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_UNSPECIFIED_ERROR\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_OVERRIDE_CLIENT_DRIVER_SELECTION:
            /* This should never be generated. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_OVERRIDE_CLIENT_DRIVER_SELECTION\n" );
#endif
            //            return false;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_1MS:
            /* This should never be generated. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_1MS\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_ALT_INTERFACE:
            /* This should never be generated. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_ALT_INTERFACE\n" );
#endif
            return true;
            break;
            /* -------------------------------------------------------------- */

        case EVENT_HOLD_BEFORE_CONFIGURATION:
            /* This should never be generated. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_HOLD_BEFORE_CONFIGURATION\n" );
#endif
            break;
            /* -------------------------------------------------------------- */

        case EVENT_MSD_ATTACH:
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: EVENT_MSD_ATTACH\n" );
#endif
            appDataUsb.getValidDeviceAdress = true;
            appDataUsb.deviceAddress = address;

            if ( false == appDataUsb.key_is_nedded )
            {
                success = USBHostSuspendDevice( appDataUsb.deviceAddress );
                    
                if ( USB_SUCCESS != success )
                {
                    if ( USB_UNKNOWN_DEVICE == success )
                    {
                        printf( "\tUnable to suspend drive: USB unknown device\n" );
                    }
                    else /* USB_ILLEGAL_REQUEST */
                    {
                        printf( "\tCannot suspend unless device is in normal run mode\n" );
                    }
                }
                else
                {
            #if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
                    printf( "\tUSB drive suspended\n" );
            #endif 
                }

            }
            break;
            /* -------------------------------------------------------------- */

        default:
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
            printf( "\tUSB: ??????? (%d)\n", ( int ) event );
#endif
            break;
    }

    return false;
}


/*******************************************************************************
 End of File
 */
