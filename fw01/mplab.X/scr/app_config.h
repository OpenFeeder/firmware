/**
 * @file app_config.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "min_ini.h"

#define sizearray(a)  (sizeof(a) / sizeof((a)[0]))

//#define LOGFILENAME "LOG.CSV"
#define DEFAULT_LOG_SEPARATOR ","

#define CONFIG_INI_FOUND 0
#define CONFIG_INI_NOT_FOUND -1

#define CONFIG_INI_READ_OK 0
#define CONFIG_INI_READ_NOT_OK -1


bool config_set( void );
int config_find_ini( void ); // FIXME: rename to config_find() ?
int8_t config_read_ini( void ); // FIXME: rename to config_read() ?
int8_t config_check_parameters( void );
void config_print( void );
void config_set_parameters( void );


#endif /* APP_CONFIG_H */


/*******************************************************************************
 End of File
 */
