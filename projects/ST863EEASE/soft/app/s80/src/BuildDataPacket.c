

/******************************************
**	FileName:
**	Time:
**	构建上传数据包
**
*******************************************/

#include "include.h"


static unsigned char 				TmpBuf[512];					//数据缓冲区
static unsigned char 				MsgLine;   						//行标识
static unsigned char 				MsgNum;							//消息体计数�

static unsigned short int 			MsgSerial;						//消息序列码



/**********************************************************************************************
**	函数名称:unsigned short int BuildPositionPacket(unsigned char *source,unsigned char big)
**	功能描述:
***********************************************************************************************/

unsigned short int BuildPositionPacket(unsigned char *source,unsigned char big)
{	
	static unsigned short int DataLen = 0;
	unsigned short int tmp = 0;
	
	MsgHeadStr 			TmpMsg;
	unsigned char 		buf[50];
	SysConfigStr 		*pSysCfg;
	
	MsgHeadStr 	 		*pMsgHead;
	MsgGnssStr   		*pMsgGnss;
	MsgFarmStr   		*pMsgFarm;
	MsgDeviceStr 		*pMsgDevice;

	GNSSSTR 	 		*pGnss;
	SysTimeStr 	 		*pTime;
	TerStateStr			*pTerState;     //系统设备状态，采集到的数据
	CanDataStr 			*pCanData;
	
	pSysCfg = GetSysCfgSpace();
	pTerState = GetTerStateDataSpace();

	MsgLine++;
	if(MsgLine <= 1)
	{
		TmpMsg.frame_start[0] = 0xF1;
		TmpMsg.frame_start[1] = 0xF2;
		TmpMsg.frame_start[2] = 0xFF;
		TmpMsg.msg_id = 0x45;
		
		SL_Memcpy(&TmpMsg.device_id,pSysCfg->TerminalID,16);
		TmpMsg.blind_flag = 0x00;
		TmpMsg.msg_body_num = 0x00;
		TmpMsg.msg_len = 0x00;
		SL_Memcpy(TmpBuf,(unsigned char *)&TmpMsg,25);           			//
		DataLen = 25;

		pMsgDevice = (MsgDeviceStr *)buf;
		pMsgDevice->msg_device_type = 0x0001;               	 			//消息体类型码(设备状态消息体)
		pMsgDevice->msg_device_len = 7;                             		//消息体长度
 
		pMsgDevice->msg_device_manu_num 		= 0x01;                  	//设备生产厂家名称
		pMsgDevice->msg_device_manu_type 		= 0x01;                 	//
		pMsgDevice->msg_device_user_num 		= 22;                   	//用户厂家编号
		pMsgDevice->msg_device_car_num 			= pSysCfg->CarType;         //车型
		pMsgDevice->msg_device_protocol_ver 	= pTerState->McuFirNum;		//单片机程序版本
		pMsgDevice->msg_device_firware_ver 		= GetVerNum();              //固件程序版本号
		pMsgDevice->msg_device_hardware_ver 	= pSysCfg->HardWareNum;     //硬件版本号
		
		SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgDeviceStr)); 
		DataLen += sizeof(MsgDeviceStr);
		MsgNum = 1;
		//MsgNum++;
	}

	//定位消息体
	pMsgGnss = (MsgGnssStr *)buf;
	pGnss = GetGnssDataSpace();
	pTime = GetSysTimeSpace();
	
	pMsgGnss->msg_gps_type = 0x0100;                   				//   消息类型码
	pMsgGnss->msg_gps_len = 26;                                     //	消息数据区域长度
	pMsgGnss->msg_gps_latitude = pGnss->latitude * 1000000;         //   定位信息-经度
	pMsgGnss->msg_gps_longitude = pGnss->longitude * 1000000;      	//	定位信息-维度
	pMsgGnss->msg_gps_speed = pGnss->speed * 10;					//	定位信息-速度
	pMsgGnss->msg_gps_azimuth  = pGnss->azimuth;         			//   定位信息-航向角度
	pMsgGnss->msg_gps_altitude = pGnss->altitude;        			//	定位信息-海拔高度

	pMsgGnss->msg_gps_year = pTime->Year;							//   定位信息-时间
	pMsgGnss->msg_gps_mon = pTime->Mon;								//   
	pMsgGnss->msg_gps_day = pTime->MDay;							//
	pMsgGnss->msg_gps_hour = pTime->Hour;							//
	pMsgGnss->msg_gps_min = pTime->Min;								//
	pMsgGnss->msg_gps_sec = pTime->Sec;								//

	pMsgGnss->msg_gps_satellite_num = pGnss->satellite_num;			//  使用卫星数据
	pMsgGnss->msg_gps_view_num = pGnss->satellite_view_num;			//  可视卫星数量
	pMsgGnss->msg_gps_ghdop_v = pGnss->ghdop_v;						//  水平定位因子
	
	if(pGnss->status == 'A')                                    //定位状态标识
		pMsgGnss->msg_gps_status = pGnss->status | 0x02;
	else
		pMsgGnss->msg_gps_status = pGnss->status & 0xFD;

	pMsgGnss->msg_gps_line = MsgLine;
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgGnssStr)); 
	DataLen += sizeof(MsgGnssStr);
	MsgNum++;

	//设备时间
	pMsgFarm = (MsgFarmStr *)buf;

	pCanData = GetCanDataSpace();
	pMsgFarm->MsgFarmType = 0x0980;                                 //消息体类型码
	pMsgFarm->MsgFarmLen = 9;                                       //消息体数据域长度
	
	switch(pSysCfg->CarType)
	{
		case 0x09:
			pMsgFarm->MsgFarmBig = pCanData->Rotate1;       //水稻机
			break;
		case 0x0f:
			pMsgFarm->MsgFarmBig = pCanData->EngineRotate / 8;       //水稻机,老国三
			break;
		case 0x03:
		case 0x0D:
			pMsgFarm->MsgFarmBig = pCanData->EngineRotate / 8;                        //拖拉机 
			//SL_Print("(1)pMsgFarm->MsgFarmBig:%d\r\n",pMsgFarm->MsgFarmBig);
			break;
		default:
			pMsgFarm->MsgFarmBig = 0;
			break;
	}
	pMsgFarm->MsgTimeYear = pTime->Year;							//
	pMsgFarm->MsgTimeMon = pTime->Mon;								//
	pMsgFarm->MsgTimeDay = pTime->MDay;								//
	pMsgFarm->MsgTimeHour = pTime->Hour;							//
	pMsgFarm->MsgTimeMin = pTime->Min;                              //
	pMsgFarm->MsgTimeSec = pTime->Sec;                              //
	pMsgFarm->MsgLine = MsgLine;                                    //
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgFarmStr)); 
	DataLen += sizeof(MsgFarmStr);
	MsgNum++;

	if(MsgLine >= 10 || big > 0)    								//打包结束                     
	{
		pMsgHead = (MsgHeadStr *)TmpBuf; 
		pMsgHead->msg_line = MsgLine;
		pMsgHead->msg_body_num = MsgNum;
		//SL_Print("BuildPositionPacket:%d\r\n",pMsgHead->msg_body_num);
		pMsgHead->msg_len = DataLen - 23; 	//加上随机码2个字节
		MsgSerial++;
		SL_Memcpy(TmpBuf + DataLen,(unsigned char *)&MsgSerial,2);
		DataLen += 2;
		*(TmpBuf + DataLen) = BccVerify(TmpBuf + 3,DataLen - 3);
		DataLen++;
		*(TmpBuf + DataLen) = 0x0D;
		DataLen++;
		SL_Memcpy(source,TmpBuf,DataLen);
		tmp = DataLen;
		//SL_Print(">App:BuildCompletePacket 1...... %d\r\n",DataLen);
		MsgNum = 0;
		DataLen = 0;
		MsgLine = 0;
	}
	return tmp;
}	



