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
 * File: em4095.h
 * Created: 06/07/2016
 * Authors: Arnauld BIGANZOLI, Jérôme BRIOT
 * Comments:
 * Supported Transponders EM4102 tag has 8 bytes of read only memory
 * The driver need 40 ms communication time with the transceiver for reading the 40-bit serial number (8x 5 byte)
 * EM4001 tag data (Hex): FF FF FF FF FF (Maximum value)
 * Bird PIT Tags: http://www.ibtechnology.co.uk/case-study-bird-tags.htm
 * Revision history: 0.0.2
 */

#ifndef XC_HEADER_EM4095_H
#define	XC_HEADER_EM4095_H

#define DATASTREAM_BUFFER_SIZE 55 /* number of bit after header data in RFID raw frame */
#define SynchroWithTimerDelayCount() {g_counter_delay_read_bit = 0;} /* timer synchronization */

/* Set-up time after a sleep period - Tset: 35ms */
/* Tableau page 5 - datasheet EM4095*/
#define EM4095_TSET_DELAY_MS 40 
/**
 * Enable EM4095 device.
 *  When SHD is low the circuit is enabled to emit RF field. 
 */
//#define EM4095_SHD_ENABLE() {EM4095_SHD_SetHigh( );} /* Enable EM4095, SHD pin set low. */
#define EM4095_SHD_ENABLE() {EM4095_SHD_SetLow( );EM4095_SHD_SetDigitalOutput( );} /* Enable EM4095, SHD pin set low. */

/**
 * Disable EM4095 device.
 * When SHD is high EM4095 is in sleep mode, current consumption is minimised.
 * Here there is mosfet to control the SHD pin, so a 0 on mosfet put 1 on SHD pin.
 */
//#define EM4095_SHD_DISABLE() {EM4095_SHD_SetLow( );} /* Disable EM4095, SHD pin set high. */
#define EM4095_SHD_DISABLE() {EM4095_SHD_SetDigitalInput( );} /* Disable EM4095, SHD pin set high. */

/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
 */

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

typedef enum {
    /* In this state, the application initialize the decoding state machine */
    // DECODING_CODE_DECODING_STATE_INITIALIZE_STATE_MACHINE, // if UNINITIALIZED

    /* In this state, application is in IDLE state after completion. */
    MANCHESTER_CODE_DECODING_STATE_IDLE,

    /* Try to detecting the end of the previous frame */
    MANCHESTER_CODE_DECODING_STATE_DETECTING_STOP_BIT_OF_PREVIOUS_FRAME,

    MANCHESTER_CODE_DECODING_STATE_HEADER_DATA_DETECTION,

    MANCHESTER_CODE_DECODING_STATE_READ_BIT_DATASTREAM,

    // MANCHESTER_CODE_DECODING_STATE_READ_PARITY_BIT_DATASTREAM,

    // MANCHESTER_CODE_DECODING_STATE_CALCULATE_PARITY_BITS,

    // MANCHESTER_CODE_DECODING_STATE_WAITTING_STOP_BIT,

    // MANCHESTER_CODE_DECODING_END_OF_STATE,

    /* Application error state */
    MANCHESTER_CODE_DECODING_STATE_ERROR

} MANCHESTER_CODE_DECODING_STATES;

extern volatile MANCHESTER_CODE_DECODING_STATES fsm_datastream_decoding_state;

typedef enum {
    INT_DELAY_OVERFLOW,
    INT_FALLING_EDGE

} ISR_ID_DECODING_RFID; // identification of the interrupt

// *****************************************************************************
// *****************************************************************************
// Section: extern declarations
// *****************************************************************************
// *****************************************************************************

//extern bool g_demod_out_value; // [0, 1] lecture de la valeur du signal DEMOD_OUT
extern volatile bool g_rfid_activate;
extern volatile bool new_pit_tag_found;
extern volatile bool g_datastream_read_bit; // [0, 1] lecture du bit identifié dans la trame depuis l'interruption du timer
//extern bool g_datastream_current_bit; // [0, 1] lecture du bit identifié dans la trame
//extern bool g_datastream_previous_bit; // [0, 1] 
//extern volatile uint8_t g_datastream_bit_counter; // [0-55] nombre de bits acquis comptés
//extern volatile uint8_t g_bit_time_counter; // [0-9] comptage d'unité de temps
extern volatile uint8_t g_rfid_datastream_acquire_tab[]; // [55] Trame brut acquise
//extern volatile uint8_t g_counter_demod_out_falling_edge; // for INT4 routine FIXME: No need for this program
extern volatile uint8_t g_counter_delay_overflow; // EM4095 Manchester routine
extern volatile uint8_t g_counter_delay_read_bit;
//extern volatile ISR_ID_DECODING_RFID g_current_interrupt_for_decoding_rfid; // interruption actuelle
extern volatile uint8_t g_pit_tag_tab[];
extern volatile uint8_t g_previous_pit_tag_tab[];


/*******************************************************************************
  Function:
    void EM4095_Initialize ( void )

  Summary:
     EM4095's initialization routine

  Description:
    This routine initializes EM4095's state machine.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    EM4095_Initialize();
    </code>

  Remarks:
    None.
 */
void RFID_Initialize(void);
void RFID_Enable(void);
void RFID_Disable(void);

void RFID_DecodingTasks(void);

/*******************************************************************************
  Function:
    void EM4095_Tasks ( void )

  Summary:
    EM4095 tasks function

  Description:
    This routine is the EM4095's tasks function.  It defines the application's
    state machine and core logic.
    Print and call EM4095 function only one time if state change

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    EM4095_Tasks();
    </code>

  Remarks:
    This routine must be called from APP_Tasks() routine.
 */
//void SerialDisplayRfidTask( void );

void displayPitTag(void);

void EM4095_PrintDataTab(uint8_t * p_datastream, uint8_t data_len);

//void DecodingRfidInitializeStateMachine( void );

#endif	/* XC_HEADER_EM4095_H */
