/**
 * @file ir_sensor.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 09/09/2016
 */

#include "app.h"
#include "ir_sensor.h"

volatile bool g_flag_ir1_sensor = false;
volatile bool g_flag_ir2_sensor = false;

void IRSensor_Initialize( void )
{
    
    IRSensorEnable( );

}

void IRSensorEnable( void )
{

    VDD_IR_ON( ); /* IR power enable. */

}

void IRSensorDisable( void )
{

    VDD_IR_OFF( ); /* IR power disable. */

}


/*******************************************************************************
 End of File
 */
