/*
 * timer32.h
 *
 *  Created on: Oct 5, 2020
 *      Author: Dong Won Nam
 */

#ifndef TIMER32_H_
#define TIMER32_H_

#include "msp.h"
#include <stdbool.h>
#include <stdint.h>

extern volatile bool ALERT_S2_PRESSED;
extern volatile bool ALERT_LCD_UPDATE;

/*
 * Configure Timer32_1 to generate interrupt every 10ms
 *
 */
void T32_1_Interrupt_10ms(void);

/*
 * Configure Timer32_1 to generate interrupt every 100ms
 *
 */
void T32_2_Interrupt_100ms(void);

/*
 * Timer32 IRQ handlers
 */
void T32_INT1_IRQHandler();
void T32_INT2_IRQHandler();

#endif /* TIMER32_H_ */
