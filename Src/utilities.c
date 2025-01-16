/*
 * utilities.c
 *
 *  Created on: Jan 10, 2025
 *      Author: user
 */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "stm32l073xx.h"
#include "utilities.h"

//// Functions to send data ///

void __io_putchar(char c) {
    while (!(USART2->ISR & USART_ISR_TXE));   // USART_ISR_TXE CHECK IF THE TRANSMISSION BUFFER IS READY (EMPTY)
    USART2->TDR = c; // Envoyer le caractère
}


		//FUNCTION TO WRITE DATA
int  __io_getchar(void) {
    while (!(USART2->ISR & USART_ISR_RXNE));
    return USART2->RDR; // Lire le registre des données reçues   RECEIVE DATA REGISTER
}

void SCANF_Init(void){
	setvbuf(stdin, NULL, _IONBF, 0);
}
