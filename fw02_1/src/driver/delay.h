/**
 * @file delay.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 * @revision history 1
 * @dependency tmr3 --> ISR every 20 ms
 */

#ifndef _DELAY_HEADER_H
#define	_DELAY_HEADER_H

#include "mcc.h"

void setDelayMs( uint16_t );
bool isDelayMsEnding( void );
void setDelayMsReadFromUart( uint16_t );
bool isDelayMsEndingReadFromUart( void );
void setDelayMsStandBy( uint16_t );
bool isDelayMsEndingStandBy( void );
void setDelayMsEM4095( uint16_t );
bool isDelayMsEndingEM4095( void );
void setDelayMsLEDsStatus( uint16_t );
bool isDelayMsEndingLEDsStatus( void );
void setDelayPunishmentMs( uint16_t );
bool isDelayPunishmentMsEnding( void );

#endif	/* _DELAY_HEADER_H */


/*******************************************************************************
 End of File
 */
