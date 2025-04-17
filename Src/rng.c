#include "stm32l073xx.h"
#include <stdio.h>
#include <stdint.h>
#include "utilities.h"

void rng_init(void) {
    // Activer l'horloge du RNG en activant le bit RNGEN dans RCC_AHBENR
    RCC->AHBENR |= RCC_AHBENR_RNGEN;

    // Activer le g�n�rateur de nombres al�atoires
    RNG->CR |= RNG_CR_RNGEN;
}

uint32_t rng_get_random(void) {
    // Attendre que les donn�es soient pr�tes (DRDY == 1)

    while (!(RNG->SR & RNG_SR_DRDY));

    // V�rifier s'il y a une erreur (CECS ou SECS)
    if (RNG->SR & (RNG_SR_CECS | RNG_SR_SECS)) {
        return 0;  // Erreur d�tect�e, gestion personnalis�e possible
    }

    // Lire et retourner le nombre al�atoire
    return RNG->DR;
}

uint32_t rng_get_random_in_range(uint32_t min, uint32_t max) {
    if (min >= max) {
        uint32_t temp = min;
        min = max;
        max = temp;
    }
    return (rng_get_random() % (max - min + 1)) + min;
}

int32_t rng_get_signed_random_in_range(int32_t min, int32_t max) {
    if (min >= max) {
        int32_t temp = min;
        min = max;
        max = temp;
    }
    return (int32_t)(rng_get_random() % (max - min + 1)) + min;
}



void test_rng(void) {
    char  buffer[50];  // Buffer pour stocker les r�sultats � envoyer via UART
    uint32_t rand_num;

    // Afficher un message pour commencer le test
    printf("Test RNG commenc�...\n");

    // Faire un test sur 10 nombres al�atoires
    for (int8_t i = 0; i < 10; i++) {
        rand_num = rng_get_random();  // R�cup�rer un nombre al�atoire
        // Convertir le nombre al�atoire en cha�ne et l'envoyer via UART
        sprintf(buffer, "Random Number %d: %lu\n", i+1, rand_num);  // Convertir en cha�ne
        printf("%s", buffer);  // Envoyer via UART en utilisant buffer
    }

    // Afficher un message de fin de test
    printf("Test RNG termin�.\n");
}
