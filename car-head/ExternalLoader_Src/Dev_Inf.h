#ifndef __DEV_INF_H
#define __DEV_INF_H

#include <stdint.h>

#define NOR_FLASH  1
#define SRAM       2
#define PSRAM      3
#define NAND_FLASH 4

struct DeviceSectors  {
  uint32_t SectorNum;      // Number of Sectors
  uint32_t SectorSize;     // Sector Size in Bytes
};

struct StorageInfo {
  char       DeviceName[100];     // Device Name and Description
  uint16_t   DeviceType;          // Device Type: ONENAND, NOR, SPI, etc.
  uint32_t   DeviceStartAddress;  // Default Device Start Address
  uint32_t   DeviceSize;          // Total Size of Device
  uint32_t   PageSize;            // Programming Page Size
  uint8_t    EraseValue;          // Content of Erased Memory
  struct     DeviceSectors sectors[10]; // Sector info
};

#endif /* __DEV_INF_H */
