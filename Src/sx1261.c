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
    SX1261_WaitWhileBusy();
    SPI1_TransmitReceive(readOP); // Commande ReadRegister
    SPI1_TransmitReceive((regAddr >> 8) & 0xFF); // Byte haut de l'adresse
    SPI1_TransmitReceive(regAddr & 0xFF); // Byte bas de l'adresse
    SPI1_TransmitReceive(0x00); // Ì†ΩÌ¥¥ Byte NOP obligatoire pour permettre la r√©ponse
    receivedData = SPI1_TransmitReceive(0x00); // Ì†ΩÌ¥ç Lire la donn√©e

    return receivedData;
}


void SX1261_WriteRegister(uint16_t RegAddr, uint8_t data2WRITE) {
	 SX1261_WaitWhileBusy();
    SPI1_TransmitReceive(writeOP); // Commande WriteRegister
    SPI1_TransmitReceive((RegAddr >> 8) & 0xFF); // Byte haut de l'adresse
    SPI1_TransmitReceive(RegAddr & 0xFF); // Byte bas de l'adresse
    SPI1_TransmitReceive(data2WRITE); // Envoyer la donn√©e
    SYSTICK_Delay(1);
}

void SX1261_WriteBuffer(uint8_t offset, uint8_t *data2WRITE, uint8_t size) {
	NSS_LOW (); // Activate the SPI slave.
    SPI1_TransmitReceive(writeBUFFERop); // Send the WriteBuffer command.
    SPI1_TransmitReceive(offset); // Send the FIFO offset.
    for (uint8_t i = 0; i < size; i++) {
        SPI1_TransmitReceive(data2WRITE[i]); // Write data to the FIFO.
    }
    NSS_HIGH(); // Deactivate the SPI slave.
}

void SX1261_ReadBuffer(uint8_t offset, uint8_t *data2READ, uint8_t size) {
	NSS_LOW (); // Activate the SPI slave.
    SPI1_TransmitReceive(readBUFFERop); // Send the ReadBuffer command.
    SPI1_TransmitReceive(offset); // Send the FIFO offset.
    for (uint8_t i = 0; i < size; i++) {
    	data2READ[i] = SPI1_TransmitReceive(0x00); // Read data from the FIFO.
    }
    NSS_HIGH(); // Deactivate the SPI slave.

}
void SX1261_Test(void) {
    printf("Test de communication avec SX1261...\r\n");

   // SX1261_Reset();
   // SYSTICK_Delay(20);


    uint8_t mode = SX1261_ReadRegister(0x0902); // Lire le mode du SX1261
    printf("Mode actuel du SX1261 : 0x%02X\r\n", mode);

    if (mode == 0x00) {
        printf("‚úÖ Le SX1261 est en mode Standby (SPI fonctionne bien).\r\n");
    } else {
        printf("‚ö†Ô∏è Probl√®me possible : Mode inattendu (0x%02X) ! V√©rifier c√¢blage et SPI.\r\n", mode);
    }
    uint8_t testData = SX1261_ReadRegister(0x0889);
    printf("Valeur lue (0x0889) : 0x%02X\r\n", testData);
}

void SX1261_Init(void) {
    // Mettre NSS √† HIGH au d√©part
    NSS_HIGH();

    // Ajouter un d√©lai pour la mise sous tension du SX1261
    for (volatile int i = 0; i < 10000; i++);

    printf("SX1261 initialise.\r\n");

    //V√©rifier que le module n'est pas occup√© avant d'envoyer des commandes
        while (GPIOB->IDR & GPIO_IDR_ID3) {
            printf("SX1261 occup√©, attente...\r\n");
        }

        printf("SX1261 pr√™t.\r\n");
}
void SX1261_Reset(void) {
    NSS_HIGH();
    SYSTICK_Delay(10);
    NSS_LOW();
    SYSTICK_Delay(10);
    NSS_HIGH();
    SYSTICK_Delay(10);

}

void SX1261_WaitWhileBusy(void) {
    while (GPIOB->IDR & GPIO_IDR_ID3);  // Attendre que PB3 (BUSY) soit LOW
}


