
/***********************************************
**	File Name:
**	Time:

************************************************/

#include "include.h"


/****************************************************/

#define DEF_SERVER_ADDR "njgps-bc.lovol.com"
#define DEF_SERVER_PORT 15000



extern HANDLE SysAppEnterTask;

static TcpIPSocketStr TcpIpSocket;                  //������Ϣ

static unsigned char GprsRecvBuf[255];           	//Gprs���ݽ��ջ�����   			

static unsigned short int GprsRecvLen;              //���յ������ݳ���			

static unsigned short int FTPLoadNum = 0;


/************************************************
**	��������:
**	��������:
*************************************************/

TcpIPSocketStr *GetTcpIpSocketSpce(void)
{
	return &TcpIpSocket;
}



/*************************************************
**	��������:
**	��������:
*************************************************/

void AppGprsNetActRsp(U8 ucCidIndex, S32 slErrorCode)
{
	if(slErrorCode == SL_RET_OK)
	{
		SL_Print(">App:Net Action OK.............\r\n");
		TcpIpSocket.GprsNetActive = 1;           //��ʶ�����Ѿ�����
	}
	else
	{
		SL_Print(">App:Net Action Fail..........%d\r\n",slErrorCode);  
		TcpIpSocket.GprsNetActive = 2; 
	}
}


/****************************************************
**	��������:
**	��������:�����
****************************************************/

void AppGprsNetDeactRsp(U8 ucCidIndex, S32 slErrorCode)
{
	if(slErrorCode != SL_RET_OK)
    {
    	TcpIpSocket.GprsNetActive = 0; 
        SL_Print(">APP:gprs net deact rsp fail[%d]", slErrorCode);
    }
	else
	{
		TcpIpSocket.GprsNetActive = 2; 
		SL_Print(">APP:Gprs net deact rsp OK");
	}
}



/****************************************************
**	��������:
** 	��������:���������Ϣ
****************************************************/

void AppTcpipConnRsp(unsigned char ucCidIndex, int slSockId, BOOL bResult, int slErrorCode)
{
	if(bResult == TRUE)
	{
		TcpIpSocket.LinkState = 1;        //���ӳɹ�
		TcpIpSocket.LinkNum++;            //���Ӵ���  
	}
	else
	{
		TcpIpSocket.LinkState = 3;
	}
}


/*************************************************
**	��������:
**	��������:   
**************************************************/

void AppTcpipSendRsp(unsigned char ucCidIndex, int slSockId, BOOL bResult, int slErrorCode)
{
	if(bResult == TRUE)
	{
		SL_Print("Send Data Finsh........\r\n");
		TcpIpSocket.SendState = 2;
	}
	else
	{
		SL_Print("Send Data Fail.........%d\r\n",slErrorCode);
		TcpIpSocket.SendState = 3;
	}
}


/*************************************************
**	��������:
**	��������:  
**************************************************/

void AppTcpipRcvRsp(unsigned char ucCidIndex, int slSockId, BOOL bResult, int slErrorCode)
{
	unsigned int slRet = 0;

	SL_TASK stSltask;
	   
	if(bResult == FALSE)
	{
		SL_Print("SLAPP: socket receive fail[%d]", slErrorCode);
	}
	else              //��ȷ����
	{
		slRet = SL_TcpipSocketRecv(slSockId, GprsRecvBuf, sizeof(GprsRecvBuf));  //��ȡ��������
		if(slRet >= 0)
		{
			
			stSltask.element[0] = SL_GetAppTaskHandle();
			//SL_MEMBLOCK(GprsRecvBuf,slRet,16);
			
			GprsRecvLen = (slRet > 1000) ? 1000 : slRet;
			SL_AppSendMsg(stSltask.element[0], EVT_APP_GPRS_RECV, 0);	//�յ�GPRS����
		}
		else
		{
			SL_Print("SLAPP: socket receive fail, ret[%d]", slRet);
		}
	}
}


/***********************************************************
**	��������:
**	��������:
************************************************************/

void AppTcpipCloseRsp(unsigned char ucCidIndex, int slSockId, BOOL bResult, int slErrorCode)
{
    if(bResult != TRUE)
    {
    	TcpIpSocket.LinkState = 0;                               //��ȷ�ر�
        SL_Print(">APP: socket Close fail[%d]", slErrorCode);
    }
	else
	{
		TcpIpSocket.LinkState = 0;   
		SL_Print(">APP: socket[%d] Close OK", slSockId);          
	}
}


/***********************************************
**	��������:void InitTcpIP(void)
**	��������:
************************************************/

void InitTcpIp(void)
{
    SL_TCPIP_CALLBACK stSlTcpipCb;

    SL_Memset(&stSlTcpipCb, 0, sizeof(stSlTcpipCb));
	
    stSlTcpipCb.pstSlnetAct = AppGprsNetActRsp;
    stSlTcpipCb.pstSlnetDea = AppGprsNetDeactRsp;
    stSlTcpipCb.pstSlsockConn = AppTcpipConnRsp;
    stSlTcpipCb.pstSlsockSend = AppTcpipSendRsp;
    stSlTcpipCb.pstSlsockRecv = AppTcpipRcvRsp;
    stSlTcpipCb.pstSlsockClose = AppTcpipCloseRsp;
	
	
    SL_TcpipGprsNetInit(0, &stSlTcpipCb);         //ע��ص����� Ŀǰֻ��ʹ��0��ͨ��    
    
    TcpIpSocket.LinkEnable = 1;
}



