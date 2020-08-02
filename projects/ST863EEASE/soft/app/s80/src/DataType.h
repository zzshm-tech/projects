
/***********************************************
**	FileName:DataType.h
**	Time:2017.01.26
**	Fun:定时系统使用的自定义数据结构
************************************************/

#ifndef DATATYPE_H
#define DATATYPE_H


#define BLIND_NUM  250                                           //盲区数量



/***************************************************************
**	定位数据结构
**	
****************************************************************/

typedef struct 
{
	double 					latitude;								//维度
	double 					longitude;	  							//经度
	unsigned short int 		altitude;       						//海拔高度
	unsigned short int 		azimuth;	  							//航向角度
	float 					speed;		            				//速度
	float  					ghdop_v;      							//定位因子
	unsigned char 			satellite_num;         	 				//使用卫星数量
	unsigned char 			satellite_view_num; 					//可视卫星数量
	unsigned char 			status;                 				//定位状态  
	unsigned char			TYear;                                  //GNSS-时间-年
	unsigned char 			TMon;									//GNSS-时间-月
	unsigned char 			TDay;									//GNSS-时间-日
	unsigned char 			THour;									//GNSS-时间-时
	unsigned char			TMin;									//GNSS-时间-分
	unsigned char 			TSec;                                   //GNSS-时间-秒                           
	unsigned int            gnss_time;              				//定位时间,改时间以2000年1月1号，0时0分0秒为基准。
	unsigned char 			latitude_ns;            				//维度标识
	unsigned char 			longitude_ew;           				//精度标识
	unsigned char 			GnssModeStatus;                         //定位状态
}GNSSSTR; 


/***************************************************
**	定位计算使用
***************************************************/
typedef struct
{
	double					StartLat;         						//开始经度
	double 					StartLong;          					//开始维度
	double 					EndLat;									//结束经度
	double 					EndLong;            					//结束维度
	float					Speed; 									//行驶速度
	unsigned short int 		StartAzimuth;     						//航向角度
	unsigned short int 		EndAzimuth;     						//航向角度
	double 					Distance;                               //行驶距离
}DistanceProStr;

/****************************************************/
//TCPIP连接信息
typedef struct 
{

	unsigned char 			GprsApn[20];           					//接入方式
	unsigned char 			GprsNetActive;		   					//GPRS网络状态
	BOOL          			SimState;         	   					//是否有Sim卡
	unsigned char 			SimIccId[25];     	   					//SIM卡ICC_ID
	unsigned char			GsmSign;       	 						//GSM信号值			
	unsigned char 			ServerAddr[50];        					//平台地址  服务器地址
	unsigned short int 		ServerPort;       						//端口    服务器端口  
	unsigned char 			UlrToIPFlag;	                        //地址标识，0:IP地址，1:URL	
	unsigned char 			LinkState;             					//连接状态
	unsigned char 			FtpLinkState;							//FTP链接状态
	unsigned char  			LinkNum;              					//连接次数 主要记住链接多少次之后，链接默认网关地址
	unsigned char 			LinkCount;								//GPRS附着次数，
	unsigned char 			GprsLinkState;             				//Gprs链接状态
	unsigned char 			SendState;              				//发送状态 
	int			 			SimStatus;              				//Sim卡状态
	int						NetWorkState;           				//移动网络状态
	unsigned char			NetGprsState;           				//GPRS网络状态   GPRS网络是否附着 
	unsigned char 			GprsAttachState;						//GPRS网络附着状态
	unsigned char 			ErrorCode;              				//误码率 ，暂时没有使用 
	unsigned char 			ResetLink;              				//重新连接
	unsigned char			LinkType;								//(1)FPT 或者(0)TCP/IP链接  
	unsigned char 			FtpLinkNum;								//FTP连接次数
	unsigned char			FtpAddr[50];           					//Ftp服务器地址
	unsigned char 			FtpUserName[20];       					//用户名
	unsigned char 			FtpUserPassd[20];      					//密码
	unsigned char 			FtpFileName[20];       					//文件名称  
	unsigned short int 		FtpPort;              					//端口
	
	unsigned short int 		FtpFileLen;            					//文件长度
	unsigned char 			*FtpFileSapce;          				//FTP下载的文件指针
	unsigned char 			FtpLoadFig;          					//下载标识
	
}TcpIPSocketStr;


/*********************************************
**	
*********************************************/

typedef struct
{
	unsigned char 			IpAddr[50];        					//平台地址  服务器地址
	unsigned short int      Port;
}IpAddrBackStr;


/************************************************
**	系统配置
************************************************/
typedef struct
{

	unsigned char 			TerminalID[18];      					//设备ID
	unsigned char           TerminalType;		 					//终端类型
	unsigned char 			FirVerNum;			 					//硬件版本号
	unsigned char 			NumNum;              					//厂价编码
	unsigned char 			ServerAddr[50];      					//服务器地址(服务器地址可以是IP)
	unsigned int short 		ServerPort;     	 					//服务器端口 
	unsigned char 			UpdateAddr[50];      					//设备更新地址
	unsigned int short 		UpdatePort;     	 					//设备固件更新地址   
	unsigned int 			RunTime;            	 				//休眠前运行时间
	unsigned char 			SleepSign;           					//休眠标识 
	unsigned int 			SleepTime;           					//休眠时间
	unsigned int 			TravelUpLoadTime;    					//车辆行驶时数据上传时间间隔  单位秒	
	unsigned int        	WorkUpLoadTime;      					//耕种时上传数据间隔时间 
	unsigned short int  	DistanceUpLoad;     	 				//定距数据上报
	unsigned short int  	AzimuthUpLoad;       					//航向角数据上报 
	unsigned short int      CanProtocolNum;		 					//CAN协议号 
	unsigned char 			CarType;             					//车辆类型
	unsigned char 			UserCode;            					//用户编码  
	unsigned char 			CfgFlag;             					//配置标志位
	unsigned char           TerApn[10];								//接入点
	unsigned char			TerUser[10];							//用户名
	unsigned char			TerPassd[10];							//密码
	unsigned char 			HardWareNum;							//硬件版本号
	unsigned char 			SaveBit;             					//读写标识
}SysConfigStr;


