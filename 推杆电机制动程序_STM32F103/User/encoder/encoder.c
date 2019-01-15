#include "encoder.h"



//TIM4 编码器输入，TIM4_CH1作A相，TIM4_CH2作B相, J9接A号电机PB6 PB7
TIM_ICInitTypeDef  TIM_ICInitStructure;
void TIM4_Encoder_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);// 使能定时器时钟	  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//io口使能使用到的捕获编码器AB相借
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//浮空输入	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 				 
	GPIO_Init(GPIOB, &GPIO_InitStructure);	   //推挽输出 ，IO口速度为50MHz 
	
	//初始化TIM4的时基单元    
	TIM_TimeBaseStructure.TIM_Period = (PULS_PER_TURN - 1) * 4; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值80K	
	TIM_TimeBaseStructure.TIM_Prescaler =0; //设置用来作为TIMx时钟频率除数的预分频值  不分频	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 
	
	//配置对应寄存器为编码器接口模式以及配置相关的输入捕获配置  //THE MODE OF ENCODER      
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge); //上升沿和下降沿都捕获     	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上  	
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获  	
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上  	
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频   	
	TIM_ICInitStructure.TIM_ICFilter = 0x01;//IC1F=0000 配置输入滤波器 不滤波  	
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
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4   
}
	//定时器4中断服务程序

//计算圈数
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
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //清除中断标志位 
}

float Speed = 0;    //编码器的速度,单位为mm/s
float Postion = 0; //刹车的位置,单位为mm
/*
@brief:获取得到电机的位置信息，单位为mm
@arg:none
@return:none
*/
void Get_Encode_Position(void)
{ 
#if 0
		//static int32_t Postion = 0; //用来记录刹车的位置
		if((TIM4->CR1 & TIM_CounterMode_Down) != TIM_CounterMode_Down)
		{
				Postion += turn_num * PULS_PER_TURN + TIM_GetCounter(TIM4)/4;//向上计数，电机正转
		}
		else
		{
				Postion += turn_num * PULS_PER_TURN - (PULS_PER_TURN - TIM_GetCounter(TIM4)/4);//向下计数，电机正转
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


















