/*******************************************
**	��������:
**	��������:
*******************************************/

void AppFtpOpenCB(S32 openst)
{
	if (SL_RET_OK == openst)
    {
    	TcpIpSocket.FtpLinkState = 1;
    }
    else
    {
    	TcpIpSocket.FtpLinkState = 2;
    }
}




/*******************************************
**	��������:
**	��������:����FTP�ļ���ɻص�����
*******************************************/
void AppFtpDownloadCB(U8 *cacheaddr, U32 cachelen, S32 dnldst)
{
    if (SL_RET_OK == dnldst)
    {
       SL_Print(">App: Ftp download success! Close ftp! %d\n",cachelen);
	   TcpIpSocket.FtpFileSapce = (unsigned char *)cacheaddr;
       TcpIpSocket.FtpFileLen = cachelen;
	   TcpIpSocket.FtpLoadFig = 2;
    }
    else
    {
       SL_Print(">App: Ftp download fail! Close ftp! %d\n",dnldst);
	   TcpIpSocket.FtpLoadFig = 3;
    }
}


/*******************************************
**	��������:
**	��������:  �ر�Ftp  ���ļ�������ɺ� �ر�RTC
*******************************************/

void AppFtpCloseCB(S32 closest)
{
    SL_EVENT event;
    SL_TASK hTask;

	UpgradeStatusStr *pUpgrade;
	
    TcpIpSocket.FtpLinkState = 0;
    SL_Print(">App: Ftp close success! %d   %d\n",closest,TcpIpSocket.FtpLoadFig ); 

	if(TcpIpSocket.FtpLoadFig == 2)
	{
		pUpgrade = GetUpgradeDataSpace();
		if(pUpgrade->UpgradeFlag == 0)
		{
			event.nEventId = SL_APP_UPDATE_REQ;
	    	event.nParam1 = (unsigned int)TcpIpSocket.FtpFileSapce;
	    	event.nParam2 = TcpIpSocket.FtpFileLen;
	    	hTask.element[0] = SysAppEnterTask;
	    	SL_SendEvents(hTask, &event);                   		 //����ʱ��  
		}
		else
		{
			WriteRunMode(1);                                    	// 
			SL_Print("App:Load STM32F App Load Finsh\r\n");
		}
	}
	
}



/*******************************************
**	��������:
**	��������:��ʼ��һ��FTP����Ҫ���õĻص�����
*******************************************/

static void AppFtpInit(void)
{
    SL_FTP_CALLBACK ftp_callbk;

    ftp_callbk.pstSlFtpLogin = AppFtpOpenCB;        //��FTP
    ftp_callbk.pstSlFtpDownload = AppFtpDownloadCB;   //�������FTP
    ftp_callbk.pstSlFtpClose = AppFtpCloseCB;          //�ر�FTP
    SL_Ftp_Callback_Init(&ftp_callbk);   
}



/*********************************************
**	��������:
**	��������:��FTP
**********************************************/

static unsigned char AppFtpOpen(void)
{
    SL_FTP_Connect_Info ftp_info;
    int ret = 0;
	
    ftp_info.u16DestPort = TcpIpSocket.FtpPort;
	
    SL_Strcpy(ftp_info.uaUrl, (const char *)TcpIpSocket.FtpAddr); 			 	//FTP��ַ
    SL_Strcpy(ftp_info.uaUsername, (const char *)TcpIpSocket.FtpUserName);		//FTP�û���
    SL_Strcpy(ftp_info.uaPassword, (const char *)TcpIpSocket.FtpUserPassd); 		//FTP����
	
    ret = SL_Ftp_Open(&ftp_info);
	if(ret == SL_RET_OK)
    	return 1;
	return 0;
	
}

/*******************************************************
**	��������:
**	��������:���ͺ���
*******************************************************/

unsigned char AppSendTcpIpSocket(unsigned char *buf,unsigned short int len)
{
	int rv;

	if(len == 0 || len > 1380)
		return 0;
	rv = SL_TcpipSocketSend(0,buf,len);
	if(rv > 0)
	{
		TcpIpSocket.SendState = 1;    //������
		return 1;
	}
	return 0;
}


/********************************************************
**	��������:
**	��������:���ط���״̬
********************************************************/

unsigned char ReadSendState(void)
{
	if(TcpIpSocket.LinkState != 2)
		return 0;
	if(TcpIpSocket.SendState == 1 )
		return 1;
	if(TcpIpSocket.SendState == 2|| TcpIpSocket.SendState == 0)
		return 2;
	return 3;                           //ʧ�ܣ�	
}


/*******************************************************
**	��������:
**	��������:��ȡICCID�ص�������
********************************************************/

