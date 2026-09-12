#include "Bsp_Lcd.h"
#include "dma2d.h"

struct LcdConfig
{
	uint32_t Color;
	uint32_t BackColor;
	uint32_t ColorMode;
	uint32_t LayerMemoryAdd;
	uint8_t  Layer;
	uint8_t  Direction;
	uint8_t  BytesPerPixel;
	uint8_t  ShowNum_Mode;
};

struct LcdConfig MyLCD;
static uint8_t isTransmittingData = 0;

void Bsp_LcdInit(void)
{
	Bsp_LcdSetDisplayDirection(Direction_H);
	Bsp_LcdSetBrightness(1);
	Bsp_LcdSetLayer(1);
	Bsp_LcdSetBackColor(0x00000000);
	Bsp_LcdClear();
	Bsp_LcdSetLayer(0);
	Bsp_LcdSetBackColor(LCD_WHITE);
	Bsp_LcdClear();
}

void Bsp_LcdSetBrightness(uint16_t uwBrightness)
{
#ifdef LCD_BL_Pin
	HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
#else
    if (uwBrightness >= 1000) {
        __HAL_TIM_SET_COMPARE(&TIMER_PWM_CHANNEL, PWM_PIN_CHANNEL, TIMER_PWM_CHANNEL.Init.Period + 1);
    } else {
        __HAL_TIM_SET_COMPARE(&TIMER_PWM_CHANNEL, PWM_PIN_CHANNEL, uwBrightness * (TIMER_PWM_CHANNEL.Init.Period + 1) / 1000);
    }
#endif
}

void Bsp_LcdSetLayer(uint8_t ubLayer)
{
#if LCD_NUM_LAYERS == 2
	if (ubLayer == 0)
	{
		MyLCD.LayerMemoryAdd = LCD_MemoryAdd;
		MyLCD.ColorMode      = ColorMode_0;
		MyLCD.BytesPerPixel  = BytesPerPixel_0;
	}
	else if(ubLayer == 1)
	{
		MyLCD.LayerMemoryAdd = LCD_MemoryAdd + LCD_MemoryAdd_OFFSET;
		MyLCD.ColorMode      = ColorMode_1;
		MyLCD.BytesPerPixel  = BytesPerPixel_1;
	}
	MyLCD.Layer = ubLayer;
#else
	MyLCD.LayerMemoryAdd = LCD_MemoryAdd;
	MyLCD.ColorMode      = ColorMode_0;
	MyLCD.BytesPerPixel  = BytesPerPixel_0;
	MyLCD.Layer = 0;
#endif
}

void Bsp_LcdSetColor(uint32_t ulColor)
{
	uint16_t Alpha_Value = 0, Red_Value = 0, Green_Value = 0, Blue_Value = 0;

	if( MyLCD.ColorMode == LTDC_PIXEL_FORMAT_RGB565	)
	{
		Red_Value   = (uint16_t)((ulColor&0x00F80000)>>8);
		Green_Value = (uint16_t)((ulColor&0x0000FC00)>>5);
		Blue_Value  = (uint16_t)((ulColor&0x000000F8)>>3);
		MyLCD.Color = (uint16_t)(Red_Value | Green_Value | Blue_Value);
	}
	else if(MyLCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB1555 )
	{
		if( (ulColor & 0xFF000000) == 0 )
			Alpha_Value = 0x0000;
		else
			Alpha_Value = 0x8000;

		Red_Value   = (uint16_t)((ulColor&0x00F80000)>>9);
		Green_Value = (uint16_t)((ulColor&0x0000F800)>>6);
		Blue_Value  = (uint16_t)((ulColor&0x000000F8)>>3);
		MyLCD.Color = (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);
	}
	else if( MyLCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB4444 )
	{

		Alpha_Value = (uint16_t)((ulColor&0xf0000000)>>16);
		Red_Value   = (uint16_t)((ulColor&0x00F00000)>>12);
		Green_Value = (uint16_t)((ulColor&0x0000F000)>>8);
		Blue_Value  = (uint16_t)((ulColor&0x000000F8)>>4);
		MyLCD.Color = (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);
	}
	else
	{
		MyLCD.Color = ulColor;
	}
}

