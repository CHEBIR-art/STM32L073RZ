/*
 * rtc.h
 *
 *  Created on: Jan 22, 2025
 *      Author: user
 */

#ifndef RTC_H_
#define RTC_H_

#include "stm32l073xx.h"

// Prototypes des fonctions
void RTC_Init(void);
void RTC_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds);
void RTC_GetTime(uint8_t *hours, uint8_t *minutes, uint8_t *seconds);
void RTC_SetWakeup(uint32_t seconds);
void RTC_WKUP_IRQHandler(void) ;
void EnterStopMode(void);
#endif /* RTC_H_ */
