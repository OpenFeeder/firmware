
/**
  OC5 Generated Driver API Source File

  @Company
    Microchip Technology Inc.

  @File Name
    oc5.c

  @Summary
    This is the generated source file for the OC5 driver using MPLAB(c) Code Configurator

  @Description
    This source file provides APIs for driver for OC5.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - pic24-dspic-pic32mm : v1.35
        Device            :  PIC24FJ256GB406
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.31
        MPLAB 	          :  MPLAB X 3.60
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
#include "oc5.h"

/** OC Mode.

  @Summary
    Defines the OC Mode.

  @Description
    This data type defines the OC Mode of operation.

*/

static uint16_t         gOC5Mode;

/**
  Section: Driver Interface
*/


void OC5_Initialize (void)
{
    // ENFLT0 disabled; ENFLT1 disabled; OCFLT2 disabled; ENFLT2 disabled; OCSIDL disabled; OCM Edge-Aligned PWM mode; OCFLT1 disabled; OCFLT0 disabled; OCTSEL TMR3; TRIGMODE Only Software; 
    OC5CON1 = 0x0406;
    // SYNCSEL TMR3; TRIGSTAT disabled; DCB Start of instruction cycle; OCINV disabled; OCTRIG Sync; OC32 disabled; FLTOUT disabled; OCTRIS disabled; FLTMD Cycle; FLTTRIEN disabled; 
    OC5CON2 = 0x000D;
    // OC5RS 0; 
    OC5RS = 0x0000;
    // OC5R 0; 
	OC5R = 0x0000;
    
    gOC5Mode = OC5CON1bits.OCM;
}



void OC5_Tasks( void )
{
    if(IFS2bits.OC5IF)
    {
        IFS2bits.OC5IF = 0;
    }
}



void OC5_Start( void )
{
    OC5CON1bits.OCM = gOC5Mode;
}


void OC5_Stop( void )
{
    OC5CON1bits.OCM = 0;
}


void OC5_SingleCompareValueSet( uint16_t value )
{
    OC5R = value;
}


void OC5_DualCompareValueSet( uint16_t priVal, uint16_t secVal )
{
    OC5R = priVal;
	
    OC5RS = secVal;
}


void OC5_CentreAlignedPWMConfig( uint16_t priVal, uint16_t secVal )
{
    OC5R = priVal;
	
    OC5RS = secVal;
}

void OC5_EdgeAlignedPWMConfig( uint16_t priVal, uint16_t secVal )
{
    OC5R = priVal;
	
    OC5RS = secVal;
}

void OC5_SecondaryValueSet( uint16_t secVal )
{
   
    OC5RS = secVal;
}


void OC5_PrimaryValueSet( uint16_t priVal )
{
   
    OC5R = priVal;
}

bool OC5_IsCompareCycleComplete( void )
{
    return(IFS2bits.OC5IF);
}


bool OC5_FaultStatusGet( OC5_FAULTS faultNum )
{
    bool status;
    /* Return the status of the fault condition */
   
    switch(faultNum)
    { 
        case OC5_FAULT0:status = OC5CON1bits.OCFLT0;
            break;
        case OC5_FAULT1:status = OC5CON1bits.OCFLT1;
            break;
        case OC5_FAULT2:status = OC5CON1bits.OCFLT2;
            break;
        default :
            break;

    }
    return(status);
}


void OC5_FaultStatusClear( OC5_FAULTS faultNum )
{
    
    switch(faultNum)
    { 
        case OC5_FAULT0:OC5CON1bits.OCFLT0 = 0;
                break;
        case OC5_FAULT1:OC5CON1bits.OCFLT1 = 0;
                break;
        case OC5_FAULT2:OC5CON1bits.OCFLT2 = 0;
                break;
        default :
                break;
    }    
}


void OC5_ManualTriggerSet( void )
{
    OC5CON2bits.TRIGSTAT= true; 
}

bool OC5_TriggerStatusGet( void )
{
    return( OC5CON2bits.TRIGSTAT );
}


void OC5_TriggerStatusClear( void )
{
    /* Clears the trigger status */
    OC5CON2bits.TRIGSTAT = 0;
}

/**
 End of File
*/
