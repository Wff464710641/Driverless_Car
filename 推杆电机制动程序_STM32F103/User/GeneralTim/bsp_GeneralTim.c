
#include "bsp_GeneralTim.h" 

static void PWM_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
	// ����Ƚ�ͨ��4 GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(PWM_TIM_CH4_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  PWM_TIM_CH4_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PWM_TIM_CH4_PORT, &GPIO_InitStructure);
	
	//����������ų�ʼ��
	RCC_APB2PeriphClockCmd(BRK_DIR_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  BRK_DIR_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(BRK_DIR_PORT, &GPIO_InitStructure);
}


///*
// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)

static void PWM_TIM_Mode_Config(void)
{
  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	PWM_TIM_APBxClock_FUN(PWM_TIM_CLK,ENABLE);

/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	// �������ڣ���������Ϊ100K
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=PWM_TIM_Period;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= PWM_TIM_Prescaler;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(PWM_TIM, &TIM_TimeBaseStructure);

	/*--------------------����ȽϽṹ���ʼ��-------------------*/	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	// ����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// ���ʹ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// ���ͨ����ƽ��������	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//�͵�ƽ��Ч

	// ����Ƚ�ͨ�� 4
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC4Init(PWM_TIM, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(PWM_TIM, TIM_OCPreload_Enable);
	
	// ʹ�ܼ�����
	TIM_Cmd(PWM_TIM, ENABLE);
}

void PWM_TIM_Init(void)
{
	PWM_TIM_GPIO_Config();
	PWM_TIM_Mode_Config();		
}
void BRK_Motor_Speed_Set(int16_t duty)
{
	if(duty > PWM_TIM_Period)  //�Ե�������źŽ�������
		duty = PWM_TIM_Period;
	if(duty < -PWM_TIM_Period)
		duty = -PWM_TIM_Period;
	
	if(duty > 0)//�����ת
	{
		GPIO_WriteBit(BRK_DIR_PORT, BRK_DIR_PIN, (BitAction)BRK_DIR_P);
		//TIM_Cmd(PWM_TIM, DISABLE);
		TIM3->CCR4 = (uint16_t)duty;
		
		//TIM_Cmd(PWM_TIM, ENABLE);
	}
	else if(duty < 0)// �����ת
	{
		GPIO_WriteBit(BRK_DIR_PORT, BRK_DIR_PIN, (BitAction)BRK_DIR_N);
		//TIM_Cmd(PWM_TIM, DISABLE);
		TIM3->CCR4 = (uint16_t)(0-duty);
		//TIM_Cmd(PWM_TIM, ENABLE);
	}
	else //ֹͣ
	{
		//TIM_Cmd(PWM_TIM, DISABLE);
		PWM_TIM->CCR4 = (uint16_t)0;
	}
}
/*********************************************END OF FILE**********************/
