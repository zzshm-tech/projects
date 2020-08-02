
/*****************************************************************
**	FileName:Rtc.c
**	Time:
******************************************************************/

#include "include.h"


/************************全局变量**************************/

static SysTimeStr 			SysTime;             //设备时间

static SysTimeStr			SetTime;

static unsigned char 		SleepStatus; 				   //系统休眠状态
     
/*****************************************************************
**	函数名称:void CalculateWeek(SysTimeStr  *t)
**	功能描述:根据日期时间计算出周
**	输入参数:
**	输出参数:无
*****************************************************************/

void CalculateWeek(SysTimeStr  *t)
{
	short int c,y,m,w,d;
	unsigned short int tm_year;

	tm_year = 2000 + t->Year;
   	c = tm_year / 100;
	y = tm_year % 100;
	m = t->Mon;
	if(m == 1)
	{
		c = (tm_year - 1) / 100;
		y = (tm_year - 1) % 100;
		m = 13;
	}
	if(m == 2)
	{
		c = (tm_year - 1) / 100;
		y = (tm_year - 1) % 100;
		m = 14;
	}
	d = t->MDay;
	w = y + (y / 4) + (c / 4) - 2 * c + (26 * (m + 1) / 10) + d - 1;
	if(w < 0)
		w =((w % 7) + 7);
	t->WDay = w % 7;

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
	
    tmp = t % 86400;
    t_tm.Hour = tmp / 3600;                    //时
    t_tm.Min = tmp % 3600 / 60;                //分
    t_tm.Sec = tmp % 3600 % 60;                //秒

    tmp = t / 86400;                           //总天数

    for(i = 2000;i < 2255;i++)       //确定那年
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
                    if(tmp1 < 29)
                    {
                        t_tm.Mon = i;
                        i = 13;
                    }
                    else
                    {
                        tmp -= 29;
                    }
                }
                break;
        }
    }
	
    t_tm.MDay = tmp + 1;   //  日期
	CalculateWeek(&t_tm);
	t_tm.TimeSec = t;
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
    for(i = 0; i < t.Year;i++)
    {
        tmp = i + 2000;
        if(tmp % 400==0 || (tmp % 4==0 && tmp % 100 != 0))
        {
            rv += 366;         //闰年
        }
        else
        {
            rv += 365;         //非闰年
        }
    }

    tmp = i + 2000;

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
******************************************************/

void ProcessSysSleep(void)
{
	static unsigned char step;
	static unsigned int counter = 0;
	unsigned char tmp;
	SysConfigStr *p;
	
	p = GetSysCfgSpace();

	tmp = ReadAccState();
	
	switch(step)
	{
		case 0:
			if(tmp > 0)
				break;
			SleepStatus = 1;  //进入休眠 
			counter = 0;
			step++;
			break;
		case 1:
			counter++;
			if(tmp > 0)
			{
				SleepStatus = 0;   //进入正常 
				step = 0;
				break;
			}
			if(counter >= p->RunTime)
			{
				SleepStatus = 2;
				SL_Print("Enter Sleep Mode");
				SaveSendQueue();							
				SL_GpioWrite(SL_GPIO_4, SL_PIN_LOW);       //关闭GNSS模块电源  
				step++;
				counter = 0;
				break;
			}
			break;
		case 2:
			if(counter++ < 10)                 //等待10秒钟，把盲区数据存储完毕 位置          
				break;
			step++;
			SendTerSleepCmd();     					//指令发送休眠   
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
	GNSSSTR 				*p_gnss;   //定义能指向邋邋GNSS数据空间的指针
	int tmp;
	SysTimeStr 				*pTime;
	SysRunDataStr			*pRun;
	
	pTime = GetTerTimeSpace();
	pRun = GetSysRunDataSpace();
	
	switch(step)
	{
		case 0:
			if(pTime->VerifyBit == 0)    //还没有获取到系统时间
				return;
			step++;
			SysTime.VerifyBit = 1;
			SysTime.TimeSec =  (pTime->TimeSec > pRun->TimeSec) ? pTime->TimeSec : pRun->TimeSec;    //在这里初始化时间  
			SysTime = UnixToCalendar(SysTime.TimeSec);       	//刷新时间 
			SendVerifyTimeCmd(&SysTime);            			//校准一下时间
			SL_Print(">App Init Sys Time :%d,%d,%d,%d\r\n",pTime->VerifyBit,pRun->TimeSec,pTime->TimeSec,SysTime.TimeSec);  //调试信息
			break;
		case 1:
			SysTime.TimeSec = pTime->TimeSec;            		//时间运行 
			SysTime = UnixToCalendar(SysTime.TimeSec);       	//刷新时间 
			
			break;
		default:
			step = 0;
			break;
	}
	
	p_gnss = GetGnssDataSpace();                    //比对GNSS时间
	if(p_gnss->status != 'A')               		//如果不定位，不进行时间  并且已经定位
		return;
	
	tmp = (p_gnss->gnss_time > SysTime.TimeSec) ? (p_gnss->gnss_time - SysTime.TimeSec) : (SysTime.TimeSec - p_gnss->gnss_time);

	if(tmp > 3)                          //校准时间
	{
		SysTime.TimeSec = p_gnss->gnss_time;
		
		SysTime = UnixToCalendar(SysTime.TimeSec);
	}

	
	tmp = (pTime->TimeSec > SysTime.TimeSec) ? (pTime->TimeSec - SysTime.TimeSec) : (SysTime.TimeSec - pTime->TimeSec);
	if(tmp > 3)
	{
		pTime->TimeSec = SysTime.TimeSec;
		SetTime.Year = SysTime.Year;                       //年 
		SetTime.Mon = SysTime.Mon;
		SetTime.MDay = SysTime.MDay;
		SetTime.Hour = SysTime.Hour;
		SetTime.Min = SysTime.Min;
		SetTime.Sec = SysTime.Sec;
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


/**********************************************
**	函数名称:
**	功能描述:
***********************************************/
void SetSysTime(unsigned int Time)
{
	SysTime = UnixToCalendar(Time);
}



/****************************File End*************************/

