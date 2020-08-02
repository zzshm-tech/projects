
/**************************************************

***************************************************/

#include "include.h"

//#define BUF_NUM

static SendQueueStr 		SendQueue;               //发送队列缓冲区

static FiFoStr				CmdData;                 //命令回传缓冲区


/***************************************************
**	函数名称:
**	功能描述:
***************************************************/

unsigned short int ReadSendQueue(unsigned char *Source)
{
	int i;
	unsigned char tmp;
	
	if(SendQueue.QNum == 0)
		return 0;
	
	SL_Print("Builid Num:%d \r\n",SendQueue.QNum);

	if(SendQueue.QData[SendQueue.QRead].DataBig > 0)     //说明是盲区数据
	{
		SL_Print("Ready Blind Data\r\n");
		tmp = CalcCrc8(SendQueue.QData[SendQueue.QRead].DataBuf,SendQueue.QData[SendQueue.QRead].DataLen);
		if(tmp != SendQueue.QData[SendQueue.QRead].DataCrc)
		{
			ClearSendQueue();
			return 0;
		}
		for(i = 0; i < SendQueue.QData[SendQueue.QRead].DataLen;i++)
		{
			*(Source + i) = SendQueue.QData[SendQueue.QRead].DataBuf[i];
		}
		SL_Print("Return Blind Data\r\n");
		return SendQueue.QData[SendQueue.QRead].DataLen;
			
	}
	else
	{
		if(SendQueue.QWrite >= SendQueue.QRead)
		{
			if(SendQueue.QNum != (SendQueue.QWrite - SendQueue.QRead))
			{
				SendQueue.QWrite = 0;
				SendQueue.QRead = 0;
				SendQueue.QNum = 0;
				return 0;
			}
		}

		if(SendQueue.QWrite < SendQueue.QRead)
		{
			if(SendQueue.QNum != (SendQueue.QWrite + BLIND_NUM - SendQueue.QRead))
			{
				SendQueue.QWrite = 0;
				SendQueue.QRead = 0;
				SendQueue.QNum = 0;
				return 0;
			}
		}
			
		for(i = 0;i < SendQueue.QData[SendQueue.QRead].DataLen;i++)
		{
			*(Source + i) = SendQueue.QData[SendQueue.QRead].DataBuf[i];
		}
		
		return SendQueue.QData[SendQueue.QRead].DataLen;
	}
}


/***************************************************
**	函数名称:unsigned char WriteSendQueue(unsigned char *Buf,unsigned short int DataLen)
**	功能描述:写发送缓冲区
***************************************************/

unsigned char WriteSendQueue(unsigned char *Buf,unsigned short int DataLen)
{
	int i;

	if(DataLen > 500)    // 这里是判断长度
		return 0;
	
	if(SendQueue.QWrite >= BLIND_NUM)
		SendQueue.QWrite = 0;
	
	for(i = 0;i < DataLen;i++)
	{
		SendQueue.QData[SendQueue.QWrite].DataBuf[i] = *(Buf + i);
	}
	SendQueue.QData[SendQueue.QWrite].DataLen = DataLen;
	SendQueue.QData[SendQueue.QWrite].DataBig = 0;
	if(SendQueue.QNum < BLIND_NUM)         //发送缓冲区所存在的数据条数
		SendQueue.QNum++;
	SL_Print(">Write Send Queue Finsh....%d----%d\r\n",SendQueue.QWrite,SendQueue.QNum);
	SendQueue.QWrite++;
	
	
	return 1;
}


/***************************************************
**	函数名称:
**	功能描述:
***************************************************/

void ClearSendQueue(void)
{
	SendQueue.QData[SendQueue.QRead].DataLen = 0;
	SendQueue.QRead++;
	
	if(SendQueue.QRead >= BLIND_NUM)
		SendQueue.QRead = 0;
	
	if(SendQueue.QNum != 0)
		SendQueue.QNum--;
	
}


/************************************************
**	函数名称:
**	功能描述:
************************************************/

void WriteCmdDataBuf(unsigned char *Buf,unsigned short int DataLen)
{
	if(DataLen > 500)
		return;
	SL_Memcpy(CmdData.DataBuf,Buf,DataLen);
	CmdData.DataLen = DataLen;
}


