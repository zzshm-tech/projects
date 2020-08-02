
/***********************************************
**	FileName:DataType.h
**	Time:2017.01.26
**	Fun:��ʱϵͳʹ�õ��Զ������ݽṹ
**	�ൺ����8T�ھ��
************************************************/

#ifndef DATATYPE_H
#define DATATYPE_H

/**************���ͻ�����Ԥ����*****************/

#define BLIND_NUM  200                                           //ä������
#define BLIND_BUF  200											//ä������������


/***************************************************************
**	��λ���ݽṹ
**	
****************************************************************/

typedef struct 
{
	double 					latitude;								//ά��
	double 					longitude;	  							//����
	unsigned short int 		altitude;       						//���θ߶�
	unsigned short int 		azimuth;	  							//����Ƕ�
	float 					speed;		            				//�ٶ�
	float  					ghdop_v;      							//��λ����
	unsigned char 			satellite_num;         	 				//ʹ����������
	unsigned char 			satellite_view_num; 					//������������  GPS 
	unsigned char 			satellite_view_num1; 					//������������ BD
	unsigned char 			status;                 				//��λ״̬  
	unsigned char			TYear;                                  //GNSS-ʱ��-��
	unsigned char 			TMon;									//GNSS-ʱ��-��
	unsigned char 			TDay;									//GNSS-ʱ��-��
	unsigned char 			THour;									//GNSS-ʱ��-ʱ
	unsigned char			TMin;									//GNSS-ʱ��-��
	unsigned char 			TSec;                                   //GNSS-ʱ��-��                           
	unsigned int            gnss_time;              				//��λʱ��,��ʱ����2000��1��1�ţ�0ʱ0��0��Ϊ��׼��
	unsigned char 			latitude_ns;            				//ά�ȱ�ʶ
	unsigned char 			longitude_ew;           				//���ȱ�ʶ
	unsigned char 			GnssModeStatus;                         //
}GNSSSTR; 


/***************************************************
**	��λ����ʹ��
***************************************************/
typedef struct
{
	double					StartLat;         						//��ʼ����
	double 					StartLong;          					//��ʼά��
	double 					EndLat;									//��������
	double 					EndLong;            					//����ά��
	float					Speed; 									//��ʻ�ٶ�
	unsigned short int 		StartAzimuth;     						//����Ƕ�
	unsigned short int 		EndAzimuth;     						//����Ƕ�
	double 					Distance;                               //��ʻ����
}DistanceProStr;

/****************************************************/
//TCPIP������Ϣ
typedef struct 
{

	unsigned char 			GprsApn[20];           					//���뷽ʽ
	unsigned char 			GprsNetActive;		   					//GPRS����״̬
	BOOL          			SimState;         	   					//�Ƿ���Sim��
	unsigned char 			SimIccId[25];     	   					//SIM��ICC_ID
	unsigned char			GsmSign;       	 						//GSM�ź�ֵ			
	unsigned char 			ServerAddr[50];        					//ƽ̨��ַ  ��������ַ
	unsigned short int 		ServerPort;       						//�˿�    �������˿�  
	unsigned char 			UlrToIPFlag;	                        //��ַ��ʶ��0:IP��ַ��1:URL	
	unsigned char 			LinkState;             					//����״̬
	unsigned char 			FtpLinkState;							//FTP����״̬
	unsigned char  			LinkNum;              					//���Ӵ��� ��Ҫ��ס���Ӷ��ٴ�֮������Ĭ�����ص�ַ
	unsigned char 			LinkCount;								//���Ӽ�����
	unsigned char 			GprsLinkState;             				//Gprs����״̬
	unsigned char 			SendState;              				//����״̬ 
	int			 			SimStatus;              				//Sim��״̬
	int						NetWorkState;           				//�ƶ�����״̬
	unsigned char			NetGprsState;           				//GPRS����״̬   GPRS�����Ƿ��� 
	unsigned char 			GprsAttachState;						//GPRS���總��״̬
	unsigned char 			ErrorCode;              				//������ ����ʱû��ʹ�� 
	unsigned char 			ResetLink;              				//��������
	unsigned char			LinkType;								//(1)FPT ����(0)TCP/IP����  
	unsigned char 			FtpLinkNum;								//FPT���Ӵ���
	unsigned char			FtpAddr[50];           					//Ftp��������ַ
	unsigned char 			FtpUserName[20];       					//�û���
	unsigned char 			FtpUserPassd[20];      					//����
	unsigned char 			FtpFileName[20];       					//�ļ�����  
	unsigned short int 		FtpPort;              					//�˿�
	
	unsigned short int 		FtpFileLen;            					//�ļ�����
	unsigned char 			*FtpFileSapce;          				//FTP���ص��ļ�ָ��
	unsigned char 			FtpLoadFig;          					//���ر�ʶ
}TcpIPSocketStr;


