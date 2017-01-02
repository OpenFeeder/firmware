/* ____________________________________________________________________________
 *
 *						Conversion d'un Nbr en U, D et C
 * ____________________________________________________________________________
 *
 * Nom  : Extraction_Nbr
 *
 * Description : permet de realiser l'extraction d'une partie d'un nombre (unite, dizaine ou centaine)
 *
 * Parametre d'entree :
 *	. nbr dont on souhaite extraction
 *	. partie de l'extraction souhaite (UNITE, DIZAINE, CENTAINE)
 *
 * Parametre de sortie :
 *	. retourne la partie extraite, unite, dizaine ou centaine
 *
 */


#include <stdint.h>
#include <stdbool.h>
#include "digits.h"

uint8_t blinkDigit = DIGIT_1; // globale variable
bool blinkDigitState = ON; // globale variable for BLINK mode only
bool blinkDigitEnable = false; // globale variable for BLINK mode only
//bool blinkDigitStatus; // ENABLE  or DISABLE

/* Seven-Segment Display (SSD), Digit: gfedcba */

// Tableau de caractères pour afficheur 7 segments à cathodes communes
// Chiffre DP G F E D C B A   BIN         HEX
//    0     0 0 1 1 1 1 1 1   0b00111111  0x3F
//    1     0 0 0 0 0 1 1 0   0b00000110  0x06
//    2     0 1 0 1 1 0 1 1   0b01011011  0x5B
//    3     0 1 0 0 1 1 1 1   0b01001111  0x4F
//    4     0 1 1 0 0 1 1 0   0b01100110  0x66
//    5     0 1 1 0 1 1 0 1   0b01101101  0x6D
//    6     0 1 1 1 1 1 0 1   0b01111101  0x7D
//    7     0 0 0 0 0 1 1 1   0b00100111  0x07
//    8     0 1 1 1 1 1 1 1   0b01111111  0x7F
//    9     0 1 1 0 1 1 1 1   0b01101111  0x6F
//    A     0 1 1 1 0 1 1 1   0b01110111  0x77
//    B     0 1 1 1 1 1 0 0   0b01111100  0x7C
//    C     0 0 1 1 1 0 0 1   0b00111001  0x39
//    D     0 1 0 1 1 1 1 0   0b01011110  0x5E
//    E     0 1 1 1 1 0 0 1   0b01111001  0x79
//    F     0 1 1 1 0 0 0 1   0b01110001  0x71

// Tableau de caractères pour afficheur 7 segments à anodes communes
// Chiffre DP G F E D C B A   BIN         HEX
//    0     1 1 0 0 0 0 0 0   0b11000000  0xC0
//    1     1 1 1 1 1 0 0 1   0b11111001  0xF9
//    2     1 0 1 0 0 1 0 0   0b10100100  0xA4
//    3     1 0 1 1 0 0 0 0   0b10110000  0xB0
//    4     1 0 0 1 1 0 0 1   0b10011001  0x99
//    5     1 0 0 1 0 0 1 0   0b10010010  0x92
//    6     1 0 0 0 0 0 1 0   0b10000010  0x82
//    7     1 1 1 1 1 0 0 0   0b11111000  0xF8
//    8     1 0 0 0 0 0 0 0   0b10000000  0x80
//    9     1 0 0 1 0 0 0 0   0b10010000  0x90
//    A     1 0 0 0 1 0 0 0   0b10001000  0x88
//    B     1 0 0 0 0 0 1 1   0b10000011  0x83
//    C     1 1 0 0 0 1 1 0   0b11000110  0xC6
//    D     1 0 1 0 0 0 0 1   0b10100001  0xA1
//    E     1 0 0 0 0 1 1 0   0b10000110  0x86
//    F     1 0 0 0 1 1 1 0   0b10001110  0x8E

// Display numerical Hexa value (0 to F) on seven segment display.
const unsigned char charHex_Digit7Seg_CA[] = {
                                              0xC0, // '0'; Hex: 0x00
                                              0xF9, // '1'; Hex: 0x01
                                              0xA4, // '2'; Hex: 0x02
                                              0xB0, // '3'; Hex: 0x03
                                              0x99, // '4'; Hex: 0x04
                                              0x92, // '5'; Hex: 0x05
                                              0x82, // '6'; Hex: 0x06
                                              0xF8, // '7'; Hex: 0x07
                                              0x80, // '8'; Hex: 0x08
                                              0x90, // '9'; Hex: 0x09
                                              0x88, // 'A'; Hex: 0x0A
                                              0x83, // 'b'; Hex: 0x0B
                                              0xC6, // 'C'; Hex: 0x0C
                                              0xA1, // 'd'; Hex: 0x0D
                                              0x86, // 'E'; Hex: 0x0E
                                              0x8E // 'F'; Hex: 0x0F
};

