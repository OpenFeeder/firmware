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

//#include <xc.h> /* include processor files - each processor file is guarded. */
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/adc1.h"
#include <stdint.h>
#include <stdbool.h>


bool isPowerBatteryGood( void );

#endif	/* APP_POWER_H */


/*******************************************************************************
 End of File
 */
