/**
 * @file app_power.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 * @revision history 1
 * @dependency 
 */

#include "app.h"
#include "app_power.h"


bool isPowerBatteryGood( void )
{
    uint16_t battery_measuring_analog;
    uint16_t i;
    
    ADC1_ChannelSelect( ADC1_CHANNEL_AN0_MA_12V );
    ADC1_Start( );
    for ( i = 0; i < 1000; i++ ); /* Provide Delay */
    ADC1_Stop( );
    while ( !ADC1_IsConversionComplete( ) )
    {
//        putchar( '.' ); // FIXME: Add for debug... ==> do only on time
        ADC1_Tasks( );
    }
    battery_measuring_analog = ADC1_ConversionResultGet( );
    /* Mesure
     * from 0 to 4095 (ADC 12 bits --> 4096)
     * si le pont de résistance fait une echelle de correspondance:
     * 16.0786 <--> 3.3
     * 16.0786/4096= 0.003925439453125
     * exemple: 12,11 V --> 3085
     * /!\ 11.5 V --> 2929 => mode SLEEP
     */

    if ( battery_measuring_analog > 2929 )
    {
        return true;
    }
    else
    {
#if defined (USE_UART1_SERIAL_INTERFACE) 
        printf( "battery_measuring_analog = %u\n", battery_measuring_analog );
        printf( "battery voltage = %2.3f\n", battery_measuring_analog * 0.003925439453125 );
#endif 
        return false; /* Battery protected */
    }
}


/*******************************************************************************
 End of File
 */