// Display ASCII value on seven segment display
// https://en.wikipedia.org/wiki/ASCII
// http://www.uize.com/examples/seven-segment-display.html
// can be optimised by substact 0x20 to a ASCII char befor adress to this tab (suppress 32 bytes of this tab)
const uint8_t charASCII_Digit7Seg_CA[] = {
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // x16 no printable ASCII code chart
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // x16 no printable ASCII code chart
                                          0b11111111, // Glyph: ' '; Hex: 0x20
                                          0b11111001, // Glyph: '!'; Hex: 0x21
                                          0b11111101, // Glyph: '"'; Hex: 0x22
                                          0b10100011, // Glyph: '#'; Hex: 0x23
                                          0b10010010, // Glyph: '$'; Hex: 0x24
                                          0b10101101, // Glyph: '%'; Hex: 0x25
                                          0b10000000, // Glyph: '&'; Hex: 0x26
                                          0b11111101, // Glyph: '''; Hex: 0x27
                                          0b11000110, // Glyph: '('; Hex: 0x28
                                          0b11110000, // Glyph: ')'; Hex: 0x29
                                          0b10011100, // Glyph: '*'; Hex: 0x2A --> '°'
                                          0b10111001, // Glyph: '+'; Hex: 0x2B
                                          0b01111111, // Glyph: ','; Hex: 0x2C
                                          0b10111111, // Glyph: '-'; Hex: 0x2D
                                          0b01111111, // Glyph: '.'; Hex: 0x2E
                                          0b10101101, // Glyph: '/'; Hex: 0x2F
                                          0b11000000, // Glyph: '0'; Hex: 0x30
                                          0b11111001, // Glyph: '1'; Hex: 0x31
                                          0b10100100, // Glyph: '2'; Hex: 0x32
                                          0b10110000, // Glyph: '3'; Hex: 0x33
                                          0b10011001, // Glyph: '4'; Hex: 0x34
                                          0b10010010, // Glyph: '5'; Hex: 0x35
                                          0b10000010, // Glyph: '6'; Hex: 0x36
                                          0b11111000, // Glyph: '7'; Hex: 0x37
                                          0b10000000, // Glyph: '8'; Hex: 0x38
                                          0b10010000, // Glyph: '9'; Hex: 0x39
                                          0b10110111, // Glyph: ':'; Hex: 0x3A
                                          0b10110111, // Glyph: ';'; Hex: 0x3B
                                          0b11000110, // Glyph: '<'; Hex: 0x3C
                                          0b10110111, // Glyph: '='; Hex: 0x3D
                                          0b11110000, // Glyph: '>'; Hex: 0x3E
                                          0b10101100, // Glyph: '?'; Hex: 0x3F
                                          0b10000100, // Glyph: '@'; Hex: 0x40
                                          0b10001000, // Glyph: 'A'; Hex: 0x41
                                          0b10000011, // Glyph: 'B'; Hex: 0x42
                                          0b11000110, // Glyph: 'C'; Hex: 0x43
                                          0b10100001, // Glyph: 'D'; Hex: 0x44
                                          0b10000110, // Glyph: 'E'; Hex: 0x45
                                          0b10001110, // Glyph: 'F'; Hex: 0x46
                                          0b11000010, // Glyph: 'G'; Hex: 0x47
                                          0b10001001, // Glyph: 'H'; Hex: 0x48
                                          //                                              0b11111001, // Glyph: 'I'; Hex: 0x49
                                          0b11001111, // Glyph: 'I'; Hex: 
                                          0b11100001, // Glyph: 'J'; Hex: 0x4A
                                          0b10001011, // Glyph: 'K'; Hex: 0x4B
                                          0b11000111, // Glyph: 'L'; Hex: 0x4C
                                          0b11001000, // Glyph: 'M'; Hex: 0x4D
                                          0b10101011, // Glyph: 'N'; Hex: 0x4E
                                          0b11000000, // Glyph: 'O'; Hex: 0x4F
                                          0b10001100, // Glyph: 'P'; Hex: 0x50
                                          0b10011000, // Glyph: 'Q'; Hex: 0x51
                                          0b10101111, // Glyph: 'R'; Hex: 0x52
                                          0b10010010, // Glyph: 'S'; Hex: 0x53
                                          0b10000111, // Glyph: 'T'; Hex: 0x54
                                          0b11000001, // Glyph: 'U'; Hex: 0x55
                                          0b11000001, // Glyph: 'V'; Hex: 0x56
                                          0b11000001, // Glyph: 'W'; Hex: 0x57
                                          0b10001001, // Glyph: 'X'; Hex: 0x58
                                          0b10010001, // Glyph: 'Y'; Hex: 0x59
                                          0b10100100, // Glyph: 'Z'; Hex: 0x5A
                                          0b11000110, // Glyph: '['; Hex: 0x5B
                                          0b10011011, // Glyph: '\'; Hex: 0x5C
                                          0b11110000, // Glyph: ']'; Hex: 0x5D
                                          0b11011100, // Glyph: '^'; Hex: 0x5E
                                          0b11110111, // Glyph: '_'; Hex: 0x5F
                                          0b11111111, // Glyph: '`'; Hex: 0x60
                                          0b10100000, // Glyph: 'a'; Hex: 0x61
                                          0b10000011, // Glyph: 'b'; Hex: 0x62
                                          0b10100111, // Glyph: 'c'; Hex: 0x63
                                          0b10100001, // Glyph: 'd'; Hex: 0x64
                                          0b10000100, // Glyph: 'e'; Hex: 0x65
                                          0b10001110, // Glyph: 'f'; Hex: 0x66
                                          0b10010000, // Glyph: 'g'; Hex: 0x67
                                          0b10001011, // Glyph: 'h'; Hex: 0x68
                                          0b11111011, // Glyph: 'i'; Hex: 0x69
                                          0b11110001, // Glyph: 'j'; Hex: 0x6A
                                          0b10001011, // Glyph: 'k'; Hex: 0x6B
                                          0b11000111, // Glyph: 'l'; Hex: 0x6C
                                          0b10101011, // Glyph: 'm'; Hex: 0x6D
                                          0b10101011, // Glyph: 'n'; Hex: 0x6E
                                          0b10100011, // Glyph: 'o'; Hex: 0x6F
                                          0b10001100, // Glyph: 'p'; Hex: 0x70
                                          0b10011000, // Glyph: 'q'; Hex: 0x71
                                          0b10101111, // Glyph: 'r'; Hex: 0x72
                                          0b10010010, // Glyph: 's'; Hex: 0x73
                                          0b10000111, // Glyph: 't'; Hex: 0x74
                                          0b11100011, // Glyph: 'u'; Hex: 0x75
                                          0b11100011, // Glyph: 'v'; Hex: 0x76
                                          0b11100011, // Glyph: 'w'; Hex: 0x77
                                          0b10001001, // Glyph: 'x'; Hex: 0x78
                                          0b10010001, // Glyph: 'y'; Hex: 0x79
                                          0b10100100, // Glyph: 'z'; Hex: 0x7A
                                          0b11000110, // Glyph: '{'; Hex: 0x7B
                                          0b11001111, // Glyph: '|'; Hex: 0x7C
                                          0b11110000, // Glyph: '}'; Hex: 0x7D
                                          0b10111111, // Glyph: '~'; Hex: 0x7E
                                          0b00000000 // 0x7F (127 dec)
};

