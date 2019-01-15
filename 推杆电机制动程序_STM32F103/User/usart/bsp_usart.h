#ifndef __USART_H
#define	__USART_H


#include "stm32f10x.h"
#include <stdio.h>

/** 
  * ���ں궨�壬��ͬ�Ĵ��ڹ��ص����ߺ�IO��һ������ֲʱ��Ҫ�޸��⼸����
	* 1-�޸�����ʱ�ӵĺ꣬uart1���ص�apb2���ߣ�����uart���ص�apb1����
	* 2-�޸�GPIO�ĺ�
  */
	
	// ����1-USART1 :�������λ������ͨ��
#define  COM_USARTx                   USART3
#define  COM_USART_CLK                RCC_APB1Periph_USART3
#define  COM_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  COM_USART_BAUDRATE           115200

// USART GPIO ���ź궨��
#define  COM_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
#define  COM_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  COM_USART_TX_GPIO_PORT       GPIOB   
#define  COM_USART_TX_GPIO_PIN        GPIO_Pin_10
#define  COM_USART_RX_GPIO_PORT       GPIOB
#define  COM_USART_RX_GPIO_PIN        GPIO_Pin_11

#define  COM_USART_IRQ                USART3_IRQn
#define  COM_USART_IRQHandler         USART3_IRQHandler

void USART_Config(void);
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
#endif /* __USART_H */
