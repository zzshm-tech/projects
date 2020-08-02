
/***********************************************
**	File Name:Gprs.c
**	Time:
************************************************/

#include "include.h"


/*********默认预定义***************/

//#define DEF_SERVER_ADDR "219.236.247.110"
//#define DEF_SERVER_PORT 13011

//#define DEF_SERVER_ADDR "211.103.179.234"//
//#define DEF_SERVER_PORT 13011


/***********本地宏定义************/

#define DEF_SERVER_ADDR "was.ym.bcnyyun.com"      
#define DEF_SERVER_PORT 26000




extern HANDLE SysAppEnterTask;											//主任务句柄


/**********************本地全局变量****************************/
static TcpIPSocketStr 					TcpIpSocket;                  	//

static IpAddrBackStr  					IpAddrBack;                   	//Host IP back up

static unsigned char 					GprsRecvBuf[100];           	//Gprs  		

static unsigned char 					SMSRecvBuf[200];				//
		
static unsigned char 					SMSRecvNum[20];					//

static unsigned short int 				GprsRecvLen;  					//			 

static unsigned short int 				SMSRecvLen;						//

static SysCmdStr						ComCmdRes;						//

static unsigned short int 				CmdID;							//

/************************************************
**	函数名称:
**	功能描述:
*************************************************/

TcpIPSocketStr *GetTcpIpSocketSpce(void)
{
	return &TcpIpSocket;
}



/*************************************************
**	函数名称:
**	功能描述:
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
**	函数名称:
**	功能描述:
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
**	函数名称:
**	功能描述:
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
**	函数名称:
**	功能描述:  
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
		SL_Print(">App:Send Data Fail.........%d\r\n",slErrorCode);
		TcpIpSocket.SendState = 3;
	}
}


/*************************************************
**	函数名称:
**	功能描述:
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
**	函数名称:
**	功能描述:
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
**	函数名称:
**	功能描述:
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
	
	
    SL_TcpipGprsNetInit(0, &stSlTcpipCb);         //  
}





/*******************************************
**	函数名称:
**	功能描述:
*******************************************/