//系统时间
typedef struct
{
	unsigned char 			Year;              								//系统时间-年
	unsigned char 			Mon;               								//系统时间-月
	unsigned char 			MDay;              								//系统时间-日
	unsigned char 			WDay;              								//系统时间-周
	unsigned char 			Hour;              								//系统时间-时
	unsigned char 			Min;               								//系统时间-分
	unsigned char 			Sec;               								//系统时间-秒
	unsigned int 			TimeSec;           								//系统时间-总秒，以2000年1月1号0时0分0秒计算
	unsigned char   		VerifyBit;         								//时间校准  标识时间是否已经校准
}SysTimeStr;


/***********************************************************
**	数据上传结构
***********************************************************/
#pragma pack(1)

typedef struct
{
	unsigned char 			DataBuf[500];     							//发送缓冲区
	unsigned short int 		DataLen;          							//数据长度
	unsigned char 			DataCrc;          							//校验
	unsigned char       	DataBig;          							//数据包标识(0:正常数据；1:盲区存储数据)
	
}FiFoStr;
#pragma pack()

/*************************************************************
**	发送队列
*************************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int 		QNum;             							//发送队列
	unsigned short int 		QWrite;           							//发送队列对头
	unsigned short int 		QRead;            							//发送队列对尾
	FiFoStr            		QData[BLIND_NUM];       				    //发送队列数据缓冲区
}SendQueueStr;
#pragma pack()


/**************************************************************
**	CAN数据结构体(小麦机)
***************************************************************/

#pragma pack(1)

typedef struct
{
	unsigned short int   EngineRotate;      						//发动机转速
	unsigned char        WaterTemp;    								//冷却水温度
	unsigned char        EngineOil;    								//机油压力
	unsigned char        FuelPercent;  								//燃料百分比
	unsigned int         EngineWorkTime;    						//发动机工作时间  
	unsigned short       SysVal;            						//系统电压
	unsigned int         TotalTravlled;     						//总行驶里程
	unsigned short int   TravlledSpeed;     						//行驶速度 
	unsigned short int   Rotate1;        							//滚筒转速 
	unsigned short int   Rotate2;        							//复脱器转速
	unsigned short int   Rotate3;        							//籽粒升运器
	unsigned short int   Rotate4;        							//清选风扇转速
	unsigned short int   Rotate5;        							//喂入搅龙转速
	unsigned short int   Rotate6;        							//过桥转速
	unsigned short int   LossRatio;         						//清选损失率
	unsigned char        CabTemp;           						//驾驶室温度
	unsigned char  		 ReapHigh;          						//割茬高度
	unsigned char        Aperture1;         						//上筛开度
	unsigned char        Aperture2;         						//下筛开度
	unsigned char        Aperture3;         						//尾筛开度
	unsigned char        WarnValue;         						//粮满报警  
	unsigned char 		 WorkFlag;									//主离合状态
}CanWheatStr;	
#pragma pack()



/************************************************************
**	Can数据结构体(水稻机)
************************************************************/

#pragma pack(1)

typedef struct
{
	unsigned short int   EngineRotate;      						//发动机转速
	unsigned char        WaterTemp;    								//冷却水温度
	unsigned short int   Rotate1;        							//滚筒转速
	unsigned short int   Rotate2;        							//滚动轮转速
	unsigned short       SysVol;            						//系统电压
	unsigned int 		 EngineWorkTime;    						//发动机工作时间 
	unsigned char        WarnValue;         						//粮满报警
	unsigned short int   TravlledSpeed;     						//行驶速度
	unsigned int         TotalTravlled;     						//总行驶里程
}CanPaddyStr;

#pragma pack()

/**************************************************************
**	Can数据结构(玉米机)
***************************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int   EngineRotate;      //发动机转速
	unsigned char        WaterTemp;    		//冷却水温度
	unsigned char        EngineOil;    		//机油压力
	unsigned char        FuelPercent;  		//燃料百分比
	unsigned int 		 EngineWorkTime;    //发动机工作时间 
	unsigned short       SysVol;            //系统电压
	unsigned int         TotalTravlled;     //总行驶里程
	unsigned short int   Rotate1;        	//
	unsigned short int   Rotate2;        	//滚动轮转速
	unsigned short int   TravlledSpeed;     //行驶速度
	unsigned short int   ReapHigh;          //割茬高度
	unsigned char        WarnValue;         //粮满报警

	
}CanMaizeStr;
#pragma pack()


/**************************************************
**	Can数据结构(拖拉机)  (M平台拖拉机)
***************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int	 EngineRotate;		//发动机转速
	unsigned char		 WaterTemp; 		//冷却水温度
	unsigned char		 EngineOil; 		//机油压力
	unsigned char		 FuelPercent;		//燃料百分比
	unsigned int		 EngineWorkTime;	//发动机工作时间 
	unsigned short		 SysVol;			//系统电压
	unsigned short int	 TravlledSpeed; 	//行驶速度
	unsigned int		 TotalTravlled; 	//总行驶里程
	unsigned char		 WarnValue; 		//粮满报警
}CanTractorStr;

#pragma pack()



/**************************************************
**	Can数据结构(拖拉机)  (P平台拖拉机)
***************************************************/
#pragma pack(1)

