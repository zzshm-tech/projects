
/***********************************************
**	File Name:Gprs.c
**	Time:
************************************************/

#include "include.h"


/***********************/

//#define DEF_SERVER_ADDR "219.236.247.110"
//#define DEF_SERVER_PORT 13011


//#define DEF_SERVER_ADDR "122.51.215.44"
//#define DEF_SERVER_PORT 8500

#define DEF_SERVER_ADDR "gcjxgps.lovol.com"      
#define DEF_SERVER_PORT 4001

/*******************/





extern HANDLE SysAppEnterTask;											//


/********************本地全局变量***************************/

static TcpIPSocketStr 					TcpIpSocket;                  	//

static IpAddrBackStr  					IpAddrBack;                   	//

static unsigned char 					GprsRecvBuf[100];           	//  	

static unsigned char 					SMSRecvBuf[200];				//
		
static unsigned char 					SMSRecvNum[20];					//

static unsigned short int 				GprsRecvLen;              		//			

static SysCmdStr						ComCmdRes;						//

static unsigned char 					SetArgID;						//

static unsigned short int 				SMSRecvLen;                     //


/************************************************
**	:
**	:
*************************************************/

TcpIPSocketStr *GetTcpIpSocketSpce(void)
{
	return &TcpIpSocket;
}



/*************************************************
**	:
**	:
*************************************************/

void AppGprsNetActRsp(U8 ucCidIndex, S32 slErrorCode)
{
	if(slErrorCode == SL_RET_OK)
	{
		SL_Print(">App:Net Action OK.............\r\n");
		TcpIpSocket.GprsNetActive = 1;           //
	}
	else
	{
		SL_Print(">App:Net Action Fail..........%d\r\n",slErrorCode); 
		SL_Memset(TcpIpSocket.ServerAddr,'\0',50);
		TcpIpSocket.GprsNetActive = 2; 
	}
}


/****************************************************
**	
**	
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
**	
** 	
****************************************************/

void AppTcpipConnRsp(unsigned char ucCidIndex, int slSockId, BOOL bResult, int slErrorCode)
{
	if(bResult == TRUE)
	{
		TcpIpSocket.LinkState = 1;        //
		//SL_Print("Link OK :\r\n");
		
	}
	else
	{
		TcpIpSocket.LinkState = 3;
	}
}


/*************************************************
**	:
**	:   
**************************************************/

void AppTcpipSendRsp(unsigned char ucCidIndex, int slSockId, BOOL bResult, int slErrorCode)
{
	if(bResult == TRUE)
	{
		SL_Print(">App:Send Data Finsh........\r\n");
		TcpIpSocket.SendState = 2;
	}
	else
	{
		SL_Print("Send Data Fail.........%d\r\n",slErrorCode);
		TcpIpSocket.SendState = 3;
	}
}


/*************************************************
**	:
**	:  
**************************************************/

void AppTcpipRcvRsp(unsigned char ucCidIndex, int slSockId, BOOL bResult, int slErrorCode)
{
	unsigned int slRet = 0;

	SL_TASK stSltask;
	   
	if(bResult == FALSE)
	{
		SL_Print("SLAPP: socket receive fail[%d]", slErrorCode);
	}
	else              //
	{
		slRet = SL_TcpipSocketRecv(slSockId, GprsRecvBuf, sizeof(GprsRecvBuf));  //
		if(slRet >= 0)
		{
			
			stSltask.element[0] = SL_GetAppTaskHandle();
			//SL_MEMBLOCK(GprsRecvBuf,slRet,16);
			
			GprsRecvLen = (slRet > 1000) ? 1000 : slRet;
			SL_AppSendMsg(stSltask.element[0], EVT_APP_GPRS_RECV, 0);	//
		}
		else
		{
			SL_Print("SLAPP: socket receive fail, ret[%d]", slRet);
		}
	}
}


/***********************************************************
**	:
**	:
************************************************************/

void AppTcpipCloseRsp(unsigned char ucCidIndex, int slSockId, BOOL bResult, int slErrorCode)
{
    if(bResult != TRUE)
    {
    	TcpIpSocket.LinkState = 0;                               //
    	TcpIpSocket.SendState = 0;
        SL_Print(">APP: socket Close fail[%d]", slErrorCode);
    }
	else
	{
		TcpIpSocket.LinkState = 0;
		TcpIpSocket.SendState = 0;
		SL_Print(">APP: The Socket[%d] Close OK........%d\r\n", slSockId,bResult);            
	}
}


/***********************************************
**	:void InitTcpIP(void)
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
}



/*******************************************
**	:
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
**
*******************************************/
void AppFtpDownloadCB(U8 *cacheaddr, U32 cachelen, S32 dnldst)
{
    if (SL_RET_OK == dnldst)
    {
       SL_Print(">App: Ftp download success! Close ftp! %d\n",cachelen);
	   TcpIpSocket.FtpFileSapce = (unsigned char *)cacheaddr;
	   //TcpIpSocket.FtpFileSapce = (unsigned char *)GetSendQueueSpace();
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
**	
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
	    	SL_SendEvents(hTask, &event);                   		 //
		}
		else
		{
			WriteRunMode(1);                                    	// 
			SL_Print("App:Load STM32F App Load Finsh\r\n");
		}
	}
	
}



/*******************************************
**	
*******************************************/

static void AppFtpInit(void)
{
    SL_FTP_CALLBACK ftp_callbk;

    ftp_callbk.pstSlFtpLogin = AppFtpOpenCB;        //
    ftp_callbk.pstSlFtpDownload = AppFtpDownloadCB;   //
    ftp_callbk.pstSlFtpClose = AppFtpCloseCB;          //
    SL_Ftp_Callback_Init(&ftp_callbk);   
}



/*********************************************
**	
**********************************************/

static unsigned char AppFtpOpen(void)
{
    SL_FTP_Connect_Info ftp_info;
    int ret = 0;
	
    ftp_info.u16DestPort = TcpIpSocket.FtpPort;
	
    SL_Strcpy(ftp_info.uaUrl, (const char *)TcpIpSocket.FtpAddr); 			 	//
    SL_Strcpy(ftp_info.uaUsername, (const char *)TcpIpSocket.FtpUserName);		//
    SL_Strcpy(ftp_info.uaPassword, (const char *)TcpIpSocket.FtpUserPassd); 		//
	
    ret = SL_Ftp_Open(&ftp_info);
	if(ret == SL_RET_OK)
    	return 1;
	return 0;
	
}

/*******************************************************
**	
*******************************************************/

unsigned char AppSendTcpIpSocket(unsigned char *buf,unsigned short int len)
{

	int rv;

	if(len == 0 || len > 1380)
		return 0;
	rv = SL_TcpipSocketSend(0,buf,len);
	if(rv > 0)
	{
		TcpIpSocket.SendState = 1;    //
		return 1;
	}
	return 0;
}


/********************************************************
**	
********************************************************/

unsigned char ReadSendState(void)
{
	if(TcpIpSocket.LinkState != 2)
		return 0;
	if(TcpIpSocket.SendState == 1 )
		return 1;
	if(TcpIpSocket.SendState == 2|| TcpIpSocket.SendState == 0)
		return 2;
	return 3;                           //
}


/*******************************************************
**	
********************************************************/

void AppGetICCId(S32 slResult, U8* pucCcid, U8 ucCcidLenth)
{
	if(slResult == SL_RET_OK)
    {
      SL_Memcpy(TcpIpSocket.SimIccId,pucCcid,20);
	  TcpIpSocket.SimIccId[ucCcidLenth] = '\0';
	  SL_Print("Read SimIccId Finsh........%d,%s\r\n",ucCcidLenth,TcpIpSocket.SimIccId);
	}
}


/*************************************************
**	
*************************************************/

