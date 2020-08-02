

/**************************************************
**	File Name:
**	Time:
**************************************************/
#include "include.h"


#define bitset(var, bitno) ((var) |= 1UL << (bitno))

#define bitclr(var, bitno) ((var) &= ~(1UL << (bitno)))


extern HANDLE SysAppEnterTask;						//


/************ STM32F升级文件缓冲区***************/

static UpgradeStatusStr 		UpgradeStatus;                //远程升级控制区域

static UpgradeFileStr			*UpgradeData;                 //          

static UpgradeST86FileStr		*pUpgradeST86;			 	//指向要升级的数据缓冲区  

unsigned char 					*pUpgradeST86File;

/*****************数据缓冲区**********************/

static unsigned char 			TerRecvBuf[1024];           //接收缓冲区

static unsigned char 			TerSendBuf[1200];			//发送缓冲区

static unsigned char 			RunMode;   					//0:正常运行，1，升级单片机程序，2，单片机升级程序(ST87程序)

static unsigned char 			McuStatus;               	//单片机运行状态

static unsigned char			ConfigFlag;					//配置模式标志

/*******************本地全局变量********************/

static CanDataStr		    	CanData;

static CurrentFaultCodeStr		CurrentFaultCode;      //P平台当前故障码   故障码 各种机型通用

static TerStateStr				TerStateData;           //设备终端状态

//static TerSDCardStr				TerSDCardData;          //设备SD卡状态，

static SysTimeStr 				TerTime;                //设备时间  单片机时间



/*********************本地全局变量*************************/

static unsigned char 			CanProNumSendBig 	= 0;         //CAN协议号发送标识

static unsigned char 			GetStatusBig 		= 0;         //读设备状态数据  


unsigned char GetStatus[12]    				= {0x5F,0x5F,0x06,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x55,0xAA};


/*************************************************
**	函数名称:unsigned char ReadAccState(void)
**	功能描述:返回ACC状态
**************************************************/

unsigned char ReadAccState(void)
{
	return TerStateData.AccState;
}

/**************************************************
**	函数名称:
**	功能描述:
***************************************************/
CanDataStr *GetCanDataSpace(void)
{
	return &CanData;
}

/****************************************************
**	函数名称:TerStateStr	*GetTerStateDataSpace(void)
**	功能描述:
****************************************************/

TerStateStr	*GetTerStateDataSpace(void)
{
	return &TerStateData;
}


/********************************************************
**	函数名称:
**	功能描述:
********************************************************/

CurrentFaultCodeStr *GetCurrentFaultCodeSpace(void)
{
	return &CurrentFaultCode;
}



/******************************************************
**	函数名称:SysTimeStr *GetTerTimeSpace(void)
**	功能描述:返回设备时间空间
******************************************************/
SysTimeStr *GetTerTimeSpace(void)
{
	return &TerTime;
}



/**************************************************
**	函数名称:UpgradeStr *GetUpgradeDataSpace(void)
**	功能描述:
**************************************************/

UpgradeStatusStr *GetUpgradeDataSpace(void)
{
	return &UpgradeStatus;
}

/**************************************************
**	函数名称:
**	功能描述:
**************************************************/

unsigned char ReadRunMode(void)
{
	return RunMode;
}


/**************************************************
**	函数名称:
**	功能描述:
**************************************************/

void WriteRunMode(unsigned char n)
{
	if(n > 3)
		n = 0;
	RunMode = n;
}	


/****************************************************
**	函数名称:void SendTerSleepCmd(void)
**	功能描述:发送睡眠指令到MCU
****************************************************/

void SendTerSleepCmd(void)
{
	SysConfigStr   			*p;
	unsigned short 			tmp;
	int 					i;
	LongToChar     			TmpData;
	IntToChar				Tmp;
	
	tmp = 0;

	p = GetSysCfgSpace();
	TerSendBuf[0] = 0x5F;
	TerSendBuf[1] = 0x5F;
	TerSendBuf[2] = 0x0A;
	TerSendBuf[3] = 0x00;
	TerSendBuf[4] = 0x10;
	TerSendBuf[5] = 0x00;
	TerSendBuf[6] = 0x04;
	TerSendBuf[7] = 0x00;
	TmpData.LongLL 	= p->SleepTime;
	TerSendBuf[8] 		= TmpData.TTbyte[0];        
	TerSendBuf[9] 		=TmpData.TTbyte[1];         
	TerSendBuf[10] 		= TmpData.TTbyte[2];         
	TerSendBuf[11] 		= TmpData.TTbyte[3];  
	for(i = 0; i < 8;i++)
		tmp += TerSendBuf[i + 4];
	Tmp.IntII = tmp;
	TerSendBuf[12] = Tmp.TTbyte[0];
	TerSendBuf[13] = Tmp.TTbyte[1];  
	TerSendBuf[14] = 0x55;
	TerSendBuf[15] = 0xAA;
	SL_UartSendData(SL_UART_2,TerSendBuf,16);
}

/****************************************************
**	函数名称:void SendVerifyTimeCmd(SysTimeStr *pTime)
**	功能描述:发送时间校验消息到MCU
****************************************************/

void SendVerifyTimeCmd(SysTimeStr *pTime)
{
	LongToChar     TmpData;
	//IntToChar		Tmp;

	TerSendBuf[0] = 0x5F;
	TerSendBuf[1] = 0x5F;
	TerSendBuf[2] = 0x0A;
	TerSendBuf[3] = 0x00;
	TerSendBuf[4] = 0x20;
	TerSendBuf[5] = 0x00;
	TerSendBuf[6] = 0x04;
	TerSendBuf[7] = 0x00;
	TmpData.LongLL = pTime->TimeSec;
	TerSendBuf[8] = TmpData.TTbyte[0];
	TerSendBuf[9] = TmpData.TTbyte[1];           
	TerSendBuf[10] = TmpData.TTbyte[2];         
	TerSendBuf[11] = TmpData.TTbyte[3];        
	*(unsigned short int *)&TerSendBuf[12] = CheckSum16(TerSendBuf + 4,7);
	TerSendBuf[14] = 0x55;
	TerSendBuf[15] = 0xAA;

	SL_UartSendData(SL_UART_2, TerSendBuf,16);
}

