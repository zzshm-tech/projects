
/***********************************************
**	FileName:DataType.h
**	Time:2017.01.26
************************************************/

#ifndef DATATYPE_H
#define DATATYPE_H

/******************************/

#define BLIND_NUM  500                                           //
#define BLIND_BUF  200											//


/***************************************************************
**	
**	
****************************************************************/

typedef struct 
{
	double 					latitude;								//
	double 					longitude;	  							//
	unsigned short int 		altitude;       						//
	unsigned short int 		azimuth;	  							//
	float 					speed;		            				//
	float  					ghdop_v;      							//
	unsigned char 			satellite_num;         	 				//
	unsigned char 			satellite_view_num; 					//
	unsigned char 			satellite_view_num1; 					//
	unsigned char 			status;                 				//
	unsigned char			TYear;                                  //GNSS-
	unsigned char 			TMon;									//GNSS-
	unsigned char 			TDay;									//GNSS-
	unsigned char 			THour;									//GNSS-
	unsigned char			TMin;									//GNSS-
	unsigned char 			TSec;                                   //GNSS-                          
	unsigned int            gnss_time;              				//
	unsigned char 			latitude_ns;            				//
	unsigned char 			longitude_ew;           				//
	unsigned char 			GnssModeStatus;                         //
}GNSSSTR; 


/***************************************************
**
***************************************************/
typedef struct
{
	double					StartLat;         						//
	double 					StartLong;          					//
	double 					EndLat;									//
	double 					EndLong;            					//
	float					Speed; 									//
	unsigned short int 		StartAzimuth;     						//
	unsigned short int 		EndAzimuth;     						//
	double 					Distance;                               //
}DistanceProStr;

/****************************************************/
//TCPIP
typedef struct 
{

	unsigned char 			GprsApn[20];           					//
	unsigned char 			GprsNetActive;		   					//
	BOOL          			SimState;         	   					//
	unsigned char 			SimIccId[25];     	   					//
	unsigned char			GsmSign;       	 						//		
	unsigned char 			ServerAddr[50];        					//
	unsigned short int 		ServerPort;       						// 
	unsigned char 			UlrToIPFlag;	                        //
	unsigned char 			LinkState;             					//
	unsigned char 			FtpLinkState;							//
	unsigned char  			LinkNum;              					//
	unsigned char 			LinkCount;								//
	unsigned char 			GprsLinkState;             				//
	unsigned char 			SendState;              				//
	int			 			SimStatus;              				//
	int						NetWorkState;           				//
	unsigned char			NetGprsState;           				//
	unsigned char 			GprsAttachState;						//
	unsigned char 			ErrorCode;              				//
	unsigned char 			ResetLink;              				//
	unsigned char			LinkType;								//
	unsigned char 			FtpLinkNum;								//
	unsigned char			FtpAddr[50];           					//
	unsigned char 			FtpUserName[20];       					//
	unsigned char 			FtpUserPassd[20];      					//
	unsigned char 			FtpFileName[20];       					// 
	unsigned short int 		FtpPort;              					//
	
	unsigned int 		    FtpFileLen;            					//
	unsigned char 			*FtpFileSapce;          				//
	unsigned char 			FtpLoadFig;          					//
}TcpIPSocketStr;


/*********************************************
**	
*********************************************/

typedef struct
{
	unsigned char 			IpAddr[50];        					//
	unsigned short int      Port;
}IpAddrBackStr;


