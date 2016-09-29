/**
  @Generated MPLAB(c) Code Configurator Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    mcc.h

  @Summary:
    This is the mcc.h file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 3.16
        Device            :  PIC24FJ128GB204
        Version           :  1.02
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.26
        MPLAB             :  MPLAB X 3.20
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

#ifndef MCC_H
#define	MCC_H
#include <xc.h>
#include "pin_manager.h"
#include <stdio.h>
#include <stdint.h>
//#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines false and true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <ctype.h>
#include "oc5.h"
#include "oc4.h"
#include "rtcc.h"
#include "traps.h"
#include "interrupt_manager.h"
#include "adc1.h"
#include "uart1.h"
#include "ext_int.h"
#include "i2c1.h"
#include "tmr4.h"
#include "oc1.h"
#include "tmr3.h"
#include "oc3.h"
#include "tmr2.h"
#include "oc2.h"
#include "led_status.h"

// XC8, XC16 delay routines taking a variable as argument
// http://www.microchip.com/forums/m735322.aspx
// http://ww1.microchip.com/downloads/en/DeviceDoc/50001456J.pdf
// __delay_ms --> DS50001456J-page 206
// #define _XTAL_FREQ  16000000UL     // mcc_generated_files\mcc.h
// #define FCK         16000000ULL    // clock frequency
#define _XTAL_FREQ  16000000UL
#define FCY             8000000ULL        // 2 Clocks--> 1 Instruction cycle --> FCY = FCK/2
#include <libpic30.h>

//extern volatile uint16_t servo_position; // servomotor position


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Initializes the device to the default states configured in the
 *                  MCC GUI
 * @Example
    SYSTEM_Initialize(void);
 */
void SYSTEM_Initialize(void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Initializes the oscillator to the default states configured in the
 *                  MCC GUI
 * @Example
    OSCILLATOR_Initialize(void);
 */
void OSCILLATOR_Initialize(void);


#endif	/* MCC_H */
/**
 End of File
*/