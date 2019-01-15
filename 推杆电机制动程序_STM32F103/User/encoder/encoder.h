#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"
#define PULS_PER_TURN        50.0
#define DISTANCE_PER_TURN    10.0    // 单位为mm,电机转一圈推杆前进多少mm 
#define TRAVEL_DISTANCE      1200  // 单位为mm,整个推杆的行程

extern float Speed;
extern float Postion;

void TIM4_Encoder_Init(void);
//void Get_Encode_Position(void);
float Get_Encoder_Speed(void);



#endif  //__LED_H





















