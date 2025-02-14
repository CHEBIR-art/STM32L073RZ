/*
 * sx1261.c
 *
 *  Created on: Nov 28, 2024
 *      Author: user
 */

#include "sx1261.h"
#include <stdio.h>
#include "spi.h"
#include "sx1261.h"
#include "systick.h"
#define NSS_LOW()  (GPIOA->ODR &= ~GPIO_ODR_OD4) // NSS = PA4
#define NSS_HIGH() (GPIOA->ODR |= GPIO_ODR_OD4)


uint8_t SX1261_ReadRegister(uint16_t regAddr) {
    uint8_t receivedData;

    SX1261_WaitWhileBusy();  // Vérifie si le module est occupé

    NSS_LOW();  // Active la communication SPI

    SPI1_TransmitReceive(readOP);  // Commande ReadRegister
    SPI1_TransmitReceive((regAddr >> 8) & 0xFF);  // MSB adresse
    SPI1_TransmitReceive(regAddr & 0xFF);  // LSB adresse
    SPI1_TransmitReceive(0x00);  // NOP (nécessaire pour l'horloge)

    while (!(SPI1->SR & SPI_SR_RXNE));  // Attendre la réception
      receivedData = SPI1->DR;  // Lire la donnée reçue


    receivedData = SPI1_TransmitReceive(0x00);  // Lire la donnée

    NSS_HIGH();  // Désactive le module SPI

    return receivedData;
}

void SX1261_WriteRegister(uint16_t RegAddr, uint8_t data2WRITE) {
	 SX1261_WaitWhileBusy();
	 NSS_LOW();  // Active la communication SPI
    SPI1_TransmitReceive(writeOP); // Commande WriteRegister
    SPI1_TransmitReceive((RegAddr >> 8) & 0xFF); // Byte haut de l'adresse
    SPI1_TransmitReceive(RegAddr & 0xFF); // Byte bas de l'adresse
    SPI1_TransmitReceive(data2WRITE); // Envoyer la donnée
    while (!(SPI1->SR & SPI_SR_RXNE));  // Attendre la réception
     uint8_t  receivedData = SPI1->DR;  // Lire la donnée reçue
      NSS_HIGH();  // Désactive le module SPI
}

void SX1261_WriteBuffer(uint8_t offset, uint8_t *data2WRITE, uint8_t size) {


	  SX1261_WaitWhileBusy();

	NSS_LOW (); // Activate the SPI slave.
    SPI1_TransmitReceive(writeBUFFERop); // Send the WriteBuffer command.
    SPI1_TransmitReceive(offset); // Send the FIFO offset.
    for (uint8_t i = 0; i < size; i++) {
        SPI1_TransmitReceive(data2WRITE[i]); // Write data to the FIFO.
    }
    while (SPI1->SR & SPI_SR_BSY); // Attendre la fin de l'opération
    NSS_HIGH(); // Deactivate the SPI slave.


}

void SX1261_ReadBuffer(uint8_t offset, uint8_t *data2READ, uint8_t size) {
	  SX1261_WaitWhileBusy();

	NSS_LOW (); // Activate the SPI slave.
    SPI1_TransmitReceive(readBUFFERop); // Send the ReadBuffer command.
    SPI1_TransmitReceive(offset); // Send the FIFO offset.
    SPI1_TransmitReceive(0x00); // Statut (nécessaire)

    for (uint8_t i = 0; i < size; i++) {
    	data2READ[i] = SPI1_TransmitReceive(0x00); // Read data from the FIFO.
    }
    NSS_HIGH(); // Deactivate the SPI slave.

}


/*
void SX1261_Test(void) {
    printf("Test de communication avec SX1261...\r\n");

   // SX1261_Reset();
   // SYSTICK_Delay(20);


    uint8_t mode = SX1261_ReadRegister(0x0902); // Lire le mode du SX1261
    printf("Mode actuel du SX1261 : 0x%02X\r\n", mode);

    if (mode == 0x00) {
        printf("✅ Le SX1261 est en mode Standby (SPI fonctionne bien).\r\n");
    } else {
        printf("⚠️ Problème possible : Mode inattendu (0x%02X) ! Vérifier câblage et SPI.\r\n", mode);
    }
    uint8_t testData = SX1261_ReadRegister(0x0889);
    printf("Valeur lue (0x0889) : 0x%02X\r\n", testData);
}*/



