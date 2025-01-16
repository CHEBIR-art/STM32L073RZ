/*
 * app_ledblink.c
 *
 *  Created on: Jan 10, 2025
 *      Author: user
 */

#include <app_ledblink.h>
#include <stdint.h>
#include "stm32l073xx.h"

/******SYSTICK TIMER APPLICATION + LED BLINK ************/
void LED_BLINK(void ){

	// DEFINE THE PIN5 MODE AS OUTPUT LED

		GPIOA->MODER &= ~(3U << (2 * 5));  // Clear the mode bits for PA5   //10TH AND 11TH BITS ARE SET TO 0
		GPIOA->MODER |= (1U << (2 * 5));   // Set the mode bits to 01 (Output)// 10TH BIT IS SET TO 1


		GPIOA->ODR ^= GPIO_ODR_OD5;   // LED TOGGLE
		  SYSTICK_Delay(1000);       // DELAY OF 500 ms
		  printf("Timer delay complete\r\n"); // TO TEST
}
