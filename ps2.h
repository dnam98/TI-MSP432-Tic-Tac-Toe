/*
 * ps2.h
 *
 *  Created on: Oct 5, 2020
 *      Author: Dong Won Nam
 */

#ifndef PS2_H_
#define PS2_H_

extern volatile uint32_t PS2_X_DIR;
extern volatile uint32_t PS2_Y_DIR;

/*
 * Configure ADC14 peripheral
 */
void peripheral_config(void);

/*
 * ADC14 IRQ Handler
 */
void ADC14_IRQHandler(void);

#endif /* PS2_H_ */
