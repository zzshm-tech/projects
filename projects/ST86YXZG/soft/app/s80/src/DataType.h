
/***********************************************
**	FileName:DataType.h
**	Time:2017.01.26
**	Code:UTF-8
************************************************/

#ifndef DATATYPE_H
#define DATATYPE_H


#define BLIND_NUM  500                                           //
#define BLIND_BUF  180	



//
typedef struct
{
	unsigned char FaultCodeNum;         	//
	unsigned char FaultCode[160];
}CurrentFaultCodeStr;             			 //


/***************************************************************
**	
****************************************************************/

typedef struct 
{
	double 					latitude;								//维度
	double 					longitude;	  							//经度
	unsigned short int 		altitude;       						//
	unsigned short int 		azimuth;	  							//
	float 					speed;		            				//
	float  					ghdop_v;      							//
	unsigned char 			satellite_num;         	 				//
	unsigned char 			satellite_view_num; 					//
	unsigned char 			status;                 				//
	unsigned char			TYear;                                  //GNSS-年
	unsigned char 			TMon;									//GNSS-月
	unsigned char 			TDay;									//GNSS-日
	unsigned char 			THour;									//GNSS-时
	unsigned char			TMin;									//GNSS-分	
	unsigned char 			TSec;                                   //GNSS-秒                      
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
//#pragma pack(1)

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
	
	unsigned short int 		FtpFileLen;            					//
	unsigned char 			*FtpFileSapce;          				//
	unsigned char 			FtpLoadFig;          					//
	unsigned int			Crc32Value;								//

	unsigned char 			LocalLogFlag;				//日志标志
}TcpIPSocketStr;
//#pragma pack()


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

	unsigned char 			TerminalID[18];      					//设备号：
	unsigned char           TerminalType;		 					//设备类型
	unsigned char 			FirVerNum;			 					//硬件版本号
	unsigned char 			NumNum;              					//
	unsigned char 			ServerAddr[50];      					//网关服务器地址
	unsigned int short 		ServerPort;     	 					//网关服务器端口
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
	unsigned char 			ICCID[25];								//
}SysConfigStr;


/************************************
**
**************************************/
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


/************************************************
**	
*************************************************/
#pragma pack(1)

typedef struct
{
	unsigned char 			DataBuf[180];     				//
	unsigned short int 		DataLen;          				//
	unsigned char 			DataCrc;          				//
	unsigned char       	DataBig;          				//
	
}FiFoStr;
#pragma pack()

/*************************************************************
**	
*************************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int 		QNum;             						//
	unsigned short int 		QWrite;           						//
	unsigned short int 		QRead;            						//
	FiFoStr            		QData[BLIND_NUM];       				//
}SendQueueStr;
#pragma pack()




/*****************************************
**	
*******************************************/
#pragma pack(1)
typedef struct
{
	unsigned short int 		SysVol;           	 		//
	unsigned char 			TempWater;           		//
	unsigned short int 		EngineRotate;		 		//
	unsigned short int 		EngineRotateSet;     		//
	unsigned char 			EngineTorque;				//
	unsigned char 			FuelTemp;					//
	unsigned short int 		OilTemp;         			//
	unsigned char 			AirPressure;         		//
	unsigned short int   	EngineNacelleTemp;			//
	unsigned short int 		AirTemp;					//
	unsigned char 			EnteredAirTemp;				//
	unsigned short int		WayTemp;					//
	unsigned int			EngineWorkTime;				//
	unsigned char 			Accelerator;				//
	unsigned char			EngineLoad;  				//
	unsigned short int 		TravelSpeed;				//
	unsigned int 			OnceTravel;					//
	unsigned int 			TotalTravel;  				//
	unsigned int 			OnceFuel;      				//
	unsigned int 	   		TotalFuel;  				//
	unsigned char			RelativeOilPressure;		//
	unsigned char 			AbsoluteOilPressure;		//
	unsigned char			RelativeAddPressure;		//
	unsigned char 			AbsoluteAddPressure;		//
	unsigned char 			OilPosition;				//
	unsigned short int  	CrankcasePressure;   		//
	unsigned char 			CoolPressure;  				//
	unsigned char 			CoolPosition;				//
	unsigned char     		LockCarState;				//
	unsigned char			ActivateStatus;   			//
	unsigned char 			KeyStatus;     				//
	unsigned char 			TerIDStatus;				//
	unsigned char 			ColdBootStatus;     		//
	unsigned char 			SpeedStatus;				//
	unsigned char 			PedalStatus;     			//
	unsigned char 			AcceleratorStatus;   		//
	unsigned char 			ClutchStatus;				//
	unsigned char			BrakeStatus;				//
	unsigned char			CruiseSpeed;				//
	unsigned char           CruiseConStatus;            //
	unsigned char			CruiseEnableStatus;			//
	unsigned char			CruiseAcitveStatus;			//
	unsigned char 			CarWarnValue1;				//
	unsigned char 			CarWarnValue2;				//
}CanDataStr;
#pragma pack()


