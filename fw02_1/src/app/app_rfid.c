/**
 * @file app_rfid.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 01/09/2016
 */

#include "app.h"
#include "app_rfid.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

volatile bool g_new_value_of_em4095_rdyclk_measurement = false;
RFID_STATES g_rfid_reading_status; // for serial display of state machine process of EM4095
RFID_STATES g_rfid_reading_status_previous;
volatile uint8_t g_timeout_reading_pit_tag;
extern volatile uint16_t rdyclk_count_in_10ms;

/* Binary to Ascii text converter with simple lookup array */
const char bin2ascii_tab[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };


//******************************************************************************
//******************************************************************************
//  Function Implementation
//******************************************************************************
//******************************************************************************


void APP_Rfid_Init( void )
{
    //    appDataUsb.getValidDeviceAdress = false;
    appDataUsb.key_is_nedded = false;
    appData.flags.bit_value.NewValidPitTag = false;
    appDataPitTag.didPitTagMatched = false;
    g_timeout_reading_pit_tag = DEFAULT_TIMEOUT_READING_PIT_TAG; // TIMEOUT_READING_PIT_TAG = 30x 160 ms
    g_rfid_reading_status = DISPLAY_RFID_INIT;
    appDataPitTag.number_of_valid_pit_tag = 0;
    clearPitTagBuffers( );
    //    CMD_VDD_APP_V_USB_SetHigh(); /* Powering VDD APP for USB and RFID. */
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

            if ( appData.test_rfid )
            {
                setLedsStatusColor( LEDS_OFF );
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
            appData.flags.bit_value.NewValidPitTag = false;
            g_rfid_reading_status = RFID_IDLE;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_PIT_TAG_INFO)
            printf( "\t\tPIT tag: " );
            displayPitTag( );
            printf( " - bad checksum\n" );
#endif
            if ( appData.test_rfid )
            {
                setLedsStatusColor( LED_RED );
            }

            appData.rfid_signal_detected = true;
            break;
        }

        case RFID_DETECT_COMPLET_DATASTREAM:
        {
            /* Disable interruption to avoid writting in g_pit_tag_tab */
            EX_INT4_InterruptDisable();
            
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
            if ( appData.test_rfid )
            {
                setLedsStatusColor( LED_BLUE );
            }

            appData.rfid_signal_detected = true;
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

        ++appDataPitTag.number_of_valid_pit_tag;

        if ( appDataPitTag.number_of_valid_pit_tag == 1 )
        {
            
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
                printf( "\t\tOne valid PIT tags read: %s\n", appDataLog.bird_pit_tag_str );
#endif
            /* Save the current PIT Tag in local tab. */
            for ( i = 0; i < 10; ++i )
            {
                g_previous_pit_tag_tab[i] = g_pit_tag_tab[i];
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
                printf("\t\tCopy: %c %c\n", bin2ascii_tab[g_previous_pit_tag_tab[i]], bin2ascii_tab[g_pit_tag_tab[i]]);
#endif  
            }

        }
        else // number_of_valid_pit_tag == 2 ou 3
        {
            /* Compare previous and current detected PIT tags. */
            //            if ( 0 == strcmp( ( const char * ) &g_pit_tag_tab, ( const char * ) &g_previous_pit_tag_tab ) )
            bool flag = true;
            for ( i = 0; i < 10; i++ )
            {
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
                printf("\t\tCheck: %c %c\n", bin2ascii_tab[g_previous_pit_tag_tab[i]], bin2ascii_tab[g_pit_tag_tab[i]]);
#endif                    
                if ( g_pit_tag_tab[i] != g_previous_pit_tag_tab[i] )
                {
                    flag = false;
                    break;
                }
            }

            //            if ( 0 == strcmp( g_pit_tag_tab, g_previous_pit_tag_tab ) )
            //            if ((true == flag) && (appDataPitTag.number_of_valid_pit_tag == 3))
            if ( true == flag )
            {
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
                printf( "\t\t%d consecutive valid PIT tags read: %s\n", appDataPitTag.number_of_valid_pit_tag, appDataLog.bird_pit_tag_str );
#endif
                if ( appDataPitTag.number_of_valid_pit_tag == NUM_CONSECUTIVE_VALID_PIT_TAG )
                {
                    appData.flags.bit_value.NewValidPitTag = true;
                    for ( i = 0; i < 10; ++i )
                    {
                        appDataLog.bird_pit_tag_str[i] = bin2ascii_tab[g_pit_tag_tab[i]];
                    }
                    clearPitTagBuffers( );
                    appDataLog.bird_pit_tag_str[10] = '\0';
                    appDataPitTag.number_of_valid_pit_tag = 0;
                }
            }
            else
            {
                appData.flags.bit_value.NewValidPitTag = false;

                for ( i = 0; i < 10; ++i )
                {
                    g_previous_pit_tag_tab[i] = g_pit_tag_tab[i];
                    g_pit_tag_tab[i] = 0;
                }
                
//                --appDataPitTag.number_of_valid_pit_tag;                
                appDataPitTag.number_of_valid_pit_tag = 0;
                
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
                printf( "\t\tPIT tag different from previous detected: invalid.\n" );
#endif
            }
        }
    }

    return true;
}


