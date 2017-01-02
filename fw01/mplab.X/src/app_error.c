/**
 * @file app_error.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 06/07/2016
 * @revision
 */

#include "app.h"
#include "app_error.h"


void printError(void)
{
    if (appError.currentLineNumber > 0)
        printf("ERROR: %s\nIn %s (%d)\n", appError.message, appError.currentFileName, appError.currentLineNumber);
    else
        printf("ERROR: %s\n", appError.message);

}


void clearError(void)
{
    memset(appError.message, '\0', sizeof ( appError.message));
    memset(appError.currentFileName, '\0', sizeof ( appError.currentFileName));
    appError.currentLineNumber = 0;
}


/*******************************************************************************
 End of File
 */
