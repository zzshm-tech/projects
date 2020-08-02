
/*****************************************************************
**	FileName:Rtc.c
**	Time:
******************************************************************/

#include "include.h"





/************************全局变量**************************/

static SysTimeStr 			SysTime;            		   //设备时间

static SysTimeStr			SetTime;					   //设定时间

static unsigned char 		SleepStatus; 				   //系统休眠状态 


/*********************************************************
**	WarningValue:报警值说明
**	Bit0:	0 电源供电；1 电源断开报警；
**	Bit1:	0 外部电源电压正常；1 外部电源电压低报警；
**	Bit2:	0 MCU连接正常；1 MCU断开报警；
**	Bit3:	0 外壳正常；1 开壳报警
**	Bit4:	0 速度正常；1 超速报警；
**	Bit5:	0 车辆位置不越界；1 越界报警。（备用）
**	Bit6:	0 未休眠；1 休眠报警
**	Bit7:	保留
*********************************************************/


static unsigned int 		WarningValue;                  //报警值   



/****************************************************************
**	函数名称:
**	功能描述:
*****************************************************************/

unsigned int GetWarningValue(void)
{
	return WarningValue;
}


/****************************************************************
**	函数名称:
**	功能描述:
****************************************************************/

void WriteDeviceAlram(char bit,unsigned char n)
{
	if(n > 0)
		bitset(WarningValue,bit);
	else
		bitclr(WarningValue,bit);
}


/*******************************************************************
**	函数名称:
**	功能描述:时间
********************************************************************/

SysTimeStr UnixToCalendar(unsigned int t)
{
	SysTimeStr t_tm;
    unsigned int i;
    unsigned int tmp;
    unsigned int tmp1;  

	SL_Memset((unsigned char *)&t_tm,0,sizeof(t_tm));
	t_tm.TimeSec = t;
    tmp = t % 86400;
    t_tm.Hour = tmp / 3600;                    //时
    t_tm.Min = tmp % 3600 / 60;                //分
    t_tm.Sec = tmp % 3600 % 60;                //秒

    tmp = t / 86400;                           //总天数

    for(i = 2000;i < 2100;i++)       //确定那年
    {
        if(i % 400==0 || (i % 4==0 && i % 100 != 0))
        {
            if(tmp < 366)
                break;        //退出for循环
            tmp -= 366;
        }
        else
        {
            if(tmp < 365)
                break;      //退出for循环
            tmp -= 365;
        }
    }

    t_tm.Year = i - 2000;   //年

    for(i = 1;i <= 12;i++)       //确定月
    {
        switch(i)
        {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                if(tmp < 31)
                {
                    t_tm.Mon = i;
                    i = 13;
                }
                else
                {
                    tmp -= 31;
                }
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                if(tmp < 30)
                {
                    t_tm.Mon = i;
                    i = 13;
                }
                else
                {
                    tmp -= 30;
                }
                break;
            case 2:
                tmp1 = t_tm.Year + 2000;
                if(tmp1 % 400==0 || (tmp1 % 4==0 && tmp1 % 100 != 0))
                {
                    if(tmp < 29)
                    {
                        t_tm.Mon = i;
                        i = 13;
                    }
                    else
                    {
                        tmp -= 29;
                    }
                }
				else
				{
					if(tmp < 28)
					{
						t_tm.Mon = i;
						i = 13;
					}
					
					else
					{
						tmp -= 28;
					}
				}
                break;
        }
    }
	
    t_tm.MDay = tmp + 1;   //  日期
	
	return t_tm;
}



/**********************************************************************
**	函数名称:
**	功能描述:时间转时间戳
**	输入参数:
**	输出参数:
**********************************************************************/

