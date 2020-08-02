
/***************************************************************
**	FileName:SysProcess.c
**	Time:	2017.08.12
****************************************************************/


#include "include.h"

/**************本地全局变量*******************/

static unsigned char 		DataPacketBuf[512];

static SysRunDataStr 		SysRunData;

static DistanceProStr 		DistancePro;

static	unsigned char 		AccStateBack = 0;     	//标识设备ACC状态

static int 					NextTime = 0;			//

static int					NextTime1 = 0;    		//



/********************************************************
**	函数名称:ResetNexTime(void)
**	功能描述:
********************************************************/

void ResetNextTime(void)
{
	NextTime = 1;		//
	NextTime1 = 1;    	//
}



/********************************************************
**	函数名称:SysRunDataStr *GetSysRunDataSpace(void)
**	功能描述:	
********************************************************/

SysRunDataStr *GetSysRunDataSpace(void)
{
	return &SysRunData;
}


/********************************************************
**	函数名称:unsigned char ReadFarmStatus(void)
**	功能描述:判断系统作业标志
*********************************************************/

unsigned char ReadFarmStatus(void)
{
	static unsigned short counter = 0;
	
	SysConfigStr		*pCfg;
	CanDataStr			*pCanData;
	
	pCfg 	= GetSysCfgSpace();
	pCanData = GetCanDataSpace();
	
	if(ReadAccState() == 0)
		return 0;
	
	switch(pCfg->CarType)
	{
		case 0x03:      					//拖拉机  不进行作业判定  主要是M平台拖拉机
		case 0x0D:
			if(pCanData->EngineRotate > 8000)    
			{
				if(counter < 240)           //
					counter++;
			}
			else
			{
				counter = 0;
			}

			if(counter > 120)
				return 1;
			return 0;
		
		case 0x09:							//水稻机  杂余转速
			if(pCanData->Rotate1 > 10)    
			{
				if(counter < 5)             //
					counter++;
			}
			else
			{
				counter = 0;
			}

			if(counter > 3)
				return 1;
			return 0;
		case 0x0f:							//水稻机  老国三 转速大于2000转，单位是0.125，需要乘于8
			if(pCanData->EngineRotate > 16000)    
			{
				if(counter < 240)           //
					counter++;
			}
			else
			{
				counter = 0;
			}

			if(counter > 120)
				return 1;
			return 0;
		default:
			return 0;
	}

}


/*****************************************************
**	函数名称:
**	功能描述:
*****************************************************/

void SaveSysRunData(unsigned char n)
{
	static unsigned short int counter = 0;

	SysTimeStr *pTime;
	GNSSSTR		*pGnss;
	
	if(n > 0)
		counter = 1000;
	
	if(counter++ >= 900)			  //没15分钟保存一次  大约15分钟
	{	
		counter = 0;
		pTime = GetSysTimeSpace();
		SysRunData.TimeSec = pTime->TimeSec;	  //保存时间 

		pGnss = GetGnssDataSpace();

		SysRunData.latitude = pGnss->latitude;									//维度
		SysRunData.longitude = pGnss->longitude;								//经度
		SysRunData.altitude = pGnss->altitude;      							//海拔高度
		SysRunData.azimuth = pGnss->azimuth;	 								//航向角度
		SysRunData.speed = pGnss->speed;		   								//速度
		SysRunData.ghdop_v = pGnss->ghdop_v;     								//定位因子
		SysRunData.satellite_num = pGnss->satellite_num;         	 			//使用卫星数量
		SysRunData.satellite_view_num = pGnss->satellite_view_num;  			//可视卫星数量
		SysRunData.status = pGnss->status;				            						//定位状态  
		SysRunData.TYear = pGnss->TYear;                                   		//GNSS-时间-年
		SysRunData.TMon = pGnss->TMon;											//GNSS-时间-月
		SysRunData.TDay = pGnss->TDay;											//GNSS-时间-日
		SysRunData.THour = pGnss->THour;										//GNSS-时间-时
		SysRunData.TMin = pGnss->TMin;											//GNSS-时间-分
		SysRunData.TSec = pGnss->TSec;                                     		//GNSS-时间-秒                           
		SysRunData.gnss_time = pGnss->gnss_time;              					//定位时间,改时间以2000年1月1号，0时0分0秒为基准。
		SysRunData.latitude_ns = pGnss->latitude_ns;           					//维度标识
		SysRunData.longitude_ew = pGnss->longitude_ew;            				//精度标识
		
		if(FileCheckOrGreate((unsigned char *)"SysRunDataFile", (unsigned char *)&SysRunData, sizeof(SysRunData),1) > 0)
			SL_Print(">App:Save Sys Run Data.......%d\r\n",n);
	}
		
}


