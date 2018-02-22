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
    appDataServo.ton_min = SERVO_DEFAULT_MIN_POSITION;
    appDataServo.ton_max = SERVO_DEFAULT_MAX_POSITION;
    appDataServo.speed = SERVO_DEFAULT_SPEED_INC;
    appDataServo.ton_cmd = appDataServo.ton_max;
    appDataServo.measure_position = appDataServo.ton_cmd;
}


uint16_t servomotorGetDoorPosition( void )
{
    int i;
    int n = 3;
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

    if ( appDataServo.ton_cmd != appDataServo.ton_goal )
    {
        /* Change servomotor position. */
        if ( appDataServo.ton_cmd > appDataServo.ton_goal )
        {        
            appDataServo.speed = appDataServo.closing_speed;
            
            if ( ( appDataServo.ton_cmd - appDataServo.ton_goal) > appDataServo.speed)
            {
                appDataServo.ton_cmd -= ( uint16_t ) appDataServo.speed;
            }
            else
            {
                appDataServo.ton_cmd -= ( appDataServo.ton_cmd - appDataServo.ton_goal);
            }
        }
        else
        {
            appDataServo.speed = appDataServo.opening_speed;
            
            if ( ( appDataServo.ton_goal-appDataServo.ton_cmd ) > appDataServo.speed)
            {
                appDataServo.ton_cmd += ( uint16_t ) appDataServo.speed;
            }
            else
            {
                appDataServo.ton_cmd += ( appDataServo.ton_goal-appDataServo.ton_cmd );
            }
        }
        
        /* Set DC of PWM5. */
        OC5_PrimaryValueSet( appDataServo.ton_cmd );

        return false;
    }

    return true;
}

void servomotorPowerEnable( void )
{

    if ( true == appDataLog.log_events )
    {
       store_event(OF_POWER_SERVO_ON); 
    }

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
    
    OC5_Start( );
    
}


void servomotorPowerDisable( void )
{

    if ( true == appDataLog.log_events )
    {
       store_event(OF_POWER_SERVO_OFF); 
    }

    if (false == appData.pir_sensor_powered)
    {
        CMD_VDD_ACC_PIR_SERVO_SetLow( );
    }
    
    CMD_VCC_SERVO_SetDigitalInput( );

    OC5_Stop( );
    
    appDataServo.cmd_vcc_servo_state = false;  
    appData.servo_powered = false;

}


/*******************************************************************************
 End of File
 */
