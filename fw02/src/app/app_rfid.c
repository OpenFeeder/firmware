/**
 * @file app_rfid.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 01/09/2016
 */

#include "app.h"
#include "app_rfid.h"

volatile bool g_new_value_of_em4095_rdyclk_measurement = false;


RFID_STATES g_rfid_reading_status; // for serial display of state machine process of EM4095
RFID_STATES g_rfid_reading_status_previous;
volatile uint8_t g_timeout_reading_pit_tag;
extern volatile uint16_t rdyclk_count_in_10ms;

/* Binary to Ascii text converter with simple lookup array */
const char bin2ascii_tab[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

void APP_Rfid_Init( void )
{
    appDataUsb.getValidDeviceAdress = false;
    appDataUsb.key_is_nedded = false;
    appData.flags.bit_value.NewValidPitTag = false;
    appDataPitTag.didPitTagMatched = false;
    g_timeout_reading_pit_tag = DEFAULT_TIMEOUT_READING_PIT_TAG; // TIMEOUT_READING_PIT_TAG = 30x 160 ms
    g_rfid_reading_status = DISPLAY_RFID_INIT;
    appDataPitTag.number_of_valid_pit_tag = 0;
    clearPitTagBuffers( );
    CMD_VDD_APP_V_USB_SetHigh( ); /* Powering VDD APP for USB and RFID. */
    RFID_Enable( );
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
            appDataPitTag.number_of_valid_pit_tag = 0;
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
            new_pit_tag_found = false;
            g_rfid_reading_status = RFID_IDLE;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_PIT_TAG_INFO)
            printf( "\t\tPIT tag: " );
            displayPitTag( );
            printf( " - bad checksum\n" );
#endif
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
            new_pit_tag_found = true;
            g_rfid_reading_status = RFID_IDLE;
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
            printf( "\t\tPIT tag: " );
            displayPitTag( );
            printf( " - good checksum\n" );
#endif
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
        for ( i = 0; i < 10; ++i )
        {
            s += g_pit_tag_tab[i];
            appDataLog.bird_pit_tag_str[i] = bin2ascii_tab[g_pit_tag_tab[i]];
        }
        appDataLog.bird_pit_tag_str[10] = '\0';
        if ( s == 0 ) // Case 0000000000   
        {
            appData.flags.bit_value.NewValidPitTag = false;

#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
            printf( " - invalid (0000000000).\n" );
#endif
            return true;
        }
        if ( OPEN_BAR != appData.scenario_number )
        {
            findPitTagInList( );
            if ( false == appDataPitTag.didPitTagMatched )
            {
                clearPitTagBuffers( );
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
                printf( "\t\tNot listed => consider as denied.\n" );
#endif
                return true;
            }
        }

        ++appDataPitTag.number_of_valid_pit_tag;

        if ( appDataPitTag.number_of_valid_pit_tag == 1 )
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
            //            if ( 0 == strcmp( ( const char * ) &g_pit_tag_tab, ( const char * ) &g_previous_pit_tag_tab ) )
            bool flag = true;
            for ( i = 0; i < 10; i++ )
            {
                if ( g_pit_tag_tab[i] != g_previous_pit_tag_tab[i] )
                {
                    flag = false;
                    break;
                }
            }

            //            if ( 0 == strcmp( g_pit_tag_tab, g_previous_pit_tag_tab ) )
            if ( true == flag )
            {
                appData.flags.bit_value.NewValidPitTag = true;
                for ( i = 0; i < 10; ++i )
                {
                    appDataLog.bird_pit_tag_str[i] = bin2ascii_tab[g_pit_tag_tab[i]];
                }
                clearPitTagBuffers( );
                appDataLog.bird_pit_tag_str[10] = '\0';
                appDataPitTag.number_of_valid_pit_tag = 0;

#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
                printf( "\tTwo valid PIT tags read: %s\n", appDataLog.bird_pit_tag_str );
#endif
            }
            else
            {
                appData.flags.bit_value.NewValidPitTag = false;

                for ( i = 0; i < 10; ++i )
                {
                    g_previous_pit_tag_tab[i] = g_pit_tag_tab[i];
                    g_pit_tag_tab[i] = 0;
                }
                --appDataPitTag.number_of_valid_pit_tag;
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
                printf( " - invalid (different from previous detected).\n" );
#endif
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

    appDataPitTag.number_of_valid_pit_tag = 0;
}

void findPitTagInList( void )
{

    int i;

    for ( i = 0; i < ( appDataPitTag.numPitTagDeniedOrColorA + appDataPitTag.numPitTagAcceptedOrColorB ); ++i )
    {
        //        printf( "%s %s ", appDataLog.bird_pit_tag_str, appDataPitTag.pit_tags_list[i] );
        if ( 0 == strcmp( appDataLog.bird_pit_tag_str, appDataPitTag.pit_tags_list[i] ) )
        {
            //            printf(" Found\n");
            /* Current PIT tag is in the all PIT tags list */
            appDataPitTag.didPitTagMatched = true;
            appDataPitTag.pitTagIndexInList = ( uint8_t ) i;
            return;
        }
        //        printf("\n");
    }

    //    printf("Not found\n");
    appDataPitTag.didPitTagMatched = false;
    appDataPitTag.pitTagIndexInList = 0;

}

bool isPitTagDenied( void )
{
    return appDataPitTag.isPitTagdeniedOrColorA[appDataPitTag.pitTagIndexInList];
}

void displayRfidFreq( void )
{
#if defined (USE_UART1_SERIAL_INTERFACE)
    printf( "RDY/CLK signal frequency: %u (x10Hz)\n", appData.rfid_rdyclk );
#endif
}

void measureRfidFreq( void )
{
    bool flag = false;

    g_new_value_of_em4095_rdyclk_measurement = false;

    if ( CMD_VDD_APP_V_USB_GetValue( ) == 0 )
    {
        flag = true;
        appDataUsb.key_is_nedded = false;
        powerUsbRfidEnable( );
    }


    EM4095_SHD_ENABLE( );
    /* Set-up time after a sleep period - Tset: 35ms */
    /* Tableau page 5 - datasheet EM4095*/
    setDelayMsEM4095( EM4095_TSET_DELAY_MS );
    while ( false == isDelayMsEndingEM4095( ) )
    {
        Nop( );
    }

    //    while ( false == EM4095_SHD_GetValue( ) )
    //    {
    //        Nop( );
    //    }
    EX_INT3_InterruptFlagClear( );
    EX_INT3_InterruptEnable( );
    while ( false == g_new_value_of_em4095_rdyclk_measurement )
    {
        Nop( );
    }
    EX_INT3_InterruptFlagClear( );
    EX_INT3_InterruptDisable( );

    appData.rfid_rdyclk = rdyclk_count_in_10ms * 5;

    g_new_value_of_em4095_rdyclk_measurement = false;
    if ( flag == true )
    {
        powerUsbRfidDisable( );
    }

}
/*******************************************************************************
End of File
 */
