
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
#include "watchdog.h"
#include "rng.h"

uint8_t receive[2] = {0};

int main(void)
{

	SystemClock_Config();
	GPIO_clock_INIT();
	UART2_INIT();
	SCANF_Init();
	UART2_test();
	//IWDG_Init();
	LSI_Enable();
	Timer2_Init();
	SYSTICK_Init();
	RTC_Init();
	SPI_Init();
	SX1261_Init();
	MCO1_CONFIG();
	rng_init();
	//test_rng();


	//test_readregister();





/******TEST OF WATCHDOG*****/
	 // Vérification de la cause du reset
	// Check_Reset_Cause();

	 //Check_LSI_Status() ;

	  //printf("Watchdog activé !\r\n");





while(1){

	/****TIMER TEST MS ***/
	//timer_test();



	/**SYSTIC TIMER  APPLICATION**/
	//LED_BLINK();


	/*****Test de spi en loopback*****/

	//SPI_LoopbackTest(0x52);
	//SYSTICK_Delay(500);

	/*****RTC TEST*******/
	    // RTC_test ();



	/****Test de watchdog*****/

	 // printf("Système fonctionnel, Watchdog rafraîchi !\r\n");
	//Feed_IWDG(); // Réinitialiser le Watchdog
 	// SYSTICK_Delay(2000); // Pause de 2 seconde <8 secondes*/



	/****Test de sx1261 communication ***/

	//SX1261_TestCommunication();
	//SYSTICK_Delay(5000);
}
}