/************************************************
**	
************************************************/
typedef struct
{

	unsigned char 			TerminalID[18];      					//
	unsigned char           TerminalType;		 					//
	unsigned char 			FirVerNum;			 					//
	unsigned char 			NumNum;              					//
	unsigned char 			ServerAddr[50];      					//
	unsigned int short 		ServerPort;     	 					// 
	unsigned char 			UpdateAddr[50];      					//
	unsigned int short 		UpdatePort;     	 					// 
	unsigned int 			RunTime;            	 				//
	unsigned char 			SleepSign;           					//
	unsigned int 			SleepTime;           					//
	unsigned int 			TravelUpLoadTime;    					//
	unsigned int        	WorkUpLoadTime;      					//
	unsigned short int  	DistanceUpLoad;     	 				//
	unsigned short int  	AzimuthUpLoad;       					// 
	unsigned short int      CanProtocolNum;		 					//
	unsigned char 			CarType;             					//
	unsigned char 			UserCode;            					//
	unsigned char 			CfgFlag;             					//
	unsigned char           TerApn[10];								//
	unsigned char			TerUser[10];							//
	unsigned char			TerPassd[10];							//
	unsigned char 			HardWareNum;							//
	unsigned char 			TestBit;             					//
	unsigned char 			ConLockCar;								// 

	unsigned char 			ICCID[25];
}SysConfigStr;


//
typedef struct
{
	unsigned char 			Year;              								//
	unsigned char 			Mon;               								//
	unsigned char 			MDay;              								//
	unsigned char 			WDay;              								//
	unsigned char 			Hour;              								//
	unsigned char 			Min;               								//
	unsigned char 			Sec;               								//
	unsigned int 			TimeSec;           								//
	unsigned char   		VerifyBit;         								//
}SysTimeStr;


/***********************************************************
**	
***********************************************************/
#pragma pack(1)

typedef struct
{
	unsigned char 			DataBuf[BLIND_BUF];     					//
	unsigned short int 		DataLen;          							//
	unsigned char 			DataCrc;          							//
	unsigned char       	DataBig;          							//
	
}FiFoStr;
#pragma pack()

/*************************************************************
**	
*************************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int 		QNum;             							//
	unsigned short int 		QWrite;           							//
	unsigned short int 		QRead;            							//
	FiFoStr            		QData[BLIND_NUM];       					//
}SendQueueStr;
#pragma pack()




#pragma pack(1)
typedef struct
{
	unsigned short int 			SysVol;           	 		//
	unsigned char 				TempWater;           		//
	unsigned short int 			EngineRotate;		 		//
	unsigned short int 			EngineRotateSet;     		//
	unsigned char 		   		WorkFlag;               	//
	unsigned short int    		RotaryVal;					//
	unsigned short int 			FuelPercent;            	//
	unsigned int				WorkTime;			 		//发动机工作时间
	unsigned char 				LockCarState;				//
	unsigned short int 			RelayVol;  					//
	unsigned char 				SwitchState;				// 
	unsigned char 				WorkModeState;   			//  
	unsigned short int			ECMFaultCode;				//
	unsigned short int 			FrontPumpPressure;			//
	unsigned char 				MsgCarWarnValue1;			//
	unsigned char 				MsgCarWarnValue2;			//
	//
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
   	unsigned char        		TransferOilTemp;  			//
   	unsigned char       	 	BrakePressure;       		//
   	unsigned char        		OuterTemp;          		//
   	unsigned char        		InnerTemp;          		//
	unsigned int 				MeterWorkTime;				//
	unsigned short int 			OilTemp;         			//
	unsigned char 				AirPressure;         		//
	unsigned short int 			AirTemp;					//
	unsigned char 				Accelerator;				//
	unsigned char 				AbsoluteOilPressure;		//
	unsigned int 				OnceFuel;      				//
	unsigned char				RelativeAddPressure;		//
    unsigned char 				EnteredAirTemp;				//
	unsigned int 	   			TotalFuel;  				//
	unsigned short int 			EnginePowerVol;				//

 	unsigned char 				ECURes;						//
	unsigned char 			    ECUState;             		//
	unsigned char 				EngineLockState;			//
}CanDataStr;
#pragma pack()



/***************************************************
**	
****************************************************/

