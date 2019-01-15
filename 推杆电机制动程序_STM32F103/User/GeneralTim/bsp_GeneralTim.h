#ifndef __BSP_GENERALTIME_H
#define __BSP_GENERALTIME_H


#include "stm32f10x.h"
// 我们使用TIM3_CH4 -> PB1
// PB14控制电机的正转和反转 1 -> 反转        0 -> 正转

// PB14引脚宏定义           
#define            BRK_DIR_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            BRK_DIR_PORT          GPIOB
#define            BRK_DIR_PIN           GPIO_Pin_14
#define            BRK_DIR_P             0
#define            BRK_DIR_N             1
//TIM3配置相关宏
#define            PWM_TIM                   TIM3
#define            PWM_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            PWM_TIM_CLK               RCC_APB1Periph_TIM3
#define            PWM_TIM_Period            1000
#define            PWM_TIM_Prescaler         71
// TIM3 输出比较通道4
#define            PWM_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            PWM_TIM_CH4_PORT          GPIOB
#define            PWM_TIM_CH4_PIN           GPIO_Pin_1

/**************************函数声明********************************/
extern uint8_t Flag_10ms;
void PWM_TIM_Init(void);
void BRK_Motor_Speed_Set(int16_t duty);

#endif	/* __BSP_GENERALTIME_H */