void AppGetICCId(S32 slResult, U8* pucCcid, U8 ucCcidLenth)
{
	if(slResult == SL_RET_OK)
    {
      SL_Memcpy(TcpIpSocket.SimIccId,pucCcid,20);
	  TcpIpSocket.SimIccId[ucCcidLenth] = '\0';
	  SL_Print("Read SimIccId Finsh........%d\r\n",ucCcidLenth);
	}
}


/*************************************************
**	��������:void AppGetHostIpByNameCb(U8 ucCidIndex, S32 slErrorCode, U8* pucIpAddr)
**	��������:��ȡ�����ص�����
*************************************************/

void AppGetHostIpByNameCb(U8 ucCidIndex, S32 slErrorCode, U8* pucIpAddr)
{
	SL_Print(">App: SL_AppGetHostIpByNameCb cid[%d], result[%d]\r\n", ucCidIndex, slErrorCode);    //��Ϊ������Ϣʹ��
	SL_Print(">App: IP:%s\r\n", pucIpAddr);                                                       //��Ϊ������Ϣʹ��

	if(slErrorCode == 0)                						//
	{
		//
		TcpIpSocket.UlrToIPFlag = 1;//
		SL_Memcpy(TcpIpSocket.ServerAddr,pucIpAddr,20);
	}
}



/*************************************************************
**	��������:void ProcessSocketClient(void)
**	��������:
*************************************************************/

