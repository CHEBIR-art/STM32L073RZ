/*
 * user_clock_config.c
 *
 *  Created on: Nov 22, 2024
 *      Author: Khouloud
 *
 *
 *      FLEXIBLE SYCTEM CLOCK SELECTROR
 *
 *
 *
 */


#include "clock_config.h"

// Define the global clock configuration
ClockConfig clockConfig = {
    .source = CLOCK_SOURCE_PLL,  // Use PLL as the clock source
    .hse_freq = 8000000,         // HSE Frequency in Hz
    .pll_mul = 6,                // PLL Multiplier
    .pll_div = 3,                // PLL Divider
    .msi_range = 0               // Not used for PLL
};
