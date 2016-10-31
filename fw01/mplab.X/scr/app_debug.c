/**
 * @file app_debug.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 06/07/2016
 * @revision history 2
 */

#include "app.h"
#include "app_debug.h"

#if defined (USE_UART1_SERIAL_INTERFACE)

/* Interface firmware terminal (Debug) - Idem in main.c */
// a or A: analogic measure of the current postion of the servomotor and the battery voltage
// b or B: set blue color value of attractive LEDs
// c or C: close reward door
// d or D: display current state of the datalogger buffer
// e or E: mesuring RDY/CLK period of EM4095
// f or F: display configuration parameters (CONFIG.INI) 
// g or G: set green color value of attractive LEDs
// i or I: enable IR 
// j or J: disable IR 
// o or O: open reward door
// p or P: change servomotor position
// r or R: set red color value of attractive LEDs
// s or S: set RTCC module date and time value
// t or T: display date and time from RTCC module
// u or U: display USB device status 
// v or V: set status of servomotor power command


void APP_SerialDebugTasks(void)
{
    uint16_t dc_pwm;

    if (UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet())
    {
        /* If there is at least one byte of data has been received. */
        uint8_t data_from_uart1 = UART1_Read();

        switch (data_from_uart1)
        {
            case 'a':
            case 'A':
            {
                /* analogic measure of the current postion of the servomotor. */
                uint16_t servomotor_position_analog;
                uint16_t battery_measuring_analog;
                uint16_t i;

                ADC1_ChannelSelect(ADC1_CHANNEL_AN1_MA_SERVO);
                ADC1_Start();
                for (i = 0; i < 1000; i++); /* Provide Delay */
                ADC1_Stop();
                while (!ADC1_IsConversionComplete())
                {
                    putchar('.'); // FIXME: Add for debug... ==> do only on time
                    ADC1_Tasks();
                }
                servomotor_position_analog = ADC1_ConversionResultGet();
                /* Mesure with jumper closed
                 * from 3904 to 4078 --> ~4060
                 */
                printf("servo_position_analog = %u\n", servomotor_position_analog);


                ADC1_ChannelSelect(ADC1_CHANNEL_AN0_MA_12V);
                ADC1_Start();
                for (i = 0; i < 1000; i++); /* Provide Delay */
                ADC1_Stop();
                while (!ADC1_IsConversionComplete())
                {
                    putchar('.'); // FIXME: Add for debug... ==> do only on time
                    ADC1_Tasks();
                }
                battery_measuring_analog = ADC1_ConversionResultGet();
                /* Mesure
                 * from 0 to 4095 (ADC 12 bits --> 4096)
                 * si le pont de résistance fait une echelle de correspondance:
                 * 16.0786 <--> 3.3
                 * 16.0786/4096= 0.003925439453125
                 */
                printf("battery_measuring_analog = %u\n", battery_measuring_analog);
                printf("battery voltage = %2.3f\n", battery_measuring_analog * 0.003925439453125);
                break;
            }
                /* -------------------------------------------------------------- */

            case 'b':
            case 'B':
                /* Set blue color value of attractive LEDs. */
                setLedsStatusColor(LED_BLUE);
                printf("Set Blue LED, enter a value from 0 to 255:\n");

                /* Read uint16_t from terminal. */
                dc_pwm = readIntFromUart1();
                if (dc_pwm > 255)
                {
                    dc_pwm = 255;
                }
                printf("Duty cycle PWM3: %u/255\n", dc_pwm); /* Check received data. */

                /* Set DC of PWM3. */
                setAttractiveBlueLedsColor(dc_pwm);

                setLedsStatusColor(LEDS_OFF);
                break;
                /* -------------------------------------------------------------- */

            case 'c':
            case 'C':
                /* Close reward door */
                appData.reward_door_status = DOOR_CLOSING;
                printf("Closing reward door in action.\n");
                break;
                /* -------------------------------------------------------------- */

            case 'd':
            case 'D':
                /* Display current state of the datalogger buffer. */
                printf("Data logger buffer:\n");
                if (appDataLog.nCharBuffer > 0)
                {
                    printf("%s", appDataLog.buffer);
                }
                else
                {
                    printf("<empty buffer>\n");
                }
                break;
                /* -------------------------------------------------------------- */

            case 'e':
            case 'E':
                /* Mesuring RDY/CLK period of EM4095 */
                printf("Mesuring RDY/CLK period of EM4095.\n");
                // TODO: appData.state = APP_STATE_RFID_MESURING_RDYCLK;
                break;
                /* -------------------------------------------------------------- */

            case 'f':
            case 'F':
                /* Display configuration parameters (CONFIG.INI) */
                if (true == appData.flags.bit_value.systemInit)
                {
                    config_print();
                }
                else
                {
                    printf("System not configured.\n");

                }
                break;
                /* -------------------------------------------------------------- */

            case 'g':
            case 'G':
                /* Set green color value of attractive LEDs. */
                setLedsStatusColor(LED_GREEN);
                printf("Set Green LED, enter a value from 0 to 255:\n");

                /* Read uint16_t from terminal. */
                dc_pwm = readIntFromUart1();
                if (dc_pwm > 255)
                {
                    dc_pwm = 255;
                }
                printf("Duty cycle PWM2: %u/255\n", dc_pwm); /* Check received data. */

                /* Set DC of PWM2. */
                setAttractiveGreenLedsColor(dc_pwm);

                setLedsStatusColor(LEDS_OFF);
                break;
                /* -------------------------------------------------------------- */

            case 'i':
            case 'I':
                /* Enable IR */
                IRSensorEnable();
                //                CMD_VCC_IR_Toggle( ); /* Change IR command setting. */
                //                Nop();
                //                Nop();
                //                Nop();
                //                if ( CMD_VCC_IR_GetValue( ) )
                //                {
                //                    printf( "IR command disable, mode low power.\n" );
                //                }
                //                else
                //                {
                printf("IR command enable, detecting mode.\n");
                //                }
                break;
                /* -------------------------------------------------------------- */

            case 'j':
            case 'J':
                /* Disable IR  */
                IRSensorDisable();
                printf("IR command disable, mode low power.\n");
                break;
                /* -------------------------------------------------------------- */

            case 'm':
            case 'M':
                /* Measuring Read/Clock (EM4095) */
                CMD_VDD_APP_V_USB_SetHigh(); /* Power up VDD APP for USB and RFID. */

                RFID_Enable();
                setDelayMsEM4095(60);
                while (isDelayMsEndingEM4095() == false)
                {
                    Nop();
                }
                
                while (EM4095_SHD_GetValue() == false)
                {
//                    printf("EM4095_SHD_GetValue()\n");
                    Nop();
                }
                EX_INT3_InterruptEnable();
                while (g_new_value_of_em4095_rdyclk_measurement == false)
                {
//                    printf("g_new_value_of_em4095_rdyclk_measurement\n");
                    Nop();
                }
                EX_INT3_InterruptDisable();

#if defined (USE_UART1_SERIAL_INTERFACE)
                printf("RDY/CLK signal frequency: %u\n", rdyclk_count_in_10ms * 5);
#endif    
                g_new_value_of_em4095_rdyclk_measurement = false;
                RFID_Disable();
                CMD_VDD_APP_V_USB_SetLow(); /* Shut down VDD APP for USB and RFID. */

                break;
                /* -------------------------------------------------------------- */

            case 'o':
            case 'O':
                /* Open reward door */
                appData.reward_door_status = DOOR_OPENING;
                printf("Opening reward door in action.\n");
                break;
                /* -------------------------------------------------------------- */

            case 'p':
            case 'P':
                /* Change servomotor position
                 * Servomotor:
                 * Absolute range: 500 us < servo_position < 2500 us
                 * HS-322HD: 600 us < servo_position < 2400 us, flexible nylon noise --> Ok
                 * PARRALAX: 600 us < servo_position < 2400 us (Product ID: 900-00005), sound gear 
                 */
                // printf( "Set servomotor position, enter a value from 500 to 2500:\n" );
                printf("Set servomotor position, enter a value from %u to %u:\n", appDataServo.ton_min, appDataServo.ton_max);

                /* Read uint16_t from terminal. */
                appDataServo.ton_cmd = readIntFromUart1();
                //                appData.servomotor.ton_cmd = readIntFromUart1( );

                if (appDataServo.ton_cmd > appDataServo.ton_max)
                {
                    appDataServo.ton_cmd = appDataServo.ton_max;
                }
                else if (appDataServo.ton_cmd < appDataServo.ton_min)
                {
                    appDataServo.ton_cmd = appDataServo.ton_min;
                }

                TMR3_Start(); // FIXME: pour l'instant je n'ai pas le signal PWM sur la broche !
                /* Set DC of PWM5. */
                OC5_SingleCompareValueSet(appDataServo.ton_cmd);
                printf("servo_position = %u\n", appDataServo.ton_cmd);
                break;
                /* -------------------------------------------------------------- */

            case 'r':
            case 'R':
                /* Set red color value of attractive LEDs. */
                setLedsStatusColor(LED_RED);
                printf("Set color of Red LED, enter a value from 0 to 255:\n");

                /* Read uint16_t from terminal. */
                dc_pwm = readIntFromUart1();
                if (dc_pwm > 255)
                {
                    dc_pwm = 255;
                }
                printf("Duty cycle PWM1: %u/255\n", dc_pwm); /* Check received data. */

                /* Set DC of PWM1. */
                setAttractiveRedLedsColor(dc_pwm);

                setLedsStatusColor(LEDS_OFF);
                break;
                /* -------------------------------------------------------------- */

            case 's':
            case 'S':
            {
                /* Set RTCC module date and time value. */
                uint8_t mday, mon, year, hour, min, sec;
                printf("Set date dd/mm/20yy and time hh:mm:ss\n");

                printf("Set date dd, enter a value from 1 to 31:\n");
                mday = readIntFromUart1();
                if ((mday > 31) || (mday == 0))
                {
                    mday = 1;
                }

                printf("Set date mm, enter a value from 1 to 12:\n");
                mon = readIntFromUart1();
                if ((mon > 12) || (mon == 0))
                {
                    mon = 1;
                }

                printf("Set date yy, enter a value from 0 to 99:\n");
                year = readIntFromUart1();
                if (year > 99)
                {
                    year = 0;
                }

                printf("Set time hh, enter a value from 0 to 23:\n");
                hour = readIntFromUart1();
                if (hour > 23)
                {
                    hour = 0;
                }

                printf("Set time mm, enter a value from 0 to 59:\n");
                min = readIntFromUart1();
                if (min > 59)
                {
                    min = 0;
                }

                printf("Set time ss, enter a value from 0 to 59:\n");
                sec = readIntFromUart1();
                if (sec > 59)
                {
                    sec = 0;
                }

                /* Dynamic configuration date, example 22/08/2016 and time to 15:59:30 */
                // setDateTime( 16, 8, 22, 15, 59, 30 ); /* Set date and time. */
                setDateTime(year, mon, mday, hour, min, sec); /* Set date and time. */
                break;
            }
                /* -------------------------------------------------------------- */

            case 't':
            case 'T':
            {
                /* Display date and time from RTCC module. */
                printCurrentDate();
                putchar('\n');
                break;
            }
                /* -------------------------------------------------------------- */

            case 'u':
            case 'U':
                /* Display USB device status */
                printf("USBHostDeviceStatus: ");
                printUSBHostDeviceStatus();
                putchar('\n');
                break;
                /* -------------------------------------------------------------- */

            case 'v':
            case 'V':
                /* Set status of servomotor power command. */
                CMD_VCC_SERVO_Toggle(); /* Change servomotor command setting. */
                Nop();
                Nop();
                Nop();
                if (CMD_VCC_SERVO_GetValue())
                {
                    printf("Servomotor power command enable.\n");
                }
                else
                {
                    printf("Servomotor power command disable.\n");
                }
                break;
                /* -------------------------------------------------------------- */

            default:
                putchar(data_from_uart1); /* echo RX data if doesn't match */
                break;
        }
    } /* end of if ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) */
}


