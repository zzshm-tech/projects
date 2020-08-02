

/**************************************************
**	File Name:
**	Time:
**************************************************/
#include "include.h"


#define bitset(var, bitno) ((var) |= 1UL << (bitno))

#define bitclr(var, bitno) ((var) &= ~(1UL << (bitno)))


extern HANDLE SysAppEnterTask;						//


/************ STM32F�����ļ�������***************/

static UpgradeStatusStr 		UpgradeStatus;                //

static UpgradeFileStr			*UpgradeData;                 //          

static UpgradeST86FileStr		*pUpgradeST86;			 	//  

unsigned char 					*pUpgradeST86File;

/*****************���ݻ�����**********************/

static unsigned char 			TerRecvBuf[1024];           //

static unsigned char 			TerSendBuf[1200];			//

static unsigned char 			RunMode;   					//

static unsigned char 			McuStatus;               	//

static unsigned char			ConfigFlag;					//

/*******************����ȫ�ֱ���********************/

static CanDataStr		    	CanData;

static TerStateStr				TerStateData;           //

static SysTimeStr 				TerTime;                //

static LockCarStr               LockCar;               //


/**********************************************/

static unsigned char 			CanProNumSendBig 	= 0;         //

static unsigned char 			GetStatusBig 		= 0;         // 

static unsigned char			MonstateTmp = 0;

unsigned char GetStatus[12]    	= {0x5F,0x5F,0x06,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x55,0xAA};


/*****************************
**	
*****************************/

void SendGetStatusCmd(void)
{
	IntToChar				Tmp;
	GNSSSTR *pGnss;
	TcpIPSocketStr *pTcp;

	TerSendBuf[0] = 0x5F;
	TerSendBuf[1] = 0x5F;
	
	TerSendBuf[2] = 0x09;  //
	TerSendBuf[3] = 0x00;
	
	TerSendBuf[4] = 0x01;  //
	TerSendBuf[5] = 0x00;
	
	TerSendBuf[6] = 0x03;   //
	TerSendBuf[7] = 0x00;

	pTcp = GetTcpIpSocketSpce();
	TerSendBuf[8] 		= pTcp->GsmSign;     //
	//SL_Print("The Gsm Sign Value:%d\r\n",TerSendBuf[8]);
	pGnss = GetGnssDataSpace();
	TerSendBuf[9] 		= pGnss->satellite_view_num;     //  
	TerSendBuf[10] 	= pGnss->satellite_view_num1;   	 //      
	TerSendBuf[11] = pTcp->GprsLinkState;				
	//SL_Print("The Gsm Sign Value:%d\r\n",TerSendBuf[11]);
	Tmp.IntII = Tmp.IntII = CheckSum16(TerSendBuf + 4,8);  //
	TerSendBuf[12] = Tmp.TTbyte[0];
	TerSendBuf[13] = Tmp.TTbyte[1]; 
	
	TerSendBuf[14] = 0x55;
	TerSendBuf[15] = 0xAA;
	SL_UartSendData(SL_UART_2,TerSendBuf,16);
}




/*************************************************
**	
**************************************************/

unsigned char ReadAccState(void)
{

	unsigned char tmp;
	unsigned int tmp1;

	tmp1 = GetWarningValue();
	tmp =tmp1 & 0x01;      //
	if(TerStateData.AccState == 0)
		return 0;   			//
	else if(tmp == 0 && TerStateData.AccState == 1)
		return 1;  				//
	
	return 2;  					//
	
}

/**************************************************
**	
***************************************************/
CanDataStr *GetCanDataSpace(void)
{
	return &CanData;
}

/****************************************************
**	
****************************************************/

TerStateStr	*GetTerStateDataSpace(void)
{
	return &TerStateData;
}



/******************************************************
**	
******************************************************/
SysTimeStr *GetTerTimeSpace(void)
{
	return &TerTime;
}

/****************************************************
**	
****************************************************/
LockCarStr *GetLockCarSpace(void)
{
	return &LockCar;  
}


/***************************************************
**	
***************************************************/

static void SendLockVehicleCmd(void)
{
	IntToChar Tmp;
	
	TerSendBuf[0] = 0x5F;
	TerSendBuf[1] = 0x5F;
	TerSendBuf[2] = 0x07;
	TerSendBuf[3] = 0x00;
	TerSendBuf[4] = 0x22;
	TerSendBuf[5] = 0x00;
	TerSendBuf[6] = 0x01;
	TerSendBuf[7] = 0x00;
	
	TerSendBuf[8] = LockCar.LockCarCmd;   //
	TerSendBuf[9] = LockCar.LockCarRank;   //
	Tmp.IntII = CheckSum16(TerSendBuf + 4,4);
	TerSendBuf[10] = Tmp.TTbyte[0];
	TerSendBuf[11] = Tmp.TTbyte[1];  
	TerSendBuf[12] = 0x55;
	TerSendBuf[13] = 0xAA;
	SL_UartSendData(SL_UART_2,TerSendBuf,14);
}




/***************************************************
**	
***************************************************/

static void SendConCmd(void)
{
	IntToChar Tmp;
	
	TerSendBuf[0] = 0x5F;
	TerSendBuf[1] = 0x5F;
	TerSendBuf[2] = 0x07;
	TerSendBuf[3] = 0x00;
	TerSendBuf[4] = 0x23;
	TerSendBuf[5] = 0x00;
	TerSendBuf[6] = 0x01;
	TerSendBuf[7] = 0x00;
	
	TerSendBuf[8] = LockCar.MonCmdByte;           //
	TerSendBuf[9] = LockCar.GpsID[0];
	TerSendBuf[10] = LockCar.GpsID[1];
	TerSendBuf[11] = LockCar.GpsID[2];

	TerSendBuf[12] = LockCar.Secret[0];
	TerSendBuf[13] = LockCar.Secret[1];
	TerSendBuf[14] = LockCar.Secret[2];

	Tmp.IntII = CheckSum16(TerSendBuf + 4,10);
	TerSendBuf[15] = Tmp.TTbyte[0];
	TerSendBuf[16] = Tmp.TTbyte[1];  
	TerSendBuf[17] = 0x55;
	TerSendBuf[18] = 0xAA;
	SL_UartSendData(SL_UART_2,TerSendBuf,19);
}




