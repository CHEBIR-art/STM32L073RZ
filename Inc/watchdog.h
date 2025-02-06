/*
 * whatchdog.h
 *
 *  Created on: Jan 23, 2025
 *      Author: user
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

void IWDG_Init(void);
void Feed_IWDG(void);
void Check_Reset_Cause(void);
#endif /* WATCHDOG_H_ */