void AppGetHostIpByNameCb(U8 ucCidIndex, S32 slErrorCode, U8* pucIpAddr)
{
	
	if(slErrorCode == 0)                						//
	{
		TcpIpSocket.UlrToIPFlag = 1;//
		
		SL_Print(">App: SL_AppGetHostIpByNameCb cid[%d], result[%d]\r\n", ucCidIndex, slErrorCode);    //
			SL_Print(">App: IP:%s\r\n", pucIpAddr); 												   //
		SL_Memcpy(TcpIpSocket.ServerAddr,pucIpAddr,20);
	}
	else
	{
		SL_Print(">App: SL_AppGetHostIpByNameCb cid[%d], result[%d]\r\n", ucCidIndex, slErrorCode);    //
		SL_Print(">Get IP Fail.........\r\n"); 													  //
		TcpIpSocket.UlrToIPFlag = 2;             //
	}
}




/*****************************************************************
**	:
**	:
*****************************************************************/

void ReadIpAddrBack(void)
{
	unsigned char Flag;
	
	Flag = FileCheckOrGreate((unsigned char *)"IpAddrBackFile",(unsigned char *)&IpAddrBack,sizeof(IpAddrBack),0);
	if(Flag > 0)
		SL_Print("Read IpAddrBackFile OK.......\r\n");
}



/****************************************************************
** 
*****************************************************************/

void SaveIpAddrBack(void)
{
	unsigned char Flag;

	Flag = FileCheckOrGreate((unsigned char *)"IpAddrBackFile",(unsigned char *)&IpAddrBack,sizeof(IpAddrBack),1);
	if(Flag > 0)
		SL_Print("Write IpAddrBackFile OK.......\r\n");
}




/*************************************************************
**	函数名称:void ProcessSocketClient(void)
**	功能描述:
**	SL_NW_REG_STATUS
**	SL_NW_STATUS_NOTREGISTERED_NOTSEARCHING =  0,// Not registered and not searching. //
**  SL_NW_STATUS_REGISTERED_HOME =             1,//Registered and in the home area.   //
**  SL_NW_STATUS_NOTREGISTERED_SEARCHING =     2,//Not registered and searching for a new operator. 
**  SL_NW_STATUS_REGISTRATION_DENIED =         3,//Registration denied.    //
**  SL_NW_STATUS_UNKNOWN =                     4,//Unknown registration.   //
**  SL_NW_STATUS_REGISTERED_ROAMING =          5,//
*************************************************************/


