
/**
  RTCC Generated Driver API Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    rtcc.c

  @Summary:
    This is the generated header file for the RTCC driver using MPLAB(c) Code Configurator

  @Description:
    This header file provides APIs for driver for RTCC.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - pic24-dspic-pic32mm : v1.25
        Device            :  PIC24FJ256GB406
        Driver Version    :  0.5
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.26
        MPLAB 	          :  MPLAB X 3.45
 */

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */


/**
 Section: Included Files
 */

#include <xc.h>
#include "rtcc.h"
#include <stdio.h>
#include "app.h"

/**
// Section: Static function
 */

static void RTCC_Lock(void);
static bool rtccTimeInitialized;
static bool RTCCTimeInitialized(void);
static uint8_t ConvertHexToBCD(uint8_t hexvalue);
static uint8_t ConvertBCDToHex(uint8_t bcdvalue);

/**
// Section: Driver Interface Function Definitions
 */

void RTCC_Initialize(void)
{

    // Clear WRLOCK to modify RTCC as needed
    __builtin_write_RTCC_WRLOCK();
    
    // Disable RTCC
    RTCCON1Lbits.RTCEN = 0;

   if(!RTCCTimeInitialized())
    {
//#if defined (USE_UART1_SERIAL_INTERFACE)
//        printf( "RTCCTimeInitialized\n" );
//#endif

//        // set 2017-02-06 22-22-37
//       DATEH = 0x1702;    // Year/Month
//       DATEL = 0x601;    // Date/Wday
//       TIMEH = 0x2222;    // hours/minutes
//       TIMEL = 0x3700;    // seconds
    }
    // set 2017-02-06 22-23-31
    ALMDATEH = 0x1702;    // Year/Month
    ALMDATEL = 0x601;    // Date/Wday
    ALMTIMEH = 0x2223;    // hours/minutes
    ALMTIMEL = 0x3100;    // seconds
    // AMASK Every Day; ALMRPT 0; CHIME disabled; ALRMEN enabled; 
    RTCCON1H = 0x8600;

    // PWCPS 1:1; PS 1:1; CLKSEL SOSC; FDIV 0; 
    RTCCON2L = 0x0000;
    // DIV 16383; 
    RTCCON2H = 0x3FFF;
    // PWCSTAB 0; PWCSAMP 0; 
    RTCCON3L = 0x0000;

    // RTCEN enabled; OUTSEL Seconds Clock; PWCPOE disabled; TSBEN disabled; PWCEN disabled; WRLOCK enabled; PWCPOL disabled; TSAEN disabled; RTCOE enabled; 
    RTCCON1L = 0x8B90; 

   RTCC_Lock();

    IEC3bits.RTCIE = 1;
}

static void RTCC_Lock(void)
{
    asm volatile("DISI #6");
    asm volatile("MOV #NVMKEY, W1");
    asm volatile("MOV #0x55, W2");
    asm volatile("MOV W2, [W1]");
    asm volatile("MOV #0xAA, W3");
    asm volatile("MOV W3, [W1]");
    asm volatile("BSET RTCCON1L, #11");
}

bool RTCC_TimeGet(struct tm *currentTime)
{
    uint16_t register_value;
    if(RTCSTATLbits.SYNC){
        return false;
    }

    __builtin_write_RTCC_WRLOCK();

    register_value = DATEH;
    currentTime->tm_year = ConvertBCDToHex((register_value & 0xFF00) >> 8);
    currentTime->tm_mon = ConvertBCDToHex(register_value & 0x00FF);

    register_value = DATEL;
    currentTime->tm_mday = ConvertBCDToHex((register_value & 0xFF00) >> 8);
    currentTime->tm_wday = ConvertBCDToHex(register_value & 0x00FF);

    register_value = TIMEH;
    currentTime->tm_hour = ConvertBCDToHex((register_value & 0xFF00) >> 8);
    currentTime->tm_min = ConvertBCDToHex(register_value & 0x00FF);

    register_value = TIMEL;
    currentTime->tm_sec = ConvertBCDToHex((register_value & 0xFF00) >> 8);

    RTCC_Lock();

    return true;
}

