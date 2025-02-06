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


// Frequency Configuration
#define REG_RF_FREQ_MSB      0x0860
#define REG_RF_FREQ_MID      0x0861
#define REG_RF_FREQ_LSB      0x0862

// Modulation Parameters
#define REG_MODULATION_PARAMS_SF    0x0889  // Spreading Factor
#define REG_MODULATION_PARAMS_BW    0x088A  // Bandwidth
#define REG_MODULATION_PARAMS_CR    0x088B  // Coding Rate
#define REG_MODULATION_PARAMS_LDRO  0x088C  // Low Data Rate Optimize

// Sync Word for LoRaWAN
#define REG_SYNC_WORD_MSB    0x09C0
#define REG_SYNC_WORD_LSB    0x09C1

// Transmit Power
#define REG_TX_PARAMS        0x08E7

// FIFO Configuration
#define REG_FIFO_TX_BASE     0x0800
#define REG_FIFO_RX_BASE     0x0801

// IRQ Configuration
#define REG_IRQ_FLAGS        0x0914
#define REG_IRQ_FLAGS_MASK   0x0915

// RSSI Measurement
#define REG_RSSI_INST        0x0910
#define REG_RX_GAIN          0x0911

// RX Buffer
#define REG_FIFO_ADDR_PTR       0x0803
#define REG_FIFO_RX_CURRENT_ADDR 0x0804
#define REG_RX_NB_BYTES         0x0805

// Payload Configuration
#define REG_PAYLOAD_LENGTH   0x0892


/********************************************************************/
// LoRa Modulation Values
#define SF12                 0xC0  // Spreading Factor 12
#define BW_125KHZ            0x70  // Bandwidth 125 kHz
#define CR_4_5               0x02  // Coding Rate 4/5
#define LDRO_ON              0x01  // Low Data Rate Optimization ON

// LoRaWAN Sync Word
#define LORAWAN_SYNC_WORD_MSB  0x34
#define LORAWAN_SYNC_WORD_LSB  0x44

// Transmit Power
#define TX_POWER_22DBM       0x16  // 22 dBm

/********************************************************************/
#define REG_IRQ_FLAGS_MASK    0x0915  // Interrupt mask register
#define REG_PAYLOAD_LENGTH    0x0892  // Payload length register
#define REG_IRQ_FLAGS         0x0914  // IRQ flags register



void SX1261_Init(void);
uint8_t SX1261_ReadRegister(uint16_t regAddr);
void SX1272_WriteRegister(uint8_t RegAddr, uint8_t data2WRITE);
void SX1261_ReadBuffer(uint8_t offset, uint8_t *data2READ, uint8_t size);
void SX1261_WriteBuffer(uint8_t offset, uint8_t *data2WRITE, uint8_t size);
void SX1261_LORA_Init(void);
void SX1261_test(void);
void SX1261_Reset(void);
void SX1261_SimpleTest(void);
void SX1261_TestWriteRead(void);
void SX1261_WaitWhileBusy(void);
void SX1261_LORA_test (void);
#endif /* SX1261_H_ */