void AppFtpOpenCB(S32 openst)
{
	SL_Print("The App Ftp Open %d\r\n",openst);
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
**	函数名称:
**	功能描述:
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
**	函数名称:
**	功能描述:
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
**	函数名称:
**	功能描述:
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
**	函数名称:
**	功能描述:
**********************************************/

static unsigned char AppFtpOpen(void)
{
    SL_FTP_Connect_Info ftp_info;
    int ret = 0;
	
    ftp_info.u16DestPort = TcpIpSocket.FtpPort;
	
    SL_Strcpy(ftp_info.uaUrl, (const char *)TcpIpSocket.FtpAddr); 			 	//
    SL_Strcpy(ftp_info.uaUsername, (const char *)TcpIpSocket.FtpUserName);		//
    SL_Strcpy(ftp_info.uaPassword, (const char *)TcpIpSocket.FtpUserPassd); 		//

	//SL_Print("The ftp_inof.Addr:%s\r\n",ftp_info.uaUrl);             //
	//SL_Print("The ftp_inof.u16DestPort:%d,%d\r\n",ftp_info.u16DestPort,TcpIpSocket.FtpPort); 
	//SL_Print("The ftp_inof.uaUserName:%s\r\n",ftp_info.uaUsername);             //
	//SL_Print("The ftp_inof.uaPassword:%s\r\n",ftp_info.uaPassword);             //
    ret = SL_Ftp_Open(&ftp_info);
	if(ret == SL_RET_OK)
    	return 1;
	return 0;
	
}

/*******************************************************
**	函数名称:
**	功能描述:
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
**	函数名称:
**	功能描述:
********************************************************/

unsigned char ReadSendState(void)
{
	if(TcpIpSocket.LinkState != 2)
		return 0;
	if(TcpIpSocket.SendState == 1 )     //
		return 1;
	if(TcpIpSocket.SendState == 2 || TcpIpSocket.SendState == 0)
		return 2;
	return 3;                           //
}


/*******************************************************
**	函数名称:
**	功能描述:
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
**	函数名称:
**	功能描述:
*************************************************/

void AppGetHostIpByNameCb(U8 ucCidIndex, S32 slErrorCode, U8* pucIpAddr)
{
	
	if(slErrorCode == 0)                						//
	{
		TcpIpSocket.UlrToIPFlag = 1;//
		
		SL_Print(">App: SL_AppGetHostIpByNameCb cid[%d], result[%d]\r\n", ucCidIndex, slErrorCode);    //
			//SL_Print(">App: IP:%s\r\n", pucIpAddr); 													  //
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
**	函数名称:
**	功能描述:
*****************************************************************/

void ReadIpAddrBack(void)
{
	unsigned char Flag;
	
	Flag = FileCheckOrGreate((unsigned char *)"IpAddrBackFile",(unsigned char *)&IpAddrBack,sizeof(IpAddrBack),0);
	if(Flag > 0)
		SL_Print(">App:Read IpAddrBackFile OK.......\r\n");
}



/****************************************************************
**	函数名称:
**	功能描述:
*****************************************************************/

void SaveIpAddrBack(void)
{
	unsigned char Flag;

	Flag = FileCheckOrGreate((unsigned char *)"IpAddrBackFile",(unsigned char *)&IpAddrBack,sizeof(IpAddrBack),1);
	if(Flag > 0)
		SL_Print("Write IpAddrBackFile OK.......\r\n");
}




/*************************************************************
**	函数名称:
**	功能描述:
**	SL_NW_REG_STATUS
**	SL_NW_STATUS_NOTREGISTERED_NOTSEARCHING =  0,// Not registered and not searching. 
**  SL_NW_STATUS_REGISTERED_HOME =             1,//Registered and in the home area.   
**  SL_NW_STATUS_NOTREGISTERED_SEARCHING =     2,//Not registered and searching for a new operator. 
**  SL_NW_STATUS_REGISTRATION_DENIED =         3,//Registration denied.    
**  SL_NW_STATUS_UNKNOWN =                     4,//Unknown registration.   
**  SL_NW_STATUS_REGISTERED_ROAMING =          5,//
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
				SL_Print(">App:Register Net 1..........OK\r\n");   //  
				TcpIpSocket.LinkState = 0;                 		 //
				TcpIpSocket.GprsLinkState = 0; 					//	
				step++;
				erro_code = 0;
				break;
			}
			
			SL_Print(">App:Register Net 2............%d\r\n",TcpIpSocket.NetWorkState);   //
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


				step = 2;               					//
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
			SL_Print(">App:Link ServerAddr(%d) %s:%d\r\n",TcpIpSocket.LinkNum,TcpIpSocket.ServerAddr,TcpIpSocket.ServerPort);    //���ӵ��ķ�����
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
			slRet = SL_TcpipGetState(0);    					//  
			if(slRet == SL_TCPIP_STATE_CONNECTOK)               //
			{
				TcpIpSocket.LinkState = 2;                    // 
				TcpIpSocket.GprsLinkState = 2;				  //
				TcpIpSocket.LinkNum = 0;
				step++;
				counter = 0;
				TcpIpSocket.LinkCount = 0;
				ResetTerMsgBig();
				SL_Print(">APP:Link Server.........OK\r\n");
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
				//SL_Print("Check Net State........\r\n");
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
				if(SendCounter++ > 10)           //
				{
					step++;                   
					TcpIpSocket.LinkState = 0;
					TcpIpSocket.SendState = 0;
					SendCounter = 0;
				}
			}
			else
			{
				SendCounter = 0;               	  //
			}
			break;
		case 13:
			//SL_Print("run 13 Step ++\r\n");
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
			SL_Sleep(5000);							//
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
			if(counter++ < 240)    			//
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




/***************************************************************
**	函数名称:
**	功能描述:
***************************************************************/

void ProcessGprsRecv(void)
{
	unsigned char 				tmp;
	SysConfigStr 				*pSysCfg;
	unsigned char 				*p;
	IntToChar     				TmpData;
	LongToChar					TmpData32;
	int 						StrLen;       //
	unsigned char 				Buf[50];      //
	UpgradeStatusStr 			*pUpgrade;
	SysCmdStr					*pSysCmd;     //
	LockCarStr					*pLockCar;    //
	
	if(GprsRecvLen < 25)                    //
		return;
	//SL_MEMBLOCK(GprsRecvBuf,GprsRecvLen,16);			   //

	pSysCmd = (SysCmdStr *)GprsRecvBuf;

	   
	if(pSysCmd->FrameStart[0] != 0xF1 && pSysCmd->FrameStart[1] != 0xF2 && pSysCmd->FrameStart[2] != 0xFF)   //
		return;

	if(*(GprsRecvBuf + GprsRecvLen - 1) != 0x0d)   //
		return;
	
	tmp = BccVerify(GprsRecvBuf + 3,GprsRecvLen - 5);    //
	
	//SL_Print("Run This is.....%d,%d\r\n",tmp, GprsRecvLen);	
	tmp = *(GprsRecvBuf + GprsRecvLen - 2);
	//SL_Print("%d\r\n",tmp);
	
	if(tmp != *(GprsRecvBuf + GprsRecvLen - 2))          //
		return;

	
	if(pSysCmd->msg_id != 0x48)
		return;

	TmpData.TTbyte[0] = *(GprsRecvBuf + sizeof(SysCmdStr));   		//
	TmpData.TTbyte[1] = *(GprsRecvBuf + sizeof(SysCmdStr) + 1);  	//
	CmdID = TmpData.IntII;
	
	//SL_Print("%d\r\n",CmdID);
	
	pSysCfg = GetSysCfgSpace();                     //
				
	if(pSysCfg->CfgFlag == 0x55)             		//
	{
		switch(pSysCmd->msg_id)
		{
			case 0x89:
				break;
			case 0xff:
				break;
			case 0x02:
				break;
			case 0x47:
				break;
			case 0x48:
				switch (CmdID)				//
				{ 
					case 0x0200:     //
						SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes)); 
						tmp = *(GprsRecvBuf + 29);
						if(tmp == 0)    //
						{
							pLockCar = GetLockCarSpace();      		//
							pLockCar->LockCarCmd = 2;        		//
							pLockCar->LockCurrentState = 1;        	//
							pLockCar->LockCmdBig = 1; 		 		//
						}
						break;
					case 0x0201:     //
						SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
						tmp = *(GprsRecvBuf + 29);
						if(tmp >= 0 && tmp <= 5)                 //
						{
							pLockCar = GetLockCarSpace();      		//
							pLockCar->LockCarCmd = 1;        		//
							pLockCar->LockCurrentState = 1;         //
							pLockCar->LockCmdBig = 1; 		 		//
							pLockCar->LockCarRank = tmp;
						}
						break;
					case 0x0203:      //
						SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes)); 
						tmp = *(GprsRecvBuf + 29);
						if(tmp == 0)
						{
							pLockCar = GetLockCarSpace();      		//
							pLockCar->MonCmdByte = 1;
							pLockCar->MonCmdBig = 1;   			    //
							pLockCar->MonCurrentState = 1;
						}
						else
						{
							pLockCar = GetLockCarSpace();      		//
							pLockCar->MonCmdByte = 2;
							pLockCar->MonCmdBig = 1;
							pLockCar->MonCurrentState = 1;
						}
						break;
					case 0xFD00:     //
						p = GprsRecvBuf + sizeof(SysCmdStr) + 4;    //
						GprsRecvLen -= sizeof(SysCmdStr);		    //
						GprsRecvLen -= 4;
						StrLen = GetComma(1,p,GprsRecvLen);         //
						if(StrLen > 0)
						{
							SL_Memcpy(TcpIpSocket.FtpAddr,p,StrLen - 1);     		//    
							SL_Print("FTP Server Addr:%s\r\n",TcpIpSocket.FtpAddr);	 //
							
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
							SL_Print("FTP FtpUserName:%s\r\n",TcpIpSocket.FtpUserName);     //
						}
								
						SL_Memset(Buf,'\0',20); 
						if(GetDataStr(3,4,p,Buf,GprsRecvLen) > 0)       //
						{
							SL_Memcpy(TcpIpSocket.FtpUserPassd,Buf,20);
							SL_Print("FTP FtpUserPassd:%s\r\n",TcpIpSocket.FtpUserPassd);     //
						}

						SL_Memset(Buf,'\0',20); 
						if(GetDataStr(4,5,p,Buf,GprsRecvLen) > 0)       //
						{
							SL_Memcpy(TcpIpSocket.FtpFileName,Buf,20);
							SL_Print("File Name:%s\r\n",TcpIpSocket.FtpFileName);
						}
						
						pUpgrade = GetUpgradeDataSpace();

						TcpIpSocket.LinkType = 1;         //
						TcpIpSocket.ResetLink = 20;       //
						pUpgrade->UpgradeFlag = 0;		  //  
						SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
						break;
					case 0xFE00:    //
						p = GprsRecvBuf + sizeof(SysCmdStr) + 4;    //
						GprsRecvLen -= sizeof(SysCmdStr);		    // 
						GprsRecvLen -= 4;
						StrLen = GetComma(1,p,GprsRecvLen);         //
						if(StrLen > 0)
						{
							SL_Memcpy(TcpIpSocket.FtpAddr,p,StrLen - 1);     		//   
							SL_Print("FTP Server Addr:%s\r\n",TcpIpSocket.FtpAddr);	 //
							
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
							SL_Print("FTP FtpUserName:%s\r\n",TcpIpSocket.FtpUserName);     //
						}
								
						SL_Memset(Buf,'\0',20); 
						if(GetDataStr(3,4,p,Buf,GprsRecvLen) > 0)       //
						{
							SL_Memcpy(TcpIpSocket.FtpUserPassd,Buf,20);
							SL_Print("FTP FtpUserPassd:%s\r\n",TcpIpSocket.FtpUserPassd);     //
						}

						SL_Memset(Buf,'\0',20); 
						if(GetDataStr(4,5,p,Buf,GprsRecvLen) > 0)       //
						{
							SL_Memcpy(TcpIpSocket.FtpFileName,Buf,20);
							SL_Print("File Name:%s\r\n",TcpIpSocket.FtpFileName);
						}
						pUpgrade = GetUpgradeDataSpace();
						TcpIpSocket.LinkType = 1;         //
						TcpIpSocket.ResetLink = 20;       //
						pUpgrade->UpgradeFlag = 1;        //
						SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
						break;
					case 0x0001:            //
						SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
						TmpData.TTbyte[0] = GprsRecvBuf[29];
						TmpData.TTbyte[1] = GprsRecvBuf[30]; 
						pSysCfg->ServerPort = TmpData.IntII;     //
						TmpData.TTbyte[0] = GprsRecvBuf[27];
						TmpData.TTbyte[1] = GprsRecvBuf[28];
						StrLen = TmpData.IntII - 2;
						if(StrLen > 50)
							StrLen = 50;
						SL_Memcpy(pSysCfg->ServerAddr,&GprsRecvBuf[31],StrLen);   // 
						pSysCfg->ServerAddr[StrLen] = '\0';  
						SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes)); 
						SaveSysCfg();                           	//
						SL_Print("Set Server Addr......%s:%d",pSysCfg->ServerAddr,pSysCfg->ServerPort); 
						TcpIpSocket.ResetLink = 20;              // 
						break;
					case 0x0003:           //
						SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes));
						TmpData32.TTbyte[0] = GprsRecvBuf[29];
						TmpData32.TTbyte[1] = GprsRecvBuf[30];
						TmpData32.TTbyte[2] = GprsRecvBuf[31];
						TmpData32.TTbyte[3] = GprsRecvBuf[32];

						if(TmpData32.LongLL < 5)						 //
							TmpData32.LongLL = 5;
						if(TmpData32.LongLL > 120)
							TmpData32.LongLL = 120;
						
						pSysCfg->TravelUpLoadTime = TmpData32.LongLL; 
						SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes)); 
						//SL_Print("Set TravelUpLoadTime......%d",pSysCfg->TravelUpLoadTime); 
						SaveSysCfg();     //
						break;
					case 0x0005:
						TmpData32.TTbyte[0] = GprsRecvBuf[29];
						TmpData32.TTbyte[1] = GprsRecvBuf[30];
						TmpData32.TTbyte[2] = GprsRecvBuf[31];
						TmpData32.TTbyte[3] = GprsRecvBuf[32];

						if(TmpData32.LongLL < 600)						 //
							TmpData32.LongLL = 600;
						if(TmpData32.LongLL > 86400)
							TmpData32.LongLL = 86400;					
						pSysCfg->SleepTime 	= TmpData32.LongLL; 
						SL_Memcpy((unsigned char *)&ComCmdRes,(unsigned char *)pSysCmd,sizeof(ComCmdRes)); 
						SaveSysCfg();
						SL_Print("Set Sleep Time......%d",pSysCfg->SleepTime); 
						break;
					default:
						break;
					}
		}
	}
}



