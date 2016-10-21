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


void getBatteryLevel(void)
{

    uint16_t i;

    ADC1_ChannelSelect(ADC1_CHANNEL_AN0_MA_12V);

    ADC1_Start();
    for (i = 0; i < 1000; i++); /* Provide Delay */
    ADC1_Stop();

    while (!ADC1_IsConversionComplete())
    {
        ADC1_Tasks();
    }

    appData.battery_level = ADC1_ConversionResultGet();

}


bool isPowerBatteryGood(void)
{

    getBatteryLevel();

    if (appData.battery_level <= LOW_BATTERY_LEVEL)
    {
#if defined (USE_UART1_SERIAL_INTERFACE) 
        printf("battery_measuring_analog = %u\n", appData.battery_level);
        printf("battery voltage = %2.3f\n", appData.battery_level * 0.003925439453125);
#endif 
        return false; /* Battery protected */
    }

    return true;

}


/*******************************************************************************
 End of File
 */
