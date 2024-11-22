/*
 * clock_config.h
 *
 *  Created on: Nov 22, 2024
 *      Author: user
 */

#ifndef CLOCK_CONFIG_H_
#define CLOCK_CONFIG_H_


#include "stm32l073xx.h"



	/************Enum for Clock Sources*******/

typedef enum {
    CLOCK_SOURCE_HSI,  // High-Speed Internal oscillator (16 MHz)
    CLOCK_SOURCE_HSE,  // High-Speed External oscillator
    CLOCK_SOURCE_MSI,  // Multi-Speed Internal oscillator
    CLOCK_SOURCE_PLL   // Phase-Locked Loop (derived from HSI/HSE/MSI)
} ClockSource;


	/**********CLOCK CONFIGURATION STRUCTURE********/

typedef struct {
    ClockSource source;   // Clock source
    uint32_t hse_freq;    // Frequency of HSE in Hz (if using HSE)
    uint8_t pll_mul;      // PLL multiplication factor
    uint8_t pll_div;      // PLL division factor
    uint8_t msi_range;    // MSI range (DEFAULT VALUE IS 0 )
} ClockConfig;



void SystemClock_Config(void);

#endif /* CLOCK_CONFIG_H_ */
