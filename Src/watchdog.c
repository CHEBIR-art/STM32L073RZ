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
    // Activer l'IWDG (d�marrage du watchdog)
    IWDG->KR = 0xCCCC;

    // D�verrouiller l'acc�s aux registres PR et RLR
    IWDG->KR = 0x5555;

    // Attendre que le bit PVU soit remis � 0 avant de modifier le prescaler
    while (IWDG->SR & (1 << 0)) {}

    // Configurer le prescaler : division par 256
    IWDG->PR = 0x6;

    // Attendre que l'op�ration soit termin�e
    while (IWDG->SR & (1 << 0)) {}

    // Configurer la valeur de rechargement
    IWDG->RLR =0x0484; // timeout of 8s   en decimal = 1156 F-IWDG = LSI + 37kHz

    // Assurer la prise en compte de la nouvelle configuration

    IWDG->KR = 0xAAAA; // Rafra�chit le compteur (�vite un reset imm�diat)
}



void Feed_IWDG(void) {
    IWDG->KR = 0xAAAA; // R�alimenter le Watchdog
}
void Check_Reset_Cause(void) {
    if (RCC->CSR & RCC_CSR_IWDGRSTF) { // V�rifie si le reset vient du Watchdog
        // Effacer le drapeau IWDGRSTF
        RCC->CSR |= RCC_CSR_RMVF;

        // Vous pouvez indiquer via UART ou LED que le reset vient du Watchdog
        printf("le reset vient du Watchdog\r\t");
        // Exemple : allumer une LED pour une indication
    }
}


