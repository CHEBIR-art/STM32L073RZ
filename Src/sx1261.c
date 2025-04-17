

#include "sx1261.h"
#include <stdio.h>
#include "spi.h"
#include "sx1261.h"
#include "systick.h"
#include "utilities.h"
#define NSS_LOW()  (GPIOA->ODR &= ~GPIO_ODR_OD4) // NSS = PA4
#define NSS_HIGH() (GPIOA->ODR |= GPIO_ODR_OD4)
#define NRESET_LOW()   (GPIOA->BSRR = GPIO_BSRR_BR_0)  // Mettre NRESET à LOW  PA0
#define NRESET_HIGH()  (GPIOA->BSRR = GPIO_BSRR_BS_0)  // Mettre NRESET à HIGH PA0


extern uint8_t receive[2];



void WaitForBusyLow(void) {
	uint32_t timeout = 1000000;  // Nombre d'itérations avant d'abandonner

	    while ((GPIOB->IDR & GPIO_IDR_ID3) && (timeout-- > 0));

	    if (timeout == 0) {
	        printf("❌ Timeout : BUSY reste HIGH !\r\n");
	    } else {
	        printf("✅ BUSY est passé à LOW, le SX1261 est prêt.\r\n");
	    }
}

void SX1261_SetStandby(void) {

	 SX1261_WaitWhileBusy();
    // Écrire dans le registre de mode pour passer en mode Standby
    SX1261_WriteRegister(REG_OP_MODE, OP_MODE_STANDBY);
    printf("Le module SX1261 est passe en mode Standby.\r\n");

    SYSTICK_Delay(10);  // Délai de 10 ms
}


//     PROBLEEEEEEEEEEEEEEM !!!!!!!


void SX1261_Init() {

	  printf("�� Initialisation du SX1261...\r\n");
		  NRESET_LOW();  // Mettre NRESET à LOW (assumer que NRESET est sur PB3)
	    for (volatile int i = 0; i < 100000; i++); // Attendre >100µs
	    NRESET_HIGH(); // Mettre NRESET à HIGH

	    WaitForBusyLow();


	    // Envoyer la commande pour passer en mode STDBY_RC
	       printf("⚙️ Passage en mode STDBY_RC...\r\n");
	       NSS_LOW();
	          SPI1_TransmitReceive(0x80); // Commande SetStandby
	          SPI1_TransmitReceive(0x00); // Paramètre : mode RC
	       NSS_HIGH();
	       printf("✅ SX1261 initialisé avec succès !\r\n");


	       NSS_LOW();
	          SPI1_TransmitReceive(0xC0); // GetStatus
	          uint8_t status = SPI1_TransmitReceive(0x00); // Lecture du statut
	       NSS_HIGH();

	       printf("�� Statut SX1261: 0x%02X\r\n", status);
	       uint8_t mode = (status >> 5) & 0x07;
	       if (mode == 0x00) {
	          printf("✅ Le module est en mode STDBY_RC.\r\n");
	       } else {
	          printf("❌ Le module n'est pas en mode STDBY_RC (mode actuel : 0x%02X).\r\n", mode);
	       }

}




void SX1261_Send(uint8_t *data, uint8_t length) {
    // Attendre que BUSY soit bas
    //while (GPIOB->IDR & GPIO_IDR_ID3);

    // Définir l’adresse de départ du buffer TX (commande 0x8F, TxBaseAddr = 0x00)
    NSS_LOW();
    SPI1_TransmitReceive(0x8F);
    SPI1_TransmitReceive(0x00); // TxBaseAddr
    SPI1_TransmitReceive(0x00); // RxBaseAddr
    NSS_HIGH();

    // Écrire les données dans le buffer (commande 0x0E)
    NSS_LOW();
    SPI1_TransmitReceive(0x0E); // WriteBuffer command
    SPI1_TransmitReceive(0x00); // Offset = 0x00
    for (uint8_t i = 0; i < length; i++) {
        SPI1_TransmitReceive(data[i]); // Envoyer les données
    }
    NSS_HIGH();

    // Lancer la transmission (commande 0x83)
    NSS_LOW();
    SPI1_TransmitReceive(0x83); // SetTx command
    SPI1_TransmitReceive(0x00); // Timeout (23:16)
    SPI1_TransmitReceive(0x00); // Timeout (15:8)
    SPI1_TransmitReceive(0x00); // Timeout (7:0)
    NSS_HIGH();

    // Attendre l'IRQ TxDone sur DIO1
    while (!(GPIOB->IDR & GPIO_IDR_ID3));
}