void clearPitTagBuffers( void )
{
    
    appDataPitTag.number_of_valid_pit_tag = 0;
    
    memset(g_pit_tag_tab, 0, 10);
    memset(g_previous_pit_tag_tab, 0, 10);

}

void clearPitTagSringBuffers( void )
{
    
    memset(appDataLog.bird_pit_tag_str, 'J', sizeof(appDataLog.bird_pit_tag_str));
    appDataLog.bird_pit_tag_str[10] = '\0';
       
    memset(appDataPitTag.previous_pit_tags_str, 'K', sizeof(appDataPitTag.previous_pit_tags_str));
    appDataPitTag.previous_pit_tags_str[10] = '\0';
    
}

bool isItANewPitTag(void)
{
    double seconds;
    time_t t1, t2;
    struct tm tm1;
    struct tm tm2;
    
    if ( 0 == strcmp( appDataLog.bird_pit_tag_str, appDataPitTag.previous_pit_tags_str ) )
    {
        
//        printf( "%02u/%02u/20%02u %02u:%02u:%02u ",
//            appDataPitTag.previous_arrived_time.tm_mday,
//            appDataPitTag.previous_arrived_time.tm_mon,
//            appDataPitTag.previous_arrived_time.tm_year,
//            appDataPitTag.previous_arrived_time.tm_hour,
//            appDataPitTag.previous_arrived_time.tm_min,
//            appDataPitTag.previous_arrived_time.tm_sec );
//        printf( "%02u/%02u/20%02u %02u:%02u:%02u\n",
//            appDataLog.bird_arrived_time.tm_mday,
//            appDataLog.bird_arrived_time.tm_mon,
//            appDataLog.bird_arrived_time.tm_year,
//            appDataLog.bird_arrived_time.tm_hour,
//            appDataLog.bird_arrived_time.tm_min,
//            appDataLog.bird_arrived_time.tm_sec );

        tm1 = appDataLog.bird_arrived_time;
        tm1.tm_year += 100;
        tm1.tm_mon -= 1;
        tm2 = appDataPitTag.previous_arrived_time;
        tm2.tm_year += 100;
        tm2.tm_mon -= 1;
        
        t1 = mktime(&tm1);
        t2 = mktime(&tm2);
        
        seconds = difftime(t1,t2);

//        printf("%d %d %f\n", (int)t1, (int)t2, seconds);
//        
//        t1 = mktime(&appDataLog.bird_arrived_time);
//        t2 = mktime(&appDataPitTag.previous_arrived_time);
//        
//        printf("%d %d\n", (int)t1, (int)t2);
        
//        seconds = difftime(mktime(&appDataLog.bird_arrived_time),mktime(&appDataPitTag.previous_arrived_time));

        
        if ( seconds > appDataPitTag.timeout_unique_visit)
        {
#if defined( USE_UART1_SERIAL_INTERFACE )
            printf( "\tSame PIT tag. %.0fs > %us => OK.\n", seconds, appDataPitTag.timeout_unique_visit );
#endif  
            appDataPitTag.previous_arrived_time = appDataLog.bird_arrived_time;
            return true;    
        }
        else
        {
#if defined( USE_UART1_SERIAL_INTERFACE )
            printf( "\tSame PIT tag. %.0fs <= %us => No way.\n", seconds, appDataPitTag.timeout_unique_visit );
#endif  
            return false;    
        }
    }
    else
    {
        strcpy(appDataPitTag.previous_pit_tags_str, appDataLog.bird_pit_tag_str);
        appDataPitTag.previous_arrived_time = appDataLog.bird_arrived_time;
        return true;
    }
    
}


