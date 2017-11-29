/******************************************************************************* 
  This is a library for the PCA9622 i2c port expander
  These componant use I2C to communicate, 2 pins are required to interface
 *******************************************************************************
 * 
 * File: pca9622.h  
 * Author: Arnauld Biganzoli
 * Comments:
 * Revision history: 1
 * Datasheet: http://www.nxp.com/products/interfaces/ic-bus-portfolio/ic-led-display-control/16-bit-fm-plus-ic-bus-100-ma-40-v-led-driver:PCA9622
 */

// PCA9622_OE, active LOW Output Enable "OE_PCA" with RE1

#ifndef _PCA9622_HEADER_H
#define	_PCA9622_HEADER_H

/* PCA9622 7-bits slave address, A0 to A6 */
#define PCA9622_ADDRESS     0x01    // => A0 = 1; 0x02 is the 8-bits slave address
#define PCA9622_SWRST_ADR   0x03
#define PCA9622_ALLCALLADR  0xE0    // Default power-up value

/* Software reset data */
#define SWRST_DATA_1        0xA5    // Software reset data byte 1
#define SWRST_DATA_2        0x5A    // Software reset data byte 2

/* Control register - Auto-Increment options */
#define CTRLREG_AUTO_INC_DISABLE 0x00 // no Auto-Increment
#define CTRLREG_AUTO_INC_ENABLE  0x80 // Reset state = Auto-Increment for all registers
    
/* Control register - Register definitions */
#define CTRLREG_MODE1       0x00    // Mode register 1
#define CTRLREG_MODE2       0x01    // Mode register 2
#define CTRLREG_PWM0        0x02    // brightness control LED0
#define CTRLREG_PWM1        0x03    // brightness control LED1
#define CTRLREG_PWM2        0x04    // brightness control LED2
#define CTRLREG_PWM3        0x05    // brightness control LED3
#define CTRLREG_PWM4        0x06    // brightness control LED4
#define CTRLREG_PWM5        0x07    // brightness control LED5
#define CTRLREG_PWM6        0x08    // brightness control LED6
#define CTRLREG_PWM7        0x09    // brightness control LED7
#define CTRLREG_PWM8        0x0A    // brightness control LED8
#define CTRLREG_PWM9        0x0B    // brightness control LED9
#define CTRLREG_PWM10       0x0C    // brightness control LED10
#define CTRLREG_PWM11       0x0D    // brightness control LED11
#define CTRLREG_PWM12       0x0E    // brightness control LED12
#define CTRLREG_PWM13       0x0F    // brightness control LED13
#define CTRLREG_PWM14       0x10    // brightness control LED14
#define CTRLREG_PWM15       0x11    // brightness control LED15
#define CTRLREG_GRPPWM      0x12    // group duty cycle control
#define CTRLREG_GRPFREQ     0x13    // group frequency
#define CTRLREG_LEDOUT0     0x14    // LED output state 0
#define CTRLREG_LEDOUT1     0x15    // LED output state 1
#define CTRLREG_LEDOUT2     0x16    // LED output state 2
#define CTRLREG_LEDOUT3     0x17    // LED output state 3

/* MODE1 - Mode register 1 (address 00h) */
#define NORMAL_MODE         0b10000001  // default value + Normal mode

/* LEDOUT0 to LEDOUT3 - LED driver output state register (address 14h to 17h), page 14 */
#define PCA9622_LDRx_OFF        0b00    // LED driver x is off (default power-up state).
#define PCA9622_LDRx_FULLY_ON   0b01    // LED driver x is fully on (individual brightness and group dimming/blinking not controlled).


/* Function prototype */
I2C1_MESSAGE_STATUS I2C1_MasterWritePCA9622( const uint16_t addr7bits,
                                             uint8_t *p_data,
                                             const uint8_t data_len );

I2C1_MESSAGE_STATUS I2C1_PCA9622_SoftwareReset( void );


#endif	/* _PCA9622_HEADER_H */


/*******************************************************************************
 End of File
 */
