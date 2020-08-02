

/******************************************
**	FileName:BuildDataPacket.c
**	Time:2018.2.24
*******************************************/

#include "include.h"


/******************************************/

static unsigned char 				TmpBuf[180];					//

static unsigned char 				MsgLine;   						//

static unsigned char 				MsgNum;							//

static unsigned short int 			MsgSerial;						//

static unsigned char 				TerMsgBig = 0;					//


void ResetTerMsgBig(void)
{
	TerMsgBig = 0;
}


/******************************************************
**	函数名称:unsigned short int BuildCompletePacket(unsigned char *source,unsigned char big)
**	功能描述:
******************************************************/

unsigned short int BuildCompletePacket(unsigned char *source,unsigned char big)
{
	static unsigned short int	DataLen = 0;

	MsgHeadStr 					TmpMsg;
	unsigned char				buf[100];
	MsgHeadStr 	 				*pMsgHead;         //
	SysConfigStr				*pSysCfg;          //
	MsgGnssStr					*pMsgGnss;         //
	MsgDeviceStr				*pMsgDevice;       //
	MsgInputStr 				*pMsgInput;        //
	MsgLoaderStr 				*pMsgLoader;       //
	GNSSSTR 					*pGnss;            //
	TerStateStr					*pTerState;        //
	CanDataStr					*pCanData;         //
	SysTimeStr					*pSysTime;         //
	CurrentFaultCodeStr			*pFaultCode1;      //
	//
	MsgNum = 0;
	MsgLine = 0;
	
	pSysCfg = GetSysCfgSpace();
	TmpMsg.frame_start[0] = 0xF1;									//
	TmpMsg.frame_start[1] = 0xF2;									//
	TmpMsg.frame_start[2] = 0xFF;									//
	TmpMsg.msg_id = 0x45;											//(45正常的数据，46盲区数据)
	SL_Memcpy(&TmpMsg.device_id,pSysCfg->TerminalID,16);         	//设备号 16位
	TmpMsg.CarTypeNum = 0x02;  										//车型编号
	TmpMsg.DataPackFlag = 0x02;   								 	//数据表示（0: 农机平台记亩 1:农机非平台记亩（或者不记亩）2:工程机械）
	
	TmpMsg.msg_body_num = 0x00;                                     //消息内部的消息体个数
	TmpMsg.msg_len = 0x00;                                          //消息长度
	SL_Memcpy(TmpBuf,(unsigned char *)&TmpMsg,25);          		//消息体序号
	DataLen = 25;													//

	pTerState = GetTerStateDataSpace();             				//
	//
	if(TerMsgBig == 0)
	{   //设备终端消息体				
		pMsgDevice = (MsgDeviceStr *)buf;               				//
		pMsgDevice->MsgDeviceType = 0x0001;              	 			//消息体类型编码   
		pMsgDevice->MsgDeviceLen = sizeof(MsgDeviceStr) - 4;  			//消息体数据区域长度

		pMsgDevice->MsgManuNum = 0x01;            						//消息体数据域-厂家编码   		
		pMsgDevice->MsgTerminalType = 0x01;        						//消息体数据域-终端型号 
		pMsgDevice->MsgUserNum = 22;                     				//消息体数据域-使用方编号
		pMsgDevice->MsgAppVer1 = (unsigned char)pTerState->FirNum;      //消息体数据域-单片机固件程序版本号
		pMsgDevice->MsgAppVer2 = GetVerNum();           				//消息体数据域-ST86应用程序版本号
		pMsgDevice->MsgHardwareVer = 20;             					//硬件版本号
		SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgDeviceStr));			// 
		DataLen += sizeof(MsgDeviceStr); 
		MsgNum++;
		TerMsgBig++;
		//SL_Print("Add TerMsg Block...........\r\n");
	}
	
	//终端状态信息消息体
	pMsgInput = (MsgInputStr *)buf;             						//
	
	pMsgInput->MsgInputType = 0x0004;									//类型编码
	pMsgInput->MsgInputLen = sizeof(MsgInputStr) - 4;					//消息体数据域长度

	pMsgInput->MsgInputIo = pTerState->IOStatus.vaule;					//开关量状态
	pMsgInput->MsgAcc = pTerState->AccState;    						//ACC状态（电锁状态）
	pMsgInput->MsgMoto = pTerState->MonState;							//MOTO状态
	pMsgInput->MsgInputFrq1 = 0;										//FRQ1输入，数字量输入频率（暂时不适用）
	pMsgInput->MsgInputFrq2 = 0;										//FRQ2输入，数字量输入频率（暂时不适用）
	pMsgInput->MsgInputFrq3 = 0;										//PWM1，输出频率（暂时不适用）
	pMsgInput->MsgInputFrq4 = 0;										//PWM2，输出频率（暂时不适用）
	pMsgInput->MsgPowVol = pTerState->PowerVol * 100;	 				//输入电压1,模拟量采集-设备供电电压
	pMsgInput->MsgBatteryVol = pTerState->BatteryVol * 100;				//输入电压2，模拟量采集-设备电池电压
	pMsgInput->MsgInputVol1 = 0;										//输入电压3，（暂时不适用）
	pMsgInput->MsgInputVol2 = 0;										//输入电压4，（暂时不适用）
	pMsgInput->MsgWarnValue = GetWarningValue();						//设备报警值
	/*
	设备报警说明：
	**	Bit0:	电源断开报警
	**	Bit1:	外部电源电压低报警
	**	Bit2:	MCU断开报警
	**	Bit3:	开外壳报警
	**	Bit4:	CAN链接断开
	**	Bit5:	GNSS天线断开报警
	**	Bit6:	GNSS天线短路报警 
	**	Bit7:	
	**	Bit8：
	**	Bit9:	
	**	Bit10
	**	Bit11:	
	**	Bit12
	**	Bit13:	
	**	Bit14
	**	Bit15:	
	**	Bit16
	**	Bit17:	
	**	Bit18
	**	Bit19:	
	**	Bit20
	*/
	//SL_Print("The Warning Value:%x\r\n",GetWarningValue());			//调试使用
	pMsgInput->MsgLine = 1;												//消息体序号
 	 
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgInputStr)); 
	DataLen += sizeof(MsgInputStr);
	MsgNum++;

	//高精度GNSS定位信息
	pMsgGnss = (MsgGnssStr *)buf;
	pGnss = GetGnssDataSpace();											//
	pSysTime = GetSysTimeSpace();                 						//
	pMsgGnss->MsgGnssType = 0x0100;										//类型编码
	pMsgGnss->MsgGnssLen = sizeof(MsgGnssStr) - 4;						//消息体数据域长度
		
	pMsgGnss->MsgGnssLatitude = pGnss->latitude * 1000000;				//维度，纬度X1000000后上传	
	pMsgGnss->MsgGnssLongitude = pGnss->longitude * 1000000;			//经度，经度X1000000后上传
	//SL_Print("The Gnss Option:%u,%u\r\n",pMsgGnss->MsgGnssLatitude,pMsgGnss->MsgGnssLongitude);
	pMsgGnss->MsgGnssSpeed = (unsigned char)pGnss->speed;				//速度，单位公里/小时，保留1位小数，上传数据为实际数据的10倍
	pMsgGnss->MsgGnssAzimuth = pGnss->azimuth;							//正北为0度顺时针增加，单位2度
	pMsgGnss->MsgGnssAltitude = pGnss->altitude;						//单位：米，高位第一位表示正负高度，为0表示海拔为正高度，为1表示海拔为负高度，表示范围-32767～32767米。
	pMsgGnss->MsgGnssYear = pSysTime->Year; 							//北京时间-年
	pMsgGnss->MsgGnssMon = pSysTime->Mon;								//北京时间-月
	pMsgGnss->MsgGnssDay = pSysTime->MDay;								//北京时间-日
	pMsgGnss->MsgGnssHour = pSysTime->Hour; 							//北京时间-时
	pMsgGnss->MsgGnssMin = pSysTime->Min;								//北京时间-分
	pMsgGnss->MsgGnssSec = pSysTime->Sec;								//北京时间-秒
		
	pMsgGnss->MsgGnssSatelliteNum = pGnss->satellite_num;				//当前正在使用的卫星颗数
	pMsgGnss->MsgGnssViewNum = pGnss->satellite_view_num;				//当前可看到的卫星颗数
	pMsgGnss->MsgGhdopV = pGnss->ghdop_v;								//水平定位因子
	if(pGnss->status == 'A')
		pMsgGnss->MsgGnssStatus = pGnss->status | 0x02; 				//GPS模块状态	  
	else
		pMsgGnss->MsgGnssStatus = pGnss->status & 0xFD;					//bit1:0 GPS不定位，1 GPS定位

	if(pGnss->longitude_ew > 0)  										//bit2:0 GPS模块正常，1 GPS模块异常
		pMsgGnss->MsgGnssStatus |= 0x01;
	else
		pMsgGnss->MsgGnssStatus &= 0xFE;

	//SL_Print("The Gnss Mode:%u\r\n",pMsgGnss->MsgGnssStatus);   

	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgGnssStr)); 				//
	DataLen += sizeof(MsgGnssStr);
	MsgNum++;

	
	//CAN数据消息体
	pMsgLoader = (MsgLoaderStr *)buf;    								//
	pCanData = GetCanDataSpace();
	//SL_Print("Run This Is :%d\r\n",pSysCfg->CarType);
	switch(pSysCfg->CarType)
	{ 
		case 0x02:						  											//
			pMsgLoader->MsgLoderMechType 		 	= 0x011F;						//类型编码
			pMsgLoader->MsgLoderMechLen           	= 68;        					//消息体数据域长度

			pMsgLoader->MsgSysVol					= pCanData->SysVol;           	 //电源电压
			pMsgLoader->MsgTempWater				= pCanData->TempWater;           //冷却水温度
			pMsgLoader->MsgEngineRotate				= pCanData->EngineRotate;		 //发动机转速
			pMsgLoader->MsgEngineRotateSet			= pCanData->EngineRotateSet;     //发动机目标转速
			pMsgLoader->MsgEngineTorque				= pCanData->EngineTorque;		 //发动机实际扭矩
			pMsgLoader->MsgFuelTemp					= pCanData->FuelTemp;			 //燃油温度
			pMsgLoader->MsgOilTemp					= pCanData->OilTemp;         	 //机油温度
			pMsgLoader->MsgAirPressure				= pCanData->AirPressure;         //大气压力
			pMsgLoader->MsgEngineNacelleTemp		= pCanData->EngineNacelleTemp;	 //发动机舱内部温度
			pMsgLoader->MsgAirTemp					= pCanData->AirTemp;			 //大气温度
			pMsgLoader->MsgEnteredAirTemp			= pCanData->EnteredAirTemp;		 //进气温度
			pMsgLoader->MsgWayTemp					= pCanData->WayTemp;			 //路面温度
			pMsgLoader->MsgEngineWorkTime			= pCanData->EngineWorkTime;		 //发动机工作时间
			pMsgLoader->MsgAccelerator				= pCanData->Accelerator;		 //加速踏板行程值
			pMsgLoader->MsgEngineLoad				= pCanData->EngineLoad;  		 //发动机负荷
			pMsgLoader->MsgTravelSpeed				= pCanData->TravelSpeed;		 //CAN速度（行驶速度）
			pMsgLoader->MsgOnceTravel				= pCanData->OnceTravel;			 //单次行驶里程
			pMsgLoader->MsgTotalTravel				= pCanData->TotalTravel;  		 //总里程
			pMsgLoader->MsgOnceFuel					= pCanData->OnceFuel;      		 //单次油耗
			pMsgLoader->MsgTotalFuel				= pCanData->TotalFuel;  		 //累计油耗
			pMsgLoader->MsgRelativeOilPressure		= pCanData->RelativeOilPressure; //相对机油压力
			pMsgLoader->MsgAbsoluteOilPressure		= pCanData->AbsoluteOilPressure; //绝对机油压力
			//SL_Print("The Mcu Status::%d\r\n",pMsgLoader->MsgEngineWorkTime);
			pMsgLoader->MsgRelativeAddPressure		= pCanData->RelativeAddPressure; //相对增压压力
			pMsgLoader->MsgAbsoluteAddPressure		= pCanData->AbsoluteAddPressure; //绝对增压压力
			pMsgLoader->MsgOilPosition				= pCanData->OilPosition;		 //机油液位
			pMsgLoader->MsgCrankcasePressure		= pCanData->CrankcasePressure;   //曲轴箱压力
			pMsgLoader->MsgCoolPressure  			= pCanData->CoolPressure;		 //冷却液压力
			pMsgLoader->MsgCoolPosition				= pCanData->CoolPosition;		 //冷却液液位
			pMsgLoader->MsglockCarStatus			= pCanData->LockCarState;		 //锁车状态
			//SL_Print("The Mcu Status::%d\r\n",pMsgLoader->MsglockCarStatus);
			pMsgLoader->MsgTerMonStatus				= pCanData->ActivateStatus;   	 //GPS监控功能
			pMsgLoader->MsgKeyStatus				= pCanData->KeyStatus;     		 //Key验证码
			pMsgLoader->MsgTerIDStatus				= pCanData->TerIDStatus;		 //GSPID匹配状态
			pMsgLoader->MsgColdBootStatus			= pCanData->ColdBootStatus;      //冷启动加热状态
			pMsgLoader->MsgSpeedStatus				= pCanData->SpeedStatus;		 //速度限制状态
			pMsgLoader->MsgPedalStatus				= pCanData->PedalStatus;     	 //踏板开关
			pMsgLoader->MsgAcceleratorStatus		= pCanData->AcceleratorStatus;   //加速踏板低怠速开关
			pMsgLoader->MsgClutchStatus				= pCanData->ClutchStatus;		 //离合开关状态
			pMsgLoader->MsgBrakeStatus				= pCanData->BrakeStatus;		 //刹车开关状态
			pMsgLoader->MsgCruiseSpeed				= pCanData->CruiseSpeed;		 //巡航控制设定速度
			pMsgLoader->MsgCruiseConStatus			= pCanData->CruiseConStatus;     //巡航控制状态
			pMsgLoader->MsgCruiseEnableStatus		= pCanData->CruiseEnableStatus;	 //巡航控制使能(不用，常设为01b)
			pMsgLoader->MsgCruiseAcitveStatus		= pCanData->CruiseAcitveStatus;	 //巡航控制激活状态标识
			pMsgLoader->MsgCarWarnValue1			= pCanData->CarWarnValue1;		 //Can报警
			pMsgLoader->MsgCarWarnValue2			= pCanData->CarWarnValue2;		 //Can报警
			//SL_Print("Can Data :%d\r\n",pMsgLoader->MsgSysVol);
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgLoaderStr)); 		//
			DataLen += sizeof(MsgLoaderStr);
			
			MsgNum++;
			break;
		default:
			break;
	}

	///*
	pFaultCode1 = GetCurrentFaultCodeSpace();	  //故障代码  （按照潍坊福田雷沃传送的）
	
	//SL_Print("Run This Is :%d\r\n",pFaultCode1->FaultCodeNum);   //
	//if(pFaultCode1->FaultCodeNum <= 160)	  		//
	//{
		IntToChar TmpInt16;
	
		TmpInt16.IntII = 0x0011;					 //消息类型
		buf[0] = TmpInt16.TTbyte[0];
		buf[1] = TmpInt16.TTbyte[1];
	
		TmpInt16.IntII = pFaultCode1->FaultCodeNum * 4 + 2;   //消息体
				
		buf[2] = TmpInt16.TTbyte[0];
		buf[3] = TmpInt16.TTbyte[1];
					
		SL_Memcpy(TmpBuf + DataLen,buf,4);			 //
		DataLen += 4;
				
		*(TmpBuf + DataLen) = pFaultCode1->FaultCodeNum;   //
		DataLen++;
		SL_Memcpy(TmpBuf + DataLen,pFaultCode1->FaultCode,pFaultCode1->FaultCodeNum * 4);
		DataLen += pFaultCode1->FaultCodeNum * 4;
		
		SL_Print("Build FaultCode Num:%d\r\n",pFaultCode1->FaultCodeNum);
		
		for(int i = 0;i < pFaultCode1->FaultCodeNum * 4;i++)
			SL_Print("The FaultCode:%x\r\n",pFaultCode1->FaultCode[i]);  //调试输出
		
		*(TmpBuf + DataLen) = 0x01;
		DataLen += 1;
		MsgNum++;			
	//}
	///*/

	pMsgHead = (MsgHeadStr *)TmpBuf;                   					//
	pMsgHead->msg_len = DataLen - 23;          							//
	pMsgHead->msg_body_num = MsgNum;  									//消息体个数（再打包）
	MsgSerial++;
	SL_Memcpy(TmpBuf + DataLen,(unsigned char *)&MsgSerial,2); 
	
	DataLen += 2;
	
	*(TmpBuf + DataLen) = BccVerify(TmpBuf + 3,DataLen - 3);
	DataLen++;
	*(TmpBuf + DataLen) = 0x0D;                        //包尾
	DataLen++;
	SL_Memcpy(source,TmpBuf,DataLen);

	SL_Print(">App:BuildCompletePacket...... %d, %d\r\n",MsgNum,DataLen);
	MsgNum = 0;
	return DataLen;
	
}



/************************************
**	向平台获取时间（新加内容）
**	和平台同步时间
*************************************/









/**************************File End***************************/









