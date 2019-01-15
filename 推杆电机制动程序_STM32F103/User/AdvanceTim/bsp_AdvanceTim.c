
#include "bsp_AdvanceTim.h" 

static void BRK_TIM_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// 输出比较通道互补通道 CH1N GPIO 初始化  控制电机B
	RCC_APB2PeriphClockCmd(BRK_TIM_CH1N_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  BRK_TIM_CH1N_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BRK_TIM_CH1N_PORT, &GPIO_InitStructure);

	// 输出比较通道互补通道 CH2N GPIO 初始化  控制电机A
	RCC_APB2PeriphClockCmd(BRK_TIM_CH2N_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  BRK_TIM_CH2N_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BRK_TIM_CH2N_PORT, &GPIO_InitStructure);	
	
	//电机A的使能引脚配置
	RCC_APB2PeriphClockCmd(MOTOR_A_ENABLE_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  MOTOR_A_ENABLE_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_A_ENABLE_GPIO_PORT, &GPIO_InitStructure);
	//失能电机
	GPIO_WriteBit(MOTOR_A_ENABLE_GPIO_PORT, MOTOR_A_ENABLE_GPIO_PIN, MOTOR_DISABLE);

	//电机B的使能引脚配置
	RCC_APB2PeriphClockCmd(MOTOR_B_ENABLE_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  MOTOR_B_ENABLE_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_B_ENABLE_GPIO_PORT, &GPIO_InitStructure);
	//失能电机
	GPIO_WriteBit(MOTOR_B_ENABLE_GPIO_PORT, MOTOR_B_ENABLE_GPIO_PIN, MOTOR_DISABLE);
	
	
}


///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

static void BRK_TIM_Mode_Config(void)
{
  // 开启定时器时钟,即内部时钟CK_INT=72M
	BRK_TIM_APBxClock_FUN(BRK_TIM_CLK,ENABLE);

/*--------------------时基结构体初始化-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=BRK_TIM_PERIOD;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= BRK_TIM_PSC;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(BRK_TIM, &TIM_TimeBaseStructure);

	/*--------------------输出比较结构体初始化-------------------*/		
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	// 配置为PWM模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 输出失能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	// 互补输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
	// 设置占空比大小
	TIM_OCInitStructure.TIM_Pulse = BRK_TIM_PULSE;
	// 输出通道电平极性配置
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// 互补输出通道电平极性配置
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	// 输出通道空闲电平极性配置
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	// 互补输出通道空闲电平极性配置
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;
	TIM_OC1Init(BRK_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(BRK_TIM, TIM_OCPreload_Enable);
	
	// 使能计数器
	TIM_Cmd(BRK_TIM, ENABLE);	
	// 主输出使能，当使用的是通用定时器时，这句不需要
	TIM_CtrlPWMOutputs(BRK_TIM, ENABLE);
}

void BRK_TIM_Init(void)
{
	BRK_TIM_GPIO_Config();
	BRK_TIM_Mode_Config();		
}
/*
* @brief :通过设置适当的占空比来调节电机的转速
* @param :duty:占空比越大代表了转速越高
motor: BRK_MOTOR_A 或者  BRK_MOTOR_B
* @return : 0-转速设置成功  1-失败 
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
