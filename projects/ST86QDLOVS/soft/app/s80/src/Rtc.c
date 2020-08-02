
/*****************************************************************
**	FileName:Rtc.c
**	Time:
******************************************************************/

#include "include.h"





/************************ȫ�ֱ���**************************/

static SysTimeStr 			SysTime;            		   //�豸ʱ��

static SysTimeStr			SetTime;					   //�趨ʱ��

static unsigned char 		SleepStatus; 				   //ϵͳ����״̬ 


/*********************************************************
**	WarningValue:����ֵ˵��
**	Bit0:	0 ��Դ���磻1 ��Դ�Ͽ�������
**	Bit1:	0 �ⲿ��Դ��ѹ������1 �ⲿ��Դ��ѹ�ͱ�����
**	Bit2:	0 MCU����������1 MCU�Ͽ�������
**	Bit3:	0 ���������1 ���Ǳ���
**	Bit4:	0 �ٶ�������1 ���ٱ�����
**	Bit5:	0 ����λ�ò�Խ�磻1 Խ�籨���������ã�
**	Bit6:	0 δ���ߣ�1 ���߱���
**	Bit7:	����
*********************************************************/


static unsigned int 		WarningValue;                  //����ֵ   



/****************************************************************
**	��������:
**	��������:
*****************************************************************/

unsigned int GetWarningValue(void)
{
	return WarningValue;
}


/****************************************************************
**	��������:
**	��������:
****************************************************************/

void WriteDeviceAlram(char bit,unsigned char n)
{
	if(n > 0)
		bitset(WarningValue,bit);
	else
		bitclr(WarningValue,bit);
}


/*******************************************************************
**	��������:
**	��������:ʱ��
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
    t_tm.Hour = tmp / 3600;                    //ʱ
    t_tm.Min = tmp % 3600 / 60;                //��
    t_tm.Sec = tmp % 3600 % 60;                //��

    tmp = t / 86400;                           //������

    for(i = 2000;i < 2100;i++)       //ȷ������
    {
        if(i % 400==0 || (i % 4==0 && i % 100 != 0))
        {
            if(tmp < 366)
                break;        //�˳�forѭ��
            tmp -= 366;
        }
        else
        {
            if(tmp < 365)
                break;      //�˳�forѭ��
            tmp -= 365;
        }
    }

    t_tm.Year = i - 2000;   //��

    for(i = 1;i <= 12;i++)       //ȷ����
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
	
    t_tm.MDay = tmp + 1;   //  ����
	
	return t_tm;
}



/**********************************************************************
**	��������:
**	��������:ʱ��תʱ���
**	�������:
**	�������:
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
            rv += 366;         //����
        }
        else
        {
            rv += 365;         //������
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
                    rv += 29;        //����
                else
                    rv += 28;        //������
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
**	:
**	：	
******************************************************/


void ProcessSysSleep(void)
{
	static unsigned char step;
	static unsigned int counter = 0;
	
	//SL_Print("SLeep Status...%d\r\n",SleepStatus);

	switch(step)
	{

		case 0:
			if(counter++ < 5)    //
				break;
			step++;
			break;
		case 1:
			if(SleepStatus == 1)			
			{
				counter = 0;
				step++;
				SaveSendQueue();    //保存盲区数据
			}
			break;
		case 2:
			if(counter++ < 8)                 //         
				break;
			step++;
			//SL_Print("Save Blin Data........\r\n");
			SaveSysRunData(2);     //保存运行数据
			
			counter = 0;
			break;
		case 3:
			if(counter++ < 7)
				break;
			SL_Print("ST86 Close........\r\n");
			SL_Sleep(1000);           //
			SL_PowerDown(1); //关机
			step++;
			break;
		default:
			step = 0;
			break;
	}
}


/********************************************************
**	��������:
**	��������:
*********************************************************/

void ProcessRtc(void)
{

	static unsigned char 	step = 0;
	static struct 			stopwatch16 sw;
	static unsigned char 	counter = 0;
	GNSSSTR 				*p_gnss;   				//
	int 					tmp;        			//
	SysTimeStr 				*pTime;
	SysRunDataStr			*pRun;
	SL_SYSTEMTIME 			TmpTime;
	
	pTime = GetTerTimeSpace();        //
	
	switch(step)
	{
		case 0:
			if(pTime->VerifyBit == 0)    //
				return;
			step++;
			pRun = GetSysRunDataSpace();     //
			//SL_Print(">Init Sys Time.......%u,%u\r\n",pRun->TimeSec,pTime->TimeSec);
			SysTime.TimeSec =  (pTime->TimeSec > pRun->TimeSec) ? pTime->TimeSec : pRun->TimeSec;    // 
			SysTime = UnixToCalendar(SysTime.TimeSec);       	//
			SysTime.VerifyBit = 1;
			SendVerifyTimeCmd(&SysTime);            			//
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
			if(SL_GetLocalTime(&TmpTime) == TRUE)   //
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
			//SL_Print("Sys Verfy Time:%u,%u\r\n",p_gnss->gnss_time,SysTime.TimeSec);
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
** ��������:
** ��������:
***************************************************/

SysTimeStr *GetSysTimeSpace(void)
{
	return &SysTime;
}




/***************************************************
** ��������:
** ��������:
***************************************************/

SysTimeStr *GetSetTimeSpace(void)
{
	return &SetTime;
}



/*************************************************
**	��������:
**	��������:
**	�������:
**	�������:��������״̬ 0:�����ߣ�1:�������ߣ�
*************************************************/

unsigned char ReadSleepStatus(void)
{	
	return SleepStatus;

}



/*************************************************
**	��������:
**	��������:
**	�������:
**	�������:��������״̬ 0:�����ߣ�1:�������ߣ�
*************************************************/

void WriteSleepStatus(unsigned char n)
{
	SleepStatus = n;
}


/**********************************************
**	��������:
**	��������:
***********************************************/
void SetSysTime(unsigned int Time)
{
	SysTime = UnixToCalendar(Time);
}





/************************************
**	�������ƣ�
**	����������
*************************************/
/*
void ShowSysTime(void)
{
	static unsigned char 	step = 0;
	static struct stopwatch16 sw;

	SysTimeStr 				*pTime;
	pTime = GetTerTimeSpace();        //��Ƭ��ʱ��
	
	switch(step)
	{
		case 0:
			init_stopwatch16(&sw);
			step++;
			break;
		case 1:
			if(read_stopwatch16_value(&sw) < 40)    //500ms
				break;
			step = 0;
			SL_Print(">The Dev Time:%d,%d\r\n",pTime->TimeSec,SysTime.TimeSec);
			break;
		default:
			step = 0;
			break;
	}
}
*/
/****************************File End*************************/

