/*
 * spi.h
 *
 *  Created on: Nov 27, 2024
 *      Author: khouloud
 */

#ifndef SPI_H
#define SPI_H

#include "stm32l073xx.h"

void SPI_Init(void);
uint8_t SPI_TransmitReceive(uint8_t data);
void SPI_Test(uint8_t regAddr);
void SPI_LoopbackTest(uint8_t testByte);

#endif // SPI_H