/************************************************
**	函数名称:
**	功能描述:
**	输入参数:
************************************************/

unsigned short int ReadCmdDataBuf(unsigned char *buf)
{
	if(CmdData.DataLen > 0)
	{
		SL_Memcpy(buf,CmdData.DataBuf,CmdData.DataLen);
		return CmdData.DataLen;
	}
	
	return 0;
}

/************************************************
**	函数名称:
**	功能描述:
************************************************/

void ClearCmdData(void)
{
	CmdData.DataLen = 0;
}

/***************************************************
**	函数名称:unsigned char SlaveSendQueue(void)
**	功能描述:保存队列到文件系统
***************************************************/

void SaveSendQueue(void)
{	
	int i;
	unsigned char tmp = 0;
	MsgHeadStr *p;
	
	if(SendQueue.QNum > 0)
	{
		for(i = 0;i < SendQueue.QNum;i++)              //值盲区标识
		{
			p = (MsgHeadStr *)SendQueue.QData[SendQueue.QRead].DataBuf;
			p->blind_flag = 1;           //盲区标识
			p->msg_id = 0x46;            //盲区消息ID;
			SendQueue.QData[SendQueue.QRead].DataBig = 1;
			tmp = SendQueue.QData[SendQueue.QRead].DataLen;
			SendQueue.QData[SendQueue.QRead].DataBuf[tmp - 2] = BccVerify(SendQueue.QData[SendQueue.QRead].DataBuf + 3,tmp - 4);
			SendQueue.QData[SendQueue.QRead].DataCrc = CalcCrc8(SendQueue.QData[SendQueue.QRead].DataBuf, tmp);
			SendQueue.QRead++;
			if(SendQueue.QRead >= BLIND_NUM)
				SendQueue.QRead = 0;
		}
		
		if(FileCheckOrGreate((unsigned char *)"SendQueueFile", (unsigned char *)&SendQueue, sizeof(SendQueue),1) > 0)
			SL_Print(">App:Save SendQueue Data.......OK\r\n");
	}
	
}



/*************************************************
**	函数名称:
**	功能描述:装载数据
*************************************************/

void LoadSendQueue(void)
{
	unsigned short int tmp1,tmp2,tmp3;
	
	SL_Print("Start Load SendQueue Data............... %d\r\n",sizeof(SendQueue));
	if(FileCheckOrGreate((unsigned char *)"SendQueueFile", (unsigned char *)&SendQueue, sizeof(SendQueue),0) > 0)
	{
		if(SendQueue.QNum > BLIND_NUM || SendQueue.QRead > BLIND_NUM || SendQueue.QWrite > BLIND_NUM)
		{
			SendQueue.QNum = 0;
			SendQueue.QRead = 0;
			SendQueue.QWrite = 0;
		}
		
		//SL_Print(">App:Load SendQueue Data.......OK(%d)\r\n",SendQueue.QNum);
		if(SendQueue.QNum > 0)
		{
			tmp1 = SendQueue.QNum;
			tmp2 = SendQueue.QRead;
			tmp3 = SendQueue.QWrite;
			
			SendQueue.QNum = 0;
			SendQueue.QRead = 0;
			SendQueue.QWrite = 0;

			if(FileCheckOrGreate((unsigned char *)"SendQueueFile", (unsigned char *)&SendQueue, sizeof(SendQueue),1) > 0)
				SL_Print(">App:Claer Blind SendQueue Data.......OK\r\n");

			SendQueue.QNum = tmp1;
			SendQueue.QRead = tmp2;
			SendQueue.QWrite = tmp3;
		}
		
	}

	//SendQueue.QRead = 2;         //作为调试使用 
	//SendQueue.QNum = 10;
}



/*********************************************
**	函数名称:
**	功能描述:清除盲区数据
**********************************************/

void ClearBindData(void)
{
	SendQueue.QNum = 0;
	SendQueue.QRead = 0;
	SendQueue.QWrite = 0;
	SL_Print("Claer Bind Data Finsh..............\r\n");
		
}


/*****************************File End************************/