/********************************************
**
*********************************************/

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
	
}GSMStationInfoStr;    						//
#pragma pack()


/******************************************
**	
**
******************************************/
#pragma pack(1)
typedef struct
{
	unsigned char 			FrameStart[3];			// 
    unsigned char			msg_id;          		//
    unsigned char 			device_id[16];   		// 
    unsigned short int 		DataPackFlag;			//
    unsigned char 			msg_body_num;     		// 	
    unsigned short int 		msg_len;        		//  
}SysCmdStr;
#pragma pack()


/********************************************
**	
**	
********************************************/
#pragma pack(1)
typedef struct
{
	unsigned short int 		MsgLoderMechType;				//
	unsigned short int		MsgLoderMechLen;        		//

	unsigned short int 		MsgSysVol;           	 		//
	unsigned char 			MsgTempWater;           		//
	unsigned short int 		MsgEngineRotate;		 		//
	unsigned short int 		MsgEngineRotateSet;     		//
	unsigned char 			MsgEngineTorque;				//
	unsigned char 			MsgFuelTemp;					//
	unsigned short int 		MsgOilTemp;         			//
	unsigned char 			MsgAirPressure;         		//
	unsigned short int 		MsgEngineNacelleTemp;			//
	unsigned short int 		MsgAirTemp;						//
	unsigned char 			MsgEnteredAirTemp;				//
	unsigned short int		MsgWayTemp;						//
	unsigned int			MsgEngineWorkTime;				//
	unsigned char 			MsgAccelerator;					//
	unsigned char			MsgEngineLoad;  				//
	unsigned short int 		MsgTravelSpeed;					//
	unsigned int 			MsgOnceTravel;					//
	unsigned int 			MsgTotalTravel;  				//
	unsigned int 			MsgOnceFuel;      				//
	unsigned int 	   		MsgTotalFuel;  					//
	unsigned char			MsgRelativeOilPressure;			//
	unsigned char 			MsgAbsoluteOilPressure;			//
	unsigned char			MsgRelativeAddPressure;			//
	unsigned char 			MsgAbsoluteAddPressure;			//
	unsigned char 			MsgOilPosition;					//
	unsigned short int  	MsgCrankcasePressure;   		//
	unsigned char 			MsgCoolPressure;  				//
	unsigned char 			MsgCoolPosition;				//
	unsigned char     		MsglockCarStatus;				//
	unsigned char			MsgTerMonStatus;   				//
	unsigned char 			MsgKeyStatus;     				//
	unsigned char 			MsgTerIDStatus;					//
	unsigned char 			MsgColdBootStatus;     			//
	unsigned char 			MsgSpeedStatus;					//
	unsigned char 			MsgPedalStatus;     			//
	unsigned char 			MsgAcceleratorStatus;   		//
	unsigned char 			MsgClutchStatus;				//
	unsigned char			MsgBrakeStatus;					//
	unsigned char			MsgCruiseSpeed;					//
	unsigned char           MsgCruiseConStatus;             //
	unsigned char			MsgCruiseEnableStatus;			//
	unsigned char			MsgCruiseAcitveStatus;			//
	unsigned char 			MsgCarWarnValue1;				//
	unsigned char 			MsgCarWarnValue2;				//
}MsgLoaderStr;                //
#pragma pack()


