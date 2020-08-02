
/***********************************************
**	FileName:
**	Time:
************************************************/

#ifndef DATATYPE_H
#define DATATYPE_H


#define BLIND_NUM  250



/***************************************************************
***定位数据结构
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
	unsigned int            gnss_time;              				//定位时间
	unsigned char 			latitude_ns;            				//维度标识
	unsigned char 			longitude_ew;           				//精度标识
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
	double 					Distance;
}DistanceProStr;

/****************************************************/
//TCPIP连接信息
typedef struct 
{
	unsigned char 			GprsApn[20];           					// 接入方式
	unsigned char 			GprsUser[20];          					//接入用户名
	unsigned char 			GprsPassd[20];  	   					//接入密码
	unsigned char 			GprsNetActive;		   					//GPRS网络状态
	BOOL          			SimState;         	   					//是否有Sim卡
	unsigned char 			SimIccId[25];     	   					//SIM卡ICC_ID
	unsigned char			GsmSign;       	 						//GSM信号值			
	unsigned char 			SocketId; 	   							//Socket编号 
	unsigned char 			SocketType;     	 					//Socket
	unsigned char 			ServerAddr[50];        					//平台地址  服务器地址
	unsigned short int 		ServerPort;       						//端口    服务器端口  
	unsigned char 			UlrToIPFlag;	                        //地址标识，0:IP地址，1:URL	
	unsigned char 			LocalAddr[20];         					//本地IP地址
	unsigned short 			LocalPort;            					//本地端口
	unsigned char 			LinkState;             					//连接状态
	unsigned char 			FtpLinkState;							//FTP链接状态
	unsigned int  			LinkNum;              					//连接次数
	unsigned char 			LinkEnable;             				//是否允许连接
	unsigned char 			SendState;              				//发送状态 
	int			 			SimStatus;              				//Sim卡状态
	int						NetWorkState;           				//移动网络状态
	unsigned char			NetGprsState;           				//GPRS网络状态  
	unsigned char 			ErrorCode;              				//误码率 ，暂时没有使用 
	unsigned char 			ResetLink;              				//重新连接
	unsigned char			LinkType;								//(1)FPT 或者(0)TCP/IP链接  
	unsigned char			FtpAddr[50];           					//Ftp服务器地址
	unsigned char 			FtpUserName[20];       					//用户名
	unsigned char 			FtpUserPassd[20];      					//密码
	unsigned char 			FtpFileName[20];       					//文件名称  
	unsigned short int 		FtpPort;              						//端口
	unsigned short int 		FtpFileLen;            					//文件长度
	unsigned char 			*FtpFileSapce;          				//FTP下载的文件指针
	unsigned char 			FtpLoadFig;          					//下载标识
	
}TcpIPSocketStr;


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
	unsigned char 	Year;              								//系统时间-年
	unsigned char 	Mon;               								//系统时间-月
	unsigned char 	MDay;              								//系统时间-日
	unsigned char 	WDay;              								//系统时间-周
	unsigned char 	Hour;              								//系统时间-时
	unsigned char 	Min;               								//系统时间-分
	unsigned char 	Sec;               								//系统时间-秒
	unsigned int 	TimeSec;           								//系统时间-总秒，以2000年1月1号0时0分0秒计算
	unsigned char   VerifyBit;         								//时间校准  标识时间是否已经校准
}SysTimeStr;


/***********************************************************
**	数据上传结构
***********************************************************/
#pragma pack(1)

typedef struct
{
	unsigned char 		DataBuf[500];     							//发送缓冲区
	unsigned short int 	DataLen;          							//数据长度
	unsigned char 		DataCrc;          							//校验
	unsigned char       DataBig;          							//数据包标识(0:正常数据；1:盲区存储数据)
	
}FiFoStr;
#pragma pack()

