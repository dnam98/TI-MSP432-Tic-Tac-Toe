/*
 * ps2.c
 *
 *  Created on: Oct 5, 2020
 *      Author: Dong Won Nam
 */

#include "msp.h"
#include "ps2.h"

volatile uint32_t PS2_X_DIR = 0;
volatile uint32_t PS2_Y_DIR = 0;


/*
 * Configure ADC14 peripheral
 */
void peripheral_config(void)
{
    // Set ADC, X and Y direction of the joystick
    // Configure the X direction as an analog input pin.
    P6->SEL0 |= BIT0;
    P6->SEL1 |= BIT0;

    // Configure the Y direction as an analog input pin.
    P4->SEL0 |= BIT4;
    P4->SEL1 |= BIT4;

    // Configure CTL0 to sample 16-times in pulsed sample mode.
    ADC14->CTL0 = ADC14_CTL0_SHP | ADC14_CTL0_SHT02;

    // Indicate that this is a sequence-of-channels.
    ADC14->CTL0 |= ADC14_CTL0_CONSEQ_1;

    // Configure ADC to return 12-bit values
    ADC14->CTL1 = ADC14_CTL1_RES_2;

    // Associate the X direction analog signal with MEM[0]
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_15;

    // Associate the Y direction analog signal with MEM[1]
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_9;

    // Indicate this is the end of a sequence.
    ADC14->MCTL[1] |= ADC14_MCTLN_EOS;

    // Enable interrupts in the ADC AFTER a value is written into MEM[1].
    ADC14->IER0 = ADC14_IER0_IE1;

    // Enable ADC Interrupt in the NVIC
    NVIC_EnableIRQ(ADC14_IRQn);

    // Turn ADC ON
    ADC14->CTL0 |= ADC14_CTL0_ON;
}

/*
 * ADC14 IRQ Handler
 */
void ADC14_IRQHandler(void)
{
    // Read the X value
    PS2_X_DIR = ADC14->MEM[0];

    // Read the Y value
    PS2_Y_DIR = ADC14->MEM[1];
}