/*****************************************************
**	函数名称:
**	功能描述:
******************************************************/

void ReadSysRunData(void)
{
	GNSSSTR *pGnss;
	
	if(FileCheckOrGreate((unsigned char *)"SysRunDataFile", (unsigned char *)&SysRunData, sizeof(SysRunData),0) > 0)
	{
		SL_Print(">App:Reads Run Data.......OK\r\n");
		if(SysRunData.TimeSec > 3155759999)        //时间
			SysRunData.TimeSec = 0;                //保存的时间回复为0
		// 对维度进行限制
		pGnss = GetGnssDataSpace();

		pGnss->latitude = SysRunData.latitude;									//维度
		pGnss->longitude = SysRunData.longitude;								//经度
		pGnss->altitude = SysRunData.altitude;      							//海拔高度
		pGnss->azimuth = SysRunData.azimuth;	 								//航向角度
		pGnss->speed = SysRunData.speed;		   								//速度
		pGnss->ghdop_v = SysRunData.ghdop_v;     								//定位因子
		pGnss->satellite_num = SysRunData.satellite_num;         	 			//使用卫星数量
		pGnss->satellite_view_num = SysRunData.satellite_view_num;  			//可视卫星数量
		pGnss->status = 'V';				            						//定位状态  
		pGnss->TYear = SysRunData.TYear;                                   		//GNSS-时间-年
		pGnss->TMon = SysRunData.TMon;											//GNSS-时间-月
		pGnss->TDay = SysRunData.TDay;											//GNSS-时间-日
		pGnss->THour = SysRunData.THour;										//GNSS-时间-时
		pGnss->TMin = SysRunData.TMin;											//GNSS-时间-分
		pGnss->TSec = SysRunData.TSec;                                     		//GNSS-时间-秒                           
		pGnss->gnss_time = SysRunData.gnss_time;              					//定位时间,改时间以2000年1月1号，0时0分0秒为基准。
		pGnss->latitude_ns = SysRunData.latitude_ns;           					//维度标识
		pGnss->longitude_ew = SysRunData.longitude_ew;            				//精度标识
		
		SL_Print(">App RunData:%d",SysRunData.TimeSec);
	}
}

/**********************************************************
**	函数名称:
**	功能描述:主要负责处理系统逻辑
***********************************************************/

