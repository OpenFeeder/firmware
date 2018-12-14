/**
 * @file app_debug.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 06/07/2016
 * @revision history 3
 */

#include <ctype.h>
#include "app.h"
#include "app_debug.h"
#include "app_i2c.h"
#include "app_datetime.h"

#if defined (USE_UART1_SERIAL_INTERFACE)

/* Current date to a C string (page 244)) */
const uint8_t BUILD_DATE[] = { __DATE__ };
const uint8_t BUILD_TIME[] = { __TIME__ };

void displayKeyMapping( void )
{
    /* Interface firmware terminal (Debug) */
    printf( "Key mapping:\n" );
    printf( " a or A: attractive LEDs\n" );
    printf( "   - b or B: set blue component\n" );
    printf( "   - g or G: set green component\n" );
    printf( "   - i or I: initialize LEDs command\n" );
    printf( "   - r or R: set red component\n" );
    printf( "   - t or T: test LEDs\n" );
    printf( " b or B: data buffers\n" );
    printf( " c or C: manage configuration\n" );
    printf( "   - d or D: display configuration parameters\n" );
    printf( "   - r or R: reconfigure the system\n" );
    printf( " d or D: door\n" );
    printf( "   - c or C: close door\n" );
    printf( "   - o or O: open door\n" );
    printf( "   - p or P: set door position\n" );
    printf( "   - r or R: toggle remain open parameter\n" );
    printf( " e or E: check status LEDs\n" );
    printf( " f or F: flush data on USB device\n" );
//                printf( " g or G: NOT USED\n" );
    printf( " h or H: firmware & hardware information\n" );
    printf( " i or I: IR barriers\n" );
    printf( "   - p or P: toggle IR barriers power\n" );
    printf( "   - s or S: get IR barriers status\n" );
    printf( " j or J: files I/O on USB device\n" );
    printf( "   - c or C: display CSV files\n" );
    printf( "   - e or E: display errors file\n" );
    printf( "   - i or I: display CONFIG.INI\n" );
    printf( "   - l or L: list files\n" );
    printf( "   - t or T: delete LOG files\n" );
    printf( "   - u or U: delete CSV files\n" );
    printf( "   - v or V: delete config files\n" );
    printf( "   - w or W: delete BIN files\n" );
    printf( "   - x or X: export files\n" );
    printf( "   - z or Z: delete all files\n" );
    printf( " k or K: USB device properties\n" );
    printf( " l or L: scan I2C bus\n" ); //  --> 0x02, 0x06 and 0xE0
    printf( " m or M: measure\n" );
    printf( "   - b or B: battery level\n" );
    printf( "   - c or C: CTMU\n" );
    printf( "   - d or D: servomotor position\n" );
    printf( "   - r or R: RFID frequency\n" );
    printf( "   - t or T: external temperature\n" );
    printf( "   - v or V: VBat level\n" );
//                printf( " n or N: NOT USED\n" );
//                printf( " o or O: NOT USED\n" );                
    printf( " p or P: power\n" );
    printf( "   - 1: toggle servomotor power\n" );
    printf( "   - 2: toggle CMD_ACC_PIR power\n" );
    printf( "   - 3: toggle CMD_VDD_APP_V_USB power\n" );
    printf( "   - 4: toggle IR barriers power\n" );
    printf( " q or Q: quit serial communication state\n" );
    printf( " r or R: system reset\n" );
    printf( " s     : set date and time (string)\n" );
    printf( " S     : set date and time (numerical)\n" );
    printf( " t     : get date and time (string)\n" );
    printf( " T     : get date and time (numerical)\n" );
    printf( " u or U: Unique Device Id (UDID)\n" );
//                printf( " v or V: NOT USED\n" );
//                printf( " w or W: NOT USED\n" );
//                printf( " x or X: NOT USED\n" );
    printf( " y or Y: display reset registers\n" );
    printf( " z or Z: test RFID\n" );
}

