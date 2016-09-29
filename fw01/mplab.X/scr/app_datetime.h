/**
 * @file app_datetime.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#ifndef APP_DATETIME_H
#define APP_DATETIME_H


#include <xc.h>
#include <time.h>
#include <stdio.h>
#include "mcc_generated_files/rtcc.h"


void setDateTime(int, int, int, int, int, int);
void printCurrentDate();
void getDateTime( struct tm *);


#endif /* APP_DATETIME_H */


/*******************************************************************************
 End of File
 */
