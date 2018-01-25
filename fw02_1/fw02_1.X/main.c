/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using MPLAB(c) Code Configurator
    Implement on OpenFeeder board02
    Configure firmware option in \firmwares\fw02\src\app\app.h

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - pic24-dspic-pic32mm : v1.25
        Device            :  PIC24FJ256GB406
        Driver Version    :  0.5
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.31
        MPLAB             :  MPLAB X 3.55
 
  Source of inspiration:
    Project "USB Host - MSD - Simple" from Microchip Libraries for Applications (MLA) 
        C:\microchip\mla\v2016_11_07\apps\usb\host\msd_simple_demo\firmware\demo_src\main.c
    Project "minIni" from http://www.compuphase.com/minini.htm
        https://github.com/compuphase/minIni
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
 * @brief Development of a test program with PIC24FJ256GB406 MCU on OpenFeeder prototype board.
 * @see [OpenFeeder Board v2_0](https://docs.google.com/document/d/1EUuRprU-K5NVF4FKyMJyoSdlVwXycWjznXlICHy2UsM/edit?usp=sharing)
 * Author: OpenFeeder Team <https://github.com/orgs/OpenFeeder/people> 
 * @version fw02
 * @revision 
 * @date 13/02/2017
 */

/**
 * ------------------------------- Coding style -------------------------------
 * info: http://tdinfo.phelma.grenoble-inp.fr/2Aproj/fiches/coding_styles.pdf
 * 
 * Le passage de paramètres entre les différentes fonctions du sytème se fait
 * par variables globales. Par exemple la commande du servomoteur entre
 * servomotor, tmr3...
 * 
 */

/**
 * ---------------------------- Naming convention -----------------------------
 * -WorkInProgress-
 * Variables glogales et fonctions
 *   Minuscule pour le premier mot, majuscule ensuite.
 * Ex: les noms des fonctions suivent la convention de nommage suivante:
 *   lowerCamelCase --> ceciEstUneFonction
 * 
 * Les noms des variables locales suivent la convention de nommage suivante:
 *   snake_case --> ceci_est_une_variable_locale
 * 
 * Les variables pointeur seront identifiées en placant "p_" au début de leurs noms.
 *   ex: p_tabPitTagsDenied
 * 
 */

/**
 * --------------------------- Documentations links ----------------------------
 *  > OpenFeeder Board v2_0
 *    https://drive.google.com/open?id=1EUuRprU-K5NVF4FKyMJyoSdlVwXycWjznXlICHy2UsM
 * 
 *  > PIC24FJ256GB406 resources for proto v2_0
 *    https://drive.google.com/open?id=1iViNcH4uDjHy5RbMI0lRRTmK-bU2HOEeX6FlfLEffbg
 */

/** 
 * ------------------ Definition and wiring for this project -------------------
 *  Debugging Interface:
 *  Used UART1 at 9600 bauds for communication (9600/8-N-1, see https://en.wikipedia.org/wiki/8-N-1)
 *  see https://fr.wikipedia.org/wiki/RS-232
 *  RB14/UART1_RX --> U1RX
 *  RB15/UART1_TX --> U1TX
 * 
 *  RFID with EM4095:
 *  . Output "RE00/EM4095_SHD" control EM4095 Shutdown
 *  25.5 ms is necessary for the module EM4095 to be activate, and function during 135 ms
 *  RE0 --> EM4095_SHD (toggle in tmr4.c)
 *  . Use INT4 to capture the falling edge of EM4095 DEMOD_OUT signal
 *  RD02/EM4095_DEMOD_OUT --> EXT_INT:INT4;
 *  . Use Timer4 to capture the value of EM4095 DEMOD_OUT signal every 442 us
 * 
 *  RGB LED status:
 *  RE05/LED_STATUS_R --> LED_STATUS_R
 *  RE06/LED_STATUS_G --> LED_STATUS_G
 *  RE07/LED_STATUS_B --> LED_STATUS_B
 * 
 *  --> 24-bit RGB
 *  256^3 = 16,777,216 colors (TrueColor)
 *  True color (24-bit) RGB24 color selected in file "CONFIG.INI".
 *  Each color ranges from 0 to 255, digital 8-bit per channel (R, G, B).
 *  Exemple "Red"   RGB triplet: (255, 0, 0) or Hex triplet: #FF0000
 *  Exemple "Green" RGB triplet: (0, 255, 0) or Hex triplet: #00FF00
 *  Exemple "Blue"  RGB triplet: (0, 0, 255) or Hex triplet: #0000FF
 *  https://en.wikipedia.org/wiki/List_of_colors_(compact)
 * 
 *  OC4 in PWM mode at 38 kHz (with TMR4 period 26.3 us)
 *  pin RD06/PWM_LED_IR --> BAR_IR_PWM
 * 
 *  OC5 in PWM mode at 50 Hz (with TMR3 period 20 ms)
 *  pin RD07/PWM_SERVO --> CMD_SERVO
 *  and RF01/CMD_VCC_SERVO use to command VDD_SERVO (5V)
 * 
 *  I2C Multiplexage of MCP23017 every 3.744 ms with TMR
 * 
 * C:\git\openfeeder\firmwares\fw02\src\driver\ir_sensor.c
 * IR2 take 50 ms to check food with 60 ms, and with 200 ms delay is taking 330 ms and powering for 585 ms
 * 
 * on PIC24FJ256GB406
 *   - RTCC_OUT --> Output of the RTCC clock signal = 1 Hz
 *   - Pins available:
 *      SPARE1: RE02/SPARE_4
 *      SPARE2: RE03/SPARE_5
 *      SPARE3: RE04
 *      SPARE4: RF00/PWM6
 */

/**
 * ------------------------------ Demo OpenFeeder ------------------------------
 * > Interface firmware terminal (Debug)
 *   - RTCC module:
 *     . send 'T' --> Read date and time from the RTCC module
 *     . send 'S' --> Set RTCC module date and time (ex: 22/08/2016 15:59:30 --> 22 <CR> 8 <CR> 16 <CR> 15 <CR> 59 <CR> 30 <CR>)
 *   - Attractive RGB LED:
 *     . send 'R' --> Set PWM duty cycle of Red color (0-255, example for 50%: 128 <CR>)
 *     . send 'G' --> Set PWM duty cycle of Green color (0-255)
 *     . send 'B' --> Set PWM duty cycle of Blue color (0-255)
 *   - Servomotor:
 *     . send 'V' --> Powering servomotor enable/disable
 *     . send 'A' --> Measure analog servomotor position (ADC return analog value from 550 < HS-322HD < 2524, servomotor power must be enable)
 *     . send 'P' --> Change servomotor position (600 < HS-322HD < 2400, ex: 1500 <CR>)
 *     . send 'O' --> Open reward door
 *     . send 'C' --> Close reward door
 *   - IR sensor module (need TMR4 to generate 38 kHz from PWM4):
 *     . send 'I' --> IR power setting ON
 *     . send 'J' --> IR power setting OFF
 *   - RFID module reader:
 *     . send 'E' --> Measuring RDY/CLK period of EM4095 device
 *   - USB status:
 *     . send 'U': display USB device status 
 */

#include "mcc.h"
#include "app.h"

/*
                         Main application
 */

int main( void )
{
    I2C1_MESSAGE_STATUS i2c_status;

    int i;
    
    appData.reset_1.reg = RCON;
    appData.reset_2.reg = RCON2;
 
#if defined (ENABLE_DEEP_SLEEP)   
    if ( RCONbits.DPSLP )
    {
        /* This means that keeping the SOSC running after waking up requires 
         * the SOSCEN bit to be set before clearing RELEASE. 
         PIC24FJ256GB406 datasheet page 202 */
        OSCCONbits.SOSCEN = 1;

        appData.dsgpr0.reg = DSGPR0;
        appData.dsgpr1.reg = DSGPR1;
        
        DSCONbits.RELEASE = 0;
        DSCONbits.RELEASE = 0;

        RCONbits.DPSLP = 0;
 
    }
#endif    
    
    /*Clear the RESET CONTROL registers */
    RCON = 0;
    RCON2 = 0;
    
    appData.dsgpr0.reg = 0;
    appData.dsgpr1.reg = 0;
        
    /* Initialize the device. */
    SYSTEM_Initialize( );

    getDeviceId();
    getUniqueDeviceId();

    /* "dsPIC33/PIC24 Family Reference Manual", "Reset" (DS39712)
     * 7.13 REGISTERS AND STATUS BIT VALUES, page 13 */
    appData.reset_status_num = appData.reset_2.bit_value.vddbor +
        (appData.reset_2.bit_value.vddpor<<1) +
        (appData.reset_2.bit_value.vbpor<<2) +
        (appData.reset_2.bit_value.vbat<<3) +
        (appData.reset_1.bit_value.por<<4) +
        (appData.reset_1.bit_value.bor<<5) +
        (appData.reset_1.bit_value.cm<<6) +        
        (appData.reset_1.bit_value.idle<<7) +
        (appData.reset_1.bit_value.sleep<<8) +
        (appData.reset_1.bit_value.wdto<<9) +
        (appData.reset_1.bit_value.swr<<10) +
        (appData.reset_1.bit_value.extr<<11) +
        (appData.reset_1.bit_value.dpslp<<12) +
        (appData.reset_1.bit_value.iopuwr<<13) +
        (appData.reset_1.bit_value.trapr<<14);

    manageResetSituation( );
    
    /* Initialize peripheral driver. */
    RFID_Initialize( );
    SERVO_Initialize( );

    /* Initialize the file IO system. */
    FILEIO_Initialize( );
    FILEIO_RegisterTimestampGet( GetTimestamp );

    /* Initialize the USB Host API. */
    if ( !USBHostInit( 0 ) )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
        printf( "\nFailure to initialize USB Host API!\n" );
#endif
        LedsStatusBlink( LEDS_ERROR, LEDS_OFF, 83, 250 );
    }

    /* Initialize the application. */
    APP_Initialize( );

    if ( BUTTON_NOT_PRESSED == USER_BUTTON_GetValue( ) )
    {
#if defined (USE_UART1_SERIAL_INTERFACE)
        /* Display information on serial terminal. */
        displayBootMessage( );
#endif

        if ( 2101 == appData.reset_status_num || 2107 == appData.reset_status_num )
        {          
            setLedsStatusColor( LEDS_ON );
            setDelayMs( 5000 );
            while ( 0 == isDelayMsEnding( ) ); 
            setLedsStatusColor( LEDS_OFF );
        }

        /* Main loop. */
        while ( 1 )
        {
            /* Maintain Device Drivers. */
            USBTasks( );

            /* Maintain the application's state machine. */
            APP_Tasks( ); /* application specific tasks */   
        }
    }
    else
    {
        printf( "\n\nEnter in serial debug mode...\n" );

        /* Status LED blinks */
        TMR3_Start( );                
        for (i=0;i<3;i++)
        {
            setLedsStatusColor( LEDS_ON );
            setDelayMs( 500 );
            while ( 0 == isDelayMsEnding( ) ); 
            setLedsStatusColor( LEDS_OFF );
            setDelayMs( 500 );
            while ( 0 == isDelayMsEnding( ) ); 
        }
        TMR3_Stop( );
                
#if defined (DISPLAY_RESET_REGISTERS)
        /* Display reset registers. */
        displayResetRegisters( );
#endif

        OC4_Stop( );
        OC5_Stop( );
        TMR4_Stop( );
        TMR2_Stop( );

        i2c_status = initAttractiveLeds( );

        if ( i2c_status )
        {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
            printf( "\tAttractive LEDs: ok\n" );
#endif
        }
        else
        {
            sprintf( appError.message, "Unable to initialize attractive LEDS via I2C" );
            appError.currentLineNumber = __LINE__;
            sprintf( appError.currentFileName, "%s", __FILE__ );
            appError.number = ERROR_ATTRACTIVE_LED_INIT;
            appData.state = APP_STATE_ERROR;
        }

        while ( 1 )
        {
            /* Maintain Device Drivers. */
            USBTasks( );
            
            /* Maintain the Serial Debug Tasks state machine. */
            APP_SerialDebugTasks( );
        }
    }

    /* Execution should not come here during normal operation. */
    setLedsStatusColor( LED_RED );

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
 */
