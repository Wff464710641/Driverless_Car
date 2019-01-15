#ifndef __BSP_ADVANCETIME_H
#define __BSP_ADVANCETIME_H
/*
�߼���ʱ��ʹ��TIM1
���ŷֲ����ã�
TIM1_CH1N --> PB13 --> ���Ƶ��B
TIM1_CH2N --> PB14 --> ���Ƶ��A

PB1 --> ���A
PB0 --> ���B

*/

#include "stm32f10x.h"


#define            BRK_MOTOR_A               1
#define            BRK_MOTOR_B               0

#define            BRK_TIM                   TIM1
#define            BRK_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            BRK_TIM_CLK               RCC_APB2Periph_TIM1
// PWM �źŵ�Ƶ�� F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            BRK_TIM_PERIOD            (1000-1)
#define            BRK_TIM_PSC               (72-1)
#define            BRK_TIM_PULSE             1000

#define            BRK_TIM_IRQ               TIM1_UP_IRQn
#define            BRK_TIM_IRQHandler        TIM1_UP_IRQHandler


// TIM1 ����Ƚ�ͨ��CH1N�Ļ���ͨ��,���Ƶ��B
#define            BRK_TIM_CH1N_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            BRK_TIM_CH1N_PORT          GPIOB
#define            BRK_TIM_CH1N_PIN           GPIO_Pin_13
// TIM1 ����Ƚ�ͨ��CH2N�Ļ���ͨ��,���Ƶ��A
#define            BRK_TIM_CH2N_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            BRK_TIM_CH2N_PORT          GPIOB
#define            BRK_TIM_CH2N_PIN           GPIO_Pin_14

//���A��ʹ������
#define            MOTOR_A_ENABLE_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            MOTOR_A_ENABLE_GPIO_PORT     GPIOB
#define            MOTOR_A_ENABLE_GPIO_PIN      GPIO_Pin_1

//���B��ʹ������
#define            MOTOR_B_ENABLE_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            MOTOR_B_ENABLE_GPIO_PORT     GPIOB
#define            MOTOR_B_ENABLE_GPIO_PIN      GPIO_Pin_0
//���ʹ�ܿ���:ʹ�ú�����void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal)
#define            MOTOR_ENABLE               Bit_RESET
#define            MOTOR_DISABLE              Bit_SET
/**************************��������********************************/



void BRK_TIM_Init(void);
int BRK_motor_speed_set(unsigned int motor, unsigned int duty);

#endif	/* __BSP_ADVANCETIME_H */