//
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
	
}LockCarStr;
#pragma pack()


//消息头定义
#pragma pack(1)
typedef struct 
{
    unsigned char 			frame_start[3];			// 
    unsigned char			msg_id;          		// 
    unsigned char 			device_id[16];   		//
    unsigned char 			CarTypeNum;             //
    unsigned char 			DataPackFlag;			//
    unsigned char 			msg_body_num;     		//
    unsigned short int 		msg_len;        		// 
}MsgHeadStr;

#pragma pack()


//

#pragma pack(1)
typedef struct 
{
    unsigned char 			FrameStart[3];			//
    unsigned char			MsgId;          		// 
	unsigned short int 		MsgLen;					//
}MsgQDHeadStr;
#pragma pack()


/*************************************************************
**
************************************************************/

#pragma pack(1)
typedef struct 
{
	unsigned short int 		MsgDeviceType;                  //
	unsigned short int 		MsgDeviceLen;                 	//
	
	
	unsigned char      		MsgManuNum;            			//	      	
	unsigned char      		MsgTerminalType;        		//
	unsigned char 			MsgUserNum;                     //
	unsigned char      		MsgAppVer1;              		//
	unsigned char 			MsgAppVer2;           			//
	unsigned char      		MsgHardwareVer;             	//
}MsgDeviceStr;
#pragma pack()



/*************************************************
**
*************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int   	MsgInputType;					//
	unsigned short int 		MsgInputLen;					//
		
	unsigned char      		MsgInputIo;						//
	unsigned char 			MsgAcc;     					//
	unsigned char 			MsgMoto;
	unsigned short int 		MsgInputFrq1;					//
	unsigned short int 		MsgInputFrq2;					//
	unsigned short int 		MsgInputFrq3;					//
	unsigned short int 		MsgInputFrq4;					//
	unsigned short int 		MsgInputVol1;					//
	unsigned short int 		MsgInputVol2;					//
	unsigned short int 		MsgPowVol;						//
	unsigned short int 		MsgBatteryVol;					//
	unsigned int			MsgWarnValue;					//
	unsigned char 			MsgLine;   						//
} MsgInputStr;

#pragma pack()



/******************************************************
**	
*******************************************************/
#pragma pack(1)

