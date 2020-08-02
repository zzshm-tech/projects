

/******************************************
**	FileName:BuildDataPacket.c
**	Time:2018.2.24
*******************************************/

#include "include.h"


/*************本地全局变量*****************/

static unsigned char 				TmpBuf[512];					//

static unsigned char 				MsgLine;   						//

static unsigned char 				MsgNum;							//

static unsigned short int 			MsgSerial;						//

static unsigned char 				TerMsgBig = 0;					//

static MsgGnssStr   				*pMulGnss;						//




void ResetTerMsgBig(void)
{
	TerMsgBig = 0;
}




/**********************************************************************************************
**	函数名称:
**	功能描述:
***********************************************************************************************/
///**
unsigned short int BuildPositionPacket(unsigned char *source,unsigned char big)
{	
	///*
	static unsigned short int DataLen = 0;
	unsigned short int tmp = 0;
	
	MsgHeadStr 			TmpMsg;
	unsigned char 		buf[100];
	SysConfigStr 		*pSysCfg;
	
	MsgHeadStr 	 		*pMsgHead;
	MsgGnssStr   		*pMsgGnss;
	MsgDeviceStr 		*pMsgDevice;
	MsgCanMaizeStr 		*pMsgMaize;       //
	GNSSSTR 	 		*pGnss;
	SysTimeStr 	 		*pSysTime;
	TerStateStr			*pTerState;     //
	CanDataStr 			*pCanData;
	
	pSysCfg = GetSysCfgSpace();
	pTerState = GetTerStateDataSpace();

	if(MsgLine <= 1)
	{
		TmpMsg.frame_start[0] = 0xF1;
		TmpMsg.frame_start[1] = 0xF2;
		TmpMsg.frame_start[2] = 0xFF;
		TmpMsg.msg_id = 0x45;
		
		SL_Memcpy(&TmpMsg.device_id,pSysCfg->TerminalID,16);

		TmpMsg.DataPackFlag = 0x00;
		TmpMsg.msg_body_num = 0x00;
		TmpMsg.msg_len = 0x00;
		SL_Memcpy(TmpBuf,(unsigned char *)&TmpMsg,25);           			//
		DataLen = 25;

		pMsgDevice = (MsgDeviceStr *)buf;
		pMsgDevice->MsgDeviceType = 0x0001;               	 			//
		pMsgDevice->MsgDeviceLen = 6;                             		//
 
		pMsgDevice->MsgManuNum = 0x01;            						//		
		pMsgDevice->MsgTerminalType = 0x01;        						//
		pMsgDevice->MsgUserNum = 32;                     				//
		pMsgDevice->MsgAppVer1 = (unsigned char)pTerState->FirNum;      //
		pMsgDevice->MsgAppVer2 = GetVerNum();           				//ST86程序版本
		pMsgDevice->MsgHardwareVer = pSysCfg->HardWareNum;				//硬件版本
		
		SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgDeviceStr)); 
		DataLen += sizeof(MsgDeviceStr);
		MsgNum = 1;
		
		//
		pMsgMaize = (MsgCanMaizeStr *)buf;    //
		pCanData = GetCanDataSpace();
		
		//SL_Print("Run This Is 1 :%d\r\n",pSysCfg->CarType);
		switch(pSysCfg->CarType)
		{ 
			case 0x0A:						  //
				pMsgMaize->MsgMaizeMechType = 0x000C;				//
				pMsgMaize->MsgMaizeMechLen = 78;        		//

				pMsgMaize->MsgSysVol = pCanData->SysVol;           	 		//
				pMsgMaize->MsgTempWater = pCanData->TempWater;           		//
				pMsgMaize->MsgEngineRotate = pCanData->EngineRotate;		 		//
				pMsgMaize->MsgEngineRotateSet = pCanData->EngineRotateSet;     		//
				pMsgMaize->MsgEngineTorque = pCanData->EngineTorque;				//
				pMsgMaize->MsgFuelTemp = pCanData->FuelTemp;					//
				pMsgMaize->MsgOilTemp = pCanData->OilTemp;         			//
				pMsgMaize->MsgAirPressure = pCanData->AirPressure;         		//
				pMsgMaize->MsgEngineNacelleTemp = pCanData->EngineNacelleTemp;			//
				pMsgMaize->MsgAirTemp = pCanData->AirTemp;					// 
				
				pMsgMaize->MsgEnteredAirTemp = pCanData->EnteredAirTemp;				//
				pMsgMaize->MsgEngineWorkTime = pCanData->EngineWorkTime;				//
				pMsgMaize->MsgTravelSpeed = pCanData->TravelSpeed;				//
				pMsgMaize->MsgOnceTravel = pCanData->OnceTravel;					//
				pMsgMaize->MsgTotalTravel = pCanData->TotalTravel;  				//
				pMsgMaize->MsgOnceFuel = pCanData->OnceFuel;      				//
				pMsgMaize->MsgTotalFuel = pCanData->TotalFuel;  				//
				pMsgMaize->MsgRelativeOilPressure = pCanData->RelativeOilPressure;		//
				pMsgMaize->MsgAbsoluteOilPressure = pCanData->AbsoluteOilPressure;		//
				pMsgMaize->MsgRelativeAddPressure = pCanData->RelativeAddPressure;		//
				pMsgMaize->MsgAbsoluteAddPressure = pCanData->AbsoluteAddPressure;		//
				pMsgMaize->MsgFuelNum = pCanData->FuelNum;					//		
				pMsgMaize->MsgFuelPercent = pCanData->FuelPercent;				//
				pMsgMaize->MsgOilPosition = pCanData->OilPosition;				//
				//SL_Print("The Msg Air Temp:%d,%d\r\n",pCanData->AirTemp,pCanData->OilPosition);
				pMsgMaize->MsgCrankcasePressure = pCanData->CrankcasePressure;   		//
				pMsgMaize->MsgCoolPressure = pCanData->CoolPressure;  				//
				pMsgMaize->MsgCoolPosition = pCanData->CoolPosition;				//
				pMsgMaize->MsgLockCarState = pCanData->LockCarState;				//
				pMsgMaize->MsgActivateStatus = pCanData->ActivateStatus;   			//
				pMsgMaize->MsgKeyStatus = pCanData->KeyStatus;     				//
				pMsgMaize->MsgTerIDStatus = pCanData->TerIDStatus;				//
				pMsgMaize->MsgWorkFlag = pCanData->WorkFlag;   				//      
				//SL_Print("The MsgWorkFlag:%d\r\n",pMsgMaize->MsgWorkFlag);
				pMsgMaize->MsgCarWarnValue1 =pCanData->CarWarnValue1;				//
				pMsgMaize->MsgCarWarnValue2 = pCanData->CarWarnValue2;				//
				pMsgMaize->MsgFoodFullWarn = pCanData->FoodFullWarn;				//
				pMsgMaize->MsgClutchState = pCanData->ClutchState;  				//
				pMsgMaize->MsgStripRotate = pCanData->StripRotate;				//
				pMsgMaize->MsgLiftRotate = pCanData->LiftRotate;					//
				pMsgMaize->MsgCutTableHigh = pCanData->CutTableHigh;				//
				pMsgMaize->MsgNC1 = 0;												//
				pMsgMaize->MsgNC2 = 0;
				pMsgMaize->MsgNC3 = 0;
				pMsgMaize->MsgNC4 = 0;     //
				//SL_Print("Run This Is :%d\r\n",pMsgLoader->MsgSysVol);
				SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanMaizeStr)); 		//
				DataLen += sizeof(MsgCanMaizeStr);
				//SL_Print("Run This Is :%d\r\n",pMsgMaize->MsgCutTableHigh);
				MsgNum++;
				break;
			default:
				break;
		}

			//
		pMsgGnss = (MsgGnssStr *)buf;
		pGnss = GetGnssDataSpace();
		pSysTime = GetSysTimeSpace();
		
		pMsgGnss->MsgGnssType = 0x0021;								//
		pMsgGnss->MsgGnssLen = 0;				//
			
		pMsgGnss->MsgGnssLatitude = pGnss->latitude * 1000000;		//
		pMsgGnss->MsgGnssLongitude = pGnss->longitude * 1000000;	//
		pMsgGnss->MsgGnssSpeed =(unsigned short int)(pGnss->speed*10);		//
		pMsgGnss->MsgGnssAzimuth = pGnss->azimuth;					//
		pMsgGnss->MsgGnssAltitude = pGnss->altitude;				//
		pMsgGnss->MsgGnssYear = pSysTime->Year; 					//Gnss
		pMsgGnss->MsgGnssMon = pSysTime->Mon;						//Gnss
		pMsgGnss->MsgGnssDay = pSysTime->MDay;						//Gnss
		pMsgGnss->MsgGnssHour = pSysTime->Hour; 					//Gnss
		pMsgGnss->MsgGnssMin = pSysTime->Min;						//Gnss 
		pMsgGnss->MsgGnssSec = pSysTime->Sec;						//Gnss 
			
		pMsgGnss->MsgGnssSatelliteNum = pGnss->satellite_num;		//
		pMsgGnss->MsgGnssViewNum = pGnss->satellite_view_num;		//	
		pMsgGnss->MsgGhdopV = pGnss->ghdop_v;						//
		if(pGnss->status == 'A')
			pMsgGnss->MsgGnssStatus = pGnss->status | 0x02; 		//	  
		else
			pMsgGnss->MsgGnssStatus = pGnss->status & 0xFD;
		
		if(pTerState->GnssAntShort > 0 || pTerState->GnssAntState > 0)    //
			pMsgGnss->MsgGnssStatus |= 0x01;
		else
			pMsgGnss->MsgGnssStatus &= 0xFE;

		pMulGnss = (MsgGnssStr *)(TmpBuf + DataLen);           //
		SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgGnssStr)); 
		DataLen += sizeof(MsgGnssStr);
		TmpBuf[DataLen] = MsgLine;     //
		MsgLine++;
		DataLen++;
		MsgNum++;
	}

	pMsgGnss = (MsgGnssStr *)buf;
	pGnss = GetGnssDataSpace();
	pSysTime = GetSysTimeSpace();
		
	pMsgGnss->MsgGnssType = 0x0021;								//
	pMsgGnss->MsgGnssLen = 0;									//

	pMsgGnss->MsgGnssLatitude = pGnss->latitude * 1000000;		//
	pMsgGnss->MsgGnssLongitude = pGnss->longitude * 1000000;	//
	pMsgGnss->MsgGnssSpeed = (unsigned short int)(pGnss->speed*10);		//
	pMsgGnss->MsgGnssAzimuth = pGnss->azimuth;					//
	pMsgGnss->MsgGnssAltitude = pGnss->altitude;				// 
	pMsgGnss->MsgGnssYear = pSysTime->Year; 					//Gnss
	pMsgGnss->MsgGnssMon = pSysTime->Mon;						//Gnss
	pMsgGnss->MsgGnssDay = pSysTime->MDay;						//Gnss
	pMsgGnss->MsgGnssHour = pSysTime->Hour; 					//Gnss
	pMsgGnss->MsgGnssMin = pSysTime->Min;						//Gnss
	pMsgGnss->MsgGnssSec = pSysTime->Sec;						//Gnss 
			
	pMsgGnss->MsgGnssSatelliteNum = pGnss->satellite_num;		//
	pMsgGnss->MsgGnssViewNum = pGnss->satellite_view_num;		//
	pMsgGnss->MsgGhdopV = pGnss->ghdop_v;						//
	if(pGnss->status == 'A')
		pMsgGnss->MsgGnssStatus = pGnss->status | 0x02; 		//  
	else
		pMsgGnss->MsgGnssStatus = pGnss->status & 0xFD;
		
	if(pTerState->GnssAntShort > 0 || pTerState->GnssAntState > 0)    //
		pMsgGnss->MsgGnssStatus |= 0x01;
	else
		pMsgGnss->MsgGnssStatus &= 0xFE;

	SL_Memcpy(TmpBuf + DataLen,(buf + 4),(sizeof(MsgGnssStr) - 4)); 
	DataLen = DataLen + (sizeof(MsgGnssStr) - 4);
	TmpBuf[DataLen] = MsgLine;     //
	DataLen++;
	MsgLine++;
	//SL_Print("The Gnss Msg :%d,%d,%d\r\n",MsgLine,DataLen,big);
	if(MsgLine >= 10 || big > 0)    								//                    
	{
		pMulGnss->MsgGnssLen = MsgLine * 26;
		pMsgHead = (MsgHeadStr *)TmpBuf; 
		pMsgHead->msg_body_num = MsgNum;
		pMsgHead->CarTypeNum = 0x02;  										//
		
		pMsgHead->msg_len = DataLen - 23; 						
		MsgSerial++;
		SL_Memcpy(TmpBuf + DataLen,(unsigned char *)&MsgSerial,2);
		DataLen += 2;
		*(TmpBuf + DataLen) = BccVerify(TmpBuf + 3,DataLen - 3);
		DataLen++;
		*(TmpBuf + DataLen) = 0x0D;
		DataLen++;
		SL_Memcpy(source,TmpBuf,DataLen);
		tmp = DataLen;
		SL_Print("App>:Build UpLoad Data Packet 1:%d,%d,%d\r\n",pMsgHead->msg_body_num,MsgLine,DataLen);
		MsgNum = 0;
		DataLen = 0;
		MsgLine = 0;
	}
	return tmp;    //
}	

