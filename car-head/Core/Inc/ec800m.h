#ifndef EC800M_H
#define EC800M_H

#include "usart.h"
#include <string.h>
#include <stdint.h>

/* So dien thoai nhan tin canh bao tai nan - doi lai so cua ban */
#define PHONE_NUMBER   "0775445216"

void EC800M_SendAT(char *cmd);
void EC800M_Call(void);
void EC800M_HangUp(void);

void EC800M_GPS_OPEN(void);

/* Hoi module vi tri GPS hien tai. Tra ve 1 neu lay duoc, 0 neu khong */
uint8_t EC800M_GetGPS(float *latitude, float *longitude);

/* Gui 1 tin nhan SMS toi 1 so dien thoai */
void EC800M_SendSMS(const char *phone, const char *message);

/* Goi ham nay khi airbag no: tu dong lay vi tri GPS va gui SMS canh bao */
void EC800M_AirbagAlert(void);

#endif