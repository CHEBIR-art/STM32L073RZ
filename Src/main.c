
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


#include "rtc.h"



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
	//SPI TEST
	SPI_Init();
	//SX1261_Init();
	//SPI_Test( 0x52);

/*TEST OF WATCHDOG

	//IWDG_Init();
	//Check_Reset_Cause();

	*/
    RTC_Init();
	RTC_SetTime(12, 34, 56);// Configurer l'heure : 12:34:56




	uint8_t hours, minutes, seconds;

while(1){
	/****TIMER TEST MS ***/
	//timer_test();

	/**SYSTIC TIMER  APPLICATION**/
	//LED_BLINK();

	//SPI_LoopbackTest(0x52);
	//SYSTICK_Delay(500);
	/******RTC TEST ****/


	 // Lire l'heure actuelle
	 RTC_GetTime(&hours, &minutes, &seconds);
	 // Code UART pour afficher les heures, minutes et secondes
	  printf("Time: %02d:%02d:%02d\r\n", hours, minutes, seconds);



	  // Petite temporisation pour éviter les rafraîchissements trop rapides

	 for (volatile int i = 0; i < 1000000; i++);
	//Feed_IWDG();// Realimenter le watchdog

}
}











