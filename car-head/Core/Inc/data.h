#ifndef DATA_H
#define DATA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define CAN_ID_BODY_TEMP      0x101
#define CAN_ID_BODY_HUM       0x102
#define CAN_ID_BODY_SPEED     0x103
#define CAN_ID_BODY_AIRBAG    0x104
#define CAN_ID_TAIL_TEMP      0x201
#define CAN_ID_TAIL_STEERING  0x203
#define CAN_ID_TAIL_DISTANCE  0x204



typedef struct
{
    float   bodyTemp;
    float   bodyHum;
    float   bodySpeed;
    uint8_t bodyAirbag;

    float   tailEngineTemp;
    float   tailSteering;
    float   tailDistance;

    uint8_t check_data_update;   
} can_data_t;

extern volatile can_data_t can_data;   


void FDCAN_Filter_Config(void);
#ifdef __cplusplus
}
#endif

#endif