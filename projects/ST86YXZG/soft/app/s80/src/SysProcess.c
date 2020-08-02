
/***************************************************************
**	FileName:
**	Time:
****************************************************************/


#include "include.h"

static unsigned char 		DataPacketBuf[512];						//

static SysRunDataStr 		SysRunData;								//

static DistanceProStr 		DistancePro;							//

static	unsigned char 		AccStateBack 			= 0;     		//

static unsigned char 		ShellStateBack 			= 0;           	//

static int 					NextTime 				= 0;			//

static int					NextTime1 				= 0;    		//


static  unsigned char 				cmd_unmon_back; 				//
static	unsigned char 				cmd_mon_back;					//FRQ1输入   ECU握手数据
static	unsigned char 				cmd_hand_back;         			//FRQ1输出  
static	unsigned char	 			cmd_lock_back;               		//锁车命令
static	unsigned char				cmd_unlock_back;						//监控命令 

/**********************************************
**	
*************************************************/

void ResetNextTime(void)
{
	NextTime = 1;		//
	NextTime1 = 1;    	//
}



/********************************************************
**	��������:
**	��������:
**	
********************************************************/

SysRunDataStr *GetSysRunDataSpace(void)
{
	return &SysRunData;
}

 

/*****************************************************
**	函数名称:
**	功能描述：
*****************************************************/

void SaveSysRunData(unsigned char n)
{
	static unsigned short int counter = 0;

	SysTimeStr *pTime;
	GNSSSTR		*pGnss;
	CanDataStr	*pCan;
	
	if(n > 0)
		counter = 1000;
	//SL_Print("Save Sys Run:%d\r\n",counter);
	if(counter++ >= 900)			  //
	{	
		counter = 0;
		pTime = GetSysTimeSpace();
		SysRunData.TimeSec = pTime->TimeSec;	  //

		pGnss = GetGnssDataSpace();

		SysRunData.latitude = pGnss->latitude;									//
		SysRunData.longitude = pGnss->longitude;								//
		SysRunData.altitude = pGnss->altitude;      							//
		SysRunData.azimuth = pGnss->azimuth;	 								//
		SysRunData.speed = pGnss->speed;		   								//
		SysRunData.ghdop_v = pGnss->ghdop_v;     								//
		SysRunData.satellite_num = pGnss->satellite_num;         	 			//
		SysRunData.satellite_view_num = pGnss->satellite_view_num;  			//
		SysRunData.status = pGnss->status;				            			//
		SysRunData.TYear = pGnss->TYear;                                   		//
		SysRunData.TMon = pGnss->TMon;											//
		SysRunData.TDay = pGnss->TDay;											//
		SysRunData.THour = pGnss->THour;										//
		SysRunData.TMin = pGnss->TMin;											//
		SysRunData.TSec = pGnss->TSec;                                     		//                       
		SysRunData.gnss_time = pGnss->gnss_time;              					//
		SysRunData.latitude_ns = pGnss->latitude_ns;           					//
		SysRunData.longitude_ew = pGnss->longitude_ew;            				//
		
		pCan = GetCanDataSpace();

		SysRunData.CanECUWorkTime = pCan->EngineWorkTime;		//发动机工作时间
		SysRunData.CanOilPosition = pCan->OilPosition;     //燃油位置
		
		if(FileCheckOrGreate((unsigned char *)"SysRunDataFile", (unsigned char *)&SysRunData, sizeof(SysRunData),1) > 0)
		{
			SL_Print(">App:Save Sys Run Data.......%d\r\n",n);
		}
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
		if(SysRunData.TimeSec > 3155759999)        //
			SysRunData.TimeSec = 0;                //
		// 
		pGnss = GetGnssDataSpace();

		pGnss->latitude = SysRunData.latitude;									//
		pGnss->longitude = SysRunData.longitude;								//
		pGnss->altitude = SysRunData.altitude;      							//
		pGnss->azimuth = SysRunData.azimuth;	 								//
		pGnss->speed = SysRunData.speed;		   								//
		pGnss->ghdop_v = SysRunData.ghdop_v;     								//
		pGnss->satellite_num = SysRunData.satellite_num;         	 			//
		pGnss->satellite_view_num = SysRunData.satellite_view_num;  			//
		pGnss->status = 'V';				            						//
		pGnss->TYear = SysRunData.TYear;                                   		//
		pGnss->TMon = SysRunData.TMon;											//
		pGnss->TDay = SysRunData.TDay;											//
		pGnss->THour = SysRunData.THour;										//
		pGnss->TMin = SysRunData.TMin;											//
		pGnss->TSec = SysRunData.TSec;                                     		//                          
		pGnss->gnss_time = SysRunData.gnss_time;              					//
		pGnss->latitude_ns = SysRunData.latitude_ns;           					//
		pGnss->longitude_ew = SysRunData.longitude_ew;            				//

		

		if(SysRunData.CanOilPosition > 100)
			SysRunData.CanOilPosition = 100;
		SL_Print(">App RunData:%d,%d,%d",SysRunData.TimeSec,SysRunData.CanECUWorkTime,SysRunData.CanOilPosition);
	}
}

