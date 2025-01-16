/*
 * spi.c
 *
 *  Created on: Nov 27, 2024
 *      Author: user
 */


#include "stm32l073xx.h"
#include "spi.h"

#define NSS_LOW()  (GPIOA->ODR &= ~GPIO_ODR_OD4) // NSS = PA4 SLAVE SELECT IS CHOSEN
#define NSS_HIGH() (GPIOA->ODR |= GPIO_ODR_OD4)   //END OF COMMUNICATION WITH THE SLAVE

void SPI_Init(void) {

	    // Enable clock for SPI1 and GPIOA
	    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;  // Enable SPI1 clock
	    // Configure GPIO pins for SPI1: NSS (PA4), SCK (PA5), MISO (PA6), MOSI (PA7)
	    GPIOA->MODER &= ~( (3U << (2 * 4)) |(3U << (2 * 5)) | (3U << (2 * 6)) | (3U << (2 * 7))); // Clear modes
	    GPIOA->MODER |= ((2U << (2 * 5)) | (2U << (2 * 6)) | (2U << (2 * 7)));  // Set PA5, PA6, PA7 to Alternate Function
	   GPIOA->MODER |= (1U << (2 * 4));  // Set PA4 (NSS) as output

	    GPIOA->AFR[0] &= ~((0xF << (4 * 5)) | (0xF << (4 * 6)) | (0xF << (4 * 7))); // Clear AF
	    GPIOA->AFR[0] |= ((0U << (4 * 5)) | (0U << (4 * 6)) | (0U << (4 * 7)));    // Assign AF0 for SPI1


	  /// Configure SPI: Master mode, baud rate, CPOL, CPHA, and software NSS management
	    SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI;  // Master mode, software NSS management
	    SPI1->CR1 &= ~SPI_CR1_BR;  //  No prescaler, SPI clock = Fpclk (16 MHz)
	    SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);  // CPOL=0, CPHA=0 (Mode 0)

	    // Configure data size to 8 bits (clear DFF bit for 8-bit frame format)
	    SPI1->CR1 &= ~SPI_CR1_DFF;  // DFF = 0 -> 8-bit data frame format

	    // Set RX FIFO threshold to 8 bits (default behavior for 8-bit data frames)
	    // No additional configuration is needed as the hardware manages this automatically.

	    // Enable SPI
	    SPI1->CR1 |= SPI_CR1_SPE;  // Enable SPI1*/
    }






uint8_t SPI1_TransmitReceive(uint8_t data2SEND) {
    SPI1->DR = data2SEND;
    while (!(SPI1->SR & SPI_SR_TXE)) {}
    while (!(SPI1->SR & SPI_SR_RXNE)) {}
    return SPI1->DR;
}

/*void SPI_Test(void) {
	//uint8_t  regAddr ;
	    uint8_t sentByte = 0xA5;  // Exemple de byte à envoyer
	    uint8_t receivedByte = SPI1_TransmitReceive(sentByte);  // Transmettre et recevoir
	    printf("Received Byte: 0x%02X\n", receivedByte);// Afficher le byte reçu
	    printf("Reading Register: 0x%02X\n", regAddr);  // Affiche l'adresse du registre

	}*/

void SPI_Test(uint8_t regAddr) {
    uint8_t sentByte = regAddr & 0x7F;  // S'assurer que le bit MSB est à 0 pour une lecture
    uint8_t receivedByte = SPI1_TransmitReceive(sentByte);  // Envoyer l'adresse et lire la réponse

    // Afficher les résultats sur le terminal
    printf("Sent Byte (Register Address): 0x%02X\n", sentByte);
    printf("Received Byte: 0x%02X\n", receivedByte);
}



