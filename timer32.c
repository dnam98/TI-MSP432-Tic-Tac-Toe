/*
 * timer32.c
 *
 *  Created on: Oct 5, 2020
 *      Author: Dong Won Nam
 */

#include "timer32.h"

volatile bool ALERT_S2_PRESSED = false;
volatile bool ALERT_LCD_UPDATE = false;

/*
 * Configure Timer32_1 to generate interrupt every 10ms
 *
 */
void T32_1_Interrupt_10ms(void)
{
    // ticks = desired period / core clock period
    uint32_t ticks = ((SystemCoreClock * 10) / 1000) - 1;

    // Stop the timer
    TIMER32_1->CONTROL = 0;

    // Set the load register
    TIMER32_1->LOAD = ticks;

    // Enable the Timer32 interrupt in NVIC
    __enable_irq(); // turns on the NVIC interrupt
    NVIC_EnableIRQ(T32_INT1_IRQn);  // enabled interrupt in timer32
    NVIC_SetPriority(T32_INT1_IRQn, 0); // Timer32 has the highest priority

    // Start Timer32 and enable interrupt
    TIMER32_1->CONTROL = TIMER32_CONTROL_ENABLE |   // turn timer on
            TIMER32_CONTROL_MODE |   // periodic mode
            TIMER32_CONTROL_SIZE | // 32 bit timer
            TIMER32_CONTROL_IE;    // enable interrupts
}

/*
 * Configure Timer32_2 to generate interrupt every 100ms
 *
 */
void T32_2_Interrupt_100ms(void)
{
    // ticks = desired period / core clock period
    uint32_t ticks = ((SystemCoreClock * 100) / 1000) - 1;

    // Stop the timer
    TIMER32_2->CONTROL = 0;

    // Set the load register
    TIMER32_2->LOAD = ticks;

    // Enable the Timer32 interrupt in NVIC
    __enable_irq(); // turns on the NVIC interrupt
    NVIC_EnableIRQ(T32_INT2_IRQn);  // enabled interrupt in timer32
    NVIC_SetPriority(T32_INT2_IRQn, 0); // Timer32 has the highest priority

    // Start Timer32 and enable interrupt
    TIMER32_2->CONTROL = TIMER32_CONTROL_ENABLE |   // turn timer on
            TIMER32_CONTROL_MODE |   // periodic mode
            TIMER32_CONTROL_SIZE | // 32 bit timer
            TIMER32_CONTROL_IE;    // enable interrupts
}

/*
 * Timer32_1 IRQ handler
 *
 */
void T32_INT1_IRQHandler()
{
    // Button pressed for at least 70ms
    static uint8_t button_state = 0x00;
    bool button_pressed = false;

    if ((P3->IN & BIT5 ) == 0)
    {
        button_pressed = true;
    }
    else
    {
        button_pressed = false;
    }

    // Debounce push button
    button_state = button_state << 1;

    if (button_pressed)
    {
        button_state |= BIT0;
    }

    // Check for debounce of 7 times
    if (button_state == 0x7F)
    {
        ALERT_S2_PRESSED = true;
    }

    // Start an ADC Conversion
    ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;

    // Clear the Timer32 interrupt
    TIMER32_1->INTCLR = BIT0;
}

/*
 * Timer32_2 IRQ handler
 *
 */
void T32_INT2_IRQHandler()
{
    // Global variable to indicate LCD screen can be updated
    ALERT_LCD_UPDATE = true;

    // Clear the Timer32 interrupt
    TIMER32_2->INTCLR = BIT0;
}

