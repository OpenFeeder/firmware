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
//const uint8_t REV[] = { "01" }; // TODO: Increment REV number each time the code was modified!

uint8_t getCompletScenarioNumber( void )
{
    if ( GO_NO_GO != appData.scenario_number)
    {
        return appData.scenario_number;
    }
    else
    {
       return (30 + appDataAttractiveLeds.pattern_number); 
    }
}


void getDeviceId( void )
{
    int tblpageReg, addrOffset, readDataL;
//    int readDataH;
    
    // Read 24 bits of data memory from address 0x012340
    // Create 24 bit EA for read by loading TBLPAG
    tblpageReg = TBLPAG;
    TBLPAG = 0x00FF;   // Load TBLPAG register with read address <23:16>
    
    addrOffset = 0x0000;                       // Load offset with read address <15:0>
    // Read data from program memory
    readDataL = __builtin_tblrdl(addrOffset);   // readDataL contains lower word data
//    readDataH = __builtin_tblrdh(addrOffset);   // readDataH contains high byte data

    appData.id.family = readDataL>>8;
    appData.id.device =  readDataL & 0xFF;

    addrOffset = 0x0002;
    readDataL = __builtin_tblrdl(addrOffset);   // readDataL contains lower word data
//    readDataH = __builtin_tblrdh(addrOffset);   // readDataH contains high byte data
    
    appData.id.revision = readDataL;
    
    TBLPAG = tblpageReg;
    
}

void displayDeviceId( void )
{
    printf("*** Individual Device Identifier ***\n");
    printf("* Family ID: %u\n", appData.id.family);
    printf("* Individual ID: %u\n" , appData.id.device);
    printf("* Revision: %u\n", appData.id.revision);
    printf("***************************************************\n"); 
}

void getUniqueDeviceId( void )
{
    int tblpageReg, addrOffset;
    
    uint32_t readDataL, readDataH;
    
    tblpageReg = TBLPAG;
    
    TBLPAG = tblpageReg;
    
    TBLPAG = 0x0080;
        
//    addrOffset = 0x1300; 
    addrOffset = 0x1308; 
    readDataL = __builtin_tblrdl(addrOffset);
    readDataH = __builtin_tblrdh(addrOffset);

    appData.udid.words[0] = (readDataH<<16)+readDataL;

//    addrOffset = 0x1302; 
    addrOffset = 0x130A; 
    readDataL = __builtin_tblrdl(addrOffset);
    readDataH = __builtin_tblrdh(addrOffset);
    
    appData.udid.words[1] = (readDataH<<16)+readDataL;

//    addrOffset = 0x1304; 
    addrOffset = 0x130C; 
    readDataL = __builtin_tblrdl(addrOffset);
    readDataH = __builtin_tblrdh(addrOffset);
    
    appData.udid.words[2] = (readDataH<<16)+readDataL;

//    addrOffset = 0x1306; 
    addrOffset = 0x130E; 
    readDataL = __builtin_tblrdl(addrOffset);
    readDataH = __builtin_tblrdh(addrOffset);
    
    appData.udid.words[3] = (readDataH<<16)+readDataL;
    
//    printf(" %u %u\n", readDataL, readDataH);
    
//    addrOffset = 0x1308; 
    addrOffset = 0x1310; 
    readDataL = __builtin_tblrdl(addrOffset);
    readDataH = __builtin_tblrdh(addrOffset);
    
    appData.udid.words[4] = (readDataH<<16)+readDataL;
    
    TBLPAG = tblpageReg;
}

void displayUniqueDeviceId( void )
{
    
    printf("UDID: %06lX %06lX %06lX %06lX %06lX\n", appData.udid.words[0],
                                        appData.udid.words[1],
                                        appData.udid.words[2],
                                        appData.udid.words[3],
                                        appData.udid.words[4]);
    
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
    printf( "Firmware: %s v%d.%d.%d\n", FW_NAME, FW_VERSION_MAJOR, FW_VERSION_MINOR, FW_VERSION_PATCH);
}