void findPitTagInList( void )
{

    int i;

    for ( i = 0; i < appDataPitTag.numPitTagStored; ++i )    
    {
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
        printf( "%s %s ", appDataLog.bird_pit_tag_str, appDataPitTag.pit_tags_list[i] );
#endif
        if ( 0 == strcmp( appDataLog.bird_pit_tag_str, appDataPitTag.pit_tags_list[i] ) )
        {
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
            printf( " Found\n" );
#endif
            /* Current PIT tag is in the all PIT tags list */
            appDataPitTag.didPitTagMatched = true;
            appDataPitTag.pitTagIndexInList = ( uint8_t ) i;
            return;
        }
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
        printf( "\n" );
#endif
    }
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined (DISPLAY_PIT_TAG_INFO)
    printf( "Not found\n" );
#endif
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
    printf( "\tRDY/CLK signal frequency: %u (x10Hz)\n", appData.rfid_rdyclk );
#endif
}


bool measureRfidFreq( void )
{
    bool flag_cmd_vdd_app = false;
    bool flag_timeout = false;
    int i;

    if ( CMD_VDD_APP_V_USB_GetValue( ) == 0 )
    {
        flag_cmd_vdd_app = true;
        appDataUsb.key_is_nedded = false;
        powerUsbRfidEnable( );
    }

    /* Up to MAX_MEASURE_ATTEMPTS attempts to measure a valid RFID frequency */
    for ( i = 0; i < MAX_MEASURE_ATTEMPTS; i++ )
    {

        EM4095_SHD_ENABLE( );

        setDelayMsEM4095( EM4095_TSET_DELAY_MS );
        while ( false == isDelayMsEndingEM4095( ) )
        {
            Nop( );
        }

        setDelayMsEM4095( 500 );

        EX_INT3_InterruptFlagClear( );
        EX_INT3_InterruptEnable( );

        counter_positive_edge_rdyclk = 0;
        g_new_value_of_em4095_rdyclk_measurement = false;
        while ( false == g_new_value_of_em4095_rdyclk_measurement )
        {
            Nop( );
            if ( true == isDelayMsEndingEM4095( ) )
            {
                flag_timeout = true;
                break;
            }
        }

        EX_INT3_InterruptFlagClear( );
        EX_INT3_InterruptDisable( );

        EM4095_SHD_DISABLE( );

        if ( false == flag_timeout )
        {
            appData.rfid_rdyclk = g_rdyclk_count_in_10ms * 5;

            g_new_value_of_em4095_rdyclk_measurement = false;

            /* If frequency is OK or if timeout occurs, quit the measure process */
            if ( appData.rfid_rdyclk > MIN_RDYCLK_FREQ )
            {
                break;
            }

        }

    }

    g_new_value_of_em4095_rdyclk_measurement = false;

    if ( true == flag_cmd_vdd_app )
    {
        //        powerUsbRfidDisable( );
    }

    if ( true == flag_timeout )
    {
        //#if defined (USE_UART1_SERIAL_INTERFACE)
        //        printf("\t Timeout reached during RFID mesure.\n");
        //#endif 
        return false;

    }

    return true;

}
/*******************************************************************************
End of File
 */
