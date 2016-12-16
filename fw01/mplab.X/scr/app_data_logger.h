/**
 * @file app_data_logger.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 08/09/2016
 */

#ifndef APP_DATA_LOGGER_H
#define APP_DATA_LOGGER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "usb_common.h"
#include "app_usb.h"
#include "app_config.h"

/* Nb de caracteres max sur une ligne du fichier LOG. */
#define MAX_CHAR_PER_LINE 55

/* Quantite de donnees a stocker dans le buffer avant ecriture dans le fichier LOG
 * --> donc nombre de passages d'oiseau
 */
#define MAX_NUM_DATA_TO_STORE 5

#define sizearray(a)  (sizeof(a) / sizeof((a)[0]))

bool dataLog(void);
bool setLogFileName(void);
void GetTimestamp(FILEIO_TIMESTAMP *);
void clearLogBuffer(void);

#endif /* APP_DATA_LOGGER_H */


/*******************************************************************************
 End of File
 */
