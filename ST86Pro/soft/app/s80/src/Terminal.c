

/**************************************************
**	File Name:
**	Time:
**************************************************/
#include "include.h"


#define bitset(var, bitno) ((var) |= 1UL << (bitno))

#define bitclr(var, bitno) ((var) &= ~(1UL << (bitno)))


/************ STM32F升级文件缓冲区***************/

static UpgradeStatusStr 		UpgradeStatus;                //

static UpgradeFileStr			*UpgradeData;                 //          

/*****************数据缓冲区**********************/

static unsigned char 			TerRecvBuf[1200];           //接收缓冲区
static unsigned char 			TerSendBuf[1200];			//发送缓冲区

static unsigned char 			RunMode;   //0:正常运行，1，升级单片机程序，2，单片机升级程序(ST87程序)

/*******************本地全局变量********************/


static CanWheatStr 			CanDataWheat;    		//小麦机
static CanPaddyStr			CanDataPaddy;     		//水稻机
static CanMaizeStr			CanDataMaize;     		//玉米机
static CanTractorStr		CanDataTractor;		 	//拖拉机

static TerStateStr			TerStateData;           //设备终端状态
static TerSDCardStr			TerSDCardData;          //设备SD卡状态，
static SysTimeStr 			TerTime;                //设备时间


/*********************本地全局变量*************************/

static unsigned char 			CanProNumSendBig 	= 0;         //CAN协议号发送标识
static unsigned char 			GetStatusBig 		= 0;         //读设备状态数据
static unsigned char 			GetSDBig 			= 0;         //获取SD卡标识
static unsigned char 			GetCanBig 			= 0;         //读CAN数据标识
static unsigned char 			GetTerTimeBig		= 0;         //获取MCU时间标识      



unsigned char GetCAN[17]    				= {0x5F,0x5F,0x0B,0x00,0x05,0x00,0x05,0x00,0xAA,0xFF,0x23,0x45,0x67,0x00,0x00,0x55,0xAA};
unsigned char GetACC[12]    				= {0x5F,0x5F,0x06,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x55,0xAA};
unsigned char GetSD[12]    					= {0x5F,0x5F,0x06,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x55,0xAA};
unsigned char GetIPandPOUR[12]  			= {0x5F,0x5F,0x06,0x00,0x12,0x00,0x00,0x00,0x00,0x00,0x55,0xAA};
unsigned char GetVIN[12]    				= {0x5F,0x5F,0x06,0x00,0x16,0x00,0x00,0x00,0x00,0x00,0x55,0xAA};
unsigned char Lock_Vehicle[13]				= {0x5F,0x5F,0x07,0x00,0x22,0x00,0x01,0x00,0x00,0x00,0x00,0x55,0xAA};
unsigned char VCUUpdataStartCom[20]			= {0x5F,0x5F,0x0E,0x00,0x2F,0x00,0x08,0x00,0x01,0x00,0x34,0xB1,0x00,0x00,0x66,0x86,0xF9,0x01,0x55,0xAA};
unsigned char GetStatus[12]    				= {0x5F,0x5F,0x06,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x55,0xAA};
unsigned char GetTerTime[12]  				= {0x5F,0x5F,0x06,0x00,0x01,0x01,0x00,0x00,0x01,0x01,0x55,0xAA};



/*************************************************
**	函数名称:unsigned char ReadAccState(void)
**	功能描述:返回ACC状态
**************************************************/

unsigned char ReadAccState(void)
{
	return TerStateData.AccState;
}

/**************************************************
**	函数名称:CanWheatStr *GetCanWheatSpace(void)
**	功能描述:返回小麦机Can数据地址
***************************************************/

CanWheatStr *GetCanWheatSpace(void)    //小麦机 
{
	return &CanDataWheat;
}


/**************************************************
**	函数名称:CanPaddyStr *GetCanPaddySpace(void)
**	功能描述:返回水稻机数据空间地址
**************************************************/

CanPaddyStr	*GetCanPaddySpace(void) 
{
	return &CanDataPaddy;     //水稻机  
}


/********************************************************
**	函数名称:CanMaizeStr *GetCanMaizeSpace(void)
**	功能描述:返回玉米机数据空间地址
*********************************************************/

CanMaizeStr *GetCanMaizeSpace(void)     //玉米机
{
	return	&CanDataMaize;     //玉米机
}


/********************************************************
**	函数名称:CanTractorStr *GetCanTractorSpace(void)
**	功能描述:返回拖拉机数据空间地址
********************************************************/

CanTractorStr *GetCanTractorSpace(void)		 //拖拉机
{
	return &CanDataTractor;		 //拖拉机 
}


/****************************************************
**	函数名称:TerStateStr	*GetTerStateDataSpace(void)
**	功能描述:
****************************************************/

TerStateStr	*GetTerStateDataSpace(void)
{
	return &TerStateData;
}


/****************************************************
**	函数名称:TerSDCardStr *GetTerSDCardSpace(void)
**	功能描述:返回设备SD卡数据区域空间地址
****************************************************/

TerSDCardStr *GetTerSDCardSpace(void)
{
	return &TerSDCardData;
}


