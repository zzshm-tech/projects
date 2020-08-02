
/***************************************************************
**	FileName:
**	Time:
****************************************************************/


#include "include.h"

static unsigned char 		DataPacketBuf[512];

static SysRunDataStr 		SysRunData;

static DistanceProStr 		DistancePro;

static	unsigned char 		AccStateBack 			= 0;     		//

static unsigned char 		ShellStateBack 			= 0;         

static int 					NextTime 				= 0;			//

static int					NextTime1 				= 0;    		//



/********************************************************
**	函数名称:
**	功能描述:
**	
********************************************************/

void ResetNextTime(void)
{
	NextTime = 1;		//
	NextTime1 = 1;    	//
}



/********************************************************
**	函数名称:
**	功能描述:
**	
********************************************************/

SysRunDataStr *GetSysRunDataSpace(void)
{
	return &SysRunData;
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
	//SL_Print("Save Sys Run:%d\r\n",counter);
	if(counter++ >= 900)			  //û15���ӱ���һ��  ��Լ15����
	{	
		counter = 0;
		pTime = GetSysTimeSpace();
		SysRunData.TimeSec = pTime->TimeSec;	  								//

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
		SysRunData.TYear = pGnss->TYear;                                   		//GNSS-
		SysRunData.TMon = pGnss->TMon;											//GNSS-
		SysRunData.TDay = pGnss->TDay;											//GNSS-
		SysRunData.THour = pGnss->THour;										//GNSS-
		SysRunData.TMin = pGnss->TMin;											//GNSS-
		SysRunData.TSec = pGnss->TSec;                                     		//GNSS-                          
		SysRunData.gnss_time = pGnss->gnss_time;              					//
		SysRunData.latitude_ns = pGnss->latitude_ns;           					//
		SysRunData.longitude_ew = pGnss->longitude_ew;            				//
		
		
		if(FileCheckOrGreate((unsigned char *)"SysRunDataFile", (unsigned char *)&SysRunData, sizeof(SysRunData),1) > 0)
		{
			//SL_Print(">App:Save Sys Run Data.......%d\r\n",n);
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
		
		SL_Print(">App RunData:%d",SysRunData.TimeSec);
	}
}

/**********************************************************
**	功能描述:
**	函数名称:
***********************************************************/

void SysProcess(void)
{

	static unsigned char 		step = 0,step1 = 0,step2 = 0;
	static struct stopwatch16   sw,sw1;
	unsigned char 				tmp = 0;
	unsigned short int 			PacketLen = 0;
	SysConfigStr 				*pSysCfg;

	SysTimeStr					*pTime;             //
	
	pSysCfg = GetSysCfgSpace();						//
	pTime = GetSysTimeSpace();                      //

	tmp = ReadWorkState();              							 //
	
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
		case 1:                    		 //  
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
			
			if(tmp > 0)                   //
				step++;
			break;
		case 2:                   		//   
			if(tmp == 0)                              //
			{
				PacketLen = BuildPositionPacket(DataPacketBuf,1);  //
				if(PacketLen > 0)
				{	
					WriteSendQueue(DataPacketBuf,PacketLen);   //
					step = 1;
					return;
				}
			}
			
			if(pTime->TimeSec >= (NextTime - 1))
			{	
				NextTime1 = pTime->TimeSec + pSysCfg->WorkUpLoadTime;
				PacketLen = BuildPositionPacket(DataPacketBuf,1);         	//
				if(PacketLen > 0)
				{
					WriteSendQueue(DataPacketBuf,PacketLen);   				//
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
				WriteSendQueue(DataPacketBuf,PacketLen);   					//
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
			if(read_stopwatch16_value(&sw) < 60)    //
				break;
			
			AccStateBack = tmp;
			ResetNextTime();
			step1 = 0;
			break;
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
			if(read_stopwatch16_value(&sw1) < 60)
				break;
			ShellStateBack = tmp;
			ResetNextTime();
			step2 = 0;
			break;
		default:
			step2 = 0;
			break;
	}
	//
	
}


/***********************************************
**	函数名称�?
**	功能描述�?
************************************************/
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


/**********************File End************************/

