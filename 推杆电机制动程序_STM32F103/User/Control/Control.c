#include "Control.h"
#include "Protocol.h"
#include "encoder.h"
#include "bsp_GeneralTim.h" 
//基本定时器TIMx,x[6,7]定时初始化函数


//中断优先级配置
static void CON_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = CON_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            都有
 *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
 *  TIM_Period               都有
 *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */

//10ms定时
static void CON_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启定时器时钟,即内部时钟CK_INT=72M
    CON_TIM_APBxClock_FUN(CON_TIM_CLK, ENABLE);
	
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period = CON_TIM_Period;	

	  // 时钟预分频数为
    TIM_TimeBaseStructure.TIM_Prescaler= CON_TIM_Prescaler;
	
		// 时钟分频因子 ，基本定时器没有，不用管
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// 计数器计数模式，基本定时器只能向上计数，没有计数模式的设置
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// 重复计数器的值，基本定时器没有，不用管
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // 初始化定时器
    TIM_TimeBaseInit(CON_TIM, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    TIM_ClearFlag(CON_TIM, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(CON_TIM,TIM_IT_Update,ENABLE);
		
		// 使能计数器
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
				if(PosSet > Postion)//电机正转
				{
					GPIO_WriteBit(BRK_DIR_PORT, BRK_DIR_PIN, (BitAction)BRK_DIR_P);
					BRK_Motor_Speed_Set(500);
					while(PosSet > Postion);//等待电机到达位置
					
				}
				else //电机反转
				{
					GPIO_WriteBit(BRK_DIR_PORT, BRK_DIR_PIN, (BitAction)BRK_DIR_N);
					BRK_Motor_Speed_Set(500);
					while(PosSet < Postion);//等待电机到达位置
				}
				BRK_Motor_Speed_Set(0);//停止电机
				Position_Set_Flag = 0;//清除电机位置置为标志
				return Postion;
}
*/
//速度环控制
int Incremental_PI(int Encoder, int Target)
{
	float Kp = 100, Ki = 100;
	static int Bias, Pwm, Last_bias;
	Bias = Encoder - Target;
	Pwm += Kp*(Bias - Last_bias) + Ki*Bias;
	Last_bias = Bias;
	return Pwm;
}
//位置环控制
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