void APP_SerialDebugTasks( void )
{
    int i;
    bool flag;
    FILEIO_DRIVE_PROPERTIES drive_properties;

    if ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) )
    {
        /* If there is at least one byte of data has been received. */
        uint8_t data_from_uart1 = UART1_Read( );

        switch ( data_from_uart1 )
        {
            case ',':
            case '?':
                
                displayKeyMapping( );
                break;

            case 'a':
            case 'A':
            {
                uint8_t user_choice;

                setDelayMsReadFromUart( MAX_READ_FROM_UART_DELAY );
                    
                while ( false == ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) && false == isDelayMsEndingReadFromUart( ) );
                if (true == isDelayMsEndingReadFromUart( ) )
                {
                    printf( "\n\tToo slow entering value => command aborted\n" );
                    break;
                }
                user_choice = UART1_Read( );

                switch ( user_choice )
                {
                    case 'b':
                    case 'B':
                    {
                        int16_t dc_pwm;
                        
                        /* Set blue color value of attractive LEDs. */
                        setLedsStatusColor( LED_BLUE );
                        printf( "Set blue LED PWM value in the range [0 255]\n" );

                        /* Read uint16_t from terminal. */
                        dc_pwm = readIntFromUart1( );
                        
                        /* If PWM value is not in the range [0 255] */
                        if ( dc_pwm < 0 || dc_pwm > 255 )
                        {
                            printf("\tWrong PWM value (%d)\r\n", dc_pwm);
                            break;
                        }                            
                            
                        printf( "Duty cycle PWM3: %u/255\n", (uint8_t)dc_pwm ); /* Check received data. */

                        /* Set DC of PWM3. */
                        setAttractiveBlueLedsColor( (uint8_t)dc_pwm );
                        setAttractiveLedsOn( );
                        break;
                    }
                    case 'g':
                    case 'G':
                    {

                        int16_t dc_pwm;
                        
                        /* Set green color value of attractive LEDs. */
                        setLedsStatusColor( LED_GREEN );
                        printf( "Set green LED PWM value in the range [0 255]\n" );

                        /* Read uint16_t from terminal. */
                        dc_pwm = readIntFromUart1( );
                        
                        /* If PWM value is not in the range [0 255] */
                        if ( dc_pwm < 0 || dc_pwm > 255 )
                        {
                            printf("\tWrong PWM value (%d)\r\n", dc_pwm);
                            break;
                        } 
                        printf( "Duty cycle PWM2: %u/255\n", (uint8_t)dc_pwm ); /* Check received data. */

                        /* Set DC of PWM2. */
                        setAttractiveGreenLedsColor( (uint8_t)dc_pwm );
                        setAttractiveLedsOn( );
                        break;
                    }
                    case 'i':
                    case 'I':
                    {
                        initAttractiveLeds( );
                        break;
                    }
                    case 'r':
                    case 'R':
                    {
                        int16_t dc_pwm;
                        
                        /* Set red color value of attractive LEDs. */
                        setLedsStatusColor( LED_RED );
                        printf( "Set red LED PWM value in the range [0 255]\n" );

                        /* Read uint16_t from terminal. */
                        dc_pwm = readIntFromUart1( );
                        
                        /* If PWM value is not in the range [0 255] */
                        if ( dc_pwm < 0 || dc_pwm > 255 )
                        {
                            printf("\tWrong PWM value (%d)\r\n", dc_pwm);
                            break;
                        } 
                        printf( "Duty cycle PWM1: %u/255\n", (uint8_t)dc_pwm ); /* Check received data. */

                        /* Set DC of PWM1. */
                        setAttractiveRedLedsColor( (uint8_t)dc_pwm );
                        setAttractiveLedsOn( );
                        break;
                    }
                    case 't':
                    case 'T':
                    {
                        testAttractiveLeds( );
                        break;
                    }

                }
            }
                break;
                /* -------------------------------------------------------------- */

            case 'b':
            case 'B':
                /* Display datalogger buffers. */
                printf( "Data logger buffer:\n" );
                if ( appDataLog.num_char_buffer > 0 )
                {
                    printf( "%s", appDataLog.buffer );
                }
                else
                {
                    printf( "\t<empty buffer>\n" );
                }
                /* Display battery level buffer  */
                printf( "\nBattery level buffer:\n" );

                if ( appDataLog.num_battery_level_stored == 0 )
                {
                    printf( "\t<empty buffer>\n" );
                }
                else
                {
                    for ( i = 0; i < appDataLog.num_battery_level_stored; i++ )
                    {
                        printf( "\t%02d - %04d - %2.3f\n",
                                appDataLog.battery_level[i][0],
                                appDataLog.battery_level[i][1],
                                appDataLog.battery_level[i][1] * BATTERY_VOLTAGE_FACTOR );
                    }
                }

                /* Display RFID frequency buffer  */
                printf( "\nRFID frequency buffer:\n" );

                if ( appDataLog.num_rfid_freq_stored == 0 )
                {
                    printf( "\t<empty buffer>\n" );
                }
                else
                {
                    for ( i = 0; i < appDataLog.num_rfid_freq_stored; i++ )
                    {
                        printf( "\t%02d:%02d - %06ld\n",
                                appDataLog.rfid_freq[i][0],
                                appDataLog.rfid_freq[i][1],
                                ( long ) appDataLog.rfid_freq[i][2]*10 );
                    }
                }

                /* Display DS3231 temperature  */
                printf( "\nCalibration buffer:\n" );
                if ( appDataLog.num_time_calib_stored == 0 )
                {
                    printf( "\t<empty buffer>\n" );
                }
                else
                {
                    for ( i = 0; i < appDataLog.num_time_calib_stored; i++ )
                    {
                        printf( "\t%02d:%02d - %.2f\n",
                                ( int ) appDataLog.time_calibration[i][0],
                                ( int ) appDataLog.time_calibration[i][1],
                                appDataLog.time_calibration[i][2] );
                    }
                }

                /* Display DS3231 temperature  */
                printf( "\nTemperature buffer:\n" );

                if ( appDataLog.num_ds3231_temp_stored == 0 )
                {
                    printf( "\t<empty buffer>\n" );
                }
                else
                {
                    for ( i = 0; i < appDataLog.num_ds3231_temp_stored; i++ )
                    {
                        printf( "\t%02d:%02d - %.2f\n",
                                ( int ) appDataLog.ds3231_temp[i][0],
                                ( int ) appDataLog.ds3231_temp[i][1],
                                ( double ) appDataLog.ds3231_temp[i][2] );
                    }
                }

                break;
                /* -------------------------------------------------------------- */

            case 'c':
            case 'C':
            {
                uint8_t user_choice;

                setDelayMsReadFromUart( MAX_READ_FROM_UART_DELAY );
                    
                while ( false == ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) && false == isDelayMsEndingReadFromUart( ) );
                if (true == isDelayMsEndingReadFromUart( ) )
                {
                    printf( "\n\tToo slow entering value => command aborted\n" );
                    break;
                }
                user_choice = UART1_Read( );

                switch ( user_choice )
                {
                    case 'd':
                    case 'D':
                    {

                        /* Display configuration parameters (CONFIG.INI) */
                        if ( false == appData.flags.bit_value.system_init )
                        {
                            printf( "/!\\ SYSTEM NOT CORRECTLY CONFIGURED.\n" );
                            config_print( );
                            printf( "/!\\ SYSTEM NOT CORRECTLY CONFIGURED.\n" );
                        }
                        else
                        {
                            config_print( );
                        }

                        break;
                    }

                    case 'r':
                    case 'R':
                    {

                        /* Reconfigure the system by reading the USB key again */
                        appData.need_to_reconfigure = true;
                        appData.state = APP_STATE_IDLE;
                        break;
                    }

                }
            }
                break;
                /* -------------------------------------------------------------- */

            case 'd':
            case 'D':
            {
                uint8_t user_choice;

                setDelayMsReadFromUart( MAX_READ_FROM_UART_DELAY );
                    
                while ( false == ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) && false == isDelayMsEndingReadFromUart( ) );
                if (true == isDelayMsEndingReadFromUart( ) )
                {
                    printf( "\n\tToo slow entering value => command aborted\n" );
                    break;
                }
                user_choice = UART1_Read( );

                switch ( user_choice )
                {
                    case 'C':
                    case 'c':

                        /* Close reward door */
                        if ( DOOR_CLOSED == appDataDoor.reward_door_status )
                        {
                            printf( "\tDoor already closed.\n" );
                            break;
                        }

                        if ( 1 == appDataDoor.remain_open )
                        {
                            appDataDoor.remain_open = 0;
                            printf( "\tClosing door (remain door open set to off)... " );
                        }
                        else
                        {
                            printf( "\tClosing door... " );
                        }

                        appDataServo.ton_goal = appDataServo.ton_min;

                        if ( false == isPowerServoEnable( ) )
                        {
                            servomotorPowerEnable( );
                            appDataServo.ton_cmd = servomotorGetDoorPosition( );
                            appDataDoor.reward_door_status = DOOR_MOVING;
                            while ( DOOR_MOVED != appDataDoor.reward_door_status );
                            servomotorPowerDisable( );
                        }
                        else
                        {
                            appDataServo.ton_cmd = servomotorGetDoorPosition( );
                            appDataDoor.reward_door_status = DOOR_MOVING;
                            while ( DOOR_MOVED != appDataDoor.reward_door_status );
                        }

                        appDataDoor.reward_door_status = DOOR_CLOSED;

                        printf( "Door closed\n" );

                        break;

                    case 'O':
                    case 'o':

                        /* Open reward door */
                        if ( DOOR_OPENED == appDataDoor.reward_door_status )
                        {
                            printf( "\tDoor already opened.\n" );
                            break;
                        }

                        printf( "\tOpening door... " );

                        appDataServo.ton_goal = appDataServo.ton_max;

                        appDataServo.num_step = appDataServo.num_empty_step;

                        if ( false == isPowerServoEnable( ) )
                        {
                            servomotorPowerEnable( );
                            appDataServo.ton_cmd = servomotorGetDoorPosition( );
                            appDataDoor.reward_door_status = DOOR_MOVING;
                            while ( DOOR_MOVED != appDataDoor.reward_door_status );
                            servomotorPowerDisable( );
                        }
                        else
                        {
                            appDataServo.ton_cmd = servomotorGetDoorPosition( );
                            appDataDoor.reward_door_status = DOOR_MOVING;
                            while ( DOOR_MOVED != appDataDoor.reward_door_status );
                        }

                        appDataDoor.reward_door_status = DOOR_OPENED;

                        printf( "Door opened\n" );

                        break;

                    case 'P':
                    case 'p':
                    {
                        /* Change servomotor position
                         * Servomotor:
                         * HS-322HD: 600 us < servo_position < 2400 us, flexible nylon noise --> Ok
                         * PARRALAX: 600 us < servo_position < 2400 us (Product ID: 900-00005), sound gear 
                         */

                        int16_t val;
                        
                        /* Get current position. */
                        if ( false == isPowerServoEnable( ) )
                        {
                            servomotorPowerEnable( );
                            appDataServo.ton_cmd = servomotorGetDoorPosition( );
                            servomotorPowerDisable( );
                        }
                        else
                        {
                            appDataServo.ton_cmd = servomotorGetDoorPosition( );
                        }

                        printf( "\tSet servomotor position\n\t                 C    O\n\t\tRange MAX: [%4u %4u]\n\t\tRange INI: [%4u %4u]\n", SERVO_DEFAULT_MIN_POSITION, SERVO_DEFAULT_MAX_POSITION, appDataServo.ton_min_night, appDataServo.ton_max );
                        if ( DOOR_HABITUATION == appData.scenario_number )
                        {
                            printf( "\t\tRange HAB: [%4u %4u] (%u%%)\n", appDataServo.ton_min, appDataServo.ton_max, appDataDoor.habituation_percent );
                        }
                        printf( "\t\tCurrent position: %u\n", appDataServo.ton_cmd );

                        /* Read uint16_t from terminal. */
                        val = readIntFromUart1( );

                        if ( 0 > val )
                        {
                            printf( "\tWrong value\n" );
                            break;
                        }
                        
                        appDataServo.ton_goal = (uint16_t)val;

                        if ( appDataServo.ton_goal > SERVO_DEFAULT_MAX_POSITION )
                        {
                            printf( "\tNo move because goal position (%u) is more than maximal position (%u)\n", appDataServo.ton_goal, SERVO_DEFAULT_MAX_POSITION );
                            break;
                        }
                        else if ( appDataServo.ton_goal < SERVO_DEFAULT_MIN_POSITION )
                        {
                            printf( "\tNo move because goal position (%u) is less than minimal position (%u)\n", appDataServo.ton_goal, SERVO_DEFAULT_MIN_POSITION );
                            break;
                        }
                        else if ( appDataServo.ton_goal > appDataServo.ton_max )
                        {
                            printf( "\tWarning: goal position (%u) is outside range specified in the INI file ([%u %u])\n", appDataServo.ton_goal, appDataServo.ton_min, appDataServo.ton_max );
                        }
                        else if ( appDataServo.ton_goal < appDataServo.ton_min )
                        {
                            printf( "\tWarning: goal position (%u) is outside range specified in the INI file ([%u %u])\n", appDataServo.ton_goal, appDataServo.ton_min, appDataServo.ton_max );
                        }

                        if ( appDataServo.ton_cmd == appDataServo.ton_goal )
                        {
                            printf( "\tDoor already at goal position.\n" );
                            break;
                        }

                        printf( "\tMoving door... " );

                        appDataServo.num_step = appDataServo.num_empty_step;

                        if ( false == isPowerServoEnable( ) )
                        {
                            servomotorPowerEnable( );
                            appDataDoor.reward_door_status = DOOR_MOVING;
                            while ( DOOR_MOVED != appDataDoor.reward_door_status );
                            servomotorPowerDisable( );
                        }
                        else
                        {
                            appDataDoor.reward_door_status = DOOR_MOVING;
                            while ( DOOR_MOVED != appDataDoor.reward_door_status );
                        }

                        printf( "Door moved\n" );

                        break;
                    }
                    
                    case 'R':
                    case 'r':

                        if ( 1 == appDataDoor.remain_open )
                        {
                            appDataDoor.remain_open = 0;
                            printf( "\tDoor remain open: off\n" );
                        }
                        else
                        {
                            appDataDoor.remain_open = 1;
                            printf( "\tDoor remain open: on\n" );
                        }

                        break;
                }
            }

                break;
                /* -------------------------------------------------------------- */

            case 'e':
            case 'E':

                /* Check status LEDs */
                checkLedsStatus( );
                
                break;
                /* -------------------------------------------------------------- */

            case 'f':
            case 'F':

                /* Flush data on USB device */
                appData.state = APP_STATE_FLUSH_DATA_TO_USB;
