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
    appDataServo.measure_position = appDataServo.ton_cmd;
}


uint16_t servomotorGetDoorPosition( void )
{
    int i;
    int n = 6;
    uint16_t pos_buffer[n];
    uint16_t position;

    for (i=0;i<n;i++)
    {
        pos_buffer[i] = getADC1value( ADC1_CHANNEL_MA_SERVO );
    }
//        printf("%u %u %u %u %u %u\n", 
//               pos_buffer[0], 
//               pos_buffer[1], 
//               pos_buffer[2],
//               pos_buffer[3],
//               pos_buffer[4],
//               pos_buffer[5]);
        
    appDataServo.measure_position = pos_buffer[n-1];
    
    position = appDataServo.measure_position;

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_SERVO_POSITION)
    printf( "\tServo at %u\n", position );
#endif 
    return position;
}


bool servomotorMoveTheDoor( void )
{
    bool last_step = false;
    
    if ( appDataServo.ton_cmd != appDataServo.ton_goal )
    {
        /* Change servomotor position. */
        if ( appDataServo.ton_cmd > appDataServo.ton_goal )
        {           
            if ( ( appDataServo.ton_cmd - appDataServo.ton_goal) > appDataServo.speed)
            {
                appDataServo.ton_cmd -= ( uint16_t ) appDataServo.speed;
            }
            else
            {
                appDataServo.ton_cmd -= ( appDataServo.ton_cmd - appDataServo.ton_goal);
                last_step = true;
            }
        }
        else
        {
            if ( ( appDataServo.ton_goal-appDataServo.ton_cmd ) > appDataServo.speed)
            {
                appDataServo.ton_cmd += ( uint16_t ) appDataServo.speed;
            }
            else
            {
                appDataServo.ton_cmd += ( appDataServo.ton_goal-appDataServo.ton_cmd );
                last_step = true;
            }
        }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_SERVO_POSITION)
        printf( "\t\tMoving at %u\n", appDataServo.ton_cmd );
#endif 

        /* Set DC of PWM5. */
        OC5_PrimaryValueSet( appDataServo.ton_cmd );
        
        return last_step;
    }

    return true;
}

void servomotorPowerEnable( void )
{

    if ( false == appData.servo_powered && false == appData.pir_sensor_powered )
    {
        CMD_VDD_ACC_PIR_SERVO_SetHigh( );
    }
    
    /* The next line should be uncommented but there is a known bug *
     The PIR sensor reset when pin is set to low */
//    CMD_VCC_SERVO_SetLow( );

    CMD_VCC_SERVO_SetDigitalOutput( );
    appDataServo.cmd_vcc_servo_state = true;
    appData.servo_powered = true;
    
    setDelayMs( 50 );
    while ( false == isDelayMsEnding( ) )
    {
        Nop( );
    }
        
    OC5R = 0x0000;
    OC5_Start( );
    
}


void servomotorPowerDisable( void )
{

    if (false == appData.pir_sensor_powered)
    {
        CMD_VDD_ACC_PIR_SERVO_SetLow( );
    }
    
    CMD_VCC_SERVO_SetDigitalInput( );

    setDelayMs( 50 );
    while ( false == isDelayMsEnding( ) )
    {
        Nop( );
    }
    
    OC5R = 0x0000;
    OC5_Stop( );
    
    appDataServo.cmd_vcc_servo_state = false;  
    appData.servo_powered = false;

}


/*******************************************************************************
 End of File
 */