/********************************************************
** 	函数名称:void SetTerCanProNum(void)
** 	功能描述:发送Can协议版本号到MCU
**	重新定义功能,发送基础信息到单片机
**	设备号、CAN协议号、休眠时间。目前先定义这三项
**	
********************************************************/

void SetTerCanProNum(void)
{
	LongToChar     	TmpData;
	SysConfigStr 	*pSysCfg;
	
	unsigned char i,len;

	len = 0;
	TerSendBuf[len++] = 0x5F;
	TerSendBuf[len++] = 0x5F;
	TerSendBuf[len++] = 0x10;
	TerSendBuf[len++] = 0x00;
	TerSendBuf[len++] = 0x14;
	TerSendBuf[len++] = 0x00;
	TerSendBuf[len++] = 0x0A;
	TerSendBuf[len++] = 0x00;
	pSysCfg = GetSysCfgSpace();
	TerSendBuf[len++] = pSysCfg->CanProtocolNum;    //CAN协议号  长度2字节
	TerSendBuf[len++] = 0xF0;

	TmpData.LongLL = pSysCfg->SleepTime;       		//休眠时间  长度4字节
	TerSendBuf[len++] = TmpData.TTbyte[0];			
	TerSendBuf[len++] = TmpData.TTbyte[1];			
	TerSendBuf[len++] = TmpData.TTbyte[2];			
	TerSendBuf[len++] = TmpData.TTbyte[3];			

	for(i = 0;i < 18;i++)
		TerSendBuf[len++] = pSysCfg->TerminalID[i];     //设备号
	
	*(unsigned short int *)&TerSendBuf[len] = CheckSum16(TerSendBuf + 4,len - 4);   //校验
	len += 2;
	TerSendBuf[len++] = 0x55;
	TerSendBuf[len++] = 0xAA;
	//SL_Print("Send Set Ter Pro Num.......\r\n");
	SL_UartSendData(SL_UART_2, TerSendBuf,len);    		//发送

}



/******************************************************
**	函数名称:
**	功能描述:
**	参数说明:
********************************************************/
void GetBinFile(unsigned short int n,unsigned short int fn)
{
    IntToChar       		TmpInt16;
	unsigned short int 		len = 0;
	
   	TerSendBuf[len++] = 0x5F;
	TerSendBuf[len++] = 0x5F;
	TerSendBuf[len++] = 0;//长度，总长度-4
	TerSendBuf[len++] = 0;
	TerSendBuf[len++] = 0x3F;
	TerSendBuf[len++] = 0;
	TerSendBuf[len++] = 0;//数据长度，总长度-8
	TerSendBuf[len++] = 0;

	TmpInt16.IntII = n;                     //请求   2,请求
    TerSendBuf[len++] = TmpInt16.TTbyte[0];
	TerSendBuf[len++] = TmpInt16.TTbyte[1];
	TmpInt16.IntII =fn;         
    TerSendBuf[len++] = TmpInt16.TTbyte[0];
	TerSendBuf[len++] = TmpInt16.TTbyte[1];
	
	TerSendBuf[len++] = 0;//校验
	TerSendBuf[len++] = 0;				//校验
	TerSendBuf[len++] = 0x55;
	TerSendBuf[len++] = 0xAA;

	TmpInt16.IntII = len - 6;								//总长度
	TerSendBuf[2] = TmpInt16.TTbyte[0];
	TerSendBuf[3] = TmpInt16.TTbyte[1];
	TmpInt16.IntII	= len - 12; 							//数据长度
	TerSendBuf[6]	=TmpInt16.TTbyte[0];
	TerSendBuf[7]	=TmpInt16.TTbyte[1];
   	//SL_MEMBLOCK(TerSendBuf,len,16);
    SL_UartSendData(SL_UART_2, TerSendBuf,len);  
}


/********************************************************
**	函数名称:static void UpgradeST86Mode(void)
**	功能描述:升级ST86模块应用程序
**	通过单片机升级ST86程序
********************************************************/

static unsigned char UpgradeST86Mode(unsigned char *p,unsigned short int len)
{
	IntToChar       		TmpInt16;
	LongToChar				TmpInt32;
	unsigned short int      FunID;
	int 					i;
					
	TmpInt16.TTbyte[0] = *(p + 0);
	TmpInt16.TTbyte[1] = *(p + 1);
	len = TmpInt16.IntII;               				//数据区域长度
	
	TmpInt16.TTbyte[0] = *(p + 2);
	TmpInt16.TTbyte[1] = *(p + 3);        
	FunID = TmpInt16.IntII;
	//SL_Print("Enter ST86 Upgrad Mode:  %d\r\n",FunID);
	//FunID = 4;
	switch(FunID)
	{
		case 1:
			pUpgradeST86 = (UpgradeST86FileStr *)SL_GetMemory(sizeof(UpgradeST86FileStr));
			if(pUpgradeST86 == NULL)
			{
				SL_Print(">App:Get UpgradeST86FileStr Mem Fail......\r\n");       //报告
				RunMode = 0;
				return 0;
			}
			
			pUpgradeST86File = (unsigned char *)GetSendQueueSpace();
			SL_Memset(pUpgradeST86,0,sizeof(UpgradeST86FileStr));    //清除生青岛的数据空间  
			SL_Memset(pUpgradeST86File,0,sizeof(SendQueueStr));    //清除生青岛的数据空间  
			TmpInt32.TTbyte[0] = *(p + 4); 
			TmpInt32.TTbyte[1] = *(p + 5); 
			TmpInt32.TTbyte[2] = *(p + 6); 
			TmpInt32.TTbyte[3] = *(p + 7); 
			pUpgradeST86->FileLen = TmpInt32.LongLL;		//升级文件总长度 

			TmpInt16.TTbyte[0] = *(p + 8); 
			TmpInt16.TTbyte[1] = *(p + 9); 
			
			pUpgradeST86->FileVerfy = TmpInt16.IntII;    //升级文件校验值 
			pUpgradeST86->CurrentFramNum++;  
		
			RunMode = 2;
			return 1;
		case 2:
			len -= 6;

			for(i = 0;i < len;i++)
			{
				*(pUpgradeST86File + pUpgradeST86->CurrentLen)= *(p + i + 6);
				pUpgradeST86->CurrentLen++;
			}	
			
			SL_Print("Recv Data:%d,%d,%d\r\n",len,pUpgradeST86->CurrentLen,pUpgradeST86->CurrentFramNum);
			pUpgradeST86->CurrentFramNum++;
			if(pUpgradeST86->CurrentLen >= pUpgradeST86->FileLen)
			{
				TmpInt16.IntII = CheckSum16(pUpgradeST86File,pUpgradeST86->FileLen);
				SL_Print("Recv Data Finsh.....%d,%d\r\n",TmpInt16.IntII,pUpgradeST86->FileVerfy);
				
				if(TmpInt16.IntII == pUpgradeST86->FileVerfy)
				{	
	    			return 2;
				}
				else
				{
					SL_FreeMemory(pUpgradeST86);       //释放内存
					RunMode = 0;
					return 0;
				}
			}
			
			return 1;
		default:
			RunMode = 0;
			return 0;
	}
	
}




