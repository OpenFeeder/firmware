/**
 * @file ir_sensor.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 09/09/2016
 */

#ifndef IR_SENSOR_H
#define	IR_SENSOR_H

#include "pin_manager.h"
#include "ext_int.h"
#include "tmr4.h"

#define VDD_IR_ON()  {CMD_VCC_IR_SetLow( );}  /* Powering IR command enable.  */
#define VDD_IR_OFF() {CMD_VCC_IR_SetHigh( );} /* Powering IR command disable. */

/* Taking reward detected. */
#define is_ir1_sensor_detected( ) g_flag_ir1_sensor
#define clear_ir1_sensor_detected( ) {g_flag_ir1_sensor = false;} /* Clear the reward detection sensor. */

#define set_flag_ir1_sensor( ) { g_flag_ir1_sensor = true; }
#define clear_flag_ir1_sensor( ) { g_flag_ir1_sensor = false; }
#define set_flag_ir2_sensor( ) { g_flag_ir2_sensor = true; }
#define clear_flag_ir2_sensor( ) { g_flag_ir2_sensor = false; }

extern volatile bool g_flag_ir1_sensor;
extern volatile bool g_flag_ir2_sensor;

void IRSensor_Initialize( void );
void IRSensorEnable( void );
void IRSensorDisable( void );


#endif	/* IR_SENSOR_H */


/*******************************************************************************
 End of File
 */