/******************************************************
**	函数名称:SysTimeStr *GetTerTimeSpace(void)
**	功能描述:返回设备时间空间
******************************************************/
SysTimeStr *GetTerTimeSpace(void)
{
	return &TerTime;
}


/***************************************************
**	函数名称:void SendLockVehicleCmd(void)
**	功能描述:发送锁车指令
***************************************************/

void SendLockVehicleCmd(void)
{
	SL_UartSendData(SL_UART_1,Lock_Vehicle,sizeof(Lock_Vehicle));    		//发送
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
**
**
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
	SysConfigStr   *p;
	unsigned short tmp;
	int i;
	
	LongToChar     TmpData;
	IntToChar		Tmp;
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
	SL_UartSendData(SL_UART_1,TerSendBuf,16);
}

/****************************************************
**	函数名称:void SendVerifyTimeCmd(SysTimeStr *pTime)
**	功能描述:发送时间校验消息到MCU
****************************************************/

void SendVerifyTimeCmd(SysTimeStr *pTime)
{
	unsigned short int Temp;

	Temp = 0;

	TerSendBuf[0] = 0x5F;
	TerSendBuf[1] = 0x5F;
	TerSendBuf[2] = 0x0C;
	TerSendBuf[3] = 0x00;
	TerSendBuf[4] = 0x20;
	TerSendBuf[5] = 0x00;
	TerSendBuf[6] = 0x06;
	TerSendBuf[7] = 0x00;
	TerSendBuf[8] = pTime->Year;          //年
	TerSendBuf[9] = pTime->Mon;           //月
	TerSendBuf[10] = pTime->MDay;         //日
	TerSendBuf[11] = pTime->Hour;         //时
	TerSendBuf[12] = pTime->Min;          //分
	TerSendBuf[13] = pTime->Sec;          //秒
	*(unsigned short int *)&TerSendBuf[14] = CheckSum16(TerSendBuf + 4,9);
	TerSendBuf[16] = 0x55;
	TerSendBuf[17] = 0xAA;

	SL_UartSendData(SL_UART_1, TerSendBuf,18);
}

/********************************************************
** 函数名称:void SetTerCanProNum(void)
** 功能描述:发送Can协议版本号到MCU
********************************************************/

