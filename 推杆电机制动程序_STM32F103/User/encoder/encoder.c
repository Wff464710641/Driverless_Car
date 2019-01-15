#include "encoder.h"



//TIM4 ���������룬TIM4_CH1��A�࣬TIM4_CH2��B��, J9��A�ŵ��PB6 PB7
TIM_ICInitTypeDef  TIM_ICInitStructure;
void TIM4_Encoder_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);// ʹ�ܶ�ʱ��ʱ��	  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//io��ʹ��ʹ�õ��Ĳ��������AB���
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//��������	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 				 
	GPIO_Init(GPIOB, &GPIO_InitStructure);	   //������� ��IO���ٶ�Ϊ50MHz 
	
	//��ʼ��TIM4��ʱ����Ԫ    
	TIM_TimeBaseStructure.TIM_Period = (PULS_PER_TURN - 1) * 4; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ80K	
	TIM_TimeBaseStructure.TIM_Prescaler =0; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ 
	
	//���ö�Ӧ�Ĵ���Ϊ�������ӿ�ģʽ�Լ�������ص����벶������  //THE MODE OF ENCODER      
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge); //�����غ��½��ض�����     	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��  	
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���  	
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��  	
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ   	
	TIM_ICInitStructure.TIM_ICFilter = 0x01;//IC1F=0000 ���������˲��� ���˲�  	
	TIM_ICInit(TIM4, &TIM_ICInitStructure);	
	
	// Enable the TIM4 Update Interrupt	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);   
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);      
	TIM_SetCounter(TIM4,0);   
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4   
}
	//��ʱ��4�жϷ������

//����Ȧ��
int turn_num = 0;
void TIM4_IRQHandler(void)
{  		
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)				
		{	    			
				if((TIM4->CR1 & TIM_CounterMode_Down) != TIM_CounterMode_Down)
				{
					turn_num++;
				}
				else
				{
					turn_num--;
				}
		}	    
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //����жϱ�־λ 
}

float Speed = 0;    //���������ٶ�,��λΪmm/s
float Postion = 0; //ɲ����λ��,��λΪmm
/*
@brief:��ȡ�õ������λ����Ϣ����λΪmm
@arg:none
@return:none
*/
void Get_Encode_Position(void)
{ 
#if 0
		//static int32_t Postion = 0; //������¼ɲ����λ��
		if((TIM4->CR1 & TIM_CounterMode_Down) != TIM_CounterMode_Down)
		{
				Postion += turn_num * PULS_PER_TURN + TIM_GetCounter(TIM4)/4;//���ϼ����������ת
		}
		else
		{
				Postion += turn_num * PULS_PER_TURN - (PULS_PER_TURN - TIM_GetCounter(TIM4)/4);//���¼����������ת
		}
		TIM_SetCounter(TIM4, 0);
		turn_num = 0;
#endif
		Postion = TIM_GetCounter(TIM4)/4/PULS_PER_TURN*DISTANCE_PER_TURN;
}
float Get_Encoder_Speed(void)
{
	static float PosPre = 0, Pos_Cur = 0;
	float speed = 0;
	//Pos_Cur = turn_num * PULS_PER_TURN + TIM4->CNT;
	//Pos_Cur = TIM_GetCounter(TIM4);
	//Get_Encode_Position();
	Postion = (float)TIM_GetCounter(TIM4)/4/PULS_PER_TURN*DISTANCE_PER_TURN;
	Pos_Cur = Postion;
	speed = Pos_Cur - PosPre;
	PosPre = Pos_Cur;
	return speed;
}


















