void ProcessSocketClient(void)
{
	static unsigned char 	step = 0;
	static unsigned int	 	counter = 0;
	static unsigned char 	erro_code = 0;
	static unsigned char 	SendCounter = 0;
	
	SysConfigStr			*pCfg;
	UpgradeStatusStr 		*pUpGrade;
	int 					slRet = 0;
	unsigned char 			tmp = 0;

	
	
	switch(step)
	{
		case 0:
			TcpIpSocket.SimState = SL_IsSIMInserted();    	//����Ƿ���Sim�� 
			if(TcpIpSocket.SimState == TRUE)
			{
				step++;
				SL_Print(">App:Sim Card OK\r\n");            //SIm��OK,
				break;
			}
			SL_Print(">App:Not Sim Card\r\n");
			break;
		case 1:
			slRet = SL_SimGetCCID(AppGetICCId);             //��ȡSIM  ICC_ID; 
			if(slRet == SL_RET_OK)
			{
				step++;
				break;
			}
			break;
		case 2:               								//��ȡSim��״̬  
			SL_GetDeviceCurrentRunState(&TcpIpSocket.SimStatus,&TcpIpSocket.NetWorkState,&slRet,&TcpIpSocket.GsmSign,&TcpIpSocket.ErrorCode);
			if(TcpIpSocket.SimStatus == 1 && TcpIpSocket.NetWorkState == 5)
			{
				SL_Print("Register Net..........OK\r\n");   //�ȴ�����ע��  
				TcpIpSocket.LinkState = 0;                  //�����Ѿ�ע��
				step++;
				erro_code = 0;
				break;
			}
			SL_Print("Register Net............\r\n");
			break;
		case 3:              //���ý��뷽ʽ
			pCfg = GetSysCfgSpace();
			SL_Memcpy(TcpIpSocket.GprsApn,pCfg->TerApn,20);
			//SL_Print(">App:Apn :%s\r\n",TcpIpSocket.GprsApn);
			slRet = SL_TcpipGprsApnSet(TcpIpSocket.GprsApn,"","");  	//���ý����
			if(slRet != SL_RET_OK)            							//����ͨ��
			{
				if(erro_code++ > 5)
					step = 14;
				break;
			}
			SL_Print(">Set Apn OK.....!\r\n");
			erro_code = 0;
			step++;
			break;
		case 4:                    							 //����
			slRet = SL_TcpipGprsNetGetState(&TcpIpSocket.GprsNetActive,&TcpIpSocket.NetGprsState);  //
			if(slRet == SL_RET_OK)
			{
				SL_Print(">App:NetGprsState:%d,GprsNetActive %d\r\n",TcpIpSocket.NetGprsState,TcpIpSocket.GprsNetActive);
				if(TcpIpSocket.GprsNetActive != 1 || TcpIpSocket.NetGprsState != 5)
				{
					
					slRet = SL_TcpipGprsNetActive();     //����������  
					if(slRet != SL_RET_OK)
					{
						if(erro_code++ > 5)
							step = 14;                  //��������
					}
					else
					{
						step++;
						TcpIpSocket.GprsNetActive = 0;
						counter = 0;
					}
				}
				else
				{
					step = 5;
				}
			}
			else
			{
				if(erro_code++ > 5)
					step = 14;
			}
			break;
		case 5:
			if(TcpIpSocket.GprsNetActive == 0 && counter++ < 20)
				break;
			if(TcpIpSocket.GprsNetActive == 2 || TcpIpSocket.GprsNetActive == 0)   		//����ʧ�ܣ�
			{
				step = 4;
				break;
			}
			TcpIpSocket.LinkState = 1;										//ע��
			if(TcpIpSocket.LinkType == 0)                 					//��������
				step++;
			else if(TcpIpSocket.LinkType == 1)            					//FTP����
				step = 15;                                					//
			erro_code = 0;
			break;
		case 6:
			slRet = SL_TcpipSocketCreate(0, SL_TCPIP_SOCK_TCP);   	 //��������
			if(slRet != SL_RET_OK)
    		{
				if(erro_code++ > 5)
					step = 14;                                 		//�����豸
				break;
    		}
			SL_Print(">App:	Creat Link......OK %d\r\n",slRet);
			step++;
			erro_code = 0;
			break;
		case 7:
			slRet = SL_TcpipSocketBind(0);                 			//�󶨱���Ip��ַ      
    		if(slRet != SL_RET_OK)
    		{
    			if(erro_code++ > 5)
					step = 14;
				break;
    		}
			step++;
			erro_code = 0;
			SL_Print(">App:Bind Addr ........OK\r\n");
			break;
		case 8:
			pCfg = GetSysCfgSpace();

			if(TcpIpSocket.LinkNum > 4)
			{
				TcpIpSocket.LinkNum = 0;                                                    //���Ӵ���
				SL_Memcpy(TcpIpSocket.ServerAddr,DEF_SERVER_ADDR,sizeof(DEF_SERVER_ADDR));  //ʹ��Ĭ��IP��ַ
				TcpIpSocket.ServerPort = DEF_SERVER_PORT;        //ʹ��Ĭ�϶˿�
			}
			else
			{
				SL_Memcpy(TcpIpSocket.ServerAddr,pCfg->ServerAddr,50);    	//���õ�ַ
				TcpIpSocket.ServerPort = pCfg->ServerPort;             		//��ȡ�˿�  
			}
			TcpIpSocket.LinkNum++;
			tmp= CheckServerAddr(TcpIpSocket.ServerAddr,SL_Strlen(TcpIpSocket.ServerAddr));
			//SL_Print(">>>>%s    %d\r\n",TcpIpSocket.ServerAddr,tmp);
			if(tmp == 0)          												   		//˵��������
			{
				tmp = SL_TcpipGetCid();
				slRet = SL_TcpipGetHostIpbyName(tmp, TcpIpSocket.ServerAddr, AppGetHostIpByNameCb);
				if(slRet != SL_RET_OK)
				{
					if(erro_code++ > 5)
						step = 14;
				}
				step++;
				erro_code = 0;
				counter = 0;
				break;
			}
			else                  // Ip��ַ
			{
				step = 10;             //
				erro_code = 0;
				counter = 0;
				break;
			}
		case 9:
			if(TcpIpSocket.UlrToIPFlag == 0 && counter++ < 30)   //�ȴ� 30 s
				break;
			if(TcpIpSocket.UlrToIPFlag == 1)
			{
				step++;
				counter = 0;
				erro_code = 0;												//��ȡ�����ɹ�
				pCfg = GetSysCfgSpace();
				if(SL_Strcmp(pCfg->UpdateAddr,TcpIpSocket.ServerAddr) > 0)
				{
					SL_Memcpy(pCfg->UpdateAddr,TcpIpSocket.ServerAddr,50);        //  
					SaveSysCfg();             //����ϵͳ����
				}
				break;
			}
			SL_Memcpy(TcpIpSocket.ServerAddr,pCfg->UpdateAddr,50);        // 
			step++;
			break;
		case 10:
			SL_Print(">App:Link ServerAddr(%d) %s:%d\r\n",TcpIpSocket.LinkNum,TcpIpSocket.ServerAddr,TcpIpSocket.ServerPort);    //���ӵ��ķ�����
			slRet = SL_TcpipSocketConnect(0, TcpIpSocket.ServerAddr,TcpIpSocket.ServerPort);   		//����������   
			
			if(slRet != SL_RET_OK)
			{
				if(erro_code++ > 5)
					step = 14;
			}
			else
			{
				step++;
				TcpIpSocket.LinkState = 0;          				//
				erro_code++;
				counter = 0;
			}
			break;
		case 11:
			if(TcpIpSocket.LinkState == 0 && counter++ < 50)   		  //�ȴ� 50 s
				break;
			if(TcpIpSocket.LinkState == 3)  //����ʧ��
			{
				step = 4;
				SL_Print(">App:Link Server.........Fail---1\r\n");    //����ʧ��
				break;
			}
			else if(TcpIpSocket.LinkState == 0)
			{
				step = 4;
				SL_Print(">App:Link Server.........Fail---2\r\n");     //����ʧ��
				break;
			}
			step++;
			counter = 0;           //�������֮��
			SL_Print(">APP:Link Server.........OK\r\n");
			break;
		case 12:
			slRet = SL_TcpipGetState(0);    					//��ȡ����״̬  
			if(slRet == SL_TCPIP_STATE_CONNECTOK)
			{
				TcpIpSocket.LinkState = 2;                    //��ʽ���ӳɹ�
			}
			else if(slRet == SL_TCPIP_STATE_CONNECTING)      //����������
			{
				step++;
				TcpIpSocket.LinkState = 0;
				break;
			}
			else
			{
				step = 4;
				TcpIpSocket.LinkState = 0;
				break;
			}
			
			counter++;
			if(counter > 20)                     					//û10���ӻ�ȡһ��״̬
			{ 
				counter = 0;
				SL_GetDeviceCurrentRunState(&TcpIpSocket.SimStatus,&TcpIpSocket.NetWorkState,&slRet,&TcpIpSocket.GsmSign,&TcpIpSocket.ErrorCode);
				TcpIpSocket.GsmSign /= 10;
				SL_Print(">App: The Gsm Sign Value: %d\r\n",TcpIpSocket.GsmSign);
			}
			
			if(TcpIpSocket.LinkState != 2)         //����״̬
			{	                                                  
				step = 4;                          //����ȥ���� 
				break;
			}

			if(TcpIpSocket.ResetLink > 1)          //
				TcpIpSocket.ResetLink--;
			
			if(TcpIpSocket.ResetLink == 1)        //�Ƿ���������
			{
				TcpIpSocket.ResetLink = 0;        //
				TcpIpSocket.LinkState = 0;
				step++;
				break;							  //�ر�����
			}
			
			if(TcpIpSocket.SendState == 1)        //��ط���״̬
			{
				if(SendCounter++ > 10)           //��10����֮�ڷ��ͻ�û����ɣ� ������������
				{
					step++;                   
					TcpIpSocket.LinkState = 0;
					TcpIpSocket.SendState = 0;
					SendCounter = 0;
				}
			}
			else
			{
				SendCounter = 0;               	  //���͵ȴ�ʱ��
			}
			break;
		case 13:
			slRet = SL_TcpipSocketClose(0);        //�ر�����
			//if(slRet == SL_RET_OK)
			//{
			//	step;                          //�ص����Ĳ����´�������
			//	break;
			//}
			step = 4;
			break;
		case 14:                                  	//��¼FTP
			SL_Print(">APP:Reset S86 Sys\r\n");   	//��������ϵͳ  
			SaveSendQueue(); 
			SL_Sleep(10000);							//�ȴ�10��
			SL_HstSendEvt(0x20161034);      		//������� 
            SL_PowerDown(0); 						//����
			break;
		case 15:                                    //��ʼ��FTP    ��15����ʼ����FTP����
			AppFtpInit(); 
			step++;
			erro_code = 0;
			if(FTPLoadNum++ > 2)
			{
					step = 14;         				//��������ϵͳ   ֱ������   
					break;
			}
			SL_Print("Init Ftp App.......\r\n");
			break;
		case 16:
			slRet = AppFtpOpen();                  //����
			if(slRet != 1)
    		{
    			if(erro_code++ > 5)
					step = 14;
				break;
    		}
			step++;
			counter = 0;
			break;
		case 17:                        							//�ȴ�FTP���ӳɹ� 
			if(TcpIpSocket.FtpLinkState== 0 && counter++ < 50)   	//�ȴ� 50
				break;
			if(TcpIpSocket.FtpLinkState == 2)  		//����ʧ��
			{
				step = 4;
				SL_Print("Link Ftp.........Fail---1\r\n");
				break;
			}
			else if(TcpIpSocket.FtpLinkState == 0)
			{
				step = 4;
				SL_Print("Link Ftp.........Fail---2\r\n");
				break;
			}
			step++;
			erro_code = 0;
			TcpIpSocket.FtpLoadFig = 0;
			SL_Print(">App:Link FTP.........OK\r\n");
			break;
		case 18:
			slRet = SL_Ftp_Download(TcpIpSocket.FtpFileName, 1);    //��ʼ����FTP�ļ�
				
			if(slRet != SL_RET_OK)                     //���ִ��ʧ��
			{
				if(erro_code++ > 5)
					step = 14;
			
			step++;
			counter = 0;
			TcpIpSocket.FtpLoadFig = 1;       //��������FTP�ļ�
			break;
		case 19:                             //�������� 
			if(TcpIpSocket.FtpLoadFig == 1 && counter++ < 90)             // 1����
				break;
			if(TcpIpSocket.FtpLoadFig == 1 || TcpIpSocket.FtpLoadFig == 3)   //FTP����ʧ��
			{ 
				SL_Print(">App:(%d)	DownLoad %s  .........Fail\r\n",FTPLoadNum,TcpIpSocket.FtpFileName);
				step++;
				break;
			}
			if(TcpIpSocket.FtpLoadFig == 2)    //�����ɹ�  ���سɹ�
			{
				step++;
				erro_code = 0;
			}
			break;
		case 20:                         				//�ر����� 
			slRet = SL_Ftp_Close();
			if(slRet != SL_RET_OK)                     	//���ִ��ʧ��
			{
				if(erro_code++ > 5)   
					step = 14;
				break;
			
			SL_Print(">App:	Start Close Ftp........\r\n");
			step++;
			counter = 0;
			break;
		case 21: 
			if(TcpIpSocket.FtpLinkState != 0 && counter++ < 50)
				break;
			TcpIpSocket.FtpLinkState= 0;
			pUpGrade = GetUpgradeDataSpace();
			if(pUpGrade->UpgradeFlag == 1 && TcpIpSocket.FtpLoadFig == 2)      
			{
				step = 22;
				counter = 0;
				break;
			}
			step = 4;
			break;
		default:
			if(counter++ < 120)
				break;
			step = 14;
			break;
			
	}
}



/***************************************************************
**	��������:
**	��������:�������������(�������ݴ���)
***************************************************************/

void ProcessGprsRecv(void)
{
	unsigned short int 			CmdID = 0,MSGLen = 0;
	unsigned char 				MsgID,tmp;
	SysConfigStr 				*pSysCfg;
	unsigned char 				*p;
	IntToChar     				TmpData;
	LongToChar					TmpData32;
	int 						StrLen;       //
	unsigned char 				Buf[50];
	MsgHeadStr					*pMsgHead;
	UpgradeStatusStr 			*pUpgrade;
	SysTimeStr					TmpTime;

	
	if(GprsRecvLen < 25)                    //���жϽ��յ����ݵĳ���
		return;
	if(GprsRecvBuf[0] != 0xF1 && GprsRecvBuf[1] != 0xF2 && GprsRecvBuf[2] != 0xFF)   //�жϰ�ͷ
		return;
	//SL_MEMBLOCK(GprsRecvBuf,GprsRecvLen,16);             //����ʹ��
	tmp = BccVerify(GprsRecvBuf + 3,GprsRecvLen - 4);    //����У����
	if(tmp != *(GprsRecvBuf + GprsRecvLen - 1))          //�Ƚ�У��
		return;
	
	
	MsgID = GprsRecvBuf[3];
	pMsgHead = (MsgHeadStr *)GprsRecvBuf;
	pMsgHead->msg_id = 0x01;
	pMsgHead->msg_body_num = 0x01;
	pMsgHead->msg_len = 0x01;
	SL_Memcpy(Buf,GprsRecvBuf,25);                 	//������������
	p = GprsRecvBuf + 25;
	TmpData.TTbyte[0] = *(p);
	TmpData.TTbyte[1] = *(p + 1);   
	CmdID = TmpData.IntII;  
	pSysCfg = GetSysCfgSpace();                     //
	
	if(pSysCfg->CfgFlag == 0x55)             		//����ģʽ����
	{
		switch (MsgID)
		{ 
			case 0x89:      //
				break;
			case 0xff:		//��Ȩ���������豸��Ӧ������	ID                     //
				break;
			case 0x02:		//ƽ̨��Ӧ������Ϣ  (���������Ϣ 20�Σ�������������)    //
				
				break;
			case 0x47:		//ƽ̨���豸�˵�����
				break;
			case 0x48:		//ƽ̨�����豸
				switch (CmdID)    //ƽ̨��������
				{
					case 0x0001:					//ƽ̨���������ü�Ȩ�������˿ں�IP(����)   
						TmpData.TTbyte[0] = GprsRecvBuf[29];
						TmpData.TTbyte[1] = GprsRecvBuf[30]; 
						pSysCfg->ServerPort = TmpData.IntII;     //��ȡ�˿�
						TmpData.TTbyte[0] = GprsRecvBuf[27];
						TmpData.TTbyte[1] = GprsRecvBuf[28];
						MSGLen = TmpData.IntII - 2;
						if(MSGLen > 50)
							MSGLen = 50;
						SL_Memcpy(pSysCfg->ServerAddr,&GprsRecvBuf[31],MSGLen);   //���õķ������˿� 
						pSysCfg->ServerAddr[MSGLen] = '\0';  
						
						SaveSysCfg();                           	//��������
						SL_Print("Set Server Addr......%s:%d",pSysCfg->ServerAddr,pSysCfg->ServerPort); 
						MSGLen = 25;
						Buf[MSGLen++] = 0x01;                          //��Ϣ������
						Buf[MSGLen++] = 0x00;
						TmpData.IntII = 2 + SL_Strlen(pSysCfg->ServerAddr);
						Buf[MSGLen++] = TmpData.TTbyte[0];                 //���ݳ���
						Buf[MSGLen++] = TmpData.TTbyte[1];
						SL_Memcpy(&Buf[MSGLen],pSysCfg->ServerAddr,TmpData.IntII);  //����IP 
						MSGLen += TmpData.IntII;       //
						Buf[MSGLen] = BccVerify(Buf + 3,MSGLen - 3);
						MSGLen++;
						Buf[MSGLen++] = 0x0D;
					
						WriteCmdDataBuf(Buf,MSGLen);     			//�ѷ�������д������Ͷ���
						TcpIpSocket.ResetLink = 10;               	//10����֮����������  
						break;
					case 0x0003://��ʱ�ϴ�ʱ����  
						TmpData32.TTbyte[0] = GprsRecvBuf[29];
						TmpData32.TTbyte[1] = GprsRecvBuf[30];
						TmpData32.TTbyte[2] = GprsRecvBuf[31];
						TmpData32.TTbyte[3] = GprsRecvBuf[32];

						if(TmpData32.LongLL < 5)						 //��
							TmpData32.LongLL = 5;
						if(TmpData32.LongLL > 120)
							TmpData32.LongLL = 120;
						
						pSysCfg->TravelUpLoadTime = TmpData32.LongLL; 
						SaveSysCfg();     //��������
						MSGLen = 25;
						Buf[MSGLen++] = 0x03;
						Buf[MSGLen++] = 0x00;   //��ϢID

						Buf[MSGLen++] = TmpData32.TTbyte[0];
						Buf[MSGLen++] = TmpData32.TTbyte[1];
						Buf[MSGLen++] = TmpData32.TTbyte[2];
						Buf[MSGLen++] = TmpData32.TTbyte[3];    //
						Buf[MSGLen] = BccVerify(Buf + 3,MSGLen - 3);
						MSGLen++;
						Buf[MSGLen++] = 0x0D;
						WriteCmdDataBuf(Buf,MSGLen);     //�ѷ�������д������Ͷ���
						SL_Print("Set UpLoad Time.....%d",pSysCfg->TravelUpLoadTime);
						break;
					case 0x0004://����ǰ������ʱ��
						TmpData32.TTbyte[0] = GprsRecvBuf[29];
						TmpData32.TTbyte[1] = GprsRecvBuf[30];
						TmpData32.TTbyte[2] = GprsRecvBuf[31];
						TmpData32.TTbyte[3] = GprsRecvBuf[32];

						if(TmpData32.LongLL < 300)						 //��
							TmpData32.LongLL = 300;
						if(TmpData32.LongLL > 86400)
							TmpData32.LongLL = 86400;
						
						pSysCfg->RunTime 	= TmpData32.LongLL;

						SaveSysCfg();    //��������
						MSGLen = 25;
						Buf[MSGLen++] = 0x04;
						Buf[MSGLen++] = 0x00;   //��ϢID

						Buf[MSGLen++] = TmpData32.TTbyte[0];
						Buf[MSGLen++] = TmpData32.TTbyte[1];
						Buf[MSGLen++] = TmpData32.TTbyte[2];
						Buf[MSGLen++] = TmpData32.TTbyte[3];    //
						Buf[MSGLen] = BccVerify(Buf + 3,MSGLen - 3);
						MSGLen++;
						Buf[MSGLen++] = 0x0D;             //��β
						WriteCmdDataBuf(Buf,MSGLen);     //�ѷ�������д������Ͷ���
						SL_Print("Set Run Time........%d\n",pSysCfg->RunTime);
						break;
					case 0x0005://�豸����ʱ��
						TmpData32.TTbyte[0] = GprsRecvBuf[29];
						TmpData32.TTbyte[1] = GprsRecvBuf[30];
						TmpData32.TTbyte[2] = GprsRecvBuf[31];
						TmpData32.TTbyte[3] = GprsRecvBuf[32];

						if(TmpData32.LongLL < 600)						 //��
							TmpData32.LongLL = 600;
						if(TmpData32.LongLL > 86400)
							TmpData32.LongLL = 86400;					
						pSysCfg->SleepTime 	= TmpData32.LongLL;  
						SaveSysCfg();
						MSGLen = 25;
						Buf[MSGLen++] = 0x05;
						Buf[MSGLen++] = 0x00;   //��ϢID
						Buf[MSGLen++] = TmpData32.TTbyte[0];
						Buf[MSGLen++] = TmpData32.TTbyte[1];
						Buf[MSGLen++] = TmpData32.TTbyte[2];
						Buf[MSGLen++] = TmpData32.TTbyte[3];    //
						Buf[MSGLen] = BccVerify(Buf + 3,MSGLen - 3); 
						MSGLen++;
						Buf[MSGLen++] = 0x0D;     //���Ӱ�β
						WriteCmdDataBuf(Buf,MSGLen);     //�ѷ�������д������Ͷ���
						
						SL_Print("Set Sleep Time.......%d\r\n",pSysCfg->SleepTime);
						break;
					case 0x0007://Уʱ(����ʱ��)
						TmpTime.Year	= GprsRecvBuf[29];
						TmpTime.Mon 	= GprsRecvBuf[30];
						TmpTime.MDay	= GprsRecvBuf[31];
						TmpTime.Hour 	= GprsRecvBuf[32];
						TmpTime.Min	    = GprsRecvBuf[33];
						TmpTime.Sec 	= GprsRecvBuf[34];
						TmpData32.LongLL = CalendarToUnix(TmpTime);
						SetSysTime(TmpData32.LongLL);            		//��������ʱ�亯��   

						MSGLen = 25;
						Buf[MSGLen++] = 0x07;
						Buf[MSGLen++] = 0x00;   //��ϢID
						Buf[MSGLen++] = TmpData32.TTbyte[0];
						Buf[MSGLen++] = TmpData32.TTbyte[1];
						Buf[MSGLen++] = TmpData32.TTbyte[2];
						Buf[MSGLen++] = TmpData32.TTbyte[3];    //
						Buf[MSGLen] = BccVerify(Buf + 3,MSGLen - 3); 
						MSGLen++;
						Buf[MSGLen++] = 0x0D; 
						WriteCmdDataBuf(Buf,MSGLen);     //�ѷ�������д������Ͷ���
						SL_Print("Set Sys Time.........%d\r\n",TmpData32.LongLL);
						break;
					case 0x00FD://ƽ̨Զ����  �չ����Գɹ�     
						//SL_MEMBLOCK(GprsRecvBuf,GprsRecvLen,16);        //
						GprsRecvLen = GprsRecvLen - 30;
						StrLen = GetComma(1,&GprsRecvBuf[29],GprsRecvLen);
						if(StrLen > 0)
						{
							SL_Memcpy(TcpIpSocket.FtpAddr,&GprsRecvBuf[29],StrLen - 1);     //ȥ�豸��ַ  
							SL_Print("FTP Server Addr:%s\r\n",TcpIpSocket.FtpAddr);     //������Ϣ�û�����
						}
							
						SL_Memset(GprsRecvBuf,'\0',20); 
						if(GetDataStr(1,2,&GprsRecvBuf[29],GprsRecvBuf,GprsRecvLen) > 0)        //�˿ں� 
						{
							TcpIpSocket.FtpPort = StrToDouble(GprsRecvBuf);
							SL_Print("FTP Server Port:%d\r\n",TcpIpSocket.FtpPort);     //
						}
							
						SL_Memset(GprsRecvBuf,'\0',20); 
						if(GetDataStr(2,3,&GprsRecvBuf[29],GprsRecvBuf,GprsRecvLen) > 0)       //�û����� 
						{
							SL_Memcpy(TcpIpSocket.FtpUserName,GprsRecvBuf,20);
							SL_Print("FTP UserName:%s\r\n",TcpIpSocket.FtpUserName);	 //
						}
						
						SL_Memset(GprsRecvBuf,'\0',20); 
						if(GetDataStr(3,4,&GprsRecvBuf[29],GprsRecvBuf,GprsRecvLen) > 0)       //�û�����
						{
							SL_Memcpy(TcpIpSocket.FtpUserPassd,GprsRecvBuf,20);
							SL_Print("FTP UserPassd:%s\r\n",TcpIpSocket.FtpUserPassd);  //
						}

						SL_Memset(GprsRecvBuf,'\0',20); 
						if(GetDataStr(4,5,&GprsRecvBuf[29],GprsRecvBuf,GprsRecvLen) > 0)       //�û����� 
						{
							SL_Memcpy(TcpIpSocket.FtpFileName,GprsRecvBuf,20);
							SL_Print("File Name:%s\r\n",TcpIpSocket.FtpFileName);
						}
						pUpgrade = GetUpgradeDataSpace();
						TcpIpSocket.LinkType = 1;
						TcpIpSocket.ResetLink = 10;       //10��֮����������
						pUpgrade->UpgradeFlag = 0;
						
						break;
					case 0x00FE://stm32Զ������
						GprsRecvLen = GprsRecvLen - 30;
						StrLen = GetComma(1,&GprsRecvBuf[29],GprsRecvLen);
						if(StrLen > 0)
						{
							SL_Memcpy(TcpIpSocket.FtpAddr,&GprsRecvBuf[29],StrLen - 1);     //ȥ�豸��ַ  
							SL_Print("FTP Server Addr:%s\r\n",TcpIpSocket.FtpAddr);     //������Ϣ�û�����
						}
							
						SL_Memset(GprsRecvBuf,'\0',20); 
						if(GetDataStr(1,2,&GprsRecvBuf[29],GprsRecvBuf,GprsRecvLen) > 0)        //�˿ں� 
						{
							TcpIpSocket.FtpPort = StrToDouble(GprsRecvBuf);
							SL_Print("FTP Server Port:%d\r\n",TcpIpSocket.FtpPort);     //
						}
							
						
						SL_Memset(GprsRecvBuf,'\0',20); 
						if(GetDataStr(2,3,&GprsRecvBuf[29],GprsRecvBuf,GprsRecvLen) > 0)       //�û����� 
						{
							SL_Memcpy(TcpIpSocket.FtpUserName,GprsRecvBuf,20);
							SL_Print("FTP UserName:%s\r\n",TcpIpSocket.FtpUserName);	 //
						}
						
						SL_Memset(GprsRecvBuf,'\0',20); 
						if(GetDataStr(3,4,&GprsRecvBuf[29],GprsRecvBuf,GprsRecvLen) > 0)       //�û�����
						{
							SL_Memcpy(TcpIpSocket.FtpUserPassd,GprsRecvBuf,20);
							SL_Print("FTP UserPassd:%s\r\n",TcpIpSocket.FtpUserPassd);  //
						}

						SL_Memset(GprsRecvBuf,'\0',20); 
						if(GetDataStr(4,5,&GprsRecvBuf[29],GprsRecvBuf,GprsRecvLen) > 0)       //�û����� 
						{
							SL_Memcpy(TcpIpSocket.FtpFileName,GprsRecvBuf,20);
							SL_Print("File Name:%s\r\n",TcpIpSocket.FtpFileName);
						}
						pUpgrade = GetUpgradeDataSpace();
						
						TcpIpSocket.LinkType = 1;
						TcpIpSocket.ResetLink = 10;       //10��֮����������
						pUpgrade->UpgradeFlag = 1;
						
						break;
					case 0x000A:
						StrLen = BuildCompletePacket(GprsRecvBuf,1);       //�������ݰ�
						WriteCmdDataBuf(GprsRecvBuf,StrLen);               //
															  	 		   //�����豸״̬
						SL_Print("StrLen %d\r\n",StrLen);
						return;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
	else                                                    //����ģʽ����
	{
		
	}
}




/******************************File End ***************************/