/***************************************************
**	
***************************************************/

static void SendReplaceDevCmd(void)
{
	IntToChar Tmp;
	
	TerSendBuf[0] = 0x5F;
	TerSendBuf[1] = 0x5F;
	TerSendBuf[2] = 0x05;
	TerSendBuf[3] = 0x00;
	TerSendBuf[4] = 0x24;
	TerSendBuf[5] = 0x00;
	TerSendBuf[6] = 0x01;
	TerSendBuf[7] = 0x00;
	
	TerSendBuf[8] = 0x01;           //


	Tmp.IntII = CheckSum16(TerSendBuf + 4,5);
	TerSendBuf[9] = Tmp.TTbyte[0];
	TerSendBuf[10] = Tmp.TTbyte[1];  
	TerSendBuf[11] = 0x55;
	TerSendBuf[12] = 0xAA;
	SL_UartSendData(SL_UART_2,TerSendBuf,13);
}



/***************************************************
**	
***************************************************/

static void SendConfigOverStatus(void)
{
	IntToChar Tmp;

	TerSendBuf[0] = 0x5F;
	TerSendBuf[1] = 0x5F;
	TerSendBuf[2] = 0x05;
	TerSendBuf[3] = 0x00;
	TerSendBuf[4] = 0x25;
	TerSendBuf[5] = 0x00;
	TerSendBuf[6] = 0x01;
	TerSendBuf[7] = 0x00;
	
	if(MonstateTmp == 0)
		TerSendBuf[8] = 2;
	else if(MonstateTmp == 1)
		TerSendBuf[8] = 1;

	Tmp.IntII = CheckSum16(TerSendBuf + 4,5);
	TerSendBuf[9] = Tmp.TTbyte[0];
	TerSendBuf[10] = Tmp.TTbyte[1];  
	TerSendBuf[11] = 0x55;
	TerSendBuf[12] = 0xAA;
	SL_UartSendData(SL_UART_2,TerSendBuf,13);
}






/**************************************************
**	
**************************************************/

UpgradeStatusStr *GetUpgradeDataSpace(void)
{
	return &UpgradeStatus;
}

/**************************************************
**	
**************************************************/

unsigned char ReadRunMode(void)
{

	return RunMode;
}


/**************************************************
**
**************************************************/

void WriteRunMode(unsigned char n)
{
	if(n > 3)
		n = 0;
	RunMode = n;
}	


/****************************************************
**	
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
**	
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

/***************************************************
** 	
****************************************************/

void SetTerCanProNum(void)
{
	LongToChar     	TmpData;
	SysConfigStr 	*pSysCfg;
	
	unsigned char i;
	unsigned short int len;

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
	TerSendBuf[len++] = pSysCfg->CanProtocolNum;    //
	TerSendBuf[len++] = 0xF0;

	TmpData.LongLL = pSysCfg->SleepTime;       		//
	TerSendBuf[len++] = TmpData.TTbyte[0];			
	TerSendBuf[len++] = TmpData.TTbyte[1];			
	TerSendBuf[len++] = TmpData.TTbyte[2];			
	TerSendBuf[len++] = TmpData.TTbyte[3];			

	for(i = 0;i < 18;i++)
		TerSendBuf[len++] = pSysCfg->TerminalID[i];     //
	//
	for(i = 0;i < 20;i++)
		TerSendBuf[len++] =  pSysCfg->ICCID[i];

	SL_Memcpy((unsigned char *)&TerSendBuf[len],(unsigned char *)pSysCfg,sizeof(SysConfigStr));
	len += sizeof(SysConfigStr);
	


	*(unsigned short int *)&TerSendBuf[len] = CheckSum16(TerSendBuf + 4,len - 4);   //
	len += 2;
	TerSendBuf[len++] = 0x55;
	TerSendBuf[len++] = 0xAA;
	SL_Print("Send Set Ter Pro Num.......%d\r\n",len);
	SL_UartSendData(SL_UART_2, TerSendBuf,len);    		//
}



/*************************************************
**	
**************************************************/
void GetBinFile(unsigned short int n,unsigned short int fn)
{
    IntToChar       		TmpInt16;
	unsigned short int 		len = 0;
	
   	TerSendBuf[len++] = 0x5F;
	TerSendBuf[len++] = 0x5F;
	TerSendBuf[len++] = 0;//
	TerSendBuf[len++] = 0;
	TerSendBuf[len++] = 0x3F;
	TerSendBuf[len++] = 0;
	TerSendBuf[len++] = 0;//
	TerSendBuf[len++] = 0;

	TmpInt16.IntII = n;                     //
    TerSendBuf[len++] = TmpInt16.TTbyte[0];
	TerSendBuf[len++] = TmpInt16.TTbyte[1];
	TmpInt16.IntII =fn;         
    TerSendBuf[len++] = TmpInt16.TTbyte[0];
	TerSendBuf[len++] = TmpInt16.TTbyte[1];
	
	TerSendBuf[len++] = 0;//
	TerSendBuf[len++] = 0;				//
	TerSendBuf[len++] = 0x55;
	TerSendBuf[len++] = 0xAA;

	TmpInt16.IntII = len - 6;								//
	TerSendBuf[2] = TmpInt16.TTbyte[0];
	TerSendBuf[3] = TmpInt16.TTbyte[1];
	TmpInt16.IntII	= len - 12; 							//
	TerSendBuf[6]	=TmpInt16.TTbyte[0];
	TerSendBuf[7]	=TmpInt16.TTbyte[1];
   	//SL_MEMBLOCK(TerSendBuf,len,16);
    SL_UartSendData(SL_UART_2, TerSendBuf,len);  
}