/*************************************************************
**	发送队列
*************************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int QNum;             							//发送队列
	unsigned short int QWrite;           							//发送队列对头
	unsigned short int QRead;            							//发送队列对尾
	FiFoStr            QData[BLIND_NUM];       						//发送队列数据缓冲区
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
	unsigned char 		 WorkFlag;
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
	unsigned short       SysVol;            //系统电压
	unsigned int 		 EngineWorkTime;    //发动机工作时间 
	unsigned char        WarnValue;         //粮满报警
	unsigned short int   TravlledSpeed;     //行驶速度
	unsigned int         TotalTravlled;     //总行驶里程
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
**	Can数据结构(拖拉机)
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



//配置用户协议号
#pragma pack(1)
typedef struct
{
	unsigned short int   CanProtocolSerial;  //车型协议编号
	unsigned char        LockCardState;      //锁车状态
}TerminalCanProStr;
#pragma pack()



#pragma pack(1)


//通讯协议消息头
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
	unsigned short int msg_input_type;
	unsigned short int msg_input_len;
	unsigned char      msg_input_io;
	unsigned char      msgAccState;
	unsigned char	   msgMotoState;
	unsigned short int msgFrq1;
	unsigned short int msgFrq2;
	unsigned short int msgPWM1;
	unsigned short int msgPWM2;
	unsigned short int msgInputVol1;
	unsigned short int msgInputVol2;
	unsigned short int msgPowVol;
	unsigned short int msgBatteryVol;
	unsigned int       msgACCTotalTime;
	unsigned char      msgLine;
} MsgInputStr;

#pragma pack()


//
#pragma pack(1)

typedef struct 
{
	unsigned short int msg_gps_type;                  	//
	unsigned short int msg_gps_len;                   	//
	unsigned int       msg_gps_latitude;        		//
	unsigned int       msg_gps_longitude;             	//???è
	unsigned short int msg_gps_speed;           		//?ù?è
	unsigned short int msg_gps_azimuth;         		//??DD·??ò
	unsigned short int msg_gps_altitude;        		//o??ò???è
	unsigned char      msg_gps_year;                  	//GPSê±??-?ê
	unsigned char      msg_gps_mon;                   	//GPSê±???a?a??
	unsigned char      msg_gps_day;									  //
	unsigned char      msg_gps_hour;                 	//
	unsigned char      msg_gps_min;                   	//
	unsigned char      msg_gps_sec;                   	//
	unsigned char      msg_gps_satellite_num;         	//
	unsigned char      msg_gps_view_num;              	//
	unsigned short int msg_gps_ghdop_v;             	//?????¨??òò×ó
	unsigned char      msg_gps_status;              	//GPS?¨??×′ì?
	unsigned char      msg_gps_line;
}MsgGnssStr;

#pragma pack()



//

#pragma pack(1)


typedef struct
{
	unsigned short int MsgFarmType;                   //
	unsigned short int MsgFarmLen;                      // 

	unsigned short int MsgFarmBig;
	unsigned char 	   MsgTimeYear;
	unsigned char 	   MsgTimeMon;
	unsigned char 	   MsgTimeDay;
	unsigned char 	   MsgTimeHour;
	unsigned char 	   MsgTimeMin;
	unsigned char 	   MsgTimeSec;
	unsigned char      MsgLine;
}MsgFarmStr;

#pragma pack()


#pragma pack(1)

/**************************************************************
**	CAN数据结构体(小麦机)
***************************************************************/
typedef struct
{
	unsigned short int 	 MsgCanWheatType;                  //
	unsigned short int 	 MsgCanWheatLen;                   //

	unsigned short int   MsgEngineRotate;      			  //发动机转速
	unsigned char        MsgWaterTemp;    				  //冷却水温度
	unsigned char        MsgEngineOil;    		          //机油压力
	unsigned char        MsgFuelPercent;  		          //燃料百分比
	unsigned int         MsgEngineWorkTime;               //发动机工作时间  
	unsigned short       MsgSysVal;                       //系统电压
	unsigned int         MsgTotalTravlled;                //总行驶里程
	unsigned short int   MsgTravlledSpeed;                //行驶速度 
	unsigned short int   MsgRotate1;        	//滚筒转速 
	unsigned short int   MsgRotate2;        	//复脱器转速
	unsigned short int   MsgRotate3;        	//籽粒升运器
	unsigned short int   MsgRotate4;        	//清选风扇转速
	unsigned short int   MsgRotate5;        	//喂入搅龙转速
	unsigned short int   MsgRotate6;        	//过桥转速
	unsigned short int   MsgLossRatio;         //清选损失率
	unsigned char        MsgCabTemp;           //驾驶室温度
	unsigned char		 MsgReapHigh;          //割茬高度
	unsigned char        MsgAperture1;         //上筛开度
	unsigned char        MsgAperture2;         //下筛开度
	unsigned char        MsgAperture3;         //尾筛开度
	unsigned char        MsgWarnValue;         //粮满报警 
	unsigned char 		 MsgLine;
}MsgCanWheatStr;	
#pragma pack()

