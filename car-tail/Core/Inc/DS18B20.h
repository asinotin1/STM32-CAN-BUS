
#ifndef DS18B20_H
#define DS18B20_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"   


#define DS18B20_PORT        GPIOA
#define DS18B20_PIN         GPIO_PIN_0


#define DS18B20_CMD_SKIPROM      0xCC // Skip rom : báo có duy nhất 1 cảm biến DS18B20 trên bus
#define DS18B20_CMD_CONVERTT     0x44 // CONVERT T : đọc nhiệt độ
#define DS18B20_CMD_RSCRATCHPAD  0xBE // SCRATCHPAD : kết quả đo được 

#define DS18B20_ERROR        -999.0f


uint8_t DS18B20_Reset(void);

void    DS18B20_WriteByte(uint8_t data);
uint8_t DS18B20_ReadByte(void);
uint8_t DS18B20_StartConvert(void);
float   DS18B20_GetTemperature(void);
#ifdef __cplusplus
}
#endif

#endif