/*********************************************************
**	函数名称:unsigned char ProTermRecvData(void)
**	功能描述:处理从STM32返回的数据
*********************************************************/

unsigned char ProTermRecvData(void)
{
	unsigned short int 		DataLen;
	unsigned short int 		tmp;
	unsigned char 			*p = NULL;
	IntToChar       		TmpInt16;
	SysConfigStr			*pSysCfg;             //配置 
	TcpIPSocketStr			*pTcpIp;
	unsigned char 			TmpStr[40];
	int i;

	if(RunMode >= 1)           //
		return 0; 

	SL_Memset(TerRecvBuf,0,sizeof(TerRecvBuf));
	DataLen = ReadComPkt(2,TerRecvBuf,sizeof(TerRecvBuf));    //读取要接收到的数据

	if(DataLen == 0)
		return 0;
	
	if(TerRecvBuf[0] != 0x5F || TerRecvBuf[1] != 0x5F)                //判断包头
		return 0;
	TmpInt16.TTbyte[0] = *(TerRecvBuf + 2);                    //数据长度，包括
	TmpInt16.TTbyte[1] = *(TerRecvBuf + 3);                    //消息体数据长度
	tmp = TmpInt16.IntII;                                      //长度
	
	if(*(TerRecvBuf + tmp + 4) != 0x55 || *(TerRecvBuf + tmp + 5) != 0xAA)   //判断包尾
		return 0;
	
	TmpInt16.TTbyte[0] = *(TerRecvBuf + tmp + 2);
	TmpInt16.TTbyte[1] = *(TerRecvBuf + tmp + 3);
	
	if(TmpInt16.IntII != CheckSum16(TerRecvBuf + 4,tmp - 2))
		return 0;
	
	pSysCfg = GetSysCfgSpace();
	TmpInt16.IntII = DataLen;
	DataLen = tmp;
	
	TmpInt16.TTbyte[0] = *(TerRecvBuf +  4);
	TmpInt16.TTbyte[1] = *(TerRecvBuf +  5);
	tmp = TmpInt16.IntII;    										//消息ID  
	
	switch(tmp)
	{
		case 0x0014:                                   	 			//设置CAN通讯协议，
			CanProNumSendBig = 1;
			break;
		case 0x0001:                                     			//解析设备状态  设备外部状态信息通过
			p = TerRecvBuf + 8;
			//SL_MEMBLOCK(p,DataLen,16);							//调试使用
			SL_Memcpy(&TerStateData,p,sizeof(TerStateData));        //单片机信息
			p += sizeof(TerStateData);
			McuStatus = *p;   						//单片机状态  
			p += 1;
			TerTime.TimeSec = TerStateData.McuTime;   //单片机时间
			TerTime.VerifyBit = 1;         			 //已经获取系统时间
			WriteSleepStatus(*p);     				 //睡眠状态    
			//SL_Print("The Mcu Status::%d\r\n",McuStatus);
			//一下为CAN数据
			SL_Memcpy(&CanData,p,sizeof(CanData));   				//解析Can数据
			p += sizeof(CanData);
			//SL_Print("The Mcu Status:%d,%d,%d,%d,",CanData.EngineRotate,CanData.WaterTemp,CanData.Rotate1,CanData.Rotate2);
			//SL_Print("%d,%d,%d,%d,%d,%d,\r\n",CanData.EngineWorkTime,CanData.WarnValue1,CanData.TravlledSpeed,CanData.TotalTravlled,CanData.OilConsumption,CanData.TatalOilConsumption);
			//SL_Print("%d,%d\r\n",CanData.EngineOil,CanData.FuelPercent);
			//一下开始读取故障码数据
			TmpInt16.TTbyte[0]			= *(p + 0);
			TmpInt16.TTbyte[1]			= *(p + 1);

			if(TmpInt16.IntII > 160)
				TmpInt16.IntII = 0;
			p += 2;
			for(i = 0;i < TmpInt16.IntII;i++)
			{
				CurrentFaultCode.FaultCode[i] = *(p + i);
			}
			CurrentFaultCode.FaultCodeNum = TmpInt16.IntII / 4;          //当前故障码数量
			//SL_Print("The Mcu Status::%d,%d,%d\r\n",TerStateData.McuFirNum,TerStateData.McuTime,TerStateData.CanConnect);
			break;
		case 0x00FE:                //有关配置的信息
			p = TerRecvBuf + 6;

			TmpInt16.TTbyte[0] = *p;
			TmpInt16.TTbyte[1] = *(p + 1);
			
			p = TerRecvBuf + 8;
			if(LookForStr(p,"AT+Test",DataLen - 6) >= 0)         			//进入生产测试
			{
				pSysCfg->CfgFlag = 0x56;
				//SL_Print("AT+Test\r\n");
				break;
			}
	
			if(LookForStr(p,"HOMER3ETESTOVER!",DataLen - 6) >= 0)  		//结束配置，回到正常模式
			{				  
				pSysCfg->CfgFlag = 0x55;
				SaveSysCfg();                                        			//保存配置    
				pTcpIp = GetTcpIpSocketSpce();
				pTcpIp->ResetLink = 1;                               			//重新链接
				break;
			}

			SL_Memset(TmpStr,'\0',12);
			SL_Memcpy(TmpStr,p,9);
			
			if(SL_Strcmp(TmpStr,"IIAP_ST87") == 0)
			{
				UpgradeST86Mode(p + 9,DataLen - 15);             //调用该函数
				break;
			}
			
			if((i = LookForStr(p,"HOMER3E:",DataLen - 6)) >= 0)   		//配置
			{
				p = p + i + 7;
				//SL_Print("Run BDWMODIF:\r\n");
				if(*(p) != ':')
					break;
				p++;
				for(i = 0;i < 16;i++)                            //设备号
					pSysCfg->TerminalID[i] = *(p + i);           //读取设备号       

				i = GetDataStr(1,2,p,TmpStr,DataLen);
				if(i > 0)
				{
					TmpStr[i] = '\0';
					SL_Memcpy(pSysCfg->ServerAddr,TmpStr,i + 1);          	//服务器地址  	
				}

				i = GetDataStr(2,3,p,TmpStr,DataLen); 
				//SL_Print(">App:The ServerPort %d\r\n",i);
				if(i > 0)
				{	
					TmpStr[i] = '\0';
					pSysCfg->ServerPort = StrToDouble(TmpStr);         			//服务器端口
				}
			
				i = GetDataStr(3,4,p,TmpStr,DataLen);
				//SL_Print(">App:The Sleep Time %d\r\n",i);
				if(i > 0)
				{
					TmpStr[i] = '\0';
					pSysCfg->SleepTime = StrToDouble(TmpStr);					//休眠时间
				}
							
				i = GetDataStr(4,5,p,TmpStr,DataLen);
				//SL_Print(">App:The DisTanceUpLoad %d\r\n",i);
				if(i > 0)
				{
					TmpStr[i] = '\0';
					pSysCfg->DistanceUpLoad = StrToDouble(TmpStr);				//定位报位
				}
			
				i = GetDataStr(5,6,p,TmpStr,DataLen);
				//SL_Print(">App:The UpLoadTime %d\r\n",i);
				if(i > 0)
				{
					TmpStr[i] = '\0';
					pSysCfg->TravelUpLoadTime = StrToDouble(TmpStr);			//定时报位时间
				}

				i = GetDataStr(6,7,p,TmpStr,DataLen);							//Can协议版本号    
			
				if(i > 0)
				{
					TmpStr[i] = '\0';
					pSysCfg->HardWareNum= StrToDouble(TmpStr);					//硬件版本号
				}

				i = GetDataStr(7,8,p,TmpStr,DataLen);                   		 	//Can协议版本号    
				if(i > 0)
				{
					TmpStr[i] = '\0';
					pSysCfg->CanProtocolNum = StrToDouble(TmpStr);   			//Can协议版本号  
					if(pSysCfg->CanProtocolNum == 0x03)                    		//3E设备 水稻机 
					{
						pSysCfg->CarType = 0x09;                                //水稻机类型码0x09 
					}
					else if(pSysCfg->CanProtocolNum == 0x04)                    //3E设备 新国三M平台拖拉机
					{
						pSysCfg->CarType = 0x03;                       
					}
					else if(pSysCfg->CanProtocolNum == 0x07)					//3E设备 老国三 M平台拖拉机
					{
						pSysCfg->CarType = 0x0D;		                        //3E设备，
					}
					else if(pSysCfg->CanProtocolNum == 0x08)					//3E设备 老国三 水稻机
					{
						pSysCfg->CarType = 0x0F;	                            //车型	
					}
					else
					{
						pSysCfg->CanProtocolNum = 0x03;                        //默认 老国三水稻机
						pSysCfg->CarType = 0x09; 
					}
				}

				i = GetDataStr(8,9,p,TmpStr,DataLen);
				if(i > 0)
				{
					SL_Memcpy(pSysCfg->TerApn,TmpStr,i);           			//Apn 接入点方式
					pSysCfg->TerApn[i] = '\0';
				}
			}
			break;
		default:
			return 0;
					
	}
	return 1;
}





