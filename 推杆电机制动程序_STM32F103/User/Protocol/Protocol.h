#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#include "stm32f10x.h"

#define FRAME_LENGTH     9
enum DownMsgType 
{
	PosSetMsg = 1,
	PulseMsg,
};
enum UpMsgType 
{
	PosMsg = 1,
	UpPulseMsg,
};
extern uint8_t FrameBuffer[FRAME_LENGTH];
extern uint8_t FrameRevFlag;
extern float Postion_Set;

int8_t Msg_Process(uint8_t * FrameNum);
int8_t SendMsgToHost(uint8_t Frame_num, uint8_t Frame_type, uint32_t Frame_data);

#endif //__PROTOCOL_H__
































