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
 * /!\ 11.5 V --> 2929 => mode SLEEP
 */

#define DELAY_AFTER_POWERUP 1000

/* 3145 --> 12.35 V */
#define HIGH_BATTERY_LEVEL 3145
/* 2929 --> 11.50 V */
#define LOW_BATTERY_LEVEL 2929
#define BATTERY_VOLTAGE_FACTOR 0.003925439453125

#define HIGH_VBAT_LEVEL 3830
#define LOW_VBAT_LEVEL 3393
/* 3.3/4096 = 0.0008056640625 */
#define VBAT_VOLTAGE_FACTOR 0.0008056640625 

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

void manageResetSituation( void );
void printResetSituation( void );

#endif	/* _APP_POWER_HEADER_H */


/*******************************************************************************
 End of File
 */
