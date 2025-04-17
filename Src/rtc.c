/*
 * rtc.c
 *
 *  Created on: Jan 22, 2025
 *      Author: khouloud
 *
 */
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "rtc.h"
#include "stm32l073xx.h"

void RTC_Init(void) {
    // Activer l'horloge pour le PWR (Power Control) et le RTC
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_DBP; // Activer l'acc�s aux registres du RTC

    // Configurer la source d'horloge RTC sur LSE
    RCC->CSR |= RCC_CSR_LSEON;          // Activer l'oscillateur LSE
    while (!(RCC->CSR & RCC_CSR_LSERDY)); // Attendre que LSE soit pr�t

    RCC->CSR |= RCC_CSR_RTCSEL_LSE;    // S�lectionner LSE comme source d'horloge RTC
    RCC->CSR |= RCC_CSR_RTCEN;         // Activer le RTC

    // Configurer le RTC
    RTC->WPR = 0xCA; // D�sactiver la protection d'�criture
    RTC->WPR = 0x53;
    RTC->ISR |= RTC_ISR_INIT; // Mettre le RTC en mode INIT
    while (!(RTC->ISR & RTC_ISR_INITF)); // Attendre le mode INIT

    RTC->PRER = (127 << 16) | 255; // Pr�diviseur: 127 pour synchrone, 255 pour asynchrone
    RTC->ISR &= ~RTC_ISR_INIT; // Quitter le mode INIT
    RTC->WPR = 0xFF; // R�activer la protection d'�criture
}

void RTC_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    RTC->WPR = 0xCA; // D�sactiver la protection d'�criture
    RTC->WPR = 0x53;
    RTC->ISR |= RTC_ISR_INIT; // Mettre le RTC en mode INIT
    while (!(RTC->ISR & RTC_ISR_INITF)); // Attendre le mode INIT

    RTC->TR = ((hours / 10) << 20) | ((hours % 10) << 16) |
              ((minutes / 10) << 12) | ((minutes % 10) << 8) |
              ((seconds / 10) << 4) | (seconds % 10);

    RTC->ISR &= ~RTC_ISR_INIT; // Quitter le mode INIT
    RTC->WPR = 0xFF; // R�activer la protection d'�criture
}

void RTC_GetTime(uint8_t *hours, uint8_t *minutes, uint8_t *seconds) {
    uint32_t tr = RTC->TR;
    *hours = ((tr >> 20) & 0x3) * 10 + ((tr >> 16) & 0xF);
    *minutes = ((tr >> 12) & 0x7) * 10 + ((tr >> 8) & 0xF);
    *seconds = ((tr >> 4) & 0x7) * 10 + (tr & 0xF);
}
void RTC_test (void){
	uint8_t hours, minutes, seconds;
		 // Lire l'heure actuelle
		RTC_SetTime(12, 34, 56);// Configurer l'heure : 12:34:56
		 RTC_GetTime(&hours, &minutes, &seconds);
		 // Code UART pour afficher les heures, minutes et secondes
		  printf("Time: %02d:%02d:%02d\r\n", hours, minutes, seconds);
		  // Petite temporisation pour �viter les rafra�chissements trop rapides

		  	 for (volatile int i = 0; i < 1000000; i++);
}

void RTC_SetWakeup(uint32_t seconds) {
    // D�sactiver la protection d'�criture
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    // D�sactiver le Wakeup Timer avant de le configurer
    RTC->CR &= ~RTC_CR_WUTE;
    while (RTC->ISR & RTC_ISR_WUTWF); // Attendre que le WUTWF soit r�gl�

    // Configurer le Wakeup Timer
    RTC->WUTR = seconds - 1; // Dur�e en secondes
    RTC->CR |= RTC_CR_WUTIE; // Activer les interruptions pour le Wakeup Timer
    RTC->CR |= RTC_CR_WUTE;  // Activer le Wakeup Timer

    // R�activer la protection d'�criture
    RTC->WPR = 0xFF;

    // Activer les interruptions dans le NVIC
    NVIC_EnableIRQ(RTC_IRQn);
}

// Handler de l'interruption du Wakeup Timer
void RTC_WKUP_IRQHandler(void) {
    if (RTC->ISR & RTC_ISR_WUTF) {
        RTC->ISR &= ~RTC_ISR_WUTF; // Effacer le flag d'interruption
        // Votre code pour transmettre les donn�es LoRaWAN ici
    }
}
void EnterStopMode(void) {
    // Configurer le microcontr�leur pour le mode Stop
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; // Activer le mode profond (deep sleep)
    PWR->CR |= PWR_CR_CWUF;           // Effacer les flags de r�veil
    __WFI();                          // Entrer en mode veille
}


