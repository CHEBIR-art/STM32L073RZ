/*
 * mco.c
 *
 *  Created on: Jan 10, 2025
 *      Author: user
 */
#include <stdint.h>
#include "stm32l073xx.h"
#include "mco.h"

 void MCO1_CONFIG(void){


	 // Set PA8 to Alternate Function mode
	 GPIOA->MODER &= ~(GPIO_MODER_MODE8); // Clear MODER bits for PA8
	 GPIOA->MODER |= GPIO_MODER_MODE8_1; // Set PA8 to AF mode

	 // Set PA8 to the correct Alternate Function (AF0 for MCO1)
	 GPIOA->AFR[1] &= ~(0xF << (8 - 8) * 4); // Clear AFR bits for PA8
	 GPIOA->AFR[1] |= (0x0 << (8 - 8) * 4);  // AF0 for MCO1

	 // Set MCO1 source to SYSCLK and no prescaler
	 RCC->CFGR &= ~RCC_CFGR_MCOPRE;          // Clear MCO prescaler
	 RCC->CFGR |= RCC_CFGR_MCOPRE_DIV1;      // Set MCO prescaler to 1 (no division)
	 RCC->CFGR |=  RCC_CFGR_MCOSEL_HSI ;    // Set MCO to output the system clock (à sélectionner par changer RCC_CFGR_MCOSEL_xxxx ) 
 }