void Bsp_LcdSetBackColor(uint32_t ulColor)
{
	uint16_t Alpha_Value = 0, Red_Value = 0, Green_Value = 0, Blue_Value = 0;  //������ɫͨ����ֵ

	if(MyLCD.ColorMode == LTDC_PIXEL_FORMAT_RGB565)
	{
		Red_Value   	= (uint16_t)((ulColor&0x00F80000)>>8);
		Green_Value 	= (uint16_t)((ulColor&0x0000FC00)>>5);
		Blue_Value  	= (uint16_t)((ulColor&0x000000F8)>>3);
		MyLCD.BackColor	= (uint16_t)(Red_Value | Green_Value | Blue_Value);
	}
	else if(MyLCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB1555)
	{
		if( (ulColor & 0xFF000000) == 0)
			Alpha_Value = 0x0000;
		else
			Alpha_Value = 0x8000;

		Red_Value   	= (uint16_t)((ulColor&0x00F80000)>>9);
		Green_Value 	= (uint16_t)((ulColor&0x0000F800)>>6);
		Blue_Value  	= (uint16_t)((ulColor&0x000000F8)>>3);
		MyLCD.BackColor 	= (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);
	}
	else if( MyLCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB4444 )
	{

		Alpha_Value 	= (uint16_t)((ulColor&0xf0000000)>>16);
		Red_Value   	= (uint16_t)((ulColor&0x00F00000)>>12);
		Green_Value 	= (uint16_t)((ulColor&0x0000F000)>>8);
		Blue_Value  	= (uint16_t)((ulColor&0x000000F8)>>4);
		MyLCD.BackColor 	= (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);
	}
	else
	{
		MyLCD.BackColor = ulColor;
	}
}

void Bsp_LcdClear(void)
{
	DMA2D->CR 		&=	~(DMA2D_CR_START);
	DMA2D->CR		=	DMA2D_R2M;
	DMA2D->OPFCCR	=	MyLCD.ColorMode;
	DMA2D->OOR		=	0;
	DMA2D->OMAR		=	MyLCD.LayerMemoryAdd;
	DMA2D->NLR		=	(LCD_Width<<16)|(LCD_Height);
	DMA2D->OCOLR	=	MyLCD.BackColor;
	while( LTDC->CDSR != 0X00000001);
	DMA2D->CR	  |=	DMA2D_CR_START;
	while (DMA2D->CR & DMA2D_CR_START) ;
	/*
	hdma2d.Instance = DMA2D;
	hdma2d.Init.Mode = DMA2D_R2M;
	hdma2d.Init.ColorMode = MyLCD.ColorMode;
	hdma2d.Init.OutputOffset = 0;
	HAL_DMA2D_Init(&hdma2d);

	HAL_DMA2D_Start(&hdma2d, MyLCD.BackColor, MyLCD.LayerMemoryAdd, LCD_Width, LCD_Height);
	HAL_DMA2D_PollForTransfer(&hdma2d, 100);
	*/
}

void Bsp_LcdClearRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{

	DMA2D->CR	  &=	~(DMA2D_CR_START);
	DMA2D->CR		=	DMA2D_R2M;
	DMA2D->OPFCCR	=	MyLCD.ColorMode;
	DMA2D->OCOLR	=	MyLCD.BackColor;

	if(MyLCD.Direction == Direction_H)
	{
		DMA2D->OOR		=	LCD_Width - width;
		DMA2D->OMAR		=	MyLCD.LayerMemoryAdd + MyLCD.BytesPerPixel*(LCD_Width * y + x);
		DMA2D->NLR		=	(width<<16)|(height);
	}
	else
	{
		DMA2D->OOR		=	LCD_Width - height;
		DMA2D->OMAR		=	MyLCD.LayerMemoryAdd + MyLCD.BytesPerPixel*((LCD_Height - x - 1 - width)*LCD_Width + y);
		DMA2D->NLR		=	(width)|(height<<16);
	}
	DMA2D->CR	  |=	DMA2D_CR_START;
	while (DMA2D->CR & DMA2D_CR_START);
}

void Bsp_LcdSetDisplayDirection(uint8_t ubDirection)
{
	MyLCD.Direction = ubDirection;
}

