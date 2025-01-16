/*
 * systick.c
 *
 *  Created on: Jan 9, 2025
 *      Author: user
 */


#include <stdint.h>
#include "stm32l073xx.h"
#include "systick.h"

uint32_t SystemCoreClock = 16000000;
uint32_t ticks = 0;

//////////////////////////////////////////////
////////////// SYSTICK TIMER /////////////////
//////////////////////////////////////////////


void SysTick_Handler(void) {
    ticks++;
}

void SYSTICK_Init(void){
	SysTick_Config(SystemCoreClock / 1000);
}

/**
 * Millisecond delays with Systick Timer, blocking function
 * @param delay : milliseconds to wait
 */
void SYSTICK_Delay(uint32_t Delay)
{
	  uint32_t tickstart = SYSTICK_Get();

	  while((SYSTICK_Get() - tickstart) < Delay);
}

uint32_t SYSTICK_Get(void){
	return ticks;
}



