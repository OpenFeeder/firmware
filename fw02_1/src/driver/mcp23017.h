/******************************************************************************* 
  This is a library for the MCP23017 i2c port expander
  These componant use I2C to communicate, 2 pins are required to interface
 *******************************************************************************
 * 
 * File: mcp23017.h  
 * Author: Arnauld Biganzoli
 * Comments:
 * Revision history: 1
 * 
 */


#ifndef _MCP23017_HEADER_H
#define	_MCP23017_HEADER_H

#include "i2c1.h"

// MCP23017 7-bits slave address
#define MCP23017_ADDRESS    0x20  // and 0x40 is the 8-bits slave addresss

/* BANK = 0 */
// PORTA registers
#define MCP23017_IODIRA     0x00
#define MCP23017_IPOLA      0x02
#define MCP23017_GPINTENA   0x04
#define MCP23017_DEFVALA    0x06
#define MCP23017_INTCONA    0x08
#define MCP23017_IOCONA     0x0A
#define MCP23017_GPPUA      0x0C
#define MCP23017_INTFA      0x0E
#define MCP23017_INTCAPA    0x10
#define MCP23017_GPIOA      0x12
#define MCP23017_OLATA      0x14

// PORTB registers
#define MCP23017_IODIRB     0x01
#define MCP23017_IPOLB      0x03
#define MCP23017_GPINTENB   0x05
#define MCP23017_DEFVALB    0x07
#define MCP23017_INTCONB    0x09
#define MCP23017_IOCONB     0x0B
#define MCP23017_GPPUB      0x0D
#define MCP23017_INTFB      0x0F
#define MCP23017_INTCAPB    0x11
#define MCP23017_GPIOB      0x13
#define MCP23017_OLATB      0x15

#define MCP23017_INT_ERR    255

#define OUTPUT              0
#define INPUT               1

/**
 * MCP23017 hardware on OpenFeeder
 * -------------------------------
 * GPA[0..7] --> SEVEN_SEGMENT[a, b, c, d, e, f, g, dp]
 * GPB[0]    --> CMD_SEVEN_SEGMENT_DIGIT_1
 * GPB[1]    --> CMD_SEVEN_SEGMENT_DIGIT_2
 * GPB[2]    --> CMD_SEVEN_SEGMENT_DIGIT_3
 * GPB[3]    --> CMD_SEVEN_SEGMENT_DIGIT_4
 * GPB[4]    <-- BUTTON_DOWN
 * GPB[5]    <-- BUTTON_UP
 * GPB[6]    <-- BUTTON_RIGHT
 * GPB[7]    <-- BUTTON_LEFT  
 */
/* Button mask in */
#define BUTTON_DOWN_MASK    0b00010000
#define BUTTON_UP_MASK      0b00100000
#define BUTTON_RIGHT_MASK   0b01000000
#define BUTTON_LEFT_MASK    0b10000000
//#define BUTTON_LEFT_MASK    0b01000000
//#define BUTTON_RIGHT_MASK   0b10000000


// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype: I2C1_MESSAGE_STATUS I2C1_MasterWriteMCP23017(const uint16_t addr7bits, uint8_t *p_data, const uint8_t data_len);</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
I2C1_MESSAGE_STATUS I2C1_MasterWriteMCP23017( const uint16_t addr7bits,
                                              uint8_t *p_data,
                                              const uint8_t data_len );

I2C1_MESSAGE_STATUS I2C1_MasterReadMCP23017( const uint16_t addr7bits,
                                             const uint8_t register_addr,
                                             uint8_t *p_data_read,
                                             const uint8_t number_of_data_to_read );

void i2cScanner( void );


#endif	/* _MCP23017_HEADER_H */


/*******************************************************************************
 End of File
 */
