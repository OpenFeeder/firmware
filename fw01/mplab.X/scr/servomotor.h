/**
 * @file servomotor.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 * @revision history 1
 */

#ifndef SERVOMOTOR_H
#define	SERVOMOTOR_H

//#include <xc.h> /* include processor files - each processor file is guarded. */
#include "mcc_generated_files/mcc.h"
#include <stdint.h>
#include <stdbool.h>

/* Change servomotor position, Ton time (in us) corresponds to an angular angle.
 * Absolute range: 500 us < servo_position < 2500 us
 * Ton value depends on the actuator type.
 * HS-322HD: 600 us < servo_position < 2400 us, flexible nylon noise --> Ok
 * PARRALAX: 600 us < servo_position < 2400 us (Product ID: 900-00005), sound gear 
 */
#define SERVO_POSITION_MIN_DEFAULT      500     /* min:  500 us */
#define SERVO_POSITION_MAX_DEFAULT      2500    /* max: 2500 us */
#define SERVO_START_POSITION_DEFAULT    1500    /* initial servomotor position at the middle */
#define SERVO_SPEED_INC_DEFAULT         10      /* servomotor position increment every 20 ms */

/** [Clockwise](https://en.wikipedia.org/wiki/Clockwise)
 * A clockwise (typically abbreviated as CW) motion is one that proceeds in the same direction as a clock's hands.
 * The opposite sense of rotation or revolution is (in Commonwealth English) anticlockwise (ACW),
 * or (in North American English) counterclockwise (CCW).
 */
//#define CW  0 // direction clockwise
//#define CCW 1 // direction counterclockwise

/* Servomotor components */
typedef struct
{
    uint16_t ton_cmd; /* command servomotor position */
    uint16_t ton_min; /* minimum servomotor position */
    uint16_t ton_max; /* maximum servomotor position */
    uint8_t speed; /* servomotor speed increment to move from position A to B(ex: every 20 ms) */
    uint16_t measure_position; /* measured servomotor position */

} APP_DATA_SERVO;

// *****************************************************************************
// *****************************************************************************
// Section: extern declarations
// *****************************************************************************
// *****************************************************************************

void SERVO_Initialize( void );

/**
 * servomotorOpenTheDoor()
 * The servomotor open the door.
 * @return true if the door is open
 */
bool servomotorOpenTheDoor( void );

/**
 * servomotorCloseTheDoor()
 * The servomotor close the door.
 * @return true if the door is closed
 */
bool servomotorCloseTheDoor( void );

void servomotorPowerEnable( void );
void servomotorPowerDisable( void );


#endif	/* SERVOMOTOR_H */


/*******************************************************************************
 End of File
 */