void SetTerCanProNum(void)
{
	unsigned short  Temp;

	SysConfigStr *pSysCfg;
	
	Temp = 0;

	TerSendBuf[0] = 0x5F;
	TerSendBuf[1] = 0x5F;
	TerSendBuf[2] = 0x10;
	TerSendBuf[3] = 0x00;
	TerSendBuf[4] = 0x14;
	TerSendBuf[5] = 0x00;
	TerSendBuf[6] = 0x0A;
	TerSendBuf[7] = 0x00;
	pSysCfg = GetSysCfgSpace();
	TerSendBuf[8] = pSysCfg->CanProtocolNum;
	TerSendBuf[9] = 0xF0;

	TerSendBuf[10] = 1;		//LockVehicleStatus;//锁车状态1
	TerSendBuf[11] = 0;		//保留2
	TerSendBuf[12] = 0;		//保留3
	TerSendBuf[13] = 0;		//保留4
	TerSendBuf[14] = 0;		//保留5
	TerSendBuf[15] = 0;		//保留6
	TerSendBuf[16] = 0;		//保留7
	TerSendBuf[17] = 0;		//保留8
	*(unsigned short int *)&TerSendBuf[18] = CheckSum16(TerSendBuf + 4,13);
	
	TerSendBuf[20] = 0x55;
	TerSendBuf[21] = 0xAA;
	//SL_Print("Send Set Ter Pro Num.......\r\n");
	SL_UartSendData(SL_UART_1, TerSendBuf,22);    		//发送

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
	LongToChar				TmpInt32;
	unsigned char 			TmpStr[50];
	int i;

	if(RunMode >= 1)           //
		return 0; 
	
	SL_Memset(TerRecvBuf,0,sizeof(TerRecvBuf));
	DataLen = ReadComPkt(1,TerRecvBuf,sizeof(TerRecvBuf));    //

	if(DataLen == 0)
		return 0;
	
	if(TerRecvBuf[0] != 0x5F || TerRecvBuf[1] != 0x5F)                //判断包头
		return 0;
	TmpInt16.TTbyte[0] = *(TerRecvBuf + 2);                    //数据长度，包括
	TmpInt16.TTbyte[1] = *(TerRecvBuf + 3);
	tmp = TmpInt16.IntII;
	
	if(*(TerRecvBuf + tmp + 4) != 0x55 || *(TerRecvBuf + tmp + 5) != 0xAA)   //判断包尾
		return 0;
	
	TmpInt16.TTbyte[0] = *(TerRecvBuf + tmp + 2);
	TmpInt16.TTbyte[1] = *(TerRecvBuf + tmp + 3);
	
	if(TmpInt16.IntII != CheckSum16(TerRecvBuf + 4,tmp - 2))
		return 0;
	
	pSysCfg = GetSysCfgSpace();
	
	TmpInt16.TTbyte[0] = *(TerRecvBuf +  4);
	TmpInt16.TTbyte[1] = *(TerRecvBuf +  5);
	tmp = TmpInt16.IntII;    //消息ID  
	
	switch(tmp)
	{
		case 0x0014:                                   	 			//设置CAN通讯协议，
			CanProNumSendBig = 1;
			break;
		case 0x0001:                                     			//解析设备状态
			p = TerRecvBuf + 8;
			TerStateData.AccState = *(p);                  			//这里解析ACC状态
			TerStateData.MotoState = *(p + 1);			   			//Moto状态
			TerStateData.SdCardState = *(p + 2);
			TerStateData.E2PROMState = *(p + 3);
			TerStateData.AntConState = *(p + 4);    				//天线连接状态
			TerStateData.ShellState  = *(p + 5);  					//外壳状态  
			//SL_Print("The Shell State :%d\r\n",TerStateData.ShellState);
			TerStateData.AntShortState = *(p + 6);   				//天线短路状态  
			TerStateData.CanStatus = *(p + 7);
			
			TmpInt16.TTbyte[0] 			= *(p + 18);
			TmpInt16.TTbyte[1] 			= *(p + 19);
			TerStateData.PowVol 		= TmpInt16.IntII;          //电源电压
			TmpInt16.TTbyte[0] 			= *(p + 20);
			TmpInt16.TTbyte[1] 			= *(p + 21);
			TerStateData.Battery 		= TmpInt16.IntII;         //电池电压
			TmpInt16.TTbyte[0] 			= *(p + 22);
			TmpInt16.TTbyte[1] 			= *(p + 23);
			TerStateData.TerTemp 		= TmpInt16.IntII;         //单片机片上温度
			GetStatusBig = 1;
			break;
		case 0x0018:
			GetSDBig = 1;                           			//
			p = TerRecvBuf + 8;
			TerSDCardData.SDCardCheck = 12;                		//SD卡是否插入
			TerSDCardData.SDCardType = 12;           			//SD卡类型
			TerSDCardData.SDCardTotalSpace = 12;    			//SD卡总容量 
			TerSDCardData.SDCardSurplusSpace = 12;         		//SD卡剩余容量
			TerSDCardData.SDCardState = 12;                		//SD卡状态
			TerSDCardData.NC= 12;  
			break;
		case 0x0005:
			bitset(GetCanBig,0);            						//把
			switch(pSysCfg->CarType)
			{
				case 0x08:          										//小麦机  新国三
				case 0x0E:                                          //小麦机 老国三
					p = TerRecvBuf + 12;
					
					TmpInt16.TTbyte[0] = *(p);
					TmpInt16.TTbyte[1] = *(p + 1);
					if(TmpInt16.IntII == 64127)
						ClearBindData();
					else
						CanDataWheat.EngineRotate = TmpInt16.IntII;          	//发动机转速
						
					CanDataWheat.WaterTemp 		= *(p + 2);    				//冷却水温度
					CanDataWheat.EngineOil 		= *(p + 3);    				//机油压力
					CanDataWheat.FuelPercent    = *(p + 4);  				//燃料百分比

					TmpInt32.TTbyte[0] 			= *(p + 5);
					TmpInt32.TTbyte[1] 			= *(p + 6);
					TmpInt32.TTbyte[2] 			= *(p + 7);
					TmpInt32.TTbyte[3] 			= *(p + 8);
					CanDataWheat.EngineWorkTime = TmpInt32.LongLL; 			//发动机工作时间  

										
					TmpInt16.TTbyte[0] 			= *(p + 9);
					TmpInt16.TTbyte[1] 			= *(p + 10);
					CanDataWheat.SysVal 		= TmpInt16.IntII;          //系统电压
					
					TmpInt32.TTbyte[0] 			= *(p + 11);
					TmpInt32.TTbyte[1] 			= *(p + 12);
					TmpInt32.TTbyte[2] 			= *(p + 13);
					TmpInt32.TTbyte[3] 			= *(p + 14);
					CanDataWheat.TotalTravlled  = TmpInt32.LongLL;     		//总行驶里程

					TmpInt16.TTbyte[0] 			= *(p + 15);
					TmpInt16.TTbyte[1] 			= *(p + 16);
					CanDataWheat.TravlledSpeed  = TmpInt16.IntII;    		//行驶速度 

					TmpInt16.TTbyte[0] 			= *(p + 17);
					TmpInt16.TTbyte[1] 			= *(p + 18);
					CanDataWheat.Rotate1 	  	= TmpInt16.IntII;       	//滚筒转速 
					//SL_Print("APP:>%d\r\n",CanDataWheat.Rotate1);
					TmpInt16.TTbyte[0] 			= *(p + 19);
					TmpInt16.TTbyte[1] 			= *(p + 20);
					CanDataWheat.Rotate2		= TmpInt16.IntII;       	//复脱器转速 

					TmpInt16.TTbyte[0] 			= *(p + 21);
					TmpInt16.TTbyte[1] 			= *(p + 22);
					CanDataWheat.Rotate3 		= TmpInt16.IntII;        	//籽粒升运器

					TmpInt16.TTbyte[0] 			= *(p + 23);
					TmpInt16.TTbyte[1] 			= *(p + 24);
					CanDataWheat.Rotate4		= TmpInt16.IntII;        	//清选风扇转速

					TmpInt16.TTbyte[0] 			= *(p + 25);
					TmpInt16.TTbyte[1] 			= *(p + 26);
					CanDataWheat.Rotate5		= TmpInt16.IntII;        	//喂入搅龙转速  

					TmpInt16.TTbyte[0] 			= *(p + 27);
					TmpInt16.TTbyte[1] 			= *(p + 28);
					CanDataWheat.Rotate6		= TmpInt16.IntII;        	//过桥转速 

					TmpInt16.TTbyte[0] 			= *(p + 29);
					TmpInt16.TTbyte[1] 			= *(p + 30);
					CanDataWheat.LossRatio 		= TmpInt16.IntII;         	//清选损失率
					
					CanDataWheat.CabTemp        = *(p + 31);  				//驾驶室温度
					CanDataWheat.ReapHigh       = *(p + 32);  				//割茬高度
					CanDataWheat.Aperture1      = *(p + 33);  				//上筛开度
					CanDataWheat.Aperture2      = *(p + 34); 				//下筛开度
					CanDataWheat.Aperture3      = *(p + 35); 				//尾筛开度
					CanDataWheat.WarnValue      = *(p + 36); 				//粮满报警  
					
					break;
				case 0x0A:			// 玉米机
					p = TerRecvBuf + 12;

					TmpInt16.TTbyte[0] 			= *(p);
					TmpInt16.TTbyte[1] 			= *(p + 1);
					
					if(TmpInt16.IntII == 64127)
						ClearBindData();               //判断清除盲区
					else
						CanDataMaize.EngineRotate 	= TmpInt16.IntII;      		//发动机转速
					
					CanDataMaize.WaterTemp 		= *(p + 2);    				//冷却水温度
					CanDataMaize.EngineOil 		= *(p + 3);    				//机油压力
					CanDataMaize.FuelPercent 	= *(p + 4);  				//燃料百分比

					TmpInt32.TTbyte[0] 			= *(p + 5);
					TmpInt32.TTbyte[1] 			= *(p + 6);
					TmpInt32.TTbyte[2] 			= *(p + 7);
					TmpInt32.TTbyte[3] 			= *(p + 8);
					CanDataMaize.EngineWorkTime = TmpInt32.LongLL;    		//发动机工作时间   

					TmpInt16.TTbyte[0] 			= *(p + 9);
					TmpInt16.TTbyte[1] 			= *(p + 10);
					CanDataMaize.SysVol 		= TmpInt16.IntII;     		//系统电压

					TmpInt32.TTbyte[0] 			= *(p + 11);
					TmpInt32.TTbyte[1] 			= *(p + 12);
					TmpInt32.TTbyte[2] 			= *(p + 13);
					TmpInt32.TTbyte[3] 			= *(p + 14);
					CanDataMaize.TotalTravlled 	= TmpInt32.LongLL;     		//总行驶里程

					TmpInt16.TTbyte[0] 			= *(p + 15);
					TmpInt16.TTbyte[1] 			= *(p + 16);
					CanDataMaize.Rotate1 		= TmpInt16.IntII;       	//剥皮机转速

					TmpInt16.TTbyte[0] 			= *(p + 17);
					TmpInt16.TTbyte[1] 			= *(p + 18);
					CanDataMaize.Rotate2 		= TmpInt16.IntII;       	//滚动轮转速

					TmpInt16.TTbyte[0] 			= *(p + 19);
					TmpInt16.TTbyte[1] 			= *(p + 20);
					CanDataMaize.TravlledSpeed 	= TmpInt16.IntII;     		//行驶速度  
					
					TmpInt16.TTbyte[0] 			= *(p + 21);
					TmpInt16.TTbyte[1] 			= *(p + 22);
					CanDataMaize.ReapHigh 		= TmpInt16.IntII;    		//割茬高度
					
					CanDataMaize.WarnValue 		= *(p + 23);         		//梁满报警，主离合状态
					break;
				case 0x09:			//水稻机 
					p = TerRecvBuf + 12;

					TmpInt16.TTbyte[0] 			= *(p);
					TmpInt16.TTbyte[1] 			= *(p + 1);

					if(TmpInt16.IntII == 64127)
						ClearBindData();
					else
						CanDataPaddy.EngineRotate 	= TmpInt16.IntII;      		//发动机转速
					
					CanDataPaddy.WaterTemp 		= *(p + 2);    				//冷却水温度

					TmpInt16.TTbyte[0] 			= *(p + 3);
					TmpInt16.TTbyte[1] 			= *(p + 4);
					CanDataPaddy.Rotate1 			= TmpInt16.IntII;       //杂余转速

					TmpInt16.TTbyte[0] 			= *(p + 5);
					TmpInt16.TTbyte[1] 			= *(p + 6);
					CanDataPaddy.Rotate2 		= TmpInt16.IntII;        	//滚动轮转速

					TmpInt16.TTbyte[0] 			= *(p + 7);
					TmpInt16.TTbyte[1] 			= *(p + 8);
					CanDataPaddy.SysVol 		= TmpInt16.IntII;           //系统电压

					TmpInt32.TTbyte[0] 			= *(p + 9);
					TmpInt32.TTbyte[1] 			= *(p + 10);
					TmpInt32.TTbyte[2] 			= *(p + 11);
					TmpInt32.TTbyte[3] 			= *(p + 12);
					CanDataPaddy.EngineWorkTime 	= TmpInt32.LongLL;    	//发动机工作时间  
					
					CanDataPaddy.WarnValue 		= *(p + 13);         		//粮满报警

					TmpInt16.TTbyte[0] 			= *(p + 14);
					TmpInt16.TTbyte[1] 			= *(p + 15);
					CanDataPaddy.TravlledSpeed 	= TmpInt16.IntII;     		//行驶速度  
					
					TmpInt32.TTbyte[0] 			= *(p + 16);
					TmpInt32.TTbyte[1] 			= *(p + 17);
					TmpInt32.TTbyte[2] 			= *(p + 18);
					TmpInt32.TTbyte[3] 			= *(p + 19);					
					CanDataPaddy.TotalTravlled	= TmpInt32.LongLL;     		//总行驶里程	
					break;
				case 0x03:          //拖拉机  
					p = TerRecvBuf + 12;

					TmpInt16.TTbyte[0] 			= *(p);
					TmpInt16.TTbyte[1] 			= *(p + 1);

					if(TmpInt16.IntII == 64127)
						ClearBindData();
					else
						CanDataTractor.EngineRotate = TmpInt16.IntII;			//发动机转速  
					
					CanDataTractor.WaterTemp 	= *(p + 2); 				//冷却水温度
					CanDataTractor.EngineOil 	= *(p + 3); 				//机油压力
					CanDataTractor.FuelPercent 	= *(p + 4);					//燃料百分比

					TmpInt32.TTbyte[0] 			= *(p + 5);
					TmpInt32.TTbyte[1] 			= *(p + 6);
					TmpInt32.TTbyte[2] 			= *(p + 7);
					TmpInt32.TTbyte[3] 			= *(p + 8);
					CanDataTractor.EngineWorkTime = TmpInt32.LongLL;		//发动机工作时间 

					TmpInt16.TTbyte[0] 			= *(p + 9);
					TmpInt16.TTbyte[1] 			= *(p + 10);
					CanDataTractor.SysVol 		= TmpInt16.IntII;			//系统电压

					TmpInt16.TTbyte[0] 			= *(p + 11);
					TmpInt16.TTbyte[1] 			= *(p + 12);
					CanDataTractor.TravlledSpeed 	= TmpInt16.IntII; 		//行驶速度

					TmpInt32.TTbyte[0] 			= *(p + 13);
					TmpInt32.TTbyte[1] 			= *(p + 14);
					TmpInt32.TTbyte[2] 			= *(p + 15);
					TmpInt32.TTbyte[3] 			= *(p + 16);
					CanDataTractor.TotalTravlled 	= TmpInt32.LongLL; 		//总行驶里程
					
					CanDataTractor.WarnValue 		= *(p + 17); 			//粮满报警
					break;   
				default:
					
					break;
			}
			
			break;
		case 0x0101:
			GetTerTimeBig = 1;
			p = TerRecvBuf + 8;
			//SL_Print(">App Recv TerTime \r\n");
			TerTime.Year 		= *(p);                 //
			TerTime.Mon 		= *(p + 1);
			TerTime.MDay 		= *(p + 2);
			TerTime.Hour 		= *(p + 3);
			TerTime.Min 		= *(p + 4);
			TerTime.Sec 		= *(p + 5);
			TerTime.VerifyBit 	= 1;
			TerTime.TimeSec     = CalendarToUnix(TerTime);    //计算出时间秒  
			//SL_Print(">App:The TerTime:%d\r\n",TerTime.TimeSec);
			break;
		case 0x00FE:
			//SL_MEMBLOCK(TerRecvBuf,resize,16);
			p = TerRecvBuf + 6;

			TmpInt16.TTbyte[0] = *p;
			TmpInt16.TTbyte[1] = *(p + 1);
			DataLen = TmpInt16.IntII;
			p = TerRecvBuf + 8;
			if(LookForStr(p,"AT+Test",sizeof("AT+Test")) >= 0)         			//进入生产测试
			{
				pSysCfg->CfgFlag = 0x56;
				SL_Print("AT+Test\r\n");
				break;
			}
			if(LookForStr(p,"HOMER3ETESTOVER!",sizeof("HOMER3ETESTOVER!")) >= 0)  		//结束配置，回到正常模式
			{				  
				pSysCfg->CfgFlag = 0x55;
				SaveSysCfg();                                        			//保存配置  
				pTcpIp = GetTcpIpSocketSpce();
				pTcpIp->ResetLink = 1;                               			//重新链接
				SL_Print("BDWTESTOVER!\r\n");
				break;
			}
			if((i = LookForStr(p,"BDWMODIF",sizeof("BDWMODIF"))) >= 0)   		//配置
			{
				p =p + i + 8;
				
				if(*(p) != ':')
					break;
				p += 1;
				for(i = 0;i < 16;i++)                                  			//去设备地址 
					pSysCfg->TerminalID[i] = *(p + i);
				
				pSysCfg->TerminalID[i] = '\0';
				p += 16;
				if(*p != ',')
					break;
				
				tmp = GetDataStr(1,2,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					SL_Memcpy(pSysCfg->ServerAddr,TmpStr,tmp + 1);          	//服务器地址  
					
				}
				
				tmp = GetDataStr(2,3,p,TmpStr,DataLen); 
				if(tmp > 0)
				{	
					TmpStr[tmp] = '\0';
					pSysCfg->ServerPort = StrToDouble(TmpStr);         			//服务器端口
				}
				
				tmp = GetDataStr(3,4,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					pSysCfg->SleepTime = StrToDouble(TmpStr);          			//休眠时间
				}
				
				tmp = GetDataStr(4,5,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					pSysCfg->DistanceUpLoad = StrToDouble(TmpStr);    			//定位报位
				}


				tmp = GetDataStr(5,6,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					pSysCfg->TravelUpLoadTime = StrToDouble(TmpStr);   			//定时报位时间
				}

				tmp = GetDataStr(6,7,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					pSysCfg->HardWareNum = StrToDouble(TmpStr);      			//硬件版本号
				}
				
				tmp = GetDataStr(7,8,p,TmpStr,DataLen);                   		 	//Can协议版本号    
				//SL_Print("pSysCfg->CanProtocolNum:%d\r\n",tmp);
				if(tmp > 0)
				{
					TmpStr[tmp] = '\0';
					pSysCfg->CanProtocolNum = StrToDouble(TmpStr);   			//Can协议版本号  
					//SL_Print("pSysCfg->CanProtocolNum:%d\r\n",pSysCfg->CanProtocolNum);
					if(pSysCfg->CanProtocolNum == 0x01)                        //玉米机
						pSysCfg->CarType = 0x0A; 
					else if(pSysCfg->CanProtocolNum == 0x02)                   //小麦机  新国三水稻机
						pSysCfg->CarType = 0x08;
					else if(pSysCfg->CanProtocolNum == 0x03)                   //水稻机新国三
						pSysCfg->CarType = 0x09;
					else if(pSysCfg->CanProtocolNum == 0x04)                   //拖拉机
						pSysCfg->CarType = 0x03;    
					else if(pSysCfg->CanProtocolNum == 0x06)                   //老国三 小麦机
						pSysCfg->CarType = 0x0E;
				}

				tmp = GetDataStr(8,9,p,TmpStr,DataLen);
				if(tmp > 0)
				{
					SL_Memcpy(pSysCfg->TerApn,TmpStr,tmp);           			//Apn 接入点方式
					pSysCfg->TerApn[tmp] = '\0';
				}
			}
			
			break;
		default:
			
			return 0;
					
	}
	return 1;
}


