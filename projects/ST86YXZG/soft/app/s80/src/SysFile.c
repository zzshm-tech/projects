
/******************************************************************
**	FileName:
**	Time:
******************************************************************/

#include "include.h"



/*********************本地全局变量*************************/

static SysConfigStr 					SysCfg;           //

//本地日志文件
static struct LogStr LocalLogFile;

/*********************
 **	
**************************/

struct LogStr *GetLocalLogFileSpace(void)
{
	return &LocalLogFile;
}


/***********
***	保存日志文件
****************************/

void SaveLocalLogFile(void)
{
	unsigned char Flag;

	Flag = FileCheckOrGreate((unsigned char *)"LocalLog",(unsigned char *)&LocalLogFile,sizeof(LocalLogFile),1);
	if(Flag > 0)
		SL_Print("Save Local Log File OK.......\r\n");
}


/******************
**	读取日志文件
*************************/
void ReadLocalLogFile(void)
{
	unsigned char Flag;
	
	Flag = FileCheckOrGreate((unsigned char *)"LocalLog",(unsigned char *)&LocalLogFile,sizeof(LocalLogFile),0);
	if(Flag > 0)
		SL_Print("Read Local Log File OK.......\r\n");

}


/*****************************
**	插入日志信息
*******************************/
void InsertLogInfo(unsigned char *source,unsigned short int len)
{
	int i,j;
	unsigned char log_len;
	char buf[50];

	SL_Memset(buf,'\0',50);
	buf[0] = '#';
	len -= 1;
	log_len = GetSysRtcString(buf + 1);   //#时间戳
	SL_Memcpy(buf + log_len + 1,source,len);
	log_len += len;
	
	LocalLogFile.LogLen += log_len;
	if(LocalLogFile.LogLen > 10239)
		LocalLogFile.LogLen = 10239;

	for(i = 0;i < log_len;i++)
	{
		for(j = LocalLogFile.LogLen;j > 0;j--)
		{
			LocalLogFile.LogFile[j] = LocalLogFile.LogFile[j - 1];
		}
	}
	
	SL_Memcpy(LocalLogFile.LogFile,buf,log_len);
	
	//SL_Print(">The Log Info Len:%d\r\n",LocalLogFile.LogLen);
	LocalLogFile.LogFile[LocalLogFile.LogLen + 1] = '\0';
	//SL_Print(">The Log Data:%d\r\n",LocalLogFile.LogLen);
	SL_Print("%s\r\n",LocalLogFile.LogFile);
	//SL_MEMBLOCK(LocalLogFile.LogFile,50,16);			   //
}


/************************************************
**	函数名称:
**	功能描述:
************************************************/

SysConfigStr *GetSysCfgSpace(void)
{
	return &SysCfg;
}



/********************************************
**	函数名称:
**	功能描述:
*********************************************/


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
	SL_StartTimer(stSltask, 4, SL_TIMER_MODE_PERIODIC, SL_MilliSecondToTicks(25));	// 

}


/***************************************************************
**	函数名称:
**	功能描述:
****************************************************************/

void InitSysConfig(void)
{
	//SL_Sleep(1000);
//	#define DEBUG 0
	ReadSysCfg();   
	SL_Sleep(1000);	               														    //
	
	#ifdef DEBUG
	SysCfg.CfgFlag = 0x56;                                       	//
	#endif 

	if(SysCfg.CfgFlag != 0x55)              												//
	{
		#ifdef DEBUG
		SL_Memset(SysCfg.TerminalID,'\0',18);   //
		SL_Memcpy(SysCfg.TerminalID,"YXZ4121809272748",sizeof("YXZ4121809272748"));  	//
		SL_Memcpy(SysCfg.ServerAddr,"123.127.244.154",sizeof("123.127.244.154")); 
		SysCfg.ServerPort = 27000;
		SysCfg.CfgFlag = 0x55;
		#else
		SL_Memset(SysCfg.TerminalID,'\0',18);   //
		SL_Memcpy(SysCfg.ServerAddr,"yxgw.bcnyyun.com",sizeof("yxgw.bcnyyun.com")); 
		SysCfg.ServerPort = 26000;
		
		#endif

		SysCfg.RunTime =0;             													//
		SysCfg.SleepSign = 1;          													//
		SysCfg.SleepTime = 21600;          												//
		SysCfg.TravelUpLoadTime = 60;    												//	
		SysCfg.WorkUpLoadTime = 1;      												//
		SysCfg.DistanceUpLoad = 200;      												//
		SysCfg.AzimuthUpLoad = 360;       												//
		SysCfg.CanProtocolNum = 0x31;         											// 
		SysCfg.UserCode = 22;                                                           //
		SysCfg.CarType = 0x02;                                                          //
		SysCfg.HardWareNum = 41;
		SL_Memcpy(SysCfg.TerApn,"CMNET",sizeof("CMNET"));		                        //	
		SL_Memset(SysCfg.ICCID,'\0',25);                                                 
	}
	SysCfg.CfgFlag = 0x55;
	SysCfg.WorkUpLoadTime = 1;
	//SysCfg.
	//SL_Memcpy(SysCfg.TerminalID,"YXZ4111809201001",sizeof("YXZ4111809201001"));  	//
	//SysCfg.SleepTime = 21600;
	//SL_Memcpy(SysCfg.ServerAddr,"123.127.244.154",sizeof("123.127.244.154")); 
	//SysCfg.ServerPort = 27000;
	//SysCfg.CfgFlag = 0x55;
	//SysCfg.TravelUpLoadTime = 5;         
	#if 0
	
	SL_Memcpy(SysCfg.TerminalID,"YXZ4121809272308",sizeof("YXZ4121809272748"));  	//
	SL_Memcpy(SysCfg.ServerAddr,"123.127.244.154",sizeof("123.127.244.154")); 
	SysCfg.ServerPort = 27000;
	//SysCfg.TravelUpLoadTime = 10;      
	#endif
	
}



/***************************File End*******************************/