void SysProcess(void)
{

	static unsigned char 		step = 0,step1 = 0;
	static struct stopwatch16   sw;
	unsigned char 				tmp = 0;
	unsigned short int 			PacketLen = 0;
	SysConfigStr 				*pSysCfg;

	SysTimeStr					*pTime;             //测试使用
	
	pSysCfg = GetSysCfgSpace();						//系统配置
	pTime = GetSysTimeSpace();                      //系统时间
	tmp = ReadFarmStatus();              			//获取工作状态

	if(pSysCfg->CfgFlag != 0x55 || ReadRunMode() > 0)
		return;
	
	switch(step)
	{
		case 0:
			if(pTime->VerifyBit == 0)
				break;
			NextTime = pTime->TimeSec + pSysCfg->TravelUpLoadTime;
			NextTime1 = pTime->TimeSec + pSysCfg->WorkUpLoadTime;
			SL_Print(">App:Init UpLoadTime :%d\r\n",NextTime);
			step++;
			break;
		case 1:                    		 //非工作状态      行驶 的过程中   非作业状态  200米以报位置   
			if(pTime->TimeSec >= NextTime)
			{
				NextTime =  pTime->TimeSec + pSysCfg->TravelUpLoadTime;
				PacketLen = BuildCompletePacket(DataPacketBuf,0);
				//SL_Print("Build Data Time:%d,%d,%d\r\n",pTime->Hour,pTime->Min,pTime->Sec);
				if(PacketLen > 0)
				{
					WriteSendQueue(DataPacketBuf,PacketLen);
					return;
				}
			}
			
			if(tmp > 0)                   //判断作业标识  
				step++;
			break;
		case 2:                   		//工作状态    
			if(tmp == 0)                              //如果工作状态结束，结束打包
			{
				PacketLen = BuildPositionPacket(DataPacketBuf,1);  //结束打包
				if(PacketLen > 0)
				{	
					WriteSendQueue(DataPacketBuf,PacketLen);   //插入到发送队列 
					step = 1;
					return;
				}
			}
			
			if(pTime->TimeSec >= (NextTime - 1))
			{	
				NextTime1 = pTime->TimeSec + pSysCfg->WorkUpLoadTime;
				PacketLen = BuildPositionPacket(DataPacketBuf,1);         	//结束打包
				if(PacketLen > 0)
				{
					WriteSendQueue(DataPacketBuf,PacketLen);   				//插入到发送队列 
					step++;
					return;
				}

			}
			if(pTime->TimeSec < NextTime1)           						//
				break;
			NextTime1 = pTime->TimeSec + pSysCfg->WorkUpLoadTime;
			PacketLen = BuildPositionPacket(DataPacketBuf,0);
			if(PacketLen > 0)
			{
				WriteSendQueue(DataPacketBuf,PacketLen);   					//插入到发送队列
			}
			return;
		case 3:
			if(pTime->TimeSec >= NextTime)
			{
				NextTime =  pTime->TimeSec + pSysCfg->TravelUpLoadTime;
				NextTime1 =  pTime->TimeSec + pSysCfg->WorkUpLoadTime;

				PacketLen = BuildCompletePacket(DataPacketBuf,0);
				if(PacketLen > 0)
				{
					WriteSendQueue(DataPacketBuf,PacketLen);
				}
				step = 2;
			}
			
			return;
		default: 
			step = 0;
			break;
	}

	tmp = ReadAccState();
	switch(step1)                       		// ACC变化上传数据
	{
		case 0:
			if(tmp == AccStateBack)         	//如果ACC发生改变
				break;
			init_stopwatch16(&sw);
			step1++;
			break;
		case 1:
			if(tmp == AccStateBack)
			{
				step1 = 0;
				break;
			}
			if(read_stopwatch16_value(&sw) < 60)
				break;
			
			AccStateBack = tmp;
			PacketLen = BuildCompletePacket(DataPacketBuf,0);
			if(PacketLen > 0)
			{
				//SL_Print("Acc State Change:\r\n");             //测试信息
				WriteSendQueue(DataPacketBuf,PacketLen);
			}
			step1 = 0;
			break;
		default:
			step1 = 0;
			break;
	}
}


/*******************************************************
**	函数名称:void ProcessRunData(void)
**	功能描述:处理运行数据
********************************************************/
void ProcessRunData(void)
{
	static unsigned char step2 = 0;

	TerStateStr 	*pTerState;
	GNSSSTR			*pGnss;

	pTerState = GetTerStateDataSpace();   		//
	
	if(pTerState->AccState > 0)               	//计算ACC的工作时间
	{
		SysRunData.ACCTotal++;                 	//该函数1秒钟执行一次   计算一次
		SysRunData.ACCTotalHour = SysRunData.ACCTotal / 3600.0;
	}
	

	pGnss = GetGnssDataSpace();             	//定位空间    
	
	switch(step2)                            	//通过定位点判断行驶里程
	{


		case 0:                        			//获得基础定位点  
			if(pGnss->status != 'A')
				break;
			if(pGnss->speed < 1)
				break;
			DistancePro.StartLat = pGnss->latitude;           //初始化开始
			DistancePro.StartLong	 = pGnss->longitude;      //初始化开始  
			
			step2++;
			break;
		case 1:
			if(pGnss->status != 'A' || pGnss->speed < 1)
			{
				step2 = 0;	
				break;
			}
			
			DistancePro.EndLat = pGnss->latitude;                   //结束位置
			DistancePro.EndLong = pGnss->longitude ;    			//结束位置   
			DistancePro.Distance = ComputeDistance(DistancePro.StartLat,DistancePro.StartLong,  DistancePro.EndLat,DistancePro.EndLong);
			SysRunData.TotalDistance += DistancePro.Distance;   	//累加行驶距离  
			
			DistancePro.StartLat = DistancePro.EndLat;       		//初始化开始
			DistancePro.StartLong	 = DistancePro.EndLong;      	//初始化开始   
		
			break;
		default:
			step2 = 1;
			break;
	
	}
	
	SaveSysRunData(0);            									//保存数据  
	
}


/********************************File End************************/




