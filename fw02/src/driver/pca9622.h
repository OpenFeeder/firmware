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

// TODO :
// PCA9622_OE, active LOW output enable "OE_PCA" => RE1

#ifndef XC_HEADER_PCA9622_H
#define	XC_HEADER_PCA9622_H

/* PCA9622 7-bits slave address, A0 to A6 */
#define PCA9622_ADDRESS     0x01    // => A0 = 1; 0x02 is the 8-bits slave address
#define PCA9622_ALLCALLADR  0xE0    // Default power-up value
#define PCA9622_ALLCALLADR  0xE0    // Default power-up value
#define PCA9622_SWRST_ADR   0x03

/* Control register */
//#define PCA9622_AUTO_INC    0x00
#define PCA9622_RST_STATE   0x80    // Reset state = Auto-Increment

/* Register definitions */
#define PCA9622_CTRLREG_MODE1       0x00    // Mode register 1
#define PCA9622_CTRLREG_MODE2       0x01    // Mode register 2
#define PCA9622_CTRLREG_PWM0        0x02    // brightness control LED0
#define PCA9622_CTRLREG_PWM1        0x03    // brightness control LED1
//...
#define PCA9622_CTRLREG_GRPPWM      0x12    // group duty cycle control
#define PCA9622_CTRLREG_GRPFREQ     0x13    // group frequency
#define PCA9622_CTRLREG_LEDOUT0     0x14    // LED output state 0

/* MODE1 - Mode register 1 (address 00h) */
#define PCA9622_VAL_MODE1           0b10000001    // default value + Normal mode

/* LEDOUT0 to LEDOUT3 - LED driver output state register (address 14h to 17h), page 14 */
#define PCA9622_LDRx_FULLY_ON       0x01    // LED driver x is fully on (individual brightness and group dimming/blinking not controlled)


/* Function prototype */
I2C1_MESSAGE_STATUS I2C1_MasterWritePCA9622( const uint16_t addr7bits,
                                             uint8_t *p_data,
                                             const uint8_t data_len );


#endif	/* XC_HEADER_PCA9622_H */

/*******************************************************************************
 End of File
 */
