
/***********************************
**	FileName:
**	Time:
***********************************/

#include "include.h"

static unsigned char UpLoadBuf[512];
static unsigned char DataBig;
/*************************************
**	函数名称:
**	功能描述:
*************************************/

void ProcessTcpIpUpLoad(void)
{
	static unsigned char step = 0;
	
	unsigned short int len = 0;
	unsigned char  tmp;
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
}

/********************File End**********************/