void ProcessSocketClient(void)
{
	static unsigned char 	step = 0;
	static unsigned int	 	counter = 0;
	static unsigned char 	erro_code = 0;
	static unsigned char 	SendCounter = 0;
	//static unsigned int     cnt = 0;

	SysConfigStr			*pCfg;
	UpgradeStatusStr 		*pUpGrade;
	int 					slRet = 0;
	unsigned char 			tmp = 0;

/*
	cnt++;
	if(cnt < 450)
	*/
	if(ReadRunMode() > 0)
		return;
	switch(step)
	{
		case 0:
			TcpIpSocket.SimState = SL_IsSIMInserted();    	//
			
			if(TcpIpSocket.SimState == TRUE)
			{
				step++;
				TcpIpSocket.LinkCount = 0;
				SL_Print(">App:Sim Card OK\r\n");            //
				break;
			}
			SL_Print(">App:Not Sim Card\r\n");
			break;
		case 1:
			slRet = SL_SimGetCCID(AppGetICCId);             // 
			if(slRet == SL_RET_OK)
			{
				step++;
				//step = 23;    //
				break;
			}
			break;
		case 2:               								//
			SL_GetDeviceCurrentRunState(&TcpIpSocket.SimStatus,&TcpIpSocket.NetWorkState,&slRet,&TcpIpSocket.GsmSign,&TcpIpSocket.ErrorCode);
			TcpIpSocket.GsmSign /= 10;
			if(TcpIpSocket.SimStatus == 1 && (TcpIpSocket.NetWorkState == 1 || TcpIpSocket.NetWorkState == 5))
			{
				SL_Print("Register Net..........OK\r\n");   //  
				TcpIpSocket.LinkState = 0;                  // 
				TcpIpSocket.GprsLinkState = 0; 				//	
				step++;
				erro_code = 0;
				break;
			}
			SL_Print("Register Net............%d,%d\r\n",TcpIpSocket.NetWorkState,TcpIpSocket.GsmSign);   //
			break;
		case 3:              //
			slRet = SL_TcpipGprsNetGetState(&TcpIpSocket.GprsNetActive,&TcpIpSocket.NetGprsState);  //
			if(slRet == SL_RET_OK)
			{
				SL_Print(">App:NetGprsState:%d,GprsNetActive %d\r\n",TcpIpSocket.NetGprsState,TcpIpSocket.GprsNetActive);
				if(TcpIpSocket.GprsNetActive != 1)    //
				{
					pCfg = GetSysCfgSpace();
					SL_Memcpy(TcpIpSocket.GprsApn,pCfg->TerApn,10);
					slRet = SL_TcpipGprsApnSet(TcpIpSocket.GprsApn,"","");  	//
					if(slRet != SL_RET_OK)            							//
					{

						if(erro_code++ > 5)
							step = 14;           //
						break;
					}
					erro_code = 0;
					step++;
					break;
				}
				TcpIpSocket.LinkCount++;      //
				if(TcpIpSocket.LinkCount >= 10)
				{
					TcpIpSocket.LinkCount = 0;
					step = 23;
					break;
				}
				TcpIpSocket.LinkState = 1;										//
				TcpIpSocket.GprsLinkState = 1;									//
				step = 6;
				break;
			}
			break;
		case 4:                    							//
			slRet = SL_TcpipGprsNetActive();    			//  
			if(slRet != SL_RET_OK)
			{


				step = 2;               					//ִ
			}
			else
			{
				step++;
				TcpIpSocket.GprsNetActive = 0;
				counter = 0;
			}
			break;
		case 5:
			if(TcpIpSocket.GprsNetActive == 0 && counter++ < 20)
				break;
			if(TcpIpSocket.GprsNetActive == 2 || TcpIpSocket.GprsNetActive == 0)   		//
			{
				step = 2;
				break;
			}
			step++;
			TcpIpSocket.LinkState = 1;										//
			TcpIpSocket.GprsLinkState = 1;									//
			break;
		case 6:
			if(TcpIpSocket.LinkType == 1)            					//
			{
				step = 15;                                					//
				break;
			}
			slRet = SL_TcpipSocketCreate(0, SL_TCPIP_SOCK_TCP);   	 //
			if(slRet != SL_RET_OK)
    		{
				step = 2;                            		//
				break;
    		}
			
			SL_Print(">App:	Creat Link......OK %d\r\n",slRet);
			step++;
			erro_code = 0;
			break;
		case 7:             //
			pCfg = GetSysCfgSpace();  

			if(TcpIpSocket.LinkNum > 3)
			{
				TcpIpSocket.LinkNum = 0;                                                    //
				SL_Memcpy(TcpIpSocket.ServerAddr,DEF_SERVER_ADDR,sizeof(DEF_SERVER_ADDR));  //
				TcpIpSocket.ServerPort = DEF_SERVER_PORT;        //
			}
			else
			{
				SL_Memcpy(TcpIpSocket.ServerAddr,pCfg->ServerAddr,50);    	//
				TcpIpSocket.ServerPort = pCfg->ServerPort;             		// 
			}
			TcpIpSocket.LinkNum++;
			tmp= CheckServerAddr(TcpIpSocket.ServerAddr,SL_Strlen(TcpIpSocket.ServerAddr));
			//SL_Print(">>>>%s    %d\r\n",TcpIpSocket.ServerAddr,tmp);
			if(tmp == 0)   //
			{
				tmp = SL_TcpipGetCid();
				slRet = SL_TcpipGetHostIpbyName(tmp, TcpIpSocket.ServerAddr, AppGetHostIpByNameCb);
					
				TcpIpSocket.UlrToIPFlag = 0;  //
				step++;
				erro_code = 0;
				counter = 0;
				break;
			}
			else                  //
			{
				step = 9;             //
				erro_code = 0;
				counter = 0;
				break;
			}
		case 8:
			if(TcpIpSocket.UlrToIPFlag == 0 && counter++ < 30)   //
				break;
			
			if(TcpIpSocket.UlrToIPFlag == 1)
			{
				step++;
				counter = 0;
				erro_code = 0;												
				if(SL_Strcmp(IpAddrBack.IpAddr,TcpIpSocket.ServerAddr) != 0)
				{
					SL_Memcpy(IpAddrBack.IpAddr,TcpIpSocket.ServerAddr,50);        //  
					SaveIpAddrBack();        //
					//SL_Print("Save Sys Confing\r\n");
				}
				break;
			}
			
			SL_Memcpy(TcpIpSocket.ServerAddr,IpAddrBack.IpAddr,50);        // 
			step++;
			break;
		case 9:
			SL_Print(">App:Link ServerAddr(%d) %s:%d\r\n",TcpIpSocket.LinkNum,TcpIpSocket.ServerAddr,TcpIpSocket.ServerPort);    //
			slRet = SL_TcpipSocketConnect(0, TcpIpSocket.ServerAddr,TcpIpSocket.ServerPort);   		// 
			if(slRet != SL_RET_OK)
			{
				if(erro_code++ > 5)    //
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
		case 10:
			if(TcpIpSocket.LinkState == 0 && counter++ < 50)   		  //
				break;
			if(TcpIpSocket.LinkState == 3 || TcpIpSocket.LinkState == 0)  //
			{
				step = 2;
				SL_Print(">App:Link Server.........Fail\r\n");    //
				break;
			}
			step++;
			counter = 0;           //
			break;
		case 11:
			counter++;
			slRet = SL_TcpipGetState(0);    					//
			if(slRet == SL_TCPIP_STATE_CONNECTOK)               //
			{
				TcpIpSocket.LinkState = 2;                    //
				TcpIpSocket.GprsLinkState = 2;				  //
				TcpIpSocket.LinkNum = 0;
				step++;
				counter = 0;
				TcpIpSocket.LinkCount = 0;
				SL_Print(">APP:Link Server.........OK\r\n");
			}
			else if(slRet == SL_TCPIP_STATE_CONNECTING)       //
			{
				break;
			}
			if(counter++ < 10)                              //
				break;
			counter = 0;   									 //
			step = 13;
			break;
		case 12:                                               //
			slRet = SL_TcpipGetState(0);    					//
			if(slRet == SL_TCPIP_STATE_CONNECTOK)               //
			{
				TcpIpSocket.LinkState = 2;                    //
				TcpIpSocket.GprsLinkState = 2;				  // 
			}
			else
			{
				step = 2;
				TcpIpSocket.LinkState = 0;
				break;
			}
			
			counter++;
			if(counter > 20)                     					//
			{ 
				counter = 0;
				SL_GetDeviceCurrentRunState(&TcpIpSocket.SimStatus,&TcpIpSocket.NetWorkState,&slRet,&TcpIpSocket.GsmSign,&TcpIpSocket.ErrorCode);
				TcpIpSocket.GsmSign /= 10;
				//SL_Print(">App: The Gsm Sign Value: %d\r\n",TcpIpSocket.GsmSign);
			}
			
			if(TcpIpSocket.LinkState != 2)         //
			{	                                                  
				step = 2;                          // 
				break;
			}

			if(TcpIpSocket.ResetLink > 1)          //
				TcpIpSocket.ResetLink--;
			
			if(TcpIpSocket.ResetLink == 1)        //
			{
				TcpIpSocket.ResetLink = 0;        //
				TcpIpSocket.LinkState = 0;
				step++;							  //
				break;							  //
			}
			
			if(TcpIpSocket.SendState == 1)        //
			{
				if(SendCounter++ > 20)           //
				{
					step++;                   
					TcpIpSocket.LinkState = 0;
					TcpIpSocket.SendState = 0;
					TcpIpSocket.GprsLinkState = 0;
					SendCounter = 0;
				}
			}
			else
			{
				SendCounter = 0;               	  //
			}
			break;
		case 13:
			slRet = SL_TcpipSocketClose(0);        //
			//if(slRet == SL_RET_OK)
			//{
			//	step;                          //
			//	break;

			//}
			//SL_Print("TcpipSocketClose........\r\n");
			step = 2;
			break;
		case 14:                                  	//
			SL_Print(">APP:Reset S86 Sys\r\n");   	//
			SaveSendQueue(); 
			SL_Sleep(10000);							//
			SL_HstSendEvt(0x20161034);      		//
            SL_PowerDown(0); 						//
			break;
		case 15:                                    // 
			if(TcpIpSocket.FtpLinkNum++ > 5)
			{
				TcpIpSocket.FtpLinkNum = 0;
				TcpIpSocket.LinkType = 0;
				step = 2;
				break;
			}
			AppFtpInit();                            
			step++;
			erro_code = 0;
			SL_Print("Init Ftp App.......\r\n");
			break;
		case 16:
			slRet = AppFtpOpen();                  //
			if(slRet != 1)
    		{
    			if(erro_code++ > 5)
					step = 14;
				break;
    		}
			step++;
			TcpIpSocket.FtpLinkState = 0;
			counter = 0;
			SL_Print("Open Ftp App.........\r\n");
			break;
		case 17:                        							//
			if(TcpIpSocket.FtpLinkState == 0 && counter++ < 50)   	//
				break;
			if(TcpIpSocket.FtpLinkState == 2)  		//
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
			slRet = SL_Ftp_Download(TcpIpSocket.FtpFileName, 1);    //
				
			if(slRet != SL_RET_OK)                     //
			{
				if(erro_code++ > 5)
					step = 14;
			
}
			step++;
			counter = 0;
			TcpIpSocket.FtpLoadFig = 1;       //
			break;
		case 19:                             //
			if(TcpIpSocket.FtpLoadFig == 1 && counter++ < 120)             //
				break;
			if(TcpIpSocket.FtpLoadFig == 1 || TcpIpSocket.FtpLoadFig == 3)   //
			{ 
				SL_Print(">App:DownLoad %s  .........Fail\r\n",TcpIpSocket.FtpFileName);
				step = 14;         //
				break; 
			}
			if(TcpIpSocket.FtpLoadFig == 2)    //
			{
				step++;
				erro_code = 0;
			}
			break;
		case 20:                         				//
			slRet = SL_Ftp_Close();
			if(slRet != SL_RET_OK)                     	//
			{
				if(erro_code++ > 5)   
					step = 14;
				break;
			
}
			step++;
			counter = 0;
			TcpIpSocket.FtpLinkState = 1;
			break;
		case 21: 
			if(TcpIpSocket.FtpLinkState != 0 && counter++ < 50)
				break;
			TcpIpSocket.FtpLinkState= 0;
			SL_Print(">App:Close Ftp Link........OK\r\n");  //
			pUpGrade = GetUpgradeDataSpace();
			if(pUpGrade->UpgradeFlag == 1 && TcpIpSocket.FtpLoadFig == 2)      
			{
				step = 22;
				counter = 0;
				break;
			}
			step = 14;
			break;
		case 22:
			if(counter++ < 120)    			//
				break;
			step = 14;
			break;
		case 23:
			slRet = SL_TcpipGprsNetDeactive();          //
			if(slRet != SL_RET_OK)                     	//
			{
				if(erro_code++ > 5)                     //
					step = 14;
				break;
			
}
			//SL_Print("Start Run SL_TcpipGprsNetDeactive:%d\r\n",slRet);
			TcpIpSocket.GprsAttachState = 0;
			counter = 0;
			step++;
			break;
		case 24:
			if(counter++ < 50 && TcpIpSocket.GprsAttachState == 0)
				break;
			//SL_Print("End Run SL_TcpipGprsNetDeactive:%d,%d\r\n",counter,TcpIpSocket.GprsAttachState);  
			if(TcpIpSocket.GprsAttachState == 0)
			{
				step = 14;
				break;
			}
			step = 2;
			break;
		default:
			step = 14;
			break;
			
	}
}




/*****************************************
**	
**	
*****************************************/

void ProcessGprsRecv(void)
{

	unsigned short int 			CmdID = 0;//MSGLen = 0;
	unsigned char 				tmp;
	SysConfigStr 				*pSysCfg;
	unsigned char 				*p;
	IntToChar     				TmpData;
	int 						StrLen;       //
	unsigned char 				Buf[50];      //
	UpgradeStatusStr 			*pUpgrade;
	SysCmdStr					*pSysCmd;     //
	LockCarStr					*pLockCar;    //
	
	if(GprsRecvLen < 25)                    //
		return;
	SL_MEMBLOCK(GprsRecvBuf,GprsRecvLen,16);			   //

	pSysCmd = (SysCmdStr *)GprsRecvBuf;
	
	if(pSysCmd->FrameStart[0] != 0xFF && pSysCmd->FrameStart[1] != 0xFF && pSysCmd->FrameStart[2] != 0xFF)   //
		return;
	
	tmp = VerfySum(GprsRecvBuf,GprsRecvLen - 3);    			//  
	         						 							
	CmdID = 32 * PowTow((double)tmp) + 23 * tmp + 71;
	TmpData.TTbyte[0] = *(GprsRecvBuf + GprsRecvLen - 3);   	//
	TmpData.TTbyte[1] = *(GprsRecvBuf + GprsRecvLen - 2);   	//
	
	if(CmdID != TmpData.IntII)          	   						//
		return;
	
	if(pSysCmd->TypeCode != 0x06)           //
		return;
	if(pSysCmd->MsgId != 0x47 && pSysCmd->MsgId != 0x48)
		return;
	pSysCfg = GetSysCfgSpace();                     //

	//SL_Print("Run this is......1\r\n");
	///*
	if(StrCompare(pSysCfg->TerminalID + 4,pSysCmd->TerminalCode,6) == 0)
		return;
	
	//SL_Print("Run this is......2\r\n");
	//*/
	//SL_Print("Run This is......\r\n");			
	if(pSysCfg->CfgFlag == 0x55)             		//
	{
		//SL_Print("The Cmd ID is......%d \r\n",pSysCmd->CmdID);
		switch (pSysCmd->CmdID)                     //Cmd
		{ 
			case 0x01:     //
				SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
				break;
			case 0x02:     //  
				p = GprsRecvBuf + sizeof(SysCmdStr);
				tmp = *p;
				switch(tmp)
				{
					case 0x07:
						p++;
						CmdID = (*p - 2);
						if(CmdID > GprsRecvLen - 30)
						{
							SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
							//暂时不返回。
						}
						p++;
						SL_Memset(pSysCfg->ServerAddr,'\0',sizeof(pSysCfg->ServerAddr));
						SL_Memcpy(pSysCfg->ServerAddr,p,CmdID);
						p += CmdID;
						TmpData.TTbyte[0] = *(p + 0);             //
						TmpData.TTbyte[1] = *(p + 1);			  //
						pSysCfg->ServerPort = TmpData.IntII;      //
						SL_Print("Recv IP Addr........%s,%d\r\n",pSysCfg->ServerAddr,pSysCfg->ServerPort);
						SaveSysCfg(); 							 //
						TcpIpSocket.ResetLink = 20;              //
						
						SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
						break;
					case 0x06:                   	//
						p += 2;
						tmp = IpAddrToStr(Buf,*p,*(p + 1),*(p + 2),*(p + 3));   //
						if(tmp > 0)
							SL_Memcpy(pSysCfg->ServerAddr,Buf,50);  //
						else
							break;
						TmpData.TTbyte[0] = *(p + 4);             //
						TmpData.TTbyte[1] = *(p + 5);			  //
						pSysCfg->ServerPort = TmpData.IntII;      //
						SetArgID = 6;
						
						SaveSysCfg(); 							 //
						TcpIpSocket.ResetLink = 20;              //
						
						SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
						break;
					case 0x15:    //
						p += 2;
						pLockCar = GetLockCarSpace();      			//
						if(*p == 1)
						{
							pLockCar->MonCmdByte = 1;
							pLockCar->MonCmdBig = 1;   					//
							//pSysCfg->ConLockCar = 0;					//
							pLockCar->MonCurrentState = 1;
							
						}
						else if(*p == 0)
						{
							pLockCar->MonCmdByte = 2;
							pLockCar->MonCmdBig = 1;
							//pSysCfg->ConLockCar = 1;   //
							pLockCar->MonCurrentState = 1;
						}
						
						SetArgID = 0x15;
						SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
						break;
					default:                                     //
						break; 
				}
				break;
			case 0x03:     // 
				pLockCar = GetLockCarSpace();      			//
				p = GprsRecvBuf + sizeof(SysCmdStr);		//
				TmpData.TTbyte[0] = *(p + 0);          		//
				TmpData.TTbyte[1] = *(p + 1);   			//

				if(TmpData.TTbyte[0] > 0 && TmpData.TTbyte[1] > 0)   //
				{
					pLockCar->LockCarCmd = 1;        		//
					pLockCar->LockCurrentState = 1;            //
					pLockCar->LockCmdBig = 1; 		 		//
					pLockCar->LockCarRank = TmpData.TTbyte[1];
				}
				else if(TmpData.TTbyte[1] == 0)				//
				{
					pLockCar->LockCarCmd = 2;  				//
					pLockCar->LockCurrentState = 1;
					pLockCar->LockCarRank = 0;
					pLockCar->LockCmdBig = 1;
					
				}
				
				SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
				//SL_Print("Lock Car Cmd:%d,%d\r\n",pSysCmd->CmdID,ComCmdRes.CmdID);
				break;
			case 0x06:     //
				p = GprsRecvBuf + sizeof(SysCmdStr);
				tmp = *p;
				GprsRecvLen -= sizeof(SysCmdStr);
				StrLen = GetComma(1,p + 1,GprsRecvLen);      //
				if(StrLen > 0)
				{
					SL_Memcpy(TcpIpSocket.FtpAddr,p + 1,StrLen - 1);     		// 
					SL_Print("FTP Server Addr:%s\r\n",TcpIpSocket.FtpAddr);     //
				}
							
				SL_Memset(Buf,'\0',20); 
				if(GetDataStr(1,2,p,Buf,GprsRecvLen) > 0)        //
				{
					TcpIpSocket.FtpPort = StrToDouble(Buf);
					SL_Print("FTP Server Port:%d\r\n",TcpIpSocket.FtpPort);     //
				}
							
				SL_Memset(Buf,'\0',20); 
				if(GetDataStr(2,3,p,Buf,GprsRecvLen) > 0)       //
				{
					SL_Memcpy(TcpIpSocket.FtpUserName,Buf,20);
					SL_Print("FTP UserName:%s\r\n",TcpIpSocket.FtpUserName);	 //
				}
						
				SL_Memset(Buf,'\0',20); 
				if(GetDataStr(3,4,p,Buf,GprsRecvLen) > 0)       //
				{
					SL_Memcpy(TcpIpSocket.FtpUserPassd,Buf,20);
					SL_Print("FTP UserPassd:%s\r\n",TcpIpSocket.FtpUserPassd);  //
				}

				SL_Memset(Buf,'\0',20); 
				if(GetDataStr(4,5,p,Buf,GprsRecvLen) > 0)       //
				{
					SL_Memcpy(TcpIpSocket.FtpFileName,Buf,20);
					SL_Print("File Name:%s\r\n",TcpIpSocket.FtpFileName);
				}
				pUpgrade = GetUpgradeDataSpace();
				
				if(tmp == 0)      //
				{
					TcpIpSocket.LinkType = 1;         //
					TcpIpSocket.ResetLink = 20;       //
					pUpgrade->UpgradeFlag = 0;		  //  
					//SL_Print("UpDate ST86 Finsh..........\r\n");
					SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
				}
				else if(tmp == 1)    //
				{
					TcpIpSocket.LinkType = 1;         //
					TcpIpSocket.ResetLink = 20;       //
					pUpgrade->UpgradeFlag = 1;        //
					//SL_Print("UpDate STM32 Finsh..........\r\n");
					SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
				}
				
				break;
			case 0x07:     //    
				p = GprsRecvBuf + sizeof(SysCmdStr);
				//
				SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
				SL_Print("Recv Loader Display String Cmd!\r\n");     //
				break;
			case 0x09:     //
				p = GprsRecvBuf + sizeof(SysCmdStr);
				TmpData.TTbyte[0] = *p;
				TmpData.TTbyte[1] = *(p + 1);        //
				if(TmpData.IntII == 0xC50A)
				{
					
					pLockCar = GetLockCarSpace();
					p += 2;
					pLockCar->GpsID[0] = *p;
					pLockCar->GpsID[1] = *(p + 1);
					pLockCar->GpsID[2] = *(p + 2);
					p += 3;
					pLockCar->Secret[0] = *p;
					pLockCar->Secret[1] = *(p + 1);
					pLockCar->Secret[2] = *(p + 2);

					pLockCar->MonCmdByte = 1;
					pLockCar->MonCmdBig = 1;   					//
					//pSysCfg->ConLockCar = 0;					// 
					pLockCar->MonCurrentState = 1;
					SL_Print("Recv Functional activation 0x0AC5 Cmd:%x,%x,%x,%x,%x,%x\r\n",pLockCar->GpsID[0],pLockCar->GpsID[1],pLockCar->GpsID[2],pLockCar->Secret[0],pLockCar->Secret[1],pLockCar->Secret[2]);   //������Ϣ
				}
				else if(TmpData.IntII == 0x6CEB)
				{	
					pLockCar = GetLockCarSpace();
					p += 2;
					pLockCar->GpsID[0] = *p;
					pLockCar->GpsID[1] = *(p + 1);
					pLockCar->GpsID[2] = *(p + 2);
					p += 3;
					pLockCar->Secret[0] = *p;
					pLockCar->Secret[1] = *(p + 1);
					pLockCar->Secret[2] = *(p + 2);

					pLockCar->MonCmdByte = 2;
					pLockCar->MonCmdBig = 1;	 //
					//pSysCfg->ConLockCar = 1;   // 
					pLockCar->MonCurrentState = 1;
					SL_Print("Recv Functional activation 0x6CEB Cmd:%x,%x,%x,%x,%x,%x\r\n",pLockCar->GpsID[0],pLockCar->GpsID[1],pLockCar->GpsID[2],pLockCar->Secret[0],pLockCar->Secret[1],pLockCar->Secret[2]);   //������Ϣ 
				}

				SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
				
				break;    
			case 0x0A:     //
				p = GprsRecvBuf + sizeof(SysCmdStr);
				SetArgID = *(p + 2);
				SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
				SL_Print("Recv History of the fault Cmd!\r\n");   //
				break;
			case 0x0C:		//
				p = GprsRecvBuf + sizeof(SysCmdStr);
				if(*p == 0)
				{
					pLockCar = GetLockCarSpace();   //
					//SL_Print("Run wang wei wei  This is......%d \r\n",pSysCmd->CmdID);
					pLockCar->ReplaceDevBig = 1;    //发送命令
				}
					
				SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
				break;
			case 0x08:      //
				p = GprsRecvBuf + sizeof(SysCmdStr);
				SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
				SL_Print("Recv Version information Cmd!\r\n");   //
				break;
			default:
				break;
		}
	}
}



/******************************************
**	函数名称:void ProcessCmd(void)
**	功能描述:命令返回
*******************************************/
void ProcessCmd(void)
{	
	static unsigned char 		Counter;
	unsigned char 				Buf[50];
	unsigned short int 			tmp;
	IntToChar 					TmpData16;
	LockCarStr 					*pLockCar;
	//CanDataStr					*pCanData;         //
	
	switch(ComCmdRes.CmdID)
	{
		case 0x00:        
			break;
		case 0x01:					//	
			SL_Print("Recv Call Info Cmd.........\r\n");   //	
			SL_Memset(GprsRecvBuf,0,sizeof(GprsRecvBuf));
			tmp = BuildCompletePacket(GprsRecvBuf,1);
			WriteCmdDataBuf(GprsRecvBuf,tmp);                 //	
			//SL_MEMBLOCK(Buf,tmp,16);	
			//	
			tmp = 0;
			SL_Memset(Buf,0,sizeof(Buf));
			tmp += sizeof(ComCmdRes);           //	
			*(Buf + tmp) = 0x01;				//	
			tmp++;
			ComCmdRes.MsgLen = tmp - 6;
			ComCmdRes.DataLen = tmp - 9;
			SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));  //	
			TmpData16.TTbyte[0] = VerfySum(Buf,tmp);								  // 
			TmpData16.IntII = (unsigned short int)(32 * PowTow((double)TmpData16.TTbyte[0]) + 23 * TmpData16.TTbyte[0] + 71);  //
			Buf[tmp++] = TmpData16.TTbyte[0];
			Buf[tmp++] = TmpData16.TTbyte[1];
			Buf[tmp++] = 0x0D;
			//SL_MEMBLOCK(Buf,tmp,16);	
			WriteCmdDataBuf(Buf,tmp);
			ResetHeatSw();                             //
			SL_Memset((unsigned char *)&ComCmdRes,0,sizeof(ComCmdRes));   //
			break;
		case 2:					//
			switch(SetArgID)
			{
				case 0x15:     //
					pLockCar = GetLockCarSpace();   //
					Counter++;
					SL_Print("Recv Mon Cmd.........%d,%d,%d\r\n",Counter,pLockCar->MonCurrentState,pLockCar->MonCmdBig);
					if((Counter < 35 && pLockCar->MonCurrentState == 1) || pLockCar->MonCmdBig == 1 || Counter < 5)
						break;
					if(Counter < 35 && pLockCar->MonCurrentState == 0)
					{
						tmp = 0;
						SL_Memset(Buf,0,sizeof(Buf));
						tmp += sizeof(ComCmdRes);           //
						*(Buf + tmp) = SetArgID;				//
						tmp++;
						*(Buf + tmp) = 0x01;				//
						tmp++;
						ComCmdRes.MsgLen = tmp - 6;
						ComCmdRes.DataLen = tmp - 9;
						SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));  // 
						TmpData16.TTbyte[0] = VerfySum(Buf,tmp);								  // 
						TmpData16.IntII = (unsigned short int)(32 * PowTow((double)TmpData16.TTbyte[0]) + 23 * TmpData16.TTbyte[0] + 71);  //����У��ֵ
						Buf[tmp++] = TmpData16.TTbyte[0];
						Buf[tmp++] = TmpData16.TTbyte[1];
						Buf[tmp++] = 0x0D;
						//SL_MEMBLOCK(Buf,tmp,16);	
						WriteCmdDataBuf(Buf,tmp);
						ResetHeatSw();                             //
						SL_Memset((unsigned char *)&ComCmdRes,0,sizeof(ComCmdRes));   //
						
					}
					else
					{
						tmp = 0;
						SL_Memset(Buf,0,sizeof(Buf));
						tmp += sizeof(ComCmdRes);           //
						*(Buf + tmp) = SetArgID;				//
						tmp++;
						*(Buf + tmp) = 0x00;				//
						tmp++;
						ComCmdRes.MsgLen = tmp - 6;
						ComCmdRes.DataLen = tmp - 9;
						SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));  //
						TmpData16.TTbyte[0] = VerfySum(Buf,tmp);								  // 
						TmpData16.IntII = (unsigned short int)(32 * PowTow((double)TmpData16.TTbyte[0]) + 23 * TmpData16.TTbyte[0] + 71);  //
						Buf[tmp++] = TmpData16.TTbyte[0];
						Buf[tmp++] = TmpData16.TTbyte[1];
						Buf[tmp++] = 0x0D;
						//SL_MEMBLOCK(Buf,tmp,16);	
						WriteCmdDataBuf(Buf,tmp);
						ResetHeatSw();                             //
						SL_Memset((unsigned char *)&ComCmdRes,0,sizeof(ComCmdRes));   //
					}
					Counter = 0 ;
					break;
				default:
					tmp = 0;
					SL_Memset(Buf,0,sizeof(Buf));
					tmp += sizeof(ComCmdRes);           //
					*(Buf + tmp) = SetArgID;				//
					tmp++;
					*(Buf + tmp) = 0x01;				//
					tmp++;
					ComCmdRes.MsgLen = tmp - 6;
					ComCmdRes.DataLen = tmp - 9;
					SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));  // 
					TmpData16.TTbyte[0] = VerfySum(Buf,tmp);								  // 
					TmpData16.IntII = (unsigned short int)(32 * PowTow((double)TmpData16.TTbyte[0]) + 23 * TmpData16.TTbyte[0] + 71);  //
					Buf[tmp++] = TmpData16.TTbyte[0];
					Buf[tmp++] = TmpData16.TTbyte[1];
					Buf[tmp++] = 0x0D;
					//SL_MEMBLOCK(Buf,tmp,16);	
					WriteCmdDataBuf(Buf,tmp);
					ResetHeatSw();                             //
					SL_Memset((unsigned char *)&ComCmdRes,0,sizeof(ComCmdRes));   //
					break;
			}
			break;
		case 0x06:		//
			//SL_Print("Recv Update Sys Cmd..........\r\n");   //
			tmp = 0;
			SL_Memset(Buf,0,sizeof(Buf));
			tmp += sizeof(ComCmdRes);           //
			*(Buf + tmp) = 0x01;				//
			tmp++;
			ComCmdRes.MsgLen = tmp - 6;
			ComCmdRes.DataLen = tmp - 9;
			SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));  //
			TmpData16.TTbyte[0] = VerfySum(Buf,tmp);								  //
			TmpData16.IntII = (unsigned short int)(32 * PowTow((double)TmpData16.TTbyte[0]) + 23 * TmpData16.TTbyte[0] + 71);  //
			Buf[tmp++] = TmpData16.TTbyte[0];
			Buf[tmp++] = TmpData16.TTbyte[1];
			Buf[tmp++] = 0x0D;
			//SL_MEMBLOCK(Buf,tmp,16);	
			WriteCmdDataBuf(Buf,tmp);
			ResetHeatSw();						//
			SL_Memset((unsigned char *)&ComCmdRes,0,sizeof(ComCmdRes));   //
			break;
		case 0x03:								//
			pLockCar = GetLockCarSpace();   //
			Counter++;
			SL_Print("Recv Lock Car Cmd.........%d,%d\r\n",Counter,pLockCar->LockCurrentState);
			if((Counter < 35 && pLockCar->LockCurrentState == 1) || pLockCar->LockCmdBig == 1 || Counter < 10)
				break;
			
			tmp = 0;
			SL_Memset(Buf,0,sizeof(Buf));
			tmp += sizeof(ComCmdRes);           //
			//SL_Print("Run This Is....%d,%d,%d\r\n",Counter,pLockCar->LockCardState,pLockCar->LockCurrentState);
			if(Counter < 35 && pLockCar->LockCurrentState == 0)
			{
				if(pLockCar->LockCarCmd == 1)   // 
				{
					*(Buf + tmp) = 0x01;	// 
					tmp++;
					*(Buf + tmp) = pLockCar->LockCarRank;   			// 
					tmp++;
				}
				else if(pLockCar->LockCarCmd == 2)  // 
				{
					*(Buf + tmp) = 0x01;			//
					tmp++;
					*(Buf + tmp) = 0x00;   			// 
					tmp++;
				}
				//SL_Print("Lcok OK :%d\r\n",pLockCar->LockCurrentState);
			}
			else                     // 
			{
				if(pLockCar->LockCarCmd == 1)   // 
				{
					*(Buf + tmp) = 0x01;	// 
					tmp++;
					*(Buf + tmp) = pLockCar->LockCarRank + 0x10;   			// 
					tmp++;
				}
				else if(pLockCar->LockCarCmd == 2)  // 解锁 
				{
					*(Buf + tmp) = 0x01;			//
					tmp++;
					*(Buf + tmp) = 0x10;   			// 
					tmp++;
				}
				SL_Print("Lcok Error :%d\r\n",pLockCar->LockCurrentState);
			}
			Counter = 0;
			//SL_Print("Run This Is....%d,%d,%d,%d\r\n",Counter,pLockCar->LockCarCmd,pLockCar->LockCardState,pLockCar->LockCurrentState);
			ComCmdRes.MsgLen = tmp - 6;
			ComCmdRes.DataLen = tmp - 9;
			SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));  // 
			TmpData16.TTbyte[0] = VerfySum(Buf,tmp);								  // 
			TmpData16.IntII = (unsigned short int)(32 * PowTow((double)TmpData16.TTbyte[0]) + 23 * TmpData16.TTbyte[0] + 71);  // 
			Buf[tmp++] = TmpData16.TTbyte[0];
			Buf[tmp++] = TmpData16.TTbyte[1];
			Buf[tmp++] = 0x0D;
			//SL_MEMBLOCK(Buf,tmp,16);	
			WriteCmdDataBuf(Buf,tmp);
			ResetHeatSw();                  							  // 
			ResetNextTime();  //
			SL_Memset((unsigned char *)&ComCmdRes,0,sizeof(ComCmdRes));   //
			break;
		case 0x07:     //
			tmp = 0;
			SL_Memset(Buf,0,sizeof(Buf));
			tmp += sizeof(ComCmdRes);           //
			SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));  //
			*(Buf + tmp) = 0x01;				//
			tmp++;
			ComCmdRes.MsgLen = tmp - 6;
			ComCmdRes.DataLen = tmp - 9;
			TmpData16.TTbyte[0] = VerfySum(Buf,tmp);								  //
			TmpData16.IntII = (unsigned short int)(32 * PowTow((double)TmpData16.TTbyte[0]) + 23 * TmpData16.TTbyte[0] + 71);  //
			Buf[tmp++] = TmpData16.TTbyte[0];
			Buf[tmp++] = TmpData16.TTbyte[1];
			Buf[tmp++] = 0x0D;
			WriteCmdDataBuf(Buf,tmp);
			ResetHeatSw();                     							  //
			SL_Memset((unsigned char *)&ComCmdRes,0,sizeof(ComCmdRes));   //
			break;
		case 0x09:     // 
			pLockCar = GetLockCarSpace();   //
			Counter++;
			SL_Print("Recv Mon Cmd.........%d,%d,%d\r\n",Counter,pLockCar->MonCurrentState,pLockCar->MonCmdBig);
			if((Counter < 35 && pLockCar->MonCurrentState == 1) || pLockCar->MonCmdBig == 1 || Counter < 10)
				break;
			if(Counter < 35 && pLockCar->MonCurrentState == 0)
			{
				tmp = 0;
				SL_Memset(Buf,0,sizeof(Buf));
				tmp += sizeof(ComCmdRes);           //
				*(Buf + tmp) = 0x00;				//
				tmp++;
				ComCmdRes.MsgLen = tmp - 6;
				ComCmdRes.DataLen = tmp - 9;
				SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));  //
				TmpData16.TTbyte[0] = VerfySum(Buf,tmp);								  //
				TmpData16.IntII = (unsigned short int)(32 * PowTow((double)TmpData16.TTbyte[0]) + 23 * TmpData16.TTbyte[0] + 71);  //
				Buf[tmp++] = TmpData16.TTbyte[0];
				Buf[tmp++] = TmpData16.TTbyte[1];
				Buf[tmp++] = 0x0D;
				SL_MEMBLOCK(Buf,tmp,16);	
				WriteCmdDataBuf(Buf,tmp);
				ResetHeatSw();                             //
				SL_Memset((unsigned char *)&ComCmdRes,0,sizeof(ComCmdRes));   //
						
			}
			else
			{
				tmp = 0;
				SL_Memset(Buf,0,sizeof(Buf));
				tmp += sizeof(ComCmdRes);           //
				*(Buf + tmp) = 0x01;				//
				tmp++;
				ComCmdRes.MsgLen = tmp - 6;
				ComCmdRes.DataLen = tmp - 9;
				SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));  //
				TmpData16.TTbyte[0] = VerfySum(Buf,tmp);				       // 
				TmpData16.IntII = (unsigned short int)(32 * PowTow((double)TmpData16.TTbyte[0]) + 23 * TmpData16.TTbyte[0] + 71);  //
				Buf[tmp++] = TmpData16.TTbyte[0];
				Buf[tmp++] = TmpData16.TTbyte[1];
				Buf[tmp++] = 0x0D;
				SL_MEMBLOCK(Buf,tmp,16);	
				WriteCmdDataBuf(Buf,tmp);
				ResetHeatSw();                             //
				SL_Memset((unsigned char *)&ComCmdRes,0,sizeof(ComCmdRes));   //
			}
			Counter = 0 ;
			break;
			
		case 0x0A:     //
			tmp = 0;
			SL_Memset(Buf,0,sizeof(Buf));
			tmp += sizeof(ComCmdRes);           //
			SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));  //
			*((unsigned short int *)(Buf + tmp)) = 3;    //
			tmp += 2;
			*(Buf + tmp) = SetArgID;       //
			tmp++;
			
			if(SetArgID == 0)
				*((unsigned short int *)(Buf + tmp)) = 0x1234;   //
			else if(SetArgID == 1)
				*((unsigned short int *)(Buf + tmp)) = 0x1234;   //
			else
				*((unsigned short int *)(Buf + tmp)) = 0;    //
				
			tmp += 2;
			ComCmdRes.MsgLen = tmp - 6;
			ComCmdRes.DataLen = tmp - 9;
			TmpData16.TTbyte[0] = VerfySum(Buf,tmp);								  //  
			TmpData16.IntII = (unsigned short int)(32 * PowTow((double)TmpData16.TTbyte[0]) + 23 * TmpData16.TTbyte[0] + 71);  //
			Buf[tmp++] = TmpData16.TTbyte[0];
			Buf[tmp++] = TmpData16.TTbyte[1];
			Buf[tmp++] = 0x0D;
			WriteCmdDataBuf(Buf,tmp);
			ResetHeatSw();                  							  //
			SL_Memset((unsigned char *)&ComCmdRes,0,sizeof(ComCmdRes));   //
			break;
		case 0x0B:		//
			tmp = 0;
			SL_Memset(Buf,0,sizeof(Buf));
			tmp += sizeof(ComCmdRes);           // 
			SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));  //
			*((unsigned short int *)(Buf + tmp)) = 9;    //
			tmp += 2;
			*(Buf + tmp) = SetArgID;       //
			tmp++;
			switch(SetArgID)
				{
					case 0x00:      //
						SL_Memset((Buf + tmp),0,8);     
						break;
					case 0x01:		//
						SL_Memset((Buf + tmp),0,8);      //
						break;
					case 0x02:		//
						SL_Memset((Buf + tmp),0,8);      //
						break;
					case 0x03:		//
						SL_Memset((Buf + tmp),0,8);      //
						break;
					case 0x04:		//
						SL_Memset((Buf + tmp),0,8);      //
						break;
					case 0x05:		//
						SL_Memset((Buf + tmp),0,8);      //
						break;
					case 0x06:		//
						SL_Memset((Buf + tmp),0,8);      //
						break;	
					default:
						break;
				}
			//
			
			tmp += 8;
			ComCmdRes.MsgLen = tmp - 6;         
			ComCmdRes.DataLen = tmp - 9;
			TmpData16.TTbyte[0] = VerfySum(Buf,tmp);								  // 
			TmpData16.IntII = (unsigned short int)(32 * PowTow((double)TmpData16.TTbyte[0]) + 23 * TmpData16.TTbyte[0] + 71);  //
			Buf[tmp++] = TmpData16.TTbyte[0];
			Buf[tmp++] = TmpData16.TTbyte[1];
			Buf[tmp++] = 0x0D;
			WriteCmdDataBuf(Buf,tmp);              //
			ResetHeatSw();                  							  //
			SL_Memset((unsigned char *)&ComCmdRes,0,sizeof(ComCmdRes));   //
			break;
		case 0x08:     //
			tmp = 0;
			SL_Memset(Buf,0,sizeof(Buf));
			tmp += sizeof(ComCmdRes);           //
			SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));  //
			*(Buf + tmp) = 0x01;				//
			tmp++;
			ComCmdRes.MsgLen = tmp - 6;
			ComCmdRes.DataLen = tmp - 9;
			TmpData16.TTbyte[0] = VerfySum(Buf,tmp);								  // 
			TmpData16.IntII = (unsigned short int)(32 * PowTow((double)TmpData16.TTbyte[0]) + 23 * TmpData16.TTbyte[0] + 71);  //
			Buf[tmp++] = TmpData16.TTbyte[0];
			Buf[tmp++] = TmpData16.TTbyte[1];
			Buf[tmp++] = 0x0D;
			WriteCmdDataBuf(Buf,tmp);
			ResetHeatSw();                     							  //
			SL_Memset((unsigned char *)&ComCmdRes,0,sizeof(ComCmdRes));   //
			break;
		case 0x0C:
			pLockCar = GetLockCarSpace();   //
			Counter++;
			SL_Print("Recv ReplaceDev Cmd.........%d,%d,%d\r\n",Counter,pLockCar->ReplaceDevState,pLockCar->ReplaceDevBig);
			if((Counter < 35 && pLockCar->ReplaceDevState == 1) || pLockCar->ReplaceDevBig == 1 || Counter < 10)
				break;
			
			if(Counter < 35 && pLockCar->ReplaceDevState == 0)
			{
				tmp = 0;
				SL_Memset(Buf,0,sizeof(Buf));
				tmp += sizeof(ComCmdRes);           //
				*(Buf + tmp) = 0x00;				//
				tmp++;
				ComCmdRes.MsgLen = tmp - 6;
				ComCmdRes.DataLen = tmp - 9;
				SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));  //
				TmpData16.TTbyte[0] = VerfySum(Buf,tmp);								  //
				TmpData16.IntII = (unsigned short int)(32 * PowTow((double)TmpData16.TTbyte[0]) + 23 * TmpData16.TTbyte[0] + 71);  //
				Buf[tmp++] = TmpData16.TTbyte[0];
				Buf[tmp++] = TmpData16.TTbyte[1];
				Buf[tmp++] = 0x0D;
				SL_MEMBLOCK(Buf,tmp,16);	
				WriteCmdDataBuf(Buf,tmp);
				ResetHeatSw();                             //
				SL_Memset((unsigned char *)&ComCmdRes,0,sizeof(ComCmdRes));   //
						
			}
			else
			{
				tmp = 0;
				SL_Memset(Buf,0,sizeof(Buf));
				tmp += sizeof(ComCmdRes);           //
				*(Buf + tmp) = 0x10;				//
				tmp++;
				ComCmdRes.MsgLen = tmp - 6;
				ComCmdRes.DataLen = tmp - 9;
				SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));  //
				TmpData16.TTbyte[0] = VerfySum(Buf,tmp);				       // 
				TmpData16.IntII = (unsigned short int)(32 * PowTow((double)TmpData16.TTbyte[0]) + 23 * TmpData16.TTbyte[0] + 71);  //
				Buf[tmp++] = TmpData16.TTbyte[0];
				Buf[tmp++] = TmpData16.TTbyte[1];
				Buf[tmp++] = 0x0D;
				SL_MEMBLOCK(Buf,tmp,16);	
				WriteCmdDataBuf(Buf,tmp);
				ResetHeatSw();                             //
				SL_Memset((unsigned char *)&ComCmdRes,0,sizeof(ComCmdRes));   //
			}
			Counter = 0 ;
			break;
		default:
			SL_Print("Recv Gprs Data Fault............\r\n");   //
			SL_Memset((unsigned char *)&ComCmdRes,0,sizeof(ComCmdRes));   //
			break;
	}
}





