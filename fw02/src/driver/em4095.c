/**
 * @file em4095.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 06/07/2016
 * @revision history 3
 * @device http://www.emmicroelectronic.com/products/rf-identification-security/rf-reader-ics/em4095
 * @datasheet http://www.emmicroelectronic.com/sites/default/files/public/products/datasheets/em4095_ds.pdf
 */

#include "app.h"
#include "em4095.h"


extern const char bin2ascii_tab[];

extern RFID_STATES g_rfid_reading_status;

/* Used with mcc_generated_files\tmr4.c */
volatile bool g_rfid_activate = false; // [0, 1]
volatile bool new_pit_tag_found = false; // [0, 1]
volatile bool g_datastream_read_bit; // [0, 1] lecture d'un bit, valeur du signal DEMOD_OUT lu depuis l'interruption du timer
volatile uint8_t g_counter_delay_overflow = 0; // increment in mcc_generated_files\tmr4.c
volatile uint8_t g_counter_delay_read_bit = 0; // increment in mcc_generated_files\tmr4.c

// Parity Bit
// Probably that these global variables can be local:
volatile uint8_t g_previous_pit_tag_tab[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
volatile uint8_t g_pit_tag_tab[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
volatile uint8_t g_bit_counter_line = 0;
volatile uint8_t g_bit_counter_column_tab[] = { 0, 0, 0, 0 };
volatile bool g_check = 1;

#if defined (USE_UART1_SERIAL_INTERFACE)

void displayPitTag( void )
{
    uint8_t i;

    for ( i = 0; i < 10; ++i )
    {
        putchar( bin2ascii_tab[g_pit_tag_tab[i]] );
    }
}
#endif

bool CheckParity( bool parity_bit )
{
    //    printf( "Parity Bit: %d - %d (%d)\n", parity_bit, bit_counter_line & 1, bit_counter_line );

    return (( g_bit_counter_line & 1 ) == parity_bit );
}

void bitsToPitTag( bool bit_read, uint8_t bit_counter )
{
    static uint8_t indice_pit_tag_tab = 0;

    // printf( "Bit Counter: %02d - Bit Read: %1d\n", bit_counter, bit_read );

    /* Line parity bits. */
    if ( ( bit_counter % 5 ) == 0 )
    {
        g_check = CheckParity( bit_read ); /* all parity check and update globale "check" */

        if ( false == g_check )
        {
#if defined (USE_UART1_SERIAL_INTERFACE)
            //            printf( "Line Parity error!\n" ); // FIXME: Try to undestend why we have "Line Parity error!"
#endif
            //DecodingRfidInitializeStateMachine( );
            //EX_INT4_PositiveEdgeSet( ); /* Set the edge of external interrupt to handle positive edge interrupts. */
            g_rfid_reading_status = RFID_DETECT_FALSE_DATASTREAM; // for serial display
#if defined (USE_UART1_SERIAL_INTERFACE)
            //displayPitTag( );
            //            UART1_Write( '-' );
            //UART1_Write( '\n' );
#endif

        }
        g_bit_counter_line = 0; // raz counter line bit
    }
        /* Data bits. */
    else
    {
        //        printf( "Indice Pit Tag: %d - Indice colonne: %d\n", bit_counter / 5, bit_counter % 5 );
        indice_pit_tag_tab = bit_counter / 5;

        g_pit_tag_tab[indice_pit_tag_tab] = g_pit_tag_tab[indice_pit_tag_tab] << 1;
        g_pit_tag_tab[indice_pit_tag_tab] += bit_read;

        g_bit_counter_line += bit_read;
        g_bit_counter_column_tab[bit_counter % 5 - 1] += bit_read; /* to bit_counter_column[0 - 3] */
    }
} /* End of bitsToPitTag() */

/*******************************************************************************
  Function:
    void RFID_Initialize( void )

  Remarks:
 */
void RFID_Initialize( void )
{
    /* At power up the input SHD has to be high to enable correct initialisation. */
    RFID_Disable( ); /* make sure SHD is in high state, EM4095 disable */
}

void RFID_Enable( void )
{
    /* CMD_VDD_APP must be set to enable 5V regulator U5 to powering EM4095 */

    EM4095_SHD_ENABLE( );
    /* Set-up time after a sleep period - Tset: 35ms */
    /* Tableau page 5 - datasheet EM4095*/
    setDelayMsEM4095( EM4095_TSET_DELAY_MS );
    while ( isDelayMsEndingEM4095( ) == false )
    {
        Nop( );
    }

    EX_INT4_PositiveEdgeSet( ); /* Set the edge of external interrupt to handle positive edge interrupts. */
    EX_INT4_InterruptEnable( );
    g_rfid_activate = true;
}

void RFID_Disable( void )
{
    EM4095_SHD_DISABLE( );
    EX_INT4_InterruptDisable( );
    g_rfid_activate = false;
}

/*******************************************************************************
  Function:
   void RFID_DecodingTasks( void )
     Real-time analysis datastream, decoding of the raw frame
     Manchester-code decoding, Frame header detection
     Décodage du code manchester, détection de l'entête de trame

  Remarks:
 */
void __attribute__( ( weak ) ) RFID_DecodingTasks( void )
{
    static volatile MANCHESTER_CODE_DECODING_STATES fsm_datastream_decoding_state = MANCHESTER_CODE_DECODING_STATE_IDLE;
    static volatile uint8_t count_one_seq = 0; /* counts the number of 'logic one' in series for decoding Header data */
    static volatile bool previous_bit_value; /* [0, 1] previous_bit_value */
    static volatile bool last_previous_bit_value; /* [0, 1] bit value before the last bit "previous_bit_value" */
    static volatile uint8_t g_datastream_bit_counter = 0; /* [0-55] nombre de bits acquis comptés */
    static volatile uint8_t bit_number; /* number of read bit in the decoding step */
    static volatile uint8_t bit_indice; /* the value of the bit indice */
    static volatile uint8_t bit_counter_line; /* counts the number of bit equal to 1 in Header data */
    static volatile uint8_t bit_counter_column[4]; /* counts the number of bit equal to 1 in Header data */
    static volatile uint8_t *p_tab_raw_rfid_frame; /* pointer to the acquired raw frame */
    static volatile uint8_t g_rfid_datastream_acquire_tab[56]; /* [55] Trame brut acquise */

    /* (en) State machine decoding of the raw frame - (fr) Machine à états du décodage de la trame brut */
    switch ( fsm_datastream_decoding_state )
    {
        case MANCHESTER_CODE_DECODING_STATE_IDLE:
#if defined (DEBUG_RFID_WORKING_ON_LED_STATUS)
            //            setLedsStatusColor( LED_RED );
            //            LED_STATUS_R_SetLow( ); // FIXME: Debug display
#endif            
            /* -1- Step "Validate stop bit" Waiting first event */
            // (en) An interrupt on the rising edge has been produced on the signal DEMOD_OUT
            // (fr) Une interruption sur front montant vient de se produit sur le signal DEMOD_OUT
            SynchroWithTimerDelayCount( ); // timer synchronization
            g_counter_delay_overflow = 0; // clear delay overflow counter
            EX_INT4_NegativeEdgeSet( ); // change edge detect for next interrupt
            fsm_datastream_decoding_state = MANCHESTER_CODE_DECODING_STATE_DETECTING_STOP_BIT_OF_PREVIOUS_FRAME;
            break;
            /* -------------------------------------------------------------- */

        case MANCHESTER_CODE_DECODING_STATE_DETECTING_STOP_BIT_OF_PREVIOUS_FRAME:
            /* -1- Step "Validate stop bit" waiting delay overflow */
            // Sherche for 0, 1 event (stop bit and next the first bit at 1)
            if ( g_counter_delay_overflow == 1 )
            {
#if defined (DEBUG_RFID_WORKING_ON_LED_STATUS)
    LED_STATUS_R_SetHigh( );
#endif               
                SynchroWithTimerDelayCount( ); // timer synchronization
                g_counter_delay_overflow = 0; // clear delay overflow counter
                EX_INT4_PositiveEdgeSet( ); // set the edge of external interrupt to handle positive edge interrupts
                count_one_seq = 1; // initialized counter of one sequence in Header data frame
                fsm_datastream_decoding_state = MANCHESTER_CODE_DECODING_STATE_HEADER_DATA_DETECTION;
            }
            else
            {
                //putchar(g_counter_delay_overflow); // --> 0
                //!\ if (( g_counter_delay_overflow < 1 ) || ( g_counter_delay_overflow >= 2 ))
                //DecodingRfidInitializeStateMachine( );
                EX_INT4_PositiveEdgeSet( ); /* Set the edge of external interrupt to handle positive edge interrupts. */
                fsm_datastream_decoding_state = MANCHESTER_CODE_DECODING_STATE_IDLE;
            }
            break;
            /* -------------------------------------------------------------- */

        case MANCHESTER_CODE_DECODING_STATE_HEADER_DATA_DETECTION:
            /* -1- Step "Validate stop bit" waiting falling edge */
            if ( g_counter_delay_overflow == 0 )
            {
                ++count_one_seq;
                if ( count_one_seq == 17 )
                {
                    previous_bit_value = 1; // update the value of the previous bit acquire
                    g_datastream_bit_counter = 0; // nombre de bits acquis comptés
                    p_tab_raw_rfid_frame = g_rfid_datastream_acquire_tab; // pointer initialized
                    //                    DecodingRfidInitializeStateMachine( );
                    //                    EX_INT4_PositiveEdgeSet( ); /* Set the edge of external interrupt to handle positive edge interrupts. */
                    EX_INT4_NegativeEdgeSet( );
                    SynchroWithTimerDelayCount( ); // timer synchronization
                    // Initialized counter
                    bit_indice = 0;
                    bit_counter_line = 0;
                    bit_counter_column[0] = 0;
                    bit_counter_column[1] = 0;
                    bit_counter_column[2] = 0;
                    bit_counter_column[3] = 0;
                    /* Go to next step for decoding raw RFID frame. */
#if defined (DEBUG_RFID_WORKING_ON_LED_STATUS)
                    //            setLedsStatusColor( LED_BLUE );
                    //                                LED_STATUS_R_SetHigh( ); // FIXME: Debug display
#endif                     
                    fsm_datastream_decoding_state = MANCHESTER_CODE_DECODING_STATE_READ_BIT_DATASTREAM;
                    break;
                }

                SynchroWithTimerDelayCount( ); // timer synchronization
                g_counter_delay_overflow = 0; // clear delay overflow counter
                INTCON2bits.INT4EP = !INTCON2bits.INT4EP;
            }
            else
            {
#if defined (DEBUG_RFID_WORKING_ON_LED_STATUS)
                //            setLedsStatusColor( LED_BLUE );
                //            LED_STATUS_R_SetLow( ); // FIXME: Debug display
#endif                     
                //!\ if (( g_counter_delay_overflow < 1 ) || ( g_counter_delay_overflow >= 2 ))
                SynchroWithTimerDelayCount( ); // timer synchronization
                g_counter_delay_overflow = 0; // clear delay overflow counter
                EX_INT4_NegativeEdgeSet( );
                fsm_datastream_decoding_state = MANCHESTER_CODE_DECODING_STATE_DETECTING_STOP_BIT_OF_PREVIOUS_FRAME;
            }
            break;
            /* -------------------------------------------------------------- */

        case MANCHESTER_CODE_DECODING_STATE_READ_BIT_DATASTREAM:
            /* TODO: MANCHESTER_CODE_DECODING_READ_BIT_DATASTREAM */
            if ( g_counter_delay_overflow == 1 )
            {
                if ( g_counter_delay_read_bit < DECODING_RFID_QUARTER_PERIODE_READING )
                {
                    /* Write bit value in data tab: case short delay */
                    *p_tab_raw_rfid_frame++ = previous_bit_value; // save the value of current bit 
                    ++g_datastream_bit_counter; // increment bit counter
                    bit_number = 1; // one bit read in this decoding step
                }
                else
                {
                    /* Write bit value in data tab: case middle delay. */
                    if ( g_datastream_read_bit == 0 )
                    {
                        *p_tab_raw_rfid_frame++ = 0; // save the value of current bit 
                        previous_bit_value = 0; // set the value of previous bit
                        ++g_datastream_bit_counter; // increment bit counter
                        bit_number = 1; // one bit read in this decoding step
                    }
                    else
                    {
                        *p_tab_raw_rfid_frame++ = 0; // save the value of current bit
                        last_previous_bit_value = 0; // set the value of last previous bit
                        *p_tab_raw_rfid_frame++ = 1; // save the value of current bit 
                        previous_bit_value = 1; // set the value of previous bit
                        g_datastream_bit_counter += 2; // increment bit counter x2
                        bit_number = 2; // two bit read in this decoding step
                    }
                }
            }

            /* Write bit value in data tab: case long delay. */
            if ( g_counter_delay_overflow == 2 )
            {
                *p_tab_raw_rfid_frame++ = 0; // save the value of current bit
                last_previous_bit_value = 0; // set the value of last previous bit
                *p_tab_raw_rfid_frame++ = 1; // save the value of current bit 
                previous_bit_value = 1; // set the value of previous bit
                g_datastream_bit_counter += 2; // increment bit counter x2
                bit_number = 2; // two bit read in this decoding step
            }

            /* End of datastream after EOF bit (0). */
            if ( g_datastream_bit_counter > DATASTREAM_BUFFER_SIZE )
            {
                /* TODO: make MANCHESTER_CODE_DECODING_CALCULATE_PARITY_BITS live */
                uint8_t indice_tab = 0; /* use one time (no need to be static) */
                uint8_t i;

                //EM4095_PrintDataTab( ( uint8_t * ) g_rfid_datastream_acquire_tab, DATASTREAM_BUFFER_SIZE ); // display RFID datastream
                //UART1_Write( '\n' );

                /* Initialized tab */
                g_check = 1;
                /* RAZ data into data stream buffer. */
                for ( i = 0; i < 10; ++i )
                {
                    g_pit_tag_tab[i] = 0;
                }

                for ( i = 0; i < 4; ++i )
                {
                    bit_counter_column[i] = 0;
                }

                while ( 1 )
                {
                    if ( indice_tab < 50 )
                    {
                        // bitsToPitTag( g_rfid_datastream_acquire_tab[indice_tab++], indice_tab ); // FIXME: warning: operation on 'indice_tab' may be undefined
                        // FIXME: if change to below then we have the error "Line Parity error"
                        // do INC before call bitsToPitTag()
                        ++indice_tab;
                        bitsToPitTag( g_rfid_datastream_acquire_tab[indice_tab - 1], indice_tab );
                    }
                    else
                    {
                        /* Column bits parity. */
                        g_check = ( ( bit_counter_column[0] & 1 ) == g_rfid_datastream_acquire_tab[50] ) && ( ( bit_counter_column[1] & 1 ) == g_rfid_datastream_acquire_tab[51] ) && ( ( bit_counter_column[2] & 1 ) == g_rfid_datastream_acquire_tab[52] ) && ( ( bit_counter_column[3] & 1 ) == g_rfid_datastream_acquire_tab[53] );

                        if ( g_check == 0 )
                        {
                            //#if defined (USE_UART1_SERIAL_INTERFACE)
                            //                            printf( "\nColumn Parity error" );
                            //#endif
                        }
                    }

                    if ( false == g_check )
                    {
                        // TODO: Check ERROR
                        break;
                    }

                    if ( indice_tab == 50 )
                    {
                        break;
                    }
                }

                if ( true == g_check )
                {
                    g_rfid_reading_status = RFID_DETECT_COMPLET_DATASTREAM; // for serial display
                }

                //DecodingRfidInitializeStateMachine( );
                EX_INT4_PositiveEdgeSet( ); /* Set the edge of external interrupt to handle positive edge interrupts. */
                // Display complete data stream on serial terminal
                fsm_datastream_decoding_state = MANCHESTER_CODE_DECODING_STATE_IDLE;
            } // end of if ( g_datastream_bit_counter > DATASTREAM_BUFFER_SIZE )

            /* Synchronization with time base. */
            SynchroWithTimerDelayCount( ); // timer synchronization
            g_counter_delay_overflow = 0; // clear delay overflow counter
            break;
            /* -------------------------------------------------------------- */

        case MANCHESTER_CODE_DECODING_STATE_ERROR:
        default:
            /* if nothing else matches, do the default. */
            //#if defined (DEBUG_RFID_WORKING_ON_LED_STATUS)
            //            setLedsStatusColor( LED_RED );
            ////            LED_STATUS_R_SetLow( ); // FIXME: Debug display
            //#endif   
            //DecodingRfidInitializeStateMachine( );
            //EX_INT4_PositiveEdgeSet( ); /* Set the edge of external interrupt to handle positive edge interrupts. */
            fsm_datastream_decoding_state = MANCHESTER_CODE_DECODING_STATE_IDLE;
            break;
    }

#if defined (DEBUG_RFID_WORKING_ON_LED_STATUS)
    LED_STATUS_R_SetLow( ); // FIXME: Debug display
    LED_STATUS_G_SetLow( ); // FIXME: Debug display
    LED_STATUS_B_SetLow( ); // FIXME: Debug display
#endif
} /* End of RFID_DecodingTasks( ) */

/*******************************************************************************
  Function:
    void EM4095_PrintDataTab( )

  Parameters:
    p_data_tab     - pointer to datastream tab
    data_len

  Remarks:
 */
void EM4095_PrintDataTab( uint8_t * p_data_tab, uint8_t data_len )
{
    uint8_t i;

    for ( i = 0; i < data_len; ++i )
    {
        putchar( *( p_data_tab++ ) + 0x30 ); /* display ASCII datastream */
    }
}


/*******************************************************************************
 End of File
 */
