

/******************************************
**	FileName:
**	Time:
**	�����ϴ����ݰ�
*******************************************/

#include "include.h"


static unsigned char 				TmpBuf[255];					//���ݻ����� 


/******************************************************
**	:unsigned short int BuildCompletePacket(unsigned char *source,unsigned char big)
**	:
******************************************************/

unsigned short int BuildCompletePacket(unsigned char *source,unsigned char big)
{
	static unsigned short int	DataLen = 0;
	unsigned char				buf[100];
	IntToChar					TmpData16;
		
	SysConfigStr				*pSysCfg;          //
	MsgGnssStr					*pMsgGnss;         //
	MsgDeviceStr				*pMsgDevice;       //
	MsgInputStr 				*pMsgInput;        //
	MsgExcavatorStr 			*pMsgExcavator;    //
	MsgLoaderStr				*pMsgLoader;	   //
	
	GNSSSTR 					*pGnss;            //
	TerStateStr					*pTerState;        //
	CanDataStr					*pCanData;         //
	SysTimeStr					*pSysTime;         //
	MsgQDHeadStr				*pMsgHead;         //

	DataLen += sizeof(MsgQDHeadStr);                //

	pTerState = GetTerStateDataSpace();             //
	//
	pMsgDevice = (MsgDeviceStr *)buf;               //
	pSysCfg = GetSysCfgSpace();
		
	pMsgDevice->MsgDeviceType = 0x01;               //  
	pMsgDevice->MsgDeviceLen = sizeof(MsgDeviceStr) - 3;  //
	
	
	SL_Memcpy(pMsgDevice->MsgDeviceID,pSysCfg->TerminalID + 2,8);	//
	pMsgDevice->MsgProtocolVer = 0x0B;								//
	pMsgDevice->MsgFirVer = 0;  //pTerState->McuFirNum;				//
	pMsgDevice->MsgManuNum = 0x03;									//
	pMsgDevice->MsgManuType = 0x04; 								//
	pMsgDevice->MsgDeviceCarNum = pSysCfg->CarType; 				//
	pMsgDevice->MsgAppVer = GetVerNum() & 0x0F ;							//  
	pMsgDevice->MsgAppVer |= ((pTerState->FirNum << 4) & 0xF0);
	pMsgDevice->MsgHardwareVer = pSysCfg->HardWareNum;				// 
	//SL_Print("The MsgApp Ver:%x\r\n",pMsgDevice->MsgAppVer);
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgDeviceStr));			// 
	DataLen += sizeof(MsgDeviceStr); 
	//
	pMsgGnss = (MsgGnssStr *)buf;
	pGnss = GetGnssDataSpace();
	pSysTime = GetSysTimeSpace();
	pMsgGnss->MsgGnssType = 0x03;									//
	pMsgGnss->MsgGnssLen = sizeof(MsgGnssStr) - 3;					//
	
	pMsgGnss->MsgGnssLatitude = pGnss->latitude * 1000000;			//	
	pMsgGnss->MsgGnssLongitude = pGnss->longitude * 1000000;		//
	pMsgGnss->MsgGnssSpeed = (unsigned char)pGnss->speed; 			//
	pMsgGnss->MsgGnssAzimuth = pGnss->azimuth;						//
	pMsgGnss->MsgGnssAltitude = pGnss->altitude;					//
	
	pMsgGnss->MsgGnssYear = pSysTime->Year; 						//
	pMsgGnss->MsgGnssMon = pSysTime->Mon;							//
	pMsgGnss->MsgGnssDay = pSysTime->MDay;							//
	pMsgGnss->MsgGnssHour = pSysTime->Hour; 						//
	pMsgGnss->MsgGnssMin = pSysTime->Min;							//
	pMsgGnss->MsgGnssSec = pSysTime->Sec;							// 
	
	pMsgGnss->MsgGnssSatelliteNum = pGnss->satellite_num;			//
	pMsgGnss->MsgGnssViewNum = pGnss->satellite_view_num;			//
	
	if(pGnss->status == 'A')
		pMsgGnss->MsgGnssStatus = pGnss->status | 0x02;         	//   
	else
		pMsgGnss->MsgGnssStatus = pGnss->status & 0xFD;				//
	
	if(pTerState->GnssAntShort > 0 || pTerState->GnssAntState > 0)   //
		pMsgGnss->MsgGnssStatus |= 0x01;
	else
		pMsgGnss->MsgGnssStatus &= 0xFE;
	//SL_Print("The Gnss Ant State:%d,%d\r\n",pTerState->GnssAntState,pTerState->GnssAntShort);
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgGnssStr));         	//
	DataLen += sizeof(MsgGnssStr);

	//
	pMsgInput = (MsgInputStr *)buf;             //
	pCanData = GetCanDataSpace();
	pMsgInput->MsgInputType = 0x04;								//
	pMsgInput->MsgInputLen = sizeof(MsgInputStr) - 3;			//

	if(ReadAccState() > 0)
	{
		pMsgInput->MsgInputIo |= 0x01;						    // 
		pMsgInput->MsgInputIo &= 0xFD;
	}
	else
	{
		pMsgInput->MsgInputIo &= 0xFE;
		pMsgInput->MsgInputIo |= 0x02;
	}

	//pCanData->LockCarState = 0x01;
	
	pMsgInput->MsgInputFrq1 = 0;								//
	pMsgInput->MsgInputFrq2 = 0;								//
	pMsgInput->MsgInputFrq3 = 0;								//
	pMsgInput->MsgInputFrq4 = 0;								//
	pMsgInput->MsgPowVol = pTerState->PowerVol;	 				//
	pMsgInput->MsgBatteryVol = pTerState->BatteryVol;			//
	pMsgInput->MsgInputVol1 = 0;								//
	pMsgInput->MsgInputVol2 = 0;								//
	//SL_Print("The Is ACC Val:%d\r\n",pTerState->AccVol);
	
	/********************************
	 * 原通讯协议使用的是继电器锁车，
	 * 现在CAN命令锁车，直接套用原协议。
	 *******************************/ 
	///*
	//SL_Print("Lock Car State:%d\r\n",pCanData->LockCarState);
	if(pCanData->LockCarState == 0)              				//未锁车
	{
		pMsgInput->MsgRelayState = 0;			
		pMsgInput->MsgLockState = 0;	
	}
	else if(pCanData->LockCarState == 1)       //预锁车  
	{
		pMsgInput->MsgRelayState = 0x01;
		pMsgInput->MsgLockState = 0x01;
	}
	else if(pCanData->LockCarState == 2)       
	{
		pMsgInput->MsgRelayState = 0x4;
		pMsgInput->MsgLockState = 0x04;
	}
	else
	{
		if(pTerState->lockRank == 0)
		{
			pMsgInput->MsgRelayState = 0;			
			pMsgInput->MsgLockState = 0;
		}
		else if(pTerState->lockRank == 1)
		{
			pMsgInput->MsgRelayState = 0x03;			
			pMsgInput->MsgLockState = 0x01;
		}
		else if(pTerState->lockRank == 2)
		{
			pMsgInput->MsgRelayState = 0x0C;			
			pMsgInput->MsgLockState = 0x04;
		}
	}
	
	

	//*/
	//pMsgInput->MsgRelayState = 0x03;			
	//pMsgInput->MsgLockState = 0x01;
	//SL_Print("LockRank:%d,pCanData->LockCarState:%d\r\n",pMsgInput->MsgRelayState,pCanData->LockCarState);

	//pMsgInput->MsgRelayState = 0x03;
	//pMsgInput->MsgLockState = 0x01;

	//SL_Print("pCanData->TravelState::%d\r\n",pCanData->LockCarState);
	pMsgInput->MsgAlrmState = (unsigned char)GetWarningValue();	//
	//SL_Print("pCanData->TravelState::%d\r\n",pTerState->MonExpectState);
	//SL_Print("pCanData->TravelState::%d\r\n",pTerState->MonExpectState);
	if(pTerState->MonExpectState > 0)
	{
		pMsgInput->MsgLockCar = 0x0000;	
	}

	else
	{
		pMsgInput->MsgLockCar = 0x0100;
	}
	//pMsgInput->MsgLockCar = 0x0001;
	//SL_Print("pCanData->TravelState 1 ::%d\r\n",pMsgInput->MsgLockCar);
	
	/*
	pMsgInput->MsgLockCar &= 0x03FF;
	if(pTerState->lockRank == 0)     //未锁车
	{
		pMsgInput->MsgLockCar &= 0x03FF;
	}
	else if(pTerState->lockRank == 1)	//一级锁车
	{
		pMsgInput->MsgLockCar |= 0x0400;
	}			
	else if(pTerState->lockRank == 2)     //二级锁车
	{
		 pMsgInput->MsgLockCar |= 0x1000;
	}
	*/
	pMsgInput->MsgAccWorkTime = pTerState->AccWorkTime;								//电锁工作时间
	//SL_Print("The MsgAccWorkTime:%d\r\n",pMsgInput->MsgAccWorkTime);
	pMsgInput->MsgEngineWorkTime = pTerState->EngineWorkTime;								//发动机工作时间
	//SL_Print("pCanData->TravelState 2 ::%d,%d,%d\r\n",pMsgInput->MsgLockCar,pTerState->LockExpectState,pTerState->MonExpectState);
 	pMsgInput->MsgDataMode = 0x00;									//
 	//SL_Print("pCanData->TravelState 2 ::%d\r\n",pMsgInput->MsgDataMode);
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgInputStr)); 			//
	DataLen += sizeof(MsgInputStr);
	//
	pCanData = GetCanDataSpace();
	//
	switch(pSysCfg->CarType)
	{ 
		case 0x01:						  //挖掘机
			pMsgExcavator = (MsgExcavatorStr *)buf;
			
			pMsgExcavator->MsgExcavatorType 			= 0x05;									//
			pMsgExcavator->MsgExcavatorLen 				= sizeof(MsgExcavatorStr) - 3;          //

			pMsgExcavator->MsgSysVol 					= pCanData->SysVol;           	 	 	//
			pMsgExcavator->MsgTempWater 				= pCanData->TempWater;           		//
			pMsgExcavator->MsgEngineRotate 				= pCanData->EngineRotate;		 		//
			pMsgExcavator->MsgEngineRotateSet 			= pCanData->EngineRotateSet;     		//
			pMsgExcavator->MsgEngineTorque 				= 0;									//
			pMsgExcavator->MsgWorkFlag 					= pCanData->WorkFlag;					//
			pMsgExcavator->MsgRotaryVal 				= pCanData->RotaryVal;         			// 
			pMsgExcavator->MsgOilTemp 					= 0;									//
			pMsgExcavator->MsgFuelPumpPressure 			= 0;									//
			pMsgExcavator->MsgEngineOilPressure 		= 0;									//
			pMsgExcavator->MsgAirPressure 				= 0;               		 				//
			pMsgExcavator->MsgAddPressure 				= 0;									//
			pMsgExcavator->MsgAddPressureTemp 			= 0;									//
			pMsgExcavator->MsgEnteredAirTemp 			= 0;									//
			pMsgExcavator->MsgFuelPercent 				= pCanData->FuelPercent;		 		//
			pMsgExcavator->MsgComPathPressure 			= 0;									// 
			pMsgExcavator->MsgComPathDis 				= 0;									//
			pMsgExcavator->MsgFuelGoalGush 				= 0;									//
			pMsgExcavator->MsgFuelGoalControl 			= 0;									//
			pMsgExcavator->MsgWorkTime 					= pCanData->WorkTime;			 		//
			pMsgExcavator->MsgLockCarState 				= pCanData->LockCarState;				//
			pMsgExcavator->MsgControllerFaultCode 		= 0;        //
			
			pMsgExcavator->MsgSwitchState            	= pCanData->SwitchState;  //
			
			pMsgExcavator->MsgRelayVol 					= pCanData->RelayVol;  					//

			pMsgExcavator->MsgWokrModeState 			= pCanData->WorkModeState;
			//SL_Print("pCanData->TravelState::%d\r\n",pMsgExcavator->MsgWokrModeState);	   				
			pMsgExcavator->MsgFuelFlow 					= 0;					//
			pMsgExcavator->MsgShowEngineRotate 			= 0;           //
			pMsgExcavator->MsgFuelQuantity 				= 0;               //
			pMsgExcavator->MsgFuelTotalQuantity 		= 0;			//
			pMsgExcavator->MsgECMFaultCode 				= pCanData->ECMFaultCode;				//
			pMsgExcavator->MsgFluidOilTemp 				= 0;				//
			pMsgExcavator->MsgFrontPumpPressure 		= pCanData->FrontPumpPressure;			//
			pMsgExcavator->MsgBackPumpPressure 			= 0;			//
			pMsgExcavator->MsgBucketRodExtend 			= 0;			//
			pMsgExcavator->MsgBucketRodAnastole 		= 0;				//
			pMsgExcavator->MsgRotation 					= 0;					//
			pMsgExcavator->MsgTravelL 					= 0;  					//
			pMsgExcavator->MsgTravelR 					= 0;					//
			pMsgExcavator->MsgSwingArmUp 				= 0;					//
			pMsgExcavator->MsgSwingArmDrop 				= 0;				//
			pMsgExcavator->MsgBucketExtend 				= 0; 				//
			pMsgExcavator->MsgBucketAnastole 			= 0;				//
			pMsgExcavator->MsgFrontPumpRadiotube 		= 0;			//
			pMsgExcavator->MsgBackPumpRadiotube 		= 0;			//
			pMsgExcavator->MsgBucketRatioRadiotube 		= 0;			//
			pMsgExcavator->MsgSwingArmRatioRadiotube 	= 0;  			//
			pMsgExcavator->MsgSwingArmRadiotube1 		= 0;			//
			pMsgExcavator->MsgSwingArmRadiotube2 		= 0;	  		//
			pMsgExcavator->MsgBreakHammerValue 			= 0;			//
			pMsgExcavator->MsgPowerControlValue 		= 0;			//
				
			pMsgExcavator->MsgCarWarnValue1				= pCanData->MsgCarWarnValue1;  //
			pMsgExcavator->MsgCarWarnValue2				= pCanData->MsgCarWarnValue2;  //
			
			//pMsgExcavator->MsgCarWarnValue2 			= 0;				//
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgExcavatorStr)); 
			
			DataLen += sizeof(MsgExcavatorStr);
			break;
		case 0x02:						  //装载机设备
			pCanData = GetCanDataSpace();
			pMsgLoader = (MsgLoaderStr *)buf;

			pMsgLoader->MsgLoaderType 					= 0x05;       							//
			pMsgLoader->MsgLoaderLen 					= sizeof(MsgLoaderStr) - 3;	   			// 

			pMsgLoader->MsgDeviceVol 					= pTerState->PowerVol;					// 
			pMsgLoader->MsgDeviceWorkTime 				= pTerState->EngineWorkTime / 60.0;									// 
			//SL_Print("pMsgLoader->MsgDeviceWorkTime:%d\r\n",pMsgLoader->MsgDeviceWorkTime);

			pMsgLoader->MsgSysVol 						= pCanData->SysVol;						// 
			pMsgLoader->MsgWorkTime 					= pCanData->MeterWorkTime;				// 
			//SL_Print("The Meter Work Time:%d\r\n",pMsgLoader->MsgWorkTime);

			pMsgLoader->MsgTransmitOilTemp 				= pCanData->TransferOilTemp;			// 			
			pMsgLoader->MsgBrakePressure 				= pCanData->BrakePressure;				// 
			pMsgLoader->MsgFuelPercent 					= pCanData->FuelPercent;				// 
			pMsgLoader->MsgCabTemp 						= pCanData->OuterTemp;					// 
			pMsgLoader->MsgOutdoorTemp 					= pCanData->InnerTemp;					//
			pMsgLoader->MsgShiftHandleLocate		 	= 0;									//
			pMsgLoader->MsgGearboxValue 				= 0;									//
			pMsgLoader->MsgGearboxPressure 				= 0;									//
			pMsgLoader->MsgClutchKVPressure 			= 0;									//
			pMsgLoader->MsgClutchKRPressure 			= 0;									//
			pMsgLoader->MsgClutchK1Pressure 			= 0;									//
			pMsgLoader->MsgClutchK2Pressure 			= 0;									//
			pMsgLoader->MsgClutchK3Pressure 			= 0;									//
			pMsgLoader->MsgClutchK4Pressure 			= 0;									//
			pMsgLoader->MsgGearboxMode 					= 0;									//
			pMsgLoader->MsgClutchKVCurrent 				= 0;									//
			pMsgLoader->MsgClutchKRCurrent 				= 0;									//
			pMsgLoader->MsgClutchK1Current 				= 0;									//
			pMsgLoader->MsgClutchK2Current 				= 0;									//
			pMsgLoader->MsgClutchK3Current 				= 0;									//
			pMsgLoader->MsgClutchK4Current 				= 0;									//
			pMsgLoader->MsgGearBoxInRotate 				= 0;									//
			pMsgLoader->MsgGearBoxGearRotate 			= 0;									//
			pMsgLoader->MsgGearBoxMidRotate 			= 0; 									//
			pMsgLoader->MsgGearBoxOutRotate 			= 0;									// 
			pMsgLoader->MsgGearBoxFault 				= 0;									//  
			TmpData16.TTbyte[0] 						= pCanData->MsgCarWarnValue1;
			TmpData16.TTbyte[1]							= pCanData->MsgCarWarnValue2;
			pMsgLoader->MsgSysWarnvalue 				= TmpData16.IntII;						//
		 	pMsgLoader->MsgTempWater 				    = pCanData->TempWater;           		//
		 	pMsgLoader->MsgEngineRotate 				= pCanData->EngineRotate;				//

			TmpData16.TTbyte[0] = GetVerNum();
			TmpData16.TTbyte[1] = pTerState->FirNum;
			TmpData16.IntII = 0x00;
			pMsgLoader->MsgEngineFault 					= TmpData16.IntII;           			//
			pMsgLoader->MsgAirPressure 					= pCanData->AirPressure;        		//
			//SL_Print("The AirPressure:%d,%d,%d\r\n",TmpData16.TTbyte[0],TmpData16.TTbyte[1],pMsgLoader->MsgEngineFault);
			pMsgLoader->MsgAirTemp 						= pCanData->AirTemp;					//
			//SL_Print("pMsgLoader->MsgAirTemp:%d\r\n");
			pMsgLoader->MsgFootboardPosition 			= pCanData->Accelerator;  		
			pMsgLoader->MsgEngineOilPressure 			= pCanData->AbsoluteOilPressure;		//
			pMsgLoader->MsgOilTemp 						= pCanData->OilTemp;					//
			//SL_Print("pMsgLoader->MsgOilTemp:%d,%d\r\n",pMsgLoader->MsgOilTemp,pMsgLoader->MsgFootboardPosition);
			pMsgLoader->MsgAirFanState 					= 0;				//  

			pMsgLoader->MsgAirFanRotate 				= 0;				//
			pMsgLoader->MsgFule 						= pCanData->OnceFuel;						//
			//SL_Print("pMsgLoader->MsgFule:%d\r\n",pMsgLoader->MsgFule );
			pMsgLoader->MsgAddPressure 					= pCanData->RelativeAddPressure;			// 
			//SL_Print("pMsgLoader->MsgAddPressure:%d\r\n",pMsgLoader->MsgAddPressure);
			pMsgLoader->MsgEnteredAirTemp 				= pCanData->EnteredAirTemp;				//
			pMsgLoader->MsgCarPowerVol 					= pCanData->EnginePowerVol;				//
			pMsgLoader->MsgEnteredWorkTime 				= pCanData->WorkTime;			//
			pMsgLoader->MsgFuleTotal 					= pCanData->TotalFuel; 					// 

			SL_Memset(&pMsgLoader->MsgECUFirVer[0],0,8);        				//     
			
			#if 0                                //
			pMsgLoader->MsgECUFirVer[0] = '1';            
			pMsgLoader->MsgECUFirVer[1] = '2';
			pMsgLoader->MsgECUFirVer[2] = '3';
			pMsgLoader->MsgECUFirVer[3] = '4';
			pMsgLoader->MsgECUFirVer[4] = '5';
			pMsgLoader->MsgECUFirVer[5] = '6';
			pMsgLoader->MsgECUFirVer[6] = 'B';
			pMsgLoader->MsgECUFirVer[7] = 'A';

			#endif
			//pMsgLoader->MsgECUFirVer[8] = pCanData->ECUFirVer[8];			//
			//pCanData->ECURes = 0x02;
			pMsgLoader->MsgECULockFunState 				= pCanData->ECURes;						//
			//pCanData->ECUState = 0x0E;
			pMsgLoader->MsgECULockState 				= pCanData->ECUState;					//pCanData->LockCarState;				//ECU����״̬
			//SL_Print("THe pCanData->ECUState:%d\r\n",pCanData->ECUState);
			pMsgLoader->MsgEntereLockState 				= pCanData->EngineLockState;			//
			pMsgLoader->MsgEntereWorkMode 				= 0;									//
			pMsgLoader->MsgHydraulicPressure 			= 0;									//

			//SL_Print("MsgLoaderStr...............%d\r\n",pMsgLoader->MsgAirFanRotate);        	//

			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgLoaderStr)); 
			
			DataLen += sizeof(MsgLoaderStr);
			break;
	}

	pMsgHead = (MsgQDHeadStr *)TmpBuf;                   										//
		
	buf[0] = VerfySum(TmpBuf,DataLen);								  							//
	TmpData16.IntII = (unsigned short int)(32 * PowTow((double)buf[0]) + 23 * buf[0] + 71);  	//
	TmpBuf[DataLen++] = TmpData16.TTbyte[0];
	TmpBuf[DataLen++] = TmpData16.TTbyte[1];
	TmpBuf[DataLen++] = 0x0D;                         //

	pMsgHead->FrameStart[0] = 0xFF;                   //
	pMsgHead->FrameStart[1] = 0xFF;
	pMsgHead->FrameStart[2] = 0xFF;
	pMsgHead->MsgId = 0x81;						     //
	pMsgHead->MsgLen = DataLen - 9;   				 //
	TmpData16.IntII = DataLen;
	
	SL_Memcpy(source,TmpBuf,DataLen);                  
	SL_Print(">App:Build Data Packet:%d\r\n",DataLen);     //
	DataLen = 0;
	return	TmpData16.IntII;                          //

}



/**************************File End***************************/