#pragma pack(1)
typedef struct
{
	unsigned char 			GsmSign;
	
	unsigned short int 		GsmLAC1;
	unsigned short int 		GsmCell1;
	unsigned char 			GsmRxLev1;
	unsigned char 			GsmTA1;

	unsigned short int 		GsmLAC2;
	unsigned short int 		GsmCell2;
	unsigned char 			GsmRxLev2;
	unsigned char 			GsmTA2;

	unsigned short int 		GsmLAC3;
	unsigned short int 		GsmCell3;
	unsigned char 			GsmRxLev3;
	unsigned char 			GsmTA3;

	unsigned short int 		GsmLAC4;
	unsigned short int 		GsmCell4;
	unsigned char 			GsmRxLev4;
	unsigned char 			GsmTA4;

	unsigned short int 		GsmLAC5;
	unsigned short int 		GsmCell5;
	unsigned char 			GsmRxLev5;
	unsigned char 			GsmTA5;

	unsigned short int 		GsmLAC6;
	unsigned short int 		GsmCell6;
	unsigned char 			GsmRxLev6;
	unsigned char 			GsmTA6;

	unsigned short int 		GsmLAC7;
	unsigned short int 		GsmCell7;
	unsigned char 			GsmRxLev7;
	unsigned char 			GsmTA7;
	
}GSMStationInfoStr;    //
#pragma pack()


/******************************************
**
******************************************/
#pragma pack(1)
typedef struct
{
	unsigned char 			FrameStart[3];			//包头 
    unsigned char			MsgId;          		//命令吗
	unsigned short int 		MsgLen;					//命令包长度
	unsigned char 			TypeCode;               //类型编码
	unsigned short int 		DataLen; 				//当前类型长度 
	unsigned char 			TerminalCode[6];        //终端流水号
	unsigned short int 		ProVerNum;   			//协议版本
	unsigned char 			ManuNum;                //厂家编号
	unsigned char 			CarType;                //机型
	unsigned int 			SerialNum;              //流水号
	unsigned char 			CmdID;					//命令ID
}SysCmdStr;
#pragma pack()