/************************************************************
**	Can数据结构体(水稻机)
************************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int 	 MsgCanPaddyType;                  //
	unsigned short int 	 MsgCanPaddyLen;                   //

	unsigned short int   MsgEngineRotate;      //发动机转速
	unsigned char        MsgWaterTemp;    		//冷却水温度
//	unsigned char        MsgEngineOil;    		//机油压力
	unsigned short int   MsgRotate1;        	//
	unsigned short int   MsgRotate2;        	//滚动轮转速
	unsigned short int   MsgSysVol;            //系统电压
	unsigned int 		 MsgEngineWorkTime;    //发动机工作时间 
	unsigned char        MsgWarnValue;         //粮满报警
	unsigned short int   MsgTravlledSpeed;     //行驶速度
	unsigned int         MsgTotalTravlled;     //总行驶里程
	unsigned char 		 MsgLine;
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

	unsigned short int   MsgEngineRotate;      //发动机转速
	unsigned char        MsgWaterTemp;    		//冷却水温度
	unsigned char        MsgEngineOil;    		//机油压力
	unsigned char        MsgFuelPercent;  		//燃料百分比
	unsigned int 		 MsgEngineWorkTime;    //发动机工作时间 
	unsigned short       MsgSysVol;            //系统电压
	unsigned int         MsgTotalTravlled;     //总行驶里程
	unsigned short int   MsgRotate1;        	//
	unsigned short int   MsgRotate2;        	//滚动轮转速
	unsigned short int   MsgTravlledSpeed;     //行驶速度
	unsigned short int   MsgReapHigh;          //割茬高度
	unsigned char        MsgWarnValue;         //粮满报警
	unsigned char 		 MsgLine;
	
}MsgCanMaizeStr;
#pragma pack()


/**************************************************
**	Can数据结构(拖拉机)
***************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int 	 MsgCanTractorType;                  //消息体类型码  
	unsigned short int 	 MsgCanTractorLen;                   //消息体长度

	unsigned short int	 MsgEngineRotate;		//发动机转速
	unsigned char		 MsgWaterTemp; 		//冷却水温度
	unsigned char		 MsgEngineOil; 		//机油压力
	unsigned char		 MsgFuelPercent;		//燃料百分比
	unsigned int		 MsgEngineWorkTime;	//发动机工作时间 
	unsigned short		 MsgSysVol;			//系统电压
	unsigned short int	 MsgTravlledSpeed; 	//行驶速度
	unsigned int		 MsgTotalTravlled; 	//总行驶里程
	unsigned char		 MsgWarnValue; 		//粮满报警 
	unsigned char 		 MsgLine;
}MsgCanTractorStr;

#pragma pack()





/***************************************************
**	STM32数据块
****************************************************/
typedef struct
{
	unsigned short int TypeCode;
	unsigned short int DataLen;
	void *table;
}TerDataBlockStr;

/****************************************************
**	读STM32控制快
*****************************************************/
typedef struct
{	
	unsigned char 	flag;
	void 			*block;
}TerDataTcbStr;



/***************************************************
**	单片机所采集到状态数据结构
***************************************************/

typedef struct
{
	unsigned char AccState;    			//ACC状态
	unsigned char MotoState;   			//Moto状态
	unsigned char SdCardState; 			//SD卡状态 
	unsigned char E2PROMState; 			//EEPROM状态
	unsigned char AntConState;    			//天线连接状态
	unsigned char ShellState;  			//外壳状态
	unsigned char AntShortState;   		//天线短路状态 
	unsigned char CanStatus;			//Can 链接状态
	//unsigned char NC1[11];               //预留11个字节
	unsigned short int PowVol;          //电源电压
	unsigned short int Battery;         //电池电压
	unsigned short int TerTemp;         //单片机片上温度
	unsigned short int NC2;             //预留两个字节
	
}TerStateStr;

/**************************************************
**	SD卡相关信息
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
***************************************************/
#pragma pack(1)
typedef struct
{
	unsigned int 	ACCTotal;                    //ACC总时间  
	double		 	ACCTotalHour;                //小时
	double 			TotalDistance;               //行驶总里程  保留两位有效小数
	unsigned int 	TimeSec;                     //系统时间
}SysRunDataStr;
#pragma pack()



//两字节数据类型
typedef union  
{
	unsigned short int	IntII;

	unsigned char	TTbyte[2];
}IntToChar;


//四字节数据类型
typedef union  
{
	unsigned int	LongLL;
	unsigned char	TTbyte[4];
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
	unsigned short int 		FileCurrentFram;	
	unsigned short int 		FileTotalFram;   		//
	unsigned short int 		FileLen[100];           
	unsigned char 			FileBuf[100][1024];   	//文件缓冲区
}UpgradeFileStr;
#pragma pack()

#endif

/*******************************File End******************************/