//                flushDataOnUsbDevice( );
                
                printf( "\r\n\t/!\\ Exit serial communication mode\r\n" );

                break;
                /* -------------------------------------------------------------- */

//            case 'g':
//            case 'G':
//                /* Not used. */
//                break;
                /* -------------------------------------------------------------- */

            case 'h':
            case 'H':

                /* Firmware information */

                putchar( '\t' );
                displayFirmwareVersion( );
                putchar( '\t' );
                displayBuildDateTime( );
                putchar( '\r' );

                /* Hardware information */

                displayDeviceId( );
                
                /* Display the Unique Device Id (UDID) */
                putchar( '\t' );
                displayUniqueDeviceId( );
                
                /* Display external interrupt and timers states */
                printf( "\tExternal interrupt states\n" );
                printf( "\t\tINT0: %d\n", IEC0bits.INT0IE );
                printf( "\t\tINT1: %d\n", IEC1bits.INT1IE );
                printf( "\t\tINT2: %d\n", IEC1bits.INT2IE );
                printf( "\t\tINT3: %d\n", IEC3bits.INT3IE );
                printf( "\t\tINT4: %d\n", IEC3bits.INT4IE );

                printf( "\r\n\tTimers\n" );
                printf( "\t\tTimer2: %d\n", T2CONbits.TON );
                printf( "\t\tTimer3: %d\n", T3CONbits.TON );
                printf( "\t\tTimer4: %d\n", T4CONbits.TON );

                /* Display all commands values */
                printf( "\r\n\tPower commands\n" );
                if ( CMD_VDD_ACC_PIR_SERVO_GetValue( ) == 1 )
                {
                    printf( "\t\tCMD_VDD_ACC_PIR: on\n" );
                }
                else
                {
                    printf( "\t\tCMD_VDD_ACC_PIR: off\n" );
                }

                if ( _TRISF1 )
                {
                    printf( "\t\tCMD_VCC_SERVO: off\n" );
                }
                else
                {
                    printf( "\t\tCMD_VCC_SERVO: on\n\n" );
                }

                if ( CMD_VDD_APP_V_USB_GetValue( ) == 0 )
                {
                    printf( "\t\tCMD_VDD_APP: off\n" );
                }
                else
                {
                    printf( "\t\tCMD_VDD_APP: on\n" );
                }

                if ( CMD_VDD_USB_GetValue( ) == 1 )
                {
                    printf( "\t\tCMD_VDD_USB: off\n" );
                }
                else
                {
                    printf( "\t\tCMD_VDD_USB: on\n\n" );
                }
                if ( CMD_VCC_IR_GetValue( ) == 1 )
                {
                    printf( "\t\tCMD_VCC_IR: off\n" );
                }
                else
                {
                    printf( "\t\tCMD_VCC_IR: on\n\n" );
                }
                if ( CMD_3V3_RF_GetValue( ) == 1 )
                {
                    printf( "\t\tCMD_3V3_RF: on\n" );
                }
                else
                {
                    printf( "\t\tCMD_3V3_RF: off\n" );
                }

                printf( "\r\n\tOutput compare\n" );
                printf( "\t\tOC4: %d\n", OC4CON1bits.OCM );
                printf( "\t\tOC4R: %d\n", OC4R );
                printf( "\t\tOC4RS: %d\n\n", OC4RS );
                printf( "\t\tOC5: %d\n", OC5CON1bits.OCM );
                printf( "\t\tOC5R: %d\n", OC5R );
                printf( "\t\tOC5RS: %d\n", OC5RS );

                /* Display USB device status */
                printf( "\r\n\tUSB device status: " );
                printUSBHostDeviceStatus( );
                putchar( '\r' );
                break;
                /* -------------------------------------------------------------- */

            case 'i':
            case 'I':

            {
                uint8_t user_choice;

                setDelayMsReadFromUart( MAX_READ_FROM_UART_DELAY );
                    
                while ( false == ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) && false == isDelayMsEndingReadFromUart( ) );
                if (true == isDelayMsEndingReadFromUart( ) )
                {
                    printf( "\n\tToo slow entering value => command aborted\n" );
                    break;
                }
                user_choice = UART1_Read( );

                switch ( user_choice )
                {
                    case 'p':
                    case 'P':
                    {
                        /* Toggle IR barriers power */
                        if ( CMD_VCC_IR_GetValue( ) == 1 )
                        {
                            /* Enable IR */
                            IRSensorEnable( );
                            setDelayMs( DELAY_MS_BEFORE_IR_ENABLE ); // TODO: adjust delay according to the datasheet
                            while ( false == isDelayMsEnding( ) );
                            printf( "\tIR enable.\n" );
                        }
                        else
                        {
                            /* Disable IR  */
                            IRSensorDisable( );
                            printf( "\tIR disable.\n" );
                        }

                        break;
                    }

                    case 's':
                    case 'S':
                    {
                        if ( CMD_VCC_IR_GetValue( ) == 1 )
                        {
                            printf( "\tIR barrier must be powered first\n");
                        }
                        else
                        {                            
                            /* Display IR1 Food status */
                            printf( "\tIR1 (reward) status: %u\n", BAR_IR1_OUT_GetValue( ) );
                            /* Display IR2 Food status */
                            printf( "\tIR2 (food) status: %u\n", BAR_IR2_OUT_GetValue( ) );
                        }

                        break;
                    }

                }

            }
                break;
                /* -------------------------------------------------------------- */

            case 'j':
            case 'J':
            {
                /* Files I/O on USB device */

                uint8_t user_choice;
                
                setDelayMsReadFromUart( MAX_READ_FROM_UART_DELAY );
                    
                while ( false == ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) && false == isDelayMsEndingReadFromUart( ) );
                if (true == isDelayMsEndingReadFromUart( ) )
                {
                    printf( "\n\tToo slow entering value => command aborted\n" );
                    break;
                }
                user_choice = UART1_Read( );

                switch ( user_choice )
                {

                    case 'a':
                    case 'A':
                        
                        if ( FILEIO_RESULT_SUCCESS != importFiles( ) )
                        {
                            printf( appError.message );
                        }
                        break;
                        
                    case 'C':
                    case 'c':

                        displayCsvFiles( );
                        break;

                    case 'e':
                    case 'E':

                        displayErrorsFile( );
                        break;

                    case 'i':
                    case 'I':

                        displayIniFile( );
                        break;

                    case 'L':
                    case 'l':

                        listFilesOnUsbDevice( );
                        break;

                    case 't':
                    case 'T':
                    {
                        uint8_t user_choice;

                        printf( "\tDo you really want to delete LOG files? (y/n)\r\n" );

                        setDelayMsReadFromUart( MAX_READ_FROM_UART_DELAY );
                    
                        while ( false == ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) && false == isDelayMsEndingReadFromUart( ) );
                        if (true == isDelayMsEndingReadFromUart( ) )
                        {
                            printf( "\n\tToo slow entering value => command aborted\n" );
                            break;
                        }
                        user_choice = UART1_Read( );

                        switch ( user_choice )
                        {
                            case 'y':
                            case 'Y':
                            {
                                deleteLogFiles( );
                                break;
                            }

                            default:
                                break;
                        }
                        
                        break;
                    }
                    
                    case 'u':
                    case 'U':
                    {
                        uint8_t user_choice;

                        printf( "\tDo you really want to delete CSV files? (y/n)\r\n" );

                        setDelayMsReadFromUart( MAX_READ_FROM_UART_DELAY );
                    
                        while ( false == ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) && false == isDelayMsEndingReadFromUart( ) );
                        if (true == isDelayMsEndingReadFromUart( ) )
                        {
                            printf( "\n\tToo slow entering value => command aborted\n" );
                            break;
                        }
                        user_choice = UART1_Read( );

                        switch ( user_choice )
                        {
                            case 'y':
                            case 'Y':
                            {
                                deleteCsvFiles( );
                                break;
                            }

                            default:
                                break;
                        }
                        
                        break;
                    }
                        
                    case 'v':
                    case 'V':
                    {
                        uint8_t user_choice;

                        printf( "\tDo you really want to delete configuration files? (y/n)\r\n" );

                        setDelayMsReadFromUart( MAX_READ_FROM_UART_DELAY );
                    
                        while ( false == ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) && false == isDelayMsEndingReadFromUart( ) );
                        if (true == isDelayMsEndingReadFromUart( ) )
                        {
                            printf( "\n\tToo slow entering value => command aborted\n" );
                            break;
                        }
                        user_choice = UART1_Read( );

                        switch ( user_choice )
                        {
                            case 'y':
                            case 'Y':
                            {
                                deleteConfigurationFiles( );
                                break;
                            }

                            default:
                                break;
                        }
                        
                        break;
                    }
                    
                    case 'w':
                    case 'W':
                    {
                        uint8_t user_choice;

                        printf( "\tDo you really want to delete BIN files? (y/n)\r\n" );

                        setDelayMsReadFromUart( MAX_READ_FROM_UART_DELAY );
                    
                        while ( false == ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) && false == isDelayMsEndingReadFromUart( ) );
                        if (true == isDelayMsEndingReadFromUart( ) )
                        {
                            printf( "\n\tToo slow entering value => command aborted\n" );
                            break;
                        }
                        user_choice = UART1_Read( );

                        switch ( user_choice )
                        {
                            case 'y':
                            case 'Y':
                            {
                                deleteEventFiles( );
                                break;
                            }

                            default:
                                break;
                        }
                        
                        break;
                    }

                    case 'x':
                    case 'X':

                        exportAllFiles( );
                        break;
                        
                    case 'z':
                    case 'Z':
                    {
                        uint8_t user_choice;

                        printf( "\tDo you really want to delete all files? (y/n)\r\n" );

                        setDelayMsReadFromUart( MAX_READ_FROM_UART_DELAY );
                    
                        while ( false == ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) && false == isDelayMsEndingReadFromUart( ) );
                        if (true == isDelayMsEndingReadFromUart( ) )
                        {
                            printf( "\n\tToo slow entering value => command aborted\n" );
                            break;
                        }
                        user_choice = UART1_Read( );

                        switch ( user_choice )
                        {
                            case 'y':
                            case 'Y':
                            {
                                deleteAllFiles( );
                                break;
                            }

                            default:
                                break;
                        }
                        
                        break;
                    }
                    default:
                        break;

                }

                break;
            }
                /* -------------------------------------------------------------- */

            case 'k':
            case 'K':
            {
                uint8_t user_choice;
                
                /* Get USB device properties */
                printf( "\tGet USB device properties is slow\n\t(approx. 7s per GB of drive space)\n" );
                printf( "\tDo you really want to continue? (y/n)\r\n" );

                setDelayMsReadFromUart( MAX_READ_FROM_UART_DELAY );
                    
                while ( false == ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) && false == isDelayMsEndingReadFromUart( ) );
                if (true == isDelayMsEndingReadFromUart( ) )
                {
                    printf( "\n\tToo slow entering value => command aborted\n" );
                    break;
                }
                
                user_choice = UART1_Read( );

                switch ( user_choice )
                {
                    case 'y':
                    case 'Y':
                    {
                        setLedsStatusColor( LED_USB_ACCESS );

                        usbMountDrive( );

                        drive_properties.new_request = true;
                        do
                        {
                            FILEIO_DrivePropertiesGet( &drive_properties, 'A' );
                        }
                        while ( drive_properties.properties_status == FILEIO_GET_PROPERTIES_STILL_WORKING );

                        if ( FILEIO_GET_PROPERTIES_NO_ERRORS == drive_properties.properties_status )
                        {
                            printf( "\tUSB device properties\n" );
                            if ( 1 == drive_properties.results.disk_format )
                            {
                                printf( "\t\tDrive format: FAT12\n" );
                            }
                            else if ( 2 == drive_properties.results.disk_format )
                            {
                                printf( "\t\tDrive format: FAT16\n" );
                            }
                            else if ( 3 == drive_properties.results.disk_format )
                            {
                                printf( "\t\tDrive format: FAT32\n" );
                            }
                            else
                            {
                                printf( "\t\tDrive format: unknown (%d)\n", drive_properties.results.disk_format );
                            }
                            printf( "\t\tSector size: %u\n", drive_properties.results.sector_size );
                            printf( "\t\tSector per cluster: %u\n", drive_properties.results.sectors_per_cluster );
                            printf( "\t\tTotal clusters: %lu\n", drive_properties.results.total_clusters );
                            printf( "\t\tFree clusters: %lu\n", drive_properties.results.free_clusters );
                            printf( "\t\tTotal space: %lu MB\n", drive_properties.results.total_clusters * drive_properties.results.sectors_per_cluster * drive_properties.results.sector_size / 1024 / 1024 );
                            printf( "\t\tFree space: %lu MB\n\n", drive_properties.results.free_clusters * drive_properties.results.sectors_per_cluster * drive_properties.results.sector_size / 1024 / 1024 );
                        }
                        else
                        {
                            printf( "\tUSB device properties\n\t\tGet properties failed (%d)\n\n", drive_properties.properties_status );
                        }

                        usbUnmountDrive( );

                        break;
                    }

                    default:
                        break;
                }
                
                break;
            }
                /* -------------------------------------------------------------- */

            case 'l':
            case 'L':
                /* Display number of I2C device found */
                i2cScanner( );
                break;
                /* -------------------------------------------------------------- */

            case 'm':
            case 'M':
            {

                uint8_t user_choice;
                uint16_t analog_measure;

                setDelayMsReadFromUart( MAX_READ_FROM_UART_DELAY );
                    
                while ( false == ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) && false == isDelayMsEndingReadFromUart( ) );
                if (true == isDelayMsEndingReadFromUart( ) )
                {
                    printf( "\n\tToo slow entering value => command aborted\n" );
                    break;
                }
                user_choice = UART1_Read( );

                switch ( user_choice )
                {
                    case 'b':
                    case 'B':
                    {
                        /* Battery level */
                        appData.battery_level = getADC1value( ADC1_CHANNEL_MA_12V );
                        printBatteryLevel( );
                        break;
                    }
                    case 'c':
                    case 'C':
                    {
                        /* CTMU */
                        analog_measure = getADC1value( ADC1_CHANNEL_CTMU_TEMPERATURE_SENSOR_INPUT );
                        printf( "\tCTMU temperature sensor: (%u)\n", analog_measure );
                        break;
                    }
                    case 'd':
                    case 'D':
                    {
                        /* Position of the servomotor. */
                        if ( false == isPowerServoEnable( ) )
                        {
                            servomotorPowerEnable( );
                            analog_measure = servomotorGetDoorPosition( );
                            servomotorPowerDisable( );
                        }
                        else
                        {
                            analog_measure = servomotorGetDoorPosition( );
                        }
                        printf( "\tServo position: %u\n", analog_measure );
                        break;
                    }
                    case 'r':
                    case 'R':
                    {
                        /* Measuring RDY/CLK period of EM4095 */
                        flag = measureRfidFreq( );
                        if ( flag )
                        {
                            displayRfidFreq( );
                        }
                        else
                        {
                            printf( "\tTimeout reached during RFID mesure.\n" );
                        }
                        break;
                    }

                    case 't':
                    case 'T':
                        /* DS3231 temperature*/

                        if ( 0 < APP_I2CMasterSeeksSlaveDevice( DS3231_I2C_ADDR, DS3231_I2C_ADDR ) )
                        {
                            getDS3231Temperature( );
                            printf( "\tTemperature: %.2f C\n", ( double ) appData.ext_temperature );
                        }
                        else
                        {
                            printf( "\tDS3231 not found.\n" );
                        }
                        break;

                    case 'v':
                    case 'V':
                    {
                        /* VBat level */
                        appData.vbat_level = getADC1value( ADC1_CHANNEL_VBAT_2 );
                        printVBatLevel( );
                        break;
                    }

                }

                break;
            }
                /* -------------------------------------------------------------- */