///*/

/******************************************************
**	函数名称:unsigned short int BuildCompletePacket(unsigned char *source,unsigned char big)
**	功能描述:
******************************************************/
//*
unsigned short int BuildCompletePacket(unsigned char *source,unsigned char big)
{
	unsigned short int	DataLen = 0;

	MsgHeadStr 					TmpMsg;
	unsigned char				buf[100];
	MsgHeadStr 	 				*pMsgHead;         //
	SysConfigStr				*pSysCfg;          //
	MsgGnssStr					*pMsgGnss;         //
	MsgDeviceStr				*pMsgDevice;       //
	MsgInputStr 				*pMsgInput;        //
	MsgCanMaizeStr 				*pMsgMaize;       //
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
	TmpMsg.msg_id = 0x45;											//
	SL_Memcpy(&TmpMsg.device_id,pSysCfg->TerminalID,16);         	//
	TmpMsg.CarTypeNum = 0x02;  										//
	TmpMsg.DataPackFlag = 0x02;   								 	//
	
	TmpMsg.msg_body_num = 0x00;                                     //
	TmpMsg.msg_len = 0x00;                                          //
	SL_Memcpy(TmpBuf,(unsigned char *)&TmpMsg,25);          		//
	DataLen = 25;													//

	pTerState = GetTerStateDataSpace();             				//
	//

	if(TerMsgBig == 0)
	{
		pMsgDevice = (MsgDeviceStr *)buf;               				//
		pMsgDevice->MsgDeviceType = 0x0001;              	 			// 
		pMsgDevice->MsgDeviceLen = sizeof(MsgDeviceStr) - 4;  			//

		pMsgDevice->MsgManuNum = 0x01;            						//   		
		pMsgDevice->MsgTerminalType = 0x01;        						//
		pMsgDevice->MsgUserNum = 32;                     				//客户编号
		pMsgDevice->MsgAppVer1 = (unsigned char)pTerState->FirNum;      //单片机版本
		pMsgDevice->MsgAppVer2 = GetVerNum();           				//ST86程序应用版本
		pMsgDevice->MsgHardwareVer = 41;             					//硬件版本
		SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgDeviceStr));			// 
		DataLen += sizeof(MsgDeviceStr); 
		MsgNum++;
		TerMsgBig++;
		//SL_Print("Add TerMsg Block...........\r\n");
	}
	
	//
	pMsgInput = (MsgInputStr *)buf;             //
	
	pMsgInput->MsgInputType = 0x0004;								//
	pMsgInput->MsgInputLen = sizeof(MsgInputStr) - 4;

	pMsgInput->MsgInputIo = pTerState->IOStatus.vaule;			//
	pMsgInput->MsgAcc = pTerState->AccState;    				// 

	pMsgInput->MsgMoto = 0;
	pMsgInput->MsgInputFrq1 = 0;								//
	pMsgInput->MsgInputFrq2 = 0;								//
	pMsgInput->MsgInputFrq3 = 0;								//
	pMsgInput->MsgInputFrq4 = 0;								//
	pMsgInput->MsgPowVol = pTerState->PowerVol;	 				//
	pMsgInput->MsgBatteryVol = pTerState->BatteryVol;			//
	pMsgInput->MsgInputVol1 = 0;								//
	pMsgInput->MsgInputVol2 = 0;								//
	pMsgInput->MsgWarnValue = 0;
	pMsgInput->MsgLine = 1;					//
 	 
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgInputStr)); 
	DataLen += sizeof(MsgInputStr);
	MsgNum++;

	//
	pMsgGnss = (MsgGnssStr *)buf;
	pGnss = GetGnssDataSpace();
	pSysTime = GetSysTimeSpace();                 				//
	pMsgGnss->MsgGnssType = 0x0100;								//
	pMsgGnss->MsgGnssLen = sizeof(MsgGnssStr) - 4;				//
		
	pMsgGnss->MsgGnssLatitude = pGnss->latitude * 1000000;		//	
	pMsgGnss->MsgGnssLongitude = pGnss->longitude * 1000000;	//
	//SL_Print("The Latitude,Longitude:%d,%d\r\n",pMsgGnss->MsgGnssLatitude,pMsgGnss->MsgGnssLongitude);
	pMsgGnss->MsgGnssSpeed =(unsigned short int)(pGnss->speed*10);		//
	pMsgGnss->MsgGnssAzimuth = pGnss->azimuth;					//
	pMsgGnss->MsgGnssAltitude = pGnss->altitude;				//
	pMsgGnss->MsgGnssYear = pSysTime->Year; 					//Gnss
	pMsgGnss->MsgGnssMon = pSysTime->Mon;						//Gnss
	pMsgGnss->MsgGnssDay = pSysTime->MDay;						//Gnss
	pMsgGnss->MsgGnssHour = pSysTime->Hour; 					//Gnss 
	pMsgGnss->MsgGnssMin = pSysTime->Min;						//Gnss 
	pMsgGnss->MsgGnssSec = pSysTime->Sec;						//Gnss
		
	pMsgGnss->MsgGnssSatelliteNum = pGnss->satellite_num;		//
	pMsgGnss->MsgGnssViewNum = pGnss->satellite_view_num;		//	
	pMsgGnss->MsgGhdopV = pGnss->ghdop_v;						//
	if(pGnss->status == 'A')
		pMsgGnss->MsgGnssStatus = pGnss->status | 0x02; 		//  
	else
		pMsgGnss->MsgGnssStatus = pGnss->status & 0xFD;
	
	if(pTerState->GnssAntShort > 0 || pTerState->GnssAntState > 0)    //
		pMsgGnss->MsgGnssStatus |= 0x01;
	else
		pMsgGnss->MsgGnssStatus &= 0xFE;

	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgGnssStr)); 		//
	DataLen += sizeof(MsgGnssStr);
	MsgNum++;

	
	//
	pMsgMaize = (MsgCanMaizeStr *)buf;    //
	pCanData = GetCanDataSpace();
	
	//SL_Print("Run This Is 1 :%d\r\n",pSysCfg->CarType);
	switch(pSysCfg->CarType)
	{ 
		case 0x0A:						 						 						//
			pMsgMaize->MsgMaizeMechType = 0x000C;										//
			pMsgMaize->MsgMaizeMechLen = 78;        									//

			pMsgMaize->MsgSysVol = pCanData->SysVol;           	 						//
			pMsgMaize->MsgTempWater = pCanData->TempWater;           		//
			pMsgMaize->MsgEngineRotate = pCanData->EngineRotate;		 		//
			pMsgMaize->MsgEngineRotateSet = pCanData->EngineRotateSet;     		//
			pMsgMaize->MsgEngineTorque = pCanData->EngineTorque;				//
			pMsgMaize->MsgFuelTemp = pCanData->FuelTemp;					//
			pMsgMaize->MsgOilTemp = pCanData->OilTemp;         			//
			pMsgMaize->MsgAirPressure = pCanData->AirPressure;         		//
			pMsgMaize->MsgEngineNacelleTemp = pCanData->EngineNacelleTemp;			//
			pMsgMaize->MsgAirTemp = pCanData->AirTemp;					//
			pMsgMaize->MsgEnteredAirTemp = pCanData->EnteredAirTemp;				//
			pMsgMaize->MsgEngineWorkTime = pCanData->EngineWorkTime;				//
			pMsgMaize->MsgTravelSpeed = pCanData->TravelSpeed;				//
			pMsgMaize->MsgOnceTravel = pCanData->OnceTravel;					//
			pMsgMaize->MsgTotalTravel = pCanData->TotalTravel;  				//
			pMsgMaize->MsgOnceFuel = pCanData->OnceFuel;      				//
			pMsgMaize->MsgTotalFuel = pCanData->TotalFuel;  				//
			pMsgMaize->MsgRelativeOilPressure = pCanData->RelativeOilPressure;		//
			pMsgMaize->MsgAbsoluteOilPressure = pCanData->AbsoluteOilPressure;		//
			pMsgMaize->MsgRelativeAddPressure = pCanData->RelativeAddPressure;		//
			pMsgMaize->MsgAbsoluteAddPressure = pCanData->AbsoluteAddPressure;		//
			pMsgMaize->MsgFuelNum = pCanData->FuelNum;					//			
			pMsgMaize->MsgFuelPercent = pCanData->FuelPercent;				//
			pMsgMaize->MsgOilPosition = pCanData->OilPosition;				//
			pMsgMaize->MsgCrankcasePressure = pCanData->CrankcasePressure;   		//
			pMsgMaize->MsgCoolPressure = pCanData->CoolPressure;  				//
			pMsgMaize->MsgCoolPosition = pCanData->CoolPosition;				//
			pMsgMaize->MsgLockCarState = pCanData->LockCarState;				//
			pMsgMaize->MsgActivateStatus = pCanData->ActivateStatus;   			//
			pMsgMaize->MsgKeyStatus = pCanData->KeyStatus;     				//
			pMsgMaize->MsgTerIDStatus = pCanData->TerIDStatus;				//
			pMsgMaize->MsgWorkFlag = pCanData->WorkFlag;   						//
			pMsgMaize->MsgCarWarnValue1 =pCanData->CarWarnValue1;				//
			pMsgMaize->MsgCarWarnValue2 = pCanData->CarWarnValue2;				//
			pMsgMaize->MsgFoodFullWarn = pCanData->FoodFullWarn;				//
			pMsgMaize->MsgClutchState = pCanData->ClutchState;  				//
			pMsgMaize->MsgStripRotate = pCanData->StripRotate;				//
			pMsgMaize->MsgLiftRotate = pCanData->LiftRotate;					//
			pMsgMaize->MsgCutTableHigh = pCanData->CutTableHigh;				//
			pMsgMaize->MsgNC1 = 0;
			pMsgMaize->MsgNC2 = 0;
			pMsgMaize->MsgNC3 = 0;
			pMsgMaize->MsgNC4 = 0;     //
			//SL_Print("Run This Is :%d\r\n",pMsgLoader->MsgSysVol);
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanMaizeStr)); 		//
			DataLen += sizeof(MsgCanMaizeStr);
			//SL_Print("Run This Is :%d\r\n",pMsgMaize->MsgCutTableHigh);
			MsgNum++;
			break;
		default:
			break;
	}


	pFaultCode1 = GetCurrentFaultCodeSpace();	  //
	//SL_Print("Run This Is :%d\r\n",pFaultCode1->FaultCodeNum);   //
	if(pFaultCode1->FaultCodeNum > 0 && pFaultCode1->FaultCodeNum <= 160)	  //
	{
		IntToChar TmpInt16;
	
		TmpInt16.IntII = 0x0011;					  //
		buf[0] = TmpInt16.TTbyte[0];
		buf[1] = TmpInt16.TTbyte[1];
	
		TmpInt16.IntII = pFaultCode1->FaultCodeNum * 4 + 2;   //
				
		buf[2] = TmpInt16.TTbyte[0];
		buf[3] = TmpInt16.TTbyte[1];
					
		SL_Memcpy(TmpBuf + DataLen,buf,4);			 // 
		DataLen += 4;
				
		*(TmpBuf + DataLen) = pFaultCode1->FaultCodeNum;   //
		DataLen++;
		SL_Memcpy(TmpBuf + DataLen,pFaultCode1->FaultCode,pFaultCode1->FaultCodeNum * 4);
		DataLen += pFaultCode1->FaultCodeNum*4;
	
		*(TmpBuf + DataLen) = 0x01;
		DataLen += 1;
		MsgNum++;
					
	}

	pMsgHead = (MsgHeadStr *)TmpBuf;                   										//
	pMsgHead->msg_len = DataLen - 23;          //
	pMsgHead->msg_body_num = MsgNum;  
	MsgSerial++;
	SL_Memcpy(TmpBuf + DataLen,(unsigned char *)&MsgSerial,2); 
	
	DataLen += 2;
	
	*(TmpBuf + DataLen) = BccVerify(TmpBuf + 3,DataLen - 3);
	DataLen++;
	*(TmpBuf + DataLen) = 0x0D;
	DataLen++;
	SL_Memcpy(source,TmpBuf,DataLen);

	SL_Print("App>:Build UpLoad Data Packet 2 %d,%d\r\n",pMsgHead->msg_body_num,DataLen);
	MsgNum = 0;
	return DataLen;
}

//*/



/**************************File End***************************/