typedef struct
{
	unsigned char 		 PTOStatus;					//PTO状态
	unsigned short int	 EngineRotate;				//发动机转速    
	unsigned char		 WaterTemp; 				//发动机水温，
	unsigned char 		 SpeedLock;					//差速锁状态（开/关）
	unsigned char        RangeStatus;        		//四驱状态
	unsigned char        BrakeFluidLight;    		//制动液灯
	unsigned char		 DistanceLight;      		//远光灯状态
	unsigned char    	 GeneralLight;		 		//普通灯光状态
	unsigned char 		 StopLight;                 //驻车灯
	unsigned char		 Light1;			 		//前工作灯
	unsigned char		 Light2;             		//前测灯
	unsigned char		 Light3;			 		//后工作灯
	unsigned char		 Light4;			 		//后单点工作灯
	unsigned char		 Light5;             		//左转向灯
	unsigned char		 Light6;             		//右转向灯
	unsigned char		 Light7;            	 	//顶灯
	unsigned char 		 PowerShiftLow;      		//动力转换低速
	unsigned char 		 PowerShiftMid;		 		//动力转换中速
	unsigned char 	     PowerShiftHight;    		//动力转换高速
	unsigned char 		 AirBreakLow;		 		//气压制动低状态
	unsigned char        FuelPositionLow;   		//燃油位置低报警
	unsigned char 		 ShuttleEmpty;       		//梭式换挡空挡
	unsigned char 		 ShuttleRetreat;     		//梭式换挡后退档
	unsigned char 		 ShuttleAdvance;     		//梭式换挡前进挡
	unsigned char 		 CrawlStatus;        		//爬行当撞他
	unsigned int		 TravlledDistance; 	 		//总行驶里程
	unsigned int		 TravlledTotalDistance; 	//总行驶里程 
	unsigned char	     Light9;					//预热塞灯
	unsigned int		 EngineWorkTime;			//发动机工作时间 
	unsigned int 	     CarWorkTime;       		//车辆工作时间
	unsigned int 	     PTOWorkTime;       		//PTO工作时间
	unsigned char    	 EngineFuelTemp;    		//发动机燃料温度    
	unsigned short int	 EngineOilTemp;     		//发动机机油温度   
	unsigned char   	 EngineOilPressure; 		//发动机机油压力
	unsigned char 	     Light10;           		//发动机水温高预警灯
	unsigned char        Light11;					//发动机机油压力低预警灯
	unsigned short int   PTOSped;           		//PTO转速
	unsigned short int   PTOSpeedSet;               //PTO转速设定
	unsigned char		 HandBrake;         		//手制动状态
	unsigned char        ClutchStatus;      		//离合器踏板状态
	unsigned short int	 TravlledSpeed;     		//行驶速度
	unsigned char        EngineAirfiltrate; 		//发动起空滤状态
	unsigned short int   BatteryVol;        		//电池电压
	unsigned char        GearBoxOilLow;     		//变速箱油压低报警
	unsigned char        GearBoxOilHight;   		//变速箱油压高报警
	unsigned char 		 FuelPosition;      		//燃料位置
	unsigned char		 DynamoStatus;      		//发电机充电状态  
}CanPTractorStr;

#pragma pack()




/*****************************************
**	所有机型的CAN数据都放在该数据结构里面
*******************************************/
#pragma pack(1)
typedef struct
{
	unsigned short int    	EngineRotate;      			 //发动机转速
	unsigned char        	WaterTemp;    				 //冷却水温度
	unsigned short int   	Rotate1;        			 //杂余转速
	unsigned short int   	Rotate2;        			 //滚动轮转速
	unsigned short int   	SysVol;            			 //系统电压
	unsigned int 		 	EngineWorkTime;    			 //发动机工作时间 
	unsigned char        	WarnValue1;         		 //粮满报警
	unsigned short int   	TravlledSpeed;     			 //行驶速度
	unsigned int         	TotalTravlled;     			 //总行驶里程  
	unsigned int 		 	OilConsumption;	             //单次油耗油耗
	unsigned int 		 	TatalOilConsumption;         //总油耗  																							
	unsigned char        	EngineOil;    				 //机油压力 
	unsigned char        	FuelPercent;  				 //燃料百分	
}CanDataStr;
#pragma pack()



//P平台当前故障码故障码
typedef struct
{
	unsigned char FaultCodeNum;         	//故障码数量
	unsigned char FaultCode[160];			//
}CurrentFaultCodeStr;             	 	//当前故障码


//配置用户协议号
#pragma pack(1)
typedef struct
{
	unsigned short int   CanProtocolSerial;  //车型协议编号
	unsigned char        LockCardState;      //锁车状态
}TerminalCanProStr;
#pragma pack()


//通讯协议消息头
#pragma pack(1)
typedef struct 
{
    unsigned char 			frame_start[3];			// 消息体头
    unsigned char			msg_id;          		// 消息ID
    unsigned char 			device_id[16];   		// 设备号
	unsigned char 			blind_flag;             // 盲区标识
    unsigned char 			msg_line;  				// 多包消息行号
    unsigned char 			msg_body_num;     		// 消息体数量
    unsigned short int 		msg_len;        		// 消息总长度
}MsgHeadStr;

#pragma pack()

/*************************************************************
**	设备状态消息体
************************************************************/

#pragma pack(1)
typedef struct 
{
	unsigned short int 	 	msg_device_type;                    //消息体类型码
	unsigned short int 		msg_device_len;                     //消息体长度(数据区域长度)
	
	unsigned char      		msg_device_manu_num;                 //
	unsigned char 	   		msg_device_manu_type;                //
	unsigned char      		msg_device_user_num;                 //
	unsigned char      		msg_device_car_num;                  //车辆类型
	unsigned char      		msg_device_protocol_ver;             //通讯协议版本
	unsigned char      		msg_device_firware_ver;              //固件版本          
	unsigned char      		msg_device_hardware_ver;             //已经版本号
}MsgDeviceStr;
#pragma pack()