/******************************************************
**	函数名称:
**	功能描述:
******************************************************/

unsigned short int BuildCompletePacket(unsigned char *source,unsigned char big)
{
	MsgHeadStr TmpMsg;
	unsigned short int DataLen;
	unsigned char buf[450];
	SysConfigStr *pSysCfg;

	//消息体指针
	MsgHeadStr 	 		*pMsgHead;              //消息头
	MsgDeviceStr 		*pMsgDevice;            //设备状态消息体
	MsgInputStr  		*pMsgInput;             //输入消息体
	MsgGnssStr   		*pMsgGnss;              //定位消息体
	MsgFarmStr   		*pMsgFarm;              //作业消息体
	MsgCanPaddyStr		*pMsgCanPaddy;			//
	MsgCanTractorStr	*pMsgCanTractor;		//

	MsgOilStr			*pMsgOil;
	
	
	//数据指针
	GNSSSTR 			*pGnss;         //定位模块数据
	TerStateStr			*pTerState;     //系统设备状态，采集到的数据
	SysTimeStr 			*pSysTime;      //系统时间
	CurrentFaultCodeStr	*pFaultCode1;   //当前故障码
	CanDataStr 			*pCanData;
	
	pSysCfg = GetSysCfgSpace();       	//获取设备配置信息
	
	DataLen = 0;                     	//标识数据长度
	big = 0;
	MsgNum = 0;
	
	//消息头
	
	TmpMsg.frame_start[0] = 0xF1;
	TmpMsg.frame_start[1] = 0xF2;
	TmpMsg.frame_start[2] = 0xFF;
	TmpMsg.msg_id = 0x45;
	SL_Memcpy(&TmpMsg.device_id,pSysCfg->TerminalID,16);            //设备号
	TmpMsg.blind_flag = 0x00;                                       //
	TmpMsg.msg_body_num = 0x00;                                     //
	TmpMsg.msg_len = 0x00;                                          //
	SL_Memcpy(TmpBuf,(unsigned char *)&TmpMsg,25);          		//
	DataLen = 25;
	
	//设备状态消息体:

	pTerState = GetTerStateDataSpace();
	
	pMsgDevice = (MsgDeviceStr *)buf;
	pMsgDevice->msg_device_type = 0x0001;               		 	//
	pMsgDevice->msg_device_len = 8;									//
	pMsgDevice->msg_device_manu_num = 0x01;                  		//
	pMsgDevice->msg_device_manu_type = 0x01;                  	 	//
	pMsgDevice->msg_device_user_num = 22;                    		//用户编号
	pMsgDevice->msg_device_car_num = pSysCfg->CarType;				//pSysCfg->CarType;            //车型编号  
	pMsgDevice->msg_device_firware_ver = GetVerNum();               //ST86程序
	pMsgDevice->msg_device_protocol_ver = pTerState->McuFirNum;     //单片机程序
	pMsgDevice->msg_device_hardware_ver = pSysCfg->HardWareNum;		//硬件程序
	
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgDeviceStr));    
	DataLen += sizeof(MsgDeviceStr); 
	*(TmpBuf + DataLen) = 1;
	DataLen += 1;
	MsgNum++;
	
	//输入状态数据
	
	pMsgInput = (MsgInputStr *)buf;
	
	pMsgInput->msg_input_type = 0x0004;                             //消息体类型码(输入状态消息体)
	pMsgInput->msg_input_len = 24;                                  //消息体长度
	pMsgInput->msg_input_io = 0x00;
	pMsgInput->msgAccState = ReadAccState();						//ACC状态  
	pMsgInput->msgMotoState = 0;
	pMsgInput->msgFrq1 = 0;
	pMsgInput->msgFrq2 = 0;
	pMsgInput->msgPWM1 = 0;
	pMsgInput->msgPWM2 =  0;
	pMsgInput->msgInputVol1 = 0;
	pMsgInput->msgInputVol2 = 0;
	pMsgInput->msgPowVol = pTerState->PowVol * 100;					//外部供电电压
	pMsgInput->msgBatteryVol = pTerState->Battery * 100;			//内部电池电压
	pMsgInput->msgWarningState = GetWarningValue();          			
	pMsgInput->msgLine = 1;
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgInputStr)); 	
	DataLen += sizeof(MsgInputStr);
	MsgNum++;
	
	
	//定位消息体
	pGnss = GetGnssDataSpace();
	pSysTime = GetSysTimeSpace();
	pMsgGnss = (MsgGnssStr *)buf;
	pMsgGnss->msg_gps_type = 0x0100;                   								//
	pMsgGnss->msg_gps_len = 26;                                     				//
	pMsgGnss->msg_gps_latitude = pGnss->latitude * 1000000;        					//
	pMsgGnss->msg_gps_longitude = pGnss->longitude * 1000000;      					//  
	pMsgGnss->msg_gps_speed = pGnss->speed * 10;									//
	pMsgGnss->msg_gps_azimuth  = pGnss->azimuth;         							//
	pMsgGnss->msg_gps_altitude = pGnss->altitude;        							//
	pMsgGnss->msg_gps_year = pSysTime->Year;
	pMsgGnss->msg_gps_mon = pSysTime->Mon;
	pMsgGnss->msg_gps_day = pSysTime->MDay;
	pMsgGnss->msg_gps_hour = pSysTime->Hour;
	pMsgGnss->msg_gps_min = pSysTime->Min;
	pMsgGnss->msg_gps_sec = pSysTime->Sec;
	
	pMsgGnss->msg_gps_satellite_num = pGnss->satellite_num;
	pMsgGnss->msg_gps_view_num = pGnss->satellite_view_num;
	pMsgGnss->msg_gps_ghdop_v = pGnss->ghdop_v;

	if(pGnss->status == 'A')
		pMsgGnss->msg_gps_status = pGnss->status | 0x02;
	else
		pMsgGnss->msg_gps_status = pGnss->status & 0xFD;

	pMsgGnss->msg_gps_line = 1;
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgGnssStr)); 
	DataLen += sizeof(MsgGnssStr);
	MsgNum++;
	
	//设备时间
	pMsgFarm = (MsgFarmStr *)buf;

	pMsgFarm->MsgFarmType = 0x0980;
	pMsgFarm->MsgFarmLen = 9;

	pCanData = GetCanDataSpace();          //获取所有机型CAN数据
	switch(pSysCfg->CarType)
	{
		case 0x09:
			pMsgFarm->MsgFarmBig = pCanData->Rotate1;       //水稻机
			break;
		case 0x0f:
			pMsgFarm->MsgFarmBig = pCanData->EngineRotate / 8;       //水稻机,老国三
			break;
		case 0x03:                                           //新国三M平台拖拉机
		case 0x0D:                                           //老国三M平台拖拉机
			pMsgFarm->MsgFarmBig = pCanData->EngineRotate / 8;                        //拖拉机  
			//SL_Print("(2)pMsgFarm->MsgFarmBig:%d\r\n",pMsgFarm->MsgFarmBig);
			break;
		default:
			pMsgFarm->MsgFarmBig = 0;
			break;
	}
	
	pMsgFarm->MsgTimeYear = pSysTime->Year;
	//SL_Print("The Year:%d\r\n",pMsgFarm->MsgTimeYear);
	pMsgFarm->MsgTimeMon = pSysTime->Mon;
	pMsgFarm->MsgTimeDay = pSysTime->MDay;
	pMsgFarm->MsgTimeHour = pSysTime->Hour;
	pMsgFarm->MsgTimeMin = pSysTime->Min;
	pMsgFarm->MsgTimeSec = pSysTime->Sec;
	pMsgFarm->MsgLine = 1;
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgFarmStr)); 
	DataLen += sizeof(MsgFarmStr);
	MsgNum++;
	switch(pSysCfg->CarType)
	{
		case 0x09:  							// 水稻机
			pMsgCanPaddy = (MsgCanPaddyStr *)buf;

			pMsgCanPaddy->MsgCanPaddyType = 0x0014;				   				//
			pMsgCanPaddy->MsgCanPaddyLen = sizeof(MsgCanPaddyStr) - 4;			//
			
			pMsgCanPaddy->MsgEngineRotate = pCanData->EngineRotate;     		//发动机转速
			pMsgCanPaddy->MsgWaterTemp = pCanData->WaterTemp;    				//冷却水温度
			pMsgCanPaddy->MsgRotate1 = pCanData->Rotate1;        				//工作速度判断
			pMsgCanPaddy->MsgRotate2 = pCanData->Rotate2;        				//滚动轮转速
			pMsgCanPaddy->MsgSysVol = pCanData->SysVol;            			//系统电压
			pMsgCanPaddy->MsgEngineWorkTime = pCanData->EngineWorkTime;    	//发动机工作时间 
			pMsgCanPaddy->MsgWarnValue = pCanData->WarnValue1;         			//粮满报警
			pMsgCanPaddy->MsgTravlledSpeed = pCanData->TravlledSpeed;     		//行驶速度
			pMsgCanPaddy->MsgTotalTravlled = pCanData->TotalTravlled;     		//总行驶里程

			pMsgCanPaddy->MsgEngineOil =  pCanData->EngineOil; 			//机油压力
			pMsgCanPaddy->MsgFuelPercent = pCanData->FuelPercent;			//燃料百分比
//			SL_Print("Oil Data............%d\r\n",pMsgCanPaddy->MsgEngineOil);
			pMsgCanPaddy->MsgLine1 = 1;
			
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanPaddyStr)); 
			DataLen += sizeof(MsgCanPaddyStr);
			MsgNum++;

			pMsgOil = (MsgOilStr *)buf;

			pMsgOil->MsgOilType = 0x0013;
			pMsgOil->MsgOilLen = 9;

			pMsgOil->MsgOil = pCanData->OilConsumption;
			pMsgOil->MsgOilTotal = pCanData->TatalOilConsumption;
			pMsgOil->MsgLine = 1;
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgOilStr)); 
			DataLen += sizeof(MsgOilStr);
			//SL_Print("Oil Data.........................3\r\n");
			MsgNum++;
			break;
		case 0x0F:  //  老国三水稻机
			pMsgCanPaddy = (MsgCanPaddyStr *)buf;

			pMsgCanPaddy->MsgCanPaddyType = 0x000E;				   				//
			pMsgCanPaddy->MsgCanPaddyLen = sizeof(MsgCanPaddyStr) - 6;			//
			
			pMsgCanPaddy->MsgEngineRotate = pCanData->EngineRotate;     		//发动机转速
			pMsgCanPaddy->MsgWaterTemp = pCanData->WaterTemp;    				//冷却水温度
			pMsgCanPaddy->MsgRotate1 = pCanData->Rotate1;        				//工作速度判断
			pMsgCanPaddy->MsgRotate2 = pCanData->Rotate2;        				//滚动轮转速
			pMsgCanPaddy->MsgSysVol = pCanData->SysVol;            				//系统电压
			pMsgCanPaddy->MsgEngineWorkTime = pCanData->EngineWorkTime;    		//发动机工作时间 
			pMsgCanPaddy->MsgWarnValue = pCanData->WarnValue1;         			//粮满报警
			pMsgCanPaddy->MsgTravlledSpeed = pCanData->TravlledSpeed;     		//行驶速度
			pMsgCanPaddy->MsgTotalTravlled = pCanData->TotalTravlled;     		//总行驶里程
			pMsgCanPaddy->MsgLine1 = 1;
			
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanPaddyStr) - 2);             //
			DataLen = DataLen + sizeof(MsgCanPaddyStr) - 2;
			MsgNum++;
			break;
		case 0x03:                                  //M平台拖拉机
			pMsgCanTractor = (MsgCanTractorStr *)buf;

			pMsgCanTractor->MsgCanTractorType = 0x000F; 				 		//消息体类型码  
			pMsgCanTractor->MsgCanTractorLen = sizeof(MsgCanTractorStr) - 4;	//消息体长度
			
			pMsgCanTractor->MsgEngineRotate = pCanData->EngineRotate;		//发动机转速
			pMsgCanTractor->MsgWaterTemp =	pCanData->WaterTemp; 			//冷却水温度
			pMsgCanTractor->MsgEngineOil =  pCanData->EngineOil; 			//机油压力
			pMsgCanTractor->MsgFuelPercent = pCanData->FuelPercent;			//燃料百分比
			pMsgCanTractor->MsgEngineWorkTime = pCanData->EngineWorkTime;	//发动机工作时间 
			pMsgCanTractor->MsgSysVol =	pCanData->SysVol;					//系统电压
			pMsgCanTractor->MsgTravlledSpeed = pCanData->TravlledSpeed; 		//行驶速度
			pMsgCanTractor->MsgTotalTravlled =	pCanData->TotalTravlled; 	//总行驶里程
			pMsgCanTractor->MsgWarnValue = pCanData->WarnValue1; 				//粮满报警
			pMsgCanTractor->MsgLine = 1;
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanTractorStr)); 
			DataLen += sizeof(MsgCanTractorStr);
			MsgNum++;

			pMsgOil = (MsgOilStr *)buf;

			pMsgOil->MsgOilType = 0x0013;
			pMsgOil->MsgOilLen = 9;

			pMsgOil->MsgOil = pCanData->OilConsumption;
			pMsgOil->MsgOilTotal = pCanData->TatalOilConsumption;
			pMsgOil->MsgLine = 1;
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgOilStr)); 
			DataLen += sizeof(MsgOilStr);
			//SL_Print("Oil Data.........................3\r\n");
			MsgNum++;
			break;
		case 0x0D:                              //M平台拖拉机 老国三
			pMsgCanTractor = (MsgCanTractorStr *)buf;

			pMsgCanTractor->MsgCanTractorType = 0x000F; 				 		//消息体类型码  
			pMsgCanTractor->MsgCanTractorLen = sizeof(MsgCanTractorStr) - 4;	//消息体长度
			
			pMsgCanTractor->MsgEngineRotate = pCanData->EngineRotate;		//发动机转速
			pMsgCanTractor->MsgWaterTemp =	pCanData->WaterTemp; 			//冷却水温度
			pMsgCanTractor->MsgEngineOil =  pCanData->EngineOil; 			//机油压力
			pMsgCanTractor->MsgFuelPercent = pCanData->FuelPercent;			//燃料百分比
			pMsgCanTractor->MsgEngineWorkTime = pCanData->EngineWorkTime;	//发动机工作时间 
			pMsgCanTractor->MsgSysVol =	pCanData->SysVol;					//系统电压
			pMsgCanTractor->MsgTravlledSpeed = pCanData->TravlledSpeed; 		//行驶速度
			pMsgCanTractor->MsgTotalTravlled =	pCanData->TotalTravlled; 	//总行驶里程
			pMsgCanTractor->MsgWarnValue = pCanData->WarnValue1; 				//粮满报警
			pMsgCanTractor->MsgLine = 1;
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanTractorStr)); 
			DataLen += sizeof(MsgCanTractorStr);
			MsgNum++;
			break;
		default:
			break;
	}

	//以下是故障码
	pFaultCode1 = GetCurrentFaultCodeSpace();     //当前故障码

	if(pFaultCode1->FaultCodeNum > 0 && pFaultCode1->FaultCodeNum <= 160)     //有故障码且在当前范围内，进行打包
	{
		IntToChar TmpInt16;

		TmpInt16.IntII = 0x0011;                      //类型编码
		buf[0] = TmpInt16.TTbyte[0];
		buf[1] = TmpInt16.TTbyte[1];

		TmpInt16.IntII = pFaultCode1->FaultCodeNum * 4 + 2;   //消息体长度  消息体长度都是8的倍数
				
		buf[2] = TmpInt16.TTbyte[0];
		buf[3] = TmpInt16.TTbyte[1];
				
		SL_Memcpy(TmpBuf + DataLen,buf,4);           //长度和类型码 
		DataLen += 4;
				
		*(TmpBuf + DataLen) = pFaultCode1->FaultCodeNum;   //上传故障码数量。故障码数量为
		DataLen++;
				
		SL_Memcpy(TmpBuf + DataLen,pFaultCode1->FaultCode,pFaultCode1->FaultCodeNum * 4);
		DataLen += pFaultCode1->FaultCodeNum * 4;

		*(TmpBuf + DataLen) = 0x01;
		DataLen += 1;
		MsgNum++;
			
	}
			
	pMsgHead = (MsgHeadStr *)TmpBuf;
	pMsgHead->msg_len = DataLen - 23;          //加上随机码长度  
	
	pMsgHead->msg_body_num = MsgNum;  
	//SL_Print("BuildCompletePacket:%d\r\n",pMsgHead->msg_body_num);
	pMsgHead->msg_line = 1;
	MsgSerial++;
	SL_Memcpy(TmpBuf + DataLen,(unsigned char *)&MsgSerial,2); 
	
	DataLen += 2;
	
	
	*(TmpBuf + DataLen) = BccVerify(TmpBuf + 3,DataLen - 3);
	DataLen++;
	*(TmpBuf + DataLen) = 0x0D;
	DataLen++;
	SL_Memcpy(source,TmpBuf,DataLen);

	SL_Print(">App:BuildCompletePacket 2...... %d\r\n",DataLen);

	return DataLen;
	
}



/**************************File End***************************/






