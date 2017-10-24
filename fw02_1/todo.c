
Smart Dog Watcher
https://create.arduino.cc/projecthub/nikhilr/smart-dog-watcher-73cb9c


#include <ctype.h>
#include <stdio.h>
#include "app.h"
#include "app_debug.h"
                     15/10/2017 15:52:23
#if defined (USE_UART1_SERIAL_INTERFACE)

/* Current date to a C string (page 244)) */
// Project code name = OpenFeeder
const uint8_t HW_VERSION[] = "v2.0"; // Board version
const uint8_t FW_VERSION[] = "fw02"; // Firmware version majeur
const uint8_t FW_REVISION[] = "r01"; // Revision of firmware version
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
    printf( "      Board: %s - Firmware: %s\n", HW_VERSION, FW_VERSION );
    printf( "      Code version: %s\n", FW_REVISION );
    printf( "      Build on %s, %s\n", BUILD_DATE, BUILD_TIME );
    printf( "============================================\n" );
    printf( "   Web page: https://github.com/OpenFeeder\n" );
    printf( "   Mail: contact.openfeeder@gmail.com\n" );
    printf( "============================================\n" );
    printf( "Type [?] key to display the Key mapping interface.\n\n" );
}

                printf( " 0: change power state of radio module\n" );
                printf( " n or N: NOT AFFECTED\n" );
                printf( " q or Q: NOT AFFECTED\n" );
                printf( " q or Q: display battery level buffer\n" );


                printf( " x or X: display RFID frequency buffer\n" );
                printf( " y or Y: display all commands values\n");
                printf( " z or Z: check important parameters\n" );

            case '0':
                if ( CMD_3V3_RF_GetValue() )
                {
                    CMD_3v3_RF_SetLow( );
                    printf( "RF Module enable\n" );
                }
                else
                {
                    CMD_3v3_RF_SetHigh( );
                    printf( "RF Module disable\n" );
                }
                /* Display date and time from RTCC module. */
                //                printCurrentDate( );
                //                printf( "CMD_3v3_RF: %u\n", CMD_3v3_RF_GetValue( ) );
                //                putchar( '\n' );
                break;
                /* -------------------------------------------------------------- */


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

                /* Light level */
                appData.light_level = getADC1value( ADC1_CHANNEL_MA_LIGHT );
                printf( "Light level: (%u)\n", appData.light_level );

                /* CTMU */
                analog_measure = getADC1value( ADC1_CHANNEL_CTMU_TEMPERATURE_SENSOR_INPUT );
                printf( "CTMU temperature sensor: (%u)\n", analog_measure );

                break;
            }
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

