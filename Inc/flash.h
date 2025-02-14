/*
 * flash.h
 *
 *  Created on: Feb 14, 2025
 *      Author: user
 */

#ifndef FLASH_H_
#define FLASH_H_



void Flash_Unlock(void) ;
void Flash_Lock(void);
void Flash_ErasePage(uint32_t PageAddress);
void Flash_WriteDoubleWord(uint32_t Address, uint64_t Data);
uint64_t Flash_ReadDoubleWord(uint32_t Address) ;
void SaveFrameCounter(uint32_t up, uint32_t down);
void LoadFrameCounter(uint32_t *up, uint32_t *down);
void TestFlash(void);


#endif /* FLASH_H_ */
