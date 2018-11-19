/**
 * @file app_config.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#include "app.h"
#include "app_config.h"


bool config_set( void )
{

    INI_READ_STATE read_ini_status;
    char buf[50];

    /* Search for the CONFIG.INI file. */
    if ( FILEIO_RESULT_FAILURE == config_find_ini( ) )
    {
        strcpy( appError.message, "CONFIG.INI not found" );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        appError.number = ERROR_INI_FILE_NOT_FOUND;
        return false;
    }

    /* Read the CONFIG.INI file. */
    read_ini_status = config_read_ini( );

    if ( INI_READ_OK != read_ini_status )
    {

        getIniPbChar( read_ini_status, buf, sizearray( buf ) );

        sprintf( appError.message, "Wrong parameters in CONFIG.INI: %s (%d)", buf, read_ini_status );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        appError.number = ERROR_INI_FILE_READ;
        return false;
    }

    if ( ( GO_NO_GO == appData.scenario_number && appDataAttractiveLeds.pattern_number > ALL_LEDS ) || appData.scenario_number > GO_NO_GO )
    {
        if ( FILEIO_RESULT_FAILURE == read_PIT_tags( ) )
        {
            return false;
        }
    }

    if ( PATCH_PROBABILITY == appData.scenario_number )
    {
        if ( FILEIO_RESULT_FAILURE == read_reward_probabilities( ) )
        {
            return false;
        }
    }

    return true;
}


FILEIO_RESULT read_reward_probabilities( void )
{

    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    char buf[4];
    int i;

    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
        store_event( OF_READ_REWARD_PROBABILITIES );
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf( "Read reward probabilities file\n" );
#endif 

    if ( appDataPitTag.num_pit_tag_accepted_or_color_B > 0 )
    {

        if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, "PROBA.TXT", FILEIO_OPEN_READ ) )
        {
            errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
            printf( "unable to open reward probabilities file (%u)", errF );
#endif 
            sprintf( appError.message, "Unable to open reward probabilities file (%u)", errF );
            appError.current_line_number = __LINE__;
            sprintf( appError.current_file_name, "%s", __FILE__ );
            FILEIO_ErrorClear( 'A' );
            appError.number = ERROR_REWARD_PROBABILITIES_FILE_OPEN;
            return FILEIO_RESULT_FAILURE;
        }

        buf[4] = '\0';

        for ( i = 0; i < appDataPitTag.num_pit_tag_accepted_or_color_B; i++ )
        {
            FILEIO_Read( buf, 1, 3, &file );
            appDataPitTag.reward_probability[i + appDataPitTag.num_pit_tag_denied_or_color_A] = atoi( buf );
        }

        if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
        {
            errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
            printf( "unable to close reward probabilities file (%u)", errF );
#endif 
            sprintf( appError.message, "Unable to close reward probabilities file (%u)", errF );
            appError.current_line_number = __LINE__;
            sprintf( appError.current_file_name, "%s", __FILE__ );
            FILEIO_ErrorClear( 'A' );
            appError.number = ERROR_REWARD_PROBABILITIES_FILE_CLOSE;
            return FILEIO_RESULT_FAILURE;
        }
    }

    return FILEIO_RESULT_SUCCESS;

}


