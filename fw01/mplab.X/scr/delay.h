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

//#include <xc.h> /* include processor files - each processor file is guarded. */
#include "mcc_generated_files/mcc.h"
#include <stdint.h>
#include <stdbool.h>

void setDelayMs( uint16_t timeout_ms );
bool isDelayMsEnding( void );
void setDelayMsLEDsStatus( uint16_t timeout_ms );
bool isDelayMsEndingLEDsStatus( void );


#endif	/* DELAY_H */


/*******************************************************************************
 End of File
 */
