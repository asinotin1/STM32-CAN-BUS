#ifndef _BPS_W25Q64_H_
#define _BPS_W25Q64_H_

#include "main.h"

#define QSPI_W25Qxx_OK           	0
#define W25Qxx_ERROR_INIT         	-1
#define W25Qxx_ERROR_WriteEnable    -2
#define W25Qxx_ERROR_AUTOPOLLING    -3
#define W25Qxx_ERROR_Erase         	-4
#define W25Qxx_ERROR_TRANSMIT       -5
#define W25Qxx_ERROR_MemoryMapped	-6

#define W25Qxx_CMD_EnableReset  	0x66
#define W25Qxx_CMD_ResetDevice   	0x99
#define W25Qxx_CMD_JedecID 			0x9F
#define W25Qxx_CMD_WriteEnable		0X06

#define W25Qxx_CMD_SectorErase 		0x20
#define W25Qxx_CMD_BlockErase_32K 	0x52
#define W25Qxx_CMD_BlockErase_64K 	0xD8
#define W25Qxx_CMD_ChipErase 		0xC7

#define W25Qxx_CMD_PageProgram           	0x02
#define W25Qxx_CMD_FastReadQuad_IO       	0xEB

#define W25Qxx_CMD_ReadStatus_REG1			0X05
#define W25Qxx_Status_REG1_BUSY  			0x01
#define W25Qxx_Status_REG1_WEL  			0x02

#define W25Qxx_PageSize       				256
#define W25Qxx_FlashSize       				0x800000
#define W25Qxx_FLASH_ID           			0Xef4017
#define W25Qxx_ChipErase_TIMEOUT_MAX		100000U
#define W25Qxx_Mem_Addr						0x90000000

int8_t	QSPI_W25Qxx_Init(void);
int8_t 	QSPI_W25Qxx_Reset(void);
uint32_t QSPI_W25Qxx_ReadID(void);
int8_t 	QSPI_W25Qxx_MemoryMappedMode(void);

int8_t 	QSPI_W25Qxx_SectorErase(uint32_t SectorAddress);
int8_t 	QSPI_W25Qxx_BlockErase_32K (uint32_t SectorAddress);
int8_t 	QSPI_W25Qxx_BlockErase_64K (uint32_t SectorAddress);
int8_t 	QSPI_W25Qxx_ChipErase (void);

int8_t	QSPI_W25Qxx_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
int8_t	QSPI_W25Qxx_WriteBuffer(uint8_t* pData, uint32_t WriteAddr, uint32_t Size);
int8_t 	QSPI_W25Qxx_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);

#endif // _BPS_W25Q64_H_
