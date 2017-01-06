/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using MPLAB(c) Code Configurator
    Implement on OpenFeeder board01

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 3.16
        Device            :  PIC24FJ128GB204
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.26
        MPLAB             :  MPLAB X 3.40
 
  Source of inspiration:
    Project "USB Host - MSD - Simple" from Microchip Libraries for Applications (MLA) 
        C:\microchip\mla\v2016_04_27\apps\usb\host\msd_simple_demo\firmware\exp16_pic24fj128gb204_pim.x\main.c
    Project "minIni" from http://www.compuphase.com/minini.htm
        https://github.com/compuphase/minIni
    Project "RTCC ms"
        C:\github_arnauld\openfeeder\project\demos\pic24\apps\pic24fj128gb204_rtcc\demo_rtcc_mc3.X
    Project "EM4095 read PIT Tag"
        C:\github_arnauld\openfeeder\project\demos\pic24\apps\rfid\em4095_read_tag_id.X
   
   File update:
        ..\framework\usb\usb_host.c
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
 * @brief Development of a test program with PIC24FJ128GB204 MCU on OpenFeeder prototype board.
 * @see [OpenFeeder - Board01](https://docs.google.com/document/d/1MuPXdwcBcdYiER29TY8erCPw7mGYlfRxIeXWazllpBY/edit?usp=sharing)
 * Author: OpenFeeder Team <https://github.com/orgs/OpenFeeder/people> 
 * @version fw01
 * @revision 
 * @date 17/09/2016
 */

/**
 * ------------------------------- Coding style -------------------------------
 * info: http://tdinfo.phelma.grenoble-inp.fr/2Aproj/fiches/coding_styles.pdf
 * 
 * Le passage de paramètres entre les différentes fonctions du sytème se fait
 * par variables globales. Par exemple la commande du servomoteur enter
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
 *  > Boards prototypes of OpenFeeder
 *    https://docs.google.com/document/d/1MuPXdwcBcdYiER29TY8erCPw7mGYlfRxIeXWazllpBY/edit?usp=sharing
 * 
 *  > OpenFeeder prototype v0-1-0 (board detail)
 *    https://docs.google.com/drawings/d/1q4b0Y3H9buA_Q7mJUtJPWDD17gppgqwepQL_1Hjc5_I/edit?usp=sharing
 * 
 *  > PIC24FJ128GB204 resources for proto v0-1-0 (FR: Ressources du PIC24FJ128GB204 pour proto v0-1-0 )
 *    https://docs.google.com/spreadsheets/d/1m1v5Y8z_wb_sX_NdWC2XxA9_l5nrbAusPvYKu8BXbQ0/edit?usp=sharing
 *
 *  > OpenFeeder - Presentation du Hardware (MCU resources)
 *    https://docs.google.com/presentation/d/1Eaiky3VBWBcO7YebayMSayr3wxSXI7rWN3clvQiNmDg/edit?usp=sharing
 */

/** 
 * ------------------ Definition and wiring for this project -------------------
 *  Debugging Interface
 *  Used UART1 at 9600 bauds for communication (9600/8-N-1, see https://en.wikipedia.org/wiki/8-N-1)
 *  see https://fr.wikipedia.org/wiki/RS-232
 *  RC4 --> UART1:U1RX;
 *  RC5 --> UART1:U1TX;
 * 
 *  RFID with EM4095:
 *  . Output RC8 control EM4095 Shutdown, sleep mode control toggle every 161 ms
 *  25.5 ms is necessary for the module EM4095 to be activate, and function during 135 ms
 *  RC8 --> EM4095_SHD (toggle in tmr4.c)
 *  . Use INT4 to capture the falling edge of EM4095 DEMOD_OUT signal
 *  RC6 --> EXT_INT:INT4;
 *  . Use Timer4 to capture the value of EM4095 DEMOD_OUT signal every 442 us
 * 
 *  RGB LED status:
 *  RA9 --> LED_STATUS_R
 *  RA7 --> LED_STATUS_G
 *  RA8 --> LED_STATUS_B
 * 
 *  OC1 to OC3 in PWM mode at 31 kHz (TMR2 at 31,875 us)
 *  pin RC0 to RC2 --> LEDS_RGB_PWM
 *  --> 24-bit RGB
 *  256^3 = 16,777,216 colors (TrueColor)
 *  True color (24-bit) RGB24 color selected in file "CONFIG.INI".
 *  Each color ranges from 0 to 255, digital 8-bit per channel (R, G, B).
 *  Exemple "Red"   RGB triplet: (255, 0, 0) or Hex triplet: #FF0000
 *  Exemple "Green" RGB triplet: (0, 255, 0) or Hex triplet: #00FF00
 *  Exemple "Blue"  RGB triplet: (0, 0, 255) or Hex triplet: #0000FF
 *  https://en.wikipedia.org/wiki/List_of_colors_(compact)
 * 
 *  OC4 in PWM mode at 38 kHz
 *  pin RC3 --> BAR_IR_PWM
 * 
 *  OC5 in PWM mode at 50 Hz
 *  pin RB3 --> CMD_SERVO
 * 
 *  I2C Multiplexage of MCP23017 every 3.744 ms with TMR
 */

/**
 * ------------------------------ Demo OpenFeeder ------------------------------
 * > Interface firmware terminal (Default)
 *   . TODO: send 'M' --> change mode, to confirm the mode change enter 'Y' or 'N' to abort.
 *           --> Default Mode = normal
 *           --> Debug Mode = serial interface for hardware test
 * 
 * > Interface firmware terminal (Debug)
 *   - RTCC module:
 *     . send 'T' --> Read date and time from the RTCC module
 *     . send 'S' --> Set RTCC module date and time (ex: 22/08/2016 15:59:30 --> 22 <CR> 8 <CR> 16 <CR> 15 <CR> 59 <CR> 30 <CR>)
 *   - Attractive RGB LED:
 *     . send 'R' --> Set PWM duty cycle of Red color (0-255, exemple for 50%: 128 <CR>)
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
 *     . send 'E' --> Mesuring RDY/CLK period of EM4095 device
 *   - USB status:
 *     . send 'U': display USB device status 
 * 
 * > PIC24FJ128GB204
 * certaines PIN ne sont pas remappable
 * voir "TABLE 1-3: PIC24FJ128GB204 FAMILY PINOUT DESCRIPTION (CONTINUED)", DS30005009C-page 20
 * 
 * > Mesure RTCC_OUT (pad sous le connecteur J10 "SERVO")
 *   --> Signal carré de l'horloge interne: 1 Hz
 * 
 * > Broches disponibles
 * SPARE1: RB13 --> BAR_IR2_OUT
 * SPARE2: RC9 is 5.5V tolerant (org: CMD_VCC_IR) --> BAR_IR1_OUT
 * SPARE3: RA10 (non remappable) --> OUTPUT, Start High = CMD_VCC_IR (MOSFET P)
 * EM4095_CLK_OUT : RC7 (il faut couper la piste sur la v0.1.0)
 * 
 * > Activer pour chaque capteurs un process dans la machine à états
 *     . "BAR_IR1_OUT": RB5/IOC == low --> coupure de la barrière infra rouge (prise de récompense)
 */

#include "mcc_generated_files/mcc.h"
#include "app.h"


/*
                         Main application

 */

int main( void )
{
    /* Initialize the device. */
    SYSTEM_Initialize( );

    /* Initialize peripheral driver. */
    RFID_Initialize( );
    SERVO_Initialize( );
    IRSensor_Initialize( );

    /* Initialize the file IO system. */
    FILEIO_Initialize( );
    FILEIO_RegisterTimestampGet( GetTimestamp );

    /* Initialize the USB Host API. */
    if ( !USBHostInit( 0 ) )
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_USB_INFO)
        printf( "\nFailure to initialize USB Host API!\n" );
#endif
        doLedsStatusBlink( LEDS_ERROR, 3, 83, 250 );
    }
    else
    {
        doLedsStatusBlink( LED_GREEN, 3, 83, 250 );
    }

    /* Initialize the application. */
    APP_Initialize( );

#if defined (USE_UART1_SERIAL_INTERFACE)
    /* Display information on serial terminal. */
    displayBootMessage( );
#endif



    /* Main loop. */
    while ( 1 )
    {
        /* Maintain Device Drivers. */
        USBTasks( );

        /* Maintain the application's state machine. */
        APP_Tasks( ); /* application specific tasks */
    }

    /* Execution should not come here during normal operation. */
    setLedsStatusColor( LED_RED );
    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
 */
