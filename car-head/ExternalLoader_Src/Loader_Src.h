#ifndef __LOADER_SRC_H
#define __LOADER_SRC_H

#include "stm32h7xx_hal.h"

#define W25Qxx_Mem_Addr 0x90000000

int Init (void);
int Write (uint32_t Address, uint32_t Size, uint8_t* buffer);
int SectorErase (uint32_t EraseStartAddress, uint32_t EraseEndAddress);
int MassErase (void);
int Read (uint32_t Address, uint32_t Size, uint8_t* buffer);

#endif /* __LOADER_SRC_H */
