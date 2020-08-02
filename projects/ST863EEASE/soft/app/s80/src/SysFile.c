
/******************************************************************
**	FileName:
**	Time:
******************************************************************/

#include "include.h"



/*********************本地全局变量******************************/

static SysConfigStr 					SysCfg;           //设备配置信息



/****************************************************************
**	函数名称:
**	功能描述:
****************************************************************/

SysConfigStr *GetSysCfgSpace(void)
{
	return &SysCfg;
}



/*****************************************************************
**	函数名称:
**	功能描述:返回文件长度
******************************************************************/


unsigned int FileCheckOrGreate(unsigned char *FileName, unsigned char *Pucbuff, unsigned int ulLenth,unsigned char WRSign)
{

	int ls1=0,fl_bt;
	unsigned int  WROKSign;

	fl_bt= SL_FileOpen(FileName, SL_FS_RDWR);	//读写方式
	
	if (fl_bt < 0)	//原来没有
	{	
		fl_bt = SL_FileCreate(FileName);
		if(fl_bt == 0)
			SL_Print("Creat File  %d\r\n",fl_bt);
		SL_Sleep(100);
	}


    if(WRSign == 1)
    {
		ls1=SL_FileWrite(fl_bt, Pucbuff, ulLenth);   //写文件
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
**	函数名称:
**	功能描述:
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
	SL_StartTimer(stSltask, 4, SL_TIMER_MODE_PERIODIC, SL_MilliSecondToTicks(25));	//两个定时器 

}


/***************************************************************
**	函数名称:
**	功能描述:
****************************************************************/

void InitSysConfig(void)
{
	SL_Sleep(1000);
	ReadSysCfg();   
	SL_Sleep(1000);	               														    //延时一段时间
	SysCfg.CfgFlag = 0x56;                                                               	//
	//SL_Print("SysCfg.CfgFlag:%d\r\n",SysCfg.CfgFlag);
	if(SysCfg.CfgFlag != 0x55)              												//如果没有配置过  回复出厂设置
	{
		//SL_Memset(SysCfg.TerminalID,'0',18);   //
		SL_Memcpy(SysCfg.TerminalID,"LOV1231703600519",sizeof("LOV1231703600519"));  		//
		
		SL_Memcpy(SysCfg.ServerAddr,"njgps-bc.lovol.com",sizeof("njgps-bc.lovol.com"));    //服务器地址
		SysCfg.ServerPort = 15000;
		SysCfg.RunTime =0;             														//休眠前运行时间
		SysCfg.SleepSign = 1;          														//休眠标识 
		SysCfg.SleepTime = 43200;          													//休眠时间
		SysCfg.TravelUpLoadTime = 20;    													//车辆行驶时数据上传时间间隔  单位秒	
		SysCfg.WorkUpLoadTime = 1;      													//耕种时上传数据间隔时间 
		SysCfg.DistanceUpLoad = 200;      													//定距数据上报
		SysCfg.AzimuthUpLoad = 360;       													//航向角数据上报 
		SysCfg.CanProtocolNum = 0x09;       												//协议号  
		SysCfg.UserCode = 22;                                                               //用户编码，福田   0x22
		SysCfg.CarType = 0x01;                                                              //车类型:小型挖掘机:0x01															//
		SL_Memcpy(SysCfg.TerApn,"CMNET",sizeof("CMNET"));		                            //			
		//SysCfg.ConLockCar = 0;
		//SysCfg.SaveBit = 0;  	//读写标识
		//SysCfg.CfgFlag = 0x55;
		//SaveSysCfg();                                                        
	}
	//SysCfg.CfgFlag = 0x55; 
	SysCfg.WorkUpLoadTime = 1;
	//SysCfg.TravelUpLoadTime = 120;  
}



/***************************File End*******************************/









