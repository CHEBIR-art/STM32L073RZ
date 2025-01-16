/*
 * gpio.c
 *
 *  Created on: Jan 9, 2025
 *      Author: user
 */

#include <stdint.h>
#include "stm32l073xx.h"
#include "gpio.h"


void GPIO_clock_INIT(void ){

	//ENABLE GPIO A CLOCK
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
	//ENABLE GPIO B CLOCK
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
	//ENABLE GPIO C CLOCK
	RCC->IOPENR |= RCC_IOPENR_GPIOCEN;
	//ENABLE GPIO D CLOCK
	RCC->IOPENR |= RCC_IOPENR_GPIODEN;
	//ENABLE GPIO E CLOCK
	RCC->IOPENR |= RCC_IOPENR_GPIOEEN;
}
