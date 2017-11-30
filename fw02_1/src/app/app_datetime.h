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


void setDateTime( int, int, int, int, int, int );
void printCurrentDate( );
void getDateTime( struct tm * );
void getCurrentDate( void );
void calibrateCurrentDate( void );

#endif /* _APP_DATETIME_HEADER_H */


/*******************************************************************************
 End of File
 */
