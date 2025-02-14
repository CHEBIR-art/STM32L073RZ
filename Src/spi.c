/*
 * spi.c
 *
 *  Created on: Nov 27, 2024
 *      Author: user
 */


#include "stm32l073xx.h"
#include <stdio.h>
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

	    //  Configurer PB3 en entr�e (BUSY signal)
	        GPIOB->MODER &= ~(3U << (2 * 3));  // Mettre PB3 en mode entr�e
	        GPIOB->PUPDR &= ~(3U << (2 * 3));  // Pas de pull-up / pull-down


	   // NSS_HIGH();
	  /// Configure SPI: Master mode, baud rate, CPOL, CPHA, and software NSS management
	    SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI;  // Master mode, software NSS management

      //Le SX1261 sp�cifie une fr�quence SPI maximale de 10 MHz
	  //SPI1->CR1 &= ~SPI_CR1_BR;  //  No prescaler, SPI clock = Fpclk (16 MHz)  TROP RAPIDE

	    SPI1->CR1 |= SPI_CR1_BR_1 | SPI_CR1_BR_0;  // Configure prescaler � FPCLK/16 (1 MHz)   011: fPCLK/16
	   // SPI1->CR1 |= SPI_CR1_BR_2;  // fPCLK/32 (500 kHz) //JUST TO TRY !!!!!!


	    SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);  // CPOL=0, CPHA=0 (Mode 0)

	    // Configure data size to 8 bits (clear DFF bit for 8-bit frame format)
	   // SPI1->CR1 |= SPI_CR1_DFF;  // DFF = 1 -> 8-bit data frame format

	    // Set RX FIFO threshold to 8 bits (default behavior for 8-bit data frames)
	    // No additional configuration is needed as the hardware manages this automatically.

	    // Enable SPI
	    SPI1->CR1 |= SPI_CR1_SPE;  // Enable SPI1*/
    }






uint16_t SPI1_TransmitReceive(uint16_t data2SEND) {
	NSS_LOW();  // Activer le slave

	    // Envoyer la donn�e
	    SPI1->DR = (uint8_t)(data2SEND & 0xFF);

	    // Attendre que la transmission soit termin�e
	    while (!(SPI1->SR & SPI_SR_TXE));  // Attendre que le buffer TX soit vide
	    while (!(SPI1->SR & SPI_SR_RXNE)); // Attendre la r�ception d'une donn�e

	    // Lire et retourner la donn�e re�ue
	    uint16_t received_data = SPI1->DR;

	    while (SPI1->SR & SPI_SR_BSY); // V�rifier que SPI est inactif

	    NSS_HIGH();  // D�sactiver le slave
	    return received_data;

}


void SPI_Test(uint8_t regAddr) {
    // Pr�parer l'octet � envoyer (lecture : bit MSB � 0)
    uint8_t sentByte = regAddr & 0x7F;  // MSB doit �tre 0
    uint8_t receivedByte;

    // Transmettre l'octet et recevoir la r�ponse
    receivedByte = SPI1_TransmitReceive(sentByte);

    // Afficher le byte envoy� et re�u
    printf("SPI Test:\r\n");
    printf("  - Byte envoy� : 0x%02X\r\n", sentByte);
    printf("  - Byte re�u   : 0x%02X\r\n", receivedByte);

    // V�rifier si les deux bytes correspondent
    if (sentByte != receivedByte) {
        printf("  -> ERREUR : Le byte envoy� et le byte re�u ne correspondent pas !\r\n");
    } else {
        printf("  -> SUCC�S : Le byte envoy� correspond au byte re�u.\r\n");
    }
}
void SPI_LoopbackTest(uint8_t testByte) {
    uint8_t receivedByte = SPI1_TransmitReceive(testByte);

    printf("SPI Loopback Test:\r\n");
    printf("  - Byte envoy� : 0x%02X\r\n", testByte);
    printf("  - Byte re�u   : 0x%02X\r\n", receivedByte);

    if (testByte == receivedByte) {
        printf("  -> SUCC�S : Communication SPI fonctionnelle en boucle locale.\r\n");
    } else {
        printf("  -> ERREUR : Probl�me de communication SPI.\r\n");
    }
}