void Bsp_LcdDrawPoint(uint16_t x,uint16_t y,uint32_t color)
{
	// ARGB8888
	if( MyLCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB8888 )
	{
		if (MyLCD.Direction == Direction_H)
		{
			*(__IO uint32_t*)( MyLCD.LayerMemoryAdd + 4*(x + y*LCD_Width) ) = color ;
		}
		else if(MyLCD.Direction == Direction_V)
		{
			*(__IO uint32_t*)( MyLCD.LayerMemoryAdd + 4*((LCD_Height - x - 1)*LCD_Width + y) ) = color ;
		}
	}
	// RGB888
	else if ( MyLCD.ColorMode == LTDC_PIXEL_FORMAT_RGB888 )
	{
		if (MyLCD.Direction == Direction_H)
		{
			*(__IO uint16_t*)( MyLCD.LayerMemoryAdd + 3*(x + y*LCD_Width) ) = color ;
			*(__IO uint8_t*)( MyLCD.LayerMemoryAdd + 3*(x + y*LCD_Width) + 2 ) = color>>16 ;
		}
		else if(MyLCD.Direction == Direction_V)
		{
			*(__IO uint16_t*)( MyLCD.LayerMemoryAdd + 3*((LCD_Height - x - 1)*LCD_Width + y) ) = color ;
			*(__IO uint8_t*)( MyLCD.LayerMemoryAdd + 3*((LCD_Height - x - 1)*LCD_Width + y) +2) = color>>16 ;
		}
	}
	// ARGB1555 || RGB565 || ARGB4444
	else
	{
		if (MyLCD.Direction == Direction_H)
		{
			*(__IO uint16_t*)( MyLCD.LayerMemoryAdd + 2*(x + y*LCD_Width) ) = color ;
		}
		else if(MyLCD.Direction == Direction_V)
		{
			*(__IO uint16_t*)( MyLCD.LayerMemoryAdd + 2*((LCD_Height - x - 1)*LCD_Width + y) ) = color ;
		}
	}
}

uint32_t Bsp_LcdReadPoint(uint16_t x, uint16_t y)
{
	uint32_t color = 0;

	// ARGB8888
	if(MyLCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB8888)
	{
		if (MyLCD.Direction == Direction_H)
		{
			color = *(__IO uint32_t*)( MyLCD.LayerMemoryAdd + 4*(x + y*LCD_Width) );
		}
		else if(MyLCD.Direction == Direction_V)
		{
			color = *(__IO uint32_t*)( MyLCD.LayerMemoryAdd + 4*((LCD_Height - x - 1)*LCD_Width + y) );
		}
	}
	// RGB888
	else if(MyLCD.ColorMode == LTDC_PIXEL_FORMAT_RGB888)
	{
		if(MyLCD.Direction == Direction_H)
		{
			color = *(__IO uint32_t*)( MyLCD.LayerMemoryAdd + 3*(x + y*LCD_Width)) &0x00ffffff;
		}
		else if(MyLCD.Direction == Direction_V)
		{
			color = *(__IO uint32_t*)( MyLCD.LayerMemoryAdd + 3*((LCD_Height - x - 1)*LCD_Width + y) ) &0x00ffffff;
		}
	}
	// ARGB1555 | RGB565 | ARGB4444
	else
	{
		if (MyLCD.Direction == Direction_H)
		{
			color = *(__IO uint16_t*)( MyLCD.LayerMemoryAdd + 2*(x + y*LCD_Width));
		}
		else if(MyLCD.Direction == Direction_V)
		{
			color = *(__IO uint16_t*)( MyLCD.LayerMemoryAdd + 2*((LCD_Height - x - 1)*LCD_Width + y));
		}
	}
	return color;
}

void Bsp_LcdDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
	curpixel = 0;

	deltax = ABS(x2 - x1);        /* The difference between the x's */
	deltay = ABS(y2 - y1);        /* The difference between the y's */
	x = x1;                       /* Start x off at the first pixel */
	y = y1;                       /* Start y off at the first pixel */

	if (x2 >= x1)                 /* The x-values are increasing */
	{
		xinc1 = 1;
		xinc2 = 1;
	}
	else                          /* The x-values are decreasing */
	{
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1)                 /* The y-values are increasing */
	{
		yinc1 = 1;
		yinc2 = 1;
	}
	else                          /* The y-values are decreasing */
	{
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay)         /* There is at least one x-value for every y-value */
	{
		xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
		yinc2 = 0;                  /* Don't change the y for every iteration */
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;         /* There are more x-values than y-values */
	}
	else                          /* There is at least one y-value for every x-value */
	{
		xinc2 = 0;                  /* Don't change the x for every iteration */
		yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;         /* There are more y-values than x-values */
	}
	for (curpixel = 0; curpixel <= numpixels; curpixel++)
	{
		Bsp_LcdDrawPoint(x,y,MyLCD.Color);             /* Draw the current pixel */
		num += numadd;              /* Increase the numerator by the top of the fraction */
		if (num >= den)             /* Check if numerator >= denominator */
		{
			num -= den;               /* Calculate the new numerator value */
			x += xinc1;               /* Change the x as appropriate */
			y += yinc1;               /* Change the y as appropriate */
		}
		x += xinc2;                 /* Change the x as appropriate */
		y += yinc2;                 /* Change the y as appropriate */
	}
}

