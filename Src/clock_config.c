/*
 * clcok_config.c
 *
 *  Created on: Nov 22, 2024
 *      Author: Khouloud
 */

#include"stm32l073xx.h"
#include "clock_config.h"
#include "user_clock_config.h"

void SystemClock_Config(void) {


    // Use the global clockConfig defined in user_clock_config.c
    ClockConfig *config = &clockConfig;

    // Step 1: Reset RCC configuration to defaults
    RCC->CR |= RCC_CR_HSION; // Ensure HSI is ON by default
    RCC->CFGR = 0;           // Reset clock configuration register
    RCC->CR &= ~(RCC_CR_PLLON); // Disable PLL
    while (RCC->CR & RCC_CR_PLLRDY); // Wait until PLL is fully disabled

    // Step 2: Configure the selected clock source
    switch (config->source) {

        case CLOCK_SOURCE_HSI:
            // HSI is enabled by default
            while (!(RCC->CR & RCC_CR_HSIRDY)); // Wait until HSI is ready
            RCC->CFGR |= RCC_CFGR_SW_HSI;      // Set HSI as SYSCLK source
            break;

        case CLOCK_SOURCE_HSE:
            RCC->CR |= RCC_CR_HSEON;           // Enable HSE
            while (!(RCC->CR & RCC_CR_HSERDY)); // Wait until HSE is ready
            RCC->CFGR |= RCC_CFGR_SW_HSE;      // Set HSE as SYSCLK source
            break;

        case CLOCK_SOURCE_MSI:
            RCC->CR |= RCC_CR_MSION;           // Enable MSI
            if (config->msi_range > 0) {
                RCC->ICSCR = (RCC->ICSCR & ~RCC_ICSCR_MSIRANGE) |
                             (config->msi_range << RCC_ICSCR_MSIRANGE_Pos);
            }
            while (!(RCC->CR & RCC_CR_MSIRDY)); // Wait until MSI is ready
            RCC->CFGR |= RCC_CFGR_SW_MSI;      // Set MSI as SYSCLK source
            break;

        case CLOCK_SOURCE_PLL:
            // Configure PLL source (HSI or HSE)
            if (RCC->CR & RCC_CR_HSEON) {
                RCC->CFGR = RCC_CFGR_PLLSRC_HSE;
            } else {
                RCC->CFGR = RCC_CFGR_PLLSRC_HSI;
            }

            // Configure PLL multiplier and divider
            RCC->CFGR |= (config->pll_mul << RCC_CFGR_PLLMUL_Pos) |
                            (config->pll_div << RCC_CFGR_PLLSRC_Pos);

            // Enable PLL
            RCC->CR |= RCC_CR_PLLON;
            while (!(RCC->CR & RCC_CR_PLLRDY)); // Wait until PLL is ready

            // Set PLL as SYSCLK source
            RCC->CFGR |= RCC_CFGR_SW_PLL;
            while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
            break;
    }

    // Step 3: Update Flash latency if needed (example assumes 1 wait state)
    FLASH->ACR |= FLASH_ACR_LATENCY;
}
