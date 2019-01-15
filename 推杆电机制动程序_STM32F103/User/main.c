/***************************************
*TIM—高级定时器-PWM互补输出带死区时间应用
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
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{
	int8_t ErrNum = 0;
	uint8_t FrameNum = 0;
	int32_t Ip = 0;  //电机的电流数据，单位为mA
	
//	uint32_t PulseNum = 0;
//	uint32_t Pos = 0;
//	TIM4_Encoder_Init();
	BASIC_TIM_Init();//产生基本时钟定时功能
//	PWM_TIM_Init();
	//串口初始化
	USART_Config();
	LED_GPIO_Config();
	
	PWM_TIM_Init();
	
	
//	while(1)
//	{
//		BRK_Motor_Speed_Set(Ip);//测试代码
//	}
	//SendMsgToHost(FrameNum++, PosMsg, (uint32_t)Postion);
//	ADCx_Init();
	while(1)
	{ 
		ErrNum = Msg_Process(&FrameNum); //从上位机获得要到达的位置并设置标志位
		if(!ErrNum)
		{
			//数据错误
		}
		if(Flag_500ms >= 500)//每1ms增加1
		{
			SendMsgToHost(FrameNum++, PosMsg, (uint32_t)Postion);
			Flag_500ms = 0;
		}
		if(Flag_200ms >= 200)//每1ms增加1
		{
			if(Read_BRK_Motor_Current(&Ip))
			{
				//电机电流过载，停止电机
				BRK_Motor_Speed_Set(0);
			}
			Flag_200ms = 0;
		}
/*		//删除心跳功能
		if(Flag_200ms > 20)//每10ms增加1
		{
			SendMsgToHost(FrameNum++, UpPulseMsg, PulseNum++);
			Flag_200ms = 0;
		}	
*/
	}
}


/*********************************************END OF FILE**********************/

