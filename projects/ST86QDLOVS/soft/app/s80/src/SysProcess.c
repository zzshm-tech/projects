
/***************************************************************
**	FileName:
**	Time:
****************************************************************/


#include "include.h"

static unsigned char 		DataPacketBuf[512];

static SysRunDataStr 		SysRunData;

static DistanceProStr 		DistancePro;

static	unsigned char 		AccStateBack 			= 0;     		//

static unsigned char 		ShellStateBack 			= 0;           //

static int 					NextTime 				= 0;			//

static int					NextTime1 				= 0;    		//



/********************************************************
**	函数名称：
**	功能描述：
********************************************************/

void ResetNextTime(void)
{
	NextTime = 1;		//
	NextTime1 = 1;    	//
}



/********************************************************
**	函数名称：
**	功能描述：	
********************************************************/

SysRunDataStr *GetSysRunDataSpace(void)
{
	return &SysRunData;
}

 

/*****************************************************
**	函数名称：
**	功能描述：
*****************************************************/

void SaveSysRunData(unsigned char n)
{
	static unsigned short int counter = 0;

	SysTimeStr *pTime;
	GNSSSTR		*pGnss;
	
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
		SysRunData.ghdop_v = pGnss->ghdop_v;     								//��λ����
		SysRunData.satellite_num = pGnss->satellite_num;         	 			//ʹ����������
		SysRunData.satellite_view_num = pGnss->satellite_view_num;  			//������������
		SysRunData.status = pGnss->status;				            						//��λ״̬  
		SysRunData.TYear = pGnss->TYear;                                   		//GNSS-ʱ��-��
		SysRunData.TMon = pGnss->TMon;											//GNSS-ʱ��-��
		SysRunData.TDay = pGnss->TDay;											//GNSS-ʱ��-��
		SysRunData.THour = pGnss->THour;										//GNSS-ʱ��-ʱ
		SysRunData.TMin = pGnss->TMin;											//GNSS-ʱ��-��
		SysRunData.TSec = pGnss->TSec;                                     		//GNSS-ʱ��-��                           
		SysRunData.gnss_time = pGnss->gnss_time;              					//��λʱ��,��ʱ����2000��1��1�ţ�0ʱ0��0��Ϊ��׼��
		SysRunData.latitude_ns = pGnss->latitude_ns;           					//ά�ȱ�ʶ
		SysRunData.longitude_ew = pGnss->longitude_ew;            				//���ȱ�ʶ
		
		
		if(FileCheckOrGreate((unsigned char *)"SysRunDataFile", (unsigned char *)&SysRunData, sizeof(SysRunData),1) > 0)
		{
			SL_Print(">App:Save Sys Run Data.......%d\r\n",n);
		}
	}
		
}


/*****************************************************
**	:
**	:
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
**	
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

	if(pSysCfg->CfgFlag != 0x55 || ReadRunMode() > 0)
		return;
	
	tmp = ReadAccState();
	
	switch(step)
	{
		case 0:
			if(pTime->VerifyBit == 0)         //
				break;
			NextTime = pTime->TimeSec + pSysCfg->TravelUpLoadTime;     //
			NextTime1 = pTime->TimeSec + pSysCfg->WorkUpLoadTime;      //
			SL_Print(">App:Init UpLoadTime :%d\r\n",NextTime);         //
			step++;
			break;
		case 1:                    		 //  
			if(pTime->TimeSec >= NextTime)
			{
				if(tmp > 0)
				{
					NextTime =  pTime->TimeSec + pSysCfg->TravelUpLoadTime;  //
				}
				else
				{
					NextTime =  pTime->TimeSec + 20;  //
				}
				
				PacketLen = BuildCompletePacket(DataPacketBuf,1);
				if(PacketLen > 0)
				{
					WriteSendQueue(DataPacketBuf,PacketLen);
					//SL_Print("Write Queue Data.........\r\n");  //
					return;
				}
			}
			
			break; 
		default: 
			step = 0;
			break;
	}

	
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
			PacketLen = BuildCompletePacket(DataPacketBuf,0);
			if(PacketLen > 0)
			{
				//SL_Print("Acc State Change:\r\n");             //������Ϣ
				WriteSendQueue(DataPacketBuf,PacketLen);        
			}
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
			PacketLen = BuildCompletePacket(DataPacketBuf,0);
			if(PacketLen > 0)
			{
				//SL_Print("Shell State Change:\r\n");             //������Ϣ
				WriteSendQueue(DataPacketBuf,PacketLen);        
			}
			step2 = 0;
			break;
		default:
			step2 = 0;
			break;
	}
	//���Ǳ���
	
}


/*******************************************************
**	
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
	
	switch(step2)                           //
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