//            case 'n':
//            case 'N':
//            {
//                /* Not used */
//                break;
//            }
//                /* -------------------------------------------------------------- */
//
//            case 'o':
//            case 'O':
//
//                /* Not used */
//                break;
                /* -------------------------------------------------------------- */

            case 'p':
            case 'P':
            {
                uint8_t user_choice;

                setDelayMsReadFromUart( MAX_READ_FROM_UART_DELAY );
                    
                while ( false == ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) && false == isDelayMsEndingReadFromUart( ) );
                if (true == isDelayMsEndingReadFromUart( ) )
                {
                    printf( "\n\tToo slow entering value => command aborted\n" );
                    break;
                }
                user_choice = UART1_Read( );
                
                switch ( user_choice )
                {
                    case '1':

                        /* Set status of servomotor power command. */
                        appDataServo.cmd_vcc_servo_state ^= 1; // Toggle state "CMD_VCC_SERVO"
                        Nop( );
                        Nop( );
                        Nop( );
                        if ( true == appDataServo.cmd_vcc_servo_state )
                        {
                            servomotorPowerEnable( );
                            printf( "Servomotor power command enable.\n" );
                        }
                        else
                        {
                            servomotorPowerDisable( );
                            printf( "Servomotor power command disable.\n" );
                        }
                        appDataDoor.reward_door_status = DOOR_IDLE;
                        break;

                    case '2':

                        /* toggle power command "CMD_ACC_PIR". */
                        CMD_VDD_ACC_PIR_SERVO_Toggle( );
                        Nop( );
                        Nop( );
                        Nop( );
                        if ( true == CMD_VDD_ACC_PIR_SERVO_GetValue( ) )
                        {
                            printf( "CMD_ACC_PIR enable.\n" );
                            appData.pir_sensor_powered = true;
                        }
                        else
                        {
                            printf( "CMD_ACC_PIR disable.\n" );
                            appData.pir_sensor_powered = false;
                        }
                        break;

                    case '3':

                        /* toggle power command "CMD_VDD_APP". */
                        CMD_VDD_APP_V_USB_Toggle( );
                        Nop( );
                        Nop( );
                        Nop( );
                        if ( true == CMD_VDD_APP_V_USB_GetValue( ) )
                        {
                            printf( "CMD_VDD_APP_V_USB enable.\n" );
                        }
                        else
                        {
                            printf( "CMD_VDD_APP_V_USB disable.\n" );
                        }
                        break;

                    case '4':
                        /* Toggle IR barriers power */
                        if ( CMD_VCC_IR_GetValue( ) == 1 )
                        {
                            /* Enable IR */
                            IRSensorEnable( );
                            setDelayMs( DELAY_MS_BEFORE_IR_ENABLE ); // TODO: adjust delay according to the datasheet
                            while ( false == isDelayMsEnding( ) );
                            printf( "\tIR enable.\n" );
                        }
                        else
                        {
                            /* Disable IR  */
                            IRSensorDisable( );
                            printf( "\tIR disable.\n" );
                        }

                        break;

                }

                break;
            }
                /* -------------------------------------------------------------- */

            case 'q':
            case 'Q':
                
                /* Quit serial communication state => back to idle state */
                appData.state = APP_STATE_IDLE;
                
                printf( "\r\n\t/!\\ Exit serial communication mode\r\n" );
                
                break;
                /* -------------------------------------------------------------- */

            case 'r':
            case 'R':
            {
                uint8_t user_choice;

                printf( "\tDo you really want to reset the system? (y/n)\r\n" );

                setDelayMsReadFromUart( MAX_READ_FROM_UART_DELAY );
                    
                while ( false == ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) && false == isDelayMsEndingReadFromUart( ) );
                if (true == isDelayMsEndingReadFromUart( ) )
                {
                    printf( "\n\tToo slow entering value => command aborted\n" );
                    break;
                }
                user_choice = UART1_Read( );

                switch ( user_choice )
                {
                    case 'y':
                    case 'Y':
                    {
                        /* Reset */
                        appData.dsgpr0.bit_value.num_software_reset = 0;
                        DSGPR0 = appData.dsgpr0.reg;
                        DSGPR0 = appData.dsgpr0.reg;

                        __asm__ volatile ( "reset" );
                        break;
                    }

                    default:
                        break;
                }

                break;
            }
            /* -------------------------------------------------------------- */
            
            case 's':
            {
                /* Set RTCC module date and time value. */
                /* Example 22/08/2016 and time to 15:59:30 */
                // setDateTime( 16, 8, 22, 15, 59, 30 );
                // setExtDateTime( 16, 8, 22, 15, 59, 30 );

                int16_t day, month, year, hour, minute, second;

                printf( "\tSet date dd/mm/20yy and time hh:mm:ss\n" );

                printf( "\tSet day (1 to 31):\n" );
                day = readIntFromUart1( );
                if ( day < 1 || day > 31 )
                {
                    printf( "\tWrong day value (%d) => Date and time not set.\n", day );
                    
                    /* Log event if required */
                    if ( true == appDataLog.log_events )
                    {
                       store_event(OF_DEBUG_SET_DATE_TIME_FAIL); 
                    }
                    break;
                }
                printf( "\tSet month (1 to 12):\n" );
                month = readIntFromUart1( );
                if ( month < 1 || month > 12 )
                {
                    printf( "\tWrong month value (%d) => Date and time not set.\n", month );
                    
                    /* Log event if required */
                    if ( true == appDataLog.log_events )
                    {
                       store_event(OF_DEBUG_SET_DATE_TIME_FAIL); 
                    }
                    break;
                }
                
                printf( "\tSet year (%d to %d):\n", MIN_ADMISSIBLE_YEAR, MAX_ADMISSIBLE_YEAR );
                year = readIntFromUart1( );
                if ( year < MIN_ADMISSIBLE_YEAR || year > MAX_ADMISSIBLE_YEAR )
                {
                    printf( "\tWrong year value (%d) => Date and time not set.\n", year );
                    
                    /* Log event if required */
                    if ( true == appDataLog.log_events )
                    {
                       store_event(OF_DEBUG_SET_DATE_TIME_FAIL); 
                    }
                    break;
                }
                
                printf( "\tSet hours (0 to 23):\n" );
                hour = readIntFromUart1( );
                if ( hour < 0 || hour > 23 )
                {
                    printf( "\tWrong hour value (%d) => Date and time not set.\n", hour );
                    
                    /* Log event if required */
                    if ( true == appDataLog.log_events )
                    {
                       store_event(OF_DEBUG_SET_DATE_TIME_FAIL); 
                    }
                    break;
                }
                
                printf( "\tSet minutes (0 to 59):\n" );
                minute = readIntFromUart1( );
                if ( minute < 0 || minute > 59 )
                {
                    printf( "\tWrong minute value (%d) => Date and time not set.\n", minute );
                    
                    /* Log event if required */
                    if ( true == appDataLog.log_events )
                    {
                       store_event(OF_DEBUG_SET_DATE_TIME_FAIL); 
                    }
                    break;
                }
                
                printf( "\tSet seconds (0 to 59):\n" );
                second = readIntFromUart1( );
                if ( second < 0 || second > 59 )
                {
                    printf( "\tWrong second value (%d) => Date and time not set.\n", second );
                    
                    /* Log event if required */
                    if ( true == appDataLog.log_events )
                    {
                       store_event(OF_DEBUG_SET_DATE_TIME_FAIL); 
                    }
                    break;
                }
               
                /* Set PIC date and time. */
                if ( setDateTime( (uint8_t)year, (uint8_t)month, (uint8_t)day, (uint8_t)hour, (uint8_t)minute, (uint8_t)second ) )
                {
                    printf( "\tPIC date and time set.\n" );
                }
                else
                {
                    printf( "\tPIC date and time not set.\n" );
                }
                /* Set DS3231 date and time. */
                if ( setExtDateTime( (uint8_t)year, (uint8_t)month, (uint8_t)day, (uint8_t)hour, (uint8_t)minute, (uint8_t)second ) )
                {
                    printf( "\tEXT date and time set.\n" );
                }
                else
                {
                    printf( "\tEXT date and time not set.\n" );
                }
                
                break;
            }
                /* -------------------------------------------------------------- */

            case 'S':
            {
                /* Set RTCC module date and time value. */
                char date[6];
                uint8_t numBytes = 0;

                setDelayMsReadFromUart( MAX_READ_FROM_UART_DELAY );
                
                while ( numBytes < 6 && false == isDelayMsEndingReadFromUart( ) )
                {
                    if ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) )
                    {
                        date[numBytes] = UART1_Read( );

                        ++numBytes;
                    }
                }
                if (true == isDelayMsEndingReadFromUart( ) )
                {
                    printf( "\n\tToo slow entering value => command aborted\n" );
                    break;
                }

                if ( date[2] < 1 || date[2] > 31 || 
                    date[1] < 1 || date[1] > 12 ||
                    date[0] < MIN_ADMISSIBLE_YEAR || date[0] > MAX_ADMISSIBLE_YEAR ||
                    date[3] < 0 || date[3] > 23 ||
                    date[4] < 0 || date[4] > 59 ||
                    date[5] < 0 || date[5] > 59)
                {
                    printf( "\tWrong value => Date and time not set.\n" );
                    
                    /* Log event if required */
                    if ( true == appDataLog.log_events )
                    {
                       store_event(OF_DEBUG_SET_DATE_TIME_FAIL); 
                    }
                }
                else
                {
                    if ( setExtDateTime( date[0], date[1], date[2], date[3], date[4], date[5] ) )
                    {
                        calibrateDateTime( );
                    }
                    else
                    {
                        setDateTime( date[0], date[1], date[2], date[3], date[4], date[5] );
                    }
                }

                break;
            }
                /* -------------------------------------------------------------- */

            case 't':

                /* Display date and time from RTCC module. */
                if ( getDateTime( ) )
                {
                    printf( "\tPIC: " );
                    printDateTime( appData.current_time );
                    printf( " (OK)\n" );
                }
                else
                {
                    printf( "\tPIC: " );
                    printDateTime( appData.current_time );
                    printf( " (PB)\n" );
                    
                    /* Log event if required */
                    if ( true == appDataLog.log_events )
                    {
                       store_event(OF_DEBUG_GET_PIC_DATE_TIME_FAIL); 
                    }
                }

                if ( getExtDateTime( ) )
                {
                    printf( "\tEXT: " );
                    printExtDateTime( );
                    printf( " (OK)\r\n" );
                }
                else
                {
                    printf( "\tEXT: " );
                    printExtDateTime( );
                    printf( " (PB)\r\n" );
                    
                    /* Log event if required */
                    if ( true == appDataLog.log_events )
                    {
                       store_event(OF_DEBUG_GET_EXT_DATE_TIME_FAIL); 
                    }
                }

                break;
                /* -------------------------------------------------------------- */

            case 'T':
            {
                uint8_t date[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

                if ( getDateTime( ) )
                {
                    date[0] = ( uint8_t ) appData.current_time.tm_year;
                    date[1] = ( uint8_t ) appData.current_time.tm_mon;
                    date[2] = ( uint8_t ) appData.current_time.tm_mday;
                    date[3] = ( uint8_t ) appData.current_time.tm_hour;
                    date[4] = ( uint8_t ) appData.current_time.tm_min;
                    date[5] = ( uint8_t ) appData.current_time.tm_sec;
                }
                else
                {
                    /* Log event if required */
                    if ( true == appDataLog.log_events )
                    {
                       store_event(OF_DEBUG_GET_PIC_DATE_TIME_FAIL); 
                    }
                }

                if ( getExtDateTime( ) )
                {
                    date[6] = ( uint8_t ) appData.i2c_current_time.year_s;
                    date[7] = ( uint8_t ) appData.i2c_current_time.mon;
                    date[8] = ( uint8_t ) appData.i2c_current_time.mday;
                    date[9] = ( uint8_t ) appData.i2c_current_time.hour;
                    date[10] = ( uint8_t ) appData.i2c_current_time.min;
                    date[11] = ( uint8_t ) appData.i2c_current_time.sec;
                }
                else
                {
                    /* Log event if required */
                    if ( true == appDataLog.log_events )
                    {
                       store_event(OF_DEBUG_GET_EXT_DATE_TIME_FAIL); 
                    }
                }

                UART1_WriteBuffer( date, 12 );

                break;
            }
                /* -------------------------------------------------------------- */

            case 'u':
            case 'U':
                /* Display the Unique Device Id (UDID) */
                putchar( '\t' );
                displayUniqueDeviceId( );
                break;
                /* -------------------------------------------------------------- */

//            case 'v':
//            case 'V':
//                /* Not used */
//                break;
//                /* -------------------------------------------------------------- */
//
//            case 'w':
//            case 'W':
//                /* Not used */
//                break;
                /* -------------------------------------------------------------- */

//            case 'x':
//            case 'X':
//            
//                break;
                /* -------------------------------------------------------------- */

            case 'y':
            case 'Y':
                /* Display reset registers */
                displayResetRegisters( );
                break;
                /* -------------------------------------------------------------- */

            case 'z':
            case 'Z':
                /* Test RFID. */
                appData.state = APP_STATE_TEST_RFID;
                break;
                /* -------------------------------------------------------------- */

            default:
                putchar( 9 );
                putchar( data_from_uart1 ); /* echo RX data if doesn't match */
                putchar( ' ' );
                putchar( '(' );
                putchar( 'd' );
                putchar( ')' );
                putchar( 13 );
                putchar( 10 );
                break;
        }
    } /* end of if ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) */
}

