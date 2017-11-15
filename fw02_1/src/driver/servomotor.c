/**
 * @file servomotor.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 * @revision history 1
 */

#include <xc.h>
#include "app.h"
#include "servomotor.h"
#include <libpic30.h>

void SERVO_Initialize( void )
{
    /* Servomotor power command disable. */
    servomotorPowerDisable( );
    /* Initialisation par defaut au démarrage du programme. */
    appDataServo.ton_min = SERVO_POSITION_MIN_DEFAULT;
    appDataServo.ton_max = SERVO_POSITION_MAX_DEFAULT;
    appDataServo.speed = SERVO_SPEED_INC_DEFAULT;
    appDataServo.ton_cmd = appDataServo.ton_max;

}

uint16_t getDoorPosition( void )
{
    return getADC1value( ADC1_CHANNEL_MA_SERVO );
}

bool servomotorMoveTheDoor( void )
{
    if ( appDataServo.ton_cmd != appDataServo.ton_goal )
    {

        /* Change servomotor position. */        
        if (appDataServo.direction == 1)
        {
            appDataServo.ton_cmd += ( uint16_t ) appDataServo.speed;
        }
        else
        {
            appDataServo.ton_cmd -= ( uint16_t ) appDataServo.speed;
        }
//#if defined (USE_UART1_SERIAL_INTERFACE)
//        printf( "%u %u\n", appDataServo.ton_cmd, getDoorPosition());
//#endif 
        if ( appDataServo.direction == 1 && appDataServo.ton_cmd >= appDataServo.ton_goal )
        {
            appDataServo.ton_cmd -= (appDataServo.ton_cmd - appDataServo.ton_goal);
//#if defined (USE_UART1_SERIAL_INTERFACE)
//            printf( "%u %u\n", appDataServo.ton_cmd, getDoorPosition());
//#endif
            OC5_PrimaryValueSet( appDataServo.ton_cmd );
//#if defined (USE_UART1_SERIAL_INTERFACE)
//            printf( "%u %u (last)\n", appDataServo.ton_cmd, getDoorPosition());
//#endif
            appDataServo.ton_cmd = appDataServo.ton_goal;
            return true;
        }
        else if ( appDataServo.direction == -1 && appDataServo.ton_cmd <= appDataServo.ton_goal )
        {
            appDataServo.ton_cmd += (appDataServo.ton_goal - appDataServo.ton_cmd);
//#if defined (USE_UART1_SERIAL_INTERFACE)
//            printf( "%u %u\n", appDataServo.ton_cmd, getDoorPosition());
//#endif
            OC5_PrimaryValueSet( appDataServo.ton_cmd );
//#if defined (USE_UART1_SERIAL_INTERFACE)
//            printf( "%u %u (last)\n", appDataServo.ton_cmd, getDoorPosition());
//#endif
            appDataServo.ton_cmd = appDataServo.ton_goal;
            return true;
        }
        
        /* Set DC of PWM5. */
        OC5_PrimaryValueSet( appDataServo.ton_cmd );

        return false;
    }

    return true;
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
            return true;
        }

        /* Set DC of PWM5. */
        OC5_PrimaryValueSet( appDataServo.ton_cmd );

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
            return true;
        }

        /* Set DC of PWM5. */
        OC5_PrimaryValueSet( appDataServo.ton_cmd );

        return false;
    }

    return true;
}

void servomotorPowerEnable( void )
{
    CMD_VDD_ACC_PIR_SERVO_SetHigh( );
    CMD_VCC_SERVO_SetLow( );
    CMD_VCC_SERVO_SetDigitalOutput( );
    appDataServo.cmd_vcc_servo_state = true;
}

void servomotorPowerDisable( void )
{
    CMD_VCC_SERVO_SetDigitalInput();
    appDataServo.cmd_vcc_servo_state = false;
}


/*******************************************************************************
 End of File
 */
