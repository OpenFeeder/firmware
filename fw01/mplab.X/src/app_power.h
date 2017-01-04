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

/* Mesure
 * from 0 to 4095 (ADC 12 bits --> 4096)
 * si le pont de résistance fait une echelle de correspondance:
 * 16.0786 <--> 3.3
 * 16.0786/4096= 0.003925439453125
 * exemple: 12,11 V --> 3085
 * /!\ 11.5 V --> 2929 => mode SLEEP
 */

#define HIGH_BATTERY_LEVEL 3145
#define LOW_BATTERY_LEVEL 2929
#define BATTERY_VOLTAGE_FACTOR 0.003925439453125

//#define LOW_VBAT_LEVEL 2929
#define VBAT_VOLTAGE_FACTOR 0.000795827117474

void powerUsbRfidEnable( void );
void powerUsbRfidDisable( void );
void getVBatLevel( void );
void printVBatLevel( void );
void getBatteryLevel( void );
void printBatteryLevel( void );
FILEIO_RESULT logBatteryLevel( void );

#endif	/* APP_POWER_H */


/*******************************************************************************
 End of File
 */
