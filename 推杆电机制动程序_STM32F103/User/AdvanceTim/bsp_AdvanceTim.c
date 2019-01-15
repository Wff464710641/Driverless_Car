
#include "bsp_AdvanceTim.h" 

static void BRK_TIM_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// ����Ƚ�ͨ������ͨ�� CH1N GPIO ��ʼ��  ���Ƶ��B
	RCC_APB2PeriphClockCmd(BRK_TIM_CH1N_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  BRK_TIM_CH1N_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BRK_TIM_CH1N_PORT, &GPIO_InitStructure);

	// ����Ƚ�ͨ������ͨ�� CH2N GPIO ��ʼ��  ���Ƶ��A
	RCC_APB2PeriphClockCmd(BRK_TIM_CH2N_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  BRK_TIM_CH2N_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BRK_TIM_CH2N_PORT, &GPIO_InitStructure);	
	
	//���A��ʹ����������
	RCC_APB2PeriphClockCmd(MOTOR_A_ENABLE_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  MOTOR_A_ENABLE_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_A_ENABLE_GPIO_PORT, &GPIO_InitStructure);
	//ʧ�ܵ��
	GPIO_WriteBit(MOTOR_A_ENABLE_GPIO_PORT, MOTOR_A_ENABLE_GPIO_PIN, MOTOR_DISABLE);

	//���B��ʹ����������
	RCC_APB2PeriphClockCmd(MOTOR_B_ENABLE_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  MOTOR_B_ENABLE_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_B_ENABLE_GPIO_PORT, &GPIO_InitStructure);
	//ʧ�ܵ��
	GPIO_WriteBit(MOTOR_B_ENABLE_GPIO_PORT, MOTOR_B_ENABLE_GPIO_PIN, MOTOR_DISABLE);
	
	
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
// PWM �źŵ����� T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)

static void BRK_TIM_Mode_Config(void)
{
  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	BRK_TIM_APBxClock_FUN(BRK_TIM_CLK,ENABLE);

/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=BRK_TIM_PERIOD;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= BRK_TIM_PSC;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(BRK_TIM, &TIM_TimeBaseStructure);

	/*--------------------����ȽϽṹ���ʼ��-------------------*/		
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	// ����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// ���ʧ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	// �������ʹ��
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
	// ����ռ�ձȴ�С
	TIM_OCInitStructure.TIM_Pulse = BRK_TIM_PULSE;
	// ���ͨ����ƽ��������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// �������ͨ����ƽ��������
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	// ���ͨ�����е�ƽ��������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	// �������ͨ�����е�ƽ��������
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;
	TIM_OC1Init(BRK_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(BRK_TIM, TIM_OCPreload_Enable);
	
	// ʹ�ܼ�����
	TIM_Cmd(BRK_TIM, ENABLE);	
	// �����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��ʱ����䲻��Ҫ
	TIM_CtrlPWMOutputs(BRK_TIM, ENABLE);
}

void BRK_TIM_Init(void)
{
	BRK_TIM_GPIO_Config();
	BRK_TIM_Mode_Config();		
}
/*
* @brief :ͨ�������ʵ���ռ�ձ������ڵ����ת��
* @param :duty:ռ�ձ�Խ�������ת��Խ��
motor: BRK_MOTOR_A ����  BRK_MOTOR_B
* @return : 0-ת�����óɹ�  1-ʧ�� 
*/
int BRK_motor_speed_set(unsigned int motor, unsigned int duty)
{
	TIM_Cmd(BRK_TIM, DISABLE);
	if(motor == BRK_MOTOR_A)
	{
		BRK_TIM->CCR2 = duty;
	}
	if(motor == BRK_MOTOR_B)
	{
		BRK_TIM->CCR1 = duty;
	}
	TIM_Cmd(BRK_TIM, ENABLE);
	return 0;
}
/*********************************************END OF FILE**********************/
