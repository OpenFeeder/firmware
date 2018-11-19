/*
 * File:   app_scenario.c
 * Author: jerome
 *
 * Created on 16 novembre 2018, 18:07
 */


#include "app.h"


void initializeLedsScenario( )
{
    /* If scenario 3 or 6 => get current time */
    if ( GO_NO_GO == appData.scenario_number || COLOR_ASSOCIATIVE_LEARNING == appData.scenario_number )
    {
        getDateTime( );
    }

    if ( GO_NO_GO == appData.scenario_number )
    {
        /* If scenario 30 - All LEDs */
        if ( ALL_LEDS == appDataAttractiveLeds.pattern_number )
        {
            appDataAttractiveLeds.pattern[0] = 0;
            appDataAttractiveLeds.pattern[1] = 0;
            appDataAttractiveLeds.pattern[2] = 0;
            appDataAttractiveLeds.pattern[3] = 0;

            /* Log event if required */
            if ( true == appDataLog.log_events )
            {
                store_event( OF_GO_NO_GO_ALL_ON );
            }
        }
            /* If scenario 31 - Left/Right */
        else if ( LEFT_RIGHT_LEDS == appDataAttractiveLeds.pattern_number )
        {
            appDataAttractiveLeds.pattern[0] = 1;
            appDataAttractiveLeds.pattern[1] = 0;
            appDataAttractiveLeds.pattern[2] = 0;
            appDataAttractiveLeds.pattern[3] = 1;

            /* Log event if required */
            if ( true == appDataLog.log_events )
            {
                store_event( OF_GO_NO_GO_LR_L );
            }
        }
            /* If scenario 32 - Top/Bottom */
        else if ( TOP_BOTTOM_LEDS == appDataAttractiveLeds.pattern_number )
        {
            appDataAttractiveLeds.pattern[0] = 1;
            appDataAttractiveLeds.pattern[1] = 1;
            appDataAttractiveLeds.pattern[2] = 0;
            appDataAttractiveLeds.pattern[3] = 0;

            /* Log event if required */
            if ( true == appDataLog.log_events )
            {
                store_event( OF_GO_NO_GO_TB_T );
            }
        }
            /* If scenario 33 - One LED */
        else
        {

            appDataAttractiveLeds.pattern[0] = 1;
            appDataAttractiveLeds.pattern[1] = 1;
            appDataAttractiveLeds.pattern[2] = 1;
            appDataAttractiveLeds.pattern[3] = 1;

            /* Log event if required */
            if ( true == appDataLog.log_events )
            {
                store_event( OF_GO_NO_GO_ONE_NONE );
            }

        }

        appDataAttractiveLeds.pattern_idx = 0;
    }
    else
    {
        appDataAttractiveLeds.current_color_index = ATTRACTIVE_LEDS_COLOR_A;

        /* Log event if required */
        if ( true == appDataLog.log_events )
        {
            store_event( OF_CAL_A );
        }
    }
}