/********************************************
**	
********************************************/

static unsigned char UpgradeST86Mode(unsigned char *p,unsigned short int len)
{
	IntToChar       		TmpInt16;
	LongToChar				TmpInt32;
	unsigned short int      FunID;
	int 					i;
					
	TmpInt16.TTbyte[0] = *(p + 0);
	TmpInt16.TTbyte[1] = *(p + 1);
	len = TmpInt16.IntII;               				//
	
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
				SL_Print(">App:Get UpgradeST86FileStr Mem Fail......\r\n");       //����
				RunMode = 0;
				return 0;
			}
			
			pUpgradeST86File = (unsigned char *)GetSendQueueSpace();
			SL_Memset(pUpgradeST86,0,sizeof(UpgradeST86FileStr));    // 
			SL_Memset(pUpgradeST86File,0,sizeof(SendQueueStr));    //
			TmpInt32.TTbyte[0] = *(p + 4); 
			TmpInt32.TTbyte[1] = *(p + 5); 
			TmpInt32.TTbyte[2] = *(p + 6); 
			TmpInt32.TTbyte[3] = *(p + 7); 
			pUpgradeST86->FileLen = TmpInt32.LongLL;		//

			TmpInt16.TTbyte[0] = *(p + 8); 
			TmpInt16.TTbyte[1] = *(p + 9); 
			
			pUpgradeST86->FileVerfy = TmpInt16.IntII;    //
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
					SL_FreeMemory(pUpgradeST86);       //
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

/***********************************************
**	
************************************************/

