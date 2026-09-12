#include "Dev_Inf.h"

/* This structure contains information used by ST-LINK Utility to program and erase the device */
#if defined (__ICCARM__)
__root struct StorageInfo const StorageInfo  =  {
#else
__attribute__((section(".Dev_info"))) struct StorageInfo const StorageInfo  =  {
#endif
   "WeAct_H743_W25Q64",       // Device Name + version number
   NOR_FLASH,                 // Device Type
   0x90000000,                // Device Start Address
   0x00800000,                // Device Size in Bytes (8MB)
   0x00000100,                // Programming Page Size (256 Bytes)
   0xFF,                      // Initial Content of Erased Memory

   // Specify Size and Address of Sectors (using 4KB sectors)
   {{0x00000800, 0x00001000}, // 2048 Sectors of 4KBytes
    {0x00000000, 0x00000000}}
};
