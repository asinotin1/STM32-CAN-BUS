#ifndef _BSP_LCD_H_
#define _BSP_LCD_H_

#include "main.h"
#include "fmc.h"
#include "tim.h"

#define HBP  43
#define VBP  12
#define HSW  1
#define VSW  1
#define HFP  8
#define VFP  8

#define LCD_Width     	480
#define LCD_Height    	272
#define LCD_MemoryAdd   SDRAM_BANK_ADDR

#define	ColorMode_0   LTDC_PIXEL_FORMAT_RGB565
#define LCD_NUM_LAYERS  1
#ifndef LCD_BL_Pin
#define TIMER_PWM_CHANNEL		htim12
#define TIMER_CCR_REGISTER		TIM12->CCR1
#define PWM_PIN_CHANNEL			TIM_CHANNEL_1
#endif

#if  LCD_NUM_LAYERS == 2
#define	ColorMode_1   LTDC_PIXEL_FORMAT_ARGB8888
#endif

#if ( ColorMode_0 == LTDC_PIXEL_FORMAT_RGB565 || ColorMode_0 == LTDC_PIXEL_FORMAT_ARGB1555 || ColorMode_0 ==LTDC_PIXEL_FORMAT_ARGB4444 )
	#define BytesPerPixel_0		2
#elif ColorMode_0 == LTDC_PIXEL_FORMAT_RGB888
	#define BytesPerPixel_0		3
#elif ColorMode_0 == LTDC_PIXEL_FORMAT_ARGB8888
	#define BytesPerPixel_0		4
#endif

#if LCD_NUM_LAYERS == 2
	#if ( ColorMode_1 == LTDC_PIXEL_FORMAT_RGB565 || ColorMode_1 == LTDC_PIXEL_FORMAT_ARGB1555 || ColorMode_1 == LTDC_PIXEL_FORMAT_ARGB4444 )
		#define BytesPerPixel_1		2
	#elif ColorMode_1 == LTDC_PIXEL_FORMAT_RGB888
		#define BytesPerPixel_1		3
	#else
		#define BytesPerPixel_1		4
	#endif
	#define LCD_MemoryAdd_OFFSET   LCD_Width * LCD_Height * BytesPerPixel_0
#endif

#define 	LCD_WHITE       0xffFFFFFF
#define 	LCD_BLACK       0xff000000

#define 	LCD_BLUE        0xff0000FF
#define 	LCD_GREEN       0xff00FF00
#define 	LCD_RED         0xffFF0000
#define 	LCD_CYAN        0xff00FFFF
#define 	LCD_MAGENTA     0xffFF00FF
#define 	LCD_YELLOW      0xffFFFF00
#define 	LCD_GREY        0xff2C2C2C

#define 	LIGHT_BLUE      0xff8080FF
#define 	LIGHT_GREEN     0xff80FF80
#define 	LIGHT_RED       0xffFF8080
#define 	LIGHT_CYAN      0xff80FFFF
#define 	LIGHT_MAGENTA   0xffFF80FF
#define 	LIGHT_YELLOW    0xffFFFF80
#define 	LIGHT_GREY      0xffA3A3A3

#define 	DARK_BLUE       0xff000080
#define 	DARK_GREEN      0xff008000
#define 	DARK_RED        0xff800000
#define 	DARK_CYAN       0xff008080
#define 	DARK_MAGENTA    0xff800080
#define 	DARK_YELLOW     0xff808000
#define 	DARK_GREY       0xff404040

#define		NEON_GREEN		0xff10FF08

#define	Direction_H	0
#define	Direction_V	1

#define ABS(X)  ((X) > 0 ? (X) : -(X))

void Bsp_LcdInit(void);
void Bsp_LcdSetBrightness(uint16_t uwBrightness);
void Bsp_LcdSetLayer(uint8_t ubLayer);
void Bsp_LcdSetColor(uint32_t ulColor);
void Bsp_LcdSetBackColor(uint32_t ulColor);
void Bsp_LcdClear(void);
void Bsp_LcdClearRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void Bsp_LcdSetDisplayDirection(uint8_t ubDirection);

void Bsp_LcdDrawPoint(uint16_t x,uint16_t y,uint32_t color);
uint32_t Bsp_LcdReadPoint(uint16_t x, uint16_t y);
void Bsp_LcdDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void BsP_LcdDrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void Bsl_LcdDrawCircle(uint16_t x, uint16_t y, uint16_t r);
void Bsp_LcdDrawEllipse(int x, int y, int r1, int r2);
void Bsp_LcdDrawEllipse(int x, int y, int r1, int r2);

void Bsp_LcdFillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void Bsp_LcdFillCircle(uint16_t x, uint16_t y, uint16_t r);
void Bsp_LcdDrawImage(uint16_t x,uint16_t y,uint16_t width,uint16_t height,const uint8_t *pImage);
uint32_t touchgfxDisplayDriverTransmitActive(void);
void Bsp_LcdDrawBitmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *pImage);

#endif // _BSP_LCD_H_
