#ifndef __CONTROL_H__
#define __CONTROL_H__
#include "stm32f10x.h"

#define BRK_STAT_INIT               0
#define BRK_STAT_FULL_STOP          1000   //用脉冲数衡量的刹车位置


/********************基本定时器TIM参数定义，只限TIM6、7************/
#define CON_TIM6 // 如果使用TIM7，注释掉这个宏即可

#ifdef  CON_TIM6 // 使用基本定时器TIM6
#define            CON_TIM                   TIM6
#define            CON_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            CON_TIM_CLK               RCC_APB1Periph_TIM6
#define            CON_TIM_Period            10000-1
#define            CON_TIM_Prescaler         72-1
#define            CON_TIM_IRQ               TIM6_IRQn
#define            CON_TIM_IRQHandler        TIM6_IRQHandler

#else  // 使用基本定时器TIM7
#define            CON_TIM                   TIM7
#define            CON_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            CON_TIM_CLK               RCC_APB1Periph_TIM7
#define            CON_TIM_Period            1000-1
#define            CON_TIM_Prescaler         71
#define            CON_TIM_IRQ               TIM7_IRQn
#define            CON_TIM_IRQHandler        TIM7_IRQHandler

#endif
/**************************函数声明********************************/
void CON_TIM_Init(void);








//uint32_t BRK_Control(uint32_t PosSet);
#endif //__CONTROL_H__





