unsigned char ProTermRecvData(void)
{
	static unsigned char		step = 0;
	static struct stopwatch16	sw;

	unsigned short int 		DataLen;
	unsigned short int 		tmp;
	unsigned char 			*p = NULL;
	IntToChar       		TmpInt16;
	SysConfigStr			*pSysCfg;             //
	SysConfigStr			*pMcuCfg;             //
	TcpIPSocketStr			*pTcpIp;
	unsigned char 			TmpStr[40];
	int i;

	if(RunMode >= 1)           //
		return 0; 

	switch(step)       //
	{
		case 0:
			init_stopwatch16(&sw);
			step++;
			break;
		case 1:
			if(read_stopwatch16_value(&sw) < 30)
				break;
			WriteDeviceAlram(2,1); 
			break;
		default:
			step = 0;
			break;
	}
	
	SL_Memset(TerRecvBuf,0,sizeof(TerRecvBuf));
	DataLen = ReadComPkt(2,TerRecvBuf,sizeof(TerRecvBuf));    //

	if(DataLen == 0)
		return 0;
	//SL_MEMBLOCK(TerRecvBuf,DataLen,16);
	init_stopwatch16(&sw);
	WriteDeviceAlram(2,0);
	if(TerRecvBuf[0] != 0x5F || TerRecvBuf[1] != 0x5F)                //
		return 0;
	TmpInt16.TTbyte[0] = *(TerRecvBuf + 2);                    //
	TmpInt16.TTbyte[1] = *(TerRecvBuf + 3);                    //
	tmp = TmpInt16.IntII;                                      //
	
	if(*(TerRecvBuf + tmp + 4) != 0x55 || *(TerRecvBuf + tmp + 5) != 0xAA)   //
		return 0;
	//SL_Print("run this is.......1\r\n");
	TmpInt16.TTbyte[0] = *(TerRecvBuf + tmp + 2);
	TmpInt16.TTbyte[1] = *(TerRecvBuf + tmp + 3);
	
	if(TmpInt16.IntII != CheckSum16(TerRecvBuf + 4,tmp - 2))
		return 0;
	//SL_Print("run this is.......2\r\n");
	pSysCfg = GetSysCfgSpace();
	TmpInt16.IntII = DataLen;
	DataLen = tmp;
	
	TmpInt16.TTbyte[0] = *(TerRecvBuf +  4);
	TmpInt16.TTbyte[1] = *(TerRecvBuf +  5);
	tmp = TmpInt16.IntII;    										//
	//SL_Print("ID run this is.......%d\r\n",tmp);
	switch(tmp)
	{
		case 0x0014: 
			CanProNumSendBig = 1;

			p = TerRecvBuf + 8;
			pMcuCfg = (SysConfigStr *) TerSendBuf;
			SL_Memcpy((unsigned char *)pMcuCfg,p,sizeof(SysConfigStr));
			//SL_Print("1 send can sys peizhi:%d\r\n",pMcuCfg->CfgFlag);                                 	 			//
			if(pMcuCfg->CfgFlag == 0x55)
			{
				// 
				SL_Memcpy((unsigned char *)pSysCfg,(unsigned char *)pMcuCfg,sizeof(SysConfigStr));
				//SL_Print("2 send can sys peizhi:%d\r\n",pSysCfg->CfgFlag); 
				SaveSysCfg();
			}
			break;
		case 0x0001:                                     			//
			p = TerRecvBuf + 8;
			//SL_MEMBLOCK(p,DataLen,16);							//
			SL_Memcpy(&TerStateData,p,sizeof(TerStateData));

			LockCar.LockCurrentState = TerStateData.LockState;		//
			LockCar.MonCurrentState = TerStateData.MonState;    	//
			//SL_Print(">The lock Car Status::%d,%d\r\n",LockCar.LockCurrentState,LockCar.MonCurrentState);
			if(TerStateData.PowerVol < 80)   //
				WriteDeviceAlram(0,1); 
			else
				WriteDeviceAlram(0,0);  	//
			
			if(TerStateData.PowerVol >= 70 && TerStateData.PowerVol < 90)    //
				WriteDeviceAlram(1,1); 
			else
				WriteDeviceAlram(1,0); 
			
			(TerStateData.ShellState > 0) ? WriteDeviceAlram(3,1) : WriteDeviceAlram(3,0);   //

			WriteDeviceAlram(5,0);       //0 
			WriteDeviceAlram(6,0);       //0 
			WriteDeviceAlram(7,0);		 //0
			WriteDeviceAlram(8,0);       //
			p += sizeof(TerStateData);
			SL_Memcpy(&CanData,p,sizeof(CanData));   //
			p += sizeof(CanData);
			//
			McuStatus = *p;  
			LockCar.ReplaceDevState = *(p + 1); 						//
			TerTime.TimeSec = TerStateData.McuTime;
			TerTime.VerifyBit = 1;         			 //
			WriteSleepStatus(*p);   				 //运行状态
			
			#if 0
			//SL_Print(">App:The CanData:%d,%d\r\n",CanData.LockCarState,CanData.TotalFuel);
			#endif
			//SL_Print("The Mcu Status::%d\r\n",McuStatus);
			break;
		case 0x0022:                                          //
			LockCar.LockCmdBig = 0;
			break;
		case 0x0023:									  	  //
			LockCar.MonCmdBig = 0;
		case 0x0024:									  	  //
			LockCar.ReplaceDevBig = 0;
			break;
		case 0x0025:
			pSysCfg->TestBit = 0;
			break;
		case 0x00FE:               							  //	
			p = TerRecvBuf + 6;

			TmpInt16.TTbyte[0] = *p;
			TmpInt16.TTbyte[1] = *(p + 1);
			
			p = TerRecvBuf + 8;
			if(LookForStr(p,"AT+Test",DataLen - 6) >= 0)         			//
			{
				pSysCfg->CfgFlag = 0x56;
				SL_Print("AT+Test\r\n");
				break;
			}
	
			if(LookForStr(p,"HOMER3ETESTOVER!",DataLen - 6) >= 0)  		//
			{				  
				pTcpIp = GetTcpIpSocketSpce();
				SL_Memset(pSysCfg->ICCID,'\0',25);
				for(i = 0;i < 20;i++)                                  			//
					pSysCfg->ICCID[i] = pTcpIp->SimIccId[i];
				pSysCfg->CfgFlag = 0x55;
				SaveSysCfg();    //保存配置数据
				pTcpIp->ResetLink = 1;                               			//
				
				break;
			}

			SL_Memset(TmpStr,'\0',12);
			SL_Memcpy(TmpStr,p,9);
			
			if(SL_Strcmp(TmpStr,"IIAP_ST87") == 0)
			{
				UpgradeST86Mode(p + 9,DataLen - 15);             //
				break;
			}
			
			if((i = LookForStr(p,"BDWMODIF",DataLen - 6)) >= 0)   		//
			{
				p = p + i + 8;
				//SL_Print("Run BDWMODIF:\r\n");
				if(*(p) != ':')
					break;
				p += 1;
				SL_Memset(pSysCfg->TerminalID,'\0',sizeof(pSysCfg->TerminalID));
				for(i = 0;i < 10;i++)                                  			//
					pSysCfg->TerminalID[i] = *(p + i);
				
				//pSysCfg->TerminalID[i] = '\0';
				p += 10;
				if(*p != ',')
					break;
				
				tmp = GetDataStr(1,2,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					SL_Memcpy(pSysCfg->ServerAddr,TmpStr,tmp + 1);          	//
					
				}
				
				tmp = GetDataStr(2,3,p,TmpStr,DataLen); 
				if(tmp > 0)
				{	
					TmpStr[tmp] = '\0';
					pSysCfg->ServerPort = StrToDouble(TmpStr);         			//
				}
				
				tmp = GetDataStr(3,4,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					pSysCfg->SleepTime = StrToDouble(TmpStr);          			//
				}
				
				tmp = GetDataStr(4,5,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					pSysCfg->DistanceUpLoad = StrToDouble(TmpStr);    			//
				}


				tmp = GetDataStr(5,6,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					pSysCfg->TravelUpLoadTime = StrToDouble(TmpStr);   			//
				}

				tmp = GetDataStr(6,7,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					pSysCfg->HardWareNum = StrToDouble(TmpStr);      			//
				}
				
				tmp = GetDataStr(7,8,p,TmpStr,DataLen);                   		//  
				
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					pSysCfg->CanProtocolNum = StrToDouble(TmpStr);   			// 
					
					if(pSysCfg->CanProtocolNum == 0x20 )                       //
					{
						pSysCfg->CarType = 0x01; 
					}
					else if(pSysCfg->CanProtocolNum == 0x21)                   //
					{
						pSysCfg->CarType = 0x02; 
					}  
					else
					{
						pSysCfg->CanProtocolNum = 0x20;                       //

						pSysCfg->CarType = 0x01;
					
					}
				}

				tmp = GetDataStr(8,9,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					SL_Memcpy(pSysCfg->TerApn,TmpStr,tmp);           			//
					pSysCfg->TerApn[tmp] = '\0';
				}
				tmp = GetDataStr(9,10,p,TmpStr,DataLen);
				//SL_Print("The Is 1 :%d,%d\r\n",tmp, TmpStr[0]);
				if(tmp > 0)
				{
					MonstateTmp = TmpStr[0] - 0x30;
					//SL_Print("The Is 2:%d\r\n",MonstateTmp);
				}
				pSysCfg->TestBit = 1;
			}

			break;
		default:
			return 0;
					
	}
	return 1;
}





