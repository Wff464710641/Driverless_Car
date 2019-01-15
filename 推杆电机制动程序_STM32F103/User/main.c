/***************************************
*TIM���߼���ʱ��-PWM�������������ʱ��Ӧ��
****************************************/
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_TiMbase.h" 
#include "bsp_usart.h"
#include "encoder.h"
#include "bsp_GeneralTim.h" 
#include "Protocol.h"
#include "Control.h"
#include "bsp_adc.h"

void TIM4_Encoder_Init(void);
extern int32_t Postion;

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{
	int8_t ErrNum = 0;
	uint8_t FrameNum = 0;
	int32_t Ip = 0;  //����ĵ������ݣ���λΪmA
	
//	uint32_t PulseNum = 0;
//	uint32_t Pos = 0;
//	TIM4_Encoder_Init();
	BASIC_TIM_Init();//��������ʱ�Ӷ�ʱ����
//	PWM_TIM_Init();
	//���ڳ�ʼ��
	USART_Config();
	LED_GPIO_Config();
	
	PWM_TIM_Init();
	
	
//	while(1)
//	{
//		BRK_Motor_Speed_Set(Ip);//���Դ���
//	}
	//SendMsgToHost(FrameNum++, PosMsg, (uint32_t)Postion);
//	ADCx_Init();
	while(1)
	{ 
		ErrNum = Msg_Process(&FrameNum); //����λ�����Ҫ�����λ�ò����ñ�־λ
		if(!ErrNum)
		{
			//���ݴ���
		}
		if(Flag_500ms >= 500)//ÿ1ms����1
		{
			SendMsgToHost(FrameNum++, PosMsg, (uint32_t)Postion);
			Flag_500ms = 0;
		}
		if(Flag_200ms >= 200)//ÿ1ms����1
		{
			if(Read_BRK_Motor_Current(&Ip))
			{
				//����������أ�ֹͣ���
				BRK_Motor_Speed_Set(0);
			}
			Flag_200ms = 0;
		}
/*		//ɾ����������
		if(Flag_200ms > 20)//ÿ10ms����1
		{
			SendMsgToHost(FrameNum++, UpPulseMsg, PulseNum++);
			Flag_200ms = 0;
		}	
*/
	}
}


/*********************************************END OF FILE**********************/