void BsP_LcdDrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	/* draw horizontal lines */
	Bsp_LcdDrawLine(x, y, x+width, y);
	Bsp_LcdDrawLine(x, y+height, x+width, y+height);

	/* draw vertical lines */
	Bsp_LcdDrawLine(x, y, x, y+height);
	Bsp_LcdDrawLine(x+width, y, x+width, y+height);
}

void Bsl_LcdDrawCircle(uint16_t x, uint16_t y, uint16_t r)
{
	int Xadd = -r, Yadd = 0, err = 2-2*r, e2;
	do {

		Bsp_LcdDrawPoint(x-Xadd,y+Yadd,MyLCD.Color);
		Bsp_LcdDrawPoint(x+Xadd,y+Yadd,MyLCD.Color);
		Bsp_LcdDrawPoint(x+Xadd,y-Yadd,MyLCD.Color);
		Bsp_LcdDrawPoint(x-Xadd,y-Yadd,MyLCD.Color);

		e2 = err;
		if (e2 <= Yadd) {
			err += ++Yadd*2+1;
			if (-Xadd == Yadd && e2 <= Xadd) e2 = 0;
		}
		if (e2 > Xadd) err += ++Xadd*2+1;
    }
    while (Xadd <= 0);
}

void Bsp_LcdDrawEllipse(int x, int y, int r1, int r2)
{
  int Xadd = -r1, Yadd = 0, err = 2-2*r1, e2;
  float K = 0, rad1 = 0, rad2 = 0;

  rad1 = r1;
  rad2 = r2;

  if (r1 > r2)
  {
	do
	{
		K = (float)(rad1/rad2);

		Bsp_LcdDrawPoint(x-Xadd,y+(uint16_t)(Yadd/K),MyLCD.Color);
		Bsp_LcdDrawPoint(x+Xadd,y+(uint16_t)(Yadd/K),MyLCD.Color);
		Bsp_LcdDrawPoint(x+Xadd,y-(uint16_t)(Yadd/K),MyLCD.Color);
		Bsp_LcdDrawPoint(x-Xadd,y-(uint16_t)(Yadd/K),MyLCD.Color);

		e2 = err;
		if (e2 <= Yadd) {
		err += ++Yadd*2+1;
		if (-Xadd == Yadd && e2 <= Xadd) e2 = 0;
		}
		if (e2 > Xadd) err += ++Xadd*2+1;
	}
	while (Xadd <= 0);
  }
  else
  {
    Yadd = -r2;
    Xadd = 0;
    do
    {
		K = (float)(rad2/rad1);

		Bsp_LcdDrawPoint(x-(uint16_t)(Xadd/K),y+Yadd,MyLCD.Color);
		Bsp_LcdDrawPoint(x+(uint16_t)(Xadd/K),y+Yadd,MyLCD.Color);
		Bsp_LcdDrawPoint(x+(uint16_t)(Xadd/K),y-Yadd,MyLCD.Color);
		Bsp_LcdDrawPoint(x-(uint16_t)(Xadd/K),y-Yadd,MyLCD.Color);

		e2 = err;
		if (e2 <= Xadd) {
		err += ++Xadd*3+1;
		if (-Yadd == Xadd && e2 <= Yadd) e2 = 0;
		}
		if (e2 > Yadd) err += ++Yadd*3+1;
    }
    while (Yadd <= 0);
  }
}

