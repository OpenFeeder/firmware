/**
  EXT_INT Generated Driver File 

  @Company:
    Microchip Technology Inc.

  @File Name:
    ext_int.c

  @Summary
    This is the generated driver implementation file for the EXT_INT 
    driver using MPLAB(c) Code Configurator

  @Description:
    This source file provides implementations for driver APIs for EXT_INT. 
    Generation Information : 
        Product Revision  :  MPLAB(c) Code Configurator - pic24-dspic-pic32mm : v1.25
        Device            :  PIC24FJ256GB406
        Driver Version    :  1.0
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.26
        MPLAB             :  MPLAB X 3.45
*/

/**
   Section: Includes
 */
#include <xc.h>
#include "ext_int.h"
//***User Area Begin->code: Add External Interrupt handler specific headers 
#include "app.h"

volatile uint16_t counter_positive_edge_rdyclk = 0;
extern volatile bool g_new_value_of_em4095_rdyclk_measurement;

//***User Area End->code: Add External Interrupt handler specific headers

/**
   Section: External Interrupt Handlers
 */
/**
  Interrupt Handler for EX_INT1 - INT1
*/
void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT1Interrupt(void)
{
    //***User Area Begin->code: INT1 - External Interrupt 1***
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_IR)
    printf( "_INT1Interrupt()\n" );
#endif 
    /* Event on infrared barrier sensor for detect if bird taking a reward. */
    set_flag_ir1_sensor( );

    //***User Area End->code: INT1 - External Interrupt 1***
    EX_INT1_InterruptFlagClear();
}
/**
  Interrupt Handler for EX_INT2 - INT2
*/
void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT2Interrupt(void)
{
    //***User Area Begin->code: INT1 - External Interrupt 1***
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_IR)
    printf( "_INT2Interrupt()\n" );
#endif 
    /* Event on infrared barrier sensor for detect if bird taking a reward. */
    set_flag_ir2_sensor( );

    //***User Area End->code: INT2 - External Interrupt 2***
    EX_INT2_InterruptFlagClear();
}
/**
  Interrupt Handler for EX_INT0 - INT0
*/
void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT0Interrupt(void)
{
    //***User Area Begin->code: INT0 - External Interrupt 0***
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_PIR)
    printf( "_INT0Interrupt()\n" );
#endif 
    /* PIR sensor event. */
    appDataLog.bird_pir_sensor_status = true;

    //***User Area End->code: INT0 - External Interrupt 0***
    EX_INT0_InterruptFlagClear();
}
/**
  Interrupt Handler for EX_INT3 - INT3
*/
void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT3Interrupt(void)
{
    //***User Area Begin->code: INT3 - External Interrupt 3***
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RFID)
    printf( "_INT3Interrupt()\n" );
#endif 
    /* Mesuring RDY/CLK signal frequency. */
    /* Check if we can update counter_positive_edge_rdyclk value. */
    if ( false == g_new_value_of_em4095_rdyclk_measurement )
    {
        ++counter_positive_edge_rdyclk;
    }

    //***User Area End->code: INT3 - External Interrupt 3***
    EX_INT3_InterruptFlagClear();
}
/**
  Interrupt Handler for EX_INT4 - INT4
*/
void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT4Interrupt(void)
{
    //***User Area Begin->code: INT4 - External Interrupt 4***
#if defined (DEBUG_RFID_WORKING_ON_LED_STATUS)
    LED_STATUS_B_SetHigh( );
#endif
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RFID)
    printf( "_INT4Interrupt()\n" );
#endif 
    /* Event on EM4095_DEMOD_OUT RFID signal. */
    
    /* EM4095_DEMOD_OUT interrupt on edge detect. */
    RFID_DecodingTasks( ); /* Call state machine decoding RFID. */

    // TODO: Update flag for DMOD_OUT presence
    // Timeout_Detecting_RFID_Tag = REALOAD_VALUE;

    //***User Area End->code: INT4 - External Interrupt 4***
    EX_INT4_InterruptFlagClear();
}
/**
    Section: External Interrupt Initializers
 */
/**
    void EXT_INT_Initialize(void)

    Initializer for the following external interrupts
    INT1
    INT2
    INT0
    INT3
    INT4
*/
void EXT_INT_Initialize(void)
{
    /*******
     * INT1
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EX_INT1_InterruptFlagClear();   
    EX_INT1_PositiveEdgeSet();
    /*******
     * INT2
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EX_INT2_InterruptFlagClear();   
    EX_INT2_NegativeEdgeSet();
    /*******
     * INT0
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EX_INT0_InterruptFlagClear();   
    EX_INT0_PositiveEdgeSet();
    EX_INT0_InterruptEnable();
    /*******
     * INT3
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EX_INT3_InterruptFlagClear();   
    EX_INT3_PositiveEdgeSet();
    EX_INT3_InterruptEnable();
    /*******
     * INT4
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EX_INT4_InterruptFlagClear();
    EX_INT4_NegativeEdgeSet();
    EX_INT4_InterruptEnable();
}
