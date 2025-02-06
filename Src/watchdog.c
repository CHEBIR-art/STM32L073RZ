/*
 * watchdog.c
 *
 *  Created on: Jan 23, 2025
 *      Author: user
 */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "stm32l073xx.h"
#include "watchdog.h"

void IWDG_Init(void) {
    // Activer l'accès à l'IWDG
    IWDG->KR = 0xCCCC; // Activer l'IWDG

    // Configurer la période de timeout
    IWDG->KR = 0x5555; // Activer l'accès aux registres de configuration
    IWDG->PR = 0x6;    // Diviseur de précharge : divise par 256
    IWDG->RLR = 0xFFF; // Reload register (timeout max : environ 32 s)

    // Redémarrer le compteur
    IWDG->KR = 0xAAAA; // Alimenter le Watchdog
}


void Feed_IWDG(void) {
    IWDG->KR = 0xAAAA; // Réalimenter le Watchdog
}
void Check_Reset_Cause(void) {
    if (RCC->CSR & RCC_CSR_IWDGRSTF) { // Vérifie si le reset vient du Watchdog
        // Effacer le drapeau IWDGRSTF
        RCC->CSR |= RCC_CSR_RMVF;

        // Vous pouvez indiquer via UART ou LED que le reset vient du Watchdog
        printf("le reset vient du Watchdog\r\t");
        // Exemple : allumer une LED pour une indication
    }
}


