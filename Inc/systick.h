/*
 * systick.h
 *
 *  Created on: Jan 9, 2025
 *      Author: user
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

void SysTick_Handler(void);
void SYSTICK_Init(void);
void SYSTICK_Delay(uint32_t Delay);
uint32_t SYSTICK_Get(void);

void DWT_Init(void);
void DWT_Delay(uint32_t _us);



#endif /* SYSTICK_H_ */