/*void SX1261_Init(void) {
    // Mettre NSS à HIGH au départ
    NSS_HIGH();

    // Ajouter un délai pour la mise sous tension du SX1261
    for (volatile int i = 0; i < 10000; i++);

    printf("SX1261 initialise.\r\n");

    //Vérifier que le module n'est pas occupé avant d'envoyer des commandes
        while (GPIOB->IDR & GPIO_IDR_ID3) {
            printf("SX1261 occupé, attente...\r\n");
        }

        printf("SX1261 prêt.\r\n");


	 // Mettre NSS à HIGH au départ
	    NSS_HIGH();

	    // Ajouter un délai pour la mise sous tension du SX1261
	    for (volatile int i = 0; i < 10000; i++);

	    printf("SX1261 initialise.\r\n");

	    // Vérifier que le module n'est pas occupé avant d'envoyer des commandes
	    int attempts = 0;
	    while (GPIOB->IDR & GPIO_IDR_ID3) {
	        attempts++;
	        if (attempts > 1000) {  // Limite du nombre de tentatives
	            printf("SX1261 occupé depuis trop longtemps, réinitialisation nécessaire.\r\n");
	            SX1261_Reset();  // Vous pouvez ajouter une fonction pour réinitialiser le module si nécessaire.
	            break;
	        }
	        printf("SX1261 occupé, attente...\r\n");
	        SYSTICK_Delay(10);  // Ajoutez un petit délai pour éviter une boucle infinie trop rapide
	    }

	    printf("SX1261 prêt.\r\n");
}*/

void SX1261_Init(void) {
    printf("Initialisation du SX1261...\r\n");

    // 1. Reset du module
    SX1261_Reset();

    // 2. Passer en mode Standby
    //SX1261_SetStandby();

    // 3. Configurer la fréquence radio (ex: 868 MHz)
    SX1261_WriteRegister(REG_RF_FREQ_MSB, 0xD9);
    SX1261_WriteRegister(REG_RF_FREQ_MID, 0x00);
    SX1261_WriteRegister(REG_RF_FREQ_LSB, 0x00);
    //printf("Fréquence radio configurée.\r\n");

    // 4. Configurer les paramètres de modulation (SF12, BW 125 kHz, CR 4/5)
    SX1261_WriteRegister(REG_MODULATION_PARAMS_SF, SF12);
    SX1261_WriteRegister(REG_MODULATION_PARAMS_BW, BW_125KHZ);
    SX1261_WriteRegister(REG_MODULATION_PARAMS_CR, CR_4_5);
    SX1261_WriteRegister(REG_MODULATION_PARAMS_LDRO, LDRO_ON);
   // printf("Paramètres de modulation configurés.\r\n");

    // 5. Définir la puissance d’émission (ex: 22 dBm)
    SX1261_WriteRegister(REG_TX_PARAMS, TX_POWER_22DBM);
   // printf("Puissance d’émission configurée.\r\n");

    // 6. Configurer le mot de synchronisation LoRaWAN
    SX1261_WriteRegister(REG_SYNC_WORD_MSB, LORAWAN_SYNC_WORD_MSB);
    SX1261_WriteRegister(REG_SYNC_WORD_LSB, LORAWAN_SYNC_WORD_LSB);
    //printf("Mot de synchronisation configuré.\r\n");

    printf("SX1261 initialisé avec succès !\r\n");
}



void SX1261_Test(void) {
    printf("Test de communication SPI avec SX1261...\r\n");
    SX1261_WaitWhileBusy();
    uint8_t mode1 = SX1261_ReadRegister(REG_OP_MODE);
    SX1261_WaitWhileBusy();
    uint8_t mode2 = SX1261_ReadRegister(REG_OP_MODE);

    printf("Valeur lue 1 : 0x%02X\r\n", mode1);
    printf("Valeur lue 2 : 0x%02X\r\n", mode2);

    if (mode1 == mode2 && mode1 == OP_MODE_STANDBY) {
        printf("✅ Test réussi : SX1261 en mode Standby.\r\n");
    } else {
        printf("❌ Erreur : Incohérence dans REG_OP_MODE !\r\n");
    }


   /* uint8_t rssi_raw = SX1261_ReadRegister(REG_RSSI_INST);
       int rssi_dbm = - (rssi_raw / 2);

       printf("RSSI mesuré : %d dBm\r\n", rssi_dbm);
       return rssi_dbm;*/

}