/**********************************************************
**	函数名称:
**	功能描述:
***********************************************************/

void SysProcess(void)
{

	static unsigned char 		step = 0,step1 = 0,step2 = 0,step3 = 0;
	static struct stopwatch16   sw,sw1,sw3;
	unsigned char 				tmp = 0;
	unsigned short int 			PacketLen = 0;
	SysConfigStr 				*pSysCfg;
	TerStateStr					*pTerState;
	SysTimeStr					*pTime;             //
	
	pSysCfg = GetSysCfgSpace();						//
	pTime = GetSysTimeSpace();                      //

	if(pSysCfg->CfgFlag != 0x55 || ReadRunMode() > 0)
		return;

	pTerState	= GetTerStateDataSpace();
	switch(step3)
	{
		 case 0:
		 	step3++;
			(pTerState->cmd_hand == 0) ? cmd_hand_back = 0 : cmd_hand_back;
			
			
			(pTerState->cmd_lock == 0)	? cmd_lock_back = 0 : cmd_lock_back;
			(pTerState->cmd_unlock == 0) ? cmd_unlock_back = 0 : cmd_unlock_back;
			(pTerState->cmd_mon == 0) ? cmd_mon_back = 0 : cmd_mon_back;
			(pTerState->cmd_unmon == 0) ? cmd_unmon_back = 0 : cmd_unmon_back;
			
			init_stopwatch16(&sw3);
		 	break;
		case 1:
			if(read_stopwatch16_value(&sw3) < 30)    //3S 
				break;
			//SL_Print("The Local Log:%d,%d,%d,%d,%d\r\n",pTerState->cmd_hand,cmd_hand_back,pTerState->cmd_unlock,pTerState->cmd_mon,pTerState->cmd_unmon);
			
			if(pTerState->cmd_hand != cmd_hand_back && pTerState->cmd_hand > 0)
			{
				cmd_hand_back = pTerState->cmd_hand;
				InsertLogInfo("H\r\n",sizeof("H\r\n"));   //ECU发出握手信号
			}
			if(pTerState->cmd_lock!= cmd_lock_back && pTerState->cmd_lock > 0)
			{
				cmd_lock_back = pTerState->cmd_lock;
				InsertLogInfo("L1\r\n",sizeof("L1\r\n"));   //L1
			}
			if(pTerState->cmd_unlock != cmd_unlock_back && pTerState->cmd_unlock > 0)
			{
				cmd_unlock_back = pTerState->cmd_unlock;
				InsertLogInfo("L0\r\n",sizeof("L0\r\n"));   //L0:
			}
			if(pTerState->cmd_mon != cmd_mon_back && pTerState->cmd_mon > 0)
			{
				cmd_mon_back = pTerState->cmd_mon;
				InsertLogInfo("M1\r\n",sizeof("M1\r\n"));  //打开锁车
			}
			if(pTerState->cmd_unmon != cmd_unmon_back && pTerState->cmd_unmon > 0)
			{
				cmd_unmon_back = pTerState->cmd_unmon;
				InsertLogInfo("M0\r\n",sizeof("M0\r\n"));  //MO关闭锁车
			}
			step3 = 0;
			break;
		default:
			break;
	}
	
	switch(step)
	{
		case 0:
			if(pTime->VerifyBit == 0)
				break;
			NextTime = pTime->TimeSec + pSysCfg->TravelUpLoadTime;     //
			NextTime1 = pTime->TimeSec + pSysCfg->WorkUpLoadTime;      //
			SL_Print(">App:Init UpLoadTime :%d\r\n",NextTime);         //
			step++;
			break;
		case 1:                    		 //  
			if(pTime->TimeSec >= NextTime)
			{
				NextTime =  pTime->TimeSec + pSysCfg->TravelUpLoadTime;  //
				PacketLen = BuildCompletePacket(DataPacketBuf,1);
				if(PacketLen > 0)
				{
					WriteSendQueue(DataPacketBuf,PacketLen);
					//SL_Print("Write Queue Data.........\r\n");  //输出调试信息
					return;
				}
			}
			
			break; 
		default: 
			step = 0;
			break;
	}

	tmp = ReadAccState();
	switch(step1)                       // 
	{
		case 0:
			if(tmp == AccStateBack)         //
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
			if(read_stopwatch16_value(&sw) < 60)    //3S 
				break;
			
			AccStateBack = tmp;
			if(tmp > 0)
				InsertLogInfo("A\r\n",sizeof("A\r\n"));
			PacketLen = BuildCompletePacket(DataPacketBuf,1);

			if(PacketLen > 0)
			{
				//SL_Print("Acc State Change:\r\n");             //
				WriteSendQueue(DataPacketBuf,PacketLen);        
			}
			
			if(pTime->TimeSec >= NextTime)
			{
				NextTime =  pTime->TimeSec + pSysCfg->TravelUpLoadTime;  //
			}
			step1 = 0;
			return;
		default:
			step1 = 0;
			break;
	}

 	tmp = (unsigned char)GetWarningValue();      //
	
	switch(step2)             //
	{
		case 0:
			if(tmp == ShellStateBack)
				break;
			init_stopwatch16(&sw1);
			step2++;
			break;
		case 1:
			if(tmp == ShellStateBack)
			{
				step2 = 0;
				break;
			}
			if(read_stopwatch16_value(&sw1) < 60)  //3S 
				break;
			ShellStateBack = tmp;
			//InsertLogInfo(unsigned char *source,unsigned short int len)
			PacketLen = BuildCompletePacket(DataPacketBuf,1);
			if(PacketLen > 0)
			{
				//SL_Print("Shell State Change:\r\n");             //
				WriteSendQueue(DataPacketBuf,PacketLen);        
			}
			//InsertLogInfo(" \r\n",siezeof());
			if(pTime->TimeSec >= NextTime)
			{
				NextTime =  pTime->TimeSec + pSysCfg->TravelUpLoadTime;  //
			}
			
			step2 = 0;
			return;
		default:
			step2 = 0;
			break;
	}
	//
	
}


