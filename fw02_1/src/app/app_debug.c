/**
 * @file app_debug.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 06/07/2016
 * @revision history 3
 */

#include "app.h"
#include "app_debug.h"
#include <ctype.h>

#if defined (USE_UART1_SERIAL_INTERFACE)

/* Current date to a C string (page 244)) */
const uint8_t BUILD_DATE[] = { __DATE__ };
const uint8_t BUILD_TIME[] = { __TIME__ };

/* Display information on serial terminal. */
void displayBuildDateTime( void )
{
    /* Displaying the build date and time. */
    printf( "Build on %s, %s\n", BUILD_DATE, BUILD_TIME );
}

void displayBootMessage( void )
{
    printf( "\n\n================ OpenFeeder ================\n" );
    printf( "      Board: v3.0 - Firmware: fw02_1\n" );
    printf( "      Build on %s, %s\n", BUILD_DATE, BUILD_TIME );
    printf( "============================================\n" );
    printf( "   Web page: https://github.com/OpenFeeder\n" );
    printf( "   Mail: contact.openfeeder@gmail.com\n" );
    printf( "============================================\n" );
    printf( "Type [?] key to display the Key mapping interface.\n\n" );
}

void displayResetRegisters( void )
{
    RESET_SYSTEM_CONTROL_REGISTER_1_t rst_sys_ctrl1_value; // variable for reading RCON1 register
    RESET_SYSTEM_CONTROL_REGISTER_2_t rst_sys_ctrl2_value; // variable for reading RCON2 register
        
    rst_sys_ctrl1_value.status_reg = RCON; // save register
    RCON = 0; // clear register
    
    /* RESET AND SYSTEM CONTROL REGISTER 2 */
    // bit 3 VDDBOR : VDD Brown - out Reset Flag bit( 1 )
    //    1 = A VDD Brown - out Reset has occurred( set by hardware )
    //    0 = A VDD Brown - out Reset has not occurred
    // bit 2 VDDPOR : VDD Power - on Reset Flag bit( 1, 2 )
    //    1 = A VDD Power - on Reset has occurred( set by hardware )
    //    0 = A VDD Power - on Reset has not occurred
    // bit 1 VBPOR : VBPOR Flag bit( 1, 3 )
    //    1 = A VBAT POR has occurred( no battery connected to VBAT pin or VBAT power below Deep Sleep
    //                                 Semaphore register retention level is set by hardware )
    //    0 = A VBAT POR has not occurred
    // bit 0 VBAT : VBAT Flag bit( 1 )
    //    1 = A POR exit has occurred while power was applied to VBAT pin( set by hardware )
    //    0 = A POR exit from VBAT has not occurred
    rst_sys_ctrl2_value.status_reg = RCON2; // save register
    RCON2 = 0; // clear register
    
//    printf( "\nReset flag bits\n" );
    printf( "\t-----------------------\n" );
    printf( "\t|   RCON1  |   RCON2  |\n" );
    printf( "\t| POR    %u | VBAT   %u |\n", rst_sys_ctrl1_value.status_bit.POR, rst_sys_ctrl2_value.status_bit.VBAT );
    printf( "\t| BOR    %u | VBPOR  %u |\n", rst_sys_ctrl1_value.status_bit.BOR, rst_sys_ctrl2_value.status_bit.VBPOR );
    printf( "\t| IDLE   %u | VDDPOR %u |\n", rst_sys_ctrl1_value.status_bit.IDLE, rst_sys_ctrl2_value.status_bit.VDDPOR );
    printf( "\t| SLEEP  %u | VDDBOR %u |\n", rst_sys_ctrl1_value.status_bit.SLEEP, rst_sys_ctrl2_value.status_bit.VDDBOR );
    printf( "\t| WDTO   %u |     -    |\n", rst_sys_ctrl1_value.status_bit.WDTO );
    printf( "\t| SWDTEN %u |     -    |\n", rst_sys_ctrl1_value.status_bit.SWDTEN );
    printf( "\t| SWR    %u |     -    |\n", rst_sys_ctrl1_value.status_bit.SWR );
    printf( "\t| EXTR   %u |     -    |\n", rst_sys_ctrl1_value.status_bit.EXTR );
    printf( "\t| PMSLP  %u |     -    |\n", rst_sys_ctrl1_value.status_bit.PMSLP );
    printf( "\t| CM     %u |     -    |\n", rst_sys_ctrl1_value.status_bit.CM );
    printf( "\t| DPSLP  %u |     -    |\n", rst_sys_ctrl1_value.status_bit.DPSLP );
    printf( "\t|     -    |     -    |\n");
    printf( "\t| RETEN  %u |     -    |\n", rst_sys_ctrl1_value.status_bit.RETEN );
    printf( "\t|     -    |     -    |\n");
    printf( "\t| IOPUWR %u |     -    |\n", rst_sys_ctrl1_value.status_bit.IOPUWR );
    printf( "\t| TRAPR  %u |     -    |\n", rst_sys_ctrl1_value.status_bit.TRAPR );
    printf( "\t-----------------------\n\n" );
}