FILEIO_RESULT read_PIT_tags( void )
{

    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    uint16_t i, j, cumulative_sum;
    char buf[13];

    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
        store_event( OF_READ_PIT_TAGS );
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf( "Read PIT tags files\n" );
#endif 

    appDataPitTag.num_pit_tag_stored = 0;

    if ( GO_NO_GO == appData.scenario_number )
    {
        if ( LEFT_RIGHT_LEDS == appDataAttractiveLeds.pattern_number )
        {
            if ( ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_denied_or_color_A && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_denied_or_color_A )
            {

                if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, "PTLEFT.TXT", FILEIO_OPEN_READ ) )
                {
                    errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf( "unable to open PIT tags left file (%u)", errF );
#endif 
                    sprintf( appError.message, "Unable to open PIT tags left file (%u)", errF );
                    appError.current_line_number = __LINE__;
                    sprintf( appError.current_file_name, "%s", __FILE__ );
                    FILEIO_ErrorClear( 'A' );
                    appError.number = ERROR_PIT_TAGS_LEFT_FILE_OPEN;
                    return FILEIO_RESULT_FAILURE;
                }

                for ( i = 0; i < appDataPitTag.num_pit_tag_denied_or_color_A; i++ )
                {
                    FILEIO_Read( appDataPitTag.pit_tags_list[i], 1, 10, &file );
                    appDataPitTag.pit_tags_list[i][11] = '\0';
                    appDataPitTag.num_pit_tag_stored += 1;
                }

                if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
                {
                    errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf( "unable to close PIT tags left file (%u)", errF );
#endif 
                    sprintf( appError.message, "Unable to close PIT tags left file (%u)", errF );
                    appError.current_line_number = __LINE__;
                    sprintf( appError.current_file_name, "%s", __FILE__ );
                    FILEIO_ErrorClear( 'A' );
                    appError.number = ERROR_PIT_TAGS_LEFT_FILE_CLOSE;
                    return FILEIO_RESULT_FAILURE;
                }

            }

            if ( ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_accepted_or_color_B && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_accepted_or_color_B )
            {

                if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, "PTRIGHT.TXT", FILEIO_OPEN_READ ) )
                {
                    errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf( "unable to open PIT tags right file (%u)", errF );
#endif 
                    sprintf( appError.message, "Unable to open PIT tags right file (%u)", errF );
                    appError.current_line_number = __LINE__;
                    sprintf( appError.current_file_name, "%s", __FILE__ );
                    FILEIO_ErrorClear( 'A' );
                    appError.number = ERROR_PIT_TAGS_RIGHT_FILE_OPEN;
                    return FILEIO_RESULT_FAILURE;
                }

                for ( i = 0; i < appDataPitTag.num_pit_tag_accepted_or_color_B; i++ )
                {
                    FILEIO_Read( appDataPitTag.pit_tags_list[i + appDataPitTag.num_pit_tag_denied_or_color_A], 1, 10, &file );
                    appDataPitTag.pit_tags_list[i + appDataPitTag.num_pit_tag_denied_or_color_A][11] = '\0';
                    appDataPitTag.num_pit_tag_stored += 1;
                }

                if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
                {
                    errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf( "unable to close PIT tags right file (%u)", errF );
#endif 
                    sprintf( appError.message, "Unable to close PIT tags right file (%u)", errF );
                    appError.current_line_number = __LINE__;
                    sprintf( appError.current_file_name, "%s", __FILE__ );
                    FILEIO_ErrorClear( 'A' );
                    appError.number = ERROR_PIT_TAGS_RIGHT_FILE_CLOSE;
                    return FILEIO_RESULT_FAILURE;
                }
            }
        }
        else if ( TOP_BOTTOM_LEDS == appDataAttractiveLeds.pattern_number )
        {
            if ( ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_denied_or_color_A && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_denied_or_color_A )
            {

                if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, "PTTOP.TXT", FILEIO_OPEN_READ ) )
                {
                    errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf( "unable to open PIT tags top file (%u)", errF );
#endif 
                    sprintf( appError.message, "Unable to open PIT tags top file (%u)", errF );
                    appError.current_line_number = __LINE__;
                    sprintf( appError.current_file_name, "%s", __FILE__ );
                    FILEIO_ErrorClear( 'A' );
                    appError.number = ERROR_PIT_TAGS_TOP_FILE_OPEN;
                    return FILEIO_RESULT_FAILURE;
                }

                for ( i = 0; i < appDataPitTag.num_pit_tag_denied_or_color_A; i++ )
                {
                    FILEIO_Read( appDataPitTag.pit_tags_list[i], 1, 10, &file );
                    appDataPitTag.pit_tags_list[i][11] = '\0';
                    appDataPitTag.num_pit_tag_stored += 1;
                }

                if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
                {
                    errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf( "unable to close PIT tags top file (%u)", errF );
#endif 
                    sprintf( appError.message, "Unable to close PIT tags top file (%u)", errF );
                    appError.current_line_number = __LINE__;
                    sprintf( appError.current_file_name, "%s", __FILE__ );
                    FILEIO_ErrorClear( 'A' );
                    appError.number = ERROR_PIT_TAGS_TOP_FILE_CLOSE;
                    return FILEIO_RESULT_FAILURE;
                }

            }

            if ( ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_accepted_or_color_B && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_accepted_or_color_B )
            {

                if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, "PTBOTTOM.TXT", FILEIO_OPEN_READ ) )
                {
                    errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf( "unable to open PIT tags bottom file (%u)", errF );
#endif 
                    sprintf( appError.message, "Unable to open PIT tags bottom file (%u)", errF );
                    appError.current_line_number = __LINE__;
                    sprintf( appError.current_file_name, "%s", __FILE__ );
                    FILEIO_ErrorClear( 'A' );
                    appError.number = ERROR_PIT_TAGS_BOTTOM_FILE_OPEN;
                    return FILEIO_RESULT_FAILURE;
                }

                for ( i = 0; i < appDataPitTag.num_pit_tag_accepted_or_color_B; i++ )
                {
                    FILEIO_Read( appDataPitTag.pit_tags_list[i + appDataPitTag.num_pit_tag_denied_or_color_A], 1, 10, &file );
                    appDataPitTag.pit_tags_list[i + appDataPitTag.num_pit_tag_denied_or_color_A][11] = '\0';
                    appDataPitTag.num_pit_tag_stored += 1;
                }

                if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
                {
                    errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf( "unable to close PIT tags bottom file (%u)", errF );
#endif 
                    sprintf( appError.message, "Unable to close PIT tags bottom file (%u)", errF );
                    appError.current_line_number = __LINE__;
                    sprintf( appError.current_file_name, "%s", __FILE__ );
                    FILEIO_ErrorClear( 'A' );
                    appError.number = ERROR_PIT_TAGS_BOTTOM_FILE_CLOSE;
                    return FILEIO_RESULT_FAILURE;
                }
            }
        }
        else if ( ONE_LED == appDataAttractiveLeds.pattern_number )
        {
            memset( buf, 0, sizeof (buf ) );

            /* Cumulative sum of PIT tags per group */
            for ( j = 0; j < 4; j++ )
            {
                /* If no PIT tag in the current group, no need to search and read corresponding file => skip the current iteration */
                if ( 0 == appDataPitTag.num_pit_tag_per_group[j] || ALL_PIT_TAG_DENIED == appDataPitTag.num_pit_tag_per_group[j] || ALL_PIT_TAG_ACCEPTED == appDataPitTag.num_pit_tag_per_group[j] )
                {
                    /* If it's the first group*/
                    if ( 0 == j )
                    {
                        cumulative_sum = 0;
                        appDataAttractiveLeds.pattern_one_led_groups[j] = 0;
                    }
                        /* Otherwise */
                    else
                    {
                        cumulative_sum += appDataPitTag.num_pit_tag_per_group[j - 1];
                        appDataAttractiveLeds.pattern_one_led_groups[j] = appDataAttractiveLeds.pattern_one_led_groups[j - 1];
                    }
                    continue;
                }

                /* Set the name of the file to read (e.g. PTONE1.TXT, PTONE2.TXT, PTONE3.TXT? */
                snprintf( buf, 13, "PTONE%d.TXT", j + 1 );

                /* Open file */
                if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, buf, FILEIO_OPEN_READ ) )
                {
                    errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf( "unable to open PIT tags group %d file (%u)", j + 1, errF );
#endif 
                    sprintf( appError.message, "Unable to open PIT tags group %d file (%u)", j + 1, errF );
                    appError.current_line_number = __LINE__;
                    sprintf( appError.current_file_name, "%s", __FILE__ );
                    FILEIO_ErrorClear( 'A' );
                    if ( 0 == j )
                    {
                        appError.number = ERROR_PIT_TAGS_PTONE1_FILE_OPEN;
                    }
                    else if ( 1 == j )
                    {
                        appError.number = ERROR_PIT_TAGS_PTONE2_FILE_OPEN;
                    }
                    else if ( 2 == j )
                    {
                        appError.number = ERROR_PIT_TAGS_PTONE3_FILE_OPEN;
                    }
                    else
                    {
                        appError.number = ERROR_PIT_TAGS_PTONE4_FILE_OPEN;
                    }
                    return FILEIO_RESULT_FAILURE;
                }

                if ( 0 == j )
                {
                    cumulative_sum = 0;
                    appDataAttractiveLeds.pattern_one_led_groups[j] = appDataPitTag.num_pit_tag_per_group[j];
                }
                else
                {
                    cumulative_sum += appDataPitTag.num_pit_tag_per_group[j - 1];
                    appDataAttractiveLeds.pattern_one_led_groups[j] = appDataAttractiveLeds.pattern_one_led_groups[j - 1] + appDataPitTag.num_pit_tag_per_group[j];
                }

                for ( i = 0; i < appDataPitTag.num_pit_tag_per_group[j]; i++ )
                {
                    FILEIO_Read( appDataPitTag.pit_tags_list[i + cumulative_sum], 1, 10, &file );
                    appDataPitTag.pit_tags_list[i + cumulative_sum][11] = '\0';
                    appDataPitTag.num_pit_tag_stored += 1;
                }

                if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
                {
                    errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                    printf( "unable to close PIT tags group %d file (%u)", j + 1, errF );
#endif 
                    sprintf( appError.message, "Unable to close PIT tags group %d file (%u)", j + 1, errF );
                    appError.current_line_number = __LINE__;
                    sprintf( appError.current_file_name, "%s", __FILE__ );
                    FILEIO_ErrorClear( 'A' );
                    if ( 0 == j )
                    {
                        appError.number = ERROR_PIT_TAGS_PTONE1_FILE_CLOSE;
                    }
                    else if ( 1 == j )
                    {
                        appError.number = ERROR_PIT_TAGS_PTONE2_FILE_CLOSE;
                    }
                    else if ( 2 == j )
                    {
                        appError.number = ERROR_PIT_TAGS_PTONE3_FILE_CLOSE;
                    }
                    else
                    {
                        appError.number = ERROR_PIT_TAGS_PTONE4_FILE_CLOSE;
                    }
                    return FILEIO_RESULT_FAILURE;
                }

            }
        }
    }
    else if ( COLOR_ASSOCIATIVE_LEARNING == appData.scenario_number )
    {
        if ( ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_denied_or_color_A && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_denied_or_color_A )
        {

            if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, "PTCOLORA.TXT", FILEIO_OPEN_READ ) )
            {
                errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                printf( "unable to open PIT tags color A file (%u)", errF );
#endif 
                sprintf( appError.message, "Unable to open PIT tags color A file (%u)", errF );
                appError.current_line_number = __LINE__;
                sprintf( appError.current_file_name, "%s", __FILE__ );
                FILEIO_ErrorClear( 'A' );
                appError.number = ERROR_PIT_TAGS_COLOR_A_FILE_OPEN;
                return FILEIO_RESULT_FAILURE;
            }

            for ( i = 0; i < appDataPitTag.num_pit_tag_denied_or_color_A; i++ )
            {
                FILEIO_Read( appDataPitTag.pit_tags_list[i], 1, 10, &file );
                appDataPitTag.pit_tags_list[i][11] = '\0';
                appDataPitTag.num_pit_tag_stored += 1;
            }

            if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
            {
                errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                printf( "unable to close PIT tags color A file (%u)", errF );
#endif 
                sprintf( appError.message, "Unable to close PIT tags color A file (%u)", errF );
                appError.current_line_number = __LINE__;
                sprintf( appError.current_file_name, "%s", __FILE__ );
                FILEIO_ErrorClear( 'A' );
                appError.number = ERROR_PIT_TAGS_COLOR_A_FILE_CLOSE;
                return FILEIO_RESULT_FAILURE;
            }

        }

        if ( ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_accepted_or_color_B && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_accepted_or_color_B )
        {

            if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, "PTCOLORB.TXT", FILEIO_OPEN_READ ) )
            {
                errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                printf( "unable to open PIT tags color B file (%u)", errF );
#endif 
                sprintf( appError.message, "Unable to open PIT tags color B file (%u)", errF );
                appError.current_line_number = __LINE__;
                sprintf( appError.current_file_name, "%s", __FILE__ );
                FILEIO_ErrorClear( 'A' );
                appError.number = ERROR_PIT_TAGS_COLOR_B_FILE_OPEN;
                return FILEIO_RESULT_FAILURE;
            }

            for ( i = 0; i < appDataPitTag.num_pit_tag_accepted_or_color_B; i++ )
            {
                FILEIO_Read( appDataPitTag.pit_tags_list[i + appDataPitTag.num_pit_tag_denied_or_color_A], 1, 10, &file );
                appDataPitTag.pit_tags_list[i + appDataPitTag.num_pit_tag_denied_or_color_A][11] = '\0';
                appDataPitTag.num_pit_tag_stored += 1;
            }

            if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
            {
                errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                printf( "unable to close PIT tags color B file (%u)", errF );
#endif 
                sprintf( appError.message, "Unable to close PIT tags color B file (%u)", errF );
                appError.current_line_number = __LINE__;
                sprintf( appError.current_file_name, "%s", __FILE__ );
                FILEIO_ErrorClear( 'A' );
                appError.number = ERROR_PIT_TAGS_COLOR_B_FILE_CLOSE;
                return FILEIO_RESULT_FAILURE;
            }
        }
    }
    else
    {
        if ( ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_denied_or_color_A && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_denied_or_color_A )
        {

            if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, "PTDENIED.TXT", FILEIO_OPEN_READ ) )
            {
                errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                printf( "unable to open PIT tags denied file (%u)", errF );
#endif 
                sprintf( appError.message, "Unable to open PIT tags denied file (%u)", errF );
                appError.current_line_number = __LINE__;
                sprintf( appError.current_file_name, "%s", __FILE__ );
                FILEIO_ErrorClear( 'A' );
                appError.number = ERROR_PIT_TAGS_DENIED_FILE_OPEN;
                return FILEIO_RESULT_FAILURE;
            }

            for ( i = 0; i < appDataPitTag.num_pit_tag_denied_or_color_A; i++ )
            {
                FILEIO_Read( appDataPitTag.pit_tags_list[i], 1, 10, &file );
                appDataPitTag.pit_tags_list[i][11] = '\0';
                appDataPitTag.num_pit_tag_stored += 1;
            }

            if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
            {
                errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                printf( "unable to close PIT tags denied file (%u)", errF );
#endif 
                sprintf( appError.message, "Unable to close PIT tags denied file (%u)", errF );
                appError.current_line_number = __LINE__;
                sprintf( appError.current_file_name, "%s", __FILE__ );
                FILEIO_ErrorClear( 'A' );
                appError.number = ERROR_PIT_TAGS_DENIED_FILE_CLOSE;
                return FILEIO_RESULT_FAILURE;
            }

        }

        if ( ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_accepted_or_color_B && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_accepted_or_color_B )
        {

            if ( FILEIO_RESULT_FAILURE == FILEIO_Open( &file, "PTACCEPT.TXT", FILEIO_OPEN_READ ) )
            {
                errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                printf( "unable to open PIT tags accepted file (%u)", errF );
#endif 
                sprintf( appError.message, "Unable to open PIT tags accepted file (%u)", errF );
                appError.current_line_number = __LINE__;
                sprintf( appError.current_file_name, "%s", __FILE__ );
                FILEIO_ErrorClear( 'A' );
                appError.number = ERROR_PIT_TAGS_ACCEPTED_FILE_OPEN;
                return FILEIO_RESULT_FAILURE;
            }

            for ( i = 0; i < appDataPitTag.num_pit_tag_accepted_or_color_B; i++ )
            {
                FILEIO_Read( appDataPitTag.pit_tags_list[i + appDataPitTag.num_pit_tag_denied_or_color_A], 1, 10, &file );
                appDataPitTag.pit_tags_list[i + appDataPitTag.num_pit_tag_denied_or_color_A][11] = '\0';
                appDataPitTag.num_pit_tag_stored += 1;
            }

            if ( FILEIO_RESULT_FAILURE == FILEIO_Close( &file ) )
            {
                errF = FILEIO_ErrorGet( 'A' );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
                printf( "unable to close PIT tags accepted file (%u)", errF );
#endif 
                sprintf( appError.message, "Unable to close PIT tags accepted file (%u)", errF );
                appError.current_line_number = __LINE__;
                sprintf( appError.current_file_name, "%s", __FILE__ );
                FILEIO_ErrorClear( 'A' );
                appError.number = ERROR_PIT_TAGS_ACCEPTED_FILE_CLOSE;
                return FILEIO_RESULT_FAILURE;
            }
        }
    }

    return FILEIO_RESULT_SUCCESS;

}


