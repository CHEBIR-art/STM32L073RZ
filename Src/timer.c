/*
 * timer.c
 *
 *  Created on: Nov 21, 2024
 *      Author: Khouloud
 */
#include "stm32l073xx.h"
#include "utilities.h"
#include <stdio.h>

void Timer2_Init(void) {

    // TIMER2 CLOCK ENABLE

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 2099; // BECAUSE THE SYSTEM CLOCK IS  2.101 MHz
    TIM2->ARR = 0xFFFF ; // Valeur maximale pour un compteur 16 bits  0xFFFF = 2^16
    TIM2->CNT = 0; // Reinitialization of the counter
    TIM2->CR1 &= ~TIM_CR1_CEN; //
}


// FUNCTION TO CREATE A DELAY OF 1ms


void Timer2_Delay_ms(uint32_t delay) {
    // ARR CONFIGURATION FOR THE DALEY NEEDED
    TIM2->ARR = delay;

    // INITIALIZATION OF THE COUNTER
    TIM2->CNT = 0;

    // TIMER ENABLE/ COUNTER ENABLE
    TIM2->CR1 |= TIM_CR1_CEN;

    // WAIT FOR THE TIMER TO REACH THE ARR VALUE /  ARR (UIF = Update Interrupt Flag)
    while (!(TIM2->SR & TIM_SR_UIF));

    // DISABLE THE TIMER
    TIM2->CR1 &= ~TIM_CR1_CEN;

    //  RESET THE INTERRUPT FLAG

    TIM2->SR &= ~TIM_SR_UIF;
}
/**********TIMER TEST********/

 void timer_test(void){
		Timer2_Delay_ms(10000); // 1-second delay              //************SUCCESS
	    printf("Timer delay complete\r\n");
 }