void APP_SerialDebugTasks( void )
{
    uint16_t dc_pwm;
    int i, j;

    if ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) )
    {
        /* If there is at least one byte of data has been received. */
        uint8_t data_from_uart1 = UART1_Read( );

        switch ( data_from_uart1 )
        {
            case ',':
            case '?':
                /* Interface firmware terminal (Debug) */
                printf( "Key mapping:\n" );
                printf( " !: displaying the build date and time\n" );
                printf( " a or A: analog measure of the servomotor position, battery voltage and VBAT voltage\n" );
                printf( " b or B: set blue color value of RGB attractive LEDs\n" );
                printf( " c or C: close door\n" );
                printf( " d or D: display internal buffers\n" );
                printf( " e or E: measuring RDY/CLK period of EM4095\n" );
                printf( " f or F: display configuration parameters (CONFIG.INI)\n" );
                printf( " g or G: set green color value of RGB attractive LEDs\n" );
                printf( " h or H: toggle door remain open parameter\n" );
                printf( " i or I: enable IR power\n" );
                printf( " j or J: disable IR power\n" );
                printf( " k or K: display IR1 and IR2 state\n" );
                printf( " l or L: call i2cScanner( ) to found I2C devices attached.\n" ); //  --> 0x02, 0x06 and 0xE0
                printf( " m or M: menu to control PCA9622 device\n" );
                printf( "\t> 1: initialize the PCA9622 device\n" );
                printf( "\t> 2: Toggle Output Enable (OE) pin\n" );
                printf( "\t> 3: Toggle LED D16 color green or red\n" );
                printf( " n or N: NOT AFFECTED\n" );
                printf( " o or O: open door\n" );
                printf( " p or P: change servomotor position\n" );
                printf( " q or Q: NOT AFFECTED\n" );
                printf( " r or R: set red color value of RGB attractive LEDs\n" );
                printf( " s or S: set RTCC module date and time value\n" );
                printf( " t or T: display date and time from RTCC module\n" );
                printf( " u or U: display USB device status\n" );
                printf( " v or V: set status of servomotor power command\n" );
                printf( " w or W: toggle power command (CMD_ACC_PIR)\n" );
                printf( " x or X: display external interrupt and timers states\n" );
                printf( " y or Y: display all commands values\n");
                printf( " z or Z: check important parameters\n" );
                break;

            case '!':
                displayBuildDateTime( );
                break;

            case 'a':
            case 'A':
            {
                /* analogic measure of the current position of the servomotor. */
                uint16_t analog_measure;
//                analog_measure = getADC1value( ADC1_CHANNEL_MA_SERVO );

                /* Mesure with jumper closed from 3904 to 4078 --> ~4060 */
//                printf( "servo_position_analog = %u\n", analog_measure );

                printf( "servo_position_analog = %u\n", getDoorPosition() );
                   
                /* Battery level */
                appData.battery_level = getADC1value( ADC1_CHANNEL_MA_12V );
                printBatteryLevel( );

                /* VBat level */
                appData.vbat_level = getADC1value( ADC1_CHANNEL_AVSS );
                Nop( );
                Nop( );
                Nop( );
                appData.vbat_level = getADC1value( ADC1_CHANNEL_VBAT_2 );
                printVBatLevel( );

                /* Light level --> not in v3_0 board */
//                appData.light_level = getADC1value( ADC1_CHANNEL_MA_LIGHT );
//                printf( "Light level: (%u)\n", appData.light_level );

                /* CTMU */
                analog_measure = getADC1value( ADC1_CHANNEL_CTMU_TEMPERATURE_SENSOR_INPUT );
                printf( "CTMU temperature sensor: (%u)\n", analog_measure );
                
                break;
            }
                /* -------------------------------------------------------------- */

            case 'b':
            case 'B':
                /* Set blue color value of attractive LEDs. */
                setLedsStatusColor( LED_BLUE );
                printf( "Set Blue LED, enter a value from 0 to 255:\n" );

                /* Read uint16_t from terminal. */
                dc_pwm = readIntFromUart1( );
                if ( dc_pwm > 255 )
                {
                    dc_pwm = 255;
                }
                printf( "Duty cycle PWM3: %u/255\n", dc_pwm ); /* Check received data. */

                /* Set DC of PWM3. */
                setAttractiveBlueLedsColor( dc_pwm );
                setAttractiveLedsOn( );

                setLedsStatusColor( LEDS_OFF );
                break;
                /* -------------------------------------------------------------- */

            case 'c':
            case 'C':
                /* Close reward door */
                if ( false == isPowerServoEnable( ) )
                {
                    servomotorPowerEnable( );
                    appDataDoor.reward_door_status = DOOR_CLOSING;
                    if ( 1 == appDataDoor.remain_open )
                    {
                        appDataDoor.remain_open = 0;
                        printf( "Closing door in action. Remain door open set to off\n" );
                    }
                    else
                    {
                        printf( "Closing door in action.\n" );
                    }

                    while ( DOOR_CLOSED != appDataDoor.reward_door_status );
                    printf( "Door closed - Servo position: %u\n", getADC1value( ADC1_CHANNEL_MA_SERVO ) ); 
                    servomotorPowerDisable( );
                }
                else
                {
                    appDataDoor.reward_door_status = DOOR_CLOSING;
                    if ( 1 == appDataDoor.remain_open )
                    {
                        appDataDoor.remain_open = 0;
                        printf( "Closing door in action. Remain door open set to off\n" );
                    }
                    else
                    {
                        printf( "Closing door in action.\n" );
                    }

                    while ( DOOR_OPENED != appDataDoor.reward_door_status );
                    printf( "Door closed - Servo position: %u\n", getADC1value( ADC1_CHANNEL_MA_SERVO ) ); 
                }
                break;
                /* -------------------------------------------------------------- */

            case 'd':
            case 'D':
                /* Display datalogger buffers. */
                printf( "Data logger buffer:\n" );
                if ( appDataLog.nCharBuffer > 0 )
                {
                    printf( "%s", appDataLog.buffer );
                }
                else
                {
                    printf( "<empty buffer>\n" );
                }
                /* Display battery level buffer  */
                printf( "\nBattery level buffer:\n" );
                for ( i = 0; i < 24; i++ )
                {
                    printf( "\t%02d - %04d - %2.3f\n",
                            appDataLog.battery_level[i][0],
                            appDataLog.battery_level[i][1],
                            appDataLog.battery_level[i][1] * BATTERY_VOLTAGE_FACTOR);
                }
                /* Display RFID frequency buffer  */
                printf( "\nRFID frequency buffer:\n" );
                for ( i = 0; i < 24; i++ )
                {
                    for (j = 0 ; j <4 ; j++) 
                    {
                        printf( "\t%02d - %02d - %06ld",
                            appDataLog.rfid_freq[i*4+j][0],
                            appDataLog.rfid_freq[i*4+j][1], 
                            (long)appDataLog.rfid_freq[i*4+j][2]*10);
                    }
                    printf( "\n");
                }
                break;
                /* -------------------------------------------------------------- */

            case 'e':
            case 'E':
                /* Mesuring RDY/CLK period of EM4095 */
                measureRfidFreq( );
                displayRfidFreq( );
                break;
                /* -------------------------------------------------------------- */

            case 'f':
            case 'F':
                /* Display configuration parameters (CONFIG.INI) */
                config_print( );
                if ( false == appData.flags.bit_value.systemInit )
                {
                    printf( "/!\\ SYSTEM NOT CORRECTLY CONFIGURED.\n" );
                }
                break;
                /* -------------------------------------------------------------- */

            case 'g':
            case 'G':
                /* Set green color value of attractive LEDs. */
                setLedsStatusColor( LED_GREEN );
                printf( "Set Green LED, enter a value from 0 to 255:\n" );

                /* Read uint16_t from terminal. */
                dc_pwm = readIntFromUart1( );
                if ( dc_pwm > 255 )
                {
                    dc_pwm = 255;
                }
                printf( "Duty cycle PWM2: %u/255\n", dc_pwm ); /* Check received data. */

                /* Set DC of PWM2. */
                setAttractiveGreenLedsColor( dc_pwm );
                setAttractiveLedsOn( );

                setLedsStatusColor( LEDS_OFF );
                break;
                /* -------------------------------------------------------------- */

            case 'h':
            case 'H':
                if ( 1 == appDataDoor.remain_open )
                {
                    appDataDoor.remain_open = 0;
                    printf( "Remain door open: off\n" );
                }
                else
                {
                    appDataDoor.remain_open = 1;
                    printf( "Remain door open: on\n" );
                }
                break;
                /* -------------------------------------------------------------- */

            case 'i':
            case 'I':
                /* Enable IR */
                IRSensorEnable( );
                setDelayMs( DELAY_MS_BEFORE_IR_ENABLE ); // TODO: adjust delay according to the datasheet
                while ( false == isDelayMsEnding( ) );
                printf( "IR command enable, detecting mode.\n" );
                break;
                /* -------------------------------------------------------------- */

            case 'j':
            case 'J':
                /* Disable IR  */
                IRSensorDisable( );
                printf( "IR command disable, mode low power.\n" );
                break;
                /* -------------------------------------------------------------- */

            case 'k':
            case 'K':
                /* Display IR1 Food status */
                printf( "IR1 reward status: %u\n", BAR_IR1_OUT_GetValue( ) );
                /* Display IR2 Food status */
                printf( "IR2 Food status: %u\n", BAR_IR2_OUT_GetValue( ) );
                break;
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
                /* Toggle PCA9622 device */
                I2C1_MESSAGE_STATUS i2c_status = I2C1_MESSAGE_COMPLETE; // the status of write data on I2C bus
                uint8_t writeBuffer[2]; // data to transmit
                uint8_t user_choice;

                printf( "PCA9622 menu:\n" );
                printf( "1/ initialize output for D16\n" );
                printf( "2/ enable/disable OE pin\n" );
                printf( "3/ toggle LED D16\n" );
                printf( "Choose a command: " );
                while ( false == ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) );
                user_choice = UART1_Read( );

                switch ( user_choice )
                {
                    case '1':
                        /* Initialize PCA9622 device in Normal mode. */
                        writeBuffer[0] = CTRLREG_MODE1;
                        writeBuffer[1] = NORMAL_MODE;
                        i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

                        writeBuffer[0] = 0x97;
                        writeBuffer[1] = 0xAA; // CTRLREG PWM on all output for LEDOUT3
                        i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

                        print_I2C_message_status( i2c_status );
                        printf( "\n" );

                        printf( "PCA9622 Initialized.\n" );
                        break;

                    case '2':
                        /* Toggle Output Enable (OE) pin. */
                        if ( true == PCA9622_OE_GetValue( ) )
                        {
                            //                            PCA9622_OE_SetLow( ); // output enable pin is active LOW
                            setAttractiveLedsOn( );
                            printf( "Output RGB LEDs enable.\n" );
                        }
                        else
                        {
                            //                            PCA9622_OE_SetHigh( );
                            setAttractiveLedsOff( );
                            //i2c_status = I2C1_PCA9622_SoftwareReset( ); /* Disable PCA9622 device */
                            printf( "Output RGB LEDs disable.\n" );
                        }
                        break;

                    case '3':
                    {
                        /* Toggle LED D16 on PCA9622 */
                        static bool led_d16_state = false;
                        //                        I2C1_MESSAGE_STATUS i2c_status = I2C1_MESSAGE_COMPLETE; // the status of write data on I2C bus
                        //                        uint8_t writeBuffer[2]; // data to transmit

                        /* Write I2C demo for  */
                        if ( false == led_d16_state )
                        {
                            /* Set Red LED low on D16 */
                            writeBuffer[0] = CTRLREG_PWM12;
                            writeBuffer[1] = 0x00;
                            i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
                            /* Set Green LED high on D16 */
                            writeBuffer[0] = CTRLREG_PWM13;
                            writeBuffer[1] = 0xFF;
                            i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

                            led_d16_state = true;
                            printf( "LED D16 Green.\n" );
                        }
                        else
                        {
                            /* Set Red LED high on D16 */
                            writeBuffer[0] = CTRLREG_PWM12;
                            writeBuffer[1] = 0xFF;
                            i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
                            /* Set Green LED low on D16 */
                            writeBuffer[0] = CTRLREG_PWM13;
                            writeBuffer[1] = 0x00;
                            i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

                            led_d16_state = false;
                            printf( "LED D16 Red.\n" );
                        }

                        print_I2C_message_status( i2c_status );
                        printf( "\n" );
                        break;
                    }

                    default:
                        putchar( data_from_uart1 ); /* echo RX data if doesn't match */
                        printf( " is a bad command!\n" );
                        break;
                }

                // printf( "i2c_status: %d\n", i2c_status );
                // if ok return 2 = I2C1_MESSAGE_COMPLETE
                // if pb return 5 = I2C1_DATA_NO_ACK
                break;
            }
                /* -------------------------------------------------------------- */

                // FIXME: case 'N' can be released, see case 'M' option '3'
            case 'n':
            case 'N':
            {
                /* Toggle LED D16 on PCA9622 */
                static bool led_d16_state = false;
                I2C1_MESSAGE_STATUS i2c_status = I2C1_MESSAGE_COMPLETE; // the status of write data on I2C bus
                uint8_t writeBuffer[2]; // data to transmit

                /* Write I2C demo */
                if ( false == led_d16_state )
                {
                    /* Set Red LED low on D16 */
                    writeBuffer[0] = CTRLREG_PWM12;
                    writeBuffer[1] = 0x00;
                    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
                    /* Set Green LED high on D16 */
                    writeBuffer[0] = CTRLREG_PWM13;
                    writeBuffer[1] = 0xFF;
                    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

                    led_d16_state = true;
                    printf( "LED D16 Green.\n" );
                }
                else
                {
                    /* Set Red LED high on D16 */
                    writeBuffer[0] = CTRLREG_PWM12;
                    writeBuffer[1] = 0xFF;
                    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
                    /* Set Green LED low on D16 */
                    writeBuffer[0] = CTRLREG_PWM13;
                    writeBuffer[1] = 0x00;
                    i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );

                    led_d16_state = false;
                    printf( "LED D16 Red.\n" );
                }
                break;
            }
                /* -------------------------------------------------------------- */

            case 'o':
            case 'O':
                /* Open reward door */
                if ( false == isPowerServoEnable( ) )
                {
                    servomotorPowerEnable( );
                    appDataDoor.reward_door_status = DOOR_OPENING;
                    printf( "Opening door in action.\n" );
                    while ( DOOR_OPENED != appDataDoor.reward_door_status );                    
                    printf( "Door opened - Servo position: %u\n", getADC1value( ADC1_CHANNEL_MA_SERVO ) );                
                    servomotorPowerDisable( );
                }
                else
                {
                    appDataDoor.reward_door_status = DOOR_OPENING;
                    printf( "Opening door in action.\n" );
                    while ( DOOR_OPENED != appDataDoor.reward_door_status );
                    printf( "Door opened - Servo position: %u\n", getADC1value( ADC1_CHANNEL_MA_SERVO ) ); 
                }
                break;
                /* -------------------------------------------------------------- */

            case 'p':
            case 'P':
                /* Change servomotor position
                 * Servomotor:
                 * HS-322HD: 600 us < servo_position < 2400 us, flexible nylon noise --> Ok
                 * PARRALAX: 600 us < servo_position < 2400 us (Product ID: 900-00005), sound gear 
                 */
                
                /* Get current position. */
                if ( false == isPowerServoEnable( ) )
                {
                    servomotorPowerEnable( );
                    appDataServo.ton_cmd = getDoorPosition();
                    servomotorPowerDisable( );
                }
                else
                {
                   appDataServo.ton_cmd = getDoorPosition(); 
                }
                                
                printf( "Set servomotor position\n\tRange MAX: [%u %u]\n\tRange INI:[%u %u]\n\tCurrent position: %u\n", SERVO_POSITION_MIN_DEFAULT, SERVO_POSITION_MAX_DEFAULT, appDataServo.ton_min, appDataServo.ton_max, appDataServo.ton_cmd );
                
                /* Read uint16_t from terminal. */
                appDataServo.ton_goal = readIntFromUart1( );
                
                if ( appDataServo.ton_goal > SERVO_POSITION_MAX_DEFAULT )
                {
                    printf("No move because goal position (%u) is more than maximal position (%u)\n", appDataServo.ton_goal, SERVO_POSITION_MAX_DEFAULT );
                    break;
                }
                else if ( appDataServo.ton_goal < SERVO_POSITION_MIN_DEFAULT )
                {
                    printf("No move because goal position (%u) is less than minimal position (%u)\n", appDataServo.ton_goal, SERVO_POSITION_MIN_DEFAULT );
                    break;
                }
                else if ( appDataServo.ton_goal >  appDataServo.ton_max )
                {
                    printf("Warning: goal position (%u) is outside range specified in the INI file ([%u %u])\n", appDataServo.ton_goal, appDataServo.ton_min , appDataServo.ton_max );
                }
                else if ( appDataServo.ton_goal <  appDataServo.ton_min )
                {
                    printf("Warning: goal position (%u) is outside range specified in the INI file ([%u %u])\n", appDataServo.ton_goal, appDataServo.ton_min , appDataServo.ton_max );
                }

                if (appDataServo.ton_cmd == appDataServo.ton_goal)
                {
                   printf( "Door already at goal position.\n" ); 
                   break;
                }
                
                if (appDataServo.ton_cmd > appDataServo.ton_goal)
                {
                    appDataServo.direction = -1;
                }
                else
                {
                    appDataServo.direction = 1;
                }
                
                TMR3_Start( );
               
                if ( false == isPowerServoEnable( ) )
                {
                    servomotorPowerEnable( );
                    appDataDoor.reward_door_status = DOOR_MOVING;
//                    printf( "Moving door in action.\n" );
                    while ( DOOR_MOVED != appDataDoor.reward_door_status );
                    servomotorPowerDisable( );
                }
                else
                {
                    appDataDoor.reward_door_status = DOOR_MOVING;
//                    printf( "Moving door in action.\n" );
                    while ( DOOR_MOVED != appDataDoor.reward_door_status );
                }

                break;
                /* -------------------------------------------------------------- */

            case 'q':
            case 'Q':
                
                break;
                /* -------------------------------------------------------------- */

            case 'r':
            case 'R':
                /* Set red color value of attractive LEDs. */
                setLedsStatusColor( LED_RED );
                printf( "Set color of Red LED, enter a value from 0 to 255:\n" );

                /* Read uint16_t from terminal. */
                dc_pwm = readIntFromUart1( );
                if ( dc_pwm > 255 )
                {
                    dc_pwm = 255;
                }
                printf( "Duty cycle PWM1: %u/255\n", dc_pwm ); /* Check received data. */

                /* Set DC of PWM1. */
                setAttractiveRedLedsColor( dc_pwm );
                setAttractiveLedsOn( );

                setLedsStatusColor( LEDS_OFF );

                break;
                /* -------------------------------------------------------------- */

            case 's':
            case 'S':
            {
                /* Set RTCC module date and time value. */
                uint8_t mday, mon, year, hour, min, sec;
                printf( "Set date dd/mm/20yy and time hh:mm:ss\n" );

                printf( "Set day (1 to 31):\n" );
                mday = readIntFromUart1( );
                if ( ( mday > 31 ) || ( mday == 0 ) )
                {
                    mday = 1;
                }

                printf( "Set month (1 to 12):\n" );
                mon = readIntFromUart1( );
                if ( ( mon > 12 ) || ( mon == 0 ) )
                {
                    mon = 1;
                }

                printf( "Set year (0 to 99):\n" );
                year = readIntFromUart1( );
                if ( year > 99 )
                {
                    year = 0;
                }

                printf( "Set hours (0 to 23):\n" );
                hour = readIntFromUart1( );
                if ( hour > 23 )
                {
                    hour = 0;
                }

                printf( "Set minutes (0 to 59):\n" );
                min = readIntFromUart1( );
                if ( min > 59 )
                {
                    min = 0;
                }

                printf( "Set seconds (0 to 59):\n" );
                sec = readIntFromUart1( );
                if ( sec > 59 )
                {
                    sec = 0;
                }

                /* Dynamic configuration date, example 22/08/2016 and time to 15:59:30 */
                // setDateTime( 16, 8, 22, 15, 59, 30 ); /* Set date and time. */
                setDateTime( year, mon, mday, hour, min, sec ); /* Set date and time. */
                break;
            }
                /* -------------------------------------------------------------- */

            case 't':
            case 'T':
                /* Display date and time from RTCC module. */
                printCurrentDate( );
                putchar( '\n' );
                break;
                /* -------------------------------------------------------------- */

            case 'u':
            case 'U':
                /* Display USB device status */
                printf( "USBHostDeviceStatus: " );
                printUSBHostDeviceStatus( );
                putchar( '\n' );
                break;
                /* -------------------------------------------------------------- */

            case 'v':
            case 'V':
                printf( "%d\n", appDataDoor.reward_door_status );
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
                /* -------------------------------------------------------------- */

            case 'w':
            case 'W':
                /* toggle power command "CMD_ACC_PIR". */
                CMD_VDD_ACC_PIR_SERVO_Toggle( );
                Nop( );
                Nop( );
                Nop( );
                if ( true == CMD_VDD_ACC_PIR_SERVO_GetValue( ) )
                {
                    printf( "CMD_ACC_PIR enable.\n" );
                }
                else
                {
                    printf( "CMD_ACC_PIR disable.\n" );
                }
                break;
                /* -------------------------------------------------------------- */
                
            case 'x':
            case 'X':
                /* Display external interrupt and timers states */
                printf("External interrupt states\n");
                printf("\tINT0: %d\n", IEC0bits.INT0IE);
                printf("\tINT1: %d\n", IEC1bits.INT1IE);
                printf("\tINT2: %d\n", IEC1bits.INT2IE);
                printf("\tINT3: %d\n", IEC3bits.INT3IE);
                printf("\tINT4: %d\n", IEC3bits.INT4IE);
                printf("Timers\n");
                printf("\tTimer2: %d\n", T2CONbits.TON);
                printf("\tTimer3: %d\n", T3CONbits.TON);
                printf("\tTimer4: %d\n", T4CONbits.TON);
                break;
                /* -------------------------------------------------------------- */
            
            case 'y':
            case 'Y':
                /* Display all commands values */
                if ( CMD_VDD_APP_V_USB_GetValue( ) == 0 )
                {
                    printf("VDD RFID/USB: off\n");
                }
                else
                {
                    printf("VDD RFID/USB: on\n");
                }
                if ( CMD_VCC_IR_GetValue( ) == 1 )
                {
                    printf("VDD IR: off\n");
                }
                else
                {
                    printf("VDD IR: on\n");
                }
                break;
                /* -------------------------------------------------------------- */
                
            case 'z':
            case 'Z':
                /* Check important parameters */
                printf("Important parameters\n");
                if (true==isPowerBatteryGood( ))
                {
                    printf("\tBattery OK\n");
                }
                else
                {
                    printf("\tBattery PB\n");
                }
                if (true==isPowerVbatGood( ))
                {
                    printf("\tVbat OK\n");
                }
                else
                {
                    printf("\tVbat PB\n");
                }
                if (true==isEnoughFood( ))
                {
                    printf("\tFood OK\n");
                }
                else
                {
                    printf("\tFood PB\n");
                }
                if (true==isRfidFreqGood( ))
                {
                    printf("\tRFID OK\n");
                }
                else
                {
                    printf("\tRFID PB\n");                    
                }                   
                break;
                /* -------------------------------------------------------------- */    
            default:
                putchar( data_from_uart1 ); /* echo RX data if doesn't match */
                break;
        }
    } /* end of if ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) */
}

uint16_t readIntFromUart1( void )
{
    char rx_data_buffer[UART1_BUFFER_SIZE];
    uint8_t numBytes;

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

            /* if received data is not a numerical value then return 0 */
            if ( isdigit( rx_data_buffer[numBytes] ) == false )
            {
                return 0;
            }

            ++numBytes;
        }
    }
    while ( numBytes < UART1_BUFFER_SIZE );

    rx_data_buffer[numBytes + 1] = '\0'; /* add end of string */

    return ( int ) strtol( rx_data_buffer, NULL, 10 );
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
    switch ( USBHostDeviceStatus( appDataUsb.deviceAddress ) )
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
            printf( "Device is holding in an error state!" );
            break;
    }
} /* End of printUSBHostDeviceStatus( ) */

#endif


/*******************************************************************************
 End of File
 */
