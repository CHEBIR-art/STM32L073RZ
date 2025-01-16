/*
 * SX1261.H
 *
 *  Created on: Nov 28, 2024
 *      Author: user
 */

#ifndef SX1261_H_
#define SX1261_H_

#include <stdint.h>

// Prototypes des fonctions SX1261

#define writeOP 0x0D
#define readOP 0x1D
#define writeBUFFERop  0x0E
#define readBUFFERop  0x1E


// Registers for SX1261
#define RegOpMode 0x01             // Operating mode configuration
#define RegFrMsb   0xD9             // Frequency setting (MSB)
#define RegFrMib  0x06             // Frequency setting (MID)
#define RegFrLsb  0x66             // Frequency setting (LSB)  Pour 868 MHz
#define RegModemConfig1 0x1D       // Modem configuration (bandwidth BW19 19.5KHz , coding rate)
#define RegModemConfig2 0x1E       // Modem configuration (spreading factor, CRC) RX_BW_9700 (9.7 kHz DSB)
#define RegFifoTxBaseAddr 0x08     // FIFO TX base address
#define RegFifoRxBaseAddr 0x09     // FIFO RX base address
#define RegPaConfig 0x08           // Power amplifier configuration
#define RegIrqFlagsMask 0x15       // Interrupt mask
#define RegFifoAddrPtr 0x0D        // FIFO address pointer
#define RegPayloadLength 0x22      // Payload length configuration
#define RegIrqFlags 0x18           // Interrupt flags
#define RegRxNbBytes 0x19          // Number of received bytes
#define RegFifoRxCurrentAddr 0x1A  // Current RX FIFO address


void SX1261_Init(void);
void SX1261_ReadRegister(uint8_t regAddr , uint8_t *data2READ );
void SX1272_WriteReg(uint8_t RegAddr, uint8_t data2WRITE);
void SX1261_ReadBuffer(uint8_t offset, uint8_t *data2READ, uint8_t size);
void SX1261_WriteBuffer(uint8_t offset, uint8_t *data2WRITE, uint8_t size);
#endif /* SX1261_H_ */