void RTCC_TimeSet(struct tm *initialTime)
{

    // Clear WRLOCK to modify RTCC as needed
    __builtin_write_RTCC_WRLOCK();

    // Disable RTCC
    RTCCON1Lbits.RTCEN = 0;

    // Disable RTCC interrupt
    IFS3bits.RTCIF = false;
    IEC3bits.RTCIE = 0;

    // Set RTCC initial time
    DATEH = (ConvertHexToBCD(initialTime->tm_year) << 8) | ConvertHexToBCD(initialTime->tm_mon) ;  // YEAR/MONTH-1
    DATEL = (ConvertHexToBCD(initialTime->tm_mday) << 8) | ConvertHexToBCD(initialTime->tm_wday) ;  // /DAY-1/WEEKDAY
    TIMEH = (ConvertHexToBCD(initialTime->tm_hour) << 8)  | ConvertHexToBCD(initialTime->tm_min); // /HOURS/MINUTES
    TIMEL = (ConvertHexToBCD(initialTime->tm_sec) << 8) ;   // SECOND

    // Enable RTCC        
    RTCCON1Lbits.RTCEN = 1;
    // Enable RTCC interrupt
    IEC3bits.RTCIE = 1;
    
    // Lock the RTCC registers
    RTCCON1Lbits.WRLOCK = 1; 
    
//    RTCC_Lock();
    
    
}

bool RTCC_BCDTimeGet(bcdTime_t *currentTime)
{
    uint16_t register_value;
    if(RTCSTATLbits.SYNC){
        return false;
    }


    __builtin_write_RTCC_WRLOCK();

    register_value = DATEH;
    currentTime->tm_year = (register_value & 0xFF00) >> 8;
    currentTime->tm_mon = register_value & 0x00FF;

    register_value = DATEL;
    currentTime->tm_mday = (register_value & 0xFF00) >> 8;
    currentTime->tm_wday = register_value & 0x00FF;

    register_value = TIMEH;
    currentTime->tm_hour = (register_value & 0xFF00) >> 8;
    currentTime->tm_min = register_value & 0x00FF;

    register_value = TIMEL;
    currentTime->tm_sec = (register_value & 0xFF00) >> 8;

    RTCC_Lock();

    return true;
}

void RTCC_BCDTimeSet(bcdTime_t *initialTime)
{

   __builtin_write_RTCC_WRLOCK();

    RTCCON1Lbits.RTCEN = 0;

    IFS3bits.RTCIF = false;
    IEC3bits.RTCIE = 0;

    // set RTCC initial time
   DATEH = (initialTime->tm_year << 8) | (initialTime->tm_mon) ;  // YEAR/MONTH-1
   DATEL = (initialTime->tm_mday << 8) | (initialTime->tm_wday) ;  // /DAY-1/WEEKDAY
   TIMEH = (initialTime->tm_hour << 8) | (initialTime->tm_min); // /HOURS/MINUTES
   TIMEL = (initialTime->tm_sec << 8);   // SECONDS   

    // Enable RTCC, clear RTCWREN         
    RTCCON1Lbits.RTCEN = 1;
   RTCC_Lock();

    //Enable RTCC interrupt
    IEC3bits.RTCIE = 1;
}

/**
 This function implements RTCC_TimeReset.This function is used to
 used by application to reset the RTCC value and reinitialize RTCC value.
 */
void RTCC_TimeReset(bool reset)
{
    rtccTimeInitialized = reset;
}

static bool RTCCTimeInitialized(void)
{
    return(rtccTimeInitialized);
}

void RTCC_TimestampAEventManualSet(void)
{
    RTCSTATLbits.TSAEVT = 1;
}

bool RTCC_TimestampADataGet(struct tm *currentTime)
{
    uint16_t register_value;
    if(!RTCSTATLbits.TSAEVT){
        return false;
    }

    register_value = TSADATEH;
    currentTime->tm_year = ConvertBCDToHex((register_value & 0xFF00) >> 8);
    currentTime->tm_mon = ConvertBCDToHex(register_value & 0x00FF);

    register_value = TSADATEL;
    currentTime->tm_mday = ConvertBCDToHex((register_value & 0xFF00) >> 8);
    currentTime->tm_wday = ConvertBCDToHex(register_value & 0x00FF);

    register_value = TSATIMEH;
    currentTime->tm_hour = ConvertBCDToHex((register_value & 0xFF00) >> 8);
    currentTime->tm_min = ConvertBCDToHex(register_value & 0x00FF);

    register_value = TSATIMEL;
    currentTime->tm_sec = ConvertBCDToHex((register_value & 0xFF00) >> 8);

    RTCSTATLbits.TSAEVT = 0;

    return true;
}



