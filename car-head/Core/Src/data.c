#include "data.h"
#include "fdcan.h" 
volatile can_data_t can_data = {0};


void FDCAN_Filter_Config(void)
{
    FDCAN_FilterTypeDef sFilterConfig;

    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig.FilterID1 = 0x000;
    sFilterConfig.FilterID2 = 0x000;   

    if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK) // nhận mọi ID
    {
        Error_Handler();
    }
    if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) // bật ngắt CAN
    {
        Error_Handler();
    }

    if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK) // bắt đầu CAN bus
    {
        Error_Handler();
    }
}


void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != 0)   // có sự kiện mới
    {
        FDCAN_RxHeaderTypeDef RxHeader;
        uint8_t RxData[8];

        if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)   
        {
            uint16_t scaled = ((uint16_t)RxData[0] << 8) | RxData[1];
            float value = (float)scaled / 100.0f;

            switch (RxHeader.Identifier)
            {
                case CAN_ID_BODY_TEMP:
                    can_data.bodyTemp = value;
                    can_data.check_data_update = 0;  
                    break;

                case CAN_ID_BODY_HUM:
                    can_data.bodyHum = value;
                    can_data.check_data_update = 0;  
                    break;

                case CAN_ID_BODY_SPEED:
                    can_data.bodySpeed = value;
                    can_data.check_data_update = 0;  
                    break;

                case CAN_ID_BODY_AIRBAG:
                    can_data.bodyAirbag = (uint8_t)value;
                    can_data.check_data_update = 0;  
                    break;

                case CAN_ID_TAIL_TEMP:
                    can_data.tailEngineTemp = value;
                    can_data.check_data_update = 0;  
                    break;

                case CAN_ID_TAIL_STEERING:
                    can_data.tailSteering = value;
                    can_data.check_data_update = 0;  
                    break;

                case CAN_ID_TAIL_DISTANCE:
                    can_data.tailDistance = value;
                    can_data.check_data_update = 0; 
                    break;

                default:
                    break;
            }
        }
    }
}