/********************************************
**	
********************************************/
#pragma pack(1)
typedef struct
{
	unsigned char 			MsgExcavatorType;				//
	unsigned short int		MsgExcavatorLen;                //

	unsigned short int 		MsgSysVol;           	 		//电源电压
	unsigned char 			MsgTempWater;           		//冷却水温度
	unsigned short int 		MsgEngineRotate;		 		//发动机实际装束
	unsigned short int 		MsgEngineRotateSet;     		//发动机目标装束
	unsigned char 			MsgEngineTorque;				//发动机实际扭矩
	unsigned char 			MsgWorkFlag;					//工作档位
	unsigned short int 		MsgRotaryVal;         			//旋钮电压
	unsigned char 			MsgOilTemp;						//燃油温度
	unsigned short int 		MsgFuelPumpPressure;			//供油泵入口压力
	unsigned short int 		MsgEngineOilPressure;			//机油压力
	unsigned char 			MsgAirPressure;                	//大气压力
	unsigned short int 		MsgAddPressure;					//增压压力
	unsigned char 			MsgAddPressureTemp;				//增压温度
	unsigned char 			MsgEnteredAirTemp;				//进气温度
	unsigned char   		MsgFuelPercent;		 			//燃油位
	unsigned char 			MsgComPathPressure;				//共轨压力
	unsigned char 			MsgComPathDis;					//共轨压差
	unsigned short int 		MsgFuelGoalGush;				//目标燃油喷射量
	unsigned char 			MsgFuelGoalControl;				//燃油喷射量控制程度
	unsigned short int		MsgWorkTime;			 		//发动机工作时间
	unsigned char 			MsgLockCarState;				//锁机状态
	unsigned char 			MsgControllerFaultCode;        	//主控制器故障代码
	unsigned char 			MsgSwitchState;					//开关状态
	unsigned short int 		MsgRelayVol;  					//主继电器电压
	unsigned char 			MsgWokrModeState;   			//状态反馈
	unsigned short int 		MsgFuelFlow;					//燃油流量
	unsigned short int 		MsgShowEngineRotate;           	//指示发动机转速
	unsigned int 			MsgFuelQuantity;               	//燃油使用量
	unsigned int 			MsgFuelTotalQuantity;			//燃油累积使用量
	unsigned short int		MsgECMFaultCode;				//ECM现在故障代码
	unsigned char 			MsgFluidOilTemp;				//液压油温
	unsigned short int 		MsgFrontPumpPressure;			//前泵主压力
	unsigned short int 		MsgBackPumpPressure;			//后泵主压力
	unsigned char 			MsgBucketRodExtend;				//斗杆伸出先导
	unsigned char 			MsgBucketRodAnastole;			//斗杆缩回先导
	unsigned char 			MsgRotation;					//回转先导
	unsigned char 			MsgTravelL;  					//行走L先导
	unsigned char 			MsgTravelR;						//行走R先导
	unsigned char 		   	MsgSwingArmUp;					//动臂上升先导
	unsigned char 			MsgSwingArmDrop;				//动臂下降先导
	unsigned char 			MsgBucketExtend; 				//铲斗伸出
	unsigned char 			MsgBucketAnastole;				//铲斗缩回
	unsigned short int		MsgFrontPumpRadiotube;			//前泵比例电磁阀
	unsigned short int 		MsgBackPumpRadiotube;			//后泵比例电磁阀
	unsigned short int 		MsgBucketRatioRadiotube;		//铲斗合流比例电磁阀
	unsigned short int 		MsgSwingArmRatioRadiotube;  	//动臂相对回转比例电磁阀
	unsigned short int 		MsgSwingArmRadiotube1;			//动臂优先1比例电磁阀
	unsigned short int  	MsgSwingArmRadiotube2;	  		//动臂优先2比例电磁阀
	unsigned short int 		MsgBreakHammerValue;			//
	unsigned char 			MsgPowerControlValue;			//功率调整当前值
	unsigned char 			MsgCarWarnValue1;				//车辆报警状态1
	unsigned char 			MsgCarWarnValue2;				//车辆报警状态2  
	unsigned char 			MsgWorkTimeDec;					//辅助仪表进行计时
}MsgExcavatorStr;               // 

#pragma pack()



