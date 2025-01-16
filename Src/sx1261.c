/*
 * sx1261.c
 *
 *  Created on: Nov 28, 2024
 *      Author: user
 */

#include "sx1261.h"
#include "spi.h"

#define NSS_LOW()  (GPIOA->ODR &= ~GPIO_ODR_OD4) // NSS = PA4
#define NSS_HIGH() (GPIOA->ODR |= GPIO_ODR_OD4)

void SX1261_ReadRegister(uint8_t regAddr , uint8_t *data2READ ) {
    NSS_LOW();  //ACTIVATE THE SPI BY RESETING THE NSS SIGNAL
    SPI1_TransmitReceive(readOP); // read register command
    SPI1_TransmitReceive((regAddr >> 8) & 0xFF); // Send high byte of the address.
       SPI1_TransmitReceive(regAddr & 0xFF); // Send low byte of the address.
    *data2READ = SPI1_TransmitReceive(0x00); // READ DATA BY SENDING A DUMMY VALUE 0x00
    NSS_HIGH();
}

void SX1261_WriteReg(uint8_t RegAddr, uint8_t data2WRITE){
	NSS_LOW ();
	SPI1_TransmitReceive (writeOP);
	SPI1_TransmitReceive((RegAddr >> 8) & 0xFF); // Send the high byte of the register address.
	    SPI1_TransmitReceive(RegAddr & 0xFF); // Send the low byte of the register address.
	    SPI1_TransmitReceive(data2WRITE); // Send the data to write.
	NSS_HIGH();
}
void SX1261_WriteBuffer(uint8_t offset, uint8_t *data2WRITE, uint8_t size) {
	NSS_LOW (); // Activate the SPI slave.
    SPI1_TransmitReceive(writeBUFFERop); // Send the WriteBuffer command.
    SPI1_TransmitReceive(offset); // Send the FIFO offset.
    for (uint8_t i = 0; i < size; i++) {
        SPI1_TransmitReceive(data2WRITE[i]); // Write data to the FIFO.
    }
    NSS_HIGH(); // Deactivate the SPI slave.
}

void SX1261_ReadBuffer(uint8_t offset, uint8_t *data2READ, uint8_t size) {
	NSS_LOW (); // Activate the SPI slave.
    SPI1_TransmitReceive(readBUFFERop); // Send the ReadBuffer command.
    SPI1_TransmitReceive(offset); // Send the FIFO offset.
    for (uint8_t i = 0; i < size; i++) {
    	data2READ[i] = SPI1_TransmitReceive(0x00); // Read data from the FIFO.
    }
    NSS_HIGH(); // Deactivate the SPI slave.
}

void SX1261_Init(void) {
    // Configure NSS as output
    GPIOA->MODER &= ~(3U << (2 * 4)); // Clear mode for PA4
    GPIOA->MODER |= (1U << (2 * 4)); // Set PA4 as output

    // Set to standby mode
    SX1261_WriteReg(RegOpMode, 0x00); // Set the device to standby mode

    // Set frequency to 868 MHz
    SX1261_WriteReg(RegFrMsb, 0xD9); // Frequency MSB for 868 MHz
    SX1261_WriteReg(RegFrMib, 0x06); // Frequency MIB for 868 MHz
    SX1261_WriteReg(RegFrLsb, 0x66); // Frequency LSB for 868 MHz

    // Configure bandwidth, coding rate, and spreading factor
    SX1261_WriteReg(RegModemConfig1, 0x72); // Bandwidth = 125 kHz, Coding Rate = 4/5
    SX1261_WriteReg(RegModemConfig2, 0x74); // Spreading Factor = 12

    // Configure FIFO base addresses
    SX1261_WriteReg(RegFifoTxBaseAddr, 0x80); // FIFO TX base address
    SX1261_WriteReg(RegFifoRxBaseAddr, 0x00); // FIFO RX base address

    // Set output power
    SX1261_WriteReg(RegPaConfig, 0x8F); // Configure output power (adjust as needed)

    // Enable necessary interrupts (adjust mask as per application requirements)
    SX1261_WriteReg(RegIrqFlagsMask, 0xF7); // Mask unnecessary interrupts

    // Set payload length (optional, based on application)
    SX1261_WriteReg(RegPayloadLength, 0x40); // Payload length = 64 bytes (adjust as needed)

    // Clear all IRQ flags
    SX1261_WriteReg(RegIrqFlags, 0xFF); // Clear any pending interrupts
}