/*******************************************************
**	函数名称:
**	功能描述:
********************************************************/
void ProcessRunData(void)
{
	static unsigned char step2 = 0;

	TerStateStr 	*pTerState;
	GNSSSTR			*pGnss;

	pTerState = GetTerStateDataSpace();   //
	
	if(pTerState->AccState > 0)               //
	{
		SysRunData.ACCTotal++;                 //
		SysRunData.ACCTotalHour = SysRunData.ACCTotal / 3600.0;
	}
	

	pGnss = GetGnssDataSpace();             //  
	
	switch(step2)                            //
	{


		case 0:                        		//
			if(pGnss->status != 'A')
				break;
			if(pGnss->speed < 1)
				break;
			DistancePro.StartLat = pGnss->latitude;           //
			DistancePro.StartLong	 = pGnss->longitude;      //  
			
			step2++;
			break;
		case 1:
			if(pGnss->status != 'A' || pGnss->speed < 1)
			{
				step2 = 0;	
				break;
			}
			
			DistancePro.EndLat = pGnss->latitude;                   //
			DistancePro.EndLong = pGnss->longitude ;    			//  
			DistancePro.Distance = ComputeDistance(DistancePro.StartLat,DistancePro.StartLong,  DistancePro.EndLat,DistancePro.EndLong);
			SysRunData.TotalDistance += DistancePro.Distance;   	// 
			
			DistancePro.StartLat = DistancePro.EndLat;       		//
			DistancePro.StartLong	 = DistancePro.EndLong;      	//  
		
			break;
		default:
			step2 = 1;
			break;
	
	}
	
	SaveSysRunData(0);            //  
	
}


/********************************File End************************/

