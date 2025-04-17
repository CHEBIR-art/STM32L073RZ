

#include "sx1261.h"
#include <stdio.h>
#include "spi.h"
#include "sx1261.h"
#include "systick.h"
#include "utilities.h"
#define NSS_LOW()  (GPIOA->ODR &= ~GPIO_ODR_OD4) // NSS = PA4
#define NSS_HIGH() (GPIOA->ODR |= GPIO_ODR_OD4)
#define NRESET_LOW()   (GPIOA->BSRR = GPIO_BSRR_BR_0)  // Mettre NRESET √† LOW  PA0
#define NRESET_HIGH()  (GPIOA->BSRR = GPIO_BSRR_BS_0)  // Mettre NRESET √† HIGH PA0


extern uint8_t receive[2];



void WaitForBusyLow(void) {
	uint32_t timeout = 1000000;  // Nombre d'it√©rations avant d'abandonner

	    while ((GPIOB->IDR & GPIO_IDR_ID3) && (timeout-- > 0));

	    if (timeout == 0) {
	        printf("‚ùå Timeout : BUSY reste HIGH !\r\n");
	    } else {
	        printf("‚úÖ BUSY est pass√© √† LOW, le SX1261 est pr√™t.\r\n");
	    }
}

void SX1261_SetStandby(void) {

	 SX1261_WaitWhileBusy();
    // √âcrire dans le registre de mode pour passer en mode Standby
    SX1261_WriteRegister(REG_OP_MODE, OP_MODE_STANDBY);
    printf("Le module SX1261 est passe en mode Standby.\r\n");

    SYSTICK_Delay(10);  // D√©lai de 10 ms
}


//     PROBLEEEEEEEEEEEEEEM !!!!!!!


void SX1261_Init() {

	  printf("Ì†ΩÌ¥Ñ Initialisation du SX1261...\r\n");
		  NRESET_LOW();  // Mettre NRESET √† LOW (assumer que NRESET est sur PB3)
	    for (volatile int i = 0; i < 100000; i++); // Attendre >100¬µs
	    NRESET_HIGH(); // Mettre NRESET √† HIGH

	    WaitForBusyLow();


	    // Envoyer la commande pour passer en mode STDBY_RC
	       printf("‚öôÔ∏è Passage en mode STDBY_RC...\r\n");
	       NSS_LOW();
	          SPI1_TransmitReceive(0x80); // Commande SetStandby
	          SPI1_TransmitReceive(0x00); // Param√®tre : mode RC
	       NSS_HIGH();
	       printf("‚úÖ SX1261 initialis√© avec succ√®s !\r\n");


	       NSS_LOW();
	          SPI1_TransmitReceive(0xC0); // GetStatus
	          uint8_t status = SPI1_TransmitReceive(0x00); // Lecture du statut
	       NSS_HIGH();

	       printf("Ì†ΩÌ≥° Statut SX1261: 0x%02X\r\n", status);
	       uint8_t mode = (status >> 5) & 0x07;
	       if (mode == 0x00) {
	          printf("‚úÖ Le module est en mode STDBY_RC.\r\n");
	       } else {
	          printf("‚ùå Le module n'est pas en mode STDBY_RC (mode actuel : 0x%02X).\r\n", mode);
	       }

}




