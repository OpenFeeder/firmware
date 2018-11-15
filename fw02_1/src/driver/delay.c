/**
 * @file delay.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 * @revision history 1
 * @dependency tmr3 --> ISR every 20 ms
 */

#include "app.h"
#include "delay.h"

/* Not blocking delay function, can be used with this project. */
// TODO: change g_timeout_x20ms

void setDelayMs( uint16_t timeout_ms )
{
    if ( timeout_ms < 20 )
    {
        g_timeout_x20ms = 1;
    }
    else
    {
        g_timeout_x20ms = timeout_ms / 20;
    }
}

bool isDelayMsEnding( void )
{
    if ( g_timeout_x20ms == 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void setDelayMsStandBy( uint16_t timeout_ms )
{
    if ( timeout_ms < 20 )
    {
        g_timeout_standby_x20ms = 1;
    }
    else
    {
        g_timeout_standby_x20ms = timeout_ms / 20;
    }
}

bool isDelayMsEndingStandBy( void )
{
    if ( g_timeout_standby_x20ms == 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void setDelayMsEM4095( uint16_t timeout_ms )
{
    if ( timeout_ms < 20 )
    {
        g_timeout_em4095_x20ms = 1;
    }
    else
    {
        g_timeout_em4095_x20ms = timeout_ms / 20;
    }
}

bool isDelayMsEndingEM4095( void )
{
    if ( g_timeout_em4095_x20ms == 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void setDelayMsLEDsStatus( uint16_t timeout_ms )
{
    if ( timeout_ms < 20 )
    {
        g_timeout_leds_status_x20ms = 1;
    }
    else
    {
        g_timeout_leds_status_x20ms = timeout_ms / 20;
    }
}

bool isDelayMsEndingLEDsStatus( void )
{
    if ( g_timeout_leds_status_x20ms == 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void setDelayPunishmentMs( uint16_t timeout_ms )
{
    if ( timeout_ms < 20 )
    {
        g_timeout_punishment_x20ms = 1;
    }
    else
    {
        g_timeout_punishment_x20ms = timeout_ms / 20;
    }
}

bool isDelayPunishmentMsEnding( void )
{
    if ( g_timeout_punishment_x20ms == 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}


/*******************************************************************************
 End of File
 */