/*************************************************
**	
*************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int 		msg_input_type;						//
	unsigned short int 		msg_input_len;						//
	unsigned char      		msg_input_io;						//
	unsigned char      		msgAccState;						//
	unsigned char	   		msgMotoState;						//
	unsigned short int 		msgFrq1;							//
	unsigned short int 		msgFrq2;							//
	unsigned short int 		msgPWM1;							//
	unsigned short int 		msgPWM2;							//
	unsigned short int 		msgInputVol1;						//
	unsigned short int 		msgInputVol2;						//
	unsigned short int 		msgPowVol;							//
	unsigned short int 		msgBatteryVol;						//
	unsigned int       		msgWarningState;					//
	unsigned char      		msgLine;							//
} MsgInputStr;

#pragma pack()


//
#pragma pack(1)

typedef struct 
{
	unsigned short int 		msg_gps_type;                  	//
	unsigned short int 		msg_gps_len;                   	//
	unsigned int       		msg_gps_latitude;        		//
	unsigned int       		msg_gps_longitude;             	//
	unsigned short int 		msg_gps_speed;           		//
	unsigned short int 		msg_gps_azimuth;         		//
	unsigned short int 		msg_gps_altitude;        		//
	unsigned char      		msg_gps_year;                  	//
	unsigned char      		msg_gps_mon;                   	//
	unsigned char      		msg_gps_day;					//
	unsigned char      		msg_gps_hour;                 	//
	unsigned char      		msg_gps_min;                   	//
	unsigned char      		msg_gps_sec;                   	//
	unsigned char      		msg_gps_satellite_num;         	//
	unsigned char      		msg_gps_view_num;              	//
	unsigned short int 		msg_gps_ghdop_v;             	//
	unsigned char      		msg_gps_status;              	//
	unsigned char      		msg_gps_line;
}MsgGnssStr;

#pragma pack()



//

#pragma pack(1)
typedef struct
{
	unsigned short int 		MsgFarmType;                   	//
	unsigned short int 		MsgFarmLen;                     // 

	unsigned short int 		MsgFarmBig;						//
	unsigned char 	   		MsgTimeYear;					//
	unsigned char 	   		MsgTimeMon;						//
	unsigned char 	   		MsgTimeDay;						//
	unsigned char 	   		MsgTimeHour;					//
	unsigned char 	   		MsgTimeMin;						//
	unsigned char 	   		MsgTimeSec;						//
	unsigned char      		MsgLine;						//
}MsgFarmStr;

#pragma pack()


#pragma pack(1)

/**************************************************************
**	CAN数据结构体(小麦机)
***************************************************************/
typedef struct
{
	unsigned short int 	 	MsgCanWheatType;                  //消息体类型码
	unsigned short int 	 	MsgCanWheatLen;                   //消息体数据长度

	unsigned short int   	MsgEngineRotate;      			  //发动机转速
	unsigned char        	MsgWaterTemp;    				  //冷却水温度
	unsigned char        	MsgEngineOil;    		          //机油压力
	unsigned char        	MsgFuelPercent;  		          //燃料百分比
	unsigned int         	MsgEngineWorkTime;               //发动机工作时间  
	unsigned short       	MsgSysVal;                       //系统电压
	unsigned int         	MsgTotalTravlled;                //总行驶里程
	unsigned short int   	MsgTravlledSpeed;                //行驶速度 
	unsigned short int   	MsgRotate1;        				 //滚筒转速 
	unsigned short int   	MsgRotate2;        				 //复脱器转速
	unsigned short int   	MsgRotate3;        				 //籽粒升运器
	unsigned short int   	MsgRotate4;        				 //清选风扇转速
	unsigned short int   	MsgRotate5;        				 //喂入搅龙转速
	unsigned short int   	MsgRotate6;        				 //过桥转速
	unsigned short int   	MsgLossRatio;         			 //清选损失率
	unsigned char        	MsgCabTemp;           			 //驾驶室温度
	unsigned char		 	MsgReapHigh;          			 //割茬高度
	unsigned char        	MsgAperture1;         			 //上筛开度
	unsigned char        	MsgAperture2;         			 //下筛开度
	unsigned char        	MsgAperture3;         			 //尾筛开度
	unsigned char        	MsgWarnValue;         			 //粮满报警 
	unsigned char 		 	MsgLine;
}MsgCanWheatStr;	
#pragma pack()


#pragma pack(1)

/**************************************************************
**	CAN数据结构体(小麦机)
***************************************************************/
typedef struct
{
	unsigned short int 	 	MsgCanWheatType;                  //
	unsigned short int 	 	MsgCanWheatLen;                   //

	unsigned short int   	MsgEngineRotate;      			  //发动机转速
	unsigned char        	MsgWaterTemp;    				  //冷却水温度
	unsigned char        	MsgEngineOil;    		          //机油压力
	unsigned char        	MsgFuelPercent;  		          //燃料百分比  
	unsigned char 			MsgLine;                          //
}MsgCanWheatOldStr;
#pragma pack()


