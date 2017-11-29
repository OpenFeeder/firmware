/*_____________________________________________________________________________
 *
 *       		R O U T I N E S    D E    M U L T I P L E X A G E
 *_____________________________________________________________________________
 *
 * Titre		 : Mise en oeuvre du multiplexage pour la gestion d'afficheurs 7 segments
 * Version		 : 01
 * Date			 : 
 * Auteur		 : Arnauld BIGANZOLI
 *
 *
 * Description :
 * ------------
 * conv_nbr_en_udc(nombre);	=> permet de réaliser un décodage d'un nbr en unité, dizaine et centaine
 *	_ Paramètre de sortie : UNITE, DIZAINE, CENTAINE
 *
 * conv_nbr_Char7Seg(nombre); => Cette routine permet de convertire un nombre de 0 à 9 en caractère affichable par un Aff 7 segments
 *	_ Paramètre de sortie : Char7Seg
 *
 *
 * Note version 01 :
 * ----------------
 *
 */

#ifndef _DIGITS_HEADER_H
#define _DIGITS_HEADER_H

// Uncomment between the two types of LED 7-segment displays :
//#define DIGIT7SEG_CC    // Common Cathode(CC)
#define DIGIT7SEG_CA    // Common Anode(CA)


/*
 *------------------------------------------------------------------------------
 *                 ASSIGNATIONS POUR LE CIRCUIT MCP23017
 *------------------------------------------------------------------------------
 */

/*	Représentation des digits associés au hardware de la carte "I2C Interface" :
 *	---------------------------------------------------------------------------
 *
 *	      ||||||||              ||||||||              ||||||||              ||||||||
 *	     |        |            |        |            |        |            |        |
 *	     |        |            |        |            |        |            |        |
 *	      ||||||||              ||||||||              ||||||||              ||||||||
 *	     |        |            |        |            |        |            |        |
 *	     |        |            |        |            |        |            |        |
 *	      ||||||||              ||||||||              ||||||||              ||||||||
 *
 *        Digit_4       |       Digit_3       |       Digit_2       |       Digit_1
 */

// Value to write on MCP23017 PORTB for display each digit :
#define CLEAR_DIGITS    0       // clear all digits
//#define DIGIT_4     0x01    // commande digit 4
//#define DIGIT_3		0x02    // commande digit 3
//#define DIGIT_2		0x04    // commande digit 2
//#define DIGIT_1		0x08    // commande digit 1
#define DIGIT_1     0x01    /* commande digit 1, the value must be 1 to reset shift register */
#define DIGIT_2		0x02    /* commande digit 2 */
#define DIGIT_3		0x04    /* commande digit 3 */
#define DIGIT_4		0x08    /* commande digit 4 */
#define ALL_DIGIT   0xFF    /* commande all digits */


/* Configuration des DIGITS 1 et 2:
 *	 '76543210'
 *	B'00000000'		-> Initialisation de l'afficheur 7 segments à 0
 *						bit<7>= segment dp
 *						bit<6>= segment g
 *						bit<5>= segment f
 *						bit<4>= segment e
 *						bit<3>= segment d
 *						bit<2>= segment c
 *						bit<1>= segment b
 *						bit<0>= segment a
 */

/*** Tableau de caractères ***/
// ASCII printable code chart
// https://en.wikipedia.org/wiki/ASCII
#ifdef DIGIT7SEG_CC
// Tableau de caractères pour afficheur 7 segments à cathodes communes
//				    	'DPgfedcba'
#define Num_0			0b00111111		// "0"
#define Num_1			0b00000110		// "1"
#define Num_2			0b01011011		// "2"
#define Num_3			0b01001111		// "3"
#define Num_4			0b01100110		// "4"
#define Num_5			0b01101101		// "5"
#define Num_6			0b01111101		// "6"
#define Num_7			0b00100111		// "7"
#define Num_8			0b01111111		// "8"
#define Num_9			0b01101111		// "9"
#define Char_A	 		0b01110111		// "A"
#define Char_B			0b01111100		// "b"
#define Char_sc			0b01011000		// "c" scripte ou small
#define Char_C			0b00111001		// "C"
#define Char_d			0b01011110		// "d"
#define Char_E			0b01111001		// "E"
#define Char_F			0b01110001		// "F"
#define Char_G			0b00111101		// "G"
#define Char_H			0b01110110		// "H"
#define Char_si			0b00000100		// "i" scripte ou small
#define Char_I			0b00001111		// "I"
#define Char_J			0b00011110		// "J"
#define Char_L			0b00111000		// "L"
#define Char_M			0b00110111		// "M"
#define Char_n			0b01010100		// "n"
#define Char_o			0b01011100		// "o"
#define Char_P			0b01110011		// "P"
#define Char_S			0b01101101		// "S"
#define Char_t			0b01111000		// "t"
#define Char_U			0b00111110		// "U"
#define Char_Nul		0b00000000		// " "
#define Symbole_moin	0b01000000		// "-"
#define Symbole_degre	0b01100011		// "°"
#define Char_1			0b00110000		// "1"
#define Char_11			0b00110110		// "11"
#endif

#ifdef DIGIT7SEG_CA
extern const unsigned char charHex_Digit7Seg_CA[];
extern const unsigned char charASCII_Digit7Seg_CA[];
#endif

#define UNITE		2
#define DIZAINE		1
#define CENTAINE	0

//#define DIGIT_FIXE  0
//#define DIGIT_BLINK 1

#define OFF         0
#define ON          1

/*  */
//union
//{
//    uint8_t blink_mode_reg;
//
//    struct
//    {
//        unsigned cmd_digits : 4;
//        unsigned : 3;
//        unsigned initialized : 1;
//    } blink_mode_bit;
//} blink_mode;

extern uint8_t blinkDigit;
extern bool blinkDigitEnable; // ENABLE == true or DISABLE == false
extern bool blinkDigitState; // ON or OFF


/* ____________________________________________________________________________
 **
 **                            Function Prototype
 ** ____________________________________________________________________________
 */

unsigned char convValueToDigit7Seg( unsigned char value );
//unsigned char conv_nbr_Char7Seg(unsigned char nombre);
unsigned char Extraction_Nbr( unsigned char NbrToConvert, unsigned char Indice ); /* Extraction d'une partie d'un Nbr (unite, dizaine, centaine)		*/

void displayDigitBlink( uint8_t whichDigit );
void displayDigitFixe( void );

#endif /* _DIGITS_HEADER_H */


/*******************************************************************************
 End of File
 */