uint8_t getCompletScenarioNumber( void )
{
    if ( GO_NO_GO != appData.scenario_number )
    {
        return appData.scenario_number;
    }
    else
    {
        return (30 + appDataAttractiveLeds.pattern_number );
    }
}


void getDeviceId( void )
{
    int tblpageReg, addrOffset, readDataL;
    //    int readDataH;

    // Read 24 bits of data memory from address 0x012340
    // Create 24 bit EA for read by loading TBLPAG
    tblpageReg = TBLPAG;
    TBLPAG = 0x00FF; // Load TBLPAG register with read address <23:16>

    addrOffset = 0x0000; // Load offset with read address <15:0>
    // Read data from program memory
    readDataL = __builtin_tblrdl( addrOffset ); // readDataL contains lower word data
    //    readDataH = __builtin_tblrdh(addrOffset);   // readDataH contains high byte data

    appData.id.family = readDataL >> 8;
    appData.id.device = readDataL & 0xFF;

    addrOffset = 0x0002;
    readDataL = __builtin_tblrdl( addrOffset ); // readDataL contains lower word data
    //    readDataH = __builtin_tblrdh(addrOffset);   // readDataH contains high byte data

    appData.id.revision = readDataL;

    TBLPAG = tblpageReg;

}


void displayDeviceId( void )
{
    /* DS30010089D-page 482 */

    if ( appData.id.family == 97 && appData.id.device == 20 )
    {
        printf( "\tMicrocontroler:\n\t\tPIC24FJ256GB406\n" );
    }
    else
    {
        printf( "\tMicrocontroler:\n\t\tunknown\n" );
    }
    printf( "\tIndividual Device Identifier\n" );
    printf( "\t\tFamily ID: %u\n", appData.id.family );
    printf( "\t\tIndividual ID: %u\n", appData.id.device );
    printf( "\t\tRevision: %u\n\n", appData.id.revision );
}


