/**
 * @file app_power.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 * @revision history 1
 * @dependency 
 */

#ifndef APP_POWER_H
#define	APP_POWER_H

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/adc1.h"
#include <stdint.h>
#include <stdbool.h>

/* Mesure
     * from 0 to 4095 (ADC 12 bits --> 4096)
     * si le pont de résistance fait une echelle de correspondance:
     * 16.0786 <--> 3.3
     * 16.0786/4096= 0.003925439453125
     * exemple: 12,11 V --> 3085
     * /!\ 11.5 V --> 2929 => mode SLEEP
     */


#define LOW_BATTERY_LEVEL 2929

void getBatteryLevel(void);
bool isPowerBatteryGood( void );
FILEIO_RESULT logBatteryLevel(void);

#endif	/* APP_POWER_H */


/*******************************************************************************
 End of File
 */
