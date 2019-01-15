#ifndef __USART_H
#define	__USART_H


#include "stm32f10x.h"
#include <stdio.h>

/** 
  * 串口宏定义，不同的串口挂载的总线和IO不一样，移植时需要修改这几个宏
	* 1-修改总线时钟的宏，uart1挂载到apb2总线，其他uart挂载到apb1总线
	* 2-修改GPIO的宏
  */
	
	// 串口1-USART1 :负责和上位机进行通信
#define  COM_USARTx                   USART3
#define  COM_USART_CLK                RCC_APB1Periph_USART3
#define  COM_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  COM_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
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
