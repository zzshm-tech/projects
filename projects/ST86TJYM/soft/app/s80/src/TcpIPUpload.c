
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
	init_stopwatch16(&HeatSw);          //��λ����
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

	if(ReadRunMode() > 0)            //
		return;
	switch(step)
	{
		case 0:                 //  
			if(ReadSendState() != 2)
				break;
			len = ReadCmdDataBuf(UpLoadBuf);     //
			if(len > 0)
			{
				SL_Print(">App Start Send Cmd .......%d\r\n",len);
				if(AppSendTcpIpSocket(UpLoadBuf,len) == 1)
				{
					step++;           //
					DataBig = 0;
					init_stopwatch16(&HeatSw);         //
					break;
				}
			}
			len = ReadSendQueue(UpLoadBuf);    // 
			if(len > 0)
			{

				SL_Print(">App Start Send UpLoad .......%d\r\n",len);
				if(AppSendTcpIpSocket(UpLoadBuf,len) == 1)
				{
					step++;           		  //
					DataBig = 1;			  //
					init_stopwatch16(&HeatSw);		//
					break;
				}
			}
			break;
		case 1:
			tmp = ReadSendState();    		//
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
		case 2:               			//
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

	switch(step1)           //
	{	
		case 0: 
			init_stopwatch16(&HeatSw);
			step1++;
			break;
		case 1:
			if(read_stopwatch16_value(&HeatSw) < 3600)    //
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


