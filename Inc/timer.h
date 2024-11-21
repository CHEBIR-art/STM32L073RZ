/*
 * timer.h
 *
 *  Created on: Nov 21, 2024
 *      Author: user
 */

#ifndef TIMER_H_
#define TIMER_H_


#include "stm32l073xx.h"

// Timer initialization
void Timer2_Init(void);

// delay function ms
void Timer2_Delay_ms(uint32_t delay);


#endif /* TIMER_H_ */
