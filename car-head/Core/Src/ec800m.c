#include "ec800m.h"
#include <stdio.h>
#include <stdlib.h>

void EC800M_SendAT(char *cmd)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)cmd, strlen(cmd), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);
}

void EC800M_Call(void)
{
    EC800M_SendAT("ATD0775445216;");
}

void EC800M_HangUp(void)
{
    EC800M_SendAT("ATH");
}

void EC800M_GPS_OPEN(void)
{
    EC800M_SendAT("AT+QGPS=1");
}


uint8_t EC800M_GetGPS(float *latitude, float *longitude)
{
    char ec800_receive[100] = {0};
    char *token;

    EC800M_SendAT("AT+QGPSLOC=2");
    HAL_UART_Receive(&huart1, (uint8_t *)ec800_receive, sizeof(ec800_receive) - 1, 3000);

    if (strstr(ec800_receive, "+QGPSLOC:") == NULL)
    {
        return 0;
    }

    token = strtok(ec800_receive, ",");   /* cum 1: "+QGPSLOC: 083629.0" - bo qua */
    token = strtok(NULL, ",");    /* cum 2: vi do */
    if (token == NULL)
    {
        return 0;
    }
    *latitude = atof(token);

    token = strtok(NULL, ",");    /* cum 3: kinh do */
    if (token == NULL)
    {
        return 0;
    }
    *longitude = atof(token);

    return 1;
}


void EC800M_SendSMS(const char *phone, const char *message)
{
    char cmd[40];
    uint8_t ctrlZ = 0x1A;

    EC800M_SendAT("AT+CMGF=1");
    HAL_Delay(300);

    snprintf(cmd, sizeof(cmd), "AT+CMGS=\"%s\"", phone);
    EC800M_SendAT(cmd);
    HAL_Delay(300);

    HAL_UART_Transmit(&huart1, (uint8_t *)message, strlen(message), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1, &ctrlZ, 1, HAL_MAX_DELAY);
}


void EC800M_Airbagwaring(void)
{
    float latitude, longitude;
    char msg[150];

    if (EC800M_GetGPS(&latitude, &longitude))
    {
        snprintf(msg, sizeof(msg),
            "XAY RA TAI NAN tai Vi tri: %.6f,%.6f",
            latitude, longitude);
    }
    else
    {
        snprintf(msg, sizeof(msg), "XAY RA TAI NAN, Khong lay duoc vi tri GPS.");
    }

    EC800M_SendSMS(PHONE_NUMBER, msg);
}