#pragma pack(1)
typedef struct
{
	unsigned short int 		MsgIntelligentType;
	unsigned short int 		MsgIntelligentLen;

	unsigned char			MsgVECUMan;							//VECU厂家
	unsigned char			MsgVECUType;						//VECU型号
	unsigned short int		MsgCANProVer;						//软件版本
	unsigned char			MsgEngineMan;						//发动机厂家
	unsigned char			MsgEngineType;						//发动机类型
	unsigned char			MsgEECUMan;							//EECU厂家
	unsigned char			MsgEECUType;						//EECU类型
	unsigned char			MsgDeviceType;						//设备种类
	unsigned char			MsgResourceCfg;						//机型资源配置
	unsigned int			MsgEngineWorkTime;					//发动机工作小时 
	unsigned int			MsgTotalTravlled;					//总里程
	unsigned short int		MsgFuelNum;							//燃油量 
	unsigned short int		MsgSysVol;							//系统电压 
	unsigned short int		MsgTravlledSpeed;					//行驶速度 
	unsigned char			MsgFuelPercent;						//燃油百分比
	unsigned char			MsgWarnValue1;						//粮满报警
	unsigned char			MsgClutchStatus;					//主离合状态
	unsigned char			MsgCutTableState;					//割台状态
	unsigned short int		MsgStripRotate;						//剥皮机转速 
	unsigned short int		MsgLiftRotate;						//升运器转速 
	unsigned short int		MsgRotate1;							//轴流(切流)滚筒转速 
	unsigned short int		MsgRotate3;							//复脱器转速 
	unsigned char			MsgRotateWarn1;						//轴流滚筒转速报警状态位
	unsigned char			MsgRotateWarn2;						//升运器转速报警状态位
	unsigned char			MsgRotateWarn3;						//复脱器转速报警状态位
	unsigned char			MsgRotateWarn4;						//喂入搅龙转速报警状态位
	unsigned char			MsgRotateWarn5;						//过桥转速报警状态位
	unsigned char			MsgRotateWarn6;						//清选风扇转速报警状态位
	unsigned char			MsgRotateWarn7;						//切碎器转速报警状态位
	unsigned char			MsgRotateWarn8;						//剥皮机转速报警状态位
	unsigned short int		MsgRotate4;							//清选风扇转速 
	unsigned short int		MsgTorque1;							//轴流(切流)滚筒转矩 
	unsigned short int		MsgRotate5;							//喂入搅龙转速 
	unsigned short int		MsgRotate6;							//过桥转速 
	unsigned short int		MsgRotate7;							//拨禾轮转速 
	unsigned short int		MsgDisplaceQuantity;				//拨禾轮前后位移量
	unsigned short int		MsgReelHigh;						//拨禾轮高度
	unsigned short int		MsgRotate8;							//切碎器转速 
	unsigned short int		MsgLossRatio;						//清选损失率（总损）
	unsigned short int		MsgCutTableHigh;					//割台高度
	unsigned short int		MsgReapHigh;						//割茬高度
	unsigned char			MsgAperture1;						//上筛开度
	unsigned char			MsgAperture2;						//下筛开度
	unsigned char			MsgAperture3;						//尾筛开度
	unsigned short int		MsgMoistureRate;					//含水率
	unsigned char			MsgBrokenRate;						//破碎率
	unsigned char			MsgCutterKnifeFre;					//割刀频率
	unsigned char			MsgSunkGap;							//凹板间隙(检测值)
	unsigned char			MsgDynamoStatus;					//充电指示
	unsigned char			MsgPlaceLight;						//位置灯
	unsigned char			MsgLight6;							//右转向
	unsigned char			MsgLight5;							//左转向
	unsigned char			MsgDistanceLight;					//远光
	unsigned char			MsgLight8;							//近光
	unsigned char			MsgAirFilterWarn;					//空滤堵塞报警
	unsigned char			MsgLight9;							//预热
	unsigned char			MsgGearN;							//空挡N
	unsigned char			MsgHandBrake;						//手刹
	unsigned char			MsgGearR;							//倒挡R
	unsigned char			MsgGearF;							//前进挡F
	unsigned char			MsgStartupState;					//起动状态
	unsigned char			MsgOilWater;						//油水分离
	unsigned char			MsgEngineFault;						//发动机故障
	unsigned char			MsgUnloadSafeSwitch;				//卸粮安全开关
	unsigned char			MsgUnfoldLimitSwitch;				//卸粮筒展开限位开关
	unsigned char			MsgRegainLimitSwitch;				//卸粮筒收回限位开关
	unsigned char			MsgButton1;							//风机增速按钮
	unsigned char			MsgButton2;							//风机减速按钮
	unsigned char			MsgButton3;							//拨禾轮提升按钮
	unsigned char			MsgButton4;							//拨禾轮下降按钮
	unsigned char			MsgButton5;							//轴流滚筒提速按钮
	unsigned char			MsgButton6;							//轴流滚筒降速按钮
	unsigned char			MsgButton7;							//主离合啮合按钮
	unsigned char			MsgButton8;							//主离合分离按钮
	unsigned char			MsgButton9;							//过桥离合啮合按钮
	unsigned char			MsgButton10;						//过桥离合分离按钮
	unsigned char			MsgButton11;						//卸粮筒展开按钮
	unsigned char			MsgButton12;						//卸粮筒收回按钮
	unsigned char			MsgButton13;						//割台上升按钮
	unsigned char			MsgButton14;						//割台下降按钮
	unsigned char			MsgECUComState;						//ECU通信状态
	unsigned char			MsgRunMode1;						//割台高度自动调节模式运行状态
	unsigned char			MsgRunMode2;						//负荷自动调节模式运行状态
	unsigned char			MsgDrivingLamp;						//日间行车灯驱动
	unsigned char			MsgRadiotube1;						//割台提升电磁阀
	unsigned char			MsgRadiotube2;						//割台下降电磁阀
	unsigned char			MsgRadiotube3;						//拨禾轮提升电磁阀
	unsigned char			MsgRadiotube4;						//拨禾轮下降电磁阀
	unsigned char			MsgRadiotube5;						//轴流滚筒速度提升电磁阀
	unsigned char			MsgRadiotube6;						//轴流滚筒速度下降电磁阀
	unsigned char			MsgRadiotube7;						//主离合啮合电磁阀
	unsigned char			MsgRadiotube8;						//主离合分离电磁阀
	unsigned char			MsgRadiotube9;						//过桥离合啮合电磁阀
	unsigned char			MsgRadiotube10;						//过桥离合分离电磁阀
	unsigned char			MsgRadiotube11;						//卸粮筒展开电磁阀
	unsigned char			MsgRadiotube12;						//卸粮筒收回电磁阀
	unsigned char			MsgRetrogressRelay;					//倒车继电器
	unsigned char			MsgBackupRelay;						//备用驱动输出
	unsigned char			MsgRadiotube13;						//风机调速电磁阀
	unsigned short int		MsgHeaderLeftDistance;				//割台左侧雷达测距
	unsigned short int		MsgHeaderRightDistance;				//割台右侧雷达测距
	unsigned char			MsgFeedQuantity;					//喂入量（检测值）
	unsigned char			MsgLeftLossRatio;					//清选左传感器损失率
	unsigned char			MsgMidLossRatio;					//清选中传感器损失率
	unsigned char			MsgRightLossRatio;					//清选右传感器损失率
	unsigned int			MsgOdometerMeter;					//米计里程
	unsigned short int		MsgOtherRatio;						//清选含杂率
	unsigned char			MsgHeaderLossRatio;					//割台损失率（检测值）
	unsigned char			MsgEntrainmentLossRatio;			//夹带损失率（检测值）
	unsigned short int		MsgFoodStirTorque;					//喂入搅龙扭矩
	unsigned short int		MsgDraughtFanAngle;					//风机分风板角度
	unsigned short int		MsgFrontMotorRatio;					//前马达转速
	unsigned short int		MsgBackMotorRatio;					//后马达转速
	unsigned short int		MsgDripAngle;						//抛撒器角度
	unsigned char			MsgCabTemp;							//驾驶室温度
	unsigned char			MsgHydraulicOil;					//液压油温度
	unsigned short int		MsgFoodWeightDry;					//粮食干重 
	unsigned short int		MsgFoodWeightWet;					//粮食湿重 
	unsigned short int		MsgAreaYield;						//亩产
	unsigned short int		MsgCerealFlowCheck;					//谷物流量检测质量
	unsigned short int		MsgAreaExpend;						//亩产耗油量
	unsigned char			MsgStateBit1;						//清选损失率状态位
	unsigned char			MsgStateBit2;						//割台仿形开关状态位
	unsigned char			MsgIdlingFlag;						//怠速标志位
	unsigned char			MsgLoadPowerSwitch;					//负荷功率开关
	unsigned short int		MsgFeedbackValue1;					//千粒重设置反馈值
	unsigned short int		MsgFeedbackValue2;					//仿形高度设置反馈值
	unsigned short int		MsgFeedbackValue3;					//轴流滚筒堵塞转速低报警反馈值
	unsigned short int		MsgFeedbackValue4;					//割台限位反馈：上限高度
	unsigned short int		MsgFeedbackValue5;					//割台限位反馈：下限高度
	unsigned char			MsgFeedbackValue6;					//当前作物种类设置反馈值
	unsigned char			MsgFeedbackValue7;					//切碎器工作模式设置反馈值
	unsigned char			MsgFeedbackValue8;					//割台损失率设置反馈值
	unsigned char			MsgFeedbackValue9;					//谷物成熟度设置反馈值
	unsigned char			MsgResetKey1;						//单次行驶里程清零（仪表按键）
	unsigned char			MsgResetKey2;						//米计清零（仪表按键）
	unsigned char			MsgResetKey3;						//割台高度一键复位（仪表按键）
	unsigned char			MsgResetKey4;						//卸粮筒控制（仪表按键）
	unsigned char			MsgResetKey5;						//负荷自动调节控制（仪表按键制）
	unsigned char			MsgResetKey6;						//粮食重量清零（仪表按键）
	unsigned short int		MsgSetValue1;						//千粒重设置值
	unsigned short int		MsgSetValue2;						//仿形高度设置值
	unsigned short int		MsgSetValue3;						//轴流滚筒堵塞转速低报警设置值
	unsigned short int		MsgSetValue4;						//割台限位设置：上限高度
	unsigned short int		MsgSetValue5;						//割台限位设置：下限高度
	unsigned char			MsgSetValue6;						//当前作物种类设置
	unsigned char			MsgSetValue7;						//切碎器工作模式
	unsigned char			MsgSetValue8;						//割台损失率设置值
	unsigned char			MsgSetValue9;						//谷物成熟度设置值
	unsigned short int		MsgEngineRotate;					//发动机转速
	unsigned char			MsgWaterTemp;						//冷却水温
	unsigned char			MsgEngineOil;						//机油压力
	unsigned char 			MsgLine;							//消息体序列号

}MsgIntelligent;          //智能小麦机消息体
#pragma pack()

