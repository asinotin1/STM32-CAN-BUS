#ifndef _BSP_SOFT_I2C_H_
#define _BSP_SOFT_I2C_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "tim.h"

#define Touch_IIC_SCL_CLK_ENABLE	__HAL_RCC_GPIOG_CLK_ENABLE()
#define Touch_IIC_SCL_PORT   		GPIOG
#define Touch_IIC_SCL_PIN     		GPIO_PIN_3

#define Touch_IIC_SDA_CLK_ENABLE	__HAL_RCC_GPIOG_CLK_ENABLE()
#define Touch_IIC_SDA_PORT   		GPIOG
#define Touch_IIC_SDA_PIN    		GPIO_PIN_7

#define Touch_INT_CLK_ENABLE    	__HAL_RCC_GPIOI_CLK_ENABLE()
#define Touch_INT_PORT   			GPIOI
#define Touch_INT_PIN    			GPIO_PIN_11

#define Touch_RST_CLK_ENABLE   		__HAL_RCC_GPIOI_CLK_ENABLE()
#define Touch_RST_PORT   			GPIOI
#define Touch_RST_PIN    			GPIO_PIN_10

#define ACK_OK  	1
#define ACK_ERR 	0

#define IIC_DelayVaule  5

#define Touch_IIC_SCL(a)	if (a)	\
										HAL_GPIO_WritePin(Touch_IIC_SCL_PORT, Touch_IIC_SCL_PIN, GPIO_PIN_SET); \
									else		\
										HAL_GPIO_WritePin(Touch_IIC_SCL_PORT, Touch_IIC_SCL_PIN, GPIO_PIN_RESET)

#define Touch_IIC_SDA(a)	if (a)	\
										HAL_GPIO_WritePin(Touch_IIC_SDA_PORT, Touch_IIC_SDA_PIN, GPIO_PIN_SET); \
									else		\
										HAL_GPIO_WritePin(Touch_IIC_SDA_PORT, Touch_IIC_SDA_PIN, GPIO_PIN_RESET)

void Touch_IIC_SetTimerBase(TIM_HandleTypeDef * pSetTimer);
void Touch_IIC_GPIO_Config(void);
void Touch_IIC_Delay(uint32_t ulTimeUs);
void Touch_INT_Out(void);
void Touch_INT_In(void);
void Touch_IIC_Start(void);
void Touch_IIC_Stop(void);
void Touch_IIC_ACK(void);
void Touch_IIC_NoACK(void);
uint8_t Touch_IIC_WaitACK(void);
uint8_t	Touch_IIC_WriteByte(uint8_t IIC_Data);
uint8_t Touch_IIC_ReadByte(uint8_t ACK_Mode);

#ifdef __cplusplus
}
#endif

#endif // _BSP_SOFT_I2C_H_
