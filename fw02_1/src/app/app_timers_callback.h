/**
 * @file app_timers_callback.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 */

#ifndef APP_TIMERS_CALLBACK_H
#define APP_TIMERS_CALLBACK_H

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

#define TIMER_ISR_TICKER_FACTOR_FOR_160MS            6124 /* 160 ms */

/* Param Fq RDY/CLK. */
// #define DECODING_RFID_INTERRUPT_TICKER_FACTOR    16 // avec 510-515 us de periode pour DEMOD_OUT (RDY/CLK = 125 kHz --> fonctionne mieux a 125.8 kHz)
// Pour autre antenne fonctionne a partir de 126.8 kHz
// Measure DMOD_OUT periode = 480 us
// --> on observe le signal DMOD_OUT retourne par le module EM4095 en fonction du signal RF que celui recoit en presence du Tag
// Commence a fonctionner avec 490 us de periode pour DEMOD_OUT (RDY/CLK = 131.5 kHz)
#define DECODING_RFID_THREE_QUARTER_PERIODE_READING 14 // --> ok avec 475 us de periode pour DEMOD_OUT (RDY/CLK = 134 kHz)
//#define DECODING_RFID_HALF_PERIODE_READING          7 // 16 / 2
#define DECODING_RFID_QUARTER_PERIODE_READING       4 // 16 / 4


// *****************************************************************************
// *****************************************************************************
// Section: extern declarations
// *****************************************************************************
// *****************************************************************************

extern volatile uint16_t g_rdyclk_count_in_10ms;
extern volatile uint16_t g_timeout_x20ms;
extern volatile uint16_t g_timeout_standby_x20ms;
extern volatile uint16_t g_timeout_em4095_x20ms;
extern volatile uint16_t g_timeout_leds_status_x20ms;
extern volatile uint8_t g_timeout_taking_reward;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
 */
void TMR2_CallBack( void );
void TMR3_CallBack( void );
void TMR4_CallBack( void );


#endif /* APP_TIMERS_CALLBACK_H */

/*******************************************************************************
 End of File
 */