unsigned int CalendarToUnix(SysTimeStr t)
{
    unsigned int rv;
    unsigned int tmp;
    unsigned short int i;

    rv = 0;
	tmp = t.Year + 2000;
    for(i = 2000; i < tmp;i++)
    {
        if(i % 400==0 || (i % 4==0 && i % 100 != 0))
        {
            rv += 366;         //闰年
        }
        else
        {
            rv += 365;         //非闰年
        }
    }

    tmp = i;

    for(i = 1;i < t.Mon;i++)
    {
        switch(i)
        {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                rv += 31;
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                rv += 30;
                break;
            case 2:
                if(tmp % 400==0 || (tmp % 4==0 && tmp % 100 != 0))
                    rv += 29;        //闰年
                else
                    rv += 28;        //非闰年
                break;
        }
    }

    for(i = 1;i < t.MDay;i++)
    {
        rv++;
    }

    rv *= 86400;

    rv = rv + t.Hour * 3600 + t.Min * 60 + t.Sec;

	return rv;               //
}




/******************************************************
**	函数名称:
** 	功能描述:
**	
******************************************************/


void ProcessSysSleep(void)
{
	static unsigned char step;
	static unsigned int counter = 0;
	
	//SL_Print("SLeep Status...%d\r\n",SleepStatus);

	switch(step)
	{
		case 0:
			if(counter++ < 5)    //上电
				break;
			step++;
			break;
		case 1:
			if(SleepStatus == 1)
			{
				counter = 0;
				step++;
				//保存数据  
				//SL_Print("Enter SLeep Status...\r\n");
				SaveSysRunData(2);       //电源断开之后    
				SaveSendQueue(); 
				step++;
			}
			break;
		case 2:
			if(counter++ < 10)                 //等待10秒钟，把盲区数据存储完毕 位置          
				break;
			step++;
			SL_PowerDown(1);     					//关闭     这个地方测试一下
			//关机
			break;
		case 3:
			break;
		default:
			step = 0;
			break;
	}
}


/********************************************************
**	函数名称:
**	功能描述:
*********************************************************/