void getUniqueDeviceId( void )
{
    /* DS30010089D-page 74 */

    int tblpageReg, addrOffset;

    uint32_t readDataL, readDataH;

    tblpageReg = TBLPAG;

    TBLPAG = tblpageReg;

    TBLPAG = 0x0080;

    //    addrOffset = 0x1300; 
    addrOffset = 0x1308;
    readDataL = __builtin_tblrdl( addrOffset );
    readDataH = __builtin_tblrdh( addrOffset );

    appData.udid.words[0] = ( readDataH << 16 ) + readDataL;

    //    addrOffset = 0x1302; 
    addrOffset = 0x130A;
    readDataL = __builtin_tblrdl( addrOffset );
    readDataH = __builtin_tblrdh( addrOffset );

    appData.udid.words[1] = ( readDataH << 16 ) + readDataL;

    //    addrOffset = 0x1304; 
    addrOffset = 0x130C;
    readDataL = __builtin_tblrdl( addrOffset );
    readDataH = __builtin_tblrdh( addrOffset );

    appData.udid.words[2] = ( readDataH << 16 ) + readDataL;

    //    addrOffset = 0x1306; 
    addrOffset = 0x130E;
    readDataL = __builtin_tblrdl( addrOffset );
    readDataH = __builtin_tblrdh( addrOffset );

    appData.udid.words[3] = ( readDataH << 16 ) + readDataL;

    //    printf(" %u %u\n", readDataL, readDataH);

    //    addrOffset = 0x1308; 
    addrOffset = 0x1310;
    readDataL = __builtin_tblrdl( addrOffset );
    readDataH = __builtin_tblrdh( addrOffset );

    appData.udid.words[4] = ( readDataH << 16 ) + readDataL;

    TBLPAG = tblpageReg;
}


