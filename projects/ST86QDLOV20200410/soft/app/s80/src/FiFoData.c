
/**************************************************

***************************************************/

#include "include.h"

//#define BUF_NUM

static SendQueueStr 		SendQueue;               //���Ͷ��л�����

static FiFoStr				CmdData;                 //����ش�������




/**************************************************
**
**
**************************************************/

SendQueueStr *GetSendQueueSpace(void)
{
	return &SendQueue;
}

/***************************************************
**	��������:unsigned short int ReadSendQueue(unsigned char *Source)
**	��������:
***************************************************/

unsigned short int ReadSendQueue(unsigned char *Source)
{
	int i;
	unsigned char tmp;
	
	if(SendQueue.QNum == 0)
		return 0;
	
	//SL_Print("Builid Num:%d,%d \r\n",SendQueue.QNum,SendQueue.QRead);

	if(SendQueue.QData[SendQueue.QRead].DataBig > 0)     //˵����ä������
	{
		//SL_Print("Ready Blind Data\r\n");
		tmp = CalcCrc8(SendQueue.QData[SendQueue.QRead].DataBuf,SendQueue.QData[SendQueue.QRead].DataLen);
		if(tmp != SendQueue.QData[SendQueue.QRead].DataCrc)
		{
			ClearSendQueue();
			//SL_Print("The SendQueue \r\n");
			return 0;
		}
		for(i = 0; i < SendQueue.QData[SendQueue.QRead].DataLen;i++)
		{
			*(Source + i) = SendQueue.QData[SendQueue.QRead].DataBuf[i];
		}
		//SL_Print("Return Blind Data\r\n");    //����
		return SendQueue.QData[SendQueue.QRead].DataLen;
			
	}
	else
	{
		if(SendQueue.QWrite > SendQueue.QRead)
		{
			if(SendQueue.QNum != (SendQueue.QWrite - SendQueue.QRead))
			{
				//SL_Print("The Queue Reset(1) %d,%d,%d\r\n",SendQueue.QNum,SendQueue.QWrite,SendQueue.QRead);
				SendQueue.QWrite = 0;
				SendQueue.QRead = 0;
				SendQueue.QNum = 0;
				
				return 0;
			}
		}

		if(SendQueue.QWrite <= SendQueue.QRead)
		{
			if(SendQueue.QNum != (SendQueue.QWrite + BLIND_NUM - SendQueue.QRead))
			{
				//SL_Print("The Queue Reset(2) %d,%d,%d\r\n",SendQueue.QNum,SendQueue.QWrite,SendQueue.QRead);
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
		SL_Print(">App:SendQueue Read:%d,%d\r\n",SendQueue.QRead,SendQueue.QNum);
		return SendQueue.QData[SendQueue.QRead].DataLen;
	}
}


/***************************************************
**	��������:unsigned char WriteSendQueue(unsigned char *Buf,unsigned short int DataLen)
**	��������:д���ͻ�����
***************************************************/

unsigned char WriteSendQueue(unsigned char *Buf,unsigned short int DataLen)
{
	int i;
	//MsgInputStr *p;


	if(DataLen > BLIND_BUF)    // �ж����ݳ���
		return 0;
	
	if(SendQueue.QWrite != SendQueue.QRead || SendQueue.QNum == 0)
	{
		for(i = 0;i < DataLen;i++)
		{
			SendQueue.QData[SendQueue.QWrite].DataBuf[i] = *(Buf + i);
		}
		SendQueue.QData[SendQueue.QWrite].DataLen = DataLen;
		SendQueue.QData[SendQueue.QWrite].DataBig = 0;
		SendQueue.QNum++;
		SendQueue.QWrite++;
		if(SendQueue.QWrite >= BLIND_NUM)
			SendQueue.QWrite = 0;
	}
	else
	{
		if(++SendQueue.QRead >= BLIND_NUM)
			SendQueue.QRead = 0;
		SendQueue.QNum--;
		
		for(i = 0;i < DataLen;i++)
		{
			SendQueue.QData[SendQueue.QWrite].DataBuf[i] = *(Buf + i);
		}
		SendQueue.QData[SendQueue.QWrite].DataLen = DataLen;
		SendQueue.QData[SendQueue.QWrite].DataBig = 0;
		SendQueue.QNum++;
		SendQueue.QWrite++;
		if(SendQueue.QWrite >= BLIND_NUM)
			SendQueue.QWrite = 0;
	}
	
	SL_Print(">App:SendQueue Write:%d,%d,%d\r\n",SendQueue.QWrite,SendQueue.QRead,SendQueue.QNum);

	//p = (MsgInputStr *)(SendQueue.QData[SendQueue.QRead].DataBuf + sizeof(MsgQDHeadStr) + sizeof(MsgDeviceStr) + sizeof(MsgGnssStr));
	//SL_Print(">App:SendQueue Write:%d,%d,%d,%d\r\n",SendQueue.QWrite,SendQueue.QRead,SendQueue.QNum,p->MsgDataMode );
	return 1;
}


/***************************************************
**	��������:
**	��������:
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

**	��������:void WriteCmdDataBuf(unsigned char *Buf,unsigned short int DataLen)
**	��������:
************************************************/

void WriteCmdDataBuf(unsigned char *Buf,unsigned short int DataLen)
{
	if(DataLen > BLIND_BUF || CmdData.DataLen > 0)     //�жϻ�����������û�����ݣ�
		return;
	SL_Memcpy(CmdData.DataBuf,Buf,DataLen);
	CmdData.DataLen = DataLen;
}


/***********************************************
**	��������:unsigned short int ReadCmdDataBuf(unsigned char *buf)
**	��������:
************************************************/

unsigned short int ReadCmdDataBuf(unsigned char *buf)
{
	if(CmdData.DataLen > 0)         //      
	{
		SL_Memcpy(buf,CmdData.DataBuf,CmdData.DataLen);
		return CmdData.DataLen;
	}
	
	return 0;
}

/************************************************
**	��������:
**	��������:
************************************************/

void ClearCmdData(void)
{

	CmdData.DataLen = 0;
}

/***************************************************
**	��������:unsigned char SlaveSendQueue(void)
**	��������:������е��ļ�ϵͳ
***************************************************/

void SaveSendQueue(void)
{	
	int i;
	unsigned char tmp = 0;
	unsigned short int Read;
	MsgInputStr *p;
	
	if(SendQueue.QNum > 0)
	{
		Read = SendQueue.QRead;
		for(i = 0;i < SendQueue.QNum;i++)              //
		{
			p =(MsgInputStr *)(SendQueue.QData[Read].DataBuf + sizeof(MsgQDHeadStr) + sizeof(MsgDeviceStr) + sizeof(MsgGnssStr));
			
			//SL_Print("Save Blind:%d\r\n",p->MsgDataMode);

			p->MsgDataMode = 1;
			//SL_Print("Save Blind:%d\r\n",p->MsgDataMode);
			SendQueue.QData[Read].DataBig = 1;
			tmp = SendQueue.QData[Read].DataLen;
			SendQueue.QData[Read].DataBuf[tmp - 2] = BccVerify(SendQueue.QData[Read].DataBuf + 3,tmp - 4);
			SendQueue.QData[Read].DataCrc = CalcCrc8(SendQueue.QData[Read].DataBuf, tmp);
			Read++;
			if(Read >= BLIND_NUM)
				Read = 0;
		}
		
		if(FileCheckOrGreate((unsigned char *)"SendQueueFile", (unsigned char *)&SendQueue, sizeof(SendQueue),1) > 0)
			SL_Print(">App:Save SendQueue Data.......OK\r\n");
	}
	
}



/*************************************************
**	��������:
**	��������:װ������
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

	//SendQueue.QRead = 2;         //��Ϊ����ʹ�� 
	//SendQueue.QNum = 10;
}



/*********************************************
**	��������:
**	��������:���ä������
**********************************************/

void ClearBindData(void)
{
	SendQueue.QNum = 0;
	SendQueue.QRead = 0;
	SendQueue.QWrite = 0;
	SL_Print("Claer Bind Data Finsh..............\r\n");
		
}


/*****************************File End************************/