void manageScenario( )
{
    switch ( appData.scenario_number )
    {
            /* Scenario 0 */
        case NO_SCENARIO:
            /* Scenario 1 */
        case OPEN_BAR:
            /* All PIT tags accepted */
            appDataLog.is_pit_tag_denied = false;
            /* Assume PIT tag found even if not listed */
            appDataPitTag.pit_tag_found_in_list = true;
            break;

            /* Scenario 2 */
        case DOOR_HABITUATION:
            /* All PIT tags accepted */
            appDataLog.is_pit_tag_denied = false;
            /* Assume PIT tag found even if not listed */
            appDataPitTag.pit_tag_found_in_list = true;
            break;

            /* Scenario 3 */
        case GO_NO_GO:

            /* If punishment is runnning => denied */
            if ( true == appData.punishment_state )
            {
                /* All PIT tags denied */
                appDataLog.is_pit_tag_denied = true;
                /* Assume PIT tag found even if not listed */
                appDataPitTag.pit_tag_found_in_list = true;
                break;
            }

            /* TODO remove next line when finish */
            appDataLog.is_pit_tag_denied = false;

            /* Scenario 30 - All LEDs */
            if ( ALL_LEDS == appDataAttractiveLeds.pattern_number )
            {
                /* Assume PIT tag found even if not listed */
                appDataPitTag.pit_tag_found_in_list = true;

                /* If all LEDs on => accepted */
                if ( ATTRACTIVE_LEDS_PATTERN_ON == appDataAttractiveLeds.pattern_idx )
                {
                    appDataLog.is_pit_tag_denied = false;
                }
                    /* If all LEDs off => denied */
                else
                {
                    appDataLog.is_pit_tag_denied = true;
                }
            }
                /* Scenario 31&32 - Top/Bottom and Left/Right LEDs */
            else if ( LEFT_RIGHT_LEDS == appDataAttractiveLeds.pattern_number || TOP_BOTTOM_LEDS == appDataAttractiveLeds.pattern_number )
            {

                /* If all PIT tags accepted for pattern A AND pattern A => accepted */
                if ( ALL_PIT_TAG_ACCEPTED == appDataPitTag.num_pit_tag_denied_or_color_A && ATTRACTIVE_LEDS_PATTERN_A == appDataAttractiveLeds.pattern_idx )
                {
                    appDataLog.is_pit_tag_denied = false;
                    /* Assume PIT tag found even if not listed */
                    appDataPitTag.pit_tag_found_in_list = true;
                }
                    /* If all PIT tags denied for pattern A AND pattern A => denied */
                else if ( ALL_PIT_TAG_DENIED == appDataPitTag.num_pit_tag_denied_or_color_A && ATTRACTIVE_LEDS_PATTERN_A == appDataAttractiveLeds.pattern_idx )
                {
                    appDataLog.is_pit_tag_denied = true;
                    /* Assume PIT tag found even if not listed */
                    appDataPitTag.pit_tag_found_in_list = true;
                }
                    /* If all PIT tags accepted for pattern B AND pattern B => accepted */
                else if ( ALL_PIT_TAG_ACCEPTED == appDataPitTag.num_pit_tag_accepted_or_color_B && ATTRACTIVE_LEDS_PATTERN_B == appDataAttractiveLeds.pattern_idx )
                {
                    appDataLog.is_pit_tag_denied = false;
                    /* Assume PIT tag found even if not listed */
                    appDataPitTag.pit_tag_found_in_list = true;
                }
                    /* If all PIT tags denied for pattern B AND pattern B => denied */
                else if ( ALL_PIT_TAG_DENIED == appDataPitTag.num_pit_tag_accepted_or_color_B && ATTRACTIVE_LEDS_PATTERN_B == appDataAttractiveLeds.pattern_idx )
                {
                    appDataLog.is_pit_tag_denied = true;
                    /* Assume PIT tag found even if not listed */
                    appDataPitTag.pit_tag_found_in_list = true;
                }
                    /* Otherwise */
                else
                {
                    /* First, assume PIT tag as accepted */
                    appDataLog.is_pit_tag_denied = false;

                    /* Search PIT tag in list */
                    findPitTagInList( );

                    /* If PIT tag is not listed => denied */
                    if ( false == appDataPitTag.pit_tag_found_in_list )
                    {
                        appDataLog.is_pit_tag_denied = true;
                    }
                        /* Otherwise check if PIT tag is denied */
                    else
                    {
                        /* If PIT tag index in second group (B) AND pattern A => denied */
                        if ( appDataPitTag.pit_tag_index_in_list >= appDataPitTag.num_pit_tag_denied_or_color_A && ATTRACTIVE_LEDS_PATTERN_A == appDataAttractiveLeds.pattern_idx )
                        {
                            appDataLog.is_pit_tag_denied = true;
                        }
                        /* If PIT tag index in first group (A) AND pattern B => denied */
                        if ( appDataPitTag.pit_tag_index_in_list < appDataPitTag.num_pit_tag_denied_or_color_A && ATTRACTIVE_LEDS_PATTERN_B == appDataAttractiveLeds.pattern_idx )
                        {
                            appDataLog.is_pit_tag_denied = true;
                        }
                    }
                }

            }
                /* Scenario 33 - One LEDs */
            else if ( ONE_LED == appDataAttractiveLeds.pattern_number )
            {
                /* If no PIT tag in current group or all PIT tags denied */
                if ( 0 == appDataPitTag.num_pit_tag_per_group[appDataAttractiveLeds.pattern_one_led_current] || ALL_PIT_TAG_DENIED == appDataPitTag.num_pit_tag_per_group[appDataAttractiveLeds.pattern_one_led_current] )
                {
                    appDataLog.is_pit_tag_denied = true;
                    /* Assume PIT tag found even if not listed */
                    appDataPitTag.pit_tag_found_in_list = true;
                }
                    /* If all PIT tags accepted */
                else if ( ALL_PIT_TAG_ACCEPTED == appDataPitTag.num_pit_tag_per_group[appDataAttractiveLeds.pattern_one_led_current] )
                {
                    appDataLog.is_pit_tag_denied = false;
                    /* Assume PIT tag found even if not listed */
                    appDataPitTag.pit_tag_found_in_list = true;
                }
                    /* Otherwise check if PIT tag is denied */
                else
                {

                    appDataLog.is_pit_tag_denied = false;

                    /* Search PIT tag in list */
                    findPitTagInList( );

                    /* If PIT tag is not listed => denied */
                    if ( false == appDataPitTag.pit_tag_found_in_list )
                    {
                        appDataLog.is_pit_tag_denied = true;
                    }
                        /* Otherwise check if PIT tag is denied */
                    else
                    {
                        if ( appDataPitTag.pit_tag_index_in_list < appDataAttractiveLeds.pattern_one_led_groups[0] )
                        {
                            if ( 0 != appDataAttractiveLeds.pattern_one_led_current )
                            {
                                appDataLog.is_pit_tag_denied = true;
                            }
                        }
                        else if ( appDataPitTag.pit_tag_index_in_list < appDataAttractiveLeds.pattern_one_led_groups[1] )
                        {
                            if ( 1 != appDataAttractiveLeds.pattern_one_led_current )
                            {
                                appDataLog.is_pit_tag_denied = true;
                            }
                        }
                        else if ( appDataPitTag.pit_tag_index_in_list < appDataAttractiveLeds.pattern_one_led_groups[2] )
                        {
                            if ( 2 != appDataAttractiveLeds.pattern_one_led_current )
                            {
                                appDataLog.is_pit_tag_denied = true;
                            }
                        }
                        else
                        {
                            if ( 3 != appDataAttractiveLeds.pattern_one_led_current )
                            {
                                appDataLog.is_pit_tag_denied = true;
                            }
                        }
                    }
                }
            }

            break;

            /* Scenario 4 */
        case LONG_TERM_SPATIAL_MEMORY:

            /* First, assume PIT tag as accepted */
            appDataLog.is_pit_tag_denied = false;

            /* Search PIT tag in list*/
            findPitTagInList( );

            /* If PIT tag is not listed => denied */
            if ( false == appDataPitTag.pit_tag_found_in_list )
            {
                appDataLog.is_pit_tag_denied = true;
            }
                /* Otherwise check if PIT tag is denied */
            else
            {
                appDataLog.is_pit_tag_denied = isPitTagDenied( );
            }
            break;

            /* Scenario 5 */
        case WORKING_SPATIAL_MEMORY:

            /* First, assume PIT tag as accepted */
            appDataLog.is_pit_tag_denied = false;

            /* Search PIT tag in list*/
            findPitTagInList( );

            /* If PIT tag is not listed => denied */
            if ( false == appDataPitTag.pit_tag_found_in_list )
            {
                appDataLog.is_pit_tag_denied = true;
            }
                /* Otherwise check if PIT tag is denied */
            else
            {
                appDataLog.is_pit_tag_denied = isPitTagDenied( );

                /* Bird is allowed only one time */
                appDataPitTag.is_pit_tag_denied_or_color_A[appDataPitTag.pit_tag_index_in_list] = true;
            }
            break;

            /* Scenario 6 */
        case COLOR_ASSOCIATIVE_LEARNING:

            /* Assume PIT tag found at first even if not listed */
            appDataPitTag.pit_tag_found_in_list = true;

            /* If punishment is runnning => denied */
            if ( true == appData.punishment_state )
            {
                appDataLog.is_pit_tag_denied = true;
                break;
            }

            /* First, assume PIT tag as accepted */
            appDataLog.is_pit_tag_denied = false;

            /* If all PIT tags accepted for color A AND color A => accepted */
            if ( ALL_PIT_TAG_ACCEPTED == appDataPitTag.num_pit_tag_denied_or_color_A && ATTRACTIVE_LEDS_COLOR_A == appDataLog.attractive_leds_current_color_index )
            {
                appDataLog.is_pit_tag_denied = false;
                /* Assume PIT tag found even if not listed */
                appDataPitTag.pit_tag_found_in_list = true;
            }
                /* If all PIT tags denied for color A AND color A => denied */
            else if ( ALL_PIT_TAG_DENIED == appDataPitTag.num_pit_tag_denied_or_color_A && ATTRACTIVE_LEDS_COLOR_A == appDataLog.attractive_leds_current_color_index )
            {
                appDataLog.is_pit_tag_denied = true;
                /* Assume PIT tag found even if not listed */
                appDataPitTag.pit_tag_found_in_list = true;
            }
                /* If all PIT tags accepted for color B AND color B => accepted */
            else if ( ALL_PIT_TAG_ACCEPTED == appDataPitTag.num_pit_tag_accepted_or_color_B && ATTRACTIVE_LEDS_COLOR_B == appDataLog.attractive_leds_current_color_index )
            {
                appDataLog.is_pit_tag_denied = false;
                /* Assume PIT tag found even if not listed */
                appDataPitTag.pit_tag_found_in_list = true;
            }
                /* If all PIT tags denied for color B AND color B => denied */
            else if ( ALL_PIT_TAG_DENIED == appDataPitTag.num_pit_tag_accepted_or_color_B && ATTRACTIVE_LEDS_COLOR_B == appDataLog.attractive_leds_current_color_index )
            {
                appDataLog.is_pit_tag_denied = true;
                /* Assume PIT tag found even if not listed */
                appDataPitTag.pit_tag_found_in_list = true;
            }
                /* Otherwise */
            else
            {
                /* Search PIT tag in list */
                findPitTagInList( );

                /* If PIT tag is not listed => denied */
                if ( false == appDataPitTag.pit_tag_found_in_list )
                {
                    appDataLog.is_pit_tag_denied = true;
                }
                    /* Otherwise check if PIT tag is denied */
                else
                {
                    /* If PIT tag index in second group (B) AND color A => denied */
                    if ( appDataPitTag.pit_tag_index_in_list >= appDataPitTag.num_pit_tag_denied_or_color_A && ATTRACTIVE_LEDS_COLOR_A == appDataLog.attractive_leds_current_color_index )
                    {
                        appDataLog.is_pit_tag_denied = true;
                    }
                    /* If PIT tag index in first group (A) AND color B => denied */
                    if ( appDataPitTag.pit_tag_index_in_list < appDataPitTag.num_pit_tag_denied_or_color_A && ATTRACTIVE_LEDS_COLOR_B == appDataLog.attractive_leds_current_color_index )
                    {
                        appDataLog.is_pit_tag_denied = true;
                    }
                }
            }

            break;

            /* Scenario 7 */
        case RISK_AVERSION:

            /* First, assume PIT tag as accepted */
            appDataLog.is_pit_tag_denied = false;

            /* Search PIT tag in list*/
            findPitTagInList( );

            /* If PIT tag is not listed => denied */
            if ( false == appDataPitTag.pit_tag_found_in_list )
            {
                appDataLog.is_pit_tag_denied = true;
            }
                /* Otherwise check if PIT tag is denied */
            else
            {
                appDataLog.is_pit_tag_denied = isPitTagDenied( );
            }
            break;

            /* Scenario 8 */
        case PATCH_PROBABILITY:

            /* First, assume PIT tag as accepted */
            appDataLog.is_pit_tag_denied = false;

            /* Search PIT tag in list*/
            findPitTagInList( );

            /* If PIT tag is not listed => denied */
            if ( false == appDataPitTag.pit_tag_found_in_list )
            {
                appDataLog.is_pit_tag_denied = true;
            }
                /* Otherwise check if PIT tag is denied */
            else
            {
                appDataLog.is_pit_tag_denied = isPitTagDenied( );
            }

            if ( false == appDataLog.is_pit_tag_denied )
            {
                appDataDoor.reward_probability = appDataPitTag.reward_probability[appDataPitTag.pit_tag_index_in_list];
            }

            break;

    }
}