unsigned char convValueToDigit7Seg( unsigned char value )
{
    if ( value < 0x10 )
    {
        return charHex_Digit7Seg_CA[value];
    }
    //    return 0b10111111; // if value >= 0x10 then return '-'
    return charHex_Digit7Seg_CA[0x7E]; // if value >= 0x10 then return '-'
}

///**
// * Tableau de conversion
// * permet la generation de caracteres pour un afficheur 7 segments
// */
//uint8_t conv_nbr_Char7Seg(uint8_t nombre)
//{
//    switch (nombre)
//    {
//        case 0: return (Num_0);
//        case 1: return (Num_1);
//        case 2: return (Num_2);
//        case 3: return (Num_3);
//        case 4: return (Num_4);
//        case 5: return (Num_5);
//        case 6: return (Num_6);
//        case 7: return (Num_7);
//        case 8: return (Num_8);
//        case 9: return (Num_9);
//        default: return (Symbole_moin);
//    }
//}

/* Extraction d'un Nbr (de 0 a 255) en 1 Nbr (centaine, dizaine, unite) */
unsigned char Extraction_Nbr( unsigned char NbrToConvert, unsigned char Indice )
{
    unsigned char temp = NbrToConvert;
    unsigned char unite = 0;
    unsigned char dizaine = 0;
    unsigned char centaine = 0;

    while ( temp != 0 )
    {
        temp--;
        unite++;
        if ( unite == 10 )
        {
            unite = 0;
            dizaine++;
            if ( dizaine == 10 )
            {
                dizaine = 0;
                centaine++;
            }
        }
    }

    switch ( Indice )
    {
        case UNITE:
            return unite;

        case DIZAINE:
            return dizaine;

        case CENTAINE:
            return centaine;

        default:
            return 0;
    }
} /* Local EOF: Extraction_Nbr */

void displayDigitBlink( uint8_t whichDigitPosition )
{
    // TODO: makeDigitBlink( )
    //    printf( "displayDigitBlink( )\n" );
    blinkDigit = whichDigitPosition;
    blinkDigitEnable = true;
}

void displayDigitFixe( void )
{
    blinkDigitEnable = false;
}


/*******************************************************************************
 End of File
 */
