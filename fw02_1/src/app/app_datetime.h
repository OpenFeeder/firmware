/**
 * @file app_datetime.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#ifndef _APP_DATETIME_HEADER_H
#define _APP_DATETIME_HEADER_H


#include <xc.h>
#include <time.h>
#include <stdio.h>
#include "rtcc.h"

extern uint16_t current_millis; // global variable for current milliseconde capture

bool setDateTime( int, int, int, int, int, int );
void printDateTime( struct tm );
bool getDateTime( void );
void calibrateDateTime( void );


#endif /* _APP_DATETIME_HEADER_H */

/*******************************************************************************
 End of File
 */
