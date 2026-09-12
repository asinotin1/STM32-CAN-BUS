#ifndef HC_SR04_H
#define HC_SR04_H

#include "main.h"   
extern TIM_HandleTypeDef htim3;
extern volatile uint32_t t1;
extern volatile uint32_t t2;
extern volatile uint32_t pulse;
extern volatile uint8_t flag;

float HCSR04_Read(void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
 
#endif