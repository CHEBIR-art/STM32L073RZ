/*
 *  SystemClock_Config.c
 *
 *  Created on: Jan 9, 2025
 *      Author: user
 */
#include <stdint.h>
#include "stm32l073xx.h"
#include "system_clock_config.h"

void SystemClock_Config(void) {
    // Enable HSI16 and wait for it to stabilize
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY));

    // Set HSI16 as the system clock (SYSCLK)
    RCC->CFGR |= RCC_CFGR_SW_HSI; // Select HSI as SYSCLK source
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);

    // Optional: Set SYSCLK to a higher frequency using PLL if needed
    // Ensure APB1 prescaler is set to support high clock speeds
}