/**************************************************
**	
***************************************************/
void SendTerminalInfo(unsigned short int id)
{
	SysConfigStr				*pSys;
	unsigned short int 			len;
	unsigned int 				tmp;
	TcpIPSocketStr 				*pTcpIp;
	IntToChar					TmpInt16;              	//
	unsigned char 				TmpStr[12];
	GNSSSTR 					*pGnss;                	//
	SysTimeStr					Time;                	//
	
	pSys = GetSysCfgSpace();
	len = 0;
	TerSendBuf[len++] = 0x5F;
	TerSendBuf[len++] = 0x5F;
	TerSendBuf[len++] = 0;								//
	TerSendBuf[len++] = 0;			
	TmpInt16.IntII = id;
	TerSendBuf[len++] = TmpInt16.TTbyte[0];
	TerSendBuf[len++] = TmpInt16.TTbyte[1];
	TerSendBuf[len++] = 0;								//
	TerSendBuf[len++] = 0;
	TerSendBuf[len++] = pSys->CfgFlag;    				//
	TerSendBuf[len++] = ConfigFlag;      				//
	TerSendBuf[len++] = 'h';
	TerSendBuf[len++] = 'o';
	TerSendBuf[len++] = 'm';
	TerSendBuf[len++] = 'e';
	TerSendBuf[len++] = 'r';
	TerSendBuf[len++] = '3';
	TerSendBuf[len++] = 'x';
	TerSendBuf[len++] = ':';
					
	for(tmp = 0;tmp < 10;tmp++)
		TerSendBuf[len++] = pSys->TerminalID[tmp];   			//
	TerSendBuf[len] = ',';                               		//
	len += 1;
	pTcpIp = GetTcpIpSocketSpce();
	tmp = SL_Strlen(pTcpIp->SimIccId);
	SL_Memcpy(TerSendBuf + len,pTcpIp->SimIccId,tmp);  			//
	len += tmp;
	TerSendBuf[len] = ',';                                		//
	len += 1;
					
	tmp =SL_Strlen(pSys->ServerAddr);
	SL_Memcpy(TerSendBuf + len,pSys->ServerAddr,tmp);     //
	len += tmp;
	TerSendBuf[len] = ',';                                //
	len += 1;
	SL_itoa(pSys->ServerPort,TmpStr,10);
	for(tmp = 0;tmp < SL_Strlen(TmpStr);tmp++)            //
		TerSendBuf[len++] = TmpStr[tmp];
	TerSendBuf[len++] = ',';

	tmp = SL_Strlen(pSys->TerApn);
	SL_Memcpy(TerSendBuf + len,pSys->TerApn,tmp);        //
	len += tmp;
	TerSendBuf[len++] = ',';                             //
					
	pGnss = GetGnssDataSpace();
	if(pGnss->status == 'A')                             //
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
					
	tmp = pGnss->latitude * 1000000;                   //
	SL_Memset(TmpStr,0x30,12);
	UInt32ToStr(tmp,TmpStr,12);
	SL_Memcpy(TerSendBuf + len,TmpStr,9);              //
	len += 9;
	tmp = pGnss->longitude * 1000000;                  //
	SL_Memset(TmpStr,0x30,12);
	UInt32ToStr(tmp,TmpStr,12);                        
	SL_Memcpy(TerSendBuf + len,TmpStr,9);              //
	len += 9;

	SL_Memcpy(TerSendBuf + len,"00000000",8);         //
	len += 8;

	Time =  UnixToCalendar(pGnss->gnss_time - 28800);
	CharToStr(Time.Year,TmpStr,2);                  //
	SL_Memcpy(TerSendBuf + len,TmpStr,2);
	len += 2;

					
	CharToStr(Time.Mon,TmpStr,2);                  //
	SL_Memcpy(TerSendBuf + len,TmpStr,2);
	len += 2;

	CharToStr(Time.MDay,TmpStr,2);                  //
	SL_Memcpy(TerSendBuf + len,TmpStr,2);
	len += 2;

	CharToStr(Time.Hour,TmpStr,2);                  //
	SL_Memcpy(TerSendBuf + len,TmpStr,2);
	len += 2;

	CharToStr(Time.Min,TmpStr,2);                  //
	SL_Memcpy(TerSendBuf + len,TmpStr,2);
	len += 2;

	CharToStr(Time.Sec,TmpStr,2);                  //
	SL_Memcpy(TerSendBuf + len,TmpStr,2);
	len += 2;
	TerSendBuf[len++] = ',';
					
	CharToStr(pGnss->satellite_num,TmpStr,2);       //
	SL_Memcpy(TerSendBuf + len,TmpStr,2);
	len += 2;
	TerSendBuf[len++] = ',';
	CharToStr(pTcpIp->GsmSign,TmpStr,2);
	//SL_Print("pTcpIp->GsmSign:%d\r\n",pTcpIp->GsmSign);
	SL_Memcpy(TerSendBuf + len,TmpStr,2);           //
	len += 2;
	TerSendBuf[len++] = ',';
					
	SL_Memset(TmpStr,0x30,12);
					
	tmp = UInt32ToStr(TerStateData.PowerVol,TmpStr,12);
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);           //
	len += tmp;
	TerSendBuf[len++] = ',';
					
	SL_Memset(TmpStr,0x30,12);
					
	tmp = UInt32ToStr(TerStateData.BatteryVol,TmpStr,12);
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);           //
	len += tmp;
	TerSendBuf[len++] = ',';
					
	TerSendBuf[len++] = TerStateData.AccState + 0x30;   //
	TerSendBuf[len++] = ',';
						
	TerSendBuf[len++] = pTcpIp->GprsLinkState + 0x30;   //
	TerSendBuf[len++] = ',';
					
	tmp = GetVerNum();

	TerSendBuf[len++] = tmp / 10 + 0x30;
	TerSendBuf[len++] = '.';
	TerSendBuf[len++] = tmp % 10 + 0x30;
					
	TerSendBuf[len++] = ',';

	SL_Memset(TmpStr,0x30,12);
	tmp = UInt32ToStr(pSys->SleepTime,TmpStr,12);      //
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
	len += tmp;
	TerSendBuf[len++] = ',';

	SL_Memset(TmpStr,0x30,12);
	tmp = UInt32ToStr(pSys->DistanceUpLoad,TmpStr,12);      //
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
	len += tmp;
	TerSendBuf[len++] = ',';

	SL_Memset(TmpStr,0x30,12);
	tmp = UInt32ToStr(pSys->TravelUpLoadTime,TmpStr,12);      //
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
	len += tmp;
	TerSendBuf[len++] = ',';

	TerSendBuf[len++] = pSys->HardWareNum / 10 + 0x30;       //
	TerSendBuf[len++] = '.';
	TerSendBuf[len++] = pSys->HardWareNum % 10 + 0x30;		 //
	TerSendBuf[len++] = ',';
					
	TerSendBuf[len++] = 0x31;    //
	TerSendBuf[len++] = ',';

	TerSendBuf[len++] = 0x31;	//
	TerSendBuf[len++] = ',';

	TerSendBuf[len++] = '1';		//
	TerSendBuf[len++] = ',';
					
	if(TerStateData.GnssAntShort > 0)
		TerSendBuf[len++] = '2';
	else if(TerStateData.GnssAntState > 0)
		TerSendBuf[len++] = '2';
	else
		TerSendBuf[len++] = '0';
	
	TerSendBuf[len++] = ',';
			
	TerSendBuf[len++] = TerStateData.ShellState + 0x30; 	//
	TerSendBuf[len++] = ',';

	TerSendBuf[len++] = '1';                                //
	TerSendBuf[len++] = ',';

	TerSendBuf[len++] = '1';								//
	TerSendBuf[len++] = ',';

	tmp = sizeof("0102") - 1;                            	//
	
	SL_Memcpy(TerSendBuf + len,"0102",tmp);
	len += tmp;
	tmp = GetVerNum();

	TerSendBuf[len++] = tmp / 10 + 0x30;     
	TerSendBuf[len++] = tmp % 10 + 0x30;
	TerSendBuf[len++] = TerStateData.FirNum / 10 + 0x30;   //
	TerSendBuf[len++] = TerStateData.FirNum % 10 + 0x30;   //
	TerSendBuf[len++] = ',';

	tmp = UInt32ToStr(pSys->CanProtocolNum,TmpStr,12);      //
	SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
	len += tmp;
	TerSendBuf[len++] = ',';
	//		
	if(TerStateData.CanConnect == 0)
		TerSendBuf[len++] = '0';                              //
	else
		TerSendBuf[len++] = '1';                              //
					
	TerSendBuf[len++] = ',';
					
	TerSendBuf[len++] = TerStateData.FirNum / 10 + 0x30;   //
	TerSendBuf[len++] = '.';
	TerSendBuf[len++] = TerStateData.FirNum % 10 + 0x30;   //
	TerSendBuf[len++] = ',';
						
	TerSendBuf[len++] = 0x0D;                                 //
	TerSendBuf[len++] = 0x0A;
					
	TerSendBuf[len] = 0x00;
	TerSendBuf[len + 1] = 0x00;
	TerSendBuf[len + 2] = 0x55;
	TerSendBuf[len + 3] = 0xAA;
	len += 4;
	TmpInt16.IntII = len - 6;                 				//
	TerSendBuf[2] = TmpInt16.TTbyte[0];
	TerSendBuf[3] = TmpInt16.TTbyte[1];
	TmpInt16.IntII	= len - 12;             				//
	TerSendBuf[6]	=TmpInt16.TTbyte[0];
	TerSendBuf[7]	=TmpInt16.TTbyte[1];
	SL_UartSendData(SL_UART_2,TerSendBuf,len);      	//
}