/**************************************************
**	函数名称
**	功能描述:
***************************************************/
void SendTerminalInfo(unsigned short int id)
{
	SysConfigStr				*pSys;
	unsigned short int 			len;
	unsigned int 				tmp;
	TcpIPSocketStr 				*pTcpIp;
	IntToChar					TmpInt16;              //临时数据 
	unsigned char 				TmpStr[12];
	GNSSSTR 					*pGnss;                //定位信息
	SysTimeStr					Time;                //时间
	
	pSys = GetSysCfgSpace();
	len = 0;
	pTcpIp = GetTcpIpSocketSpce();
	
	TerSendBuf[len++] = 0x5F;
	TerSendBuf[len++] = 0x5F;
	TerSendBuf[len++] = 0;//长度，总长度-4
	TerSendBuf[len++] = 0;
	TmpInt16.IntII = id;
	TerSendBuf[len++] = TmpInt16.TTbyte[0];
	TerSendBuf[len++] = TmpInt16.TTbyte[1];
	TerSendBuf[len++] = 0;						//数据长度，总长度-8
	TerSendBuf[len++] = 0;
	TerSendBuf[len++] = pSys->CfgFlag;    //配置标志  
	TerSendBuf[len++] = ConfigFlag;      //默认为0
					
	TerSendBuf[len++] = '\r';
	TerSendBuf[len++] = '\n';
	SL_Memcpy(TerSendBuf + len,"发送GPRS调试数据",sizeof("发送GPRS调试数据"));   //这两句代码要和老吴确认一下
	len += sizeof("发送GPRS调试数据");
	TerSendBuf[len++] = 'h';
	TerSendBuf[len++] = 'o';
	TerSendBuf[len++] = 'm';
	TerSendBuf[len++] = 'e';
	TerSendBuf[len++] = 'r';
	TerSendBuf[len++] = '3';
	TerSendBuf[len++] = 'e';
	TerSendBuf[len++] = ':';

	//设备号
	for(tmp = 0;tmp < 16;tmp++)
		TerSendBuf[len++] = pSys->TerminalID[tmp];  
	TerSendBuf[len++] = ',';
			
	//SIM卡ICCID号码
	for(tmp = 0;tmp < 20;tmp++)
		TerSendBuf[len++] = pTcpIp->SimIccId[tmp]; 
	TerSendBuf[len++] = ',';
			
	//服务器地址:
	TmpInt16.IntII = SL_Strlen(pSys->ServerAddr);
	SL_Memcpy(TerSendBuf + len,pSys->ServerAddr,TmpInt16.IntII);
	len += TmpInt16.IntII;
	TerSendBuf[len++] = ',';
	
	//服务器端口
	SL_itoa(pSys->ServerPort,TmpStr,10);
	for(tmp = 0;tmp < SL_Strlen(TmpStr);tmp++)			  //服务器端口
		TerSendBuf[len++] = TmpStr[tmp];
	TerSendBuf[len++] = ',';

	//ANP接入点
	tmp = SL_Strlen(pSys->TerApn);
	SL_Memcpy(TerSendBuf + len,pSys->TerApn,tmp);        // APNGprs接入点  
	len += tmp;
	TerSendBuf[len++] = ',';                             //逗号3

	//定位数据  定位状态
	pGnss = GetGnssDataSpace();
			
	if(pGnss->status == 'A')                             //定位标识
	{
		TerSendBuf[len] = '8';
		TerSendBuf[len + 1] = '0';
	}
	else
	{
		TerSendBuf[len] = '0';
		TerSendBuf[len + 1] = '0';
	}
	len += 2;
			
	//定位维度
	tmp = pGnss->latitude * 1000000;                   //维度
	SL_Memset(TmpStr,0x30,12);
	UInt32ToStr(tmp,TmpStr,12);
	SL_Memcpy(TerSendBuf + len,TmpStr,9);              //复制到发送缓冲区
	len += 9;
	//定位经度
	tmp = pGnss->longitude * 1000000;                  //经度
	SL_Memset(TmpStr,0x30,12);
	UInt32ToStr(tmp,TmpStr,12);                        
	SL_Memcpy(TerSendBuf + len,TmpStr,9);              //复制到发送缓冲区
	len += 9;

	//定位速度
	tmp = pGnss->speed * 10;
	//SL_Print(">App:The :%d\r\n",i);
	SL_Memset(TmpStr,0x30,12);
	UInt32ToStr(tmp,TmpStr,12);
	for(tmp = 0;tmp < 4;tmp++)
		TerSendBuf[len++] = TmpStr[4 -tmp -1];

	//航向角度
	tmp = pGnss->azimuth * 100;
	SL_Memset(TmpStr,0x30,12);
	UInt32ToStr(tmp,TmpStr,12);
	for(tmp = 0;tmp < 4;tmp++)
		TerSendBuf[len++] = TmpStr[4 -tmp -1];
			
	//系统时间
	Time =  UnixToCalendar(pGnss->gnss_time - 28800);
	CharToStr(Time.Year,TmpStr,2);                  //GPS时间-年
	SL_Memcpy(TerSendBuf + len,TmpStr,2);
	len += 2;
			
	CharToStr(Time.Mon,TmpStr,2);                  //GPS时间-月
	SL_Memcpy(TerSendBuf + len,TmpStr,2);
	len += 2;

	CharToStr(Time.MDay,TmpStr,2);                  //GPS时间-日
	SL_Memcpy(TerSendBuf + len,TmpStr,2);
	len += 2;

	CharToStr(Time.Hour,TmpStr,2);                  //GPS时间-时
	SL_Memcpy(TerSendBuf + len,TmpStr,2);
	len += 2;

	CharToStr(Time.Min,TmpStr,2);                  //GPS时间-分
	SL_Memcpy(TerSendBuf + len,TmpStr,2);
	len += 2;

	CharToStr(Time.Sec,TmpStr,2);                  //GPS时间-秒
	SL_Memcpy(TerSendBuf + len,TmpStr,2);
	len += 2;
	TerSendBuf[len++] = ',';
					
	SL_Memset(TmpStr,0x30,12);
	CharToStr(pGnss->satellite_num,TmpStr,2);       //可用卫星数量
	SL_Memcpy(TerSendBuf + len,TmpStr,2);
	len += 2;
	TerSendBuf[len++] = ',';

	SL_Memset(TmpStr,0x30,12);
	CharToStr(pTcpIp->GsmSign,TmpStr,2);
	SL_Memcpy(TerSendBuf + len,TmpStr,2);           //信号值
	len += 2;
	TerSendBuf[len++] = ',';

	//TerStateData.PowVol = 120;
	SL_Memset(TmpStr,0x30,12);
	tmp = UInt32ToStr(TerStateData.PowVol,TmpStr,12);
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);           //电源电压
	len += tmp;
	TerSendBuf[len++] = ',';
					
	SL_Memset(TmpStr,0x30,12);
	tmp = UInt32ToStr(TerStateData.Battery,TmpStr,12);
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);           //电池电压 
	len += tmp;
	TerSendBuf[len++] = ',';
			
	TerSendBuf[len++] = TerStateData.AccState + 0x30;   //ACC 状态
	TerSendBuf[len++] = ',';
	if(pTcpIp->LinkState > 0)
		TerSendBuf[len++] =  0x32;      //GSM网络状态  0:未注册;1:已注册;2:已连接。
	else
		TerSendBuf[len++] =  0x30;
	TerSendBuf[len++] = ',';

	tmp = GetVerNum();
	UInt32ToStr(tmp,TmpStr,12); 								 //固件协议版本号
	SL_Memcpy(TerSendBuf + len,TmpStr,2);
	len += 2;
	TerSendBuf[len++] = ',';
			
	SL_Memset(TmpStr,0x30,12);
	tmp = UInt32ToStr(pSys->SleepTime,TmpStr,12);	   		//睡眠时间
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
	len += tmp;
	TerSendBuf[len++] = ',';
					
	SL_Memset(TmpStr,0x30,12);
	tmp = UInt32ToStr(pSys->DistanceUpLoad,TmpStr,12);		//报位置距离
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
	len += tmp;
	TerSendBuf[len++] = ',';
					
	SL_Memset(TmpStr,0x30,12);
	tmp = UInt32ToStr(pSys->TravelUpLoadTime,TmpStr,12);	//行驶报位时间
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
	len += tmp;
	TerSendBuf[len++] = ',';
			
	//pSysCfg->HardWareNum = 12;                            // 硬件版本号，
	SL_Memset(TmpStr,0x30,12);
	tmp = UInt32ToStr(pSys->HardWareNum,TmpStr,12);	  	//硬件版本号
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
	len += tmp;
	TerSendBuf[len++] = ',';
					
	tmp = UInt32ToStr(pSys->CanProtocolNum,TmpStr,12);		//Can协议版本号
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
	len += tmp;
					
	TerSendBuf[len++] = ',';
	if(TerStateData.CanConnect> 0)                       //
		TerSendBuf[len++] = 0x30;
	else
		TerSendBuf[len++] = 0x31;
					
	TerSendBuf[len++] = ',';
	TerSendBuf[len++] = '0';   //SD卡状态  ,默认传送0
	TerSendBuf[len++] = ',';

	TerSendBuf[len++] = TerStateData.McuFirNum / 10 + 0x30;   //单片机版本号--主版本号
	TerSendBuf[len++] = '.'; 
	TerSendBuf[len++] = TerStateData.McuFirNum % 10 + 0x30;   //
	TerSendBuf[len++] = ',';
					
	TerSendBuf[len++] = 0x0D;								  //回车
	TerSendBuf[len++] = 0x0A;
									
	TerSendBuf[len] = 0x00;
	TerSendBuf[len + 1] = 0x00;
	TerSendBuf[len + 2] = 0x55;
	TerSendBuf[len + 3] = 0xAA;
	len += 4;
	TmpInt16.IntII = len - 6;								//总长度
	TerSendBuf[2] = TmpInt16.TTbyte[0];
	TerSendBuf[3] = TmpInt16.TTbyte[1];
	TmpInt16.IntII	= len - 12; 							//数据长度
	TerSendBuf[6]	=TmpInt16.TTbyte[0];
	TerSendBuf[7]	=TmpInt16.TTbyte[1];
	SL_UartSendData(SL_UART_2,TerSendBuf,len);	
	//SL_Print(">App:Send Debug Data:  %d\r\n",pSysCfg->CfgFlag);
}