void displayUniqueDeviceId( void )
{

    printf( "UDID: %06lX %06lX %06lX %06lX %06lX\r\n", appData.udid.words[0],
            appData.udid.words[1],
            appData.udid.words[2],
            appData.udid.words[3],
            appData.udid.words[4] );

}


/* Display information on serial terminal. */
void displayBuildDateTime( void )
{
    /* Displaying the build date and time. */
    printf( "Build on %s, %s\n", BUILD_DATE, BUILD_TIME );
}


void displayFirmwareVersion( void )
{
    /* Displaying firmware version. */
    printf( "Firmware: %s v%d.%d.%d\n", FW_NAME, FW_VERSION_MAJOR, FW_VERSION_MINOR, FW_VERSION_PATCH );
}


void displayBootMessage( void )
{
    printf( "\n\n================ OpenFeeder ================\n\t" );
    displayFirmwareVersion( );
    printf( "\t" );
    displayBuildDateTime( );
    printf( "\tFor board v3.0\n" );
    printf( "============================================\n\t" );
    getDateTime( );
    printDateTime( appData.current_time );
    printf( "\n============================================\n\t" );
    printResetSituation( );
    printf( "============================================\n\t" );
    displayUniqueDeviceId( );
    printf( "============================================\n" );
    printf( "\tWeb: https://github.com/OpenFeeder\n" );
    printf( "\tMail: contact.openfeeder@gmail.com\n" );
    printf( "============================================\n" );
    printf( "\tType [?] key to display debug options.\n" );
    printf( "============================================\n\n" );
}