/**********************************************************
**	函数名称:
**	功能描述:负责与STM32交换数据  发送数据
**********************************************************/

void ProTerminalData(void)
{
	static unsigned char 		step = 0,step1 = 0,step2 = 0;
	static struct stopwatch16 	sw,sw1;

	SysTimeStr					Time;
	unsigned short int 			len;
	int 						tmp;
	SysConfigStr 				*pSys;                 //配置
	IntToChar					TmpInt16;              //临时数据 
	LongToChar					TmpInt32; 			   //临时数据
	unsigned char 				TmpStr[12];
	GNSSSTR 					*pGnss;                //定位信息
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
					if(read_stopwatch16_value(&sw) < 5)                       	     //延时250
						break;
					SL_UartSendData(SL_UART_1,GetStatus,sizeof(GetStatus));    		//读取设备状态信息 主要是ACC
					GetStatusBig = 0;
					init_stopwatch16(&sw); 
					step++;
					break;
				case 4:
					if(GetStatusBig == 0 && read_stopwatch16_value(&sw) < 20)   	//延时 500ms
						break;
					step++;                                                         
					init_stopwatch16(&sw); 
					break;
				case 5:
					if(read_stopwatch16_value(&sw) < 5)
						break;
					SL_UartSendData(SL_UART_1,GetCAN,sizeof(GetCAN));           	//读CAN信息
					 
					bitclr(GetCanBig,0);       							        	//
					init_stopwatch16(&sw);
					step++;
				case 6:
					if((GetCanBig & 0x01) == 0 && read_stopwatch16_value(&sw) < 20)    //等待  500
						break;
					if((GetCanBig & 0x01) == 0)
						bitset(GetCanBig,1);  
					else
						bitclr(GetCanBig,1);
					step++;
					init_stopwatch16(&sw);
					break;
				case 7:                                                            //读取设备时间
					if(read_stopwatch16_value(&sw) < 5)             			   //延时250ms，
						break;
					SL_UartSendData(SL_UART_1,GetTerTime,sizeof(GetTerTime));      //读设备时间
					GetTerTimeBig = 0;
					init_stopwatch16(&sw);
					step++;
					break;
				case 8:
					if(GetTerTimeBig == 0 && read_stopwatch16_value(&sw) < 20)
						break;
					init_stopwatch16(&sw);
					
					if(pTime->VerifyBit > 0)
					{
						step = 10;
						pTime->VerifyBit = 0;
						break;
					}
					pSys = GetSysCfgSpace();          //获取配置
					if(pSys->CfgFlag == 0x55)
					{
						step = 2;                     //
						break;
					}
					step++;
					break;
				case 9:                               //发送配置信息
					if(read_stopwatch16_value(&sw) < 10)
						break;
					pSys = GetSysCfgSpace();
					len = 0;
					TerSendBuf[len++] = 0x5F;
					TerSendBuf[len++] = 0x5F;
					TerSendBuf[len++] = 0;//长度，总长度-4
					TerSendBuf[len++] = 0;
					TerSendBuf[len++] = 0xFE;
					TerSendBuf[len++] = 0;
					TerSendBuf[len++] = 0;//数据长度，总长度-8
					TerSendBuf[len++] = 0;
					TerSendBuf[len++] = 'h';
					TerSendBuf[len++] = 'o';
					TerSendBuf[len++] = 'm';
					TerSendBuf[len++] = 'e';
					TerSendBuf[len++] = 'r';
					TerSendBuf[len++] = '3';
					TerSendBuf[len++] = 'x';
					TerSendBuf[len++] = ':';
					
					for(tmp = 0;tmp < 16;tmp++)
						TerSendBuf[len++] = pSys->TerminalID[tmp];   			//设备号
					TerSendBuf[len] = ',';                               		//逗号1
					len += 1;
					pTcpIp = GetTcpIpSocketSpce();
					tmp = SL_Strlen(pTcpIp->SimIccId);
					SL_Memcpy(TerSendBuf + len,pTcpIp->SimIccId,tmp);  			//设备ICC-ID号
					len += tmp;
					TerSendBuf[len] = ',';                                		//逗号2
					len += 1;
					
					tmp =SL_Strlen(pSys->ServerAddr);
					SL_Memcpy(TerSendBuf + len,pSys->ServerAddr,tmp);     //服务器地址
					len += tmp;
					TerSendBuf[len] = ',';                                //逗号3
					len += 1;
					SL_itoa(pSys->ServerPort,TmpStr,10);
					for(tmp = 0;tmp < SL_Strlen(TmpStr);tmp++)            //服务器端口
						TerSendBuf[len++] = TmpStr[tmp];
					TerSendBuf[len++] = ',';

					tmp = SL_Strlen(pSys->TerApn);
					SL_Memcpy(TerSendBuf + len,pSys->TerApn,tmp);        // APNGprs接入点  
					len += tmp;
					TerSendBuf[len++] = ',';                             //逗号3
					
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
					
					tmp = pGnss->latitude * 1000000;                   //维度
					SL_Memset(TmpStr,0x30,12);
					UInt32ToStr(tmp,TmpStr,12);
					SL_Memcpy(TerSendBuf + len,TmpStr,9);              //复制到发送缓冲区
					len += 9;
					tmp = pGnss->longitude * 1000000;                  //经度
					SL_Memset(TmpStr,0x30,12);
					UInt32ToStr(tmp,TmpStr,12);                        
					SL_Memcpy(TerSendBuf + len,TmpStr,9);              //复制到发送缓冲区
					len += 9;

					SL_Memcpy(TerSendBuf + len,"00000000",8);         //对接8个0
					len += 8;

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
					
					CharToStr(pGnss->satellite_num,TmpStr,2);       //可用卫星数量
					SL_Memcpy(TerSendBuf + len,TmpStr,2);
					len += 2;
					TerSendBuf[len++] = ',';

					CharToStr(pTcpIp->GsmSign,TmpStr,2);
					SL_Memcpy(TerSendBuf + len,TmpStr,2);           //信号值
					len += 2;
					TerSendBuf[len++] = ',';
					
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
						
					TerSendBuf[len++] = pTcpIp->LinkState + 0x30;      //GSM网络状态  0:未注册;1:已注册;2:已连接。
					TerSendBuf[len++] = ',';
						
					TerSendBuf[len++] = '2';                           //研发定义的内部信息                
					TerSendBuf[len++] = '6';
					TerSendBuf[len++] = ',';

					SL_Memset(TmpStr,0x30,12);
					tmp = UInt32ToStr(pSys->SleepTime,TmpStr,12);      //睡眠时间
					SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
					len += tmp;
					TerSendBuf[len++] = ',';

					SL_Memset(TmpStr,0x30,12);
					tmp = UInt32ToStr(pSys->DistanceUpLoad,TmpStr,12);      //报位置距离
					SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
					len += tmp;
					TerSendBuf[len++] = ',';

					SL_Memset(TmpStr,0x30,12);
					tmp = UInt32ToStr(pSys->TravelUpLoadTime,TmpStr,12);      //时间报位时间
					SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
					len += tmp;
					TerSendBuf[len++] = ',';

					SL_Memset(TmpStr,0x30,12);
					tmp = UInt32ToStr(pSys->HardWareNum,TmpStr,12);      //时间报位时间
					SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
					len += tmp;
					TerSendBuf[len++] = ',';
					TerStateData.SdCardState = 1;  						// 默认状态SD状态1
					TerSendBuf[len++] = TerStateData.SdCardState + 0x30;    //SD卡状态  1:功能正常，0:功能异常
					TerSendBuf[len++] = ',';

					TerSendBuf[len++] = TerStateData.E2PROMState + 0x30;	//EEPROM状态 1:功能正常
					TerSendBuf[len++] = ',';

					TerSendBuf[len++] = '1';								// 外部RTC状态；1:功能正常        (目前写为1)
					TerSendBuf[len++] = ',';

					TerSendBuf[len++] = TerStateData.AntConState + 0x30;   //天线状态  0:正常
					TerSendBuf[len++] = ',';

					TerSendBuf[len++] = TerStateData.ShellState + 0x30; 	//光敏状态
					TerSendBuf[len++] = ',';

					TerSendBuf[len++] = '1';                                //Key1状态
					TerSendBuf[len++] = ',';

					TerSendBuf[len++] = '1';								//Key2状态
					TerSendBuf[len++] = ',';

					tmp = GetVerNum();
					UInt32ToStr(tmp,TmpStr,12);      						//Can协议版本号
					SL_Memcpy(TerSendBuf + len,TmpStr,2);
					len += 2;
					tmp = sizeof("010101") - 1;                            //
					SL_Memcpy(TerSendBuf + len,"010101",tmp);
					len += tmp;
					TerSendBuf[len++] = ',';

					tmp = UInt32ToStr(pSys->CanProtocolNum,TmpStr,12);      //Can协议版本号
					SL_Memcpy(TerSendBuf + len,TmpStr,tmp);
					len += tmp;
					TerSendBuf[len++] = ',';

					//外部CAN链接状态
					if(TerStateData.CanStatus == 0)
						TerSendBuf[len++] = '0';                                //
					else
						TerSendBuf[len++] = '1';                                //
					
					TerSendBuf[len++] = ',';
						
					TerSendBuf[len++] = 0x0D;                               	//回车
					TerSendBuf[len++] = 0x0A;
					
					TerSendBuf[len] = 0x00;
					TerSendBuf[len + 1] = 0x00;
					TerSendBuf[len + 2] = 0x55;
					TerSendBuf[len + 3] = 0xAA;
					len += 4;
					TmpInt16.IntII = len -6;                 				//总长度
					TerSendBuf[2] = TmpInt16.TTbyte[0];
					TerSendBuf[3] = TmpInt16.TTbyte[1];
					TmpInt16.IntII	= len - 12;             				//数据长度
					TerSendBuf[6]	=TmpInt16.TTbyte[0];
					TerSendBuf[7]	=TmpInt16.TTbyte[1];
					
					SL_UartSendData(SL_UART_1,TerSendBuf,len);      		//
					
					step = 2;   											//
					init_stopwatch16(&sw);
					break;
				case 10:
					if(read_stopwatch16_value(&sw) < 5)
						break;
					step = 2;
					SendVerifyTimeCmd(pTime);              //发送校准时间  
					SL_Print("Verfy STM32 Time:%d\r\n",pTime->VerifyBit);
					init_stopwatch16(&sw);
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
					SL_MEMBLOCK(UpgradeData->FileBuf[UpgradeData->FileTotalFram - 1],UpgradeData->FileLen[UpgradeData->FileTotalFram - 1],16);
					
					len = 0;
					SL_Memset(TerSendBuf,0,1200);          //
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
					SL_UartSendData(SL_UART_1,TerSendBuf,len);      //发送出去 
					init_stopwatch16(&sw1);
					step1++;
					break;
				case 1:                                                //开始升级  升级的过程中需要添加  
					if(read_stopwatch16_value(&sw1) > 4800)        //1分钟内要升级结束，却不结束
					{
						SL_FreeMemory(UpgradeData);
						RunMode = 0;         
						UpgradeStatus.UpgradeStatus = 1;     //升级失败 
						//保存升级状态 
						return;
					}
					tmp = ReadComPkt(1,TerRecvBuf,sizeof(TerRecvBuf));    //
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
					
					TmpInt16.TTbyte[0] = TerRecvBuf[10];      //数据帧号码
					TmpInt16.TTbyte[1] = TerRecvBuf[11];
					/*
					if(TmpInt16.IntII >= UpgradeData->FileTotalFram)
					{	
						SL_FreeMemory(UpgradeData);
						RunMode = 0;         
						UpgradeStatus.UpgradeStatus = 1;     //升级失败
						//保存升级状态 
						return;
					}
					*/
					UpgradeData->FileCurrentFram = TmpInt16.IntII - 1;                  //当前数据帧号码  
					SL_Print("UpgradeData->FileCurrentFram:%d\r\n",UpgradeData->FileCurrentFram);
					
					len = 0;
					SL_Memset(TerSendBuf,0,1200);    //清空数据
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
					SL_Print("UpgradeData->FileCurrentFram %d\r\n",UpgradeData->FileLen[UpgradeData->FileCurrentFram]);

					TmpInt16.IntII= CheckSum16(TerSendBuf + 12,len - 12);            //校验 
					
					TerSendBuf[len++] = TmpInt16.TTbyte[0];
					TerSendBuf[len++] = TmpInt16.TTbyte[1];
					SL_Print("CheckSum16: %d\r\n",TmpInt16.IntII);
					TerSendBuf[len++] = 0x55;
					TerSendBuf[len++] = 0xAA;
					SL_UartSendData(SL_UART_1,TerSendBuf,len);      //
					SL_Print("Run This  %d\r\n",len);
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
					step2++;
					break;
				case 1:
					step2++;
					break;
				default:
					step2 = 0;
					break;
			}
			break;
	}
	
}




/**************************File End*************************/


