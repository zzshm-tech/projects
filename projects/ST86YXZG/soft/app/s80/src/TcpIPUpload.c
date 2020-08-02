
/***********************************
**	FileName:
**	Time:
***********************************/

#include "include.h"

static unsigned char UpLoadBuf[512];

static unsigned char DataBig;

static unsigned char HeartBeat[] = {0x00,0x00,0x0D}; 

static struct stopwatch16 HeatSw;

/************************************
**	
**	
************************************/

void ResetHeatSw(void)
{
	init_stopwatch16(&HeatSw);          //复位心跳
}

/*************************************
**	函数名称:
**	功能描述:
*************************************/

void ProcessTcpIpUpLoad(void)
{
	static unsigned char step = 0,step1 = 0;
	
	unsigned short int len = 0;
	unsigned char  tmp;

	if(ReadRunMode() > 0)            //在升级ST87/ST86
		return;
	switch(step)
	{
		case 0:                 //取出数据，   
			if(ReadSendState() != 2)
				break;
			len = ReadCmdDataBuf(UpLoadBuf);     //有命令 就发送命令
			if(len > 0)
			{
				SL_Print(">App Start Send Cmd .......%d\r\n",len);
				if(AppSendTcpIpSocket(UpLoadBuf,len) == 1)
				{
					step++;           //调到发送 
					DataBig = 0;
					init_stopwatch16(&HeatSw);         //有数据发送
					break;
				}
			}
			len = ReadSendQueue(UpLoadBuf);    //正常发送数据 
			if(len > 0)
			{
				SL_Print(">App Start Send UpLoad .......%d\r\n",len);
				if(AppSendTcpIpSocket(UpLoadBuf,len) == 1)
				{
					step++;           		  //调到发送 
					DataBig = 1;			  //
					init_stopwatch16(&HeatSw);		//有数据发送  清除心跳
					break;
				}
			}
			break;
		case 1:
			tmp = ReadSendState();    		//检测到发送状态
			if(tmp == 2)
			{
				step++;
				break;
			}
			else if(tmp == 3 || tmp == 0)
			{
				step = 0;
				break;
			}
			break;
		case 2:               			//负责清楚缓冲区
			if(DataBig == 0)
			{
				ClearCmdData();
				SL_Print("App Clear Cmd Data............\r\n\r\n");
			}
			else
			{
				ClearSendQueue();
				SL_Print(">App Clear SendQueue............\r\n");
			}
			step = 0;
			break;
		default:
			break;
	
	}

	switch(step1)           //发送心跳包
	{	
		case 0: 
			init_stopwatch16(&HeatSw);
			step1++;
			break;
		case 1:
			if(read_stopwatch16_value(&HeatSw) < 3600)    //45秒钟
				break;
			step1 = 0;
			WriteCmdDataBuf(HeartBeat,3);
			//SL_Print("Send HeartBeat...................... \r\n");
			break;
		default:
			step1 = 0;
			break;
	}
}

/********************File End**********************/


