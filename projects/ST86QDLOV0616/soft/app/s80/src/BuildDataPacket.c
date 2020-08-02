

/******************************************
**	FileName:
**	Time:
**	�����ϴ����ݰ�
*******************************************/

#include "include.h"


static unsigned char 				TmpBuf[200];					//


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
	
	pMsgGnss->MsgGnssLatitude = pGnss->latitude * 1000000;			//维度	
	pMsgGnss->MsgGnssLongitude = pGnss->longitude * 1000000;		//经度
	pMsgGnss->MsgGnssSpeed = (unsigned char)pGnss->speed; 			//GNSS输出速度
	pMsgGnss->MsgGnssAzimuth = pGnss->azimuth;						//航向角
	pMsgGnss->MsgGnssAltitude = pGnss->altitude;					//海拔高度
	
	pMsgGnss->MsgGnssYear = pSysTime->Year; 						//设备内部时间-年
	pMsgGnss->MsgGnssMon = pSysTime->Mon;							//设备内部事件-月
	pMsgGnss->MsgGnssDay = pSysTime->MDay;							//设备内部事件-日
	pMsgGnss->MsgGnssHour = pSysTime->Hour; 						//设备内部事件-时
	pMsgGnss->MsgGnssMin = pSysTime->Min;							//设备内部事件-分
	pMsgGnss->MsgGnssSec = pSysTime->Sec;							// 设备内部事件-秒
	
	pMsgGnss->MsgGnssSatelliteNum = pGnss->satellite_num;			//使用卫星数目（GPS+北斗之和）
	pMsgGnss->MsgGnssViewNum = pGnss->satellite_view_num;			//可视卫星数目（GPS+北斗之和）
	
	//SL_Print("The Gnss Ant State222:%d\r\n",pMsgGnss->MsgGnssStatus);

	if(pGnss->status == 'A')
		pMsgGnss->MsgGnssStatus = pGnss->status | 0x02;         	//   定位状态
	else
		pMsgGnss->MsgGnssStatus = pGnss->status & 0xFD;				//
	
	if(pTerState->GnssAntShort > 0 || pTerState->GnssAntState > 0)   //
		pMsgGnss->MsgGnssStatus |= 0x01;
	else
		pMsgGnss->MsgGnssStatus &= 0xFE;

	
	
	//SL_Print("The Gnss Ant State:%d,%d,%d\r\n",pMsgGnss->MsgGnssStatus,pTerState->GnssAntState,pTerState->GnssAntShort);
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
	
	switch(pSysCfg->CarType)
	{
		case 0x01:
			if(pTerState->lockRank == 1)
			{
				pMsgInput->MsgRelayState = 0x01;
				pMsgInput->MsgLockState = 0x01;
			}
			else if(pTerState->lockRank == 2)
			{
				pMsgInput->MsgRelayState = 0x4;
				pMsgInput->MsgLockState = 0x04;
			}
			else
			{
				pMsgInput->MsgRelayState = 0;			
				pMsgInput->MsgLockState = 0;
			}
			
			break;
		case 0x02:
			if(pCanData->LockCarState == 0)              				//未锁车
			{
				pMsgInput->MsgRelayState = 0;			
				pMsgInput->MsgLockState = 0;	
			}
			else if(pCanData->LockCarState == 1)       //
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
			break;
	}
	

	pMsgInput->MsgAccWorkTime = 0;//pTerState->AccWorkTime / 60;								//电锁工作时间
	//SL_Print("The MsgAccWorkTime:%d\r\n",pMsgInput->MsgAccWorkTime);
	pMsgInput->MsgEngineWorkTime = 0;//pTerState->EngineWorkTime / 60.0;								//发动机工作时间
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

			pMsgExcavator->MsgSysVol 					= pCanData->SysVol;           	 	 	//电源电压
			pMsgExcavator->MsgTempWater 				= pCanData->TempWater;           		//冷却水温度
			pMsgExcavator->MsgEngineRotate 				= pCanData->EngineRotate;		 		//发动机实际转速
			pMsgExcavator->MsgEngineRotateSet 			= pCanData->EngineRotateSet;     		//发动时目标转速
			pMsgExcavator->MsgEngineTorque 				= 0;									//发动机实际扭矩
			pMsgExcavator->MsgWorkFlag 					= pCanData->WorkFlag;					//工作档位
			pMsgExcavator->MsgRotaryVal 				= pCanData->RotaryVal;         			//旋钮电压
			pMsgExcavator->MsgOilTemp 					= 0;									//燃油温度
			pMsgExcavator->MsgFuelPumpPressure 			= 0;									//供油泵入口压力
			pMsgExcavator->MsgEngineOilPressure 		= pCanData->AbsoluteOilPressure;		//机油压力
			pMsgExcavator->MsgAirPressure 				= 0;               		 				//大气压力
			pMsgExcavator->MsgAddPressure 				= 0;									//增压压力
			pMsgExcavator->MsgAddPressureTemp 			= 0;									//增压温度
			pMsgExcavator->MsgEnteredAirTemp 			= 0;									//进气温度
			pMsgExcavator->MsgFuelPercent 				= pCanData->FuelPercent;		 		//燃油位
			pMsgExcavator->MsgComPathPressure 			= 0;									//共轨压力
			pMsgExcavator->MsgComPathDis 				= 0;									//共轨压差
			pMsgExcavator->MsgFuelGoalGush 				= 0;									//目标燃油喷射量
			pMsgExcavator->MsgFuelGoalControl 			= 0;									//燃油喷射量控制程度
			pMsgExcavator->MsgWorkTime 					= pCanData->WorkTime;			 		//工作时间
			pMsgExcavator->MsgLockCarState 				= pCanData->LockCarState;				//锁机状态
			
			//SL_Print("the LockCarState:%d\r\n",pCanData->LockCarState);
			pMsgExcavator->MsgControllerFaultCode 		= 0;        //主控制器故障代码
			
			pMsgExcavator->MsgSwitchState            	= pCanData->SwitchState;  				//开关状态
			
			pMsgExcavator->MsgRelayVol 					= pCanData->RelayVol;  					//主继电器电压

			pMsgExcavator->MsgWokrModeState 			= pCanData->WorkModeState;				//状态反馈
			//SL_Print("pCanData->TravelState::%d\r\n",pMsgExcavator->MsgWokrModeState);	   				
			pMsgExcavator->MsgFuelFlow 					= 0;									//燃油流量
			pMsgExcavator->MsgShowEngineRotate 			= 0;           							//指示发动机转速
			pMsgExcavator->MsgFuelQuantity 				= 0;               //燃油使用量
			pMsgExcavator->MsgFuelTotalQuantity 		= pCanData->TotalFuel;			//燃油累积使用量
			//SL_Print("pMsgExcavator->MsgFuelTotalQuantity:%d\r\n",pMsgExcavator->MsgFuelTotalQuantity);
			pMsgExcavator->MsgECMFaultCode 				= 0;				//ECM现在故障代码
			pMsgExcavator->MsgFluidOilTemp 				= 0;				//液压油温
			pMsgExcavator->MsgFrontPumpPressure 		= pCanData->FrontPumpPressure;			//前泵主压力
			pMsgExcavator->MsgBackPumpPressure 			= 0;			//后泵主压力
			pMsgExcavator->MsgBucketRodExtend 			= 0;			//斗杆伸出先导
			pMsgExcavator->MsgBucketRodAnastole 		= 0;				//斗杆缩回先导
			pMsgExcavator->MsgRotation 					= 0;					//回转先导
			pMsgExcavator->MsgTravelL 					= 0;  					//行走L先导
			pMsgExcavator->MsgTravelR 					= 0;					//行走R先导
			pMsgExcavator->MsgSwingArmUp 				= 0;					//动臂上升先导
			pMsgExcavator->MsgSwingArmDrop 				= 0;				//动臂下降先导
			pMsgExcavator->MsgBucketExtend 				= 0; 				//铲斗伸出
			pMsgExcavator->MsgBucketAnastole 			= 0;				//铲斗缩回
			pMsgExcavator->MsgFrontPumpRadiotube 		= pCanData->ECMFaultCode;			//前泵比例电磁阀
			//SL_Print("pCanData->ECMFaultCode :%d\r\n",pCanData->ECMFaultCode);
			pMsgExcavator->MsgBackPumpRadiotube 		= 0;			//后泵比例电磁阀
			pMsgExcavator->MsgBucketRatioRadiotube 		= 0;			//铲斗合流比例电磁阀
			pMsgExcavator->MsgSwingArmRatioRadiotube 	= 0;  			//动臂相对回转比例电磁阀
			pMsgExcavator->MsgSwingArmRadiotube1 		= 0;			//动臂优先1比例电磁阀
			pMsgExcavator->MsgSwingArmRadiotube2 		= 0;	  		//动臂优先2比例电磁阀
			pMsgExcavator->MsgBreakHammerValue 			= 0;			//智能操作时间
			pMsgExcavator->MsgPowerControlValue 		= 0;			//功率调整当前值
				
			pMsgExcavator->MsgCarWarnValue1				= pCanData->MsgCarWarnValue1;  //车辆报警状态1
			pMsgExcavator->MsgCarWarnValue2				= pCanData->MsgCarWarnValue2;  //车辆报警状态2
			pMsgExcavator->MsgWorkTimeDec				= pTerState->EngineWorkTime / 60;   //工作小时计
			//SL_Print("pMsgExcavator->MsgWorkTimeDec:%d\r\n",pMsgExcavator->MsgWorkTimeDec);
			//pMsgExcavator->MsgCarWarnValue2 			= 0;				//
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgExcavatorStr)); 
			
			DataLen += sizeof(MsgExcavatorStr);
			break;
		case 0x02:						  //装载机设备
			pCanData = GetCanDataSpace();
			pMsgLoader = (MsgLoaderStr *)buf;

			pMsgLoader->MsgLoaderType 					= 0x05;       							//
			pMsgLoader->MsgLoaderLen 					= sizeof(MsgLoaderStr) - 3;	   			// 

			pMsgLoader->MsgDeviceVol 					= pTerState->PowerVol;					// 电源电压
			pMsgLoader->MsgDeviceWorkTime 				= pTerState->EngineWorkTime / 60.0;									//GPS上的小时计
			//SL_Print("pMsgLoader->MsgDeviceWorkTime:%d\r\n",pMsgLoader->MsgDeviceWorkTime);

			pMsgLoader->MsgSysVol 						= pCanData->SysVol;						//系统电压（来自仪表）
			pMsgLoader->MsgWorkTime 					= pCanData->MeterWorkTime;				//仪表上的小时计 
			//SL_Print("The Meter Work Time:%d\r\n",pMsgLoader->MsgWorkTime);

			pMsgLoader->MsgTransmitOilTemp 				= pCanData->TransferOilTemp;			//传动油温度 			
			pMsgLoader->MsgBrakePressure 				= pCanData->BrakePressure;				//制动气压
			pMsgLoader->MsgFuelPercent 					= pCanData->FuelPercent;				//燃油位
			pMsgLoader->MsgCabTemp 						= pCanData->OuterTemp;					//驾驶室内温度
			pMsgLoader->MsgOutdoorTemp 					= pCanData->InnerTemp;					//环境温度
			pMsgLoader->MsgShiftHandleLocate		 	= 0;									//换挡手柄档位值
			pMsgLoader->MsgGearboxValue 				= 0;									//变速箱实际档位值
			pMsgLoader->MsgGearboxPressure 				= 0;									//变速箱主压力值
			pMsgLoader->MsgClutchKVPressure 			= 0;									//离合器KV压力值
			pMsgLoader->MsgClutchKRPressure 			= 0;									//离合器KR压力值
			pMsgLoader->MsgClutchK1Pressure 			= 0;									//离合器K1压力值
			pMsgLoader->MsgClutchK2Pressure 			= 0;									//离合器K2压力值
			pMsgLoader->MsgClutchK3Pressure 			= 0;									//离合器K3压力值
			pMsgLoader->MsgClutchK4Pressure 			= 0;									//离合器K4压力值
			pMsgLoader->MsgGearboxMode 					= 0;									//变速箱控制模式
			pMsgLoader->MsgClutchKVCurrent 				= 0;									//离合器KV电流值
			pMsgLoader->MsgClutchKRCurrent 				= 0;									//离合器KR电流值
			pMsgLoader->MsgClutchK1Current 				= 0;									//离合器K1电流值
			pMsgLoader->MsgClutchK2Current 				= 0;									//离合器K2电流值
			pMsgLoader->MsgClutchK3Current 				= 0;									//离合器K3电流值
			pMsgLoader->MsgClutchK4Current 				= 0;									//离合器K4电流值
			pMsgLoader->MsgGearBoxInRotate 				= 0;									//变速箱输入轴转速
			pMsgLoader->MsgGearBoxGearRotate 			= 0;									//变速箱涡轮转速
			pMsgLoader->MsgGearBoxMidRotate 			= 0; 									//变速箱中间轴转速
			pMsgLoader->MsgGearBoxOutRotate 			= 0;									//变速箱输出轴转速
			pMsgLoader->MsgGearBoxFault 				= 0;									//变速箱故障
			TmpData16.TTbyte[0] 						= pCanData->MsgCarWarnValue1;
			TmpData16.TTbyte[1]							= pCanData->MsgCarWarnValue2;
			pMsgLoader->MsgSysWarnvalue 				= TmpData16.IntII;						//报警值
		 	pMsgLoader->MsgTempWater 				    = pCanData->TempWater;           		//冷却水温度
		 	pMsgLoader->MsgEngineRotate 				= pCanData->EngineRotate;				//发动机实际转速

			TmpData16.TTbyte[0] = GetVerNum();
			TmpData16.TTbyte[1] = pTerState->FirNum;
			TmpData16.IntII = 0x00;
			pMsgLoader->MsgEngineFault 					= TmpData16.IntII;           			//当前发动机故障
			pMsgLoader->MsgAirPressure 					= pCanData->AirPressure;        		//大气压力
			//SL_Print("The AirPressure:%d,%d,%d\r\n",TmpData16.TTbyte[0],TmpData16.TTbyte[1],pMsgLoader->MsgEngineFault);
			pMsgLoader->MsgAirTemp 						= pCanData->AirTemp;					//大气温度
			//SL_Print("pMsgLoader->MsgAirTemp:%d\r\n");
			pMsgLoader->MsgFootboardPosition 			= pCanData->Accelerator;  				//油门踏板位置
			pMsgLoader->MsgEngineOilPressure 			= pCanData->AbsoluteOilPressure;		//机油压力
			pMsgLoader->MsgOilTemp 						= pCanData->OilTemp;					//机油温度
			//SL_Print("pMsgLoader->MsgOilTemp:%d,%d\r\n",pMsgLoader->MsgOilTemp,pMsgLoader->MsgFootboardPosition);
			pMsgLoader->MsgAirFanState 					= 0;				// 风扇驱动状态 

			pMsgLoader->MsgAirFanRotate 				= 0;				//风扇转速
			pMsgLoader->MsgFule 						= pCanData->OnceFuel;						//油耗
			//SL_Print("pMsgLoader->MsgFule:%d\r\n",pMsgLoader->MsgFule );
			pMsgLoader->MsgAddPressure 					= pCanData->RelativeAddPressure;			//相对增压压力
			//SL_Print("pMsgLoader->MsgAddPressure:%d\r\n",pMsgLoader->MsgAddPressure);
			pMsgLoader->MsgEnteredAirTemp 				= pCanData->EnteredAirTemp;				//发动机进气歧管温度
			pMsgLoader->MsgCarPowerVol 					= pCanData->EnginePowerVol;				//整车电源电压（发动机）
			pMsgLoader->MsgEnteredWorkTime 				= pCanData->WorkTime;			//发动机累计运行时间
			pMsgLoader->MsgFuleTotal 					= pCanData->TotalFuel; 					//燃油总消耗量
			//SL_Print("pMsgExcavator->MsgFuelTotalQuantity:%d\r\n",pMsgLoader->MsgFuleTotal );
			SL_Memset(&pMsgLoader->MsgECUFirVer[0],0,8);        				//ECU软件版本 
			
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
			pMsgLoader->MsgECULockFunState 				= pCanData->ECURes;						//ECU反馈应答帧（锁车功能）
			//pCanData->ECUState = 0x0E;
			pMsgLoader->MsgECULockState 				= pCanData->ECUState;					//ECU反馈状态帧（锁车状态）
			//SL_Print("THe pCanData->ECUState:%d\r\n",pCanData->ECUState);
			pMsgLoader->MsgEntereLockState 				= pCanData->EngineLockState;			//发动机主动锁车状态
			pMsgLoader->MsgEntereWorkMode 				= 0;									//发动机工作模式(作业强度)
			pMsgLoader->MsgHydraulicPressure 			= 0;									//液压系统工作压力

			//pMsgLoader->MsgMildLoadTime                 = 0;									//空载时间
			//pMsgLoader->MsgMediumLoadTime				= 0;									//中载时间
			//pMsgLoader->MsgSevereLoadTime				= 0;									//重载时间
			//

			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgLoaderStr)); 
			//SL_Print("MsgLoaderStr Len...............%d\r\n",sizeof(MsgLoaderStr));        	//
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









