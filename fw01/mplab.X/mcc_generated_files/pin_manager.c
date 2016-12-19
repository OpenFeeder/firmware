/**
  System Interrupts Generated Driver File 

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.c

  @Summary:
    This is the generated manager file for the MPLAB(c) Code Configurator device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description:
    This source file provides implementations for MPLAB(c) Code Configurator interrupts.
    Generation Information : 
        Product Revision  :  MPLAB(c) Code Configurator - 3.16
        Device            :  PIC24FJ128GB204
        Version           :  1.02
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.26
        MPLAB             :  MPLAB X 3.20

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/


/**
    Section: Includes
*/
#include <xc.h>
#include "pin_manager.h"

/**
    void PIN_MANAGER_Initialize(void)
*/
void PIN_MANAGER_Initialize(void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0780;
    LATB = 0x0000;
    LATC = 0x0100;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x000F;
    TRISB = 0xACC3;
    TRISC = 0x02D0;

    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    CNPD1 = 0x0000;
    CNPD2 = 0x0000;
    CNPD3 = 0x0000;
    CNPU1 = 0x0000;
    CNPU2 = 0x0000;
    CNPU3 = 0x0000;

    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000;
    ODCB = 0x0000;
    ODCC = 0x0000;

    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSA = 0x000F;
    ANSB = 0x0040;
    ANSC = 0x0000;

    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    RPINR0bits.INT1R = 0x0019;   //RC9->EXT_INT:INT1;
    RPOR8bits.RP16R = 0x000D;   //RC0->OC1:OC1;
    RPOR8bits.RP17R = 0x000E;   //RC1->OC2:OC2;
    RPINR18bits.U1RXR = 0x0014;   //RC4->UART1:U1RX;
    RPINR1bits.INT3R = 0x0017;   //RC7->EXT_INT:INT3;
    RPOR9bits.RP19R = 0x0010;   //RC3->OC4:OC4;
    RPOR9bits.RP18R = 0x000F;   //RC2->OC3:OC3;
    RPINR2bits.INT4R = 0x0016;   //RC6->EXT_INT:INT4;
    RPOR10bits.RP21R = 0x0003;   //RC5->UART1:U1TX;
    RPOR1bits.RP3R = 0x0011;   //RB3->OC5:OC5;

    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    /****************************************************************************
     * Interrupt On Change for group CNEN1 - any
     ***************************************************************************/
	CNEN1bits.CN11IE = 1; // Pin : RB15

    IEC1bits.CNIE = 1; // Enable CNI interrupt 
}

/* Interrupt service routine for the CNI interrupt. */
void __attribute__ (( interrupt, no_auto_psv )) _CNInterrupt ( void )
{
    if(IFS1bits.CNIF == 1)
    {
        // Clear the flag
        IFS1bits.CNIF = 0;
    }
}
