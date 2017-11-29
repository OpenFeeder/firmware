/******************************************************************************* 
  This is a library for the DS3231, extremely accurate I²C real-time clock (RTC)
  with an integrated temperature-compensated crystal oscillator (TCXO) and crystal.
  https://www.maximintegrated.com/en/products/digital/real-time-clocks/DS3231.html 
  These componant use I2C to communicate, 2 pins are required to interface
 *******************************************************************************
 * 
 * File: ds3231.h  
 * Author: Arnauld Biganzoli
 * Comments:
 * Revision history: 1
 * 
 */

#ifndef _DS3231_HEADER_H
#define _DS3231_HEADER_H

#include <stdint.h>
#include <time.h>
#include "i2c1.h"

/* 7-bit i2c slave address of the DS3231 chip */
#define DS3231_I2C_ADDR             0x68

/* Timekeeping registers */
#define DS3231_TIME_CAL_ADDR        0x00
#define DS3231_ALARM1_ADDR          0x07
#define DS3231_ALARM2_ADDR          0x0B
#define DS3231_CONTROL_ADDR         0x0E
#define DS3231_STATUS_ADDR          0x0F
#define DS3231_AGING_OFFSET_ADDR    0x10
#define DS3231_TEMPERATURE_ADDR     0x11

/* Control register bits */
#define DS3231_A1IE                 0x01
#define DS3231_A2IE                 0x02
#define DS3231_INTCN                0x04

/* Status register bits */
#define DS3231_A1F                  0x01
#define DS3231_A2F                  0x02
#define DS3231_OSF                  0x80

//#define SECONDS_FROM_1970_TO_2000   946684800

struct ts
{
    uint8_t sec; /* seconds */
    uint8_t min; /* minutes */
    uint8_t hour; /* hours */
    uint8_t mday; /* day of the month */
    uint8_t mon; /* month */
    int16_t year; /* year */
    uint8_t wday; /* day of the week */
    uint8_t yday; /* day in the year */
    uint8_t isdst; /* daylight saving time */
    uint8_t year_s; /* year in short notation*/
#ifdef CONFIG_UNIXTIME
    uint32_t unixtime; /* seconds since 01.01.1970 00:00:00 UTC*/
#endif
};

/* Main functions */
I2C1_MESSAGE_STATUS I2C1_MasterReadDS3231_get( struct ts *t );
I2C1_MESSAGE_STATUS I2C1_MasterReadDS3231_set( struct ts *t );


/* Helpers */
uint8_t dectobcd( const uint8_t val );
uint8_t bcdtodec( const uint8_t val );

#endif	/* _DS3231_HEADER_H */


/*******************************************************************************
 End of File
 */
