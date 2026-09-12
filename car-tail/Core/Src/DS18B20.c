#include "ds18b20.h"

extern TIM_HandleTypeDef htim2;
extern void delay_us(uint16_t time_us);
extern void delay_ms(uint16_t time_ms);

static void DS18B20_SetPinOutput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin   = DS18B20_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
}

static void DS18B20_SetPinInput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin  = DS18B20_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
}

uint8_t DS18B20_Reset(void)
{
    uint8_t check = 0;
    __disable_irq();
    DS18B20_SetPinOutput();
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);
    delay_us(480);
    DS18B20_SetPinInput();
    delay_us(70);
    check = HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN);
    delay_us(410);
    __enable_irq();

    return (check == GPIO_PIN_RESET) ? 1 : 0;
}

void DS18B20_WriteBit(uint8_t bit)
{
    __disable_irq();
    DS18B20_SetPinOutput();
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);

    if (bit == 1)
    {
        delay_us(8);
        DS18B20_SetPinInput();
        delay_us(64);
    }
    else
    {
        delay_us(60);
        DS18B20_SetPinInput();
        delay_us(10);
    }
    __enable_irq();
}

void DS18B20_WriteByte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        DS18B20_WriteBit(data & 0x01);
        data >>= 1;
    }
}

static uint8_t DS18B20_ReadBit(void)
{
    uint8_t bit;
    __disable_irq();
    DS18B20_SetPinOutput();
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);
    delay_us(2);

    DS18B20_SetPinInput();
    delay_us(10);

    bit = (HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN) == GPIO_PIN_SET);

    delay_us(50);
    __enable_irq();
    return bit;
}

uint8_t DS18B20_ReadByte(void)
{
    uint8_t data = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        data |= (DS18B20_ReadBit() << i);
    }
    return data;
}


uint8_t DS18B20_StartConvert(void)
{
    if (!DS18B20_Reset())
    {
        return 0;  
    }
    DS18B20_WriteByte(DS18B20_CMD_SKIPROM);
    DS18B20_WriteByte(DS18B20_CMD_CONVERTT);
    return 1;   
}

float DS18B20_GetTemperature(void)
{
    uint8_t tempL, tempH;
    int16_t result;

    if (!DS18B20_Reset())
    {
        return DS18B20_ERROR;
    }
    DS18B20_WriteByte(DS18B20_CMD_SKIPROM);
    DS18B20_WriteByte(DS18B20_CMD_RSCRATCHPAD);

    tempL = DS18B20_ReadByte();
    tempH = DS18B20_ReadByte();
    result = (int16_t)((tempH << 8) | tempL);

    return result * 0.0625f;
}