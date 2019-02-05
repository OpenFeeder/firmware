/*
 * File:   app_rtc_action.c
 * Author: jerome
 *
 * Created on 17 novembre 2018, 07:16
 */


#include "app.h"

void manageRtcAction(  )
{
    bool flag;
    int i;
    
    /* If the OF is awaken => go to sleep mode */
    if ( OPENFEEDER_IS_AWAKEN == appData.openfeeder_state && RTCC_ALARM_SLEEP_OPENFEEDER == appData.rtcc_alarm_action )
    {
        appData.state = APP_STATE_SLEEP;
        return;
    }

    /* If the OF is sleeping => wake up */
    if ( OPENFEEDER_IS_SLEEPING == appData.openfeeder_state && RTCC_ALARM_WAKEUP_OPENFEEDER == appData.rtcc_alarm_action )
    {
        appData.state = APP_STATE_WAKE_UP;
        return;
    }

    /* Calibrate date and time using the external RTC module */
    if ( RTCC_RTC_CALIBRATION == appData.rtcc_alarm_action )
    {
        calibrateDateTime( );
    }

    /* Get temperature from the external RTC module */
    if ( RTCC_DS3231_TEMPERATURE == appData.rtcc_alarm_action )
    {
        if (0 < APP_I2CMasterSeeksSlaveDevice(DS3231_I2C_ADDR, DS3231_I2C_ADDR))
        {
            /* Log event if required */
            if ( true == appDataLog.log_events )
            {
               store_event(OF_DS3231_GET_TEMPERATURE); 
            }

            getDateTime( );
            getDS3231Temperature( );
            
            if ( appDataLog.num_ds3231_temp_stored < NUM_DS3231_TEMP_TO_LOG )
            {
                appDataLog.ds3231_temp[appDataLog.num_ds3231_temp_stored][0] = (float)appData.current_time.tm_hour;
                appDataLog.ds3231_temp[appDataLog.num_ds3231_temp_stored][1] = (float)appData.current_time.tm_min;
                appDataLog.ds3231_temp[appDataLog.num_ds3231_temp_stored][2] = appData.ext_temperature;
                ++appDataLog.num_ds3231_temp_stored;
            }
            else
            {
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_DS3231_TEMP_OVERFLOW );
                }
            }
        }
    }

    /* Turn off the attractive LEDs */
    if ( RTCC_ALARM_SET_ATTRACTIVE_LEDS_OFF == appData.rtcc_alarm_action )
    {
        if ( ATTRACTIVE_LEDS_ON == appDataAttractiveLeds.status )
        {
            setAttractiveLedsOff( );
        }
    }

    /* Turn on the attractive LEDs */
    if ( RTCC_ALARM_SET_ATTRACTIVE_LEDS_ON == appData.rtcc_alarm_action )
    {
        if ( ATTRACTIVE_LEDS_OFF == appDataAttractiveLeds.status )
        {
            setAttractiveLedsOn( );
        }
    }

    /* Alternate attractive LEDs color (scenario 6 - Color Associative Learning) */
    if ( RTCC_ALARM_ALT_ATTRACTIVE_LEDS_COLOR == appData.rtcc_alarm_action && false == appData.punishment_state )
    {
        if ( rand( ) > (RAND_MAX/2) ) // t / RAND_MAX ) > 0.5
        {
            appDataAttractiveLeds.current_color_index = !appDataAttractiveLeds.current_color_index;
            setAttractiveLedsColor( );

            /* Log event if required */
            if ( true == appDataLog.log_events )
            {
                if ( 0 == appDataAttractiveLeds.current_color_index )
                {
                    store_event(OF_CAL_A);
                }
                else
                {
                    store_event(OF_CAL_B);
                }
            }  
        }                                
    }
    
    /* Alternate attractive LEDs pattern (scenario 3 - Go-No go) */
    if ( RTCC_ALARM_ALT_ATTRACTIVE_LEDS_PATTERN == appData.rtcc_alarm_action && false == appData.punishment_state )
    {
        int randomInteger = rand( );

        /* If scenario 33 - One LED */
        if (ONE_LED == appDataAttractiveLeds.pattern_number)
        {
            for (i=0;i<4;i++)
            {
               appDataAttractiveLeds.pattern[i] = 1; 
            }

            if ( randomInteger > (RAND_MAX/4*3) )
            {
                appDataAttractiveLeds.pattern_one_led_current = 0;

                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_GO_NO_GO_ONE_1 );
                }
            }                            
            else if ( randomInteger > (RAND_MAX/2) )
            {
                appDataAttractiveLeds.pattern_one_led_current = 1;

                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_GO_NO_GO_ONE_2 );
                }
            }
            else if ( randomInteger > (RAND_MAX/4) )
            {
                appDataAttractiveLeds.pattern_one_led_current = 2; 

                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_GO_NO_GO_ONE_3 );
                }
            }
            else
            {
                appDataAttractiveLeds.pattern_one_led_current = 3;

                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_GO_NO_GO_ONE_4 );
                }
            }

            appDataAttractiveLeds.pattern[appDataAttractiveLeds.pattern_one_led_current] = 0;      

           setAttractiveLedsPattern( );
        }
        /* If scenario 30 - All LEDs */
        else if (ALL_LEDS == appDataAttractiveLeds.pattern_number)
        {
            if ( randomInteger > (RAND_MAX/100*appDataAttractiveLeds.pattern_percent) )
            {
                for (i=0;i<4;i++)
                {
                   appDataAttractiveLeds.pattern[i] = !appDataAttractiveLeds.pattern[i]; 
                }
                appDataAttractiveLeds.pattern_idx = !appDataAttractiveLeds.pattern_idx;
                setAttractiveLedsPattern( );

                appDataAttractiveLeds.pattern_percent = 100-appDataAttractiveLeds.pattern_percent;

                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    if ( 0 == appDataAttractiveLeds.pattern_idx )
                    {
                        store_event( OF_GO_NO_GO_ALL_ON );
                    }
                    else
                    {
                        store_event( OF_GO_NO_GO_ALL_OFF );
                    }
                }                                
            }
        }
         /* If scenario 31 - Left/Right or scenario 32 - Top/Bottom */
        else
        {
            if ( randomInteger > (RAND_MAX/2) )
            {
                for (i=0;i<4;i++)
                {
                   appDataAttractiveLeds.pattern[i] = !appDataAttractiveLeds.pattern[i]; 
                }
                appDataAttractiveLeds.pattern_idx = !appDataAttractiveLeds.pattern_idx;
                setAttractiveLedsPattern( );   

                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    if ( 0 == appDataAttractiveLeds.pattern_idx )
                    {
                        if ( LEFT_RIGHT_LEDS == appDataAttractiveLeds.pattern_number )
                        {
                            store_event( OF_GO_NO_GO_LR_L );
                        }
                        else
                        {
                            store_event( OF_GO_NO_GO_TB_T );
                        }
                    }
                    else
                    {
                        if ( LEFT_RIGHT_LEDS == appDataAttractiveLeds.pattern_number )
                        {
                            store_event( OF_GO_NO_GO_LR_R );
                        }
                        else
                        {
                            store_event( OF_GO_NO_GO_TB_B );
                        }
                    }
                }  
            }
        }

    }
    
    if ( DOOR_OPENED != appDataDoor.reward_door_status && RTCC_ALARM_OPEN_DOOR == appData.rtcc_alarm_action )
    {
        /* Open reward door */
        servomotorPowerEnable( );

        appDataServo.ton_cmd = servomotorGetDoorPosition( );
        appDataServo.ton_goal = appDataServo.ton_max;

        if ( appDataServo.ton_cmd != appDataServo.ton_goal )
        {
            appDataDoor.reward_door_status = DOOR_MOVING;
            while ( DOOR_MOVED != appDataDoor.reward_door_status );
        }
        appDataDoor.reward_door_status = DOOR_OPENED;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_SERVO_POSITION)
        servomotorGetDoorPosition( );