bool RTCC_TimestampA_BCDDataGet(bcdTime_t *currentTime)
{
    uint16_t register_value;
    if(!RTCSTATLbits.TSAEVT){
        return false;
    }

    register_value = TSADATEH;
    currentTime->tm_year = (register_value & 0xFF00) >> 8;
    currentTime->tm_mon = (register_value & 0x00FF);

    register_value = TSADATEL;
    currentTime->tm_mday = (register_value & 0xFF00) >> 8;
    currentTime->tm_wday = (register_value & 0x00FF);

    register_value = TSATIMEH;
    currentTime->tm_hour = (register_value & 0xFF00) >> 8;
    currentTime->tm_min = (register_value & 0x00FF);

    register_value = TSATIMEL;
    currentTime->tm_sec = (register_value & 0xFF00) >> 8;

    RTCSTATLbits.TSAEVT = 0;

    return true;
}

void RTCC_TimestampBEventManualSet(void)
{
    RTCSTATLbits.TSBEVT = 1;
}

bool RTCC_TimestampBDataGet(struct tm *currentTime)
{
    uint16_t register_value;
    if(!RTCSTATLbits.TSBEVT){
        return false;
    }

    register_value = TSBDATEH;
    currentTime->tm_year = ConvertBCDToHex((register_value & 0xFF00) >> 8);
    currentTime->tm_mon = ConvertBCDToHex(register_value & 0x00FF);

    register_value = TSBDATEL;
    currentTime->tm_mday = ConvertBCDToHex((register_value & 0xFF00) >> 8);
    currentTime->tm_wday = ConvertBCDToHex(register_value & 0x00FF);

    register_value = TSBTIMEH;
    currentTime->tm_hour = ConvertBCDToHex((register_value & 0xFF00) >> 8);
    currentTime->tm_min = ConvertBCDToHex(register_value & 0x00FF);

    register_value = TSBTIMEL;
    currentTime->tm_sec = ConvertBCDToHex((register_value & 0xFF00) >> 8);

    RTCSTATLbits.TSBEVT = 0;

    return true;
}

bool RTCC_TimestampB_BCDDataGet(bcdTime_t *currentTime)
{
    uint16_t register_value;
    if(!RTCSTATLbits.TSBEVT){
        return false;
    }

    register_value = TSBDATEH;
    currentTime->tm_year = (register_value & 0xFF00) >> 8;
    currentTime->tm_mon = (register_value & 0x00FF);

    register_value = TSBDATEL;
    currentTime->tm_mday = (register_value & 0xFF00) >> 8;
    currentTime->tm_wday = (register_value & 0x00FF);

    register_value = TSBTIMEH;
    currentTime->tm_hour = (register_value & 0xFF00) >> 8;
    currentTime->tm_min = (register_value & 0x00FF);

    register_value = TSBTIMEL;
    currentTime->tm_sec = (register_value & 0xFF00) >> 8;

    RTCSTATLbits.TSBEVT = 0;

    return true;
}




static uint8_t ConvertHexToBCD(uint8_t hexvalue)
{
    uint8_t bcdvalue;
    bcdvalue = (hexvalue / 10) << 4;
    bcdvalue = bcdvalue | (hexvalue % 10);
    return (bcdvalue);
}

static uint8_t ConvertBCDToHex(uint8_t bcdvalue)
{
    uint8_t hexvalue;
    hexvalue = (((bcdvalue & 0xF0) >> 4)* 10) + (bcdvalue & 0x0F);
    return hexvalue;
}


/* Function:
  void __attribute__ ( ( interrupt, no_auto_psv ) ) _ISR _RTCCInterrupt( void )

  Summary:
    Interrupt Service Routine for the RTCC Peripheral

  Description:
    This is the interrupt service routine for the RTCC peripheral. Add in code if 
    required in the ISR. 
 */
void __attribute__( ( interrupt, no_auto_psv ) ) _ISR _RTCCInterrupt( void )
{
    if ( true == appData.flags.bit_value.systemInit )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RTCC)
        printf( "_RTCCInterrupt() " );
#endif 

        appData.rtcc_alarm_action = RTCC_ALARM_IDLE;

        if ( appData.openfeeder_state == OPENFEEDER_IS_SLEEPING )
        {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RTCC)
            printf( "- Wakeup\n" );
