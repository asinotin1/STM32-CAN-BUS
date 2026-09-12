#include "DHT11.h"

extern TIM_HandleTypeDef htim2;
extern void delay_us(uint16_t time_us);
extern void delay_ms(uint16_t time_ms);

void DHT11_SetOutput(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


void DHT11_SetInput(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void DHT11_Start(void)
{
  uint32_t timeout;
  DHT11_SetOutput();
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
  delay_ms(18);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
  delay_us(30);
  DHT11_SetInput();

  timeout = 0;
  while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_SET)
    { delay_us(1); if(++timeout > 100) return 1; }

  timeout = 0;
  while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET)
    { delay_us(1); if(++timeout > 100) return 1; }

  timeout = 0;
  while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_SET)
    { delay_us(1); if(++timeout > 100) return 1; }

 
}

uint8_t DHT11_ReadByte(void)
{
  uint8_t i, byte = 0;
  uint32_t timeout;   

  for(i = 0; i < 8; i++)
  {

    timeout = 0;
    while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET)
    {
      delay_us(1);
      timeout++;
      if(timeout > 100) return byte;   
    }

    delay_us(50); 

    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_SET)
    {
        byte = (byte << 1) | 1;

        
        timeout = 0;
        while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_SET)
        {
          delay_us(1);
          timeout++;
          if(timeout > 100) return byte;
        }
    }
    else
    {
        byte = (byte << 1) & ~(1<<0);

        timeout = 0;
        while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_SET)
        {
          delay_us(1);
          timeout++;
          if(timeout > 100) return byte;
        }
    }
  }
  return byte;
}

 uint8_t DHT11_Humidity_Int = 0;
 uint8_t DHT11_Humidity_Dec = 0;
 uint8_t DHT11_Temperature_Int = 0;
 uint8_t DHT11_Temperature_Dec =0;

void DHT11_ReadData(void)
{
    DHT11_Start();
    DHT11_Humidity_Int = DHT11_ReadByte();
    DHT11_Humidity_Dec = DHT11_ReadByte();
    DHT11_Temperature_Int = DHT11_ReadByte();
    DHT11_Temperature_Dec = DHT11_ReadByte();
  
    
}
