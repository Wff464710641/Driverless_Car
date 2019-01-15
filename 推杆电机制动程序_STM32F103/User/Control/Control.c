#include "Control.h"
#include "Protocol.h"
#include "encoder.h"
#include "bsp_GeneralTim.h" 
//������ʱ��TIMx,x[6,7]��ʱ��ʼ������


//�ж����ȼ�����
static void CON_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = CON_TIM_IRQ ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            ����
 *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
 *  TIM_Period               ����
 *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */

//10ms��ʱ
static void CON_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    CON_TIM_APBxClock_FUN(CON_TIM_CLK, ENABLE);
	
		// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period = CON_TIM_Period;	

	  // ʱ��Ԥ��Ƶ��Ϊ
    TIM_TimeBaseStructure.TIM_Prescaler= CON_TIM_Prescaler;
	
		// ʱ�ӷ�Ƶ���� ��������ʱ��û�У����ù�
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// ����������ģʽ��������ʱ��ֻ�����ϼ�����û�м���ģʽ������
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// �ظ���������ֵ��������ʱ��û�У����ù�
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // ��ʼ����ʱ��
    TIM_TimeBaseInit(CON_TIM, &TIM_TimeBaseStructure);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(CON_TIM, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(CON_TIM,TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(CON_TIM, ENABLE);	
}

void CON_TIM_Init(void)
{
	CON_TIM_NVIC_Config();
	CON_TIM_Mode_Config();
}

/*
uint32_t BRK_Control(uint32_t PosSet)
{
				if(PosSet > Postion)//�����ת
				{
					GPIO_WriteBit(BRK_DIR_PORT, BRK_DIR_PIN, (BitAction)BRK_DIR_P);
					BRK_Motor_Speed_Set(500);
					while(PosSet > Postion);//�ȴ��������λ��
					
				}
				else //�����ת
				{
					GPIO_WriteBit(BRK_DIR_PORT, BRK_DIR_PIN, (BitAction)BRK_DIR_N);
					BRK_Motor_Speed_Set(500);
					while(PosSet < Postion);//�ȴ��������λ��
				}
				BRK_Motor_Speed_Set(0);//ֹͣ���
				Position_Set_Flag = 0;//������λ����Ϊ��־
				return Postion;
}
*/
//�ٶȻ�����
int Incremental_PI(int Encoder, int Target)
{
	float Kp = 100, Ki = 100;
	static int Bias, Pwm, Last_bias;
	Bias = Encoder - Target;
	Pwm += Kp*(Bias - Last_bias) + Ki*Bias;
	Last_bias = Bias;
	return Pwm;
}
//λ�û�����
int Position_PI(int Encoder, int Target)
{
	float Kp = 15, Ki = 0.01;
	static int Bias, Pwm, Integral_bias;
	Bias = Encoder - Target;
	Integral_bias += Bias;
	Pwm = Kp*Bias + Ki*Integral_bias;
	return Pwm;
}

/**
  * @brief  This function handles CON_TIM interrupt request.
  * @param  None
  * @retval None
  */
void  CON_TIM_IRQHandler (void)
{
	int Pwm = 0;
	if ( TIM_GetITStatus( CON_TIM, TIM_IT_Update) != RESET ) 
	{	
		Speed = Get_Encoder_Speed();
		Pwm = Position_PI(Postion, Postion_Set);
		Pwm = Incremental_PI(Speed, -Pwm);
		BRK_Motor_Speed_Set(Pwm);
		TIM_ClearITPendingBit(CON_TIM , TIM_FLAG_Update);  		 
	}		 	
}