/**************************************************************
**	:void AppSmsRcvCb(S32 slResult, U8 ucIndex, U8* pucbuf, U16 usLenth, U8* pucTelNumber)
******************************************************************/
void AppSmsRcvCb(S32 slResult, U8 ucIndex, U8* pucbuf, U16 usLenth, U8* pucTelNumber)
{
	SL_TASK stSltask;
	
	if(slResult != SL_RET_OK)
	{
		return;
	}
	if(SL_SmsGetSmsFormat())
	{
	
		SL_Memset(SMSRecvBuf, 0, 100);		 					// 			
		SL_Memcpy(SMSRecvBuf, pucbuf, usLenth);					// 
	
		SL_Memset(SMSRecvNum, 0, 20); 							// 
		SL_Memcpy(SMSRecvNum, pucTelNumber, SL_Strlen(pucTelNumber));
		stSltask.element[0] = SL_GetAppTaskHandle();
		SL_AppSendMsg(stSltask.element[0], EVT_APP_GSM_RECV, 0);			//
	
	}
}


/****************************************
**	��������:
**	��������:
****************************************/
void AppSmsFullCb(S32 slResult)
{
    //SL_Print("SLAPP: SL_AppSmsFullCb result[%d].", slResult);
    SL_SmsDeleteSms(1, SL_SMS_DEL_ALL);
}




