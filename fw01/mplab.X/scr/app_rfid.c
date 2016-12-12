/**
 * @file app_rfid.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 01/09/2016
 */

#include "app.h"
#include "app_rfid.h"

RFID_STATES g_rfid_reading_status = DISPLAY_RFID_INIT;
RFID_STATES g_rfid_reading_status_previous = DISPLAY_RFID_ERROR;
volatile uint8_t g_timeout_reading_pit_tag = 0;


void APP_Rfid_Init( void )
{
    CMD_VDD_APP_V_USB_SetHigh( ); /* Powering VDD APP for USB and RFID. */
    appDataUsb.key_is_nedded = false;
    appData.flags.bit_value.NewValidPitTag = false;
    appDataPitTag.didPitTagMatched = false;
    RFID_Enable( );
    g_timeout_reading_pit_tag = DEFAULT_TIMEOUT_READING_PIT_TAG;
    g_rfid_reading_status = DISPLAY_RFID_INIT;
}


bool APP_Rfid_Task( void )
{

    switch ( g_rfid_reading_status )
    {
        case DISPLAY_RFID_INIT:
            if ( g_rfid_reading_status != g_rfid_reading_status_previous )
            {
                g_rfid_reading_status_previous = g_rfid_reading_status;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_RFID_STATE)
                printf( "\t> DISPLAY_RFID_INIT\n" );
#endif
            }
            number_of_valid_pit_tag = 0;
            g_rfid_reading_status = RFID_IDLE;
            break;

        case RFID_IDLE:
            if ( g_rfid_reading_status != g_rfid_reading_status_previous )
            {
                g_rfid_reading_status_previous = g_rfid_reading_status;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_RFID_STATE)
                printf( "\t> RFID_IDLE\n" );
#endif
            }
            // waiting for signal from DEMOD_OUT pin of EM4095...
            // --> process decoding call in _INT4Interrupt() ISR routine
            break;

        case RFID_DETECT_FALSE_DATASTREAM:
        {
            if ( g_rfid_reading_status != g_rfid_reading_status_previous )
            {
                g_rfid_reading_status_previous = g_rfid_reading_status;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_RFID_STATE)
                printf( "\t> RFID_DETECT_FALSE_DATASTREAM\n" );
#endif
            }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_PIT_TAG_INFO)
            printf( "Wrong PIT Tag: " );
            displayPitTag( );
            UART1_Write( '\n' );
#endif
            new_pit_tag_found = false;
            g_rfid_reading_status = RFID_IDLE;
            break;
        }

        case RFID_DETECT_COMPLET_DATASTREAM:
        {
            if ( g_rfid_reading_status != g_rfid_reading_status_previous )
            {
                g_rfid_reading_status_previous = g_rfid_reading_status;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_RFID_STATE)
                printf( "\t> RFID_DETECT_COMPLET_DATASTREAM\n" );
#endif
            }

#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
            printf( "Correct PIT Tag: " );
            displayPitTag( );
            UART1_Write( '\n' );
#endif
            new_pit_tag_found = true;
            g_rfid_reading_status = RFID_IDLE;
            break;
        }

        case DISPLAY_RFID_ERROR:
        default:
            if ( g_rfid_reading_status != g_rfid_reading_status_previous )
            {
                g_rfid_reading_status_previous = g_rfid_reading_status;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_RFID_STATE)
                printf( "\t> DISPLAY_RFID_ERROR\n" );
#endif
            }
    }

    if ( new_pit_tag_found )
    {
        uint8_t i;
        uint8_t s = 0;

        new_pit_tag_found = false;

        /* Pit tag validity */
        // Case 0000000000        
        for ( i = 0; i < 10; ++i )
        {
            s += g_pit_tag_tab[i];
        }
        if ( s == 0 )
        {
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
            printf( "Invalid PIT Tag (0000000000).\n" );
#endif
            appData.flags.bit_value.NewValidPitTag = false;
            return true;
        }

        ++number_of_valid_pit_tag;

        if ( number_of_valid_pit_tag == 1 )
        {
            /* Save the current PIT Tag in local tab. */
            for ( i = 0; i < 10; ++i )
            {
                g_previous_pit_tag_tab[i] = g_pit_tag_tab[i];
            }
        }
        else // number_of_valid_pit_tag == 2
        {
            /* Compare previous and current detected PIT tags. */
            if ( 0 == strcmp( ( const char * ) &g_pit_tag_tab, ( const char * ) &g_previous_pit_tag_tab ) )
            {
                appData.flags.bit_value.NewValidPitTag = true;

#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
                printf( "Valid PIT Tag.\n" );
#endif
                for ( i = 0; i < 10; ++i )
                {
                    appDataLog.bird_pit_tag_str[i] = bin2ascii_tab[g_pit_tag_tab[i]];
                }
                clearPitTagBuffers( );
                appDataLog.bird_pit_tag_str[10] = '\0';
                number_of_valid_pit_tag = 0;
            }
            else
            {
                appData.flags.bit_value.NewValidPitTag = false;
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
                printf( "Invalid PIT Tag (different from previous detected).\n" );
#endif
                for ( i = 0; i < 10; ++i )
                {
                    g_previous_pit_tag_tab[i] = g_pit_tag_tab[i];
                    g_pit_tag_tab[i] = 0;
                }
                --number_of_valid_pit_tag;
            }
        }
    }

    return true;
}


void clearPitTagBuffers( void )
{
    int i;

    for ( i = 0; i < 10; ++i )
    {
        g_pit_tag_tab[i] = 0;
        g_previous_pit_tag_tab[i] = 0;
    }

    number_of_valid_pit_tag = 0;
}


bool checkPitTagDenied( void )
{
    bool isDenied = false;
    int i;

    for ( i = 0; i < ( appDataPitTag.numPitTagDeniedOrColorA + appDataPitTag.numPitTagAcceptedOrColorB ); ++i )
    {
        if ( 0 == strcmp( appDataLog.bird_pit_tag_str, appDataPitTag.pit_tags_list[i] ) )
        {
//#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
//            printf( "PIT tag index: %d\n", i );
//#endif
            /* Current PIT tag is in the denied list */
            isDenied = appDataPitTag.isPitTagdeniedOrColorA[i];
            /* Current PIT tag is in the all PIT tags list */
            appDataPitTag.didPitTagMatched = true;
            break;
        }
    }

    return isDenied;
}


/*******************************************************************************
 End of File
 */
