/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "stm32l073xx.h"
#include "main.h"
#include "timer.h"
#include "clock_config.h"
#include "user_clock_config.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
	 SystemClock_Config();
	// Initialization of peripherals

	GPIO_clock_INIT();
	//UART2_INIT();
	//Timer2_Init();


	/**************UART2 TEST **********/

				//TRANSMISSION

	printf("Hello, USART2!\r\n");


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


	/**************TEST GPIO************/


		// DEFINE THE PIN5 MODE AS OUTPUT LED

	GPIOA->MODER &= ~(3U << (2 * 5));  // Clear the mode bits for PA5   //10TH AND 11TH BITS ARE SET TO 0
	GPIOA->MODER |= (1U << (2 * 5));   // Set the mode bits to 01 (Output)// 10TH BIT IS SET TO 1



	/**************MCO1 CONFIGURATION*******/

	 MCO1_CONFIG();



while(1){
	/**********TIMER TEST********
		Timer2_Delay_ms(10000); // 1-second delay              //************SUCCESS
	    printf("Timer delay complete\r\n");*/


	/******TIMER APPLICATION***********/

		GPIOA->ODR ^= GPIO_ODR_OD5;   // LED TOGGLE
		  Timer2_Delay_ms(1000000);       // DELAY OF 500 ms*/
		  printf("Timer delay complete\r\n"); // TO TEST
}
}


void GPIO_clock_INIT(void ){

	//ENABLE GPIO A CLOCK
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
	//ENABLE GPIO B CLOCK
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
	//ENABLE GPIO C CLOCK
	RCC->IOPENR |= RCC_IOPENR_GPIOCEN;
	//ENABLE GPIO D CLOCK
	RCC->IOPENR |= RCC_IOPENR_GPIODEN;
	//ENABLE GPIO E CLOCK
	RCC->IOPENR |= RCC_IOPENR_GPIOEEN;
}


void UART2_INIT(void){


      //ENABLE THE UART2 CLOCK
	 RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	 // CONFIGURE THE UART2
	 USART2->BRR = 2101000 / 9600;   // Baudrate = 9600 (2.101 MHz clock)

	 USART2->CR1 = USART_CR1_TE |     // Enable transmitter
	               USART_CR1_RE |     // Enable receiver
	               USART_CR1_UE;      // Enable USART2

	 //configure the PINS PA2 AND PA3 AS ALTERNATE FUNCTION
	 GPIOA->MODER &= ~((3U << (2 * 2)) | (3U << (2 * 3)));// Clear bits
	 GPIOA->MODER |= (2U << (2 * 2)) | (2U << (2 * 3));    // Set alternate function mode


	 GPIOA->AFR[0] &= ~((0xF << (4 * 2)) | (0xF << (4 * 3))); // Clear AF bits
	 GPIOA->AFR[0] |= (4U << (4 * 2)) | (4U << (4 * 3));      // Set AF4 for USART2
}


// FUNCTION TO SEND DATA

void __io_putchar(char c) {


    while (!(USART2->ISR & USART_ISR_TXE)) {   // USART_ISR_TXE CHECK IF THE TRANSMISSION BUFFER IS READY (EMPTY)
        // Attente : buffer de transmission vide
    }
    USART2->TDR = c; // Envoyer le caractère
}


		//FUNCTION TO WRITE DATA
int  __io_getchar(void) {

    while (!(USART2->ISR & USART_ISR_RXNE)) {  //USART_ISR_RXNE  Read Data Register Not Empty
        // Attente : caractère reçu
    }
    return USART2->RDR; // Lire le registre des données reçues   RECEIVE DATA REGISTER
}




 void MCO1_CONFIG(void){


	 // Set PA8 to Alternate Function mode
	 GPIOA->MODER &= ~(GPIO_MODER_MODE8); // Clear MODER bits for PA8
	 GPIOA->MODER |= GPIO_MODER_MODE8_1; // Set PA8 to AF mode

	 // Set PA8 to the correct Alternate Function (AF0 for MCO1)
	 GPIOA->AFR[1] &= ~(0xF << (8 - 8) * 4); // Clear AFR bits for PA8
	 GPIOA->AFR[1] |= (0x0 << (8 - 8) * 4);  // AF0 for MCO1

	 // Set MCO1 source to SYSCLK and no prescaler
	 RCC->CFGR &= ~(RCC_CFGR_MCOSEL | RCC_CFGR_MCOPRE); // Clear existing settings  mco selector and the prescaler
	 RCC->CFGR |= (RCC_CFGR_MCOSEL_SYSCLK | RCC_CFGR_MCOPRE_DIV1); // SYSCLK and DIV1    DEFINE THE MCO SOURCE AS THE SYSTEM CLOCK AND SET THE PRESCALER TO 1

 }

/* // Overriding _write to use USART2_SendChar

int _write(int file, char *data, int len) {

     for (int i = 0; i < len; i++) {
         __io_putchar(data[i]);
     }
     return len; // Return the number of characters written
 }*/


 // Overriding _read to use USART2_ReceiveChar


 int _read(int file, char *ptr, int len) {
     (void)file; // Suppress unused parameter warning
     for (int i = 0; i < len; i++) {
         ptr[i] =  __io_getchar();
     }
     return len;
 }