/************************************************************
**	Can数据结构体(水稻机)
************************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int 	 	MsgCanPaddyType;                  	//
	unsigned short int 	 	MsgCanPaddyLen;                   	//

	unsigned short int   	MsgEngineRotate;      				//发动机转速
	unsigned char        	MsgWaterTemp;    					//冷却水温度
	unsigned short int   	MsgRotate1;        					//
	unsigned short int   	MsgRotate2;        					//滚动轮转速
	unsigned short int   	MsgSysVol;            				//系统电压
	unsigned int 		 	MsgEngineWorkTime;    				//发动机工作时间 
	unsigned char        	MsgWarnValue;         				//粮满报警
	unsigned short int   	MsgTravlledSpeed;     				//行驶速度
	unsigned int         	MsgTotalTravlled;     				//总行驶里程  
	
	unsigned char        	MsgEngineOil;    					//机油压力  (新加)
	unsigned char        	MsgFuelPercent;  					//燃料百分比
	unsigned char 			MsgLine1;

}MsgCanPaddyStr;
#pragma pack()


/**************************************************************
**	Can数据结构(玉米机)
***************************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int 	 MsgCanMaizeType;                  //
	unsigned short int 	 MsgCanMaizeLen;                   //

	unsigned short int   MsgEngineRotate;      				//发动机转速
	unsigned char        MsgWaterTemp;    					//冷却水温度
	unsigned char        MsgEngineOil;    					//机油压力
	unsigned char        MsgFuelPercent;  					//燃料百分比
	unsigned int 		 MsgEngineWorkTime;    				//发动机工作时间 
	unsigned short       MsgSysVol;            				//系统电压
	unsigned int         MsgTotalTravlled;     				//总行驶里程
	unsigned short int   MsgRotate1;        				//
	unsigned short int   MsgRotate2;        				//滚动轮转速
	unsigned short int   MsgTravlledSpeed;     				//行驶速度
	unsigned short int   MsgReapHigh;          				//割茬高度
	unsigned char        MsgWarnValue;         				//粮满报警
	unsigned char 		 MsgLine;							//
	
}MsgCanMaizeStr;
#pragma pack()


/**************************************************
**	Can数据结构(拖拉机)
***************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int 	 MsgCanTractorType;                 //消息体类型码  
	unsigned short int 	 MsgCanTractorLen;                  //消息体长度

	unsigned short int	 MsgEngineRotate;					//发动机转速
	unsigned char		 MsgWaterTemp; 						//冷却水温度
	unsigned char		 MsgEngineOil; 						//机油压力
	unsigned char		 MsgFuelPercent;					//燃料百分比
	unsigned int		 MsgEngineWorkTime;					//发动机工作时间 
	unsigned short		 MsgSysVol;							//系统电压
	unsigned short int	 MsgTravlledSpeed; 					//行驶速度
	unsigned int		 MsgTotalTravlled; 					//总行驶里程
	unsigned char		 MsgWarnValue; 						//粮满报警 
	unsigned char 		 MsgLine;
}MsgCanTractorStr;

#pragma pack()



/**************************************************
**	Can数据结构(P平台拖拉机)
***************************************************/