uint8_t SX1261_GetStatus(void) {
    uint8_t status;

    NSS_LOW();
    SPI1_TransmitReceive(0xC0);  // Commande GetStatus
    status = SPI1_TransmitReceive(0x00);  // Lire la réponse
    NSS_HIGH();

    printf("Statut du SX1261 : 0x%02X\r\n", status);
    return status;
}






void SX1261_Reset(void) {
	 printf("Renitialisation du SX1261...\r\n");
	    // Effectuez ici les actions nécessaires pour réinitialiser le module
	    NSS_LOW();
	    SYSTICK_Delay(10);  // Attendre suffisamment longtemps pour réinitialiser le module
	    NSS_HIGH();
	    SYSTICK_Delay(100);   // Attendre un peu avant de continuer
}


void SX1261_WaitWhileBusy(void) {
	 int timeout = 100;  // Timeout après 1000 itérations (10ms chacune)
	    while (GPIOB->IDR & GPIO_IDR_ID3) {  // Attendre que PB3 (BUSY) soit LOW
	        if (--timeout <= 0) {
	            printf("Timeout : Le module SX1261 est toujours occupé !\r\n");
	            return;  // Sortir si le timeout est atteint
	        }
	        SYSTICK_Delay(10);  // Attendre 10ms pour éviter une boucle trop rapide
	    }
}

void SX1261_SetStandby(void) {

	 SX1261_WaitWhileBusy();
    // Écrire dans le registre de mode pour passer en mode Standby
    SX1261_WriteRegister(REG_OP_MODE, OP_MODE_STANDBY);
    printf("Le module SX1261 est passe en mode Standby.\r\n");

    SYSTICK_Delay(10);  // Délai de 10 ms
}










void SX1261_LORA_Init(void) {
	 SX1261_SetStandby();
	// Configuration de la modulation LoRa (SF12, BW 125kHz, CR 4/5, LDRO ON)
	SX1261_WriteRegister(REG_MODULATION_PARAMS_SF, SF12);
	SX1261_WriteRegister(REG_MODULATION_PARAMS_BW, BW_125KHZ);
	SX1261_WriteRegister(REG_MODULATION_PARAMS_CR, CR_4_5);
	SX1261_WriteRegister(REG_MODULATION_PARAMS_LDRO, LDRO_ON);

	// Configuration de la fréquence 868 MHz
	SX1261_WriteRegister(REG_RF_FREQ_MSB, 0x6C);
	SX1261_WriteRegister(REG_RF_FREQ_MID, 0x80);
	SX1261_WriteRegister(REG_RF_FREQ_LSB, 0x00);

	// Configuration du Sync Word pour LoRaWAN
	SX1261_WriteRegister(REG_SYNC_WORD_MSB, LORAWAN_SYNC_WORD_MSB);
	SX1261_WriteRegister(REG_SYNC_WORD_LSB, LORAWAN_SYNC_WORD_LSB);

	// Configuration de la puissance d’émission à 22 dBm
	SX1261_WriteRegister(REG_TX_PARAMS, TX_POWER_22DBM);

	// Enable necessary interrupts (adjust mask as per application requirements)
	SX1261_WriteRegister(REG_IRQ_FLAGS_MASK, 0xF7); // Mask unnecessary interrupts

	// Set payload length (optional, based on application)
	SX1261_WriteRegister(REG_PAYLOAD_LENGTH, 0x40); // Payload length = 64 bytes (adjust as needed)

	// Clear all IRQ flags
	SX1261_WriteRegister(REG_IRQ_FLAGS, 0xFF); // Clear any pending interrupts

}
void SX1261_SimpleTest(void) {
    printf("\n--- Test de communication SX1261 ---\r\n");

    //  Reset du SX1261 avant test
    SX1261_Reset();
    SYSTICK_Delay(20);

    // ������ Lire un registre connu (ex: 0x0902 = mode du SX1261)
    uint8_t mode = SX1261_ReadRegister(0x0902);

    printf("Mode du SX1261 (0x0902) : 0x%02X\r\n", mode);

    // Vérification du mode attendu
    if (mode == 0x00 || mode == 0x01) {
        printf("Success :Communication SPI avec SX1261 OK !\r\n");
    } else {
        printf(" Erreur : mode inattendu (0x%02X). Vérifie câblage et SPI.\r\n", mode);
    }
}