/**********************************************************
**	函数名称:
**	功能描述:负责与STM32交换数据  发送数据
**	三种运行模式:
**	1、正常模式，主动从单片机读取数据，例如CAN数据、状态信息等。
**	2、升级单片机程序，通过FPT方式下载单片机升级文件，分包传送到单片机。
**	3、升级ST86/ST87应用程序，设备通过串口3获取ST87/86应用程序文件，分包发送到ST87/86模块。
**	4、青岛福田使用串口2
**********************************************************/

void ProTerminalData(void)
{	
	static unsigned char 		step = 0,step1 = 0,step2 = 0;
	static struct stopwatch16 	sw,sw1;

	unsigned short int 			len;
	int 						tmp;
	SysConfigStr 				*pSys;                 //配置
	IntToChar					TmpInt16;              //临时数据 
	LongToChar					TmpInt32; 			   //临时数据
	unsigned char 				TmpStr[12];			   //
	SysTimeStr					*pTime;                //时间
	TcpIPSocketStr				*pTcpIp;

		
	pTime = GetSetTimeSpace();
	
	switch(RunMode)
	{
		case 0:
			switch(step)
			{
				case 0:                             //发送CAN协议号  
					SetTerCanProNum();              //发送CAN协议号   
					CanProNumSendBig = 0;
					init_stopwatch16(&sw);  
					step++;
					break;
				case 1:
					if(CanProNumSendBig == 0 && read_stopwatch16_value(&sw) < 20)      //延时一段时间，其他 
						break;
					step++;     
					break;
				case 2:
					init_stopwatch16(&sw); 
					step++;
					break;
				case 3:
					if(read_stopwatch16_value(&sw) < 19)                       	     //延时250
						break;
					SL_UartSendData(SL_UART_2,GetStatus,sizeof(GetStatus));    		//读取设备状态信息 主要是ACC
					GetStatusBig = 0;
					init_stopwatch16(&sw); 
					step++;
					break;
				case 4:
					if(GetStatusBig == 0 && read_stopwatch16_value(&sw) < 10)   	//延时 500ms  等待
						break;
					
					if(pTime->VerifyBit > 0)    	//如果需要
					{
						step = 6;                 	//需要去校时  
						init_stopwatch16(&sw);
						pTime->VerifyBit = 0;
						break;
					}
					if(McuStatus == 3)				//测试模式
					{
						step = 7;
						init_stopwatch16(&sw);
						break;
					}
					pSys = GetSysCfgSpace();
					//SL_Print("THe The Mcu Status:%d\r\n",McuStatus);
					if(McuStatus == 2 || pSys->CfgFlag == 0x56)				//
					{
						step++; 
						if(ConfigFlag == 0)
							ConfigFlag = 1;
						init_stopwatch16(&sw);		//
						break;
					}
					
					step = 2;                         //进入配置模式
					init_stopwatch16(&sw);
					break;
				case 5:                               //发送配置信息
					if(read_stopwatch16_value(&sw) < 10)
						break;
					//发送设备信息
					SendTerminalInfo(0x00FE);			//配置信息
					step = 2;   								//
					init_stopwatch16(&sw);
					break;
				case 6:
					if(read_stopwatch16_value(&sw) < 5)
						break;
					step = 2;
					SendVerifyTimeCmd(pTime);              		//发送校准时间  
					init_stopwatch16(&sw);
					break;
				case 7:
					if(read_stopwatch16_value(&sw) < 5)
						break;
					SendTerminalInfo(0x00FD);   				 //发送检测信息  发送测试信息
					step = 2;
					break;
				default:
					step = 0;
					break;
			}
			break;
		case 1:                                       //升级STM32 单片机   
			switch(step1)
			{
				case 0:                                //发送升级命令  向单片机发送升级指令    
					UpgradeData = (UpgradeFileStr *)SL_GetMemory(sizeof(UpgradeFileStr));
					if(UpgradeData == NULL)
					{
						SL_Print(">App:Get Sys Mem Fail......\r\n");       //报告
						RunMode = 0;
						break;
					}
					
					pTcpIp = GetTcpIpSocketSpce();
					UpgradeData->FileTotalLen = pTcpIp->FtpFileLen;
					UpgradeData->FileVerfy = CheckSum16(pTcpIp->FtpFileSapce,pTcpIp->FtpFileLen);   //计算校验  
					UpgradeData->FileTotalFram = UpgradeData->FileTotalLen / 1024;
					
					for(tmp = 0;tmp < UpgradeData->FileTotalFram;tmp++)
					{
						UpgradeData->FileLen[tmp] = 1024;       //数据包长度
						SL_Memcpy(UpgradeData->FileBuf[tmp],(pTcpIp->FtpFileSapce + 1024 * tmp),UpgradeData->FileLen[tmp]);  //
					}
					tmp = UpgradeData->FileTotalLen % 1024;
					
					if(tmp > 0)
					{
						UpgradeData->FileLen[UpgradeData->FileTotalFram] = tmp;
						SL_Memcpy(UpgradeData->FileBuf[UpgradeData->FileTotalFram],(pTcpIp->FtpFileSapce + 1024 * UpgradeData->FileTotalFram),UpgradeData->FileLen[UpgradeData->FileTotalFram]);
						UpgradeData->FileTotalFram++;
					}
					
					SL_Print(">App:Load Stm32 FirWare Finsh.....%d,%d,%d\r\n",UpgradeData->FileTotalLen,UpgradeData->FileTotalFram,UpgradeData->FileVerfy);     //输出调试信息  
					//SL_MEMBLOCK(UpgradeData->FileBuf[UpgradeData->FileTotalFram - 1],UpgradeData->FileLen[UpgradeData->FileTotalFram - 1],16);
					
					len = 0;
					SL_Memset(TerSendBuf,0,sizeof(TerSendBuf));          //
					TerSendBuf[len++] = 0x5F;
					TerSendBuf[len++] = 0x5F;
					TerSendBuf[len++] = 0x0E;
					TerSendBuf[len++] = 0x00;            //长度
					TerSendBuf[len++] = 0x1F;
					TerSendBuf[len++] = 0x00;
					TerSendBuf[len++] = 0x08;
					TerSendBuf[len++] = 0x00;           //
					TerSendBuf[len++] = 0x01;
					TerSendBuf[len++] = 0x00;           //数据区域长度 
					TmpInt32.LongLL = UpgradeData->FileTotalLen;
					TerSendBuf[len++] = TmpInt32.TTbyte[0];
					TerSendBuf[len++] = TmpInt32.TTbyte[1];
					TerSendBuf[len++] = TmpInt32.TTbyte[2];
					TerSendBuf[len++] = TmpInt32.TTbyte[3];
					TmpInt16.IntII = UpgradeData->FileVerfy;
					TerSendBuf[len++] = TmpInt16.TTbyte[0];
					TerSendBuf[len++] = TmpInt16.TTbyte[1];
					TmpInt16.IntII = CheckSum16(TerSendBuf,len - 4);
					TerSendBuf[len++] = TmpInt16.TTbyte[0];
					TerSendBuf[len++] = TmpInt16.TTbyte[1];
					TerSendBuf[len++] = 0x55;
					TerSendBuf[len++] = 0xAA;
					SL_UartSendData(SL_UART_2,TerSendBuf,len);      //发送出去 
					init_stopwatch16(&sw1);
					step1++;
					break;
				case 1:                                                //开始升级  升级的过程中需要添加  
					if(read_stopwatch16_value(&sw1) > 9600)        //1分钟内要升级结束，却不结束
					{
						SL_FreeMemory(UpgradeData);
						RunMode = 0;         
						UpgradeStatus.UpgradeStatus = 1;     //升级失败 
						//保存升级状态 
						return;
					}
					tmp = ReadComPkt(2,TerRecvBuf,sizeof(TerRecvBuf));    //
					if(tmp == 0)
						return ;
					if(TerRecvBuf[0] != 0x5F || TerRecvBuf[1] != 0x5F)                //判断包头
						return ;
					
					TmpInt16.TTbyte[0] = *(TerRecvBuf + 2);
					TmpInt16.TTbyte[1] = *(TerRecvBuf + 3);
					len = TmpInt16.IntII;
	
					if(*(TerRecvBuf + len + 4) != 0x55 || *(TerRecvBuf + len + 5) != 0xAA)   //判断包尾
						return;
					
					TmpInt16.TTbyte[0] = *(TerRecvBuf + len + 2);
					TmpInt16.TTbyte[1] = *(TerRecvBuf + len + 3);
					tmp = TmpInt16.IntII;
					if(tmp != CheckSum16(TerRecvBuf + 4,len - 2))
						return;

					TmpInt16.TTbyte[0] = *(TerRecvBuf + 4);
					TmpInt16.TTbyte[1] = *(TerRecvBuf + 5);
					if(TmpInt16.IntII != 0x001F)
						return;
					
					TmpInt16.TTbyte[0] = TerRecvBuf[10];      //数据帧号码
					TmpInt16.TTbyte[1] = TerRecvBuf[11];
				
					UpgradeData->FileCurrentFram = TmpInt16.IntII - 1;                  //当前数据帧号码  
					SL_Print("The FileCurrentFram Num:%d\r\n",UpgradeData->FileCurrentFram);
					
					len = 0;
					SL_Memset(TerSendBuf,0,sizeof(TerSendBuf));    //清空数据
					TerSendBuf[len++] = 0x5F;
					TerSendBuf[len++] = 0x5F;
					TmpInt16.IntII = UpgradeData->FileLen[UpgradeData->FileCurrentFram] + 10;
					TerSendBuf[len++] = TmpInt16.TTbyte[0];         //总长度
					TerSendBuf[len++] =	TmpInt16.TTbyte[1];
					TerSendBuf[len++] = 0x1F;         //数据类型码
					TerSendBuf[len++] = 0x00;
					TmpInt16.IntII    = UpgradeData->FileLen[UpgradeData->FileCurrentFram] + 6;   //长度加
					TerSendBuf[len++] = TmpInt16.TTbyte[0];
					TerSendBuf[len++] = TmpInt16.TTbyte[1];       //数据区域长度   
					
					TerSendBuf[len++] = 0x02;
					TerSendBuf[len++] = 0x00;
					TerSendBuf[len++] = 0x00;
					TerSendBuf[len++] = 0x00;
					
					SL_Memcpy(&TerSendBuf[len],UpgradeData->FileBuf[UpgradeData->FileCurrentFram],UpgradeData->FileLen[UpgradeData->FileCurrentFram]);
					
					len += UpgradeData->FileLen[UpgradeData->FileCurrentFram];
					//SL_Print("The FileCurrentFram Len:%d\r\n",UpgradeData->FileLen[UpgradeData->FileCurrentFram]);

					TmpInt16.IntII= CheckSum16(TerSendBuf + 12,len - 12);            //校验 
					
					TerSendBuf[len++] = TmpInt16.TTbyte[0];
					TerSendBuf[len++] = TmpInt16.TTbyte[1];
					//SL_Print("CheckSum16: %d\r\n",TmpInt16.IntII);
					TerSendBuf[len++] = 0x55;
					TerSendBuf[len++] = 0xAA;
					SL_UartSendData(SL_UART_2,TerSendBuf,len);      //
					//SL_Print("Send Data Len %d\r\n",len);
					if(UpgradeData->FileCurrentFram >= UpgradeData->FileTotalFram)
					{
						SL_FreeMemory(UpgradeData);
						RunMode = 0;         
						UpgradeStatus.UpgradeStatus = 0;     //升级完成
						//保存升级状态 
						return;
						
					}
					break; 
				default:
					step1 = 0;
					break;
			}
			break;
		case 2:                                      //单片机升级ST86/ST87程序。
			switch(step2)
			{
				case 0:
					init_stopwatch16(&sw);
					GetBinFile(2,pUpgradeST86->CurrentFramNum);				//请求数据包 
					step2++;
					break;
				case 1:
					SL_Memset(TerRecvBuf,0,sizeof(TerRecvBuf));
					TmpInt16.IntII = ReadComPkt(2,TerRecvBuf,sizeof(TerRecvBuf));    //读取要接收到的数据
					if(read_stopwatch16_value(&sw) < 15 && TmpInt16.IntII == 0)
						break;

					if(TerRecvBuf[0] != 0x5F || TerRecvBuf[1] != 0x5F)                //判断包头
					{
						SL_Memset(pUpgradeST86File,0,sizeof(SendQueueStr));    //清除生青岛的数据空间  
						WriteRunMode(0);      //返回到正常运行模式
						break;
					}
					tmp = 0;
					TmpInt16.TTbyte[0] = *(TerRecvBuf + 2);                    //数据长度，包括
					TmpInt16.TTbyte[1] = *(TerRecvBuf + 3);                    //消息体数据长度
					tmp = TmpInt16.IntII;                                      //长度
			
					if(*(TerRecvBuf + tmp + 4) != 0x55 || *(TerRecvBuf + tmp + 5) != 0xAA)   //判断包尾
					{
						SL_Memset(pUpgradeST86File,0,sizeof(SendQueueStr));    //清除生青岛的数据空间  
						WriteRunMode(0);       //返回到正常运行模式
						break;
					}
					
					TmpInt16.TTbyte[0] = *(TerRecvBuf + tmp + 2);
					TmpInt16.TTbyte[1] = *(TerRecvBuf + tmp + 3);
	
					if(TmpInt16.IntII != CheckSum16(TerRecvBuf + 4,tmp - 2))
					{
						SL_Memset(pUpgradeST86File,0,sizeof(SendQueueStr));    //清除生青岛的数据空间  
						WriteRunMode(0);     //返回到正常运行模式
						break;
					}
					
					//正式接收数据    TmpInt16.IntII作为接收到的数据的数据长度使用
					SL_Memset(TmpStr,'\0',12);
					SL_Memcpy(TmpStr,&TerRecvBuf[8],9);
					if(SL_Strcmp(TmpStr,"IIAP_ST87") == 0)
					{
						//SL_Print("Run The IIIIII\r\n");
						if(UpgradeST86Mode(&TerRecvBuf[8] + 9,tmp) == 2)        //数据 
						{	
							GetBinFile(5,0);//发送升级完成命令
							init_stopwatch16(&sw);
							step2 = 3;
							break;
						}
					}
					init_stopwatch16(&sw);
					step2++;
					break;
				case 2:
					if(read_stopwatch16_value(&sw) < 5)
						break;
					step2 = 0;
					break;
  				case 3:                   					//升级完成
  					if(read_stopwatch16_value(&sw) < 40)   //延时1秒后重启
						break;
					TmpInt32.LongLL = SL_AppUpdateInit(pUpgradeST86File, pUpgradeST86->FileLen);
					
                    SL_Print("---> SL APP update init ret:%d\n", TmpInt32.LongLL);
					
					
                    if (SL_APP_UPDATE_OK != TmpInt32.LongLL)
                    {
                        SL_Print("---> SL App update condition check fail, do not update!\n");
                    }
                    else
                    {
                        SL_Print("---> SL App update condition check ok, reset system and update ... \n");
						SL_Sleep(5000);				//等待10秒
						SL_HstSendEvt(0x20161034);      //更新完成   重启设备
                        SL_PowerDown(0); 				//重启
                    }
                    RunMode = 0;
					break;
				default:
					step2 = 0;
					break;
			}
			break;
	}
	
}












/**************************File End*************************/


