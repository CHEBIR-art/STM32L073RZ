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
void Check_LSI_Status(void) {
    if (RCC->CSR & RCC_CSR_LSIRDY) {
        printf("LSI est deja active \r\n");
    } else {
        printf("LSI n'est PAS active !\r\n");
    }
}
void LSI_Enable(void) {
    // Activer l'horloge LSI
    RCC->CSR |= RCC_CSR_LSION;

    // Attendre que LSI soit prêt
    while ((RCC->CSR & RCC_CSR_LSIRDY) == 0) {}

    Check_LSI_Status();
}