/***************************************
**	��������:void InitSms(void)
**	��������:��ʼ�����Ź���
***************************************/

void InitSms(void)
{
	SL_SmsSetSmsFormat(SL_SMS_FMT_TXT);
    SL_SmsSetStorage(SL_SMS_STORAGE_SIM, SL_SMS_STORAGE_SIM, SL_SMS_STORAGE_SIM);
    SL_SmsSetTxtModeDataCodSch(8);
	
    SL_SmsSetNewSmsCallback(AppSmsRcvCb);
    SL_SmsSetSmsFullCallback(AppSmsFullCb);
}


/******************************
**	��������:void ProcessSMSRecv(void)
**	��������:
**	����Զ����������
*******************************/

void ProcessSMSRecv(void)
{
	UpgradeStatusStr 			*pUpgrade;
	SysConfigStr 				*pSysCfg;
	unsigned char 				*p;
	int			 				tmp;
	unsigned char 				Buf[20];
	
	//SL_Print("Recv SMS Info:%d\r\n",SMSRecvLen);    //������Ϣ
	SL_SmsDeleteSms(1,SL_SMS_DEL_INDEX);			  //ɾ�����յ��Ķ���Ϣ

	
	//SL_MEMBLOCK(SMSRecvBuf,SMSRecvLen,16);      //���������Ϣ

	tmp = LookForStr(SMSRecvBuf,"FTPUPD",SMSRecvLen);   //��ѯ
	
	//SL_Print("Recv SMS Info:%d,%c\r\n",tmp,*(SMSRecvBuf + tmp + 6));    //������Ϣ
	if(tmp >= 0)    //��ѯ��Ϣͷ
	{
		p = SMSRecvBuf + tmp + 6;
		if(*p != ':')
			return;
		SMSRecvLen = SMSRecvLen - tmp - 6;
		p++;
	}
	else
	{
		p = SMSRecvBuf + 11;
		pSysCfg = GetSysCfgSpace();   //

		//��ʼȥIP��ַ
		
		
		return;
	}


	tmp = GetComma(1,p,SMSRecvLen);      //�ҵ���һ������
	SL_Memcpy((unsigned char *)&TcpIpSocket.FtpAddr,p,tmp - 1);    //����FTP��ַ  
	SL_Print(">FTP Server Addr:%s\r\n",TcpIpSocket.FtpAddr);

	SL_Memset(Buf,0,20);
	if(GetDataStr(1,2,p,Buf,SMSRecvLen) > 0)       		   //�˿ں� 
	{
		TcpIpSocket.FtpPort = StrToDouble(Buf);
		SL_Print(">FTP Server Port:%d\r\n",TcpIpSocket.FtpPort);     //����FTP�˿�
	}
	else
	{
		return;
	}

	SL_Memset(Buf,0,20);
	if(GetDataStr(2,3,p,Buf,SMSRecvLen) > 0)     		   //FTP�û���
	{
		SL_Memcpy((unsigned char *)&TcpIpSocket.FtpUserName,Buf,20);
		SL_Print(">FTP User Name:%s\r\n",TcpIpSocket.FtpUserName);   //FTP�û�����:
	}
	else
	{
		return;
	}

	SL_Memset(Buf,0,20);
	if(GetDataStr(3,4,p,Buf,SMSRecvLen) > 0)          //FTP����
	{
		SL_Memcpy((unsigned char *)&TcpIpSocket.FtpUserPassd,Buf,20);
		SL_Print(">FTP User PassdWord:%s\r\n",TcpIpSocket.FtpUserPassd); //FTP�û�����
	}
	else
	{
		return;
	}

	SL_Memset(Buf,0,20);
	if(GetDataStr(4,5,p,Buf,SMSRecvLen) > 0)
	{
		SL_Memcpy((unsigned char *)&TcpIpSocket.FtpFileName,Buf,20);
		SL_Print(">FTP File Name:%s\r\n",TcpIpSocket.FtpFileName);    //FTP�ļ�����
	}
	else
	{
		return;
	}

	SL_Memset(Buf,0,20);
	if(GetDataStr(5,6,p,Buf,SMSRecvLen) > 0)
	{
		if(Buf[0] == 'S')
		{
			pUpgrade = GetUpgradeDataSpace();
			pUpgrade->UpgradeFlag = 0;		  //����ST86/ST87	 
			TcpIpSocket.LinkType = 1;		  //FTP����
			TcpIpSocket.ResetLink = 20; 	  //10��֮����������  
			SL_Print("Start UpGrade_ST86....\r\n");
			SL_Memset(SMSRecvBuf,0,sizeof(SMSRecvBuf));
			return;
		}
		else if(Buf[0] == 'T')
		{
			pUpgrade = GetUpgradeDataSpace();
			pUpgrade->UpgradeFlag = 1;		  //STM32   
			TcpIpSocket.LinkType = 1;		  //FTP����
			TcpIpSocket.ResetLink = 20; 	  //10��֮����������  
			SL_Print("Start UpGrade_STM32....\r\n");
			SL_Memset(SMSRecvBuf,0,sizeof(SMSRecvBuf));
		}
		//���һλ�����ַ�"S"����"T",���Ը�������
	}
	else
	{
		return;
	}
		
    //һ����Ϊ������Ϣ  
}





/******************************File End ***************************/









