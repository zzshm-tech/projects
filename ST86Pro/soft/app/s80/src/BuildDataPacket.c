

/******************************************
**	FileName:
**	Time:
**	构建上传数据包
*******************************************/

#include "include.h"


static unsigned char TmpBuf[512];
static unsigned char MsgLine;   						//行标识
static unsigned char MsgNum;
static unsigned short int MsgSerial;



/*********************************************
**	函数名称:
**	功能描述:
*********************************************/

unsigned short int BuildPositionPacket(unsigned char *source,unsigned char big)
{	
	static unsigned short int DataLen = 0;
	unsigned short int tmp = 0;
	
	MsgHeadStr TmpMsg;
	unsigned char buf[50];
	SysConfigStr *pSysCfg;
	
	MsgHeadStr 	 *pMsgHead;
	MsgGnssStr   *pMsgGnss;
	MsgFarmStr   *pMsgFarm;
	MsgDeviceStr *pMsgDevice;

	GNSSSTR 	 *pGnss;
	SysTimeStr 	 *pTime;

	CanWheatStr  		*pWheat;
	CanMaizeStr 		*pMaize;
	CanPaddyStr 		*pPaddy;
	CanTractorStr 		*pTractor;
	
	pSysCfg = GetSysCfgSpace();
	//MsgNum = 0;
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
		pMsgDevice->msg_device_protocol_ver 	= 18;                		//上传协议版本号
		pMsgDevice->msg_device_firware_ver 		= GetVerNum();              //固件程序版本号
		pMsgDevice->msg_device_hardware_ver 	= pSysCfg->HardWareNum;     //硬件版本号
		
		SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgDeviceStr)); 
		DataLen += sizeof(MsgDeviceStr);
		MsgNum++;
	}

	//定位消息体
	pMsgGnss = (MsgGnssStr *)buf;
	pGnss = GetGnssDataSpace();
	pTime = GetSysTimeSpace();
	
	pMsgGnss->msg_gps_type = 0x0100;                   				//
	pMsgGnss->msg_gps_len = 26;                                     //
	pMsgGnss->msg_gps_latitude = pGnss->latitude * 1000000;         //
	pMsgGnss->msg_gps_longitude = pGnss->longitude * 1000000;      	//
	pMsgGnss->msg_gps_speed = pGnss->speed * 10;					//
	pMsgGnss->msg_gps_azimuth  = pGnss->azimuth;         			//
	pMsgGnss->msg_gps_altitude = pGnss->altitude;        			//
	pMsgGnss->msg_gps_year = pTime->Year;							//
	pMsgGnss->msg_gps_mon = pTime->Mon;								//
	pMsgGnss->msg_gps_day = pTime->MDay;							//
	pMsgGnss->msg_gps_hour = pTime->Hour;							//
	pMsgGnss->msg_gps_min = pTime->Min;								//
	pMsgGnss->msg_gps_sec = pTime->Sec;								//
	pMsgGnss->msg_gps_satellite_num = pGnss->satellite_num;			//
	pMsgGnss->msg_gps_view_num = pGnss->satellite_view_num;			//
	pMsgGnss->msg_gps_ghdop_v = pGnss->ghdop_v;						//
	
		if(pGnss->status == 'A')
		pMsgGnss->msg_gps_status = pGnss->status | 0x02;
	else
		pMsgGnss->msg_gps_status = pGnss->status & 0xFD;
	
	pMsgGnss->msg_gps_status = pGnss->status;						//
	pMsgGnss->msg_gps_line = MsgLine;
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgGnssStr)); 
	DataLen += sizeof(MsgGnssStr);
	MsgNum++;

	//设备时间
	pMsgFarm = (MsgFarmStr *)buf;

	pWheat = GetCanWheatSpace();     	//小麦机
	pPaddy = GetCanPaddySpace();     	//水稻机
	pMaize = GetCanMaizeSpace();     	//玉米机  
	pTractor = GetCanTractorSpace();   	//拖拉机 
	pMsgFarm->MsgFarmType = 0x0980;                                 //消息体类型码
	pMsgFarm->MsgFarmLen = 9;                                       //消息体数据域长度
	switch(pSysCfg->CarType)
	{
		case 0x08:
			pMsgFarm->MsgFarmBig = pWheat->Rotate1;         		//流滚筒转速
			break;
		case 0x0E:
			pMsgFarm->MsgFarmBig = pWheat->EngineRotate / 8;    
			break;
		case 0x0A:
			pMsgFarm->MsgFarmBig = pMaize->Rotate1;
			break;
		case 0x09:
			pMsgFarm->MsgFarmBig = pPaddy->Rotate1;
			break;
		case 0x03:
			pMsgFarm->MsgFarmBig = 0;
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
		SL_Print(">App:CompletePacket Finsh  1...... %d\r\n",DataLen);
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
	unsigned char buf[50];
	SysConfigStr *pSysCfg;
	

	//消息体指针
	MsgHeadStr 	 		*pMsgHead;
	MsgDeviceStr 		*pMsgDevice;
	MsgInputStr  		*pMsgInput;
	MsgGnssStr   		*pMsgGnss;
	MsgFarmStr   		*pMsgFarm;
	MsgCanWheatStr 		*pMsgCanWheat;
	MsgCanPaddyStr		*pMsgCanPaddy;
	MsgCanMaizeStr		*pMsgCanMaize;
	MsgCanTractorStr	*pMsgCanTractor;
	
	//数据指针
	GNSSSTR 			*pGnss;
	TerStateStr			*pTerState;
	SysRunDataStr 		*pSysRun;
	SysTimeStr 			*pSysTime;      //系统时间
	CanWheatStr			*pCanWheat;     //小麦机
	CanPaddyStr			*pCanPaddy;     //水稻机
	CanMaizeStr			*pCanMaize;     //玉米机
	CanTractorStr		*pCanTractor;   //拖拉机
	
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

	
	pMsgDevice = (MsgDeviceStr *)buf;
	pMsgDevice->msg_device_type = 0x0001;               		 	//
	pMsgDevice->msg_device_len = 8;									//

	pMsgDevice->msg_device_manu_num = 0x01;                  		//
	pMsgDevice->msg_device_manu_type = 0x01;                  	 	//
	pMsgDevice->msg_device_user_num = 22;                    		//用户编号
	pMsgDevice->msg_device_car_num = pSysCfg->CarType;				//pSysCfg->CarType;            //车型编号  
	pMsgDevice->msg_device_firware_ver = GetVerNum();
	pMsgDevice->msg_device_protocol_ver = 18;                		//
	pMsgDevice->msg_device_hardware_ver = pSysCfg->HardWareNum;		//
	
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgDeviceStr));    
	DataLen += sizeof(MsgDeviceStr); 
	*(TmpBuf + DataLen) = 1;
	DataLen += 1;
	MsgNum++;
	
	//输入状态数据

	pTerState = GetTerStateDataSpace();
	pSysRun  = GetSysRunDataSpace();
	
	pMsgInput = (MsgInputStr *)buf;
	
	pMsgInput->msg_input_type = 0x0004;                             //消息体类型码(输入状态消息体)
	pMsgInput->msg_input_len = 24;                                  //消息体长度
	pMsgInput->msg_input_io = 0x00;
	pMsgInput->msgAccState = 1;                						//ACC状态
	pMsgInput->msgMotoState = 0;
	pMsgInput->msgFrq1 = 0;
	pMsgInput->msgFrq2 = 0;
	pMsgInput->msgPWM1 = 0;
	pMsgInput->msgPWM2 =  0;
	pMsgInput->msgInputVol1 = 0;
	pMsgInput->msgInputVol2 = 0;
	pMsgInput->msgPowVol = pTerState->PowVol * 100;//外部供电电压
	pMsgInput->msgBatteryVol = pTerState->Battery * 100;//内部电池电压
	pMsgInput->msgACCTotalTime = pSysRun->ACCTotalHour * 10;                      			//ACC总开启时间
	pMsgInput->msgLine = 1;
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgInputStr)); 	
	DataLen += sizeof(MsgInputStr);
	MsgNum++;
	
	
	//定位消息体
	pGnss = GetGnssDataSpace();
	pSysTime = GetSysTimeSpace();
	pMsgGnss = (MsgGnssStr *)buf;
	pMsgGnss->msg_gps_type = 0x0100;                   														//
	pMsgGnss->msg_gps_len = 26;                                     						//
	pMsgGnss->msg_gps_latitude = pGnss->latitude * 1000000;        					//
	pMsgGnss->msg_gps_longitude = pGnss->longitude * 1000000;      					//
	pMsgGnss->msg_gps_speed = pGnss->speed * 10;									 						//?ù?è
	pMsgGnss->msg_gps_azimuth  = pGnss->azimuth;         										//???ˉ·??ò
	pMsgGnss->msg_gps_altitude = pGnss->altitude;        										//o??ò??
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

	pCanWheat = GetCanWheatSpace();     	//小麦机
	pCanPaddy = GetCanPaddySpace();     	//水稻机
	pCanMaize = GetCanMaizeSpace();     	//玉米机  
	pCanTractor = GetCanTractorSpace();   	//拖拉机 

	switch(pSysCfg->CarType)
	{
		case 0x08:
			pMsgFarm->MsgFarmBig = pCanWheat->Rotate1;   //小麦机 新国三 
			break;
		case 0x0E:
			pMsgFarm->MsgFarmBig = pCanWheat->EngineRotate / 8;
			break;
		case 0x0A:
			pMsgFarm->MsgFarmBig = pCanMaize->Rotate1;       //玉米机
			break;
		case 0x09:
			pMsgFarm->MsgFarmBig = pCanPaddy->Rotate1;       //水稻机
			break;
		case 0x03:
			pMsgFarm->MsgFarmBig = 0;                        //拖拉机
			break;
		default:
			pMsgFarm->MsgFarmBig = 0;
			break;
	}
	
	pMsgFarm->MsgTimeYear = pSysTime->Year;
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
		case 0x08:  //小麦机 
		case 0x0E:
			pMsgCanWheat = (MsgCanWheatStr *)buf;
			pMsgCanWheat->MsgCanWheatType = 0x000D;
			pMsgCanWheat->MsgCanWheatLen = sizeof(MsgCanWheatStr) - 4;
			pMsgCanWheat->MsgEngineRotate = pCanWheat->EngineRotate;      //发动机转速
			pMsgCanWheat->MsgWaterTemp = pCanWheat->WaterTemp;    		//冷却水温度
			pMsgCanWheat->MsgEngineOil = pCanWheat->EngineOil;    		//机油压力
			pMsgCanWheat->MsgFuelPercent = pCanWheat->FuelPercent;  		//燃料百分比
			pMsgCanWheat->MsgEngineWorkTime = pCanWheat->EngineWorkTime;    //发动机工作时间  
			pMsgCanWheat->MsgSysVal = pCanWheat->SysVal;            //系统电压
			pMsgCanWheat->MsgTotalTravlled = pCanWheat->TotalTravlled;     //总行驶里程
			pMsgCanWheat->MsgTravlledSpeed = pCanWheat->TravlledSpeed;     //行驶速度 
			pMsgCanWheat->MsgRotate1 = pCanWheat->Rotate1;        	//滚筒转速 
			pMsgCanWheat->MsgRotate2 = pCanWheat->Rotate2;        	//复脱器转速
			pMsgCanWheat->MsgRotate3 = pCanWheat->Rotate3;        	//籽粒升运器
			pMsgCanWheat->MsgRotate4 = pCanWheat->Rotate4;        	//清选风扇转速
			pMsgCanWheat->MsgRotate5 = pCanWheat->Rotate5;        	//喂入搅龙转速
			pMsgCanWheat->MsgRotate6 = pCanWheat->Rotate6;        	//过桥转速
			pMsgCanWheat->MsgLossRatio = pCanWheat->LossRatio;         //清选损失率
			pMsgCanWheat->MsgCabTemp = pCanWheat->CabTemp;           //驾驶室温度
			pMsgCanWheat->MsgReapHigh = pCanWheat->ReapHigh;          //割茬高度
			pMsgCanWheat->MsgAperture1 = pCanWheat->Aperture1;         //上筛开度
			pMsgCanWheat->MsgAperture2 = pCanWheat->Aperture2;         //下筛开度
			pMsgCanWheat->MsgAperture3 = pCanWheat->Aperture3;         //尾筛开度
			pMsgCanWheat->MsgWarnValue = pCanWheat->WarnValue;         //粮满报警
			pMsgCanWheat->MsgLine = 1;
			
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanWheatStr)); 
			DataLen += sizeof(MsgCanWheatStr);
			MsgNum++;
			//SL_Print(">APP:Xiao Mai Ji.........\r\n");
			break;
		case 0x0A:                                  //玉米机
			pCanMaize = GetCanMaizeSpace();
			pMsgCanMaize = (MsgCanMaizeStr *)buf;

			pMsgCanMaize->MsgCanMaizeType = 0x000C;
			pMsgCanMaize->MsgCanMaizeLen = sizeof(MsgCanMaizeStr) - 4;

			pMsgCanMaize->MsgEngineRotate = pCanMaize->EngineRotate;      		//发动机转速
			pMsgCanMaize->MsgWaterTemp = pCanMaize->WaterTemp;    				//冷却水温度
			pMsgCanMaize->MsgEngineOil = pCanMaize->EngineOil;    				//机油压力
			pMsgCanMaize->MsgFuelPercent = pCanMaize->FuelPercent;  			//燃料百分比
			pMsgCanMaize->MsgEngineWorkTime = pCanMaize->EngineWorkTime;    	//发动机工作时间 
			pMsgCanMaize->MsgSysVol = pCanMaize->SysVol;            			//系统电压
			pMsgCanMaize->MsgTotalTravlled = pCanMaize->TotalTravlled;     		//总行驶里程
			pMsgCanMaize->MsgRotate1 = pCanMaize->Rotate1;        				//剥皮机转速
			pMsgCanMaize->MsgRotate2 = pCanMaize->Rotate2;        				//滚动轮转速
			pMsgCanMaize->MsgTravlledSpeed = pCanMaize->TravlledSpeed;     		//行驶速度
			pMsgCanMaize->MsgReapHigh = pCanMaize->ReapHigh;          			//割茬高度
			pMsgCanMaize->MsgWarnValue = pCanMaize->WarnValue;         			//粮满报警
			pMsgCanMaize->MsgLine = 1;
			
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanMaizeStr)); 
			DataLen += sizeof(MsgCanMaizeStr);
			MsgNum++;
			//SL_Print(">APP:Yu Mi Ji..................\r\n");
			break;
		case 0x09:                                  							//水稻机  包括老国三 水稻机 
			pCanPaddy = GetCanPaddySpace();
			pMsgCanPaddy = (MsgCanPaddyStr *)buf;

			pMsgCanPaddy->MsgCanPaddyType = 0x000E;				   				//
			pMsgCanPaddy->MsgCanPaddyLen = sizeof(MsgCanPaddyStr) - 4;			//
			
			pMsgCanPaddy->MsgEngineRotate = pCanPaddy->EngineRotate;     		//发动机转速
			pMsgCanPaddy->MsgWaterTemp = pCanPaddy->WaterTemp;    				//冷却水温度
			pMsgCanPaddy->MsgRotate1 = pCanPaddy->Rotate1;        				//工作速度判断
			pMsgCanPaddy->MsgRotate2 = pCanPaddy->Rotate2;        				//滚动轮转速
			pMsgCanPaddy->MsgSysVol = pCanPaddy->SysVol;            			//系统电压
			pMsgCanPaddy->MsgEngineWorkTime = pCanPaddy->EngineWorkTime;    	//发动机工作时间 
			pMsgCanPaddy->MsgWarnValue = pCanPaddy->WarnValue;         			//粮满报警
			pMsgCanPaddy->MsgTravlledSpeed = pCanPaddy->TravlledSpeed;     		//行驶速度
			pMsgCanPaddy->MsgTotalTravlled = pCanPaddy->TotalTravlled;     		//总行驶里程
			pMsgCanPaddy->MsgLine = 1;
			
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanPaddyStr)); 
			DataLen += sizeof(MsgCanPaddyStr);
			MsgNum++;
			//SL_Print(">App:Shui Dao Ji.....................\r\n");
			break;								
		case 0x03:
			pCanTractor = GetCanTractorSpace();
			pMsgCanTractor = (MsgCanTractorStr *)buf;

			pMsgCanTractor->MsgCanTractorType = 0x000F; 				 		//消息体类型码  
			pMsgCanTractor->MsgCanTractorLen = sizeof(MsgCanTractorStr) - 4;	//消息体长度
			
			pMsgCanTractor->MsgEngineRotate = pCanTractor->EngineRotate;		//发动机转速
			pMsgCanTractor->MsgWaterTemp =	pCanTractor->WaterTemp; 			//冷却水温度
			pMsgCanTractor->MsgEngineOil =  pCanTractor->EngineOil; 			//机油压力
			pMsgCanTractor->MsgFuelPercent = pCanTractor->FuelPercent;			//燃料百分比
			pMsgCanTractor->MsgEngineWorkTime = pCanTractor->EngineWorkTime;	//发动机工作时间 
			pMsgCanTractor->MsgSysVol =	pCanTractor->SysVol;					//系统电压
			pMsgCanTractor->MsgTravlledSpeed = pCanTractor->TravlledSpeed; 		//行驶速度
			pMsgCanTractor->MsgTotalTravlled =	pCanTractor->TotalTravlled; 	//总行驶里程
			pMsgCanTractor->MsgWarnValue = pCanTractor->WarnValue; 				//粮满报警
			pMsgCanTractor->MsgLine = 1;
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanTractorStr)); 
			DataLen += sizeof(MsgCanTractorStr);
			MsgNum++;
			//SL_Print("M Tuo La ji..............................\r\n");
			break;								//拖拉机
		default:
			break;
	}
	
	pMsgHead = (MsgHeadStr *)TmpBuf;
	pMsgHead->msg_len = DataLen - 23;          //加上随机码长度  
	pMsgHead->msg_body_num = MsgNum;
	pMsgHead->msg_line = 1;
	MsgSerial++;
	SL_Memcpy(TmpBuf + DataLen,(unsigned char *)&MsgSerial,2); 
	
	DataLen += 2;
	
	
	*(TmpBuf + DataLen) = BccVerify(TmpBuf + 3,DataLen - 3);
	DataLen++;
	*(TmpBuf + DataLen) = 0x0D;
	DataLen++;
	SL_Memcpy(source,TmpBuf,DataLen);

	SL_Print(">App:CompletePacket Finsh 2...... %d\r\n",DataLen);

	return DataLen;
	
}



/************************************************************
**	函数名称:
**	功能描述:
************************************************************/



/**************************File End***************************/