void SX1261_LORA_Init(void) {
	// Configuration de la modulation LoRa (SF12, BW 125kHz, CR 4/5, LDRO ON)
	SX1261_WriteRegister(REG_MODULATION_PARAMS_SF, SF12);
	SX1261_WriteRegister(REG_MODULATION_PARAMS_BW, BW_125KHZ);
	SX1261_WriteRegister(REG_MODULATION_PARAMS_CR, CR_4_5);
	SX1261_WriteRegister(REG_MODULATION_PARAMS_LDRO, LDRO_ON);

	// Configuration de la fr√©quence 868 MHz
	SX1261_WriteRegister(REG_RF_FREQ_MSB, 0x6C);
	SX1261_WriteRegister(REG_RF_FREQ_MID, 0x80);
	SX1261_WriteRegister(REG_RF_FREQ_LSB, 0x00);

	// Configuration du Sync Word pour LoRaWAN
	SX1261_WriteRegister(REG_SYNC_WORD_MSB, LORAWAN_SYNC_WORD_MSB);
	SX1261_WriteRegister(REG_SYNC_WORD_LSB, LORAWAN_SYNC_WORD_LSB);

	// Configuration de la puissance d‚Äô√©mission √† 22 dBm
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

    // Ì†ΩÌ¥ç Lire un registre connu (ex: 0x0902 = mode du SX1261)
    uint8_t mode = SX1261_ReadRegister(0x0902);

    printf("Mode du SX1261 (0x0902) : 0x%02X\r\n", mode);

    // V√©rification du mode attendu
    if (mode == 0x00 || mode == 0x01) {
        printf("Success :Communication SPI avec SX1261 OK !\r\n");
    } else {
        printf(" Erreur : mode inattendu (0x%02X). V√©rifie c√¢blage et SPI.\r\n", mode);
    }
}

void SX1261_TestWriteRead(void) {
    uint8_t testReg = 0x000D;  // Ì†ΩÌ≥å Choix d'un registre libre pour le test
    uint8_t testValue = 0xAA; // Ì†ΩÌ≥© Valeur √† √©crire
    uint8_t readValue = 0x00; // Ì†ΩÌ≥• Valeur lue apr√®s √©criture

    printf("\n--- Test Write/Read SX1261 ---\r\n");

    // Ì†ΩÌ¥¥ √âcriture du byte dans le registre
    SX1261_WriteRegister(testReg, testValue);
    printf("‚úÖ √âcriture : 0x%02X -> Registre 0x%02X\r\n", testValue, testReg);

    // SMALL DELAY TO MAKE SURE THE WRITING IS DONE
    SYSTICK_Delay(10);

    // READ THE REGISTER
    readValue = SX1261_ReadRegister(testReg);
    printf("Ì†ΩÌ≥• Lecture du registre 0x%02X : 0x%02X\r\n", testReg, readValue);

    // Ì†ΩÌ≥å V√©rification
    if (readValue == testValue) {
        printf("Ì†ºÌæØ SUCC√àS : Valeur correcte re√ßue !\r\n");
    } else {
        printf("‚ö†Ô∏è ERREUR : Valeur inattendue (0x%02X). V√©rifier SPI et registre.\r\n", readValue);
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

    // Lancer la transmission avec la commande TX (SetTx)
    SPI1_TransmitReceive(0x83); // Commande TX
    SPI1_TransmitReceive(0x00); // Param√®tre (ajuster selon besoin)
    SPI1_TransmitReceive(0x00);
    SPI1_TransmitReceive(0x00);

    // V√©rifier si l'IRQ de fin de transmission (TX_DONE) est activ√©
    uint8_t irqFlags = SPI1_TransmitReceive(REG_IRQ_FLAGS);
    if (irqFlags & 0x08) { // Si TX_DONE est activ√©
        printf("Transmission r√©ussie!\n");
        // Effacer l'IRQ
        SPI1_TransmitReceive(REG_IRQ_FLAGS);
    }

    // Lancer la r√©ception (SetRx)
    SPI1_TransmitReceive(0x82); // Commande RX
    SPI1_TransmitReceive(0x00); // Param√®tre (ajuster selon besoin)
    SPI1_TransmitReceive(0x00);
    SPI1_TransmitReceive(0x00);

    // Lire le message depuis le FIFO
    uint8_t receivedMessage[64]; // Assumer un max de 64 octets
    for (int i = 0; i < 64; i++) {
        receivedMessage[i] = SPI1_TransmitReceive(0x0800 + i);
    }
    printf("Message re√ßu : %s\n", receivedMessage);

    // V√©rifier si l'IRQ RX_DONE est activ√©
    irqFlags = SPI1_TransmitReceive(REG_IRQ_FLAGS);
    if (irqFlags & 0x20) { // Si RX_DONE est activ√©
        printf("R√©ception r√©ussie!\n");
        // Effacer l'IRQ
        SPI1_TransmitReceive(REG_IRQ_FLAGS);
    }
}

