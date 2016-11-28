/**
 * @file servomotor.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 * @revision history 1
 */

#include "app.h"
#include "servomotor.h"

void SERVO_Initialize( void )
{
    /* Servomotor power command enable. */
    servomotorPowerDisable( );
    /* Initialisation par defaut au démarrage du programme. */
    appDataServo.ton_min = SERVO_POSITION_MIN_DEFAULT;
    appDataServo.ton_max = SERVO_POSITION_MAX_DEFAULT;
    appDataServo.speed = SERVO_SPEED_INC_DEFAULT;
    appDataServo.ton_cmd = appDataServo.ton_min;
}

/**
 * bool servomotorOpenTheDoor( void )
 * @return true if appDataServo.ton_max is reach
 */
bool servomotorOpenTheDoor( void )
{
    if ( appDataServo.ton_cmd != appDataServo.ton_max )
    {
        /* Change servomotor position. */
        appDataServo.ton_cmd = appDataServo.ton_cmd + ( uint16_t ) appDataServo.speed;

        if ( appDataServo.ton_cmd >= appDataServo.ton_max )
        {
            appDataServo.ton_cmd = appDataServo.ton_max;
            appData.reward_door_status = DOOR_IDLE;
            return true;
        }

        /* Set DC of PWM5. */
        OC5_SingleCompareValueSet( appDataServo.ton_cmd );
        
        return false;
    }

    return true;
}

/**
 * bool servomotorCloseTheDoor( void )
 * @return true if appDataServo.ton_min is reach
 */
bool servomotorCloseTheDoor( void )
{
    if ( appDataServo.ton_cmd != appDataServo.ton_min )
    {
        /* Change servomotor position. */
        appDataServo.ton_cmd = appDataServo.ton_cmd - ( uint16_t ) appDataServo.speed;

        if ( appDataServo.ton_cmd <= appDataServo.ton_min )
        {
            appDataServo.ton_cmd = appDataServo.ton_min;
            appData.reward_door_status = DOOR_IDLE;
            return true;
        }

        /* Set DC of PWM5. */
        OC5_SingleCompareValueSet( appDataServo.ton_cmd );
        
        return false;
    }

    return true;
}

void servomotorPowerEnable( void )
{
    CMD_VCC_SERVO_SetHigh( );
}

void servomotorPowerDisable( void )
{
    __delay_ms( 10 );
    CMD_VCC_SERVO_SetLow( );
}


/*******************************************************************************
 End of File
 */
