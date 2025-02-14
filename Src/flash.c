/*
 * flash.c
 *
 *  Created on: Feb 14, 2025
 *      Author: user
 */
#include "stm32l073xx.h"
#include "utilities.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "flash.h"
#include "utilities.h"


#define FLASH_STORAGE_ADDR  ((uint32_t)0x080FF800) // Adresse dans la Flash

#define FLASH_USER_START_ADDR  ((uint32_t)0x080FF800) // Adresse de d�but d'une page Flash
#define FLASH_USER_END_ADDR    ((uint32_t)0x08100000) // Fin de la page
#define FLASH_PAGE_SIZE    1024   // 1 Ko par page


void Flash_Unlock(void) {
    // D�verrouillage de la m�moire Flash
    if ((FLASH->PECR & FLASH_PECR_PELOCK) != 0) {
        FLASH->PEKEYR = 0x89ABCDEF;
        FLASH->PEKEYR = 0x02030405;
    }
}

void Flash_Lock(void) {
    // Verrouillage de la m�moire Flash
    FLASH->PECR |= FLASH_PECR_PELOCK;
}

void Flash_ErasePage(uint32_t PageAddress) {
    Flash_Unlock();

    // V�rifier si on est bien align� sur une page
    if (PageAddress % FLASH_PAGE_SIZE != 0) {
        return; // Adresse invalide
    }

    // Attente de disponibilit�
    while (FLASH->SR & FLASH_SR_BSY);

    // Effacement de la page
    FLASH->PECR |= FLASH_PECR_ERASE;
    FLASH->PECR |= FLASH_PECR_PROG;
    *(volatile uint32_t *)PageAddress = 0x00000000;

    // Attente de la fin de l'effacement
    while (FLASH->SR & FLASH_SR_BSY);

    FLASH->PECR &= ~FLASH_PECR_PROG;
    FLASH->PECR &= ~FLASH_PECR_ERASE;

    Flash_Lock();
}

void Flash_WriteDoubleWord(uint32_t Address, uint64_t Data) {
    Flash_Unlock();

    // Attente de disponibilit�
    while (FLASH->SR & FLASH_SR_BSY);

    FLASH->PECR |= FLASH_PECR_PROG;  // Activer le mode programmation

    // �crire le double-mot (64 bits) dans la Flash
    *(volatile uint64_t*)Address = Data;

    // Attente de la fin de l'�criture
    while (FLASH->SR & FLASH_SR_BSY);

    FLASH->PECR &= ~FLASH_PECR_PROG;  // D�sactiver le mode programmation

    Flash_Lock();
}
// lecture de la flash


uint64_t Flash_ReadDoubleWord(uint32_t Address) {
    return *(volatile uint64_t*)Address;
}



void SaveFrameCounter(uint32_t up, uint32_t down) {
    uint64_t data = ((uint64_t)up << 32) | down;  // Stocker les deux compteurs dans un seul double-mot (64 bits)
    Flash_ErasePage(FLASH_STORAGE_ADDR);         // Effacer la page avant �criture
    Flash_WriteDoubleWord(FLASH_STORAGE_ADDR, data);
}

void LoadFrameCounter(uint32_t *up, uint32_t *down) {
    uint64_t data = Flash_ReadDoubleWord(FLASH_STORAGE_ADDR);
    *up = (uint32_t)(data >> 32);   // Extraire FCntUp
    *down = (uint32_t)data;         // Extraire FCntDown
}




// flash test

void TestFlash(void) {
    uint32_t FCntUp = 1000;  // Exemple de valeur FCntUp
    uint32_t FCntDown = 2000;  // Exemple de valeur FCntDown

    // Afficher avant l'enregistrement
    printf("Avant enregistrement - FCntUp: %lu, FCntDown: %lu\r\n", FCntUp, FCntDown);

    // Enregistrer dans la Flash
    SaveFrameCounter(FCntUp, FCntDown);

    // Simuler un red�marrage ou une remise � z�ro du syst�me

    // Lire depuis la Flash
    uint32_t loadedFCntUp = 0;
    uint32_t loadedFCntDown = 0;
    LoadFrameCounter(&loadedFCntUp, &loadedFCntDown);

    // Afficher apr�s lecture
    printf("Apr�s lecture - FCntUp: %lu, FCntDown: %lu\n", loadedFCntUp, loadedFCntDown);

    // V�rification simple des valeurs
    if (FCntUp == loadedFCntUp && FCntDown == loadedFCntDown) {
        printf("Test Flash r�ussi !\n");
    } else {
        printf("Test Flash �chou� !\n");
    }
}