void displayBootMessage( void )
{
    printf( "\n\n================ OpenFeeder ================\n\t" );
//    printf( "      Firmware: %s v%d.%d.%d\n", FW_NAME, FW_VERSION_MAJOR, FW_VERSION_MINOR, FW_VERSION_PATCH);
//    printf( "      Built on %s, %s\n", BUILD_DATE, BUILD_TIME );
    displayFirmwareVersion();
    printf( "\t" );
    displayBuildDateTime();
    printf( "\tFor board v3.0\n");
    printf( "============================================\n\t" );
    displayUniqueDeviceId();
    printf( "============================================\n" );
    printf( "\tWeb page: https://github.com/OpenFeeder\n" );
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

void APP_SerialDebugTasks( void )
{
    uint16_t dc_pwm;
    int i;
    bool flag;

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
                printf( " i or I: toggle IR barriers power\n" );
                printf( " j or J: NOT AFFECTED\n" );
                printf( " k or K: display IR1 and IR2 state\n" );
                printf( " l or L: call i2cScanner( ) to found I2C devices attached.\n" ); //  --> 0x02, 0x06 and 0xE0
                printf( " m or M: menu to control PCA9622 device\n" );
                printf( "\t> 1: initialize the PCA9622 device\n" );
                printf( "\t> 2: Toggle Output Enable (OE) pin\n" );
                printf( "\t> 3: Toggle LED D16 color green or red\n" );
                printf( " n or N: Test display battery level\n"); // NOT AFFECTED
                printf( " o or O: open door\n" );
                printf( " p or P: change servomotor position\n" );
                printf( " q or Q: check status LEDs\n" );
                printf( " r or R: set red color value of RGB attractive LEDs\n" );
                printf( " s or S: set RTCC module date and time value\n" );
                printf( " t or T: display date and time from RTCC module\n" );
                printf( " u or U: reconfigure the system (USB key read)\n" );
                printf( " v or V: set status of servomotor power command\n" );
                printf( " w or W: toggle power command (CMD_ACC_PIR)\n" );
                printf( " x or X: display external interrupt and timers states\n" );
                printf( " y or Y: display all commands values\n" );
                printf( " z or Z: check important parameters\n" );
                break;

            case '!':
                displayBuildDateTime( );
                break;

            case 'a':
            case 'A':
            {
                /* Analogic measures */
                uint16_t analog_measure;

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
                printf( "Servo position: %u\n", analog_measure );

                /* Battery level */
                appData.battery_level = getADC1value( ADC1_CHANNEL_MA_12V );
                printBatteryLevel( );

                /* VBat level */
                appData.vbat_level = getADC1value( ADC1_CHANNEL_VBAT_2 );
                printVBatLevel( );

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
                
                if ( appDataLog.numBatteryLevelStored == 0)
                {
                    printf( "<empty buffer>\n" );
                }
                else
                {     
                    for ( i = 0; i < appDataLog.numBatteryLevelStored; i++ )
                    {
                        printf( "\t%02d - %04d - %2.3f\n",
                            appDataLog.battery_level[i][0],
                            appDataLog.battery_level[i][1],
                            appDataLog.battery_level[i][1] * BATTERY_VOLTAGE_FACTOR );
                    }
                }

                /* Display RFID frequency buffer  */
                printf( "\nRFID frequency buffer:\n" );
                
                if ( appDataLog.numRfidFreqStored == 0)
                {
                    printf( "<empty buffer>\n" );
                }
                else
                {
                     for ( i = 0; i < appDataLog.numRfidFreqStored; i++ )
                    {
                         
                        printf( "\t%02d - %02d - %06ld\n",
                                    appDataLog.rfid_freq[i][0],
                                    appDataLog.rfid_freq[i][1],
                                    ( long ) appDataLog.rfid_freq[i][2]*10 );
                    }               
                }

                break;
                /* -------------------------------------------------------------- */

            case 'e':
            case 'E':
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
                /* -------------------------------------------------------------- */

            case 'f':
            case 'F':
                /* Display configuration parameters (CONFIG.INI) */

                if ( false == appData.flags.bit_value.systemInit )
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

                /* Toggle IR barriers power */
                if ( CMD_VCC_IR_GetValue( ) == 1 )
                {
                    /* Enable IR */
                    IRSensorEnable( );
                    setDelayMs( DELAY_MS_BEFORE_IR_ENABLE ); // TODO: adjust delay according to the datasheet
                    while ( false == isDelayMsEnding( ) );
                    printf( "IR command enable (detecting mode).\n" );
                }
                else
                {
                    /* Disable IR  */
                    IRSensorDisable( );
                    printf( "IR command disable (low power mode).\n" );
                }
                break;
                /* -------------------------------------------------------------- */

            case 'j':
            case 'J':
   
                display_events();
//                printf( "\t<NOT AFFECTED>\n" );
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

                printf( "Door opened\n");

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
                appDataServo.ton_goal = readIntFromUart1( );

                if ( 0 == appDataServo.ton_goal )
                {
                    printf( "\tWrong value\n" );
                    break;
                }

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
  
                printf( "Door moved\n");
                
                break;
                /* -------------------------------------------------------------- */

            case 'q':
            case 'Q':
                /* Check status LEDs */
                checkLedsStatus( );

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
                if ( APP_I2CRTC_DateTime_set( year, mon, mday, hour, min, sec ) )
                {
                    printf( "\nEXT RTC done.\n" );
                }
                else
                {
                    printf( "\nEXT RTC not found!\n" );
                }
                printf( "\nTime set.\n" );
                break;
            }
            /* -------------------------------------------------------------- */
            
            case 'S':
            {
                /* Set RTCC module date and time value. */
                char date[6];
                uint8_t numBytes;
                uint16_t delay = 1000;
                bool flag = false;
                
                setDelayMs(delay);
                  
                numBytes = 0; /* initialized numBytes */
                
                while (false == isDelayMsEnding())
                {
                    while ( numBytes < 6 )
                    {
                        if ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) )
                        {
                            date[numBytes] = UART1_Read( );

                            ++numBytes;
                        }
                    }
                }
                
                delay /= 1000;
                    
                if ( date[5] > (60 - delay - 1) )
                {
                    date[5] += delay - 60;
                    date[4] += 1;
                    if ( date[4] > 60)
                    {
                       date[4] = 0;
                       date[3] += 1;
                    }
                }
                else
                {
                    date[5] += delay;
                }
    
                setDelayMs(delay);
                
                while ( false == isDelayMsEnding() )
                {
                    if ( false == flag )
                    {
                        flag = setDateTime( date[0], date[1], date[2], date[3], date[4], date[5] ); /* Set date and time. */ 
                    }
                    
                }
                
                if ( date[5] > (60 - delay - 1) )
                {
                    date[5] += delay - 60;
                    date[4] += 1;
                    if ( date[4] > 60)
                    {
                       date[4] = 0;
                       date[3] += 1;
                    }
                }
                else
                {
                    date[5] += delay;
                }
                
                APP_I2CRTC_DateTime_set( date[0], date[1], date[2], date[3], date[4], date[5] );

                break;
            }
                /* -------------------------------------------------------------- */

            case 't':
                /* Display date and time from RTCC module. */
                getCurrentDate( );
                APP_I2CRTC_DateTime_get( );
                
                printf( "PIC: " );
                printCurrentDate( );
                putchar( '\n' );
                printf( "EXT: ");                 
                APP_I2CRTC_DateTime_print( );
                putchar( '\n' );
                break;
                /* -------------------------------------------------------------- */
                
            case 'T':
            {
                uint8_t date[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                
                struct tm date_time;
                
                while ( !RTCC_TimeGet( &date_time ) )
                {
                    Nop( );
                }
                
                date[0] = (uint8_t)date_time.tm_year;
                date[1] = (uint8_t)date_time.tm_mon;
                date[2] = (uint8_t)date_time.tm_mday;
                date[3] = (uint8_t)date_time.tm_hour;
                date[4] = (uint8_t)date_time.tm_min;
                date[5] = (uint8_t)date_time.tm_sec;

                if ( I2C1_MESSAGE_COMPLETE == I2C1_MasterReadDS3231_get( &appData.i2c_current_time ))
                {
                    date[6] = (uint8_t)appData.i2c_current_time.year_s;
                    date[7] = (uint8_t)appData.i2c_current_time.mon;
                    date[8] = (uint8_t)appData.i2c_current_time.mday;
                    date[9] = (uint8_t)appData.i2c_current_time.hour;
                    date[10] = (uint8_t)appData.i2c_current_time.min;
                    date[11] = (uint8_t)appData.i2c_current_time.sec;
                }

                UART1_WriteBuffer(date, 12);

                break;
            }
                /* -------------------------------------------------------------- */

            case 'u':
            case 'U':
                /* Reconfigure the system by reading the USB key again */
                appData.need_to_reconfigure = true;
                break;
                /* -------------------------------------------------------------- */

            case 'v':
            case 'V':
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
                    appData.pir_sensor_powered = true;
                }
                else
                {
                    printf( "CMD_ACC_PIR disable.\n" );
                    appData.pir_sensor_powered = false;
                }
                break;
                /* -------------------------------------------------------------- */

            case 'x':
            case 'X':
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
                /* -------------------------------------------------------------- */

            case 'y':
            case 'Y':
 
                displayFirmwareVersion();
                displayBuildDateTime();
                putchar( '\n' );
                displayUniqueDeviceId();
                
                /* Display external interrupt and timers states */
                printf( "\nExternal interrupt states\n" );
                printf( "\tINT0: %d\n", IEC0bits.INT0IE );
                printf( "\tINT1: %d\n", IEC1bits.INT1IE );
                printf( "\tINT2: %d\n", IEC1bits.INT2IE );
                printf( "\tINT3: %d\n", IEC3bits.INT3IE );
                printf( "\tINT4: %d\n", IEC3bits.INT4IE );

                printf( "\nTimers\n" );
                printf( "\tTimer2: %d\n", T2CONbits.TON );
                printf( "\tTimer3: %d\n", T3CONbits.TON );
                printf( "\tTimer4: %d\n", T4CONbits.TON );

                /* Display all commands values */
                printf( "\nPower commands\n" );
                if ( CMD_VDD_ACC_PIR_SERVO_GetValue( ) == 1 )
                {
                    printf( "\tCMD_VDD_ACC_PIR: on\n" );
                }
                else
                {
                    printf( "\tCMD_VDD_ACC_PIR: off\n" );
                }

                if ( _TRISF1 )
                {
                    printf( "\tCMD_VCC_SERVO: off\n" );
                }
                else
                {
                    printf( "\tCMD_VCC_SERVO: on\n" );
                }
                printf( "\t******\n" );
                if ( CMD_VDD_APP_V_USB_GetValue( ) == 0 )
                {
                    printf( "\tCMD_VDD_APP: off\n" );
                }
                else
                {
                    printf( "\tCMD_VDD_APP: on\n" );
                }

                if ( CMD_VDD_USB_GetValue( ) == 1 )
                {
                    printf( "\tCMD_VDD_USB: off\n" );
                }
                else
                {
                    printf( "\tCMD_VDD_USB: on\n" );
                }
                printf( "\t******\n" );
                if ( CMD_VCC_IR_GetValue( ) == 1 )
                {
                    printf( "\tCMD_VCC_IR: off\n" );
                }
                else
                {
                    printf( "\tCMD_VCC_IR: on\n" );
                }
                printf( "\t******\n" );
                if ( CMD_3V3_RF_GetValue( ) == 1 )
                {
                    printf( "\tCMD_3V3_RF: on\n" );
                }
                else
                {
                    printf( "\tCMD_3V3_RF: off\n" );
                }

                printf( "\nOutput compare\n" );
                printf( "\tOC4: %d\n", OC4CON1bits.OCM );
                printf( "\tOC4R: %d\n", OC4R );
                printf( "\tOC4RS: %d\n", OC4RS );
                printf( "\t******\n" );
                printf( "\tOC5: %d\n", OC5CON1bits.OCM );
                printf( "\tOC5R: %d\n", OC5R );
                printf( "\tOC5RS: %d\n", OC5RS );
                
                /* Display USB device status */
                printf( "\nUSB device status: " );
                printUSBHostDeviceStatus( );
                putchar( '\n' );

                break;
                /* -------------------------------------------------------------- */

            case 'z':
            case 'Z':
                /* Check important parameters */
                printf( "Important parameters\n" );
                if ( true == isPowerBatteryGood( ) )
                {
                    printf( "\tBattery OK\n" );
                }
                else
                {
                    printf( "\tBattery PB\n" );
                }
                if ( true == isPowerVbatGood( ) )
                {
                    printf( "\tVbat OK\n" );
                }
                else
                {
                    printf( "\tVbat PB\n" );
                }
                if ( true == isEnoughFood( ) )
                {
                    printf( "\tFood OK\n" );
                }
                else
                {
                    printf( "\tFood PB\n" );
                }
                if ( true == isRfidFreqGood( ) )
                {
                    printf( "\tRFID OK\n" );
                }
                else
                {
                    printf( "\tRFID PB\n" );
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