/**********************************************************
**	
**********************************************************/

void ProTerminalData(void)
{	
	static unsigned char 		step = 0,step1 = 0,step2 = 0;
	static struct stopwatch16 	sw,sw1;

	unsigned short int 			len;					//
	int 						tmp;					//
	SysConfigStr 				*pSys;                 	//
	IntToChar					TmpInt16;              	//
	LongToChar					TmpInt32; 			   	//
	unsigned char 				TmpStr[12];			   	//
	SysTimeStr					*pTime;                	//
	TcpIPSocketStr				*pTcpIp;

		
	pTime = GetSetTimeSpace();
	
	switch(RunMode)
	{
		case 0:
			switch(step)
			{
				case 0:  
					SetTerCanProNum();              			//  
					CanProNumSendBig = 0;
					init_stopwatch16(&sw);  
					step++;
					break;
				case 1:
					if(CanProNumSendBig == 0 && read_stopwatch16_value(&sw) < 25)      //
						break;
					if(CanProNumSendBig == 0)
					{
						step = 0;
						break;     
					}	
					step++;
					break;
				case 2:
					init_stopwatch16(&sw); 
					step++;
					break;
				case 3:
					if(read_stopwatch16_value(&sw) < 19)                       	     //
						break;
					SendGetStatusCmd();
					GetStatusBig = 0;
					init_stopwatch16(&sw); 
					step++;
					break;
				case 4:
					if(GetStatusBig == 0 && read_stopwatch16_value(&sw) < 10)   	//
						break;
					
					if(pTime->VerifyBit > 0)    	//
					{
						step = 6;                 	// 
						init_stopwatch16(&sw);
						pTime->VerifyBit = 0;
						break;
					}
					if(LockCar.LockCmdBig > 0)    	//
					{
						step = 7;
						init_stopwatch16(&sw);
						break;
					}
					if(LockCar.MonCmdBig > 0)   	// 
					{
						step = 8;
						init_stopwatch16(&sw);
						break;
					}

					if(LockCar.ReplaceDevBig > 0)   	//   
					{
						step = 10;
						init_stopwatch16(&sw);
						break;
					}

					if(McuStatus == 3)				//
					{
						step = 9;
						init_stopwatch16(&sw);
						break;
					}
					pSys = GetSysCfgSpace();

					if(pSys->TestBit > 0)
					{
						step = 11;
						init_stopwatch16(&sw);
						break;
					}

					if(McuStatus == 2 || pSys->CfgFlag == 0x56)				//
					{
						step++; 
						if(ConfigFlag == 0)
							ConfigFlag = 1;
						init_stopwatch16(&sw);		//
						break;
					}
					
					step = 2;                         //
					init_stopwatch16(&sw);
					break;
				case 5:                               //
					if(read_stopwatch16_value(&sw) < 10)
						break;
					//
					SendTerminalInfo(0x00FE);			//
					step = 2;   								//
					init_stopwatch16(&sw);
					break;
				case 6:
					if(read_stopwatch16_value(&sw) < 5)
						break;
					step = 2;
					SendVerifyTimeCmd(pTime);              		//
					init_stopwatch16(&sw);
					break;
				case 7:
					if(read_stopwatch16_value(&sw) < 5)
						break;
					step = 2;
					SendLockVehicleCmd();                 		//
					init_stopwatch16(&sw);                		//
					//SL_Print("Lock Car Send Cmd...........\r\n"); //
					break;
				case 8:
					if(read_stopwatch16_value(&sw) < 5)      //
						break;
					step = 2;
					SendConCmd();
					init_stopwatch16(&sw);
					SL_Print("Con Send Cmd...........\r\n");  //
					break;
				case 9:
					if(read_stopwatch16_value(&sw) < 5)
						break;
					SendTerminalInfo(0x00FD);   				 //
					step = 2;
					break;
				case 10:
					if(read_stopwatch16_value(&sw) < 5)
						break;
					SendReplaceDevCmd();
					//SL_Print("ReplaceDev Send Cmd...........\r\n");  //
					step = 2;
					break;
				case 11:
					if(read_stopwatch16_value(&sw) < 5)
						break;
					SendConfigOverStatus();
					step = 2;
					break;
				default:
					step = 0;
					break;
			}
			break;
		case 1:                                       // 
			switch(step1)
			{
				case 0:                                //    
					UpgradeData = (UpgradeFileStr *)SL_GetMemory(sizeof(UpgradeFileStr));
					if(UpgradeData == NULL)
					{
						SL_Print(">App:Get Sys Mem Fail......\r\n");       //
						RunMode = 0;
						break;
					}
					
					pTcpIp = GetTcpIpSocketSpce();
					UpgradeData->FileTotalLen = pTcpIp->FtpFileLen;
					UpgradeData->FileVerfy = CheckSum16(pTcpIp->FtpFileSapce,pTcpIp->FtpFileLen);   //  
					UpgradeData->FileTotalFram = UpgradeData->FileTotalLen / 1024;
					
					for(tmp = 0;tmp < UpgradeData->FileTotalFram;tmp++)
					{
						UpgradeData->FileLen[tmp] = 1024;       //
						SL_Memcpy(UpgradeData->FileBuf[tmp],(pTcpIp->FtpFileSapce + 1024 * tmp),UpgradeData->FileLen[tmp]);  //
					}
					tmp = UpgradeData->FileTotalLen % 1024;
					
					if(tmp > 0)
					{
						UpgradeData->FileLen[UpgradeData->FileTotalFram] = tmp;
						SL_Memcpy(UpgradeData->FileBuf[UpgradeData->FileTotalFram],(pTcpIp->FtpFileSapce + 1024 * UpgradeData->FileTotalFram),UpgradeData->FileLen[UpgradeData->FileTotalFram]);
						UpgradeData->FileTotalFram++;
					}
					
					SL_Print(">App:Load Stm32 FirWare Finsh.....%d,%d,%d\r\n",UpgradeData->FileTotalLen,UpgradeData->FileTotalFram,UpgradeData->FileVerfy);     //
					//SL_MEMBLOCK(UpgradeData->FileBuf[UpgradeData->FileTotalFram - 1],UpgradeData->FileLen[UpgradeData->FileTotalFram - 1],16);
					
					len = 0;
					SL_Memset(TerSendBuf,0,sizeof(TerSendBuf));          //
					TerSendBuf[len++] = 0x5F;
					TerSendBuf[len++] = 0x5F;
					TerSendBuf[len++] = 0x0E;
					TerSendBuf[len++] = 0x00;            //
					TerSendBuf[len++] = 0x1F;
					TerSendBuf[len++] = 0x00;
					TerSendBuf[len++] = 0x08;
					TerSendBuf[len++] = 0x00;           //
					TerSendBuf[len++] = 0x01;
					TerSendBuf[len++] = 0x00;           //
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
					SL_UartSendData(SL_UART_2,TerSendBuf,len);      //
					init_stopwatch16(&sw1);
					step1++;
					break;
				case 1:                                                //
					if(read_stopwatch16_value(&sw1) > 9600)        //
					{
						SL_FreeMemory(UpgradeData);
						RunMode = 0;         
						UpgradeStatus.UpgradeStatus = 1;     //
						//
						return;
					}
					tmp = ReadComPkt(2,TerRecvBuf,sizeof(TerRecvBuf));    //
					if(tmp == 0)
						return ;
					if(TerRecvBuf[0] != 0x5F || TerRecvBuf[1] != 0x5F)                //
						return ;
					
					TmpInt16.TTbyte[0] = *(TerRecvBuf + 2);
					TmpInt16.TTbyte[1] = *(TerRecvBuf + 3);
					len = TmpInt16.IntII;
	
					if(*(TerRecvBuf + len + 4) != 0x55 || *(TerRecvBuf + len + 5) != 0xAA)   //
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
					
					TmpInt16.TTbyte[0] = TerRecvBuf[10];      //
					TmpInt16.TTbyte[1] = TerRecvBuf[11];
				
					UpgradeData->FileCurrentFram = TmpInt16.IntII - 1;                  //  
					SL_Print("The FileCurrentFram Num:%d\r\n",UpgradeData->FileCurrentFram);
					
					len = 0;
					SL_Memset(TerSendBuf,0,sizeof(TerSendBuf));    //
					TerSendBuf[len++] = 0x5F;
					TerSendBuf[len++] = 0x5F;
					TmpInt16.IntII = UpgradeData->FileLen[UpgradeData->FileCurrentFram] + 10;
					TerSendBuf[len++] = TmpInt16.TTbyte[0];         //
					TerSendBuf[len++] =	TmpInt16.TTbyte[1];
					TerSendBuf[len++] = 0x1F;         //
					TerSendBuf[len++] = 0x00;
					TmpInt16.IntII    = UpgradeData->FileLen[UpgradeData->FileCurrentFram] + 6;   //
					TerSendBuf[len++] = TmpInt16.TTbyte[0];
					TerSendBuf[len++] = TmpInt16.TTbyte[1];       //
					
					TerSendBuf[len++] = 0x02;
					TerSendBuf[len++] = 0x00;
					TerSendBuf[len++] = 0x00;
					TerSendBuf[len++] = 0x00;
					
					SL_Memcpy(&TerSendBuf[len],UpgradeData->FileBuf[UpgradeData->FileCurrentFram],UpgradeData->FileLen[UpgradeData->FileCurrentFram]);
					
					len += UpgradeData->FileLen[UpgradeData->FileCurrentFram];
					//SL_Print("The FileCurrentFram Len:%d\r\n",UpgradeData->FileLen[UpgradeData->FileCurrentFram]);

					TmpInt16.IntII= CheckSum16(TerSendBuf + 12,len - 12);            //
					
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
						UpgradeStatus.UpgradeStatus = 0;     // 
						//
						return;
						
					}
					break; 
				default:
					step1 = 0;
					break;
			}
			break;
		case 2:                                      // 
			switch(step2)
			{
				case 0:
					init_stopwatch16(&sw);
					GetBinFile(2,pUpgradeST86->CurrentFramNum);				// 
					step2++;
					break;
				case 1:
					SL_Memset(TerRecvBuf,0,sizeof(TerRecvBuf));
					TmpInt16.IntII = ReadComPkt(2,TerRecvBuf,sizeof(TerRecvBuf));    // 
					if(read_stopwatch16_value(&sw) < 15 && TmpInt16.IntII == 0)
						break;

					if(TerRecvBuf[0] != 0x5F || TerRecvBuf[1] != 0x5F)                // 
					{
						SL_Memset(pUpgradeST86File,0,sizeof(SendQueueStr));    //  
						WriteRunMode(0);      // 
						break;
					}
					tmp = 0;
					TmpInt16.TTbyte[0] = *(TerRecvBuf + 2);                    // 
					TmpInt16.TTbyte[1] = *(TerRecvBuf + 3);                    // 
					tmp = TmpInt16.IntII;                                      // 
			
					if(*(TerRecvBuf + tmp + 4) != 0x55 || *(TerRecvBuf + tmp + 5) != 0xAA)   // 
					{
						SL_Memset(pUpgradeST86File,0,sizeof(SendQueueStr));    //  
						WriteRunMode(0);       // 
						break;
					}
					
					TmpInt16.TTbyte[0] = *(TerRecvBuf + tmp + 2);
					TmpInt16.TTbyte[1] = *(TerRecvBuf + tmp + 3);
	
					if(TmpInt16.IntII != CheckSum16(TerRecvBuf + 4,tmp - 2))
					{
						SL_Memset(pUpgradeST86File,0,sizeof(SendQueueStr));    //  
						WriteRunMode(0);     //
						break;
					}
					
					//
					SL_Memset(TmpStr,'\0',12);
					SL_Memcpy(TmpStr,&TerRecvBuf[8],9);
					if(SL_Strcmp(TmpStr,"IIAP_ST87") == 0)
					{
						//SL_Print("Run The IIIIII\r\n");
						if(UpgradeST86Mode(&TerRecvBuf[8] + 9,tmp) == 2)        //  
						{	
							GetBinFile(5,0);// 
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
  				case 3:                   					//
  					if(read_stopwatch16_value(&sw) < 40)   //
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
						SL_Sleep(5000);				//
						SL_HstSendEvt(0x20161034);      //
                        SL_PowerDown(0); 				//
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


