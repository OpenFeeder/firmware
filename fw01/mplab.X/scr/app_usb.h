/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File: app_usb.h
 * Author: OpenFeeder Team <https://github.com/orgs/OpenFeeder/people> 
 * Comments:
 * Revision history: 1
 */

#ifndef APP_USB_H
#define	APP_USB_H

#include "usb.h"
#include "usb_common.h"
#include "usb_host_msd.h"


#define USB_DRIVE_NOT_MOUNTED false
#define USB_DRIVE_MOUNTED true


/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    bool getValidDeviceAdress;
    uint8_t deviceAddress;
    bool usbDriveStatus;
    bool key_is_nedded;

} APP_DATA_USB;


// *****************************************************************************
// *****************************************************************************
// Section: extern declarations
// *****************************************************************************
// *****************************************************************************
bool usbMountDrive( void );
bool usbUnmountDrive( void );

/*******************************************************************************
  Function: USB_ApplicationEventHandler( )
 
  Overview: Application Event Handler

  PreCondition: ...
 
  Input: address - Address of the USB device to mount
 
  Output: None
 */
bool USB_ApplicationEventHandler( uint8_t address, USB_EVENT event, void *data, uint32_t size );


/*******************************************************************************
  Function: APP_MountDrive( );
 
  Overview: Initializes the demo code

  PreCondition: ...
 
  Input: address - Address of the USB device to mount
 
  Output: None
 */
void APP_MountDrive( uint8_t address );


#endif	/* APP_USB_H */
/*******************************************************************************
 End of File
 */
