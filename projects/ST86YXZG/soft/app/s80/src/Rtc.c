
/*****************************************************************
**	FileName:Rtc.c
**	Time:
******************************************************************/

#include "include.h"




extern union task_state SysTask; 				//任务定义标志位


/********************本地全局变量*************************/

static SysTimeStr 			SysTime;            		   //

static SysTimeStr			SetTime;					   //

static unsigned char 		SleepStatus; 				   //


/*********************************************************
**	WarningValue:
**	Bit0:	
**	Bit1:	
**	Bit2:	
**	Bit3:	
**	Bit4:	
**	Bit5:	
**	Bit6:	
**	Bit7:	
**	Bit8：
**	Bit9:	
**	Bit10
**	Bit11:	
**	Bit12
**	Bit13:	
**	Bit14
**	Bit15:	
**	Bit16
**	Bit17:	
**	Bit18
**	Bit19:	
**	Bit20
*********************************************************/


static unsigned int 		WarningValue;                  // 



/*************************************************
**	函数名称:
**	功能描述:
**************************************************/

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
**	功能描述:
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
    t_tm.Hour = tmp / 3600;                    //
    t_tm.Min = tmp % 3600 / 60;                //
    t_tm.Sec = tmp % 3600 % 60;                //

    tmp = t / 86400;                           //

    for(i = 2000;i < 2100;i++)       //
    {
        if(i % 400==0 || (i % 4==0 && i % 100 != 0))
        {
            if(tmp < 366)
                break;        //
            tmp -= 366;
        }
        else
        {
            if(tmp < 365)
                break;      //
            tmp -= 365;
        }
    }

    t_tm.Year = i - 2000;   //

    for(i = 1;i <= 12;i++)       //
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
	
    t_tm.MDay = tmp + 1;   //
	
	return t_tm;
}