void SX1261_Send(uint8_t *data, uint8_t length) {
    // Attendre que BUSY soit bas
    //while (GPIOB->IDR & GPIO_IDR_ID3);

    // D√©finir l‚Äôadresse de d√©part du buffer TX (commande 0x8F, TxBaseAddr = 0x00)
    NSS_LOW();
    SPI1_TransmitReceive(0x8F);
    SPI1_TransmitReceive(0x00); // TxBaseAddr
    SPI1_TransmitReceive(0x00); // RxBaseAddr
    NSS_HIGH();

    // √âcrire les donn√©es dans le buffer (commande 0x0E)
    NSS_LOW();
    SPI1_TransmitReceive(0x0E); // WriteBuffer command
    SPI1_TransmitReceive(0x00); // Offset = 0x00
    for (uint8_t i = 0; i < length; i++) {
        SPI1_TransmitReceive(data[i]); // Envoyer les donn√©es
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

    // D√©finir l'adresse de d√©but du buffer RX (commande 0x8F)
    NSS_LOW();
    SPI1_TransmitReceive(0x8F);
    SPI1_TransmitReceive(0x00); // RxBaseAddr
    SPI1_TransmitReceive(0x00); // TxBaseAddr
    NSS_HIGH();

    // Passer en mode r√©ception (commande 0x82)
    NSS_LOW();
    SPI1_TransmitReceive(0x82);
    SPI1_TransmitReceive(0x00); // Timeout (23:16)
    SPI1_TransmitReceive(0x00); // Timeout (15:8)
    SPI1_TransmitReceive(0x00); // Timeout (7:0)
    NSS_HIGH();

    // Attendre l'IRQ RxDone sur DIO1
    while (!(GPIOB->IDR & GPIO_IDR_ID3));

    // Lire les donn√©es re√ßues (commande 0x1E)
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

	printf("Ì†ΩÌø° D√©but du test SX1261...\r\n");



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
   receive[0] = SPI1_TransmitReceive(0x00);//NOP pour lire la  donn√©√©
   receive[1] = SPI1_TransmitReceive(0x00);//NOP pour lire la  donn√©√©
   printf("BUSY LOW Yes  \r\n");
   NSS_HIGH();

   uint16_t receivedata = ((uint16_t)receive[0]<< 8) | receive[1];  // Concat√©nation


    // V√©rifier la r√©ponse et afficher le r√©sultat sur l'UART
    if ( receivedata == 0x1D) {  // La valeur typique du registre 0x89 est 0x12
        printf("‚úÖ Test r√©ussi : Communication SPI avec SX1261 OK !, Registre 0x0889 : 0x%04X\r\n",receivedata);
    } else {
        printf("‚ùå Test √©chou√© : Pas de r√©ponse valide du SX1261 (0x%04X)\r\n", receivedata);
    }


}



void Check_BUSY_State() {
    printf("Ì†ΩÌ≥° V√©rification de la broche BUSY...\r\n");

    // Lire l'√©tat initial de BUSY
    if (GPIOB->IDR & GPIO_IDR_ID3) {
        printf("Ì†ΩÌ∫® BUSY est HIGH au d√©marrage !\r\n");
    } else {
        printf("‚úÖ BUSY est LOW au d√©marrage.\r\n");
    }

    // Effectuer un reset mat√©riel du SX1261
    printf("Ì†ΩÌ¥Ñ Reset du SX1261...\r\n");
    GPIOA->ODR &= ~GPIO_ODR_OD4; // Mettre NRESET √† LOW
    for (volatile int i = 0; i < 10000; i++); // Attendre > 100¬µs
    GPIOA->ODR |= GPIO_ODR_OD4; // Mettre NRESET √† HIGH

    // Attendre que BUSY change d'√©tat (timeout apr√®s 100ms)
    uint32_t timeout = 100000;
    while ((GPIOB->IDR & GPIO_IDR_ID3) && timeout--) ;

    if (timeout == 0) {
        printf("‚ùå BUSY est toujours HIGH apr√®s reset !\r\n");
    } else {
        printf("‚úÖ BUSY est devenu LOW, SX1261 pr√™t !\r\n");
    }
}



void SX1261_SoftReset() {
    NSS_LOW();
    SPI1_TransmitReceive(0x80); // Commande SetStandby (STDBY_RC)
    SPI1_TransmitReceive(0x00);
    NSS_HIGH();

    printf("Ì†ΩÌª†Ô∏è Soft reset du SX1261 envoy√©.\r\n");

    // V√©rifier si BUSY passe enfin √† LOW
    uint32_t timeout = 100000;
    while ((GPIOB->IDR & GPIO_IDR_ID3) && timeout--) ;

    if (timeout == 0) {
        printf("‚ùå BUSY reste bloqu√© apr√®s le soft reset.\r\n");
    } else {
        printf("‚úÖ BUSY est maintenant LOW apr√®s le soft reset !\r\n");
    }
}