FILEIO_RESULT config_find_ini( void )
{

    FILEIO_SEARCH_RECORD searchRecord;

    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
        store_event( OF_FIND_INI );
    }

    return FILEIO_Find( "CONFIG.INI", FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, true );
}


INI_READ_STATE config_read_ini( void )
{

    int32_t read_parameter;
    int s, i;
    char str[20];
    bool flag = false;

    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
        store_event( OF_READ_INI );
    }

    /* Scenario number */
    read_parameter = ini_getl( "scenario", "num", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_SCENARIO_NUM;
    }
    else
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tScenario number... read.\n" );
#endif 
        appData.scenario_number = ( uint8_t ) read_parameter;
    }

    /* Site identification. */
    s = ini_gets( "siteid", "zone", "XXXX", appData.siteid, sizearray( appData.siteid ), "CONFIG.INI" );
    for ( i = s; i < 4; i++ )
    {
        appData.siteid[i] = 'X';
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
    printf( "\tSite ID... read.\n" );
#endif 

    /* Wake up time. */
    read_parameter = ini_getl( "time", "wakeup_hour", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_TIME_WAKEUP_HOUR;
    }
    else
    {
        appDataAlarmWakeup.time.tm_hour = ( int ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tTime Wakeup hour... read.\n" );
#endif     
    }
    read_parameter = ini_getl( "time", "wakeup_minute", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_TIME_WAKEUP_MINUTE;
    }
    else
    {
        appDataAlarmWakeup.time.tm_min = ( int ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tTime Wakeup minute... read.\n" );
#endif
    }
    appDataAlarmWakeup.time.tm_sec = 0;

    /* Sleep time. */
    read_parameter = ini_getl( "time", "sleep_hour", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_TIME_SLEEP_HOUR;
    }
    else
    {
        appDataAlarmSleep.time.tm_hour = ( int ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tTime sleep hour... read.\n" );
#endif
    }
    read_parameter = ini_getl( "time", "sleep_minute", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_TIME_SLEEP_MINUTE;
    }
    else
    {
        appDataAlarmSleep.time.tm_min = ( int ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tTime sleep minute... read.\n" );
#endif
    }
    appDataAlarmSleep.time.tm_sec = 0;

    /* Attractive LEDs Color. */
    appData.flags.bit_value.attractive_leds_status = 0;
    /* Check if "attractiveleds" is present in the INI file */
    for ( s = 0; ini_getsection( s, str, 20, "CONFIG.INI" ) > 0; s++ )
    {
        if ( 0 == strcmp( str, "attractiveleds" ) )
        {
            appData.flags.bit_value.attractive_leds_status = true;
        }
    }

    if ( true == appData.flags.bit_value.attractive_leds_status )
    {
        read_parameter = ini_getl( "attractiveleds", "red_a", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_ATTRACTIVE_LEDS_RED_A;
        }
        else
        {
            appDataAttractiveLeds.red[0] = ( uint8_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tAttractive LEDs red A... read.\n" );
#endif
        }
        read_parameter = ini_getl( "attractiveleds", "green_a", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_ATTRACTIVE_LEDS_GREEN_A;
        }
        else
        {
            appDataAttractiveLeds.green[0] = ( uint8_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tAttractive LEDs green A... read.\n" );
#endif
        }
        read_parameter = ini_getl( "attractiveleds", "blue_a", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_ATTRACTIVE_LEDS_BLUE_A;
        }
        else
        {
            appDataAttractiveLeds.blue[0] = ( uint8_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tAttractive LEDs blue A... read.\n" );
#endif
        }

        if ( appData.scenario_number == COLOR_ASSOCIATIVE_LEARNING )
        {
            read_parameter = ini_getl( "attractiveleds", "red_b", -1, "CONFIG.INI" );
            if ( -1 == read_parameter )
            {
                return INI_PB_ATTRACTIVE_LEDS_RED_B;
            }
            else
            {
                appDataAttractiveLeds.red[1] = ( uint8_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                printf( "\tAttractive LEDs red B... read.\n" );
#endif
            }
            read_parameter = ini_getl( "attractiveleds", "green_b", -1, "CONFIG.INI" );
            if ( -1 == read_parameter )
            {
                return INI_PB_ATTRACTIVE_LEDS_GREEN_B;
            }
            else
            {
                appDataAttractiveLeds.green[1] = ( uint8_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                printf( "\tAttractive LEDs green B... read.\n" );
#endif
            }
            read_parameter = ini_getl( "attractiveleds", "blue_b", -1, "CONFIG.INI" );
            if ( -1 == read_parameter )
            {
                return INI_PB_ATTRACTIVE_LEDS_BLUE_B;
            }
            else
            {
                appDataAttractiveLeds.blue[1] = ( uint8_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                printf( "\tAttractive LEDs blue B... read.\n" );
#endif
            }
        }

        if ( GO_NO_GO == appData.scenario_number || COLOR_ASSOCIATIVE_LEARNING == appData.scenario_number )
        {
            /* Attractive LEDs alternate delay. */
            read_parameter = ini_getl( "attractiveleds", "alt_delay", -1, "CONFIG.INI" );
            if ( -1 == read_parameter )
            {
                return INI_PB_ATTRACTIVE_LEDS_ALT_DELAY;
            }
            else
            {
                appDataAttractiveLeds.alt_delay = ( uint8_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                printf( "\tAttractive LEDs alternate delay... read.\n" );
#endif
            }
        }

        /* Attractive LEDs wake up time. */
        read_parameter = ini_getl( "attractiveleds", "on_hour", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_ATTRACTIVE_LEDS_ON_HOUR;
        }
        else
        {
            appDataAttractiveLeds.wake_up_time.tm_hour = ( int ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tAttractive LEDs on hour... read.\n" );
#endif
        }
        read_parameter = ini_getl( "attractiveleds", "on_minute", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_ATTRACTIVE_LEDS_ON_MINUTE;
        }
        else
        {
            appDataAttractiveLeds.wake_up_time.tm_min = ( int ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tAttractive LEDs on minute... read.\n" );
#endif
        }

        appDataAttractiveLeds.wake_up_time.tm_sec = 0;

        /* Attractive LEDs sleep time. */
        read_parameter = ini_getl( "attractiveleds", "off_hour", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_ATTRACTIVE_LEDS_OFF_HOUR;
        }
        else
        {
            appDataAttractiveLeds.sleep_time.tm_hour = ( int ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tAttractive LEDs off hour... read.\n" );
#endif
        }
        read_parameter = ini_getl( "attractiveleds", "off_minute", -1, "CONFIG.INI" );
        if ( read_parameter == -1 )
        {
            return INI_PB_ATTRACTIVE_LEDS_OFF_MINUTE;
        }
        else
        {
            appDataAttractiveLeds.sleep_time.tm_min = ( int ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tAttractive LEDs off minute... read.\n" );
#endif
        }
        appDataAttractiveLeds.sleep_time.tm_sec = 0;

        if ( GO_NO_GO == appData.scenario_number )
        {
            read_parameter = ini_getl( "attractiveleds", "pattern", -1, "CONFIG.INI" );
            if ( read_parameter == -1 )
            {
                return INI_PB_ATTRACTIVE_LEDS_PATTERN;
            }
            else
            {
                appDataAttractiveLeds.pattern_number = ( uint8_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                printf( "\tAttractive LEDs pattern number... read.\n" );
#endif
            }

            if ( ALL_LEDS == appDataAttractiveLeds.pattern_number )
            {

                read_parameter = ini_getl( "attractiveleds", "pattern_percent", -1, "CONFIG.INI" );
                if ( read_parameter == -1 )
                {
                    return INI_PB_ATTRACTIVE_LEDS_PATTERN_PERCENT;
                }
                else
                {
                    appDataAttractiveLeds.pattern_percent = ( uint8_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                    printf( "\tAttractive LEDs pattern percent... read.\n" );
#endif
                }

                //                ini_gets( "attractiveleds", "pattern_percent", "1.0", str, sizearray( str ), "CONFIG.INI" );
                //                appDataAttractiveLeds.pattern_percent = atof(str);
                //#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                //                printf( "\tAttractive LEDs pattern percent... read.\n" );
                //#endif
            }

        }
    }

    if ( appData.scenario_number > DOOR_HABITUATION )
    {
        if ( GO_NO_GO == appData.scenario_number )
        {
            if ( LEFT_RIGHT_LEDS == appDataAttractiveLeds.pattern_number )
            {
                /* PIT tags associated with left LEDs. */
                read_parameter = ini_getl( "pittags", "num_left", -1, "CONFIG.INI" );
                if ( -1 == read_parameter )
                {
                    return INI_PB_PIT_TAG_LEFT;
                }
                else
                {
                    appDataPitTag.num_pit_tag_denied_or_color_A = ( int16_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                    printf( "\tPIT tags number of left... read.\n" );
#endif
                }
                /* PIT tags associated with left LEDs. */
                read_parameter = ini_getl( "pittags", "num_right", -1, "CONFIG.INI" );
                if ( -1 == read_parameter )
                {
                    return INI_PB_PIT_TAG_RIGHT;
                }
                else
                {
                    appDataPitTag.num_pit_tag_accepted_or_color_B = ( int16_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                    printf( "\tPIT tags number of right... read.\n" );
#endif
                }

                if ( ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_denied_or_color_A && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_denied_or_color_A )
                {
                    for ( i = 0; i < appDataPitTag.num_pit_tag_denied_or_color_A; i++ )
                    {
                        appDataPitTag.is_pit_tag_denied_or_color_A[i] = true;
                    }
                }
            }
            else if ( TOP_BOTTOM_LEDS == appDataAttractiveLeds.pattern_number )
            {
                /* PIT tags associated with top LEDs. */
                read_parameter = ini_getl( "pittags", "num_top", -1, "CONFIG.INI" );
                if ( -1 == read_parameter )
                {
                    return INI_PB_PIT_TAG_TOP;
                }
                else
                {
                    appDataPitTag.num_pit_tag_denied_or_color_A = ( int16_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                    printf( "\tPIT tags number of top... read.\n" );
#endif
                }
                /* PIT tags associated with bottom LEDs. */
                read_parameter = ini_getl( "pittags", "num_bottom", -1, "CONFIG.INI" );
                if ( -1 == read_parameter )
                {
                    return INI_PB_PIT_TAG_BOTTOM;
                }
                else
                {
                    appDataPitTag.num_pit_tag_accepted_or_color_B = ( int16_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                    printf( "\tPIT tags number of bottom... read.\n" );
#endif
                }

                if ( ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_denied_or_color_A && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_denied_or_color_A )
                {
                    for ( i = 0; i < appDataPitTag.num_pit_tag_denied_or_color_A; i++ )
                    {
                        appDataPitTag.is_pit_tag_denied_or_color_A[i] = true;
                    }
                }
            }
            else if ( ONE_LED == appDataAttractiveLeds.pattern_number )
            {
                appDataPitTag.num_pit_tag_per_group[0] = 0;
                appDataPitTag.num_pit_tag_per_group[1] = 0;
                appDataPitTag.num_pit_tag_per_group[2] = 0;
                appDataPitTag.num_pit_tag_per_group[3] = 0;

                /* PIT tags associated with group LEDs 1. */
                read_parameter = ini_getl( "pittags", "num_led_1", -1, "CONFIG.INI" );
                if ( -1 == read_parameter )
                {
                    return INI_PB_PIT_TAG_LED_1;
                }
                else
                {
                    appDataPitTag.num_pit_tag_per_group[0] = ( int16_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                    printf( "\tPIT tags group 1 number... read.\n" );
#endif
                }
                /* PIT tags associated with group LEDs 2. */
                read_parameter = ini_getl( "pittags", "num_led_2", -1, "CONFIG.INI" );
                if ( -1 == read_parameter )
                {
                    return INI_PB_PIT_TAG_LED_2;
                }
                else
                {
                    appDataPitTag.num_pit_tag_per_group[1] = ( int16_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                    printf( "\tPIT tags group 2 number... read.\n" );
#endif
                }
                /* PIT tags associated with group LEDs 3. */
                read_parameter = ini_getl( "pittags", "num_led_3", -1, "CONFIG.INI" );
                if ( -1 == read_parameter )
                {
                    return INI_PB_PIT_TAG_LED_3;
                }
                else
                {
                    appDataPitTag.num_pit_tag_per_group[2] = ( int16_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                    printf( "\tPIT tags group 3 number... read.\n" );
#endif
                }
                /* PIT tags associated with group LEDs 4. */
                read_parameter = ini_getl( "pittags", "num_led_4", -1, "CONFIG.INI" );
                if ( -1 == read_parameter )
                {
                    return INI_PB_PIT_TAG_LED_4;
                }
                else
                {
                    appDataPitTag.num_pit_tag_per_group[3] = ( int16_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                    printf( "\tPIT tags group 4 number... read.\n" );
#endif
                }
            }
        }
        else if ( COLOR_ASSOCIATIVE_LEARNING == appData.scenario_number )
        {
            /* PIT tags associated with color A. */
            read_parameter = ini_getl( "pittags", "num_color_A", -1, "CONFIG.INI" );
            if ( -1 == read_parameter )
            {
                return INI_PB_PIT_TAG_COLOR_A;
            }
            else
            {
                appDataPitTag.num_pit_tag_denied_or_color_A = ( int16_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                printf( "\tPIT tags number of color A... read.\n" );
#endif
            }
            /* PIT tags associated with color B. */
            read_parameter = ini_getl( "pittags", "num_color_B", -1, "CONFIG.INI" );
            if ( -1 == read_parameter )
            {
                return INI_PB_PIT_TAG_COLOR_B;
            }
            else
            {
                appDataPitTag.num_pit_tag_accepted_or_color_B = ( int16_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                printf( "\tPIT tags number of color B... read.\n" );
#endif
            }

            if ( ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_denied_or_color_A && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_denied_or_color_A )
            {
                for ( i = 0; i < appDataPitTag.num_pit_tag_denied_or_color_A; i++ )
                {
                    appDataPitTag.is_pit_tag_denied_or_color_A[i] = true;
                }
            }

        }
        else
        {
            /* PIT tags denied. */
            read_parameter = ini_getl( "pittags", "num_denied", -1, "CONFIG.INI" );
            if ( -1 == read_parameter )
            {
                return INI_PB_PIT_TAG_DENIED;
            }
            else
            {
                appDataPitTag.num_pit_tag_denied_or_color_A = ( int16_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                printf( "\tPIT tags number of denied... read.\n" );
#endif
            }
            /* PIT tags accepted. */
            read_parameter = ini_getl( "pittags", "num_accepted", -1, "CONFIG.INI" );
            if ( -1 == read_parameter )
            {
                return INI_PB_PIT_TAG_ACCEPTED;
            }
            else
            {
                appDataPitTag.num_pit_tag_accepted_or_color_B = ( int16_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                printf( "\tPIT tags number of accepted... read.\n" );
#endif
            }

            if ( ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_denied_or_color_A && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_denied_or_color_A )
            {
                for ( i = 0; i < appDataPitTag.num_pit_tag_denied_or_color_A; i++ )
                {
                    appDataPitTag.is_pit_tag_denied_or_color_A[i] = true;
                }
            }
        }
    }

    /* Door/servomotor configuration. */
    read_parameter = ini_getl( "door", "close_position", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_CLOSE_POSITION;
    }
    else
    {
        if ( read_parameter >= SERVO_DEFAULT_MIN_POSITION )
        {
            appDataServo.ton_min = ( uint16_t ) read_parameter;
            appDataServo.ton_min_night = appDataServo.ton_min;
        }
        else
        {
            appDataServo.ton_min = SERVO_DEFAULT_MIN_POSITION;
            appDataServo.ton_min_night = appDataServo.ton_min;
        }
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tDoor close position... read.\n" );
#endif
    }

    read_parameter = ini_getl( "door", "open_position", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_OPEN_POSITION;
    }
    else
    {
        if ( read_parameter <= SERVO_DEFAULT_MAX_POSITION )
        {
            appDataServo.ton_max = ( uint16_t ) read_parameter;
        }
        else
        {
            appDataServo.ton_max = SERVO_DEFAULT_MAX_POSITION;
        }
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tDoor open position... read.\n" );
#endif
    }
    /* Closing speed */
    read_parameter = ini_getl( "door", "closing_speed", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_CLOSING_SPEED;
    }
    else
    {
        appDataServo.closing_speed = ( uint8_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tDoor close speed... read.\n" );
#endif
    }
    /* Opening speed */
    read_parameter = ini_getl( "door", "opening_speed", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_OPENING_SPEED;
    }
    else
    {
        appDataServo.opening_speed = ( uint8_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tDoor opening speed... read.\n" );
#endif
    }
    /* Door open/closee delays. */
    read_parameter = ini_getl( "door", "open_delay", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_OPEN_DELAY;
    }
    else
    {
        appDataDoor.open_delay = ( uint16_t ) read_parameter * 1000;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tDoor open delay... read.\n" );
#endif
    }
    read_parameter = ini_getl( "door", "close_delay", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_CLOSE_DELAY;
    }
    else
    {
        appDataDoor.close_delay = ( uint16_t ) read_parameter * 1000;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tDoor close delay... read.\n" );
#endif
    }

    /* Door habituation */
    if ( DOOR_HABITUATION == appData.scenario_number )
    {
        read_parameter = ini_getl( "door", "habituation", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_DOOR_HABITUATION;
        }
        else
        {
            appDataDoor.habituation_percent = ( uint8_t ) read_parameter;
            //            appDataServo.ton_max = (appDataServo.ton_max-appDataServo.ton_min)/100*appDataDoor.habituation_percent+appDataServo.ton_min;
            appDataServo.ton_min = ( appDataServo.ton_max - appDataServo.ton_min ) / 100 * appDataDoor.habituation_percent + appDataServo.ton_min;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tDoor habituation... read.\n" );
#endif
        }
    }
    /* Door remain open */
    read_parameter = ini_getl( "door", "remain_open", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_REMAIN_OPEN;
    }
    else
    {
        appDataDoor.remain_open = ( uint8_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tDoor remain opened... read.\n" );
#endif
    }
    //    /* Door reward probability */
    ////    read_parameter = ini_getf( "door", "reward_probability", -1, "CONFIG.INI" );
    //    ini_gets( "door", "reward_probability", "1.0", str, sizearray( str ), "CONFIG.INI" );
    //    appDataDoor.reward_probability = atof(str);

    /* Door open time. */
    read_parameter = ini_getl( "door", "open_hour", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_OPEN_HOUR;
    }
    else
    {
        appDataDoor.open_time.tm_hour = ( int ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tDoor open hour... read.\n" );
#endif
    }
    read_parameter = ini_getl( "door", "open_minute", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_OPEN_MINUTE;
    }
    else
    {
        appDataDoor.open_time.tm_min = ( int ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tDoor open minute... read.\n" );
#endif
    }
    appDataDoor.open_time.tm_sec = 0;

    /* Door close time. */
    read_parameter = ini_getl( "door", "close_hour", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_CLOSE_HOUR;
    }
    else
    {
        appDataDoor.close_time.tm_hour = ( int ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tDoor close hour... read.\n" );
#endif
    }
    read_parameter = ini_getl( "door", "close_minute", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_DOOR_CLOSE_MINUTE;
    }
    else
    {
        appDataDoor.close_time.tm_min = ( int ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tDoor close minute... read.\n" );
#endif
    }
    appDataDoor.close_time.tm_sec = 0;

    /* Logs */
    /* Check if "logs" section is present in the INI file */
    flag = false;
    for ( s = 0; ini_getsection( s, str, 20, "CONFIG.INI" ) > 0; s++ )
    {
        if ( 0 == strcmp( str, "logs" ) )
        {
            flag = true;
        }
    }

    if ( flag )
    {
        /* Data separator in the log file. */
        ini_gets( "logs", "separator", DEFAULT_LOG_SEPARATOR, appDataLog.separator, sizearray( appDataLog.separator ), "CONFIG.INI" );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tLogs separator... read.\n" );
#endif
        //        read_parameter = ini_getl( "logs", "birds", -1, "CONFIG.INI" );
        //        if ( -1 == read_parameter )
        //        {
        //            return INI_PB_LOGS_BIRDS;
        //        }
        //        else
        //        {
        //            appDataLog.log_data = ( bool ) read_parameter;
        //        }
        read_parameter = ini_getl( "logs", "udid", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            appDataLog.log_udid = true;
            //            return INI_PB_LOGS_UDID;
        }
        else
        {
            appDataLog.log_udid = ( bool ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tLogs UDID... read.\n" );
#endif
        }
        read_parameter = ini_getl( "logs", "events", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            appDataLog.log_events = true;
            //            return INI_PB_LOGS_EVENTS;
        }
        else
        {
            appDataLog.log_events = ( bool ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tLogs events... read.\n" );
#endif
        }
        read_parameter = ini_getl( "logs", "errors", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            appDataLog.log_errors = true;
            //            return INI_PB_LOGS_ERRORS;
        }
        else
        {
            appDataLog.log_errors = ( bool ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tLogs errors... read.\n" );
#endif
        }
        read_parameter = ini_getl( "logs", "battery", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            appDataLog.log_battery = true;
            //            return INI_PB_LOGS_BATTERY;
        }
        else
        {
            appDataLog.log_battery = ( bool ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tLogs battery... read.\n" );
#endif
        }
        read_parameter = ini_getl( "logs", "rfid", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            appDataLog.log_rfid = true;
            //            return INI_PB_LOGS_RFID;
        }
        else
        {
            appDataLog.log_rfid = ( bool ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tLogs RFID... read.\n" );
#endif
        }
        read_parameter = ini_getl( "logs", "temperature", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            //            return INI_PB_LOGS_RFID;
            appDataLog.log_temp = true;
        }
        else
        {
            appDataLog.log_temp = ( bool ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tLogs temperature... read.\n" );
#endif
        }
        read_parameter = ini_getl( "logs", "calibration", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            //            return INI_PB_LOGS_RFID;
            appDataLog.log_calibration = true;
        }
        else
        {
            appDataLog.log_calibration = ( bool ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tLogs calibration... read.\n" );
#endif
        }
    }
    else
    {
        /* Data separator in the log file. */
        ini_gets( "logfile", "separator", DEFAULT_LOG_SEPARATOR, appDataLog.separator, sizearray( appDataLog.separator ), "CONFIG.INI" );
        appDataLog.log_birds = true;
        appDataLog.log_udid = true;
        appDataLog.log_events = true;
        appDataLog.log_errors = true;
        appDataLog.log_battery = true;
        appDataLog.log_rfid = true;
        appDataLog.log_temp = true;
        appDataLog.log_calibration = true;
    }

    /* Reward. */
    read_parameter = ini_getl( "reward", "enable", -1, "CONFIG.INI" );
    if ( -1 == read_parameter )
    {
        return INI_PB_REWARD_ENABLE;
    }
    else
    {
        appData.reward_enable = ( uint8_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf( "\tReward enable... read.\n" );
#endif
    }

    if ( 1 == appData.reward_enable )
    {
        /* Timeout taking reward. */
        read_parameter = ini_getl( "reward", "timeout", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_REWARD_TIMEOUT;
        }
        else
        {
            appData.timeout_taking_reward = ( uint16_t ) read_parameter * 1000;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tReward timeout... read.\n" );
#endif
        }
    }
    else
    {
        appData.timeout_taking_reward = 0;
    }

    if ( appData.scenario_number > DOOR_HABITUATION && appData.scenario_number != PATCH_PROBABILITY )
    {
        /* Reward probability */
        read_parameter = ini_getl( "reward", "probability", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_REWARD_PROBABILITY;
        }
        else
        {
            appDataDoor.reward_probability = ( uint8_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tReward probability... read.\n" );
#endif
        }
    }
    else
    {
        appDataDoor.reward_probability = 100;
    }

    /* Timeout before standby. */
    appData.timeout_standby = 0;
    appData.timeout_pir = 0;

    if ( appData.scenario_number == PATCH_PROBABILITY )
    {
        /* Reward probability */
        read_parameter = ini_getl( "timeouts", "unique_visit", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_TIMEOUTS_UNIQUE_VISIT;
        }
        else
        {
            appDataPitTag.timeout_unique_visit = ( uint8_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tTimeout unique visit... read.\n" );
#endif
        }
    }

    //    read_parameter = ini_getl( "timeouts", "sleep", -1, "CONFIG.INI" );
    //    if ( -1 == read_parameter )
    //    {
    //        return INI_PB_TIMEOUTS_SLEEP;
    //    }
    //    else
    //    {
    //        appData.timeout_standby = ( uint16_t ) read_parameter * 1000;
    //#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
    //        printf( "\tTimeout standby... read.\n" );
    //#endif
    //    }
    //    /* Timeout before pir. */
    //    read_parameter = ini_getl( "timeouts", "pir", -1, "CONFIG.INI" );
    //    if ( -1 == read_parameter )
    //    {
    //        return INI_PB_TIMEOUTS_PIR;
    //    }
    //    else
    //    {
    //        appData.timeout_pir = ( uint16_t ) read_parameter * 1000;
    //#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
    //        printf( "\tTimeout problem PIR... read.\n" );
    //#endif
    //    }

    /* Timeout guillotine. */
    appData.timeout_guillotine = ( appDataServo.ton_max - appDataServo.ton_min ) / appDataServo.closing_speed * ( PR3 / 1000 ) + 500;

    if ( appData.scenario_number > DOOR_HABITUATION )
    {
        /* Punishment delay. */
        read_parameter = ini_getl( "punishment", "delay", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_PUNISHMENT_DELAY;
        }
        else
        {
            appData.punishment_delay = ( uint16_t ) read_parameter * 1000;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tPunishment delay... read.\n" );
#endif
        }

        if ( PATCH_PROBABILITY == appData.scenario_number )
        {
            /* Punishment probability treshold */
            read_parameter = ini_getl( "punishment", "proba_threshold", -1, "CONFIG.INI" );
            if ( -1 == read_parameter )
            {
                return INI_PB_PUNISHMENT_PROBA_THRESH;
            }
            else
            {
                appData.punishment_proba_thresh = ( uint8_t ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                printf( "\tPunishment probability threshold... read.\n" );
#endif
            }
        }
        else
        {
            appData.punishment_proba_thresh = 0;
        }
    }

    /* Security */
    /* Check if "security" section is present in the INI file */
    flag = false;
    for ( s = 0; ini_getsection( s, str, 20, "CONFIG.INI" ) > 0; s++ )
    {
        if ( 0 == strcmp( str, "security" ) )
        {
            flag = true;
        }
    }

    if ( flag )
    {
        read_parameter = ini_getl( "security", "bird_reward_reopen", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_SECURITY_BIRD_REWARD_REOPEN;
        }
        else
        {
            appData.secu_bird_reward_reopen = ( bool ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tSecurity bird reward reopen... read.\n" );
#endif
        }
    }
    else
    {
        appData.secu_bird_reward_reopen = true;
    }

    /* Check */
    /* Check if "check" section is present in the INI file */
    flag = false;
    for ( s = 0; ini_getsection( s, str, 20, "CONFIG.INI" ) > 0; s++ )
    {
        if ( 0 == strcmp( str, "check" ) )
        {
            flag = true;
        }
    }

    if ( flag )
    {
        read_parameter = ini_getl( "check", "food_level", -1, "CONFIG.INI" );
        if ( -1 == read_parameter )
        {
            return INI_PB_CHECK_FOOD_LEVEL;
        }
        else
        {
            appData.chk_food_level = ( bool ) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf( "\tCheck food level... read.\n" );
#endif
        }
    }
    else
    {
        appData.chk_food_level = false;
    }

    return INI_READ_OK;
}


void config_print( void )
{
    int i, j;

    printf( "Configuration parameters\n" );

    printf( "\tScenario\n\t\tNumber: %u\n\t\tTitle:",
            getCompletScenarioNumber( ) );
    switch ( appData.scenario_number )
    {
        case NO_SCENARIO:
            printf( " no scenario\n" );
            break;
        case OPEN_BAR:
            printf( " open-bar\n" );
            break;
        case GO_NO_GO:
            if ( 0 == appDataAttractiveLeds.pattern_number )
            {
                printf( " go-no go, all LEDs (pattern 0)\n" );
            }
            else if ( 1 == appDataAttractiveLeds.pattern_number )
            {
                printf( " go-no go, left/right LEDs (pattern 1)\n" );
            }
            else if ( 2 == appDataAttractiveLeds.pattern_number )
            {
                printf( " go-no go, top/bottom LEDs (pattern 2)\n" );
            }
            else if ( 3 == appDataAttractiveLeds.pattern_number )
            {
                printf( " go-no go, one LED (pattern 3)\n" );
            }
            break;
        case LONG_TERM_SPATIAL_MEMORY:
            printf( " long term spatial memory\n" );
            break;
        case WORKING_SPATIAL_MEMORY:
            printf( " working spatial memory\n" );
            break;
        case COLOR_ASSOCIATIVE_LEARNING:
            printf( " color associative learning\n" );
            break;
        case DOOR_HABITUATION:
            printf( " door habituation\n" );
            break;
        case RISK_AVERSION:
            printf( " risk aversion\n" );
            break;
        case PATCH_PROBABILITY:
            printf( " patch probability\n" );
            break;
    }

    printf( "\tSite ID\n\t\tZone: %s\n",
            appData.siteid );

    printf( "\tTime\n" );
    printf( "\t\tWake up: %02d:%02d\n",
            appDataAlarmWakeup.time.tm_hour,
            appDataAlarmWakeup.time.tm_min );
    printf( "\t\tSleep: %02d:%02d\n",
            appDataAlarmSleep.time.tm_hour,
            appDataAlarmSleep.time.tm_min );



    printf( "\tLoggers\n" );
    printf( "\t\tSeparator: %s\n", appDataLog.separator );
    if ( true == appDataLog.log_birds )
    {
        printf( "\t\tBirds: enable - %s\n", appDataLog.filename );
    }
    else
    {
        printf( "\t\tBirds: disable\n" );
    }
    if ( true == appDataLog.log_battery )
    {
        printf( "\t\tBattery: enable - %s\n", BATTERY_LOG_FILE );
    }
    else
    {
        printf( "\t\tBattery: disable\n" );
    }
    if ( true == appDataLog.log_rfid )
    {
        printf( "\t\tRfid: enable - %s\n", RFID_LOG_FILE );
    }
    else
    {
        printf( "\t\tRfid: disable\n" );
    }
    if ( true == appDataLog.log_temp )
    {
        printf( "\t\tTemperature: enable - %s\n", EXT_TEMP_LOG_FILE );
    }
    else
    {
        printf( "\t\tTemperature: disable\n" );
    }
    if ( true == appDataLog.log_calibration )
    {
        printf( "\t\tCalibration: enable - %s\n", CALIBRATION_LOG_FILE );
    }
    else
    {
        printf( "\t\tTemperature: disable\n" );
    }
    if ( true == appDataLog.log_udid )
    {
        printf( "\t\tUdid: enable - %s\n", UDID_LOG_FILE );
    }
    else
    {
        printf( "\t\tUdid: disable\n" );
    }
    if ( true == appDataLog.log_errors )
    {
        printf( "\t\tErrors: enable - %s\n", ERRORS_LOG_FILE );
    }
    else
    {
        printf( "\t\tErrors: disable\n" );
    }
    if ( true == appDataLog.log_events )
    {
        if ( EVENT_FILE_BINARY == appDataEvent.file_type )
        {
            printf( "\t\tEvents: enable - %s\n", appDataEvent.binfilename );
        }
        else if ( EVENT_FILE_TEXT == appDataEvent.file_type )
        {
            printf( "\t\tEvents: enable - %s\n", appDataEvent.filename );
        }
        else
        {
            printf( "\t\tEvents: enable - %s - %s\n", appDataEvent.filename, appDataEvent.binfilename );
        }
    }
    else
    {
        printf( "\t\tEvents: disable\n" );
    }

    printf( "\tChecks\n" );
    if ( true == appData.chk_food_level )
    {
        printf( "\t\tFood level: enable\n" );
    }
    else
    {
        printf( "\t\tFood level: disable\n" );
    }

    if ( true == appData.flags.bit_value.attractive_leds_status )
    {

        printf( "\tAttractive LEDs\n" );

        printf( "\t\tLEDs order: %u %u %u %u\n",
                appDataAttractiveLeds.leds_order[0], appDataAttractiveLeds.leds_order[1],
                appDataAttractiveLeds.leds_order[2], appDataAttractiveLeds.leds_order[3] );

        if ( COLOR_ASSOCIATIVE_LEARNING == appData.scenario_number )
        {

            printf( "\t\tColor A: RGB(%d, %d, %d)\n",
                    appDataAttractiveLeds.red[0],
                    appDataAttractiveLeds.green[0],
                    appDataAttractiveLeds.blue[0] );
            printf( "\t\tColor B: RGB(%d, %d, %d)\n",
                    appDataAttractiveLeds.red[1],
                    appDataAttractiveLeds.green[1],
                    appDataAttractiveLeds.blue[1] );
            printf( "\t\tAlternate delay: %us\n", appDataAttractiveLeds.alt_delay );
        }
        else if ( DOOR_HABITUATION == appData.scenario_number || RISK_AVERSION == appData.scenario_number )
        {
            printf( "\t\tColor: RGB(%d, %d, %d)\n",
                    appDataAttractiveLeds.red[0],
                    appDataAttractiveLeds.green[0],
                    appDataAttractiveLeds.blue[0] );
        }
        else if ( GO_NO_GO == appData.scenario_number )
        {
            printf( "\t\tColor: RGB(%d, %d, %d)\n",
                    appDataAttractiveLeds.red[0],
                    appDataAttractiveLeds.green[0],
                    appDataAttractiveLeds.blue[0] );
            printf( "\t\tAlternate delay: %us\n", appDataAttractiveLeds.alt_delay );
        }

        printf( "\t\tOn time: %02d:%02d\n",
                appDataAttractiveLeds.wake_up_time.tm_hour,
                appDataAttractiveLeds.wake_up_time.tm_min );
        printf( "\t\tOff time: %02d:%02d\n",
                appDataAttractiveLeds.sleep_time.tm_hour,
                appDataAttractiveLeds.sleep_time.tm_min );
    }

    printf( "\tDoor\n" );
    printf( "\t\tOpen delay: %ds\n\t\tClose delay: %ds\n",
            appDataDoor.open_delay / 1000,
            appDataDoor.close_delay / 1000 );
    if ( 0 == appDataDoor.remain_open )
    {
        printf( "\t\tRemain open: no\n" );
    }
    else
    {
        printf( "\t\tRemain open: yes\n" );
    }
    printf( "\t\tOpen time: %02d:%02d\n",
            appDataDoor.open_time.tm_hour,
            appDataDoor.open_time.tm_min );
    printf( "\t\tClose time: %02d:%02d\n",
            appDataDoor.close_time.tm_hour,
            appDataDoor.close_time.tm_min );

    if ( DOOR_HABITUATION == appData.scenario_number )
    {
        printf( "\t\tDoor habituation: %d%%\n", appDataDoor.habituation_percent );
    }

    printf( "\tServo\n\t\tPosition full closed: %d\n", appDataServo.ton_min_night );
    printf( "\t\tPosition full opened: %d\n", appDataServo.ton_max );

    if ( DOOR_HABITUATION == appData.scenario_number )
    {
        printf( "\t\tPosition habituation closed: %d\n", appDataServo.ton_min );
    }
    printf( "\t\tFull closing time: %.3fs\n", ( ( float ) ( appDataServo.ton_max - appDataServo.ton_min_night ) ) / ( ( float ) appDataServo.closing_speed )*0.02 );
    printf( "\t\tFull opening time: %.3fs\n", ( ( float ) ( appDataServo.ton_max - appDataServo.ton_min_night ) ) / ( ( float ) appDataServo.opening_speed )*0.02 );
    printf( "\t\tClosing speed factor: %d\n", appDataServo.closing_speed );
    printf( "\t\tOpening speed factor: %d\n", appDataServo.opening_speed );

    printf( "\tReward\n" );
    if ( 0 == appData.reward_enable )
    {
        printf( "\t\tEnable: no\n" );
    }
    else
    {
        printf( "\t\tEnable: yes\n" );
    }
    printf( "\t\tTimeout: %us\n", appData.timeout_taking_reward / 1000 );

    if ( appData.scenario_number > DOOR_HABITUATION && PATCH_PROBABILITY != appData.scenario_number )
    {
        printf( "\t\tProbability: %u%%\n", appDataDoor.reward_probability );
    }

    printf( "\tTimeouts\n" );
    //    printf( "\t\tSleep: %us\n", appData.timeout_standby / 1000 );
    //    printf( "\t\tPIR: %us\n", appData.timeout_pir / 1000 );    
    printf( "\t\tGuillotine: %.3fs\n", ( float ) appData.timeout_guillotine * 0.001 );

    if ( appData.scenario_number == PATCH_PROBABILITY )
    {
        printf( "\t\tUnique visit: %ds\n", appDataPitTag.timeout_unique_visit );
    }

    if ( true == appData.flags.bit_value.attractive_leds_status )
    {
        printf( "\tPunishment\n" );
        printf( "\t\tDelay: %us\n", appData.punishment_delay / 1000 );

        if ( PATCH_PROBABILITY == appData.scenario_number )
        {
            printf( "\t\tProbability threshold: %u%%\n", appData.punishment_proba_thresh );
        }
    }

    if ( appData.scenario_number > DOOR_HABITUATION )
    {

        if ( GO_NO_GO == appData.scenario_number && ONE_LED == appDataAttractiveLeds.pattern_number )
        {

            //            for (i=0;i<appDataPitTag.numPitTagStored;i++)
            //            {
            //              printf( "\t\tSN%d: %s\n", i + 1, appDataPitTag.pit_tags_list[i] );  
            //            }

            for ( i = 0; i < 4; i++ )
            {
                printf( "\tPIT tags associated with LED %d\n", i + 1 );
                if ( 0 == i )
                {
                    for ( j = 0; j < appDataAttractiveLeds.pattern_one_led_groups[0]; j++ )
                    {
                        printf( "\t\t%03d: %s\n", j + 1, appDataPitTag.pit_tags_list[j] );
                    }
                }
                else
                {
                    for ( j = appDataAttractiveLeds.pattern_one_led_groups[i - 1]; j < appDataAttractiveLeds.pattern_one_led_groups[i]; j++ )
                    {
                        printf( "\t\t%03d: %s\n", j + 1, appDataPitTag.pit_tags_list[j] );
                    }
                }
            }

        }
        else if ( PATCH_PROBABILITY == appData.scenario_number )
        {

            printf( "\tPIT tags denied\n" );
            if ( appDataPitTag.num_pit_tag_denied_or_color_A > 0 )
            {
                for ( i = 0; i < appDataPitTag.num_pit_tag_denied_or_color_A; ++i )
                {
                    printf( "\t\t%03d: %s\n", i + 1, appDataPitTag.pit_tags_list[i] );
                }
            }

            printf( "\tPIT tags accepted and associated reward probability\n" );
            if ( appDataPitTag.num_pit_tag_accepted_or_color_B > 0 )
            {
                for ( i = appDataPitTag.num_pit_tag_denied_or_color_A; i < ( appDataPitTag.num_pit_tag_denied_or_color_A + appDataPitTag.num_pit_tag_accepted_or_color_B ); ++i )
                {
                    printf( "\t\t%03d: %s - %u%%\n", i + 1 - appDataPitTag.num_pit_tag_denied_or_color_A, appDataPitTag.pit_tags_list[i], appDataPitTag.reward_probability[i] );
                }
            }

        }
        else
        {
            if ( COLOR_ASSOCIATIVE_LEARNING == appData.scenario_number )
            {
                printf( "\tPIT tags associated with color A\n" );
            }
            else if ( GO_NO_GO == appData.scenario_number )
            {
                if ( LEFT_RIGHT_LEDS == appDataAttractiveLeds.pattern_number )
                {
                    printf( "\tPIT tags associated with left attractive LEDs\n" );
                }
                else if ( TOP_BOTTOM_LEDS == appDataAttractiveLeds.pattern_number )
                {
                    printf( "\tPIT tags associated with top attractive LEDs\n" );
                }
                else if ( appDataAttractiveLeds.pattern_number > ALL_LEDS )
                {
                    printf( "\tPIT tags associated with pattern 1/2\n" );
                }
            }
            else
            {
                printf( "\tPIT tags denied\n" );
            }

            if ( ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_denied_or_color_A && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_denied_or_color_A )
            {
                for ( i = 0; i < appDataPitTag.num_pit_tag_denied_or_color_A; ++i )
                {
                    printf( "\t\t%03d: %s\n", i + 1, appDataPitTag.pit_tags_list[i] );
                }
            }
            else if ( ALL_PIT_TAG_DENIED == appDataPitTag.num_pit_tag_denied_or_color_A )
            {
                printf( "\t\tnone (%d)\n", ALL_PIT_TAG_DENIED );
            }
            else if ( ALL_PIT_TAG_ACCEPTED == appDataPitTag.num_pit_tag_denied_or_color_A )
            {
                printf( "\t\tall (%d)\n", ALL_PIT_TAG_ACCEPTED );
            }

            if ( COLOR_ASSOCIATIVE_LEARNING == appData.scenario_number )
            {
                printf( "\tPIT tags associated with color B\n" );
            }
            else if ( GO_NO_GO == appData.scenario_number )
            {
                if ( LEFT_RIGHT_LEDS == appDataAttractiveLeds.pattern_number )
                {
                    printf( "\tPIT tags associated with right attractive LEDs\n" );
                }
                else if ( TOP_BOTTOM_LEDS == appDataAttractiveLeds.pattern_number )
                {
                    printf( "\tPIT tags associated with bottom attractive LEDs\n" );
                }
                else if ( appDataAttractiveLeds.pattern_number > ALL_LEDS )
                {
                    printf( "\tPIT tags associated with pattern 2/2\n" );
                }
            }
            else
            {
                printf( "\tPIT tags accepted\n" );
            }

            if ( ALL_PIT_TAG_DENIED != appDataPitTag.num_pit_tag_accepted_or_color_B && ALL_PIT_TAG_ACCEPTED != appDataPitTag.num_pit_tag_accepted_or_color_B )
            {
                for ( i = appDataPitTag.num_pit_tag_denied_or_color_A; i < ( appDataPitTag.num_pit_tag_denied_or_color_A + appDataPitTag.num_pit_tag_accepted_or_color_B ); ++i )
                {
                    printf( "\t\t%03d: %s\n", i + 1 - appDataPitTag.num_pit_tag_denied_or_color_A, appDataPitTag.pit_tags_list[i] );
                }
            }
            else if ( ALL_PIT_TAG_DENIED == appDataPitTag.num_pit_tag_accepted_or_color_B )
            {
                printf( "\t\tnone (%d)\n", ALL_PIT_TAG_DENIED );
            }
            else if ( ALL_PIT_TAG_ACCEPTED == appDataPitTag.num_pit_tag_accepted_or_color_B )
            {
                printf( "\t\tall (%d)\n", ALL_PIT_TAG_ACCEPTED );
            }
        }

    }

    printf( "\n" );
}


void getIniPbChar( INI_READ_STATE state, char *buf, uint8_t n )
{

    switch ( state )
    {

        case INI_PB_SCENARIO_NUM:
            snprintf( buf, n, "Scenario: number" );
            break;
        case INI_PB_SITEID_ZONE:
            snprintf( buf, n, "Site ID: zone" );
            break;
        case INI_PB_TIME_WAKEUP_HOUR:
            snprintf( buf, n, "Wake-up: hour" );
            break;
        case INI_PB_TIME_WAKEUP_MINUTE:
            snprintf( buf, n, "Wake-up: minute" );
            break;
        case INI_PB_TIME_SLEEP_HOUR:
            snprintf( buf, n, "Sleep: hour" );
            break;
        case INI_PB_TIME_SLEEP_MINUTE:
            snprintf( buf, n, "Sleep: minute" );
            break;
        case INI_PB_LOGS_BIRDS:
            snprintf( buf, n, "Logs: birds" );
            break;
        case INI_PB_LOGS_UDID:
            snprintf( buf, n, "Logs: udid" );
            break;
        case INI_PB_LOGS_EVENTS:
            snprintf( buf, n, "Logs: events" );
            break;
        case INI_PB_LOGS_ERRORS:
            snprintf( buf, n, "Logs: errors" );
            break;
        case INI_PB_LOGS_BATTERY:
            snprintf( buf, n, "Logs: battery" );
            break;
        case INI_PB_LOGS_RFID:
            snprintf( buf, n, "Logs: rfid" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_RED_A:
            snprintf( buf, n, "Attractive LEDs: red A" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_GREEN_A:
            snprintf( buf, n, "Attractive LEDs: green A" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_BLUE_A:
            snprintf( buf, n, "Attractive LEDs: blue A" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_RED_B:
            snprintf( buf, n, "Attractive LEDs: red B" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_GREEN_B:
            snprintf( buf, n, "Attractive LEDs: green B" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_BLUE_B:
            snprintf( buf, n, "Attractive LEDs: blue B" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_ALT_DELAY:
            snprintf( buf, n, "Attractive LEDs: alternate delay" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_ON_HOUR:
            snprintf( buf, n, "Attractive LEDs: off minute" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_ON_MINUTE:
            snprintf( buf, n, "Attractive LEDs: on minute" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_OFF_HOUR:
            snprintf( buf, n, "Attractive LEDs: off hour" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_OFF_MINUTE:
            snprintf( buf, n, "Attractive LEDs: off minute" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_PATTERN:
            snprintf( buf, n, "Attractive LEDs: pattern" );
            break;
        case INI_PB_ATTRACTIVE_LEDS_PATTERN_PERCENT:
            snprintf( buf, n, "Attractive LEDs: pattern percent" );
            break;
        case INI_PB_PIT_TAG_LEFT:
            snprintf( buf, n, "PIT tags: left LEDs" );
            break;
        case INI_PB_PIT_TAG_RIGHT:
            snprintf( buf, n, "PIT tags: right LEDs" );
            break;
        case INI_PB_PIT_TAG_TOP:
            snprintf( buf, n, "PIT tags: top LEDs" );
            break;
        case INI_PB_PIT_TAG_BOTTOM:
            snprintf( buf, n, "PIT tags: bottom LEDs" );
            break;
        case INI_PB_PIT_TAG_LED_1:
            snprintf( buf, n, "PIT tags: LED 1" );
            break;
        case INI_PB_PIT_TAG_LED_2:
            snprintf( buf, n, "PIT tags: LED 2" );
            break;
        case INI_PB_PIT_TAG_LED_3:
            snprintf( buf, n, "PIT tags: LED 3" );
            break;
        case INI_PB_PIT_TAG_LED_4:
            snprintf( buf, n, "PIT tags: LED 4" );
            break;
        case INI_PB_PIT_TAG_COLOR_A:
            snprintf( buf, n, "PIT tags: color A" );
            break;
        case INI_PB_PIT_TAG_COLOR_B:
            snprintf( buf, n, "PIT tags: color B" );
            break;
        case INI_PB_PIT_TAG_DENIED:
            snprintf( buf, n, "PIT tags: denied" );
            break;
        case INI_PB_PIT_TAG_ACCEPTED:
            snprintf( buf, n, "PIT tags: accepted" );
            break;
        case INI_PB_DOOR_CLOSE_POSITION:
            snprintf( buf, n, "Door: close position" );
            break;
        case INI_PB_DOOR_OPEN_POSITION:
            snprintf( buf, n, "Door: open position" );
            break;
        case INI_PB_DOOR_CLOSING_SPEED:
            snprintf( buf, n, "Door: closing speed" );
            break;
        case INI_PB_DOOR_OPENING_SPEED:
            snprintf( buf, n, "Door: opening speed" );
            break;
        case INI_PB_DOOR_OPEN_DELAY:
            snprintf( buf, n, "Door: open delay" );
            break;
        case INI_PB_DOOR_CLOSE_DELAY:
            snprintf( buf, n, "Door: close delay" );
            break;
        case INI_PB_DOOR_REMAIN_OPEN:
            snprintf( buf, n, "Door: remain open" );
            break;
        case INI_PB_DOOR_OPEN_HOUR:
            snprintf( buf, n, "Door: open hour" );
            break;
        case INI_PB_DOOR_OPEN_MINUTE:
            snprintf( buf, n, "Door: open minute" );
            break;
        case INI_PB_DOOR_CLOSE_HOUR:
            snprintf( buf, n, "Door: close hour" );
            break;
        case INI_PB_DOOR_CLOSE_MINUTE:
            snprintf( buf, n, "Door: close minute" );
            break;
        case INI_PB_DOOR_HABITUATION:
            snprintf( buf, n, "Door: habituation" );
            break;
        case INI_PB_REWARD_ENABLE:
            snprintf( buf, n, "Reward: enable" );
            break;
        case INI_PB_REWARD_TIMEOUT:
            snprintf( buf, n, "Timeouts: reward" );
            break;
        case INI_PB_REWARD_PROBABILITY:
            snprintf( buf, n, "Timeouts: probability" );
            break;
        case INI_PB_TIMEOUTS_SLEEP:
            snprintf( buf, n, "Timeouts: sleep" );
            break;
        case INI_PB_TIMEOUTS_PIR:
            snprintf( buf, n, "Timeouts: pir" );
            break;
        case INI_PB_TIMEOUTS_UNIQUE_VISIT:
            snprintf( buf, n, "Timeouts: unique visit" );
            break;
        case INI_PB_PUNISHMENT_DELAY:
            snprintf( buf, n, "Punishment: delay" );
            break;
        case INI_PB_PUNISHMENT_PROBA_THRESH:
            snprintf( buf, n, "Punishment: probability threshold" );
            break;
        case INI_PB_CHECK_FOOD_LEVEL:
            snprintf( buf, n, "Check: food level" );
            break;
        case INI_PB_SECURITY_BIRD_REWARD_REOPEN:
            snprintf( buf, n, "Security: reopen x10" );
            break;

        default:
            snprintf( buf, n, "Error not listed" );
            break;
    }


}

/*******************************************************************************
 End of File
 */
