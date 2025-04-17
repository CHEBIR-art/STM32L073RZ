/*
 * spi.c
 *
 *  Created on: Nov 27, 2024
 *      Author: user
 */


#include "stm32l073xx.h"
#include <stdio.h>
#include "spi.h"
#include "sx1261.h"

#define NSS_LOW()  (GPIOA->ODR &= ~GPIO_ODR_OD4) // NSS = PA4 SLAVE SELECT IS CHOSEN
#define NSS_HIGH() (GPIOA->ODR |= GPIO_ODR_OD4)   //END OF COMMUNICATION WITH THE SLAVE

void SPI_Init(void) {

	    // Enable clock for SPI1
	    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;  // Enable SPI1 clock
	    // Configure GPIO pins for SPI1: NSS (PA4), SCK (PA5), MISO (PA6), MOSI (PA7)
	    GPIOA->MODER &= ~( (3U << (2 * 4)) |(3U << (2 * 5)) | (3U << (2 * 6)) | (3U << (2 * 7))); // Clear modes
	    GPIOA->MODER |= ((2U << (2 * 5)) | (2U << (2 * 6)) | (2U << (2 * 7)));  // Set PA5, PA6, PA7 to Alternate Function

	   GPIOA->MODER |= (1U << (2 * 4));  // Set PA4 (NSS) as output

	    GPIOA->AFR[0] &= ~((0xF << (4 * 5)) | (0xF << (4 * 6)) | (0xF << (4 * 7))); // Clear AF
	    GPIOA->AFR[0] |= ((0U << (4 * 5)) | (0U << (4 * 6)) | (0U << (4 * 7)));    // Assign AF0 for SPI1

	    //  Configurer PB3 en entrée (BUSY signal)
	        GPIOB->MODER &= ~(3U << (2 * 3));  // Mettre PB3 en mode entrée
	       // GPIOB->PUPDR &= ~(3U << (2 * 3));  // Pas de pull-up / pull-down


	   // NSS_HIGH();
	  /// Configure SPI: Master mode, baud rate, CPOL, CPHA, and software NSS management
	    SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI;  // Master mode, software NSS management

      //Le SX1261 spécifie une fréquence SPI maximale de 10 MHz
	  //SPI1->CR1 &= ~SPI_CR1_BR;  //  No prescaler, SPI clock = Fpclk (16 MHz)  TROP RAPIDE

	    SPI1->CR1 |= SPI_CR1_BR_1 | SPI_CR1_BR_0;  // Configure prescaler à FPCLK/16 (1 MHz)   011: fPCLK/16
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


	//NSS_LOW();  // Activer le slave (mettre NSS à LOW)

    // Nettoyer le registre RX si nécessaire
    while (SPI1->SR & SPI_SR_RXNE) {
        (void)SPI1->DR;  // Lire DR pour vider le buffer RX
    }  //wait pour qu'il soit vide '


     //data sending
    SPI1->DR = data2SEND;

    // Attendre que la transmission soit terminée (TXE = 1, BSY = 0)

    while (!(SPI1->SR & SPI_SR_TXE));
    while (SPI1->SR & SPI_SR_BSY);  // Attendre que le SPI soit inactif

    //NSS_HIGH();  // Désactiver le slave (mettre NSS à HIGH)
    return (uint16_t) SPI1->DR;
}



void SPI_LoopbackTest(uint8_t testByte) {
    uint8_t receivedByte = SPI1_TransmitReceive(testByte);

    printf("SPI Loopback Test:\r\n");
    printf("  - Byte envoyé : 0x%02X\r\n", testByte);
    printf("  - Byte reçu   : 0x%02X\r\n", receivedByte);

    if (testByte == receivedByte) {
        printf("  -> SUCCÈS : Communication SPI fonctionnelle en boucle locale.\r\n");
    } else {
        printf("  -> ERREUR : Problème de communication SPI.\r\n");
    }
}
void test_readregister (void ){
// ReadRegister commande = 0x1D
NSS_LOW();
  SPI1_TransmitReceive(0x1D);        // Opcode: ReadRegister
  SPI1_TransmitReceive(0x08);        // Adresse MSB = 0x08
  SPI1_TransmitReceive(0x89);        // Adresse LSB = 0x89
  SPI1_TransmitReceive(0x01);        // Nombre d’octets à lire = 1
  uint8_t status = SPI1_TransmitReceive(0x00); // Lire la donnée
NSS_HIGH();

printf("?? Registre 0x0889 = 0x%02X\r\n", status);
const char* mode_names[] = {
    "STDBY_RC", "STDBY_XOSC", "FS", "RX", "TX", "CAD", "UNKNOWN"
};
uint8_t mode = (status >> 5) & 0x07;
printf("?? Mode actuel (depuis registre 0x0889) : %s (0x%02X)\r\n", mode_names[mode], mode);

}