void SX1261_TestWriteRead(void) {
    uint8_t testReg = 0x000D;  //  Choix d'un registre libre pour le test
    uint8_t testValue = 0xAA; //  Valeur à écrire
    uint8_t readValue = 0x00; //  Valeur lue après écriture

    printf("\n--- Test Write/Read SX1261 ---\r\n");

    // ������ Écriture du byte dans le registre
    SX1261_WriteRegister(testReg, testValue);
    printf("✅ Écriture : 0x%02X -> Registre 0x%02X\r\n", testValue, testReg);

    // SMALL DELAY TO MAKE SURE THE WRITING IS DONE
    SYSTICK_Delay(10);

    // READ THE REGISTER
    readValue = SX1261_ReadRegister(testReg);
    printf("������ Lecture du registre 0x%02X : 0x%02X\r\n", testReg, readValue);

    // ������ Vérification
    if (readValue == testValue) {
        printf("������ SUCCÈS : Valeur correcte reçue !\r\n");
    } else {
        printf("⚠️ ERREUR : Valeur inattendue (0x%02X). Vérifier SPI et registre.\r\n", readValue);
    }
}



/*******      PROBLEEEEEEEEEEEEEEM !!!!!!!!     **********/



void SX1261_LORA_test(void) {
    // Charger le message dans le FIFO (ex. "Hello LoRaWAN")
    SPI1_TransmitReceive(0x0800); // Adresse du FIFO
    SPI1_TransmitReceive('H');
    SPI1_TransmitReceive('e');
    SPI1_TransmitReceive('l');
    SPI1_TransmitReceive('l');
    SPI1_TransmitReceive('o');
    SPI1_TransmitReceive(' ');
    SPI1_TransmitReceive('L');
    SPI1_TransmitReceive('o');
    SPI1_TransmitReceive('R');
    SPI1_TransmitReceive('a');
    SPI1_TransmitReceive('W');
    SPI1_TransmitReceive('A');
    SPI1_TransmitReceive('N');


    // Attendre que le module ne soit pas occupé avant d'envoyer des données
       SX1261_WaitWhileBusy();


    // Lancer la transmission avec la commande TX (SetTx)
   // SPI1_TransmitReceive(0x83); // Commande TX
   // SPI1_TransmitReceive(0x00); // Paramètre (ajuster selon besoin)
   // SPI1_TransmitReceive(0x00);
    //SPI1_TransmitReceive(0x00);

    // Vérifier si l'IRQ de fin de transmission (TX_DONE) est activé
    uint8_t irqFlags = SPI1_TransmitReceive(REG_IRQ_FLAGS);
    if (irqFlags & 0x08) { // Si TX_DONE est activé
        printf("Transmission réussie!\n");
        // Effacer l'IRQ
        SPI1_TransmitReceive(REG_IRQ_FLAGS);
    }


    // Attendre que le module ne soit pas occupé avant d'envoyer des données
       SX1261_WaitWhileBusy();


    // Lancer la réception (SetRx)
    SPI1_TransmitReceive(0x82); // Commande RX
   // SPI1_TransmitReceive(0x00); // Paramètre (ajuster selon besoin)
   // SPI1_TransmitReceive(0x00);
   // SPI1_TransmitReceive(0x00);

    // Lire le message depuis le FIFO
    uint8_t receivedMessage[64]; // Assumer un max de 64 octets
    for (int i = 0; i < 64; i++) {
        receivedMessage[i] = SPI1_TransmitReceive(0x0800 + i);
    }
    printf("Message reçu : %s\n", receivedMessage);

    // Vérifier si l'IRQ RX_DONE est activé
    irqFlags = SPI1_TransmitReceive(REG_IRQ_FLAGS);
    if (irqFlags & 0x20) { // Si RX_DONE est activé
        printf("Réception réussie!\n");
        // Effacer l'IRQ
        SPI1_TransmitReceive(REG_IRQ_FLAGS);
    }
}