void displayResetRegisters( void )
{

    printf( "\t-----------------------\n" );
    printf( "\t|   RCON1  |   RCON2  |\n" );
    printf( "\t| POR    %u | VBAT   %u |\n", appData.reset_1.bit_value.por, appData.reset_2.bit_value.vbat );
    printf( "\t| BOR    %u | VBPOR  %u |\n", appData.reset_1.bit_value.bor, appData.reset_2.bit_value.vbpor );
    printf( "\t| IDLE   %u | VDDPOR %u |\n", appData.reset_1.bit_value.idle, appData.reset_2.bit_value.vddpor );
    printf( "\t| SLEEP  %u | VDDBOR %u |\n", appData.reset_1.bit_value.sleep, appData.reset_2.bit_value.vddbor );
    printf( "\t| WDTO   %u |     -    |\n", appData.reset_1.bit_value.wdto );
    printf( "\t| SWDTEN %u |     -    |\n", appData.reset_1.bit_value.swdten );
    printf( "\t| SWR    %u |     -    |\n", appData.reset_1.bit_value.swr );
    printf( "\t| EXTR   %u |     -    |\n", appData.reset_1.bit_value.extr );
    printf( "\t| PMSLP  %u |     -    |\n", appData.reset_1.bit_value.pmslp );
    printf( "\t| CM     %u |     -    |\n", appData.reset_1.bit_value.cm );
    printf( "\t| DPSLP  %u |     -    |\n", appData.reset_1.bit_value.dpslp );
    printf( "\t|     -    |     -    |\n" );
    printf( "\t| RETEN  %u |     -    |\n", appData.reset_1.bit_value.reten );
    printf( "\t|     -    |     -    |\n" );
    printf( "\t| IOPUWR %u |     -    |\n", appData.reset_1.bit_value.iopuwr );
    printf( "\t| TRAPR  %u |     -    |\n", appData.reset_1.bit_value.trapr );
    printf( "\t-----------------------\n\n" );

}

int16_t readIntFromUart1( void )
{
    char rx_data_buffer[UART1_BUFFER_SIZE];
    uint8_t numBytes;

    setDelayMsReadFromUart( MAX_READ_FROM_UART_DELAY );
        
    numBytes = 0; /* initialized numBytes */
    do
    {
        if ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) )
        {
            rx_data_buffer[numBytes] = UART1_Read( );

            /* if received <CR> the stop received */
            if ( '\r' == rx_data_buffer[numBytes] )
            {
                break;
            }

            /* if received data is not a numerical value then return -1 */
            if ( false == isdigit( rx_data_buffer[numBytes] ) )
            {
                return -1;
            }

            setDelayMsReadFromUart( MAX_READ_FROM_UART_DELAY );
            
            ++numBytes;
        }
    }
    while ( numBytes < UART1_BUFFER_SIZE && false == isDelayMsEndingReadFromUart( ) );

    if ( false == isDelayMsEndingReadFromUart( ) )
    {
        rx_data_buffer[numBytes + 1] = '\0'; /* add end of string */

        return ( int16_t ) strtol( rx_data_buffer, NULL, 10 );
    }
    else
    {
        printf( "\n\tToo slow entering value\n" );
        return -1;
    }
} /* End of readIntFromUart1( ) */


/**
 * Print the status of USBHostDeviceStatus()
 *  USB_DEVICE_ATTACHED                 - Device is attached and running
 *  USB_DEVICE_DETACHED                 - No device is attached
 *  USB_DEVICE_ENUMERATING              - Device is enumerating
 *  USB_HOLDING_OUT_OF_MEMORY           - Not enough heap space available
 *  USB_HOLDING_UNSUPPORTED_DEVICE      - Invalid configuration or unsupported class
 *  USB_HOLDING_UNSUPPORTED_HUB         - Hubs are not supported
 *  USB_HOLDING_INVALID_CONFIGURATION   - Invalid configuration requested
 *  USB_HOLDING_PROCESSING_CAPACITY     - Processing requirement excessive
 *  USB_HOLDING_POWER_REQUIREMENT       - Power requirement excessive
 *  USB_HOLDING_CLIENT_INIT_ERROR       - Client driver failed to initialize
 *  USB_DEVICE_SUSPENDED                - Device is suspended
 *  Other                               - Device is holding in an error state.
 *                                        The return value indicates the error.
 */
void printUSBHostDeviceStatus( void )
{
    uint8_t status = USBHostDeviceStatus( appDataUsb.device_address );

    switch ( status )
    {
        case USB_DEVICE_ATTACHED:
            printf( "USB_DEVICE_ATTACHED" );
            break;

        case USB_DEVICE_DETACHED:
            printf( "USB_DEVICE_DETACHED" );
            break;

        case USB_DEVICE_ENUMERATING:
            printf( "USB_DEVICE_ENUMERATING" );
            break;

        case USB_HOLDING_OUT_OF_MEMORY:
            printf( "USB_HOLDING_OUT_OF_MEMORY" );
            break;

        case USB_HOLDING_UNSUPPORTED_DEVICE:
            printf( "USB_HOLDING_UNSUPPORTED_DEVICE" );
            break;

        case USB_HOLDING_UNSUPPORTED_HUB:
            printf( "USB_HOLDING_UNSUPPORTED_HUB" );
            break;

        case USB_HOLDING_INVALID_CONFIGURATION:
            printf( "USB_HOLDING_INVALID_CONFIGURATION" );
            break;

        case USB_HOLDING_PROCESSING_CAPACITY:
            printf( "USB_HOLDING_PROCESSING_CAPACITY" );
            break;

        case USB_HOLDING_POWER_REQUIREMENT:
            printf( "USB_HOLDING_POWER_REQUIREMENT" );
            break;

        case USB_HOLDING_CLIENT_INIT_ERROR:
            printf( "USB_HOLDING_CLIENT_INIT_ERROR" );
            break;

        case USB_DEVICE_SUSPENDED:
            printf( "USB_DEVICE_SUSPENDED" );
            break;

        default:
            /* If nothing else matches, do the default. */
            printf( "Device is holding in an error state! (%d)", status );
            break;
    }
} /* End of printUSBHostDeviceStatus( ) */

#endif


/*******************************************************************************
 End of File
 */