#pragma pack(1)

typedef struct
{
	unsigned short int 	 MsgCanPtractorType;			//
	unsigned short int   MsgCanPtractorLen;				//
	
	unsigned char 		 MsgPTOStatus;					//PTO状态
	unsigned short int	 MsgEngineRotate;				//发动机转速    
	unsigned char		 MsgWaterTemp; 					//发动机水温，
	unsigned char 		 MsgSpeedLock;					//差速锁状态（开/关）
	unsigned char        MsgRangeStatus;        		//四驱状态
	unsigned char        MsgBrakeFluidLight;    		//制动液灯
	unsigned char		 MsgDistanceLight;      		//远光灯状态
	unsigned char    	 MsgGeneralLight;		 		//普通灯光状态
	unsigned char 		 MsgStopLight;                  //驻车灯
	unsigned char		 MsgLight1;			 			//前工作灯
	unsigned char		 MsgLight2;             		//前测灯
	unsigned char		 MsgLight3;			 			//后工作灯
	unsigned char		 MsgLight4;			 			//后单点工作灯
	unsigned char		 MsgLight5;             		//左转向灯
	unsigned char		 MsgLight6;             		//右转向灯
	unsigned char		 MsgLight7;            	 		//顶灯
	unsigned char 		 MsgPowerShiftLow;      		//动力转换低速
	unsigned char 		 MsgPowerShiftMid;		 		//动力转换中速
	unsigned char 	     MsgPowerShiftHight;    		//动力转换高速
	unsigned char 		 MsgAirBreakLow;		 		//气压制动低状态
	unsigned char        MsgFuelPositionLow;   			//燃油位置低报警
	unsigned char 		 MsgShuttleEmpty;       		//梭式换挡空挡
	unsigned char 		 MsgShuttleRetreat;     		//梭式换挡后退档
	unsigned char 		 MsgShuttleAdvance;     		//梭式换挡前进挡
	unsigned char 		 MsgCrawlStatus;        		//爬行当撞他
	unsigned int		 MsgTravlledDistance; 	 		//总行驶里程
	unsigned int		 MsgTravlledTotalDistance; 		//总行驶里程 
	unsigned char	     MsgLight9;						//预热塞灯
	unsigned int		 MsgEngineWorkTime;				//发动机工作时间 
	unsigned int 	     MsgCarWorkTime;       			//车辆工作时间
	unsigned int 	     MsgPTOWorkTime;       			//PTO工作时间
	unsigned char    	 MsgEngineFuelTemp;    			//发动机燃料温度    
	unsigned short int	 MsgEngineOilTemp;     			//发动机机油温度   
	unsigned char   	 MsgEngineOilPressure; 			//发动机机油压力
	unsigned char 	     MsgLight10;           			//发动机水温高预警灯
	unsigned char        MsgLight11;					//发动机机油压力低预警灯
	unsigned short int   MsgPTOSped;           			//PTO转速
	unsigned short int   MsgPTOSpeedSet;                //PTO转速设定
	unsigned char		 MsgHandBrake;         			//手制动状态
	unsigned char        MsgClutchStatus;      			//离合器踏板状态
	unsigned short int	 MsgTravlledSpeed;     			//行驶速度
	unsigned char        MsgEngineAirfiltrate; 			//发动起空滤状态
	unsigned short int   MsgBatteryVol;        			//电池电压
	unsigned char        MsgGearBoxOilLow;     			//变速箱油压低报警
	unsigned char        MsgGearBoxOilHight;   			//变速箱油压高报警
	unsigned char 		 MsgFuelPosition;      			//燃料位置
	unsigned char		 MsgDynamoStatus;      			//发电机充电状态   
	
	unsigned char 	 	 MsgLine;						//消息
	
}MsgCanPTractorStr;         //