typedef struct 
{
	unsigned short int  	MsgGnssType;                  	//
	unsigned short int 		MsgGnssLen;                   	//
	
	unsigned int       		MsgGnssLatitude;        		//
	unsigned int       		MsgGnssLongitude;             	//
	unsigned short int	 	MsgGnssSpeed;           		//
	unsigned short int	 	MsgGnssAzimuth;         		//
	short int 				MsgGnssAltitude;        		//
	unsigned char      		MsgGnssYear;                  	//
	unsigned char      		MsgGnssMon;                   	//
	unsigned char      		MsgGnssDay;						//
	unsigned char      		MsgGnssHour;                 	//
	unsigned char      		MsgGnssMin;                   	//
	unsigned char      		MsgGnssSec;                   	//
	unsigned char      		MsgGnssSatelliteNum;         	//
	unsigned char      		MsgGnssViewNum;              	//
	unsigned short 			MsgGhdopV;						//
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




union di_state
{
	struct
	{
		unsigned char Acc :1;
		unsigned char Moto:1;
		unsigned char I1 :1;
		unsigned char I2 :1;
		unsigned char Q1 :1;
		unsigned char Q2 :1;
		unsigned char Q3 :1;
		unsigned char Q4 :1;
	} bit;		
	unsigned char vaule;
};




/***************************************************
**	
***************************************************/
#pragma pack(1)

typedef struct
{
	unsigned char 				AccState;				//ACC状态
	unsigned char 				GnssAntState;           //Gnss天线状态
	unsigned char 				GnssAntShort;			//Gnss天线短路
	unsigned char 				ShellState;				//外壳开启状态
	unsigned char 				CanConnect;				//CAN连接状态
	unsigned short int 			PowerVol;               //外部供电电压
	unsigned short int 			BatteryVol;             //设备备用电池电压
	unsigned char	 			cmd_lock;               //锁车命令
	unsigned char				cmd_unlock;				//监控命令
	unsigned int 				McuTime;				//单片机时间  		()
	union di_state				IOStatus;				//外部输入状态 																							//bit0:Acc 0��???3��1?��?��?1��???3���䨰?a  
	unsigned char				cmd_unmon; 				//MOTO状态
	unsigned char 				cmd_mon;				//FRQ1输入   ECU握手数据
	unsigned char 				cmd_hand;         		//FRQ1输出 
	unsigned short int 			FirNum;                 //单片机版本号
	unsigned char 				LockState;				//锁车状态
	unsigned char 				MonState;				//车辆监控状态  	 ()
	
}TerStateStr;
#pragma pack()

/**************************************************
**	
**************************************************/
typedef struct
{
	unsigned char 			SDCardCheck;                //SD
	unsigned char 			SDCardType;           		//SD
	unsigned short int 		SDCardTotalSpace;    		//SD
	unsigned short int 		SDCardSurplusSpace;         //SD
	unsigned char 			SDCardState;                //SD
	unsigned char 			NC;                         //
}TerSDCardStr;


/**************************************************
**	
***************************************************/
#pragma pack(1)
typedef struct
{
	unsigned int 			ACCTotal;                    			// 
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
	unsigned char			TYear;                                  //
	unsigned char 			TMon;									//
	unsigned char 			TDay;									//
	unsigned char 			THour;									//
	unsigned char			TMin;									//
	unsigned char 			TSec;                                   //                          
	unsigned int            gnss_time;              				//
	unsigned char 			latitude_ns;            				//
	unsigned char 			longitude_ew;           				//

	unsigned int 			CanECUWorkTime;							//ECU工作时长
	unsigned char 			CanOilPosition;							//燃油位

	unsigned char 			LockCarBank;

	unsigned char 			LockCarState;
	unsigned char 			KeyStatus;
	unsigned char 			TerIDStatus;
	unsigned char 			ActivateStatus;

	unsigned short int 		SaveFlag;								//

}SysRunDataStr;
#pragma pack()



//
typedef union  
{
	unsigned short int		IntII;
	unsigned char			TTbyte[2];
}IntToChar;


//
typedef union  
{
	unsigned int			LongLL;
	unsigned char			TTbyte[4];
}LongToChar;


/**************************************
**	
***************************************/
#pragma pack(1)
typedef struct 
{
	unsigned char			UpgradeStatus;		   //0:
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




/*************************

**************************/
union task_state
{
  struct
  {
    unsigned char task_gprs  :1;      
    unsigned char task_gnss  :1;
    unsigned char task_Up  :1;
    unsigned char task_Run :1;
    unsigned char task_Led  :1;
    unsigned char task_SysPro :1;
    unsigned char task_Rtc :1;
    unsigned char Q4 :1;
  } bit;		
  unsigned char value;
};



/****************
**	定义本地日志文件
**********************/

struct LogStr
{
	unsigned char LogFile[10240];		//日志文件缓冲区
	unsigned short int LogLen;   		//日志长度  总长度不会超过10240   10K
};


#endif









/*******************************File End******************************/









