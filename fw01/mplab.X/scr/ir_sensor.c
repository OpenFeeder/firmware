/**
 * @file ir_sensor.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 09/09/2016
 */

#include "mcc_generated_files/mcc.h"
#include "ir_sensor.h"

bool g_flag_ir1_sensor = false;
bool g_flag_ir2_sensor = false;

void IRSensor_Initialize( void )
{
    IRSensorDisable( );
}

void IRSensorEnable( void )
{
    /* Clear the interrupt flag and re-enable the interrupt */
    VDD_IR_ON( ); /* powering IR command enable. */
    TMR4_Start( );
    EX_INT1_InterruptDisable( );
    EX_INT1_PositiveEdgeSet( ); // set the edge
    EX_INT1_InterruptFlagClear( );
    EX_INT1_InterruptEnable( );   
    __delay_ms( 10 );
    Nop( );
    Nop( );
    Nop( );
    Nop( );
    Nop( );
    EX_INT1_InterruptFlagClear( );
    clear_ir1_sensor_detected( );
}

void IRSensorDisable( void )
{
    /* Clear the interrupt flag and re-enable the interrupt */
    EX_INT1_InterruptDisable( );
    VDD_IR_OFF( ); /* powering IR command disable. */
    TMR4_Stop( ); /* en commun avec l'EM4095 */
    _LATC3 = 1; // couper l'alimentation PWM de la LED IR en se mettant à l'état haut
    clear_ir1_sensor_detected( );
}


/*******************************************************************************
 End of File
 */