/*********************************************
**	��ʱû��ʹ��
*********************************************/

typedef struct
{
	unsigned char 			IpAddr[50];        					//ƽ̨��ַ  ��������ַ
	unsigned short int      Port;
}IpAddrBackStr;


/************************************************
**	ϵͳ����
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
	unsigned char 			Year;              								//ϵͳʱ��-��
	unsigned char 			Mon;               								//ϵͳʱ��-��
	unsigned char 			MDay;              								//ϵͳʱ��-��
	unsigned char 			WDay;              								//ϵͳʱ��-��
	unsigned char 			Hour;              								//ϵͳʱ��-ʱ
	unsigned char 			Min;               								//ϵͳʱ��-��
	unsigned char 			Sec;               								//ϵͳʱ��-��
	unsigned int 			TimeSec;           								//ϵͳʱ��-���룬��2000��1��1��0ʱ0��0�����
	unsigned char   		VerifyBit;         								//ʱ��У׼  ��ʶʱ���Ƿ��Ѿ�У׼
}SysTimeStr;


/***********************************************************
**	�����ϴ��ṹ
***********************************************************/
#pragma pack(1)

typedef struct
{
	unsigned char 			DataBuf[BLIND_BUF];     							//���ͻ�����
	unsigned short int 		DataLen;          							//���ݳ���
	unsigned char 			DataCrc;          							//У��
	unsigned char       	DataBig;          							//���ݰ���ʶ(0:�������ݣ�1:ä���洢����)
	
}FiFoStr;
#pragma pack()

/*************************************************************
**	���Ͷ���
*************************************************************/
#pragma pack(1)

typedef struct
{
	unsigned short int 		QNum;             							//���Ͷ���
	unsigned short int 		QWrite;           							//���Ͷ��ж�ͷ
	unsigned short int 		QRead;            							//���Ͷ��ж�β
	FiFoStr            		QData[BLIND_NUM];       						//���Ͷ������ݻ�����
}SendQueueStr;
#pragma pack()




/*****************************************
**	���л��͵�CAN���ݶ����ڸ����ݽṹ����
** 	��������
*******************************************/
/*
#pragma pack(1)
typedef struct
{
	unsigned short int 		SysVol;           	 		//��Դ��ѹ
	unsigned char 			TempWater;           		//��ȴˮ�¶�
	unsigned short int 		EngineRotate;		 		//������ʵ��ת��
	unsigned short int 		EngineRotateSet;     		//������Ŀ��ת�٣�Ԥ����
	unsigned char   		FuelPercent;		 		//ȼ��λ
	unsigned short int		WorkTime;			 		//����ʱ�䷴��
	unsigned char 			LockCarState;				//����״̬
	unsigned char 			PreheatSign;				//Ԥ���ź� 
	unsigned char 			FlameoutSign;				//Ϩ����ʱ����ź�
	unsigned char 			TravelState;				//�����ٶȷ���
	unsigned char 			ChargeWarn;					//��籨�� 
	unsigned char 			AirFiltraWarn;				//������о��������
	unsigned char 			WaterLowWarn;				//ˮλ�ͱ���
	unsigned char			PowerVolLowWarn;  			//��Դ��ѹ�ͱ���
	unsigned char 			OilWateWarn;   				//��ˮ���뱨��
	unsigned char			EngineOilWarn;				//����ѹ���ͱ���
	unsigned char 			WaterTempUpWarn;			//ˮ����������
	unsigned char			WaterTempHighWarn;      	//ˮ�¸߱��� 
	unsigned char		 	FuelOilWarn;            	//ȼ��λ����

}CanDataStr;
#pragma pack()
*/