#pragma pack(1)
/*****************************************************
**	  装载机CAN数据
*****************************************************/
typedef struct
{
	unsigned char 			MsgLoaderType;       		//
	unsigned short int 		MsgLoaderLen;	   			// 

	unsigned short int 		MsgDeviceVol;				//电源电压（GPS的电源电压）
	unsigned int 			MsgDeviceWorkTime;			//GPS上传的小时计

	unsigned short int 		MsgSysVol;					//系统电压（仪表传送过来的）
	unsigned int            MsgWorkTime;				//仪表上的小时计
	unsigned char 			MsgTransmitOilTemp;			//传动油温			
	unsigned char 			MsgBrakePressure;			//制动气压
	unsigned char 			MsgFuelPercent;				//燃油液位
	unsigned char 			MsgCabTemp;					//驾驶室内温度
	unsigned char 			MsgOutdoorTemp;				//环境温度
	unsigned char 			MsgShiftHandleLocate;		//换挡手柄位置
	unsigned char 			MsgGearboxValue;			//变速箱实际档位值
	unsigned char 			MsgGearboxPressure;			//变速箱主压力值
	unsigned char 			MsgClutchKVPressure;		//离合器KV压力值
	unsigned char 			MsgClutchKRPressure;		//离合器KR压力值
	unsigned char 			MsgClutchK1Pressure;		//离合器K1压力值
	unsigned char 			MsgClutchK2Pressure;		//离合器K2压力值
	unsigned char 			MsgClutchK3Pressure;		//离合器K3压力值
	unsigned char 			MsgClutchK4Pressure;		//离合器K4压力值
	unsigned char 			MsgGearboxMode;				//变速箱控制模式
	unsigned char 			MsgClutchKVCurrent;			//离合器KV电流值
	unsigned char 			MsgClutchKRCurrent;			//离合器KR电流值
	unsigned char 			MsgClutchK1Current;			//离合器K1电流值
	unsigned char 			MsgClutchK2Current;			//离合器K2电流值
	unsigned char 			MsgClutchK3Current;			//离合器K3电流值
	unsigned char 			MsgClutchK4Current;			//离合器K4电流值
	unsigned short int		MsgGearBoxInRotate;			//变速箱输入轴转速
	unsigned short int      MsgGearBoxGearRotate;		//变速箱涡轮转速
	unsigned short int 		MsgGearBoxMidRotate; 		//变速箱中间轴转速
	unsigned short int		MsgGearBoxOutRotate;		//变速箱输出轴转速
	unsigned short int 		MsgGearBoxFault;			//变速箱故障
	unsigned short int 		MsgSysWarnvalue;			//报警值（0-）
 	unsigned char 			MsgTempWater;           	//发动机冷却水温度
 	unsigned short int 		MsgEngineRotate;			//发动机转速
	unsigned short int 		MsgEngineFault;           	//当前发动机故障													
	unsigned char 			MsgAirPressure;        		//大气压力
	unsigned short int		MsgAirTemp;					//大气温度
	unsigned char			MsgFootboardPosition;  		//踏板位置
	unsigned char 		    MsgEngineOilPressure;		//机油压力
	unsigned short int 		MsgOilTemp;					//机油温度
	unsigned char 			MsgAirFanState;				//风扇驱动状态
	unsigned short int 		MsgAirFanRotate;			//风扇转速
	unsigned short int 		MsgFule;					//油耗
	unsigned char 			MsgAddPressure;				//相对增压压力
	unsigned char 			MsgEnteredAirTemp;			//发动机进气歧管温度
	unsigned short int		MsgCarPowerVol;				//整车电源电压（发动机）
	unsigned int 			MsgEnteredWorkTime;			//发动机累计工作时间
	unsigned int 			MsgFuleTotal; 				//燃油总消耗量
	unsigned char 			MsgECUFirVer[8];			//ECU软件版本
	unsigned char		    MsgECULockFunState;			//ECU反馈应答帧（锁车功能）
	unsigned short int		MsgECULockState;			//ECU反馈状态帧（锁车状态）
	unsigned char 			MsgEntereLockState;			//发动机主动锁车状态
	unsigned char 			MsgEntereWorkMode;			//发动机工作模式(作业强度)
	unsigned short int		MsgHydraulicPressure;		//液压系统工作压力
	//unsigned int 			MsgMildLoadTime;				//空载时间
	//unsigned int 			MsgMediumLoadTime;				//中载时间
	//unsigned int 			MsgSevereLoadTime;			//重载时间
}MsgLoaderStr;               //
#pragma pack()


/*****************************
 * 
*****************************/
#pragma pack(1)
typedef struct
{
	unsigned char		 	LockCarRank;  			//
	unsigned char		 	LockCarCmd;            	//
	unsigned char        	LockCardState;      	//
	unsigned char 			LockCurrentState;		//
	unsigned char 			MonCurrentState;		//
	unsigned char 		 	LockCmdBig;				//
	unsigned char 			MonCmdByte;				//
	unsigned char 			MonCmdBig;				//
	
	unsigned char 			GpsID[3];				//
	unsigned char 			Secret[3];				//
	unsigned char 			ReplaceDevBig;			//
	unsigned char 			ReplaceDevState;		//	
}LockCarStr;
#pragma pack()