void SX1261_Receive(uint8_t *buffer, uint8_t length) {
    // Attendre que BUSY soit bas
    while (GPIOB->IDR & GPIO_IDR_ID3);

    // Définir l'adresse de début du buffer RX (commande 0x8F)
    NSS_LOW();
    SPI1_TransmitReceive(0x8F);
    SPI1_TransmitReceive(0x00); // RxBaseAddr
    SPI1_TransmitReceive(0x00); // TxBaseAddr
    NSS_HIGH();

    // Passer en mode réception (commande 0x82)
    NSS_LOW();
    SPI1_TransmitReceive(0x82);
    SPI1_TransmitReceive(0x00); // Timeout (23:16)
    SPI1_TransmitReceive(0x00); // Timeout (15:8)
    SPI1_TransmitReceive(0x00); // Timeout (7:0)
    NSS_HIGH();

    // Attendre l'IRQ RxDone sur DIO1
    while (!(GPIOB->IDR & GPIO_IDR_ID3));

    // Lire les données reçues (commande 0x1E)
    NSS_LOW();
    SPI1_TransmitReceive(0x1E); // ReadBuffer command
    SPI1_TransmitReceive(0x00); // Offset = 0x00
    for (uint8_t i = 0; i < length; i++) {
        buffer[i] = SPI1_TransmitReceive(0x00); // Lire les octets un par un
    }
    NSS_HIGH();
}




// Fonction pour tester la communication SPI avec le SX1261




void SX1261_TestCommunication(void ) {

	printf("�� Début du test SX1261...\r\n");



    // Envoyer la commande de lecture du registre 0x0889
   NSS_LOW();
   // Attendre que BUSY soit bas
  while (GPIOB->IDR & GPIO_IDR_ID3);
   printf("BUSY LOW Yes  \r\n");



   SPI1_TransmitReceive(0x1D);  // Commande ReadRegister
   SPI1_TransmitReceive(0x06);  // MSB de l'adresse (0x06BC)
   SPI1_TransmitReceive(0xBC);  // LSB de l'adresse (0x06BC)

   //printf("BUSY LOW Yes  \r\n");
   uint8_t status;
   status = SPI1_TransmitReceive(0x00);// NOP pour lire lel status
   printf("THE STATUS IS : 0x%02X  \r\n",status);
   receive[0] = SPI1_TransmitReceive(0x00);//NOP pour lire la  donnéé
   receive[1] = SPI1_TransmitReceive(0x00);//NOP pour lire la  donnéé
   printf("BUSY LOW Yes  \r\n");
   NSS_HIGH();

   uint16_t receivedata = ((uint16_t)receive[0]<< 8) | receive[1];  // Concaténation


    // Vérifier la réponse et afficher le résultat sur l'UART
    if ( receivedata == 0x1D) {  // La valeur typique du registre 0x89 est 0x12
        printf("✅ Test réussi : Communication SPI avec SX1261 OK !, Registre 0x0889 : 0x%04X\r\n",receivedata);
    } else {
        printf("❌ Test échoué : Pas de réponse valide du SX1261 (0x%04X)\r\n", receivedata);
    }


}



void Check_BUSY_State() {
    printf("�� Vérification de la broche BUSY...\r\n");

    // Lire l'état initial de BUSY
    if (GPIOB->IDR & GPIO_IDR_ID3) {
        printf("�� BUSY est HIGH au démarrage !\r\n");
    } else {
        printf("✅ BUSY est LOW au démarrage.\r\n");
    }

    // Effectuer un reset matériel du SX1261
    printf("�� Reset du SX1261...\r\n");
    GPIOA->ODR &= ~GPIO_ODR_OD4; // Mettre NRESET à LOW
    for (volatile int i = 0; i < 10000; i++); // Attendre > 100µs
    GPIOA->ODR |= GPIO_ODR_OD4; // Mettre NRESET à HIGH

    // Attendre que BUSY change d'état (timeout après 100ms)
    uint32_t timeout = 100000;
    while ((GPIOB->IDR & GPIO_IDR_ID3) && timeout--) ;

    if (timeout == 0) {
        printf("❌ BUSY est toujours HIGH après reset !\r\n");
    } else {
        printf("✅ BUSY est devenu LOW, SX1261 prêt !\r\n");
    }
}



void SX1261_SoftReset() {
    NSS_LOW();
    SPI1_TransmitReceive(0x80); // Commande SetStandby (STDBY_RC)
    SPI1_TransmitReceive(0x00);
    NSS_HIGH();

    printf("��️ Soft reset du SX1261 envoyé.\r\n");

    // Vérifier si BUSY passe enfin à LOW
    uint32_t timeout = 100000;
    while ((GPIOB->IDR & GPIO_IDR_ID3) && timeout--) ;

    if (timeout == 0) {
        printf("❌ BUSY reste bloqué après le soft reset.\r\n");
    } else {
        printf("✅ BUSY est maintenant LOW après le soft reset !\r\n");
    }
}


