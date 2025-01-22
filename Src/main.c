
#define STM32L073xx
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "stm32l073xx.h"
#include "system_stm32l0xx.h"
#include "system_clock_config.h"
#include "main.h"
#include "app_ledblink.h"
#include "timer.h"
#include "gpio.h"
#include "systick.h"
#include "spi.h"
#include "sx1261.h"
#include "mco.h"
#include "uart2.h"
#include "utilities.h"



int main(void)
{

	SystemClock_Config();
	GPIO_clock_INIT();
	UART2_INIT();
	SCANF_Init();
	UART2_test();
	Timer2_Init();
	SYSTICK_Init();
	MCO1_CONFIG();
	// TEST SPI
	SPI_Init();
	SX1261_Init();
	SPI_Test( 0x01);
	// Initialiser le RTC
    RTC_Init();
	// Configurer l'heure : 12:34:56
	RTC_SetTime(12, 34, 56);
	// Boucle principale
	uint8_t hours, minutes, seconds;

while(1){
	/****TIMER TEST MS ***/
	//timer_test();

	/**SYSTIC TIMER  APPLICATION**/
	//LED_BLINK();
	 // Lire l'heure actuelle
	 RTC_GetTime(&hours, &minutes, &seconds);
	 // Insérez ici votre code UART pour afficher les heures, minutes et secondes
	   printf("Time: %02d:%02d:%02d\r\n", hours, minutes, seconds);

	  // Petite temporisation pour éviter les rafraîchissements trop rapides
	 for (volatile int i = 0; i < 1000000; i++);


}
}











