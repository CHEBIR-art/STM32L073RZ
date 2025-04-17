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
uint16_t SPI1_TransmitReceive(uint16_t data2SEND);
void SPI_Test(uint8_t regAddr);
void SPI_LoopbackTest(uint8_t testByte);
void test_readregister  (void );
#endif // SPI_H

