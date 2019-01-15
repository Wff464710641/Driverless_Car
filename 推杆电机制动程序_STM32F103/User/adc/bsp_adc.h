#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"

//传感器参数
#define SENSITIVITY       40.0    //单位：mV/A
#define Ip_MAX            50     
#define VCC               5000    //单位：mV

#define I_MOTOR_MAX       20000   //最大电流设定单位为mA










// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
/********************ADC1输入通道（引脚）配置**************************/
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOA  
#define    ADC_PORT                      GPIOA

// 转换通道个数
#define    NOFCHANEL										 2

#define    ADC_BAT_1_PIN                 GPIO_Pin_4
#define    ADC_BAT_1_CHANNEL             ADC_Channel_4

#define    ADC_36V_1_PIN                 GPIO_Pin_5
#define    ADC_36V_1_CHANNEL             ADC_Channel_5


// ADC1 对应 DMA1通道1，ADC3对应DMA2通道5，ADC2没有DMA功能
#define    ADC_x                         ADC1
#define    ADC_DMA_CHANNEL               DMA1_Channel1
#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1

/**************************函数声明********************************/
void ADCx_Init(void);
int8_t Read_BRK_Motor_Current(int32_t * Ip);

#endif /* __ADC_H */