#pragma pack(1)
typedef struct
{
	unsigned short int 			SysVol;           	 		//��Դ��ѹ
	unsigned char 				TempWater;           		//��ȴˮ�¶�
	unsigned short int 			EngineRotate;		 		//������ʵ��ת��
	unsigned short int 			EngineRotateSet;     		//������Ŀ��ת�٣�Ԥ����
	unsigned char 		   		WorkFlag;               	//������λ
	unsigned short int    		RotaryVal;					//��ť��ѹ
	unsigned short int 			FuelPercent;            	//ȼ�Ͱٷֱ�
	unsigned int				WorkTime;			 		//发动机工作时间
	unsigned char 				LockCarState;				//����״̬
	unsigned short int 			RelayVol;  					//�̵�����ѹ 
	unsigned char 				SwitchState;				//����״̬  
	unsigned char 				WorkModeState;   			//����ģʽ����  
	unsigned short int			ECMFaultCode;				//ECM�ֹ��ϴ���
	unsigned short int 			FrontPumpPressure;			//ǰ��ѹ��
	unsigned char 				MsgCarWarnValue1;			//��������״̬1
	unsigned char 				MsgCarWarnValue2;			//��������״̬2
	//��������¸߱���
    //��������ѹ�ͱ���
    //���ָʾ
    //������ˮ�¸߱���
    //��ˮ���뱨��
    //���˱���
    //�����ʹ��˱���
    //ȼ�������������
    //ɢ�ȱ���
    //��ɢ�ȱ���
    //�ƶ�����
    //��������
   	unsigned char        		TransferOilTemp;  			//�������¶�
   	unsigned char       	 	BrakePressure;       		//�ƶ���ѹ
   	unsigned char        		OuterTemp;          		//�����¶�
   	unsigned char        		InnerTemp;          		//��ʻ�����¶�
	unsigned int 				MeterWorkTime;				//�Ǳ�����Сʱ
	unsigned short int 			OilTemp;         			//�����¶�
	unsigned char 				AirPressure;         		//����ѹ��
	unsigned short int 			AirTemp;					//�����¶�
	unsigned char 				Accelerator;				//����̤��λ��
	unsigned char 				AbsoluteOilPressure;		//���Ի���ѹ��
	unsigned int 				OnceFuel;      				//�����ͺ�
	unsigned char				RelativeAddPressure;		//��Ի���ѹ��
    unsigned char 				EnteredAirTemp;				//��������������¶�
	unsigned int 	   			TotalFuel;  				//�ۼ��ͺ�
	unsigned short int 			EnginePowerVol;				//������Դ��ѹ����������

 	unsigned char 				ECURes;						//ECU����Ӧ��֡
	unsigned char 			    ECUState;             		//ECU����״̬
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
	
}GSMStationInfoStr;    //GSM��վ��Ϣ
#pragma pack()


