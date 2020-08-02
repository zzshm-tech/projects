
/*****************************************************
**	FileName:
**	Time:
*******************************************************/

#include "include.h"



/*********************本地全局变量****************************/

static SysConfigStr 					SysCfg;           //	



/****************************************************************
**	函数名称:
**	功能描述:
****************************************************************/

SysConfigStr *GetSysCfgSpace(void)
{
	return &SysCfg;
}



/*****************************************************
**	函数名称:
**	功能描述:
******************************************************/


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
		ls1=SL_FileWrite(fl_bt, Pucbuff, ulLenth);   //
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
		SL_Print(">App:The Read File.....%d\r\n",ls1);
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
**	函数名称:
**	功能描述�?
*****************************************************************/

void ReadSysCfg(void)
{
	unsigned char Flag;
	
	Flag = FileCheckOrGreate((unsigned char *)"ConfigFile",(unsigned char *)&SysCfg,sizeof(SysCfg),0);
	if(Flag > 0)
		SL_Print("Read CfgFile OK.......\r\n");
}



/****************************************************************
**	函数名称:
**	功能描述:
*****************************************************************/

void SaveSysCfg(void)
{
	unsigned char Flag;

	Flag = FileCheckOrGreate((unsigned char *)"ConfigFile",(unsigned char *)&SysCfg,sizeof(SysCfg),1);
	if(Flag > 0)
		SL_Print("Write CfgFile OK.......\r\n");
}

/********************************************************
**	函数名称:
**	功能描述:
********************************************************/

void InitSysTimer(void)
{
	SL_TASK 	   stSltask;

	stSltask.element[0] = SL_GetAppTaskHandle();

	SL_StartTimer(stSltask, 3, SL_TIMER_MODE_PERIODIC, SL_SecondToTicks(1)); 
	SL_StartTimer(stSltask, 4, SL_TIMER_MODE_PERIODIC, SL_MilliSecondToTicks(25));	//

}


/***************************************************************
**	函数名称:
**	功能描述:
****************************************************************/

void InitSysConfig(void)
{

	SL_Sleep(1000);
	ReadSysCfg();   
	SL_Sleep(1000);	               														    //
	//SysCfg.CfgFlag = 0x56;                                                               	//
	if(SysCfg.CfgFlag != 0x55)              												//
	{
		SL_Memset(SysCfg.TerminalID,'\0',18);   //
		//SL_Memcpy(SysCfg.TerminalID,"YX14141808100002",sizeof("YX14141808100002"));  	//
		//SL_Memcpy(SysCfg.ServerAddr,"123.127.244.154",sizeof("123.127.244.154")); 
		//SysCfg.ServerPort = 8502;
		
		SL_Memcpy(SysCfg.ServerAddr,"was.ym.bcnyyun.com",sizeof("was.ym.bcnyyun.com")); 
		SysCfg.ServerPort = 26000;
		SysCfg.RunTime =0;             													//
		SysCfg.SleepSign = 1;          														//
		SysCfg.SleepTime = 21600;          													//
		SysCfg.TravelUpLoadTime = 60;    													//
		SysCfg.WorkUpLoadTime = 1;      													//
		SysCfg.DistanceUpLoad = 200;      													//
		SysCfg.AzimuthUpLoad = 360;       													// 
		SysCfg.CanProtocolNum = 0x40;         											// 
		SysCfg.UserCode = 32;                                                               //
		SysCfg.CarType = 0x0A;                                                              //
		SysCfg.HardWareNum = 41;
		SL_Memcpy(SysCfg.TerApn,"CMNET",sizeof("CMNET"));		                            //	
		SL_Memset(SysCfg.ICCID,'\0',25);
		SysCfg.CfgFlag = 0x56;
		//SysCfg.CfgFlag = 0x55;
		//SaveSysCfg();                                                        
	}
	//SysCfg.CfgFlag = 0x55; 
	SysCfg.WorkUpLoadTime = 1;
	//SysCfg.TravelUpLoadTime = 120; 
	//SysCfg.SleepTime = 21600; 
	#if 0

	SL_Memcpy(SysCfg.ServerAddr,"was.ym.bcnyyun.com",sizeof("was.ym.bcnyyun.com")); 
	SysCfg.ServerPort = 26000;
	SysCfg.TravelUpLoadTime = 5;
	#endif

	

}



/***************************File End*******************************/









