/* _____________________________________________________________________________
 *
 *               P R O J E T    S E L E C T - D E S I G N
 * _____________________________________________________________________________
 *
 * Title            : Reading a button
 * Version          : v01r00
 * Latest Revision  : 29/12/2015
 * Author           : Arnauld BIGANZOLI (AB)
 * Contact          : arnauld.biganzoli@gmail.com
 * Collaborator     : none
 * Company          : SELECT DESIGN
 * Web page         : http://www.select-design.net
 * Hardware         : button with pull-up resistor
 * Processor        : all
 * Tools used       : MPLAB® X IDE v3.15
 * Complier         : Microchip MPLAB XC Compiler
 * Dependencies     : use with Timer ISR routine every (100) ms
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * .:: (en) Change History / (fr) Historique des modifications ::.
 *
 * Rev      Date        Author      Comment
 * ---------------------------------------------------------------------------
 *  1.0     29/12/2015  AB          Initial release
 *
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * .:: (en) Comment / (fr) Remarque ::.
 *
 * . Consider only one button at a time is taken into account in the program
 * . The variable "buttonPressed" will receive the identifier of the recently pressed button
 *   from BUTTON_PRESSED enum {BUTTON_NONE, BUTTON_DOWN, BUTTON_UP, BUTTON_RIGHT, BUTTON_LEFT}
 * . After this, the routine whose function is to scan the state of the buttons must waiting even if state of the buttons was modified
 * . Once the status of the button was read by the program, it affects the BUTTON_READ value to the variable "buttonPressed"
 * . The scan routine can now updated the state of the variable "buttonPressed"
 * 
 * _____________________________________________________________________________
 */

/*******************************************************************************
 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 ******************************************************************************/


#ifndef _BUTTONS_HEADER_H
#define _BUTTONS_HEADER_H

//#include <stdbool.h>


/*** Button Definitions *******************************************************/
//      S1  is MCLR button
//#define S3_PORT  PORTAbits.RA7
//
//#define S3_TRIS  TRISAbits.TRISA7

#define BUTTON_PRESSED       0   /* active state */
#define BUTTON_NOT_PRESSED   1   /* released state */
#define BUTTON_LONG_PRESSED  2   /* active state during more than 2 sec */

//#define PIN_INPUT           1
//#define PIN_OUTPUT          0

typedef enum
{
    BUTTON_NONE, /* return value if no buttons are pressed */
    BUTTON_DOWN,
    BUTTON_UP,
    BUTTON_RIGHT,
    BUTTON_LEFT,
    BUTTON_NUMBER, /* specify the number of button sets */
    BUTTON_READ
} BUTTON;


/*******************************************************************************
 * Function: bool BUTTON_IsPressed(BUTTON button);
 *
 * Overview: Returns the current state of the requested button
 *
 * PreCondition: button configured via BUTTON_SetConfiguration()
 *
 * Input: BUTTON button - enumeration of the buttons available in
 *        this demo.  They should be meaningful names and not the names 
 *        of the buttons on the silkscreen on the board (as the demo 
 *        code may be ported to other boards).
 *         i.e. - ButtonIsPressed(BUTTON_SEND_MESSAGE);
 *
 * Output: TRUE if pressed; FALSE if not pressed.
 *
 *******************************************************************************/
//bool isPressed(BUTTON button);

#endif /* _BUTTONS_HEADER_H */


/*******************************************************************************
 End of File
 */

