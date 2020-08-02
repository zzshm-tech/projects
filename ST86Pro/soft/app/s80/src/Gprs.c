
/***********************************************
**	File Name:
**	Time:

************************************************/

#include "include.h"


/****************************************************/

#define DEF_SERVER_ADDR "njgps-bc.lovol.com"
#define DEF_SERVER_PORT 15000



extern HANDLE SysAppEnterTask;

static TcpIPSocketStr TcpIpSocket;                  //链接信息

static unsigned char GprsRecvBuf[255];           	//Gprs数据接收缓冲区   			

static unsigned short int GprsRecvLen;              //接收到的数据长度			

static unsigned short int FTPLoadNum = 0;


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
		TcpIpSocket.GprsNetActive = 1;           //标识网络已经激活
	}
	else
	{
		SL_Print(">App:Net Action Fail..........%d\r\n",slErrorCode);  
		TcpIpSocket.GprsNetActive = 2; 
	}
}


/****************************************************
**	函数名称:
**	功能描述:输出网
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
** 	功能描述:输出连接信息
****************************************************/

void AppTcpipConnRsp(unsigned char ucCidIndex, int slSockId, BOOL bResult, int slErrorCode)
{
	if(bResult == TRUE)
	{
		TcpIpSocket.LinkState = 1;        //链接成功
		TcpIpSocket.LinkNum++;            //连接次数  
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
	else              //正确接收
	{
		slRet = SL_TcpipSocketRecv(slSockId, GprsRecvBuf, sizeof(GprsRecvBuf));  //读取接收数据
		if(slRet >= 0)
		{
			
			stSltask.element[0] = SL_GetAppTaskHandle();
			//SL_MEMBLOCK(GprsRecvBuf,slRet,16);
			
			GprsRecvLen = (slRet > 1000) ? 1000 : slRet;
			SL_AppSendMsg(stSltask.element[0], EVT_APP_GPRS_RECV, 0);	//收到GPRS数据
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
    	TcpIpSocket.LinkState = 0;                               //正确关闭
        SL_Print(">APP: socket Close fail[%d]", slErrorCode);
    }
	else
	{
		TcpIpSocket.LinkState = 0;   
		SL_Print(">APP: socket[%d] Close OK", slSockId);          
	}
}


/***********************************************
**	函数名称:void InitTcpIP(void)
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
	
	
    SL_TcpipGprsNetInit(0, &stSlTcpipCb);         //注册回调函数 目前只能使用0号通道    
    
    TcpIpSocket.LinkEnable = 1;
}



/*******************************************
**	函数名称:
**	功能描述:
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
**	函数名称:
**	功能描述:下载FTP文件完成回调函数
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
**	函数名称:
**	功能描述:  关闭Ftp  当文件下载完成后 关闭RTC
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
	    	SL_SendEvents(hTask, &event);                   		 //发送时间  
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
**	功能描述:初始化一下FTP功能要调用的回调函数
*******************************************/

static void AppFtpInit(void)
{
    SL_FTP_CALLBACK ftp_callbk;

    ftp_callbk.pstSlFtpLogin = AppFtpOpenCB;        //打开FTP
    ftp_callbk.pstSlFtpDownload = AppFtpDownloadCB;   //下载完成FTP
    ftp_callbk.pstSlFtpClose = AppFtpCloseCB;          //关闭FTP
    SL_Ftp_Callback_Init(&ftp_callbk);   
}



/*********************************************
**	功能描述:
**	功能描述:打开FTP
**********************************************/

static unsigned char AppFtpOpen(void)
{
    SL_FTP_Connect_Info ftp_info;
    int ret = 0;
	
    ftp_info.u16DestPort = TcpIpSocket.FtpPort;
	
    SL_Strcpy(ftp_info.uaUrl, (const char *)TcpIpSocket.FtpAddr); 			 	//FTP地址
    SL_Strcpy(ftp_info.uaUsername, (const char *)TcpIpSocket.FtpUserName);		//FTP用户名
    SL_Strcpy(ftp_info.uaPassword, (const char *)TcpIpSocket.FtpUserPassd); 		//FTP密码
	
    ret = SL_Ftp_Open(&ftp_info);
	if(ret == SL_RET_OK)
    	return 1;
	return 0;
	
}

/*******************************************************
**	函数名称:
**	功能描述:发送函数
*******************************************************/

unsigned char AppSendTcpIpSocket(unsigned char *buf,unsigned short int len)
{
	int rv;

	if(len == 0 || len > 1380)
		return 0;
	rv = SL_TcpipSocketSend(0,buf,len);
	if(rv > 0)
	{
		TcpIpSocket.SendState = 1;    //发送中
		return 1;
	}
	return 0;
}


/********************************************************
**	函数名称:
**	功能描述:返回发送状态
********************************************************/

unsigned char ReadSendState(void)
{
	if(TcpIpSocket.LinkState != 2)
		return 0;
	if(TcpIpSocket.SendState == 1 )
		return 1;
	if(TcpIpSocket.SendState == 2|| TcpIpSocket.SendState == 0)
		return 2;
	return 3;                           //失败，	
}


/*******************************************************
**	函数名称:
**	功能描述:获取ICCID回调函数。
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
**	函数名称:void AppGetHostIpByNameCb(U8 ucCidIndex, S32 slErrorCode, U8* pucIpAddr)
**	功能描述:获取域名回调函数
*************************************************/

void AppGetHostIpByNameCb(U8 ucCidIndex, S32 slErrorCode, U8* pucIpAddr)
{
	SL_Print(">App: SL_AppGetHostIpByNameCb cid[%d], result[%d]\r\n", ucCidIndex, slErrorCode);    //作为调试信息使用
	SL_Print(">App: IP:%s\r\n", pucIpAddr);                                                       //作为调试信息使用

	if(slErrorCode == 0)                						//
	{
		//
		TcpIpSocket.UlrToIPFlag = 1;//
		SL_Memcpy(TcpIpSocket.ServerAddr,pucIpAddr,20);
	}
}



/*************************************************************
**	函数名称:void ProcessSocketClient(void)
**	功能描述:
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
			TcpIpSocket.SimState = SL_IsSIMInserted();    	//检测是否有Sim卡 
			if(TcpIpSocket.SimState == TRUE)
			{
				step++;
				SL_Print(">App:Sim Card OK\r\n");            //SIm卡OK,
				break;
			}
			SL_Print(">App:Not Sim Card\r\n");
			break;
		case 1:
			slRet = SL_SimGetCCID(AppGetICCId);             //获取SIM  ICC_ID; 
			if(slRet == SL_RET_OK)
			{
				step++;
				break;
			}
			break;
		case 2:               								//获取Sim卡状态  
			SL_GetDeviceCurrentRunState(&TcpIpSocket.SimStatus,&TcpIpSocket.NetWorkState,&slRet,&TcpIpSocket.GsmSign,&TcpIpSocket.ErrorCode);
			if(TcpIpSocket.SimStatus == 1 && TcpIpSocket.NetWorkState == 5)
			{
				SL_Print("Register Net..........OK\r\n");   //等待网络注册  
				TcpIpSocket.LinkState = 0;                  //网络已经注册
				step++;
				erro_code = 0;
				break;
			}
			SL_Print("Register Net............\r\n");
			break;
		case 3:              //设置接入方式
			pCfg = GetSysCfgSpace();
			SL_Memcpy(TcpIpSocket.GprsApn,pCfg->TerApn,20);
			//SL_Print(">App:Apn :%s\r\n",TcpIpSocket.GprsApn);
			slRet = SL_TcpipGprsApnSet(TcpIpSocket.GprsApn,"","");  	//设置接入点
			if(slRet != SL_RET_OK)            							//调试通过
			{
				if(erro_code++ > 5)
					step = 14;
				break;
			}
			SL_Print(">Set Apn OK.....!\r\n");
			erro_code = 0;
			step++;
			break;
		case 4:                    							 //激活
			slRet = SL_TcpipGprsNetGetState(&TcpIpSocket.GprsNetActive,&TcpIpSocket.NetGprsState);  //
			if(slRet == SL_RET_OK)
			{
				SL_Print(">App:NetGprsState:%d,GprsNetActive %d\r\n",TcpIpSocket.NetGprsState,TcpIpSocket.GprsNetActive);
				if(TcpIpSocket.GprsNetActive != 1 || TcpIpSocket.NetGprsState != 5)
				{
					
					slRet = SL_TcpipGprsNetActive();     //激活上下文  
					if(slRet != SL_RET_OK)
					{
						if(erro_code++ > 5)
							step = 14;                  //重新启动
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
			if(TcpIpSocket.GprsNetActive == 2 || TcpIpSocket.GprsNetActive == 0)   		//激活失败，
			{
				step = 4;
				break;
			}
			TcpIpSocket.LinkState = 1;										//注册
			if(TcpIpSocket.LinkType == 0)                 					//整除链接
				step++;
			else if(TcpIpSocket.LinkType == 1)            					//FTP链接
				step = 15;                                					//
			erro_code = 0;
			break;
		case 6:
			slRet = SL_TcpipSocketCreate(0, SL_TCPIP_SOCK_TCP);   	 //创建连接
			if(slRet != SL_RET_OK)
    		{
				if(erro_code++ > 5)
					step = 14;                                 		//重启设备
				break;
    		}
			SL_Print(">App:	Creat Link......OK %d\r\n",slRet);
			step++;
			erro_code = 0;
			break;
		case 7:
			slRet = SL_TcpipSocketBind(0);                 			//绑定本地Ip地址      
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
				TcpIpSocket.LinkNum = 0;                                                    //链接次数
				SL_Memcpy(TcpIpSocket.ServerAddr,DEF_SERVER_ADDR,sizeof(DEF_SERVER_ADDR));  //使用默认IP地址
				TcpIpSocket.ServerPort = DEF_SERVER_PORT;        //使用默认端口
			}
			else
			{
				SL_Memcpy(TcpIpSocket.ServerAddr,pCfg->ServerAddr,50);    	//配置地址
				TcpIpSocket.ServerPort = pCfg->ServerPort;             		//获取端口  
			}
			TcpIpSocket.LinkNum++;
			tmp= CheckServerAddr(TcpIpSocket.ServerAddr,SL_Strlen(TcpIpSocket.ServerAddr));
			//SL_Print(">>>>%s    %d\r\n",TcpIpSocket.ServerAddr,tmp);
			if(tmp == 0)          												   		//说明是域名
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
			else                  // Ip地址
			{
				step = 10;             //
				erro_code = 0;
				counter = 0;
				break;
			}
		case 9:
			if(TcpIpSocket.UlrToIPFlag == 0 && counter++ < 30)   //等待 30 s
				break;
			if(TcpIpSocket.UlrToIPFlag == 1)
			{
				step++;
				counter = 0;
				erro_code = 0;												//获取域名成功
				pCfg = GetSysCfgSpace();
				if(SL_Strcmp(pCfg->UpdateAddr,TcpIpSocket.ServerAddr) > 0)
				{
					SL_Memcpy(pCfg->UpdateAddr,TcpIpSocket.ServerAddr,50);        //  
					SaveSysCfg();             //保存系统配置
				}
				break;
			}
			SL_Memcpy(TcpIpSocket.ServerAddr,pCfg->UpdateAddr,50);        // 
			step++;
			break;
		case 10:
			SL_Print(">App:Link ServerAddr(%d) %s:%d\r\n",TcpIpSocket.LinkNum,TcpIpSocket.ServerAddr,TcpIpSocket.ServerPort);    //链接到的服务器
			slRet = SL_TcpipSocketConnect(0, TcpIpSocket.ServerAddr,TcpIpSocket.ServerPort);   		//服务器连接   
			
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
			if(TcpIpSocket.LinkState == 0 && counter++ < 50)   		  //等待 50 s
				break;
			if(TcpIpSocket.LinkState == 3)  //连接失败
			{
				step = 4;
				SL_Print(">App:Link Server.........Fail---1\r\n");    //链接失败
				break;
			}
			else if(TcpIpSocket.LinkState == 0)
			{
				step = 4;
				SL_Print(">App:Link Server.........Fail---2\r\n");     //链接失败
				break;
			}
			step++;
			counter = 0;           //链接完成之后
			SL_Print(">APP:Link Server.........OK\r\n");
			break;
		case 12:
			slRet = SL_TcpipGetState(0);    					//获取连接状态  
			if(slRet == SL_TCPIP_STATE_CONNECTOK)
			{
				TcpIpSocket.LinkState = 2;                    //正式链接成功
			}
			else if(slRet == SL_TCPIP_STATE_CONNECTING)      //正在连接中
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
			if(counter > 20)                     					//没10秒钟获取一次状态
			{ 
				counter = 0;
				SL_GetDeviceCurrentRunState(&TcpIpSocket.SimStatus,&TcpIpSocket.NetWorkState,&slRet,&TcpIpSocket.GsmSign,&TcpIpSocket.ErrorCode);
				TcpIpSocket.GsmSign /= 10;
				SL_Print(">App: The Gsm Sign Value: %d\r\n",TcpIpSocket.GsmSign);
			}
			
			if(TcpIpSocket.LinkState != 2)         //链接状态
			{	                                                  
				step = 4;                          //重新去连接 
				break;
			}

			if(TcpIpSocket.ResetLink > 1)          //
				TcpIpSocket.ResetLink--;
			
			if(TcpIpSocket.ResetLink == 1)        //是否重新链接
			{
				TcpIpSocket.ResetLink = 0;        //
				TcpIpSocket.LinkState = 0;
				step++;
				break;							  //关闭链接
			}
			
			if(TcpIpSocket.SendState == 1)        //监控发送状态
			{
				if(SendCounter++ > 10)           //若10秒钟之内发送还没有完成， 启动重新链接
				{
					step++;                   
					TcpIpSocket.LinkState = 0;
					TcpIpSocket.SendState = 0;
					SendCounter = 0;
				}
			}
			else
			{
				SendCounter = 0;               	  //发送等待时间
			}
			break;
		case 13:
			slRet = SL_TcpipSocketClose(0);        //关闭链接
			//if(slRet == SL_RET_OK)
			//{
			//	step;                          //回到第四部重新创建链接
			//	break;
			//}
			step = 4;
			break;
		case 14:                                  	//登录FTP
			SL_Print(">APP:Reset S86 Sys\r\n");   	//重新启动系统  
			SaveSendQueue(); 
			SL_Sleep(10000);							//等待10秒
			SL_HstSendEvt(0x20161034);      		//更新完成 
            SL_PowerDown(0); 						//重启
			break;
		case 15:                                    //初始化FTP    从15部开始进行FTP链接
			AppFtpInit(); 
			step++;
			erro_code = 0;
			if(FTPLoadNum++ > 2)
			{
					step = 14;         				//从新启动系统   直接重启   
					break;
			}
			SL_Print("Init Ftp App.......\r\n");
			break;
		case 16:
			slRet = AppFtpOpen();                  //链接
			if(slRet != 1)
    		{
    			if(erro_code++ > 5)
					step = 14;
				break;
    		}
			step++;
			counter = 0;
			break;
		case 17:                        							//等待FTP链接成功 
			if(TcpIpSocket.FtpLinkState== 0 && counter++ < 50)   	//等待 50
				break;
			if(TcpIpSocket.FtpLinkState == 2)  		//连接失败
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
			slRet = SL_Ftp_Download(TcpIpSocket.FtpFileName, 1);    //开始下载FTP文件
				
			if(slRet != SL_RET_OK)                     //如果执行失败
			{
				if(erro_code++ > 5)
					step = 14;
			}
			step++;
			counter = 0;
			TcpIpSocket.FtpLoadFig = 1;       //正在下载FTP文件
			break;
		case 19:                             //正在下载 
			if(TcpIpSocket.FtpLoadFig == 1 && counter++ < 90)             // 1分钟
				break;
			if(TcpIpSocket.FtpLoadFig == 1 || TcpIpSocket.FtpLoadFig == 3)   //FTP下载失败
			{ 
				SL_Print(">App:(%d)	DownLoad %s  .........Fail\r\n",FTPLoadNum,TcpIpSocket.FtpFileName);
				step++;
				break;
			}
			if(TcpIpSocket.FtpLoadFig == 2)    //升级成功  下载成功
			{
				step++;
				erro_code = 0;
			}
			break;
		case 20:                         				//关闭链接 
			slRet = SL_Ftp_Close();
			if(slRet != SL_RET_OK)                     	//如果执行失败
			{
				if(erro_code++ > 5)   
					step = 14;
				break;
			}
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
**	函数名称:
**	功能描述:解析接收命令处理(接收数据处理)
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

	
	if(GprsRecvLen < 25)                    //先判断接收到数据的长度
		return;
	if(GprsRecvBuf[0] != 0xF1 && GprsRecvBuf[1] != 0xF2 && GprsRecvBuf[2] != 0xFF)   //判断包头
		return;
	//SL_MEMBLOCK(GprsRecvBuf,GprsRecvLen,16);             //调试使用
	tmp = BccVerify(GprsRecvBuf + 3,GprsRecvLen - 4);    //计算校验码
	if(tmp != *(GprsRecvBuf + GprsRecvLen - 1))          //比较校验
		return;
	
	
	MsgID = GprsRecvBuf[3];
	pMsgHead = (MsgHeadStr *)GprsRecvBuf;
	pMsgHead->msg_id = 0x01;
	pMsgHead->msg_body_num = 0x01;
	pMsgHead->msg_len = 0x01;
	SL_Memcpy(Buf,GprsRecvBuf,25);                 	//附近返回命令
	p = GprsRecvBuf + 25;
	TmpData.TTbyte[0] = *(p);
	TmpData.TTbyte[1] = *(p + 1);   
	CmdID = TmpData.IntII;  
	pSysCfg = GetSysCfgSpace();                     //
	
	if(pSysCfg->CfgFlag == 0x55)             		//生产模式配置
	{
		switch (MsgID)
		{ 
			case 0x89:      //
				break;
			case 0xff:		//鉴权服务器给设备回应的数据	ID                     //
				break;
			case 0x02:		//平台回应错误信息  (如果错误信息 20次，进行重新连接)    //
				
				break;
			case 0x47:		//平台读设备端的配置
				break;
			case 0x48:		//平台配置设备
				switch (CmdID)    //平台配置命令
				{
					case 0x0001:					//平台服务器配置鉴权服务器端口和IP(域名)   
						TmpData.TTbyte[0] = GprsRecvBuf[29];
						TmpData.TTbyte[1] = GprsRecvBuf[30]; 
						pSysCfg->ServerPort = TmpData.IntII;     //获取端口
						TmpData.TTbyte[0] = GprsRecvBuf[27];
						TmpData.TTbyte[1] = GprsRecvBuf[28];
						MSGLen = TmpData.IntII - 2;
						if(MSGLen > 50)
							MSGLen = 50;
						SL_Memcpy(pSysCfg->ServerAddr,&GprsRecvBuf[31],MSGLen);   //配置的服务器端口 
						pSysCfg->ServerAddr[MSGLen] = '\0';  
						
						SaveSysCfg();                           	//保存配置
						SL_Print("Set Server Addr......%s:%d",pSysCfg->ServerAddr,pSysCfg->ServerPort); 
						MSGLen = 25;
						Buf[MSGLen++] = 0x01;                          //消息类型码
						Buf[MSGLen++] = 0x00;
						TmpData.IntII = 2 + SL_Strlen(pSysCfg->ServerAddr);
						Buf[MSGLen++] = TmpData.TTbyte[0];                 //数据长度
						Buf[MSGLen++] = TmpData.TTbyte[1];
						SL_Memcpy(&Buf[MSGLen],pSysCfg->ServerAddr,TmpData.IntII);  //域名IP 
						MSGLen += TmpData.IntII;       //
						Buf[MSGLen] = BccVerify(Buf + 3,MSGLen - 3);
						MSGLen++;
						Buf[MSGLen++] = 0x0D;
					
						WriteCmdDataBuf(Buf,MSGLen);     			//把返回命令写入命令发送队列
						TcpIpSocket.ResetLink = 10;               	//10秒钟之后重新链接  
						break;
					case 0x0003://定时上传时间间隔  
						TmpData32.TTbyte[0] = GprsRecvBuf[29];
						TmpData32.TTbyte[1] = GprsRecvBuf[30];
						TmpData32.TTbyte[2] = GprsRecvBuf[31];
						TmpData32.TTbyte[3] = GprsRecvBuf[32];

						if(TmpData32.LongLL < 5)						 //对
							TmpData32.LongLL = 5;
						if(TmpData32.LongLL > 120)
							TmpData32.LongLL = 120;
						
						pSysCfg->TravelUpLoadTime = TmpData32.LongLL; 
						SaveSysCfg();     //保存数据
						MSGLen = 25;
						Buf[MSGLen++] = 0x03;
						Buf[MSGLen++] = 0x00;   //消息ID

						Buf[MSGLen++] = TmpData32.TTbyte[0];
						Buf[MSGLen++] = TmpData32.TTbyte[1];
						Buf[MSGLen++] = TmpData32.TTbyte[2];
						Buf[MSGLen++] = TmpData32.TTbyte[3];    //
						Buf[MSGLen] = BccVerify(Buf + 3,MSGLen - 3);
						MSGLen++;
						Buf[MSGLen++] = 0x0D;
						WriteCmdDataBuf(Buf,MSGLen);     //把返回命令写入命令发送队列
						SL_Print("Set UpLoad Time.....%d",pSysCfg->TravelUpLoadTime);
						break;
					case 0x0004://休眠前的运行时间
						TmpData32.TTbyte[0] = GprsRecvBuf[29];
						TmpData32.TTbyte[1] = GprsRecvBuf[30];
						TmpData32.TTbyte[2] = GprsRecvBuf[31];
						TmpData32.TTbyte[3] = GprsRecvBuf[32];

						if(TmpData32.LongLL < 300)						 //对
							TmpData32.LongLL = 300;
						if(TmpData32.LongLL > 86400)
							TmpData32.LongLL = 86400;
						
						pSysCfg->RunTime 	= TmpData32.LongLL;

						SaveSysCfg();    //保存配置
						MSGLen = 25;
						Buf[MSGLen++] = 0x04;
						Buf[MSGLen++] = 0x00;   //消息ID

						Buf[MSGLen++] = TmpData32.TTbyte[0];
						Buf[MSGLen++] = TmpData32.TTbyte[1];
						Buf[MSGLen++] = TmpData32.TTbyte[2];
						Buf[MSGLen++] = TmpData32.TTbyte[3];    //
						Buf[MSGLen] = BccVerify(Buf + 3,MSGLen - 3);
						MSGLen++;
						Buf[MSGLen++] = 0x0D;             //包尾
						WriteCmdDataBuf(Buf,MSGLen);     //把返回命令写入命令发送队列
						SL_Print("Set Run Time........%d\n",pSysCfg->RunTime);
						break;
					case 0x0005://设备休眠时间
						TmpData32.TTbyte[0] = GprsRecvBuf[29];
						TmpData32.TTbyte[1] = GprsRecvBuf[30];
						TmpData32.TTbyte[2] = GprsRecvBuf[31];
						TmpData32.TTbyte[3] = GprsRecvBuf[32];

						if(TmpData32.LongLL < 600)						 //对
							TmpData32.LongLL = 600;
						if(TmpData32.LongLL > 86400)
							TmpData32.LongLL = 86400;					
						pSysCfg->SleepTime 	= TmpData32.LongLL;  
						SaveSysCfg();
						MSGLen = 25;
						Buf[MSGLen++] = 0x05;
						Buf[MSGLen++] = 0x00;   //消息ID
						Buf[MSGLen++] = TmpData32.TTbyte[0];
						Buf[MSGLen++] = TmpData32.TTbyte[1];
						Buf[MSGLen++] = TmpData32.TTbyte[2];
						Buf[MSGLen++] = TmpData32.TTbyte[3];    //
						Buf[MSGLen] = BccVerify(Buf + 3,MSGLen - 3); 
						MSGLen++;
						Buf[MSGLen++] = 0x0D;     //添加包尾
						WriteCmdDataBuf(Buf,MSGLen);     //把返回命令写入命令发送队列
						
						SL_Print("Set Sleep Time.......%d\r\n",pSysCfg->SleepTime);
						break;
					case 0x0007://校时(设置时间)
						TmpTime.Year	= GprsRecvBuf[29];
						TmpTime.Mon 	= GprsRecvBuf[30];
						TmpTime.MDay	= GprsRecvBuf[31];
						TmpTime.Hour 	= GprsRecvBuf[32];
						TmpTime.Min	    = GprsRecvBuf[33];
						TmpTime.Sec 	= GprsRecvBuf[34];
						TmpData32.LongLL = CalendarToUnix(TmpTime);
						SetSysTime(TmpData32.LongLL);            		//调用设置时间函数   

						MSGLen = 25;
						Buf[MSGLen++] = 0x07;
						Buf[MSGLen++] = 0x00;   //消息ID
						Buf[MSGLen++] = TmpData32.TTbyte[0];
						Buf[MSGLen++] = TmpData32.TTbyte[1];
						Buf[MSGLen++] = TmpData32.TTbyte[2];
						Buf[MSGLen++] = TmpData32.TTbyte[3];    //
						Buf[MSGLen] = BccVerify(Buf + 3,MSGLen - 3); 
						MSGLen++;
						Buf[MSGLen++] = 0x0D; 
						WriteCmdDataBuf(Buf,MSGLen);     //把返回命令写入命令发送队列
						SL_Print("Set Sys Time.........%d\r\n",TmpData32.LongLL);
						break;
					case 0x00FD://平台远程升  颜工测试成功     
						//SL_MEMBLOCK(GprsRecvBuf,GprsRecvLen,16);        //
						GprsRecvLen = GprsRecvLen - 30;
						StrLen = GetComma(1,&GprsRecvBuf[29],GprsRecvLen);
						if(StrLen > 0)
						{
							SL_Memcpy(TcpIpSocket.FtpAddr,&GprsRecvBuf[29],StrLen - 1);     //去设备地址  
							SL_Print("FTP Server Addr:%s\r\n",TcpIpSocket.FtpAddr);     //调试信息用户名称
						}
							
						SL_Memset(GprsRecvBuf,'\0',20); 
						if(GetDataStr(1,2,&GprsRecvBuf[29],GprsRecvBuf,GprsRecvLen) > 0)        //端口号 
						{
							TcpIpSocket.FtpPort = StrToDouble(GprsRecvBuf);
							SL_Print("FTP Server Port:%d\r\n",TcpIpSocket.FtpPort);     //
						}
							
						SL_Memset(GprsRecvBuf,'\0',20); 
						if(GetDataStr(2,3,&GprsRecvBuf[29],GprsRecvBuf,GprsRecvLen) > 0)       //用户名称 
						{
							SL_Memcpy(TcpIpSocket.FtpUserName,GprsRecvBuf,20);
							SL_Print("FTP UserName:%s\r\n",TcpIpSocket.FtpUserName);	 //
						}
						
						SL_Memset(GprsRecvBuf,'\0',20); 
						if(GetDataStr(3,4,&GprsRecvBuf[29],GprsRecvBuf,GprsRecvLen) > 0)       //用户密码
						{
							SL_Memcpy(TcpIpSocket.FtpUserPassd,GprsRecvBuf,20);
							SL_Print("FTP UserPassd:%s\r\n",TcpIpSocket.FtpUserPassd);  //
						}

						SL_Memset(GprsRecvBuf,'\0',20); 
						if(GetDataStr(4,5,&GprsRecvBuf[29],GprsRecvBuf,GprsRecvLen) > 0)       //用户密码 
						{
							SL_Memcpy(TcpIpSocket.FtpFileName,GprsRecvBuf,20);
							SL_Print("File Name:%s\r\n",TcpIpSocket.FtpFileName);
						}
						pUpgrade = GetUpgradeDataSpace();
						TcpIpSocket.LinkType = 1;
						TcpIpSocket.ResetLink = 10;       //10秒之后重启链接
						pUpgrade->UpgradeFlag = 0;
						
						break;
					case 0x00FE://stm32远程升级
						GprsRecvLen = GprsRecvLen - 30;
						StrLen = GetComma(1,&GprsRecvBuf[29],GprsRecvLen);
						if(StrLen > 0)
						{
							SL_Memcpy(TcpIpSocket.FtpAddr,&GprsRecvBuf[29],StrLen - 1);     //去设备地址  
							SL_Print("FTP Server Addr:%s\r\n",TcpIpSocket.FtpAddr);     //调试信息用户名称
						}
							
						SL_Memset(GprsRecvBuf,'\0',20); 
						if(GetDataStr(1,2,&GprsRecvBuf[29],GprsRecvBuf,GprsRecvLen) > 0)        //端口号 
						{
							TcpIpSocket.FtpPort = StrToDouble(GprsRecvBuf);
							SL_Print("FTP Server Port:%d\r\n",TcpIpSocket.FtpPort);     //
						}
							
						
						SL_Memset(GprsRecvBuf,'\0',20); 
						if(GetDataStr(2,3,&GprsRecvBuf[29],GprsRecvBuf,GprsRecvLen) > 0)       //用户名称 
						{
							SL_Memcpy(TcpIpSocket.FtpUserName,GprsRecvBuf,20);
							SL_Print("FTP UserName:%s\r\n",TcpIpSocket.FtpUserName);	 //
						}
						
						SL_Memset(GprsRecvBuf,'\0',20); 
						if(GetDataStr(3,4,&GprsRecvBuf[29],GprsRecvBuf,GprsRecvLen) > 0)       //用户密码
						{
							SL_Memcpy(TcpIpSocket.FtpUserPassd,GprsRecvBuf,20);
							SL_Print("FTP UserPassd:%s\r\n",TcpIpSocket.FtpUserPassd);  //
						}

						SL_Memset(GprsRecvBuf,'\0',20); 
						if(GetDataStr(4,5,&GprsRecvBuf[29],GprsRecvBuf,GprsRecvLen) > 0)       //用户密码 
						{
							SL_Memcpy(TcpIpSocket.FtpFileName,GprsRecvBuf,20);
							SL_Print("File Name:%s\r\n",TcpIpSocket.FtpFileName);
						}
						pUpgrade = GetUpgradeDataSpace();
						
						TcpIpSocket.LinkType = 1;
						TcpIpSocket.ResetLink = 10;       //10秒之后重启链接
						pUpgrade->UpgradeFlag = 1;
						
						break;
					case 0x000A:
						StrLen = BuildCompletePacket(GprsRecvBuf,1);       //正常数据包
						WriteCmdDataBuf(GprsRecvBuf,StrLen);               //
															  	 		   //还有设备状态
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
	else                                                    //测试模式配置
	{
		
	}
}




/******************************File End ***************************/


