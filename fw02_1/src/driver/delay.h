/**
 * @file delay.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 * @revision history 1
 * @dependency tmr3 --> ISR every 20 ms
 */

#ifndef DELAY_H
#define	DELAY_H

//#include "mcc_generated_files/mcc.h"
#include "mcc.h"

void setDelayMs( uint16_t );
bool isDelayMsEnding( void );
void setDelayMsStandBy( uint16_t );
bool isDelayMsEndingStandBy( void );
void setDelayMsEM4095( uint16_t );
bool isDelayMsEndingEM4095( void );
void setDelayMsLEDsStatus( uint16_t );
bool isDelayMsEndingLEDsStatus( void );

#endif	/* DELAY_H */


/*******************************************************************************
 End of File
 */