/******************************************
**	函数名称:void ProcessCmd(void)
**	功能描述:
*******************************************/
void ProcessCmd(void)
{	
	static unsigned char 		Counter;
	
	unsigned char 				Buf[50];
	unsigned short int 			tmp;
	IntToChar 					TmpData16;
	LockCarStr 					*pLockCar;
	//CanDataStr					*pCanData;         //
	
	switch(CmdID)
	{
		case 0x00:        
			break;
		case 0x0200:					//
		case 0x0201:					//
			pLockCar = GetLockCarSpace();   //
			Counter++;
			//SL_Print("Recv Lock Car Cmd.........%d,%d\r\n",Counter,pLockCar->LockCurrentState);
			if((Counter < 35 && pLockCar->LockCurrentState == 1) || pLockCar->LockCmdBig == 1 || Counter < 5)
				break;
			if(Counter < 35 && pLockCar->LockCurrentState == 0)
			{
				tmp = 25;    //
				TmpData16.IntII= CmdID;
				*(Buf + tmp) = TmpData16.TTbyte[0];
				tmp++;
				*(Buf + tmp) = TmpData16.TTbyte[1];
				tmp++;
				TmpData16.IntII = 1;
				*(Buf + tmp) = TmpData16.TTbyte[0];
				tmp++;
				*(Buf + tmp) = TmpData16.TTbyte[1];
				tmp++;
				if(CmdID == 0x0200)
					*(Buf + tmp) = 0x00;     //
				else if(CmdID == 0x0201)
					*(Buf + tmp) = 0x08;
				tmp++;
				*(Buf +tmp) = BccVerify(Buf + 3,tmp - 3);    //   
				tmp++;
				*(Buf + tmp) = 0x0d;
				tmp++;
				ComCmdRes.msg_id = 0x01;	//
				ComCmdRes.msg_body_num = 1;  //
				ComCmdRes.msg_len = 5;      //
				SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));
				SL_MEMBLOCK(Buf,tmp,16);			   //
				WriteCmdDataBuf(Buf,tmp);           //
			}
			else 
			{
				tmp = 25;    //
				TmpData16.IntII= CmdID;
				*(Buf + tmp) = TmpData16.TTbyte[0];
				tmp++;
				*(Buf + tmp) = TmpData16.TTbyte[1];
				tmp++;
				TmpData16.IntII = 1;
				*(Buf + tmp) = TmpData16.TTbyte[0];
				tmp++;
				*(Buf + tmp) = TmpData16.TTbyte[1];
				tmp++;
				*(Buf + tmp) = 0x09;     //��Ҫ���� 
				tmp++;
				*(Buf +tmp) = BccVerify(Buf + 3,tmp - 3);    //����У����   
				tmp++;
				*(Buf + tmp) = 0x0d;
				tmp++;
				ComCmdRes.msg_id = 0x01;	//��Ϣ������
				ComCmdRes.msg_body_num = 1;  //��Ϣ�����
				ComCmdRes.msg_len = 5;      //��Ϣ�����ݳ���
				SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));
				SL_MEMBLOCK(Buf,tmp,16);			   //����ʹ��  
				WriteCmdDataBuf(Buf,tmp);           //д���������
			}
			SL_Memset((unsigned char *)&ComCmdRes,0,sizeof(ComCmdRes));
			CmdID = 0;   //�������  ����ID
			break;
		case 0x0203:
			pLockCar = GetLockCarSpace();   //�������ݽṹ��
			Counter++;
			//SL_Print("Recv Mon Cmd.........%d,%d,%d\r\n",Counter,pLockCar->MonCurrentState,pLockCar->MonCmdBig);
			if((Counter < 35 && pLockCar->MonCurrentState == 1) || pLockCar->MonCmdBig == 1 || Counter < 5)
				break;
			if(Counter < 35 && pLockCar->MonCurrentState == 0)  //�����ɹ�
			{
				tmp = 25;    //���ݳ���
				TmpData16.IntII= CmdID;
				*(Buf + tmp) = TmpData16.TTbyte[0];
				tmp++;
				*(Buf + tmp) = TmpData16.TTbyte[1];
				tmp++;
				TmpData16.IntII = 1;
				*(Buf + tmp) = TmpData16.TTbyte[0];
				tmp++;
				*(Buf + tmp) = TmpData16.TTbyte[1];
				tmp++;
				*(Buf + tmp) = 0x00;     //��Ҫ���� 
				tmp++;
				*(Buf +tmp) = BccVerify(Buf + 3,tmp - 3);    //����У����   
				tmp++;
				*(Buf + tmp) = 0x0d;
				tmp++;
				ComCmdRes.msg_id = 0x01;	//��Ϣ������
				ComCmdRes.msg_body_num = 1;  //��Ϣ�����
				ComCmdRes.msg_len = 5;      //��Ϣ�����ݳ���
				SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));
				SL_MEMBLOCK(Buf,tmp,16);			   //����ʹ��  
				WriteCmdDataBuf(Buf,tmp);           //д���������
			}
			else										//����ʧ��
			{ 
				tmp = 25;	 //���ݳ���
				TmpData16.IntII= CmdID;
				*(Buf + tmp) = TmpData16.TTbyte[0];
				tmp++;
				*(Buf + tmp) = TmpData16.TTbyte[1];
				tmp++;
				TmpData16.IntII = 1;
				*(Buf + tmp) = TmpData16.TTbyte[0];
				tmp++;
				*(Buf + tmp) = TmpData16.TTbyte[1];
				tmp++;
				*(Buf + tmp) = 0x00;     //��Ҫ���� 
				tmp++;
				*(Buf +tmp) = BccVerify(Buf + 3,tmp - 3);    //����У����   
				tmp++;
				*(Buf + tmp) = 0x0d;
				tmp++;
				ComCmdRes.msg_id = 0x01;	//��Ϣ������
				ComCmdRes.msg_body_num = 1;  //��Ϣ�����
				ComCmdRes.msg_len = 5;		//��Ϣ�����ݳ���
				SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));
				SL_MEMBLOCK(Buf,tmp,16);			   //����ʹ��   
				
				WriteCmdDataBuf(Buf,tmp);           //д���������
			}
			SL_Memset((unsigned char *)&ComCmdRes,0,sizeof(ComCmdRes));
			CmdID = 0;   //�������
			break;
		case 0xFD00:     //Զ��������ST87����
			tmp = 25;	 //���ݳ���
			TmpData16.IntII= CmdID;
			*(Buf + tmp) = TmpData16.TTbyte[0];
			tmp++;
			*(Buf + tmp) = TmpData16.TTbyte[1];
			tmp++;
			TmpData16.IntII = 1;
			*(Buf + tmp) = TmpData16.TTbyte[0];
			tmp++;
			*(Buf + tmp) = TmpData16.TTbyte[1];
			tmp++;
			*(Buf + tmp) = 0x00;     //��Ҫ���� 
			tmp++;
			*(Buf + tmp) = BccVerify(Buf + 3,tmp - 3);    //����У����   
			tmp++;
			*(Buf + tmp) = 0x0d;
			tmp++;
			ComCmdRes.msg_id = 0x01;	//��Ϣ������
			ComCmdRes.msg_body_num = 1;  //��Ϣ�����
			ComCmdRes.msg_len = 5;		//��Ϣ�����ݳ���
			SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));
			SL_MEMBLOCK(Buf,tmp,16);			   //����ʹ��   
			WriteCmdDataBuf(Buf,tmp);           //д���������
			CmdID = 0;   //�������
			break;
		case 0xFE00:         //Զ��������Ƭ���ظ�
			tmp = 25;	 //���ݳ���
			TmpData16.IntII= CmdID;
			*(Buf + tmp) = TmpData16.TTbyte[0];
			tmp++;
			*(Buf + tmp) = TmpData16.TTbyte[1];
			tmp++;
			TmpData16.IntII = 1;
			*(Buf + tmp) = TmpData16.TTbyte[0];
			tmp++;
			*(Buf + tmp) = TmpData16.TTbyte[1];
			tmp++;
			*(Buf + tmp) = 0x00;     //��Ҫ���� 
			tmp++;
			*(Buf +tmp) = BccVerify(Buf + 3,tmp - 3);    //����У����   
			tmp++;
			*(Buf + tmp) = 0x0d;
			tmp++;
			ComCmdRes.msg_id = 0x01;	//��Ϣ������
			ComCmdRes.msg_body_num = 1;  //��Ϣ�����
			ComCmdRes.msg_len = 5;		//��Ϣ�����ݳ���
			SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));
			SL_MEMBLOCK(Buf,tmp,16);			   //����ʹ��   
			WriteCmdDataBuf(Buf,tmp);           //д���������
			CmdID = 0;   //�������
			break;
		case 0x0001:            //�޸����ص�ַ
			tmp = 25;	 //���ݳ���
			TmpData16.IntII= CmdID;
			*(Buf + tmp) = TmpData16.TTbyte[0];
			tmp++;
			*(Buf + tmp) = TmpData16.TTbyte[1];
			tmp++;
			TmpData16.IntII = 1;
			*(Buf + tmp) = TmpData16.TTbyte[0];
			tmp++;
			*(Buf + tmp) = TmpData16.TTbyte[1];
			tmp++;
			*(Buf + tmp) = 0x00;     //��Ҫ���� 
			tmp++;
			*(Buf +tmp) = BccVerify(Buf + 3,tmp - 3);    //����У����   
			tmp++;
			*(Buf + tmp) = 0x0d;
			tmp++;
			ComCmdRes.msg_id = 0x01;	//��Ϣ������
			ComCmdRes.msg_body_num = 1;  //��Ϣ�����
			ComCmdRes.msg_len = 5;		//��Ϣ�����ݳ���
			SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));
			SL_MEMBLOCK(Buf,tmp,16);			   //����ʹ��   
			WriteCmdDataBuf(Buf,tmp);           //д���������
			//SL_Print("Recv Modmif Cmd 0x0100\r\n");
			CmdID = 0;
			break;
		case 0x0003:           //�޸������ϴ�ʱ��  
			tmp = 25;	 //���ݳ���
			TmpData16.IntII= CmdID;
			*(Buf + tmp) = TmpData16.TTbyte[0];
			tmp++;
			*(Buf + tmp) = TmpData16.TTbyte[1];
			tmp++;
			TmpData16.IntII = 1;
			*(Buf + tmp) = TmpData16.TTbyte[0];
			tmp++;
			*(Buf + tmp) = TmpData16.TTbyte[1];
			tmp++;
			*(Buf + tmp) = 0x00;     //��Ҫ���� 
			tmp++;
			*(Buf +tmp) = BccVerify(Buf + 3,tmp - 3);    //����У����   
			tmp++;
			*(Buf + tmp) = 0x0d;
			tmp++;
			ComCmdRes.msg_id = 0x01;	//��Ϣ������
			ComCmdRes.msg_body_num = 1;  //��Ϣ�����
			ComCmdRes.msg_len = 5;		//��Ϣ�����ݳ���
			SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));
			SL_MEMBLOCK(Buf,tmp,16);			   //����ʹ��   
			WriteCmdDataBuf(Buf,tmp);           //д���������
			//SL_Print("Recv Modmif Cmd 0x0200\r\n");
			CmdID = 0;
			break;
		case 0x0005:
			tmp = 25;	 //���ݳ���
			TmpData16.IntII= CmdID;
			*(Buf + tmp) = TmpData16.TTbyte[0];
			tmp++;
			*(Buf + tmp) = TmpData16.TTbyte[1];
			tmp++;
			TmpData16.IntII = 1;
			*(Buf + tmp) = TmpData16.TTbyte[0];
			tmp++;
			*(Buf + tmp) = TmpData16.TTbyte[1];
			tmp++;
			*(Buf + tmp) = 0x00;     //��Ҫ���� 
			tmp++;
			*(Buf +tmp) = BccVerify(Buf + 3,tmp - 3);    //����У����   
			tmp++;
			*(Buf + tmp) = 0x0d;
			tmp++;
			ComCmdRes.msg_id = 0x01;	//��Ϣ������
			ComCmdRes.msg_body_num = 1;  //��Ϣ�����
			ComCmdRes.msg_len = 5;		//��Ϣ�����ݳ���
			SL_Memcpy(Buf,(unsigned char *)&ComCmdRes,sizeof(ComCmdRes));
			SL_MEMBLOCK(Buf,tmp,16);			   //����ʹ��   
			WriteCmdDataBuf(Buf,tmp);           //д���������
			//SL_Print("Recv Modmif Cmd 0x0500\r\n");
			CmdID = 0;
			break;
		default:
			CmdID = 0;
			break;
	}
}




/*************************************
**	��������:
**	��������:���յ�SMS��Ϣ����
*************************************/
void AppSmsRcvCb(S32 slResult, U8 ucIndex, U8* pucbuf, U16 usLenth, U8* pucTelNumber)
{
	SL_TASK stSltask;
	
	if(slResult != SL_RET_OK)
	{
		return;
	}
	if(SL_SmsGetSmsFormat())
	{
	
		SL_Memset(SMSRecvBuf, 0, 200);		 					//����Ž��ջ���
		SL_Memcpy(SMSRecvBuf, pucbuf, usLenth);//�����ݸ������ջ���
		SMSRecvLen = usLenth / 2;
		SL_Memset(SMSRecvNum, 0, 20); //���������ֻ����뻺��
		SL_Memcpy(SMSRecvNum, pucTelNumber, SL_Strlen(pucTelNumber));
		stSltask.element[0] = SL_GetAppTaskHandle();
		SL_AppSendMsg(stSltask.element[0], EVT_APP_GSM_RECV, 0);			//qyw_sms_rxdata_index
	
	}
}


/****************************************
**	��������:void AppSmsFullCb(S32 slResult)
**	��������:���Ŵ洢��������ɾ��
****************************************/
void AppSmsFullCb(S32 slResult)
{
    //SL_Print("SLAPP: SL_AppSmsFullCb result[%d].", slResult);
    SL_SmsDeleteSms(1, SL_SMS_DEL_ALL);
}




