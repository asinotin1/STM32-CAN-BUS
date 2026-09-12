#include "Hc-Sr04.h"

volatile uint32_t t1 = 0;
volatile uint32_t t2 = 0;
volatile uint32_t pulse = 0;
volatile uint8_t flag = 0;

float HCSR04_Read(void)
{
    pulse = 0; // reset giá trị pulse trước khi đo
     uint32_t timeout = HAL_GetTick();
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

      while(pulse == 0)
    {
        if(HAL_GetTick() - timeout > 50)   
        {
            return -1;
        }
    }

    
    return pulse * 0.0343f / 2.0f;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM3)
    {
        if(flag == 0)
        {
            t1 = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);

            flag = 1;

            __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING);
        }
        else
        {
            t2 = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);

            pulse = t2 - t1;

            flag = 0;

            __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING);
        }
    }
}