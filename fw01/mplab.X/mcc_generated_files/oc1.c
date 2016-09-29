
/**
  OC1 Generated Driver API Source File

  @Company
    Microchip Technology Inc.

  @File Name
    oc1.c

  @Summary
    This is the generated source file for the OC1 driver using MPLAB(c) Code Configurator

  @Description
    This source file provides APIs for driver for OC1.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 3.16
        Device            :  PIC24FJ128GB204
        Driver Version    :  0.5
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.26
        MPLAB 	          :  MPLAB X 3.20
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include <xc.h>
#include "oc1.h"

/** OC Mode.

  @Summary
    Defines the OC Mode.

  @Description
    This data type defines the OC Mode of operation.

*/

static uint16_t         gOC1Mode;

/**
  Section: Driver Interface
*/


void OC1_Initialize (void)
{
    // ENFLT0 disabled; ENFLT1 disabled; OCFLT2 disabled; ENFLT2 disabled; OCSIDL disabled; OCM Edge-Aligned PWM mode; OCFLT1 disabled; OCFLT0 disabled; OCTSEL TMR2; TRIGMODE Only Software; 
    OC1CON1 = 0x0006;
    // SYNCSEL TMR2; TRIGSTAT disabled; DCB Start of instruction cycle; OCINV disabled; OCTRIG Sync; OC32 disabled; FLTOUT disabled; OCTRIS disabled; FLTMD Cycle; FLTTRIEN disabled; 
    OC1CON2 = 0x000C;
    // OC1RS 0; 
    OC1RS = 0x0000;
    // OC1R 64; 
    //OC1R = 0x0040;
    OC1R = 0;
	
    gOC1Mode = OC1CON1bits.OCM;
}



void OC1_Tasks( void )
{
    if(IFS0bits.OC1IF)
    {
        IFS0bits.OC1IF = 0;
    }
}



void OC1_Start( void )
{
    OC1CON1bits.OCM = gOC1Mode;
}


void OC1_Stop( void )
{
    OC1CON1bits.OCM = 0;
}


void OC1_SingleCompareValueSet( uint16_t value )
{
    OC1R = value;
}


void OC1_DualCompareValueSet( uint16_t priVal, uint16_t secVal )
{
    OC1R = priVal;
	
    OC1RS = secVal;
}


void OC1_CentreAlignedPWMConfig( uint16_t priVal, uint16_t secVal )
{
    OC1R = priVal;
	
    OC1RS = secVal;
}


void OC1_EdgeAlignedPWMConfig( uint16_t priVal, uint16_t secVal )
{
    OC1R = priVal;
	
    OC1RS = secVal;
}

bool OC1_IsCompareCycleComplete( void )
{
    return(IFS0bits.OC1IF);
}


bool OC1_FaultStatusGet( OC1_FAULTS faultNum )
{
    bool status;
    /* Return the status of the fault condition */
   
    switch(faultNum)
    { 
        case OC1_FAULT0:status = OC1CON1bits.OCFLT0;
            break;
        case OC1_FAULT1:status = OC1CON1bits.OCFLT1;
            break;
        case OC1_FAULT2:status = OC1CON1bits.OCFLT2;
            break;
        default :
            break;

    }
    return(status);
}


void OC1_FaultStatusClear( OC1_FAULTS faultNum )
{
    
    switch(faultNum)
    { 
        case OC1_FAULT0:OC1CON1bits.OCFLT0 = 0;
                break;
        case OC1_FAULT1:OC1CON1bits.OCFLT1 = 0;
                break;
        case OC1_FAULT2:OC1CON1bits.OCFLT2 = 0;
                break;
        default :
                break;
    }    
}


void OC1_ManualTriggerSet( void )
{
    OC1CON2bits.TRIGSTAT= true; 
}

bool OC1_TriggerStatusGet( void )
{
    return( OC1CON2bits.TRIGSTAT );
}


void OC1_TriggerStatusClear( void )
{
    /* Clears the trigger status */
    OC1CON2bits.TRIGSTAT = 0;
}

/**
 End of File
*/