/***************************************
**	��������:
**	��������:��ʼ�����Ź���
***************************************/

void InitSms(void)
{
	SL_SmsSetSmsFormat(SL_SMS_FMT_TXT);    //���Ÿ�ʽ
    SL_SmsSetStorage(SL_SMS_STORAGE_SIM, SL_SMS_STORAGE_SIM, SL_SMS_STORAGE_SIM);   //�洢λ��
    SL_SmsSetTxtModeDataCodSch(4);    //�����ı���ʽ
	 
    SL_SmsSetNewSmsCallback(AppSmsRcvCb);     //�¶��Żص�����
    SL_SmsSetSmsFullCallback(AppSmsFullCb);   //
}





/******************************
**	��������:void ProcessSMSRecv(void)
**	��������:������������
**	��ʱʹ�ù̶�FTP��ַ�����û���
*******************************/

/*
void ProcessSMSRecv(void)
{
	UpgradeStatusStr 			*pUpgrade;
	unsigned char 				*p;
	int			 				tmp;
	unsigned char 				Buf[20];
	SysConfigStr 				*pSysCfg;
	//SL_Print("Recv SMS Info:%d\r\n",SMSRecvLen);    //������Ϣ
	SL_SmsDeleteSms(1,SL_SMS_DEL_INDEX);	//ɾ��

	tmp = LookForStr(SMSRecvBuf,"FTPUPD",SMSRecvLen);   //��ѯ�豸ͷ
	//SL_Print("Recv SMS Info:%d,%c,%d\r\n",tmp,*(SMSRecvBuf + tmp + 6),SMSRecvLen);    //������Ϣ
	
	//SL_MEMBLOCK(SMSRecvBuf,SMSRecvLen,16);      //���������Ϣ

	
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
		return;    
	}

	
	
	tmp = GetComma(1,p,SMSRecvLen);      //�ҵ���һ������ 
	SL_Memset(Buf,'\0',20);
	SL_Memcpy(Buf,p,tmp - 1);    		//�����豸��
	SL_Print("> The Device ID:%s\r\n",Buf);
	pSysCfg = GetSysCfgSpace();
	if(StrCompare(pSysCfg->TerminalID,Buf,16) == 0)
		return;
	SL_Memset(Buf,0,20);
	if(GetDataStr(1,2,p,Buf,SMSRecvLen) > 0)
	{
		SL_Memcpy((unsigned char *)&TcpIpSocket.FtpAddr,Buf,50);	 //����FTP��ַ	
		SL_Print(">FTP Server Addr:%s\r\n",TcpIpSocket.FtpAddr);     //����ʹ��
	}
	
	SL_Memset(Buf,0,20);
	if(GetDataStr(2,3,p,Buf,SMSRecvLen) > 0)       		   //�˿ں� 
	{
		TcpIpSocket.FtpPort = StrToDouble(Buf);
		SL_Print(">FTP Server Port:%d\r\n",TcpIpSocket.FtpPort);     //����FTP�˿�
	}
	else
	{
		return;
	}

	SL_Memset(Buf,0,20);
	if(GetDataStr(3,4,p,Buf,SMSRecvLen) > 0)     		   //FTP�û���
	{
		SL_Memcpy((unsigned char *)&TcpIpSocket.FtpUserName,Buf,20);
		SL_Print(">FTP User Name:%s\r\n",TcpIpSocket.FtpUserName);   //FTP�û�����:
	}
	else
	{
		return;
	}

	SL_Memset(Buf,0,20);
	if(GetDataStr(4,5,p,Buf,SMSRecvLen) > 0)          //FTP����
	{
		SL_Memcpy((unsigned char *)&TcpIpSocket.FtpUserPassd,Buf,20);
		SL_Print(">FTP User PassdWord:%s\r\n",TcpIpSocket.FtpUserPassd); //FTP

	}
	else
	{
		return;
	}

	SL_Memset(Buf,0,20);
	if(GetDataStr(5,6,p,Buf,SMSRecvLen) > 0)
	{
		SL_Memcpy((unsigned char *)&TcpIpSocket.FtpFileName,Buf,20);
		SL_Print(">FTP File Name:%s\r\n",TcpIpSocket.FtpFileName);    //FTP�ļ�����
	}
	else
	{
		return;
	}

	SL_Memset(Buf,0,20);
	if(GetDataStr(6,7,p,Buf,SMSRecvLen) > 0)
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


*/




