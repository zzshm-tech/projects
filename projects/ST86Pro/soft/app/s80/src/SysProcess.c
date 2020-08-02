
/***************************************************************
**	FileName:
**	Time:
****************************************************************/


#include "include.h"

static unsigned char 		DataPacketBuf[512];

static SysRunDataStr 		SysRunData;

static DistanceProStr 		DistancePro;

static	unsigned char 		AccStateBack = 0;

/********************************************************
**	函数名称:
**	功能描述:
********************************************************/

SysRunDataStr *GetSysRunDataSpace(void)
{
	return &SysRunData;
}


/********************************************************
**	函数名称:
**	功能描述:判断系统作业标志
*********************************************************/

unsigned char ReadFarmStatus(void)
{
	static unsigned short counter = 0;
	
	TerStateStr 		*p;
	SysConfigStr		*pCfg;
	CanWheatStr 		*pWheat;		//小麦机
	CanPaddyStr 		*pPaddy;		//水稻机
	CanMaizeStr			*pMaize;		//玉米机
	GNSSSTR				*pGnss;         //GPS定位信息
	
	pCfg 	= GetSysCfgSpace();
	p 		= GetTerStateDataSpace();
	pWheat 	= GetCanWheatSpace();
	pPaddy 	= GetCanPaddySpace();
	pMaize 	= GetCanMaizeSpace();
	//SL_Print("CarType:%d,%d\r\n",pCfg->CarType,pWheat->Rotate1);
	switch(pCfg->CarType)
	{
		case 0x03:      //拖拉机  不进行作业判定
			return 0;
		case 0x08:				//小麦机  轴流滚筒转速  
			if(pWheat->Rotate1 > 10)    
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
		case 0x09:				//水稻机  杂余转速
			if(pPaddy->Rotate1 > 10)    
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
		case 0x0A:				//玉米机 剥皮机转速
			if(pMaize->Rotate1 > 10)    
			{
				if(counter < 200)             //
					counter++;
			}
			else
			{
				counter = 0;
			}

			if(counter > 120)
				return 1;
			return 0;
		case 0x0E:                  //小麦机  老国三
			pGnss = GetGnssDataSpace();
			if(pWheat->EngineRotate > 16000 && pGnss->speed > 1 && pGnss->speed  < 12)
			{
				if(counter < 240)
					counter++;
			}
			else
			{
				if(counter > 0)
					counter--;
			}
			
			if(counter > 120)
			{
				pWheat->WorkFlag = 1;
				
				return 1;
			}
			return 0;
		default:
			return 0;
	}
	
}


/*****************************************************
**	函数名称:
**	功能描述:
*****************************************************/

void SaveSysRunData(void)
{
	if(FileCheckOrGreate((unsigned char *)"SysRunDataFile", (unsigned char *)&SysRunData, sizeof(SysRunData),1) > 0)
		SL_Print(">App:Save Sys Run Data.......OK\r\n");
		
}


/*****************************************************
**	函数名称:
**	功能描述:
******************************************************/

void ReadSysRunData(void)
{
	if(FileCheckOrGreate((unsigned char *)"SysRunDataFile", (unsigned char *)&SysRunData, sizeof(SysRunData),0) > 0)
	{
		SL_Print(">App:Reads Run Data.......OK\r\n");
		if(SysRunData.TimeSec > 3155759999)
			SysRunData.TimeSec = 0;                //保存的时间回复为0
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
	static unsigned int 		NextTime = 0,NextTime1 = 0;    //
	static struct stopwatch16   sw;
	unsigned char 				tmp = 0;
	unsigned short int 			PacketLen = 0;
	SysConfigStr 				*pSysCfg;

	SysTimeStr					*pTime;             //测试使用
	
	pSysCfg = GetSysCfgSpace();
	pTime = GetSysTimeSpace();
	tmp = ReadFarmStatus();              			//获取工作状态

	if(pSysCfg->CfgFlag != 0x55)
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
				PacketLen = BuildPositionPacket(DataPacketBuf,1);         //结束打包
				if(PacketLen > 0)
				{
					WriteSendQueue(DataPacketBuf,PacketLen);   //插入到发送队列 
					step++;
					return;
				}

			}
			if(pTime->TimeSec < NextTime1)           		//明天处理
				break;
			NextTime1 = pTime->TimeSec + pSysCfg->WorkUpLoadTime;
			PacketLen = BuildPositionPacket(DataPacketBuf,0);
			if(PacketLen > 0)
			{
				WriteSendQueue(DataPacketBuf,PacketLen);   	//插入到发送队列
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
	switch(step1)
	{
		case 0:
			if(tmp == AccStateBack)
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
				SL_Print("Acc State Change:\r\n");
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
	static unsigned char counter2 = 0;
	
	TerStateStr 	*pTerState;
	GNSSSTR			*pGnss;
	SysTimeStr 		*pTime;
	//unsigned int 	Tmp = 0,Tmp1 = 0;

	pTerState = GetTerStateDataSpace();   //
	
	if(pTerState->AccState > 0)               //计算ACC的工作时间
	{
		SysRunData.ACCTotal++;                 //该函数1秒钟执行一次   计算一次
		SysRunData.ACCTotalHour = SysRunData.ACCTotal / 3600.0;
	}
	

	pGnss = GetGnssDataSpace();             //定位空间
	
	switch(step2)                            //通过定位点判断行驶里程
	{
		case 0:                        		//获得基础定位点  
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
			//Tmp = SysRunData.TotalDistance * 10;
			//Tmp1 = DistancePro.Distance * 1000;
			//SL_Print("The TotalDistance1:%d***********%d\r\n",Tmp,Tmp1);
			break;
		default:
			step2 = 1;
			break;
	
	}
	
	pTime = GetSysTimeSpace();
	
	if(counter2++ >= 900)             //没15分钟保存一次  大约15分钟
	{	
		counter2 = 0;
		pTime = GetSysTimeSpace();
		SysRunData.TimeSec = pTime->TimeSec;
		SaveSysRunData();            //保存数据  
	}
}


/********************************File End************************/