#pragma pack()



/******************************************
**
**
*******************************************/
#pragma pack(1)
typedef struct
{
	unsigned short int 	MsgOilType;
	unsigned short int 	MsgOilLen;

	unsigned int 		MsgOil;      				//单次油耗
	unsigned int 	   	MsgOilTotal;  				//累计油耗
	unsigned char 		MsgLine;    				//
}MsgOilStr;
#pragma pack()


//P平台当前故障码故障码
#pragma pack(1)
typedef struct
{
	unsigned short int MsgFaultCodeType;         	//故障码数量 
	unsigned short int MsgFaultCodeLen;				//
	unsigned char MsgFaultCode[10][8];				//
}MsgCurrentFaultCodeStr;             	 		//当前故障码
#pragma pack()


//P平台历史故障码
#pragma pack(1)
typedef struct
{
	unsigned short int MsgFaultCodeType;
	unsigned short int MsgFaultCodeLen;
	
	unsigned char MsgFaultCode[10][8];
}MsgHistoryFaultCodeStr;                   //历史故障码
#pragma pack()



/***************************************************
**	STM32数据块
****************************************************/
typedef struct
{
	unsigned short int TypeCode;					//
	unsigned short int DataLen;						//
	void *table;
}TerDataBlockStr;

/****************************************************
**	读STM32控制快
*****************************************************/
typedef struct
{	
	unsigned char 	flag;						  //
	void 			*block;						  //
}TerDataTcbStr;



/***************************************************
**	单片机所采集到状态数据结构
**	
***************************************************/
#pragma pack(1)

typedef struct
{
	unsigned char 			AccState;		//ACC状态
	unsigned char 			CanConnect;		//CAN连接状态
	unsigned short int 		PowVol;       //外部供电电压温度
	unsigned short int 		Battery;     //电池电压温度
	unsigned short int 		McuTemp;        //单片机温度
	unsigned int 			McuTime;		//单片机时间
	unsigned short int 		McuFirNum;     //单片机版本号
}TerStateStr;
#pragma pack()

/**************************************************
**	SD卡相关信息
**	
**************************************************/
typedef struct
{
	unsigned char 			SDCardCheck;                //SD卡是否插入
	unsigned char 			SDCardType;           		//SD卡类型
	unsigned short int 		SDCardTotalSpace;    		//SD卡总容量 
	unsigned short int 		SDCardSurplusSpace;         //SD卡剩余容量
	unsigned char 			SDCardState;                //SD卡状态
	unsigned char 			NC;                         //预留一个字节
}TerSDCardStr;


/**************************************************
**	系统运行，相关信息
**	
***************************************************/
#pragma pack(1)
typedef struct
{

	unsigned int 			ACCTotal;                    			//ACC总时间  
	double		 			ACCTotalHour;                			//小时
	double 					TotalDistance;               			//行驶总里程  保留两位有效小数
	unsigned int 			TimeSec;                     			//系统时间 
	double 					latitude;								//维度
	double 					longitude;	  							//经度
	unsigned short int 		altitude;       						//海拔高度
	unsigned short int 		azimuth;	  							//航向角度
	float 					speed;		            				//速度
	float  					ghdop_v;      							//定位因子
	unsigned char 			satellite_num;         	 				//使用卫星数量
	unsigned char 			satellite_view_num; 					//可视卫星数量
	unsigned char 			status;                 				//定位状态  
	unsigned char			TYear;                                  //GNSS-时间-年
	unsigned char 			TMon;									//GNSS-时间-月
	unsigned char 			TDay;									//GNSS-时间-日
	unsigned char 			THour;									//GNSS-时间-时
	unsigned char			TMin;									//GNSS-时间-分
	unsigned char 			TSec;                                   //GNSS-时间-秒                           
	unsigned int            gnss_time;              				//定位时间
	unsigned char 			latitude_ns;            				//维度标识
	unsigned char 			longitude_ew;           				//精度标识
}SysRunDataStr;
#pragma pack()



//两字节数据类型
typedef union  
{
	unsigned short int		IntII;					//
	unsigned char			TTbyte[2];				//
}IntToChar;


//四字节数据类型
typedef union  
{
	unsigned int			LongLL;					//
	unsigned char			TTbyte[4];				//
}LongToChar;


/**************************************
**	固件程序升级
***************************************/
#pragma pack(1)
typedef struct 
{
	unsigned char			UpgradeStatus;		   //0:
	unsigned char 			UpgradeFlag;           //升级标识
 }UpgradeStatusStr;
#pragma pack()

/**************************************
**	固件升级文件
**************************************/
#pragma pack(1)
typedef struct
{
	unsigned int 			FileTotalLen;           //文件总长度，以字节为单位。
	unsigned short int 		FileVerfy;				//文件校验
	unsigned short int 		FileCurrentFram;	    //
	unsigned short int 		FileTotalFram;   		//
	unsigned short int 		FileLen[100];           //
	unsigned char 			FileBuf[100][1024];   	//文件缓冲区
}UpgradeFileStr;
#pragma pack()




/**************************************
**	说明:
**  通过串口升级ST86程序。
**	把收到的数据包
**************************************/
#pragma pack(1)
typedef struct
{
	unsigned int 			FileLen;           			//升级文件总长度
	unsigned short int 		FileVerfy;					//文件校验值，采用校验和
	unsigned int 			CurrentLen;					//当前长度
	unsigned int 			CurrentFramNum;   			//当前数据包   
}UpgradeST86FileStr;
#pragma pack()



#endif




/*******************************File End******************************/