uint16_t readIntFromUart1(void)
{
    char rx_data_buffer[UART1_BUFFER_SIZE];
    uint8_t numBytes;

    numBytes = 0; /* initialized numBytes */
    do
    {
        if (UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet())
        {
            rx_data_buffer[numBytes] = UART1_Read();

            /* if received <CR> the stop received */
            if ('\r' == rx_data_buffer[numBytes])
            {
                break;
            }

            /* if received data is not a numerical value then return 0 */
            if (isdigit(rx_data_buffer[numBytes]) == false)
            {
                return 0;
            }

            ++numBytes;
        }
    }
    while (numBytes < UART1_BUFFER_SIZE);

    rx_data_buffer[numBytes + 1] = '\0'; /* add end of string */

    return (int) strtol(rx_data_buffer, NULL, 10);
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
void printUSBHostDeviceStatus(void)
{
    switch (USBHostDeviceStatus(appDataUsb.deviceAddress))
    {
        case USB_DEVICE_ATTACHED:
            printf("USB_DEVICE_ATTACHED");
            break;

        case USB_DEVICE_DETACHED:
            printf("USB_DEVICE_DETACHED");
            break;

        case USB_DEVICE_ENUMERATING:
            printf("USB_DEVICE_ENUMERATING");
            break;

        case USB_HOLDING_OUT_OF_MEMORY:
            printf("USB_HOLDING_OUT_OF_MEMORY");
            break;

        case USB_HOLDING_UNSUPPORTED_DEVICE:
            printf("USB_HOLDING_UNSUPPORTED_DEVICE");
            break;

        case USB_HOLDING_UNSUPPORTED_HUB:
            printf("USB_HOLDING_UNSUPPORTED_HUB");
            break;

        case USB_HOLDING_INVALID_CONFIGURATION:
            printf("USB_HOLDING_INVALID_CONFIGURATION");
            break;

        case USB_HOLDING_PROCESSING_CAPACITY:
            printf("USB_HOLDING_PROCESSING_CAPACITY");
            break;

        case USB_HOLDING_POWER_REQUIREMENT:
            printf("USB_HOLDING_POWER_REQUIREMENT");
            break;

        case USB_HOLDING_CLIENT_INIT_ERROR:
            printf("USB_HOLDING_CLIENT_INIT_ERROR");
            break;

        case USB_DEVICE_SUSPENDED:
            printf("USB_DEVICE_SUSPENDED");
            break;

        default:
            /* If nothing else matches, do the default. */
            printf("Device is holding in an error state!");
            break;
    }
} /* End of printUSBHostDeviceStatus( ) */


#endif


/*******************************************************************************
 End of File
 */
