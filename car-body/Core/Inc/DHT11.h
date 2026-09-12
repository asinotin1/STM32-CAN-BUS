#ifndef DHT11_H
#define DHT11_H
#include "main.h"

extern uint8_t DHT11_Humidity_Int;
extern uint8_t DHT11_Humidity_Dec;
extern uint8_t DHT11_Temperature_Int;
extern uint8_t DHT11_Temperature_Dec;
void DHT11_SetOutput(void);
void DHT11_SetInput(void);
void DHT11_Start(void);
uint8_t DHT11_ReadByte(void);
void DHT11_ReadData(void);

#endif