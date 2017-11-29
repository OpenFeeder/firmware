/**
 * @file app_power.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 */

#ifndef _APP_POWER_HEADER_H
#define	_APP_POWER_HEADER_H

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
// Change value 07/02/2017, but R20 and R21 not in place on PCB !!!
//#define HIGH_BATTERY_LEVEL 3145
//#define LOW_BATTERY_LEVEL 3783
//#define BATTERY_VOLTAGE_FACTOR 0.00303951

#define HIGH_VBAT_LEVEL 3830
#define LOW_VBAT_LEVEL 3393
//#define VBAT_VOLTAGE_FACTOR 0.000795827117474
#define VBAT_VOLTAGE_FACTOR 0.0008056640625 // => 3.3/4096

bool isPowerServoEnable( void );
void powerPIREnable( void );
void powerPIRDisable( void );
void powerUsbRfidEnable( void );
void powerUsbRfidDisable( void );
uint16_t getADC1value( ADC1_CHANNEL channel );
void getVBatLevel( void );
void printVBatLevel( void );
void getBatteryLevel( void );
void printBatteryLevel( void );

#endif	/* _APP_POWER_HEADER_H */


/*******************************************************************************
 End of File
 */