/*****************************
 * 
*****************************/
#pragma pack(1)
typedef struct 
{
    unsigned char 			frame_start[3];			// 设备开头
    unsigned char			msg_id;          		// 消息体ID
    unsigned char 			device_id[16];   		// 设备DI
	unsigned char 			blind_flag;             // 盲区标志
    unsigned char 			msg_line;  				// 
    unsigned char 			msg_body_num;     		// 
    unsigned short int 		msg_len;        		// 
}MsgHeadStr;

#pragma pack()


/*****************************
 * 
*****************************/

#pragma pack(1)
typedef struct 
{
    unsigned char 			FrameStart[3];			// 命令设备开头
    unsigned char			MsgId;          		// 消息ID
	unsigned short int 		MsgLen;					// 消息长度
}MsgQDHeadStr;
#pragma pack()


/*************************************************************
**	
************************************************************/

#pragma pack(1)
typedef struct 
{
	unsigned char 	 		MsgDeviceType;                  //
	unsigned short int 		MsgDeviceLen;                 	//
	
	unsigned char 			MsgDeviceID[8];					//
	unsigned char	 		MsgProtocolVer;     			//
	unsigned char 			MsgFirVer;						//
	unsigned char      		MsgManuNum;            			//
	unsigned char 	   		MsgManuType;            		//
	unsigned char      		MsgDeviceCarNum;        		//  
	unsigned char      		MsgAppVer;              		//       
	unsigned char      		MsgHardwareVer;             	//
}MsgDeviceStr;
#pragma pack()



/*************************************************
**	
*************************************************/
#pragma pack(1)

typedef struct
{
	unsigned char   		MsgInputType;
	unsigned short int 		MsgInputLen;
		
	unsigned char      		MsgInputIo;						//
	unsigned short int 		MsgInputFrq1;					//
	unsigned short int 		MsgInputFrq2;					//
	unsigned short int 		MsgInputFrq3;					//
	unsigned short int 		MsgInputFrq4;					//
	unsigned short int 		MsgPowVol;						//电源电压
	unsigned char	 		MsgBatteryVol;					//锂电池电压
	unsigned short int 		MsgInputVol1;					//第一路电压检测
	unsigned short int 		MsgInputVol2;					//第二路电压检测
	unsigned char 			MsgRelayState;					//继电器锁车状态反馈
	unsigned char 			MsgAlrmState;  					//报警状态
	unsigned short int 		MsgLockCar;						//锁车方式
	unsigned char 			MsgLockState;					//锁车状态
	unsigned int 			MsgAccWorkTime;    				//
	unsigned int 			MsgEngineWorkTime;  			//
	unsigned char 			MsgDataMode;   					//
} MsgInputStr;

#pragma pack()



/******************************************************
**
*******************************************************/
#pragma pack(1)

typedef struct 
{
	unsigned char 			MsgGnssType;                  	//
	unsigned short int 		MsgGnssLen;                   	//
	
	unsigned int       		MsgGnssLatitude;        		//
	unsigned int       		MsgGnssLongitude;             	//
	unsigned char	 		MsgGnssSpeed;           		//
	unsigned char	 		MsgGnssAzimuth;         		//
	short int 				MsgGnssAltitude;        		//
	unsigned char      		MsgGnssYear;                  	//
	unsigned char      		MsgGnssMon;                   	//
	unsigned char      		MsgGnssDay;						//
	unsigned char      		MsgGnssHour;                 	//
	unsigned char      		MsgGnssMin;                   	//
	unsigned char      		MsgGnssSec;                   	//
	unsigned char      		MsgGnssSatelliteNum;         	//
	unsigned char      		MsgGnssViewNum;              	//
	unsigned char      		MsgGnssStatus;              	//
}MsgGnssStr;

#pragma pack()



/***************************************************
**	
****************************************************/
typedef struct
{
	unsigned short int TypeCode;
	unsigned short int DataLen;
	void *table;
}TerDataBlockStr;

/****************************************************
**	
*****************************************************/
typedef struct
{	
	unsigned char 	flag;
	void 			*block;
}TerDataTcbStr;