/******************************************
**
******************************************/
#pragma pack(1)
typedef struct
{
	unsigned char 			FrameStart[3];			// 
    unsigned char			MsgId;          		//
	unsigned short int 		MsgLen;					// 
	unsigned char 			TypeCode;               //
	unsigned short int 		DataLen; 				// 
	unsigned char 			TerminalCode[6];        //
	unsigned short int 		ProVerNum;   			//
	unsigned char 			ManuNum;                //
	unsigned char 			CarType;                //
	unsigned int 			SerialNum;              // 
	unsigned char 			CmdID;					//
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

	unsigned short int 		MsgSysVol;           	 		//
	unsigned char 			MsgTempWater;           		//
	unsigned short int 		MsgEngineRotate;		 		//
	unsigned short int 		MsgEngineRotateSet;     		//
	unsigned char 			MsgEngineTorque;				//
	unsigned char 			MsgWorkFlag;					//
	unsigned short int 		MsgRotaryVal;         			//
	unsigned char 			MsgOilTemp;						//
	unsigned short int 		MsgFuelPumpPressure;			//
	unsigned short int 		MsgEngineOilPressure;			//
	unsigned char 			MsgAirPressure;                	//
	unsigned short int 		MsgAddPressure;					//
	unsigned char 			MsgAddPressureTemp;				//
	unsigned char 			MsgEnteredAirTemp;				//
	unsigned char   		MsgFuelPercent;		 			//
	unsigned char 			MsgComPathPressure;				//
	unsigned char 			MsgComPathDis;					//����ѹ��
	unsigned short int 		MsgFuelGoalGush;				//ȼ�Ͳ���������
	unsigned char 			MsgFuelGoalControl;				//
	unsigned short int		MsgWorkTime;			 		//发动机工作时间
	unsigned char 			MsgLockCarState;				//
	unsigned char 			MsgControllerFaultCode;        	//
	unsigned char 			MsgSwitchState;					//����״̬  
	unsigned short int 		MsgRelayVol;  					//�̵�����ѹ 
	unsigned char 			MsgWokrModeState;   			//����ģʽ����  
	unsigned short int 		MsgFuelFlow;					//ȼ������
	unsigned short int 		MsgShowEngineRotate;           	//ָʾ������ת��  
	unsigned int 			MsgFuelQuantity;               	//ȼ����
	unsigned int 			MsgFuelTotalQuantity;			//�ۼ�ȼ����
	unsigned short int		MsgECMFaultCode;				//ECM�ֹ��ϴ���
	unsigned char 			MsgFluidOilTemp;				//Һѹ���¶�
	unsigned short int 		MsgFrontPumpPressure;			//ǰ��ѹ��
	unsigned short int 		MsgBackPumpPressure;			//���ѹ�� 
	unsigned char 			MsgBucketRodExtend;				//��������ȵ�
	unsigned char 			MsgBucketRodAnastole;			//���������ȵ�
	unsigned char 			MsgRotation;					//��ת�ȵ�
	unsigned char 			MsgTravelL;  					//����L�ȵ�
	unsigned char 			MsgTravelR;						//����R�ȵ�
	unsigned char 		   	MsgSwingArmUp;					//���������ȵ�
	unsigned char 			MsgSwingArmDrop;				//�����½��ȵ�
	unsigned char 			MsgBucketExtend; 				//�������
	unsigned char 			MsgBucketAnastole;				//��������
	unsigned short int		MsgFrontPumpRadiotube;			//ǰ�ñ�����ŷ� 
	unsigned short int 		MsgBackPumpRadiotube;			//��ñ�����ŷ�
	unsigned short int 		MsgBucketRatioRadiotube;		//��������������ŷ�
	unsigned short int 		MsgSwingArmRatioRadiotube;  	//������Ի�ת������ŷ�
	unsigned short int 		MsgSwingArmRadiotube1;			//��������1������ŷ�
	unsigned short int  	MsgSwingArmRadiotube2;	  		//��������2������ŷ�
	unsigned short int 		MsgBreakHammerValue;			//���鴸������������ǰֵ 
	unsigned char 			MsgPowerControlValue;			//���ʵ�����ǰֵ
	unsigned char 			MsgCarWarnValue1;				//��������״̬1
	unsigned char 			MsgCarWarnValue2;				//��������״̬2
}MsgExcavatorStr;               //�ھ��CAN         

#pragma pack()