#endif 
            appData.rtcc_alarm_action = RTCC_ALARM_WAKEUP_OPENFEEDER;
        }
        else
        {
            while ( !RTCC_TimeGet( &appData.current_time ) )
            {
                Nop( );
            }

            if ( appData.current_time.tm_hour >= appDataAlarmSleep.time.tm_hour &&
                 appData.current_time.tm_min >= appDataAlarmSleep.time.tm_min )
            {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RTCC)
                printf( "- Sleep\n" );
#endif 
                appData.rtcc_alarm_action = RTCC_ALARM_SLEEP_OPENFEEDER;
            }
            else
            {

                /* Attractive LEDs on/off */
                if (true == appData.flags.bit_value.attractive_leds_status)
                {
                    if ( ( appData.current_time.tm_hour * 60 + appData.current_time.tm_min ) >= ( appDataAttractiveLeds.wake_up_time.tm_hour * 60 + appDataAttractiveLeds.wake_up_time.tm_min ) &&
                         ( appData.current_time.tm_hour * 60 + appData.current_time.tm_min )< ( appDataAttractiveLeds.sleep_time.tm_hour * 60 + appDataAttractiveLeds.sleep_time.tm_min ) )

                    {
    #if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RTCC)
                        printf( "- LEDs on\n" );
    #endif 
                        appData.rtcc_alarm_action = RTCC_ALARM_SET_ATTRACTIVE_LEDS_ON;
                    }
                    else
                    {
    #if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RTCC)
                        printf( "- LEDs off\n" );
    #endif 
                        appData.rtcc_alarm_action = RTCC_ALARM_SET_ATTRACTIVE_LEDS_OFF;
                    }
                }

                /* Door open/close */
                if ( 1 == appDataDoor.remain_open )
                {

                    if ( ( appData.current_time.tm_hour * 60 + appData.current_time.tm_min ) >= ( appDataDoor.open_time.tm_hour * 60 + appDataDoor.open_time.tm_min ) &&
                         ( appData.current_time.tm_hour * 60 + appData.current_time.tm_min )< ( appDataDoor.close_time.tm_hour * 60 + appDataDoor.close_time.tm_min ) )

                    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RTCC)
                        printf( "- Door open\n" );
#endif 
                        appData.rtcc_alarm_action = RTCC_ALARM_OPEN_DOOR;
                    }
                    else
                    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RTCC)
                        printf( "- Door close\n" );
#endif 
                        appData.rtcc_alarm_action = RTCC_ALARM_CLOSE_DOOR;
                    }
                }

                /* Alternate LEDs color */
                if ( COLOR_ASSOCIATIVE_LEARNING == appData.scenario_number && ATTRACTIVE_LEDS_ON == appDataAttractiveLeds.status )
                {
                    if ( appDataAttractiveLeds.alt_sec_elapsed == appDataAttractiveLeds.alt_delay - 1 )
                    {
                        appData.rtcc_alarm_action = RTCC_ALARM_ALT_ATTRACTIVE_LEDS;
                        appDataAttractiveLeds.alt_sec_elapsed = 0;
                    }
                    else
                    {
                        ++appDataAttractiveLeds.alt_sec_elapsed;
                    }
                }

                /* Battery level */
                if ( appData.current_time.tm_min == 0 && appData.current_time.tm_sec == 30 )
                {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RTCC)
                    printf( "- Battery check\n" );
#endif 
                    appData.rtcc_alarm_action = RTCC_BATTERY_LEVEL_CHECK;
                }
                
                /* Food level */
                if ( appData.current_time.tm_min == 5 && appData.current_time.tm_sec == 30 )
                {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RTCC)
                    printf( "- Food check\n" );
#endif 
                    appData.rtcc_alarm_action = RTCC_FOOD_LEVEL_CHECK;
                }
                
                /* RFID frequency */
                if (( appData.current_time.tm_min == 0 || 
                     appData.current_time.tm_min == 15 || 
                     appData.current_time.tm_min == 30 || 
                     appData.current_time.tm_min == 45) && appData.current_time.tm_sec == 45 )
                {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_ISR_RTCC)
                    printf( "- RFID check\n" );
#endif 
                    appData.rtcc_alarm_action = RTCC_RFID_FREQ_CHECK;
                }

            }
        }
    }

    IFS3bits.RTCIF = false; /* Clear interrupt flag. */
}
/**
 End of File
 */