void ProcessRtc(void)
{
	static unsigned char 	step = 0;
	static struct stopwatch16 sw;

	GNSSSTR 				*p_gnss;   //定义能指向邋邋GNSS数据空间的指针
	int tmp;
	SysTimeStr 				*pTime;
	SysRunDataStr			*pRun;
	SL_SYSTEMTIME 			TmpTime;
	
	pTime = GetTerTimeSpace();        //单片机时间
	
	switch(step)
	{
		case 0:
			if(pTime->VerifyBit == 0)    //还没有获取到系统时间
				return;
			step++;
			pRun = GetSysRunDataSpace();     //系统运行时间
			//SL_Print(">Init Sys Time.......%u,%u\r\n",pRun->TimeSec,pTime->TimeSec);
			SysTime.TimeSec =  (pTime->TimeSec > pRun->TimeSec) ? pTime->TimeSec : pRun->TimeSec;    //在这里初始化时间  
			SysTime = UnixToCalendar(SysTime.TimeSec);       	//刷新时间 
			SysTime.VerifyBit = 1;
			SendVerifyTimeCmd(&SysTime);            			//校准一下时间 
			TmpTime.uYear = 2000 + SysTime.Year;				//
			TmpTime.uMonth = SysTime.Mon;
			TmpTime.uDay = SysTime.MDay;
			TmpTime.uHour = SysTime.Hour;
			TmpTime.uMinute = SysTime.Min;
			TmpTime.uSecond = SysTime.Sec;
			if(SL_SetLocalTime(&TmpTime) == TRUE)	
				SL_Print(">App Init Sys Time OK.......\r\n");
			break;
		case 1:
			init_stopwatch16(&sw);
			if(SL_GetLocalTime(&TmpTime) == TRUE)   //获取ST87系统时间
			{
				//SL_Print(">The Local Time:%d,%d,%d:%d-%d-%d\r\n",TmpTime.uYear,TmpTime.uMonth,TmpTime.uDay,TmpTime.uHour,TmpTime.uMinute,TmpTime.uSecond);
				SysTime.Year = TmpTime.uYear - 2000;
				SysTime.Mon = TmpTime.uMonth;
				SysTime.MDay = TmpTime.uDay;
				SysTime.Hour = TmpTime.uHour;
				SysTime.Min = TmpTime.uMinute;
				SysTime.Sec = TmpTime.uSecond;
				SysTime.TimeSec = CalendarToUnix(SysTime);
				//SL_Print(">The Local Time:%u\r\n",SysTime.TimeSec);
			}
			else
			{
				SysTime.TimeSec = pTime->TimeSec;            		//时间运行 
				SysTime = UnixToCalendar(SysTime.TimeSec);       	//刷新时间 
			}
			step++;
			break;
		case 2:
			if(read_stopwatch16_value(&sw) < 20)    //500ms
				break;
			
			step = 1;
			break;
		default:
			step = 0;
			break;
	}
	
	p_gnss = GetGnssDataSpace();                    //比对GNSS时间
	if(p_gnss->status != 'A')               		//如果不定位，不进行时间  并且已经定位
		return;
	
	tmp = (p_gnss->gnss_time > SysTime.TimeSec) ? (p_gnss->gnss_time - SysTime.TimeSec) : (SysTime.TimeSec - p_gnss->gnss_time);

	if(tmp > 30)                          //校准时间
	{
		SysTime.TimeSec = p_gnss->gnss_time;        //GNSS的时间
		SL_Print("Gnss Verfy Time:%d\r\n",p_gnss->gnss_time);
		SysTime = UnixToCalendar(SysTime.TimeSec);
		TmpTime.uYear = 2000 + SysTime.Year;				//
		TmpTime.uMonth = SysTime.Mon;
		TmpTime.uDay = SysTime.MDay;
		TmpTime.uHour = SysTime.Hour;
		TmpTime.uMinute = SysTime.Min;
		TmpTime.uSecond = SysTime.Sec;
		if(SL_SetLocalTime(&TmpTime) == TRUE)	;
			//SL_Print(">App Verfy Sys Time OK.......\r\n");
	}

	
	tmp = (pTime->TimeSec > SysTime.TimeSec) ? (pTime->TimeSec - SysTime.TimeSec) : (SysTime.TimeSec - pTime->TimeSec);
	if(tmp > 30)
	{
		pTime->TimeSec = SysTime.TimeSec;             
		SetTime.Year = SysTime.Year;                       //年 
		SetTime.Mon = SysTime.Mon;
		SetTime.MDay = SysTime.MDay;
		SetTime.Hour = SysTime.Hour;
		SetTime.Min = SysTime.Min;
		SetTime.Sec = SysTime.Sec;
		SetTime.TimeSec = SysTime.TimeSec;
		SetTime.VerifyBit = 1;      //校准时间 
	}
}



/***************************************************
** 函数名称:
** 功能描述:
***************************************************/

SysTimeStr *GetSysTimeSpace(void)
{
	return &SysTime;
}




/***************************************************
** 函数名称:
** 功能描述:
***************************************************/

SysTimeStr *GetSetTimeSpace(void)
{
	return &SetTime;
}



/*************************************************
**	函数名称:
**	功能描述:
**	输入参数:
**	输出参数:返回休眠状态 0:不休眠，1:进入休眠，
*************************************************/

unsigned char ReadSleepStatus(void)
{	
	return SleepStatus;
}



/*************************************************
**	函数名称:
**	功能描述:
**	输入参数:
**	输出参数:返回休眠状态 0:不休眠，1:进入休眠，
*************************************************/

void WriteSleepStatus(unsigned char n)
{
	SleepStatus = n;
}


/**********************************************
**	函数名称:
**	功能描述:
***********************************************/
void SetSysTime(unsigned int Time)
{
	SysTime = UnixToCalendar(Time);
}



/****************************File End*************************/

