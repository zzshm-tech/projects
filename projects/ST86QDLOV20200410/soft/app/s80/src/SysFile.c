
/******************************************************************
**	FileName:
**	Time:
******************************************************************/

#include "include.h"



/*********************����ȫ�ֱ���******************************/

static SysConfigStr 					SysCfg;           //系统配置



/****************************************************************
**	函数名称：
**	功能描述：
****************************************************************/

SysConfigStr *GetSysCfgSpace(void)
{
	return &SysCfg;
}



/*****************************************************************
**	函数名称：
**	功能描述：
******************************************************************/


unsigned int FileCheckOrGreate(unsigned char *FileName, unsigned char *Pucbuff, unsigned int ulLenth,unsigned char WRSign)
{

	int ls1=0,fl_bt;
	unsigned int  WROKSign;

	fl_bt= SL_FileOpen(FileName, SL_FS_RDWR);	//
	
	if (fl_bt < 0)	//
	{	
		fl_bt = SL_FileCreate(FileName);
		if(fl_bt == 0)
			SL_Print("Creat File  %d\r\n",fl_bt);
		SL_Sleep(100);
	}


    if(WRSign == 1)
    {
		ls1=SL_FileWrite(fl_bt, Pucbuff, ulLenth);   //д�ļ�
		if (ls1==ulLenth)
		{
			WROKSign = 1;
		}
	   else
		{
			WROKSign = 0;
		}
    }
   else
    {
		ls1 = SL_FileRead(fl_bt, Pucbuff, ulLenth);
		SL_Print("The Read File.....%d\r\n",ls1);
		if (ls1 > 0)
		{
			WROKSign = ls1;
		}
	   else
		{
			WROKSign = 0;
		}
    }
    SL_Sleep(100);
	ls1=SL_FileClose(fl_bt);	
	SL_Sleep(100);
	
	
	return WROKSign;

}



/*****************************************************************
**	��������:
**	��������:
*****************************************************************/

void ReadSysCfg(void)
{
	unsigned char Flag;
	
	Flag = FileCheckOrGreate((unsigned char *)"ConfigFile",(unsigned char *)&SysCfg,sizeof(SysCfg),0);
	if(Flag > 0)
		SL_Print("Read CfgFile OK.......\r\n");
}



/****************************************************************
**	��������:
**	��������:
*****************************************************************/

void SaveSysCfg(void)
{
	unsigned char Flag;

	Flag = FileCheckOrGreate((unsigned char *)"ConfigFile",(unsigned char *)&SysCfg,sizeof(SysCfg),1);
	if(Flag > 0)
		SL_Print("Write CfgFile OK.......\r\n");
}

/*******************************************
**	
********************************************/

void InitSysTimer(void)
{
	SL_TASK 	   stSltask;

	stSltask.element[0] = SL_GetAppTaskHandle();

	SL_StartTimer(stSltask, 3, SL_TIMER_MODE_PERIODIC, SL_SecondToTicks(1)); 
	SL_StartTimer(stSltask, 4, SL_TIMER_MODE_PERIODIC, SL_MilliSecondToTicks(25));	//

}


/********************************************
**	
*********************************************/

void InitSysConfig(void)
{
	//#define CFGTEST 1
	SL_Sleep(1000);
	ReadSysCfg();   
	SL_Sleep(1000);	               														    //
	//SysCfg.CfgFlag = 0x56;                                                 				//
	//SL_Print("SysCfg.CfgFlag:%d\r\n",SysCfg.CfgFlag);
	if(SysCfg.CfgFlag != 0x55)              												// 
	{
		SL_Memset(SysCfg.TerminalID,'\0',18);   											//
		SL_Memcpy(SysCfg.ServerAddr,"gcjxgps.lovol.com",sizeof("gcjxgps.lovol.com"));      		// 
		SysCfg.ServerPort = 4001;
		//SL_Memcpy(SysCfg.TerminalID,"Z311292588",sizeof("Z311291000"));  	//
		//SL_Memcpy(SysCfg.ServerAddr,"123.127.244.154",sizeof("123.127.244.154"));      //
		//SysCfg.ServerPort = 8500;
		SysCfg.RunTime =0;             													//
		SysCfg.SleepSign = 1;          													//
		SysCfg.SleepTime = 21600;          												//
		SysCfg.TravelUpLoadTime = 60;    												//  
		SysCfg.WorkUpLoadTime = 1;      												// 
		SysCfg.DistanceUpLoad = 200;      												//
		SysCfg.AzimuthUpLoad = 360;       												// 
		SysCfg.CanProtocolNum = 0x21;       											// 
		SysCfg.UserCode = 22;                                                           //
		SysCfg.CarType = 0x02;                                                          //															//
		SL_Memcpy(SysCfg.TerApn,"CMNET",sizeof("CMNET"));		                        //			
		SysCfg.CfgFlag = 0x56;     														//
		//SaveSysCfg();                                                        
	}

	//SysCfg.SleepTime = 1200; 
	//SL_Memcpy(SysCfg.ServerAddr,"122.51.215.44",sizeof("122.51.215.44"));      		// 
	//SysCfg.ServerPort = 13011;
	//SysCfg.CanProtocolNum = 0x20;
	//SysCfg.CfgFlag = 0x55; 
	
	//SL_Memcpy(SysCfg.TerminalID,"W311190413",sizeof("W311190413"));   //
	#if 0             //挖掘机
	SysCfg.WorkUpLoadTime = 1;
	SysCfg.CarType = 0x01;
	SysCfg.TravelUpLoadTime = 60;  
	SysCfg.CanProtocolNum = 0x20;         
	#endif

	#if 0
	//SL_Memset(SysCfg.TerminalID,'\0',18);
	//SL_Memcpy(SysCfg.TerminalID,"W311291000",sizeof("W311291000"));
	SysCfg.TravelUpLoadTime = 5;  
	SysCfg.CanProtocolNum = 0x20;
	SysCfg.CarType = 0x01;
	#endif
	//SysCfg.TravelUpLoadTime = 5; 

}



/***************************File End*******************************/