#pragma pack(1)
/*****************************************************
**	װ�ػ����ݽṹ
*****************************************************/
typedef struct
{
	unsigned char 			MsgLoaderType;       		//
	unsigned short int 		MsgLoaderLen;	   			// 

	unsigned short int 		MsgDeviceVol;				//
	unsigned int 			MsgDeviceWorkTime;			//

	unsigned short int 		MsgSysVol;					//
	unsigned int            MsgWorkTime;				//
	unsigned char 			MsgTransmitOilTemp;			//			
	unsigned char 			MsgBrakePressure;			//
	unsigned char 			MsgFuelPercent;				//
	unsigned char 			MsgCabTemp;					//
	unsigned char 			MsgOutdoorTemp;				//
	unsigned char 			MsgShiftHandleLocate;		//
	unsigned char 			MsgGearboxValue;			//
	unsigned char 			MsgGearboxPressure;			//
	unsigned char 			MsgClutchKVPressure;		//
	unsigned char 			MsgClutchKRPressure;		//
	unsigned char 			MsgClutchK1Pressure;		//
	unsigned char 			MsgClutchK2Pressure;		//
	unsigned char 			MsgClutchK3Pressure;		//
	unsigned char 			MsgClutchK4Pressure;		//
	unsigned char 			MsgGearboxMode;				//
	unsigned char 			MsgClutchKVCurrent;			//
	unsigned char 			MsgClutchKRCurrent;			//
	unsigned char 			MsgClutchK1Current;			//
	unsigned char 			MsgClutchK2Current;			//
	unsigned char 			MsgClutchK3Current;			//
	unsigned char 			MsgClutchK4Current;			//
	unsigned short int		MsgGearBoxInRotate;			//
	unsigned short int      MsgGearBoxGearRotate;		//
	unsigned short int 		MsgGearBoxMidRotate; 		// 
	unsigned short int		MsgGearBoxOutRotate;		// 
	unsigned short int 		MsgGearBoxFault;			//
	unsigned short int 		MsgSysWarnvalue;			//
 	unsigned char 			MsgTempWater;           	//
 	unsigned short int 		MsgEngineRotate;			//
	unsigned short int 		MsgEngineFault;           	//													
	unsigned char 			MsgAirPressure;        		//
	unsigned short int		MsgAirTemp;					//
	unsigned char			MsgFootboardPosition;  		//
	unsigned char 		    MsgEngineOilPressure;		//
	unsigned short int 		MsgOilTemp;					//
	unsigned char 			MsgAirFanState;				//
	unsigned short int 		MsgAirFanRotate;			//
	unsigned short int 		MsgFule;					//
	unsigned char 			MsgAddPressure;				//
	unsigned char 			MsgEnteredAirTemp;			//
	unsigned short int		MsgCarPowerVol;				//
	unsigned int 			MsgEnteredWorkTime;			//发动机累计工作时间
	unsigned int 			MsgFuleTotal; 				//
	unsigned char 			MsgECUFirVer[8];			//
	unsigned char		    MsgECULockFunState;			//
	unsigned short int		MsgECULockState;			//
	unsigned char 			MsgEntereLockState;			//
	unsigned char 			MsgEntereWorkMode;			//
	unsigned short int		MsgHydraulicPressure;		//
}MsgLoaderStr;                //
#pragma pack()


//�����û�Э���
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


//ͨѶЭ����Ϣͷ
#pragma pack(1)
typedef struct 
{
    unsigned char 			frame_start[3];			// ��Ϣ��ͷ
    unsigned char			msg_id;          		// ��ϢID
    unsigned char 			device_id[16];   		// �豸��
	unsigned char 			blind_flag;             // ä����ʶ
    unsigned char 			msg_line;  				// �����Ϣ�к�
    unsigned char 			msg_body_num;     		// ��Ϣ������
    unsigned short int 		msg_len;        		// ��Ϣ�ܳ���
}MsgHeadStr;

#pragma pack()


//���̻�е����Э��ͷ

#pragma pack(1)
typedef struct 
{
    unsigned char 			FrameStart[3];			// ��Ϣ��ͷ
    unsigned char			MsgId;          		// ��ϢID
	unsigned short int 		MsgLen;					// ���ݳ���
}MsgQDHeadStr;
#pragma pack()


/*************************************************************
**	�豸״̬��Ϣ��
************************************************************/

#pragma pack(1)
typedef struct 
{
	unsigned char 	 		MsgDeviceType;                  //��Ϣ��������
	unsigned short int 		MsgDeviceLen;                 	//��Ϣ�峤��(�������򳤶�)
	
	unsigned char 			MsgDeviceID[8];					//�豸ID�� 
	unsigned char	 		MsgProtocolVer;     			//Э��汾��  zuowei  
	unsigned char 			MsgFirVer;						//
	unsigned char      		MsgManuNum;            			//���ұ��     		//
	unsigned char 	   		MsgManuType;            		//�����ն�����        			    //
	unsigned char      		MsgDeviceCarNum;        		//���ͱ��    
	unsigned char      		MsgAppVer;              		//�̼��汾          
	unsigned char      		MsgHardwareVer;             	//�Ѿ��汾��
}MsgDeviceStr;
#pragma pack()