void Bsp_LcdFillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	DMA2D->CR	  &=	~(DMA2D_CR_START);
	DMA2D->CR		=	DMA2D_R2M;
	DMA2D->OPFCCR	=	MyLCD.ColorMode;
	DMA2D->OCOLR	=	MyLCD.Color;

	if(MyLCD.Direction == Direction_H)
	{
		DMA2D->OOR		=	LCD_Width - width;
		DMA2D->OMAR		=	MyLCD.LayerMemoryAdd + MyLCD.BytesPerPixel*(LCD_Width * y + x);
		DMA2D->NLR		=	(width<<16)|(height);
	}
	else
	{
		DMA2D->OOR		=	LCD_Width - height;
		DMA2D->OMAR		=	MyLCD.LayerMemoryAdd + MyLCD.BytesPerPixel*((LCD_Height - x - 1 - width)*LCD_Width + y);
		DMA2D->NLR		=	(width)|(height<<16);
	}
	DMA2D->CR	  |=	DMA2D_CR_START;
	while (DMA2D->CR & DMA2D_CR_START) ;
}

void Bsp_LcdFillCircle(uint16_t x, uint16_t y, uint16_t r)
{
  int32_t  D;    /* Decision Variable */
  uint32_t  CurX;/* Current X Value */
  uint32_t  CurY;/* Current Y Value */

  D = 3 - (r << 1);

  CurX = 0;
  CurY = r;

  while (CurX <= CurY)
  {
    if(CurY > 0)
    {
    	Bsp_LcdDrawLine(x - CurX, y - CurY,x - CurX,y - CurY + 2*CurY);
    	Bsp_LcdDrawLine(x + CurX, y - CurY,x + CurX,y - CurY + 2*CurY);
    }

    if(CurX > 0)
    {
    	Bsp_LcdDrawLine(x - CurY, y - CurX,x - CurY,y - CurX + 2*CurX);
    	Bsp_LcdDrawLine(x + CurY, y - CurX,x + CurY,y - CurX + 2*CurX);
    }
    if (D < 0)
    {
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }

  Bsl_LcdDrawCircle(x, y, r);
}

void Bsp_LcdDrawImage(uint16_t x,uint16_t y,uint16_t width,uint16_t height,const uint8_t *pImage)
{
   uint8_t   disChar;
	uint16_t  Xaddress = x;
	uint16_t  i=0,j=0,m=0;

	for(i = 0; i <height; i++)
	{
		for(j = 0; j <(float)width/8; j++)
		{
			disChar = *pImage;

			for(m = 0; m < 8; m++)
			{
				if(disChar & 0x01)
				{
					Bsp_LcdDrawPoint(Xaddress,y,MyLCD.Color);
				}
				else
				{
					Bsp_LcdDrawPoint(Xaddress,y,MyLCD.BackColor);
				}
				disChar >>= 1;
				Xaddress++;

				if( (Xaddress - x)==width )
				{
					Xaddress = x;
					y++;
					break;
				}
			}
			pImage++;
		}
	}
}

void Bsp_LcdDma2dTransferCompleteCallback(DMA2D_HandleTypeDef *hdma2d)
{
	isTransmittingData = 0;
}

uint32_t touchgfxDisplayDriverTransmitActive(void)
{
	return isTransmittingData;
}

void touchgfxDisplayDriverTransmitBlock(uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
	isTransmittingData = 1;
	Bsp_LcdDrawBitmap(x, y, w, h, pixels);
}

void Bsp_LcdDrawBitmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *pImage)
{
	uint16_t* sourceImage = (uint16_t*)pImage;
	uint32_t dstAddress = 0;

	hdma2d.Instance = DMA2D;
	hdma2d.Init.Mode = DMA2D_M2M;
	hdma2d.Init.ColorMode = MyLCD.ColorMode;

	if(MyLCD.Direction == Direction_H)
	{
		hdma2d.Init.OutputOffset		=	LCD_Width - width;
		dstAddress = MyLCD.LayerMemoryAdd + MyLCD.BytesPerPixel* (y * LCD_Width + x);
	}
	else
	{
		hdma2d.Init.OutputOffset		=	LCD_Width - height;
		dstAddress = MyLCD.LayerMemoryAdd + MyLCD.BytesPerPixel* ((LCD_Height - x - 1 - width)*LCD_Width + y);
	}
#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
	HAL_DMA2D_RegisterCallback(&hdma2d, HAL_DMA2D_TRANSFERCOMPLETE_CB_ID, Bsp_LcdDma2dTransferCompleteCallback);
#endif
	HAL_DMA2D_Init(&hdma2d);

	HAL_DMA2D_Start_IT(&hdma2d, (uint32_t)sourceImage, dstAddress, width, height);
	isTransmittingData = 1;
	while(isTransmittingData);
}
