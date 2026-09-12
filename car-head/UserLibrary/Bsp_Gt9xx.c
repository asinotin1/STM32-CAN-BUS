#include "Bsp_Gt9xx.h"

volatile TouchStructure touchInfo;

void GT9XX_Reset(void)
{
	Touch_INT_Out();

	HAL_GPIO_WritePin(Touch_INT_PORT,Touch_INT_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Touch_RST_PORT,Touch_RST_PIN,GPIO_PIN_SET);
	HAL_Delay(100);

	HAL_GPIO_WritePin(Touch_RST_PORT,Touch_RST_PIN,GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(Touch_RST_PORT,Touch_RST_PIN,GPIO_PIN_SET);
	HAL_Delay(100);
	Touch_INT_In();
	HAL_Delay(100);
}

uint8_t GT9XX_WriteHandle (uint16_t addr)
{
	uint8_t status;

	Touch_IIC_Start();
	if( Touch_IIC_WriteByte(GT9XX_IIC_WADDR) == ACK_OK )
	{
		if( Touch_IIC_WriteByte((uint8_t)(addr >> 8)) == ACK_OK )
		{
			if( Touch_IIC_WriteByte((uint8_t)(addr)) != ACK_OK )
			{
				status = ERROR;
			}
		}
	}
	status = SUCCESS;
	return status;
}

uint8_t GT9XX_WriteData (uint16_t addr,uint8_t value)
{
	uint8_t status;

	Touch_IIC_Start();

	if( GT9XX_WriteHandle(addr) == SUCCESS)
	{
		if (Touch_IIC_WriteByte(value) != ACK_OK)
		{
			status = ERROR;
		}
	}
	Touch_IIC_Stop();

	status = SUCCESS;
	return status;
}

uint8_t GT9XX_WriteReg (uint16_t addr, uint8_t cnt, uint8_t *value)
{
	uint8_t status;
	uint8_t i;

	Touch_IIC_Start();

	if( GT9XX_WriteHandle(addr) == SUCCESS)
	{
		for(i = 0 ; i < cnt; i++)
		{
			Touch_IIC_WriteByte(value[i]);
		}
		Touch_IIC_Stop();
		status = SUCCESS;
	}
	else
	{
		Touch_IIC_Stop();
		status = ERROR;
	}
	return status;
}

uint8_t GT9XX_ReadReg (uint16_t addr, uint8_t cnt, uint8_t *value)
{
	uint8_t status;
	uint8_t i;

	status = ERROR;
	Touch_IIC_Start();

	if( GT9XX_WriteHandle(addr) == SUCCESS)
	{
		Touch_IIC_Start();

		if (Touch_IIC_WriteByte(GT9XX_IIC_RADDR) == ACK_OK)
		{
			for(i = 0 ; i < cnt; i++)
			{
				if (i == (cnt - 1))
				{
					value[i] = Touch_IIC_ReadByte(0);
				}
				else
				{
					value[i] = Touch_IIC_ReadByte(1);
				}
			}
			Touch_IIC_Stop();
			status = SUCCESS;
		}
	}
	Touch_IIC_Stop();
	return (status);
}

uint8_t Touch_Init(void)
{
	uint8_t GT9XX_Info[11];
	uint8_t cfgVersion = 0;

	Touch_IIC_GPIO_Config();
	GT9XX_Reset();

	GT9XX_ReadReg (GT9XX_ID_ADDR,11,GT9XX_Info);
	GT9XX_ReadReg (GT9XX_CFG_ADDR,1,&cfgVersion);

	if( GT9XX_Info[0] == '9' )
	{

		return SUCCESS;
	}
	else
	{
		return ERROR;
	}

}

void Touch_Scan(void)
{
 	uint8_t  touchData[2 + 8 * TOUCH_MAX ];
	uint8_t  i = 0;

	GT9XX_ReadReg (GT9XX_READ_ADDR,2 + 8 * TOUCH_MAX ,touchData);
	GT9XX_WriteData (GT9XX_READ_ADDR,0);
	touchInfo.num = touchData[0] & 0x0f;

	if ( (touchInfo.num >= 1) && (touchInfo.num <=5) )
	{
		for(i=0;i<touchInfo.num;i++)
		{
			touchInfo.y[i] = (touchData[5+8*i]<<8) | touchData[4+8*i];
			touchInfo.x[i] = (touchData[3+8*i]<<8) | touchData[2+8*i];
		}
		touchInfo.flag = 1;
	}
	else
	{
		touchInfo.flag = 0;
	}
}