/***************************************************
**	
***************************************************/
#pragma pack(1)

typedef struct
{
	unsigned char 				AccState;				//ACC状态
	unsigned char 				GnssAntState;           //Gnss
	unsigned char 				GnssAntShort;			//Gnss
	unsigned char 				ShellState;				//
	unsigned char 				CanConnect;				//
	unsigned short int 			PowerVol;               //
	unsigned short int 			BatteryVol;             //
	unsigned short int 			McuTemp;                //
	unsigned int 				McuTime;				//
	unsigned short int 			AccVol;					//

	unsigned short int 			FirNum;                 //
	unsigned char 				LockState;				//
	unsigned char 				MonState;				//

	unsigned char 				MonExpectState;			//期望状态
	unsigned char 				lockRank;		//期望锁车状态
	
	unsigned int				AccWorkTime;			//ACC工作时间
	unsigned int				EngineWorkTime;        	//发动机工作时间
	
}TerStateStr;
#pragma pack()

/**************************************************
**
**	
**************************************************/
typedef struct
{
	unsigned char 			SDCardCheck;                //
	unsigned char 			SDCardType;           		//
	unsigned short int 		SDCardTotalSpace;    		//
	unsigned short int 		SDCardSurplusSpace;         //
	unsigned char 			SDCardState;                //
	unsigned char 			NC;                         //
}TerSDCardStr;


/**************************************************
**	
***************************************************/
#pragma pack(1)
typedef struct
{

	unsigned int 			ACCTotal;                    			//ACC总开启时间
	double		 			ACCTotalHour;                			//
	double 					TotalDistance;               			//
	unsigned int 			TimeSec;                     			//
	double 					latitude;								//
	double 					longitude;	  							//
	unsigned short int 		altitude;       						//
	unsigned short int 		azimuth;	  							//
	float 					speed;		            				//
	float  					ghdop_v;      							//
	unsigned char 			satellite_num;         	 				//
	unsigned char 			satellite_view_num; 					//
	unsigned char 			status;                 				//
	unsigned char			TYear;                                  //GNSS-
	unsigned char 			TMon;									//GNSS
	unsigned char 			TDay;									//GNSS
	unsigned char 			THour;									//GNSS
	unsigned char			TMin;									//GNSS
	unsigned char 			TSec;                                   //GNSS                          
	unsigned int            gnss_time;              				//
	unsigned char 			latitude_ns;            				//
	unsigned char 			longitude_ew;           				//
}SysRunDataStr;
#pragma pack()



/***********************
**
***************************/
typedef union  
{
	unsigned short int		IntII;
	unsigned char			TTbyte[2];
}IntToChar;


/***********************
**
***************************/
typedef union  
{
	unsigned int			LongLL;
	unsigned char			TTbyte[4];
}LongToChar;


/***********************
**
***************************/
#pragma pack(1)
typedef struct 
{
	unsigned char			UpgradeStatus;		   //
	unsigned char 			UpgradeFlag;           //
 }UpgradeStatusStr;
#pragma pack()



/**************************************
**
**************************************/
#pragma pack(1)
typedef struct
{
	unsigned int 			FileTotalLen;           //
	unsigned short int 		FileVerfy;				//
	unsigned short int 		FileCurrentFram;	
	unsigned short int 		FileTotalFram;   		//
	unsigned short int 		FileLen[100];           
	unsigned char 			FileBuf[100][1024];   	//
}UpgradeFileStr;
#pragma pack()




/**************************************
**	
**************************************/
#pragma pack(1)
typedef struct
{
	unsigned int 			FileLen;           			//
	unsigned short int 		FileVerfy;					//
	unsigned int 			CurrentLen;					//
	unsigned int 			CurrentFramNum;   			//  
}UpgradeST86FileStr;
#pragma pack()



#endif









/*******************************File End******************************/









