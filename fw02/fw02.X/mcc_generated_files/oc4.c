
/**
  OC4 Generated Driver API Source File

  @Company
    Microchip Technology Inc.

  @File Name
    oc4.c

  @Summary
    This is the generated source file for the OC4 driver using MPLAB(c) Code Configurator

  @Description
    This source file provides APIs for driver for OC4.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - pic24-dspic-pic32mm : v1.25
        Device            :  PIC24FJ256GB406
        Driver Version    :  0.5
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.26
        MPLAB 	          :  MPLAB X 3.45
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
#include "oc4.h"

/** OC Mode.

  @Summary
    Defines the OC Mode.

  @Description
    This data type defines the OC Mode of operation.

*/

static uint16_t         gOC4Mode;

/**
  Section: Driver Interface
*/


void OC4_Initialize (void)
{
    // ENFLT0 disabled; ENFLT1 disabled; OCFLT2 disabled; ENFLT2 disabled; OCSIDL disabled; OCM Edge-Aligned PWM mode; OCFLT1 disabled; OCFLT0 disabled; OCTSEL TMR4; TRIGMODE Hardware or Software; 
    OC4CON1 = 0x080E;
    // SYNCSEL Self; TRIGSTAT disabled; DCB Start of instruction cycle; OCINV disabled; OCTRIG Sync; OC32 disabled; FLTOUT disabled; OCTRIS disabled; FLTMD Cycle; FLTTRIEN disabled; 
    OC4CON2 = 0x001F;
    // OC4RS 209; 
    OC4RS = 0x00D1;
    // OC4R 105; 
    OC4R = 0x0069;
	
    gOC4Mode = OC4CON1bits.OCM;
}



void OC4_Tasks( void )
{
    if(IFS1bits.OC4IF)
    {
        IFS1bits.OC4IF = 0;
    }
}



void OC4_Start( void )
{
    OC4CON1bits.OCM = gOC4Mode;
}


void OC4_Stop( void )
{
    OC4CON1bits.OCM = 0;
}


void OC4_SingleCompareValueSet( uint16_t value )
{
    OC4R = value;
}


void OC4_DualCompareValueSet( uint16_t priVal, uint16_t secVal )
{
    OC4R = priVal;
	
    OC4RS = secVal;
}


void OC4_CentreAlignedPWMConfig( uint16_t priVal, uint16_t secVal )
{
    OC4R = priVal;
	
    OC4RS = secVal;
}

void OC4_EdgeAlignedPWMConfig( uint16_t priVal, uint16_t secVal )
{
    OC4R = priVal;
	
    OC4RS = secVal;
}

void OC4_SecondaryValueSet( uint16_t secVal )
{
   
    OC4RS = secVal;
}


void OC4_PrimaryValueSet( uint16_t priVal )
{
   
    OC4R = priVal;
}

bool OC4_IsCompareCycleComplete( void )
{
    return(IFS1bits.OC4IF);
}


bool OC4_FaultStatusGet( OC4_FAULTS faultNum )
{
    bool status;
    /* Return the status of the fault condition */
   
    switch(faultNum)
    { 
        case OC4_FAULT0:status = OC4CON1bits.OCFLT0;
            break;
        case OC4_FAULT1:status = OC4CON1bits.OCFLT1;
            break;
        case OC4_FAULT2:status = OC4CON1bits.OCFLT2;
            break;
        default :
            break;

    }
    return(status);
}


void OC4_FaultStatusClear( OC4_FAULTS faultNum )
{
    
    switch(faultNum)
    { 
        case OC4_FAULT0:OC4CON1bits.OCFLT0 = 0;
                break;
        case OC4_FAULT1:OC4CON1bits.OCFLT1 = 0;
                break;
        case OC4_FAULT2:OC4CON1bits.OCFLT2 = 0;
                break;
        default :
                break;
    }    
}


void OC4_ManualTriggerSet( void )
{
    OC4CON2bits.TRIGSTAT= true; 
}

bool OC4_TriggerStatusGet( void )
{
    return( OC4CON2bits.TRIGSTAT );
}


void OC4_TriggerStatusClear( void )
{
    /* Clears the trigger status */
    OC4CON2bits.TRIGSTAT = 0;
}

/**
 End of File
*/
