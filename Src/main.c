
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
#include "flash.h"




int main(void)
{

	SystemClock_Config();
	GPIO_clock_INIT();
	UART2_INIT();
	SCANF_Init();
	UART2_test();
	LSI_Enable();
	Timer2_Init();
	SYSTICK_Init();
	RTC_Init();


	  TestFlash();


	//SPI_Init();
	//SX1261_Init();
	//MCO1_CONFIG();
	// SX1261_LORA_Init();

	// IWDG_Init();
	// SX1261_Test();
	// SX1261_GetStatus();
/*      // communication test
	 SX1261_Test();

   	SX1261_SimpleTest();
	SX1261_TestWriteRead();// read and write test between the master and the slave
	SX1261_WriteRegister(0x06C0, 0xBB);

	uint8_t val = SX1261_ReadRegister(0x06C0);
	printf("Valeur lue : 0x%02X\r\n", val);

*/
	 //SPI_Test( 0x52);
	 //SPI_LoopbackTest( 0x52);






/*TEST OF WATCHDOG
	 // Vérification de la cause du reset
	 Check_Reset_Cause();

	 Check_LSI_Status() ;

	  printf("Watchdog activé !\r\n");*/





while(1){
	/****TIMER TEST MS ***/
	//timer_test();



	/**SYSTIC TIMER  APPLICATION**/
	//LED_BLINK();
	 //SX1261_Test();


	//SPI_LoopbackTest(0x52);
	//SYSTICK_Delay(500);

	/*     RTC TEST        */
	     //RTC_test ();



	/*    test de watchdog   */
	 // Simuler une tâche (par ex. envoi d’un message LoRa toutes les 2s)
	 //  SX1261_LORA_test();
	  //printf("Système fonctionnel, Watchdog rafraîchi !\r\n");
	        Feed_IWDG(); // Réinitialiser le Watchdog
	        SYSTICK_Delay(2000); // Pause de 2 seconde <8 secondes
}
}

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
	/* initialisation */
	SPI_Init();
	SX1261_Init();

	/* communication test */
	 //SX1261_Test();

   //SX1261_SimpleTest();
	SX1261_TestWriteRead();// read and write test between the master and the slave
	//SX1261_WriteRegister(0x06C0, 0xBB);

	//uint8_t val = SX1261_ReadRegister(0x06C0);
	//printf("Valeur lue : 0x%02X\r\n", val);


	  // Tester la communication LoRa une seule fois
	   // SX1261_LORA_test();

	//SPI_Test( 0x52);

/*TEST OF WATCHDOG

	IWDG_Init();
	Check_Reset_Cause();

	
	    // test de rtc 
    RTC_Init();
	RTC_SetTime(12, 34, 56);// Configurer l'heure : 12:34:56




	//uint8_t hours, minutes, seconds;

while(1){
	/****TIMER TEST MS ***/
	//timer_test();

	/**SYSTIC TIMER  APPLICATION**/
	//LED_BLINK();
	 //SX1261_Test();

	//SPI_LoopbackTest(0x52);
	//SYSTICK_Delay(500);


	/******RTC TEST ****/

	 // Lire l'heure actuelle
	// RTC_GetTime(&hours, &minutes, &seconds);
	 // Code UART pour afficher les heures, minutes et secondes
	 // printf("Time: %02d:%02d:%02d\r\n", hours, minutes, seconds);



	  // Petite temporisation pour éviter les rafraîchissements trop rapides

	// for (volatile int i = 0; i < 1000000; i++);
	Feed_IWDG();// Realimenter le watchdog

}
}











