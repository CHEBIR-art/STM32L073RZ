/*
 * uart2.c
 *
 *  Created on: Jan 10, 2025
 *      Author: user
 */

#include <stdint.h>
#include "stm32l073xx.h"
#include "uart2.h"

void UART2_INIT(void){

	  // Enable USART2 Clock
	    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	    // Configure USART2 clock source as SYSCLK (01)
	    RCC->CCIPR &= ~RCC_CCIPR_USART2SEL_Msk;  // Clear USART2 clock source bits
	    RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;     // Set SYSCLK as USART2 clock source




	 // CONFIGURE THE UART2
	 USART2->BRR =  16000000 / 9600;   // Baudrate = 9600 (16000000 MHz clock)

	 USART2->CR1 = USART_CR1_TE |     // Enable transmitter
	               USART_CR1_RE |     // Enable receiver
	               USART_CR1_UE;      // Enable USART2

	 //configure the PINS PA2 AND PA1 AS ALTERNATE FUNCTION
	 GPIOA->MODER &= ~((3U << (2 * 2)) | (3U << (2 * 3)));// Clear bits
	 GPIOA->MODER |= (2U << (2 * 2)) | (2U << (2 * 3));    // Set alternate function mode


	 GPIOA->AFR[0] &= ~((0xF << (4 * 2)) | (0xF << (4 * 3))); // Clear AF bits
	 GPIOA->AFR[0] |= (4U << (4 * 2)) | (4U << (4 * 3));      // Set AF4 for USART2
}

void UART2_test (void){
	//TRANSMISSION

		printf("Hello, USART2!\r\n");
		/*scanf("%d",&test);
		*while(1){
		*while (!(USART2->ISR & USART_ISR_RXNE));
		*USART2->TDR = USART2->RDR;
		*scanf("%c",&test);
		*printf("%c\r\n",test); // Lire le registre des données reçues   RECEIVE DATA REGISTER
	**/

					//RECEPTION

		/*char received_char;
		printf("\r\nEnter a key : ");
		received_char = __io_getchar(); // Wait for character input

		        // Display the received character
		        printf("\r\nYou pressed: %c\r\n", received_char);

		/*received_char = __io_getchar(); // Wait for character input
		if (received_char != '\r' && received_char != '\n' && received_char != ' ' ) {
		    printf("You pressed: %c\r\n", received_char); // Echo back
		}*/


}