/**************************************************
**	函数名称:
**	功能描述:
**************************************************/

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
            rv += 366;         //
        }
        else
        {
            rv += 365;         //
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
                    rv += 29;        //
                else
                    rv += 28;        //
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




/***********************************************
**	函数名称:
**	功能描述:处理休眠时间
*********************************************/


void ProcessSysSleep(void)
{
	static unsigned char 			step;
	static unsigned int 			counter = 0;
	static unsigned int    			WakeUpTime;
	SysConfigStr 					*pCfg;
	TerStateStr						*pTerData;
	GNSSSTR 						*pGnss;   					//

	pTerData = GetTerStateDataSpace();
	//SL_Print("SLeep Status...%d,%d,%d,%d\r\n",SleepStatus,pTerData->AccState,step,counter);
	switch(step)
	{
		case 0:
			if(counter++ < 5)    //
				break;
			step++;
			counter = 0;
			SysTask.value = 0xFF;
			break;
		case 1:
			if(pTerData->AccState == 0)
			{
				if(counter++ > 300)			//
				{
					counter = 0;			//
					step++; 				//
					SaveSendQueue(); 		//
					//SL_Print("2 SLeep Status...%d,%d,%d\r\n",SleepStatus,pTerData->AccState,step);
					break;
				}
			}
			else
			{
				counter = 0;
			}
			
			if(SleepStatus == 1)
			{
				counter = 0;
				step++; 
				SL_GpioWrite(SL_GPIO_4,SL_PIN_LOW);			//关闭
				SaveSendQueue(); 
				//SL_Print("3 SLeep Status...%d,%d,%d\r\n",SleepStatus,pTerData->AccState,step);
				break;
			}
			
			break;
		case 2:
			if(counter++ < 5)  				//        
				break;
			step++;
			counter = 0;
			//SL_Print("4 SLeep Status...%d,%d,%d\r\n",SleepStatus,pTerData->AccState,step);
			SaveSysRunData(2);       			//
			break; 
		case 3:
			if(counter++ < 5)
				break;
			step++;
			counter = 0;
			SaveLocalLogFile();
			//SL_TcpipSocketClose(0);             //关闭网络
			break;
		case 4:
			if(counter++ < 5)
				break;
			if(SleepStatus == 1)
			{
				step = 7;
				break;
			}
			step++;
			counter = 0;
			break;     						//关闭外部定位模块，关闭TCP连接关闭
		case 5:
			step++;
			pGnss = GetGnssDataSpace();
			pCfg = GetSysCfgSpace();
			WakeUpTime = SysTime.TimeSec + pCfg->SleepTime - 300;
			SysTask.value = 0x00;
			SL_GpioWrite(SL_GPIO_4,SL_PIN_LOW);			//关闭
			GnssLedOff();
			GsmLedOff();
			pGnss->status = 'V';
			break;
		case 6:
			if(pTerData->AccState > 0)
				step++;
			if(WakeUpTime <= SysTime.TimeSec)
			{
				step++;
				break;
			}
			//SL_Print("1 SLeep Status ing...%d\r\n",SysTime.TimeSec);
			break;
		case 7:
			SL_Print("ST86 Close........\r\n");
			SL_Sleep(1000);
			SL_PowerDown(1); 			//
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
	static struct 			stopwatch16 sw;
	static unsigned char 	counter = 0;
	GNSSSTR 				*p_gnss;   					//
	int 					tmp;        				//
	SysTimeStr 				*pTime;
	SysRunDataStr			*pRun;
	SL_SYSTEMTIME 			TmpTime;                  //注意这个时间
	
	pTime = GetTerTimeSpace();        //单片机内部时间  ()
	
	switch(step)
	{
		case 0:
			if(pTime->VerifyBit == 0)    //
				return;
			step++;
			pRun = GetSysRunDataSpace();     //
			
			SL_Print(">Init Sys Time.......%u,%u\r\n",pRun->TimeSec,pTime->TimeSec);
			
			SysTime.TimeSec =  (pTime->TimeSec > pRun->TimeSec) ? pTime->TimeSec : pRun->TimeSec;    //
			SysTime = UnixToCalendar(SysTime.TimeSec);       	//
			SL_Print("The SysTime：%d,%d,%d-%d,%d-%d\r\n",SysTime.Year,SysTime.Mon,SysTime.MDay,SysTime.Hour,SysTime.Min,SysTime.Sec);     //输出系统时间
			SysTime.VerifyBit = 1;
			SendVerifyTimeCmd(&SysTime);            			//
			TmpTime.uYear = 2000 + SysTime.Year;				//
			TmpTime.uMonth = SysTime.Mon;
			TmpTime.uDay = SysTime.MDay;
			TmpTime.uHour = SysTime.Hour;
			TmpTime.uMinute = SysTime.Min;
			TmpTime.uSecond = SysTime.Sec;
			if(SL_SetLocalTime(&TmpTime) == TRUE)				//设置ST86模块内部RTC时间
				SL_Print(">App Init Sys Time OK.......\r\n");
			break;
		case 1:
			init_stopwatch16(&sw);
			if(SL_GetLocalTime(&TmpTime) == TRUE)   //获取ST86的内部时间
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
				SysTime.TimeSec = pTime->TimeSec;            		//
				SysTime = UnixToCalendar(SysTime.TimeSec);       	//
			}
			step++;
			break;
		case 2:
			if(read_stopwatch16_value(&sw) < 20)    //500ms
				break;
			
			step = 1;
			p_gnss = GetGnssDataSpace();                    //获取定位状态
			
			if(p_gnss->status != 'A')               		//
				return;
			
			/************************************
			 * 现在用的是ST86的内部RTC，精准度有待测量
			 * 在确保定位之后，使用BTC时间校准ST86及单片机时间
			 * 
			*************************************/

			tmp =  SysTime.TimeSec - p_gnss->gnss_time;
			//SL_Print("Time IDDDD:%d,%c\r\n",tmp,p_gnss->status);   //输出调试信息
			if(tmp >= 0 || tmp <= -20)                          //
			{
				if(counter++ > 5)           //定位65秒钟之后校验时间
				{
					SysTime.TimeSec = p_gnss->gnss_time - 5;        //
					SL_Print("Gnss Verfy Time:%d\r\n",p_gnss->gnss_time);
					SysTime = UnixToCalendar(SysTime.TimeSec);
					TmpTime.uYear = 2000 + SysTime.Year;				//
					TmpTime.uMonth = SysTime.Mon;
					TmpTime.uDay = SysTime.MDay;
					TmpTime.uHour = SysTime.Hour;
					TmpTime.uMinute = SysTime.Min;
					TmpTime.uSecond = SysTime.Sec;
					if(SL_SetLocalTime(&TmpTime) == TRUE)
					{
						//SL_Print(">App Verfy Sys Time OK.......\r\n");
					}	
					pTime->TimeSec = SysTime.TimeSec;             
					SetTime.Year = SysTime.Year;                       //
					SetTime.Mon = SysTime.Mon;
					SetTime.MDay = SysTime.MDay;
					SetTime.Hour = SysTime.Hour;
					SetTime.Min = SysTime.Min;
					SetTime.Sec = SysTime.Sec;
					SetTime.TimeSec = SysTime.TimeSec;
					SetTime.VerifyBit = 1;      //
					counter = 0;
				}
				
			}
			break;
		default:
			step = 0;
			break;
	}
}

/***************************************************
**	函数名称:
**	功能描述:
***************************************************/

SysTimeStr *GetSysTimeSpace(void)
{
	return &SysTime;
}



/************************
 **	2019-09-29,08:46:43
 **************************/





/************************
 **	2019-09-29,08:46:43
 **************************/
unsigned char GetSysTimerString(unsigned char *buf)
{
	int len;
	unsigned int tmp;
	unsigned char ch;
	len = 0;
	
	tmp = SysTime.TimeSec;

	do
	{
		ch = tmp % 10;
		tmp /= 10;
		for(int i = len;i > 0;i--)
		{
			buf[i] = buf[i - 1];
		}
		buf[0] = ch + 0x30;
		len++;
	}while(tmp != 0);
	
	buf[len] = ' ';
	len++;
	buf[len] = ' ';
	len++;
	
	return len;
}



/***************************************************
**	函数名称:
**	功能描述:
***************************************************/

unsigned char GetSysRtcString(unsigned char *buf)
{
	unsigned char len;

	len = 0;

	buf[len] = SysTime.Year / 10 + 0x30;
	len++;
	buf[len] = SysTime.Year % 10 + 0x30;  //年
	len++;
	buf[len] = SysTime.Mon / 10 + 0x30;
	len++;
	buf[len] = SysTime.Mon % 10 + 0x30;  //月
	len++;
	buf[len] = SysTime.MDay / 10 + 0x30;
	len++;
	buf[len] = SysTime.MDay % 10 + 0x30;    //日
	len++;

	buf[len] = '-';
	len++;

	buf[len] = SysTime.Hour / 10 + 0x30;
	len++;
	buf[len] = SysTime.Hour % 10 + 0x30;   //时
	len++;
	buf[len] = SysTime.Min / 10 + 0x30;
	len++;
	buf[len] = SysTime.Min % 10 + 0x30;  //分
	len++;
	buf[len] = SysTime.Sec / 10 + 0x30;
	len++;
	buf[len] = SysTime.Sec % 10 + 0x30;  //秒
	len++;

	return len;

}



/***************************************************
**	函数名称:
**	功能描述:
***************************************************/

SysTimeStr *GetSetTimeSpace(void)
{
	return &SetTime;
}



/*************************************************
**	函数名称:
**	功能描述:
*************************************************/

unsigned char ReadSleepStatus(void)
{	
	return SleepStatus;

}



/*************************************************
**	函数名称:
**	功能描述:
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



/***********************File End*************************/

