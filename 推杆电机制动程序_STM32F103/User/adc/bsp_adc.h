#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"

//����������
#define SENSITIVITY       40.0    //��λ��mV/A
#define Ip_MAX            50     
#define VCC               5000    //��λ��mV

#define I_MOTOR_MAX       20000   //�������趨��λΪmA










// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
/********************ADC1����ͨ�������ţ�����**************************/
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOA  
#define    ADC_PORT                      GPIOA

// ת��ͨ������
#define    NOFCHANEL										 2

#define    ADC_BAT_1_PIN                 GPIO_Pin_4
#define    ADC_BAT_1_CHANNEL             ADC_Channel_4

#define    ADC_36V_1_PIN                 GPIO_Pin_5
#define    ADC_36V_1_CHANNEL             ADC_Channel_5


// ADC1 ��Ӧ DMA1ͨ��1��ADC3��ӦDMA2ͨ��5��ADC2û��DMA����
#define    ADC_x                         ADC1
#define    ADC_DMA_CHANNEL               DMA1_Channel1
#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1

/**************************��������********************************/
void ADCx_Init(void);
int8_t Read_BRK_Motor_Current(int32_t * Ip);

#endif /* __ADC_H */

