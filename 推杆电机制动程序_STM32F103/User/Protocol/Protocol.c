#include "Protocol.h"
#include "bsp_usart.h"
#include "bsp_TiMbase.h"
uint8_t FrameBuffer[FRAME_LENGTH] = {0};
uint8_t FrameRevFlag = 0;
float Postion_Set = 0; //收到的位置设定信息，0<=Postion_Set<=TRAVEL_DISTANCE
int32_t PulseRev = 0;


uint16_t CheckSum(uint8_t * Buf, uint8_t Num)
{
	uint16_t sum = 0;
	while(Num)
	{
		sum += *Buf;
		Buf++;
		Num--;
	}
	return sum;
}

int8_t Msg_Process(uint8_t * FrameNum)
{
	uint16_t SumRev = 0;
	uint32_t Data;
	enum DownMsgType FrameType;
	if(FrameRevFlag)
	{
		FrameType = (enum DownMsgType)*(FrameBuffer + 2);
		SumRev = *(FrameBuffer + 7) | (*(FrameBuffer + 8) << 8);
		Data = (*(FrameBuffer + 3) << 0)| (*(FrameBuffer + 4) << 8)| (*(FrameBuffer + 5) << 16)| (*(FrameBuffer + 6) << 24);
		if(SumRev == CheckSum(FrameBuffer, FRAME_LENGTH - 2))
		{
			switch(FrameType)
			{
				case PosSetMsg:
				{
					if(Data > TRAVEL_DISTANCE)
						Data = TRAVEL_DISTANCE;
					Postion_Set = Data;
					//*FrameNum = *(FrameBuffer + 1);
					break;
				}
#if 0
				case PulseMsg:
				{
					PulthErrCounter = 0;
					PulseRev = Data;
				}				
#endif 
				
				default:
				{
					return -2;
				}
			}
			
		}
		else
		{
			//帧数据错误
		}
		FrameRevFlag = 0;
	}
	return -3;//未收到数据
}

int8_t SendMsgToHost(uint8_t Frame_num, uint8_t Frame_type, uint32_t Frame_data)
{
	uint8_t buf[FRAME_LENGTH] = {0};
	uint16_t Sum_send = 0;
	
	buf[0] = 0x66;
	buf[1] = Frame_num;
	buf[2] = Frame_type;
	buf[3] = (Frame_data >> 0) & 0xff;
	buf[4] = (Frame_data >> 8) & 0xff;
	buf[5] = (Frame_data >> 16) & 0xff;
	buf[6] = (Frame_data >> 24) & 0xff;
	
	Sum_send = CheckSum(buf, FRAME_LENGTH - 2);
	
	buf[7] = (Sum_send >> 0) & 0xff;
	buf[8] = (Sum_send >> 8) & 0xff;
	
	Usart_SendArray( COM_USARTx, buf, FRAME_LENGTH);
	
	return 0;
}