/*************************************************
**	��������״̬
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
	unsigned char	 		MsgGnssAzimuth;         		//�����   ʹ��һ���ֽ�
	short int 				MsgGnssAltitude;        		//���θ߶�
	unsigned char      		MsgGnssYear;                  	//����-��
	unsigned char      		MsgGnssMon;                   	//����-��
	unsigned char      		MsgGnssDay;						//����-��
	unsigned char      		MsgGnssHour;                 	//����-ʱ
	unsigned char      		MsgGnssMin;                   	//����-��
	unsigned char      		MsgGnssSec;                   	//����-��
	unsigned char      		MsgGnssSatelliteNum;         	//ʹ�����ǿ���
	unsigned char      		MsgGnssViewNum;              	//����������Ŀ
	unsigned char      		MsgGnssStatus;              	//GNSS��λ״̬
}MsgGnssStr;

#pragma pack()



/***************************************************
**	STM32���ݿ�
****************************************************/
typedef struct
{
	unsigned short int TypeCode;
	unsigned short int DataLen;
	void *table;
}TerDataBlockStr;

/****************************************************
**	��STM32���ƿ�
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
**	ϵͳ���У������Ϣ
**	
***************************************************/
#pragma pack(1)
typedef struct
{

	unsigned int 			ACCTotal;                    			//ACC��ʱ��  
	double		 			ACCTotalHour;                			//Сʱ
	double 					TotalDistance;               			//��ʻ�����  ������λ��ЧС��
	unsigned int 			TimeSec;                     			//ϵͳʱ�� 
	double 					latitude;								//ά��
	double 					longitude;	  							//����
	unsigned short int 		altitude;       						//���θ߶�
	unsigned short int 		azimuth;	  							//����Ƕ�
	float 					speed;		            				//�ٶ�
	float  					ghdop_v;      							//��λ����
	unsigned char 			satellite_num;         	 				//ʹ����������
	unsigned char 			satellite_view_num; 					//������������
	unsigned char 			status;                 				//��λ״̬  
	unsigned char			TYear;                                  //GNSS-ʱ��-��
	unsigned char 			TMon;									//GNSS-ʱ��-��
	unsigned char 			TDay;									//GNSS-ʱ��-��
	unsigned char 			THour;									//GNSS-ʱ��-ʱ
	unsigned char			TMin;									//GNSS-ʱ��-��
	unsigned char 			TSec;                                   //GNSS-ʱ��-��                           
	unsigned int            gnss_time;              				//��λʱ��
	unsigned char 			latitude_ns;            				//ά�ȱ�ʶ
	unsigned char 			longitude_ew;           				//���ȱ�ʶ
}SysRunDataStr;
#pragma pack()



//���ֽ���������
typedef union  
{
	unsigned short int		IntII;
	unsigned char			TTbyte[2];
}IntToChar;


//���ֽ���������
typedef union  
{
	unsigned int			LongLL;
	unsigned char			TTbyte[4];
}LongToChar;


/**************************************
**	�̼���������
***************************************/
#pragma pack(1)
typedef struct 
{
	unsigned char			UpgradeStatus;		   //0:
	unsigned char 			UpgradeFlag;           //������ʶ
 }UpgradeStatusStr;
#pragma pack()

/**************************************
**	�̼������ļ�
**************************************/
#pragma pack(1)
typedef struct
{
	unsigned int 			FileTotalLen;           //�ļ��ܳ��ȣ����ֽ�Ϊ��λ��
	unsigned short int 		FileVerfy;				//�ļ�У��
	unsigned short int 		FileCurrentFram;	
	unsigned short int 		FileTotalFram;   		//
	unsigned short int 		FileLen[100];           
	unsigned char 			FileBuf[100][1024];   	//�ļ�������
}UpgradeFileStr;
#pragma pack()




/**************************************
**	˵��:
**  ͨ����������ST86����
**	���յ������ݰ�
**************************************/
#pragma pack(1)
typedef struct
{
	unsigned int 			FileLen;           			//�����ļ��ܳ���
	unsigned short int 		FileVerfy;					//�ļ�У��ֵ������У���
	unsigned int 			CurrentLen;					//��ǰ����
	unsigned int 			CurrentFramNum;   			//��ǰ���ݰ�   
}UpgradeST86FileStr;
#pragma pack()



#endif









/*******************************File End******************************/