/******************************
**	��������:void ProcessSMSRecv(void)
**	��������:������������
**	��ʱʹ�ù̶�FTP��ַ�����û���
*******************************/

///*
void ProcessSMSRecv(void)
{
	UpgradeStatusStr 			*pUpgrade;
	unsigned char 				*p;
	int			 				tmp;
	unsigned char 				Buf[50];
	SysConfigStr 				*pSysCfg;

	//SL_MEMBLOCK(SMSRecvBuf,SMSRecvLen,16);	  //���������Ϣ

	SL_SmsDeleteSms(1,SL_SMS_DEL_INDEX);	//ɾ��
	tmp = LookForStr(SMSRecvBuf,"<<up",SMSRecvLen);   //��ѯ�豸ͷ
	
	if(tmp >= 0)    //��ѯ��Ϣͷ
	{
		p = SMSRecvBuf + tmp + 4;
		if(*p != ':')
			return;
		SMSRecvLen = SMSRecvLen - tmp - 4;
		p++;
			
		tmp = GetComma(1,p,SMSRecvLen);      //�ҵ���һ������ 
		SL_Memset(Buf,'\0',50);
		SL_Memcpy(Buf,p,tmp - 1);    		//�����豸��

		SL_Memcpy((unsigned char *)&TcpIpSocket.FtpAddr,Buf,50);	 //����FTP��ַ	
		SL_Print(">FTP Server Addr:%s\r\n",TcpIpSocket.FtpAddr);     //����ʹ��
		
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

		tmp = GetComma(6,p,SMSRecvLen);      //�ҵ���һ������ 
		pSysCfg = GetSysCfgSpace();
		//SL_Print("The DevID:%c,%d,%c,%c\r\n",*(p + tmp),tmp,*(p + tmp + 16),*(p + tmp + 17));
		if(*(p + tmp + 16) != '>' || *(p + tmp + 17) != '>')
			return;
		SL_Memset(Buf,'\0',sizeof(Buf));
		SL_Memcpy(Buf + 20,p + tmp,16);
		
		for(tmp = 0;tmp < 16;tmp++)
		{
			Buf[tmp] = Buf[20 + 15 - tmp];	
		}
		
		//SL_Print("The DevID:%s\r\n",Buf);

		if(StrCompare(pSysCfg->TerminalID,Buf,16) == 0)
			return;
		
		
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
				return;
			}
			//���һλ�����ַ�"S"����"T",���Ը�������
		}
		else
		{
			return;
		}
	}

	tmp = LookForStr(SMSRecvBuf,"<<mg",SMSRecvLen);   //��ѯ�豸ͷ
	
	if(tmp >= 0)    //��ѯ��Ϣͷ
	{
		p = SMSRecvBuf + tmp + 4;
		if(*p != ':')
			return;
		SMSRecvLen = SMSRecvLen - tmp - 4;
		p++;

		pSysCfg = GetSysCfgSpace();
		
		tmp = GetComma(1,p,SMSRecvLen);      //�ҵ���һ������ 
		SL_Memset(Buf,'\0',50);
		SL_Memcpy(Buf,p,tmp - 1);    		//�����豸��
		
		SL_Memcpy((unsigned char *)&pSysCfg->ServerAddr,Buf,50);	 //����FTP��ַ	
		SL_Print(">The ServerAddr:%s\r\n",pSysCfg->ServerAddr);     //����ʹ��
		SL_Memset(Buf,0,20);
		if(GetDataStr(1,2,p,Buf,SMSRecvLen) > 0)       		   //�˿ں� 
		{
			
			pSysCfg->ServerPort = StrToDouble(Buf);
			SL_Print(">The ServerPort:%d\r\n",pSysCfg->ServerPort);     //����FTP�˿�
		}
		else
		{
			return;
		}

		tmp = GetComma(2,p,SMSRecvLen);      //�ҵ���һ������ 
		pSysCfg = GetSysCfgSpace();
		//SL_Print("The DevID:%c,%d,%c,%c\r\n",*(p + tmp),tmp,*(p + tmp + 16),*(p + tmp + 17));
		if(*(p + tmp + 16) != '>' || *(p + tmp + 17) != '>')
			return;
		SL_Memset(Buf,'\0',sizeof(Buf));
		SL_Memcpy(Buf + 20,p + tmp,16);
		
		for(tmp = 0;tmp < 16;tmp++)
		{
			Buf[tmp] = Buf[20 + 15 - tmp];	
		}
		
		//SL_Print("The DevID:%s\r\n",Buf);

		if(StrCompare(pSysCfg->TerminalID,Buf,16) == 0)
			return;

		SaveSysCfg();
		
		//SL_Print(">App:Save Sys Cfg...........");
		TcpIpSocket.ResetLink = 20; 
	}
}


//*/



/******************************File End ***************************/









