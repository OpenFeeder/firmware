/**
 * @file servomotor.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 * @revision history 1
 */

#ifndef SERVOMOTOR_H
#define	SERVOMOTOR_H

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

typedef enum
{
    DOOR_IDLE, /* Not in use. */
    DOOR_OPENED,
    DOOR_CLOSED,
    DOOR_OPENING, /* Opening in action. */
    DOOR_CLOSING /* Closing in action. */

} DOOR_STATUS;

typedef struct
{
    uint16_t open_delay;
    uint16_t close_delay;

    struct tm open_time;
    struct tm close_time;

    uint8_t remain_open;

    DOOR_STATUS reward_door_status;
    
    double reward_probability;

} APP_DATA_DOOR;

/* Servomotor components */
typedef struct
{
    bool cmd_vcc_servo_state;
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