#endif  
        servomotorPowerDisable( );
    }

    if ( DOOR_CLOSED != appDataDoor.reward_door_status && RTCC_ALARM_CLOSE_DOOR == appData.rtcc_alarm_action )
    {
        /* Close reward door */
        servomotorPowerEnable( );

        appDataServo.ton_cmd = servomotorGetDoorPosition( );
        appDataServo.ton_goal = appDataServo.ton_min;

        if ( appDataServo.ton_cmd != appDataServo.ton_goal )
        {
            appDataDoor.reward_door_status = DOOR_MOVING;
            while ( DOOR_MOVED != appDataDoor.reward_door_status );
        }
        appDataDoor.reward_door_status = DOOR_CLOSED;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_SERVO_POSITION)
        servomotorGetDoorPosition( );
#endif  
        servomotorPowerDisable( );
    }

    if ( RTCC_BATTERY_LEVEL_CHECK == appData.rtcc_alarm_action )
    {
        flag = isPowerBatteryGood( );
        
        if ( appDataLog.num_battery_level_stored < NUM_BATTERY_LEVEL_TO_LOG )
        {
            appDataLog.battery_level[appDataLog.num_battery_level_stored][0] = appData.current_time.tm_hour;
            appDataLog.battery_level[appDataLog.num_battery_level_stored][1] = appData.battery_level;
            ++appDataLog.num_battery_level_stored;
        }
        else
        {
            /* Log event if required */
            if ( true == appDataLog.log_events )
            {
                store_event( OF_BATTERY_LEVEL_OVERFLOW );
            }
        }

        if ( false == flag )
        {
            appData.state = APP_STATE_FLUSH_DATA_BEFORE_ERROR;
            return;
        }
    }
    
    if ( RTCC_FOOD_LEVEL_CHECK == appData.rtcc_alarm_action && true == appData.chk_food_level )
    {
        flag = isEnoughFood( );
        if ( false == flag )
        {
            appData.state = APP_STATE_FLUSH_DATA_BEFORE_ERROR;
            return;
        }
    }
    
    if ( RTCC_RFID_FREQ_CHECK == appData.rtcc_alarm_action )
    {
        flag = isRfidFreqGood( );
        
        if ( appDataLog.num_rfid_freq_stored < NUM_RFID_FREQ_TO_LOG )
        {
            appDataLog.rfid_freq[appDataLog.num_rfid_freq_stored][0] = appData.current_time.tm_hour;
            appDataLog.rfid_freq[appDataLog.num_rfid_freq_stored][1] = appData.current_time.tm_min;
            appDataLog.rfid_freq[appDataLog.num_rfid_freq_stored][2] = appData.rfid_rdyclk;
            ++appDataLog.num_rfid_freq_stored;
        }
        else
        {
            /* Log event if required */
            if ( true == appDataLog.log_events )
            {
                store_event( OF_RFID_FREQ_OVERFLOW );
            }
        }

        if ( false == flag )
        {
            appData.state = APP_STATE_FLUSH_DATA_BEFORE_ERROR;
            return;
        }
    }
    
    /* Alternate video */
    if ( RTCC_ALARM_STIMULI_ALT == appData.rtcc_alarm_action )
    {
//        appDataStimuli.current_stimuli = (int)(rand() / (double)RAND_MAX * (appDataStimuli.max_stimuli - 1));
        appDataStimuli.current_stimuli = rand() % ( appDataStimuli.max_stimuli );

        UART2_Write( appDataStimuli.current_stimuli );
    }

    appData.rtcc_alarm_action = RTCC_ALARM_IDLE;
}


