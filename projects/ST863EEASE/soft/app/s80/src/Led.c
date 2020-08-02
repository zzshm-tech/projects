
/**********************************************
**	File Name:
**	Time:
***********************************************/
#include "include.h"


/**********************************************
**函数名称:void Init_Led(void)
**功能描述:初始化IO LedIO口
**	GPIO_2:GNSS Status;
**	GPIO_4:
**********************************************/

void Init_Led(void)
{
	SL_GpioSetDir(SL_GPIO_2, SL_GPIO_OUT);        //
	SL_GpioWrite(SL_GPIO_2, SL_PIN_LOW);          //GPS Status 
	SL_GpioSetDir(SL_GPIO_4, SL_GPIO_OUT);        //
	SL_GpioWrite(SL_GPIO_4, SL_PIN_HIGH);          //GSM Mode Run Status
}


/***********************************************
**	函数名称:void GnssLedOn(void)
**	功能描述:
************************************************/

void GnssLedOn(void)
{
	SL_GpioWrite(SL_GPIO_2, SL_PIN_HIGH); 
}


/************************************************
**	函数名称:
**	功能描述:
************************************************/

void GnssLedOff(void)
{
	SL_GpioWrite(SL_GPIO_2, SL_PIN_LOW); 
}


/************************************************
**	函数名称:
** 	功能描述:
************************************************/

static void GsmLedOn(void)
{
	SL_GpioWrite(SL_GPIO_4, SL_PIN_HIGH); 
}



/************************************************
**	函数名称:
**	功能描述:
************************************************/

static void GsmLedOff(void)
{
	SL_GpioWrite(SL_GPIO_4, SL_PIN_LOW); 
}


/************************************************
**	函数名称:
** 	功能描述:
************************************************/

void ProcessGsmLed(void)
{
	static unsigned char step = 0;
	static struct stopwatch16 sw;
	
	switch(step)
	{
		case 0:
			init_stopwatch16(&sw);
			step++;
			GsmLedOn();
			break;
		case 1:
			if(read_stopwatch16_value(&sw) < 20)
				break;
			GsmLedOff();
			init_stopwatch16(&sw);
			step++;
			break;
		case 2:
			if(read_stopwatch16_value(&sw) < 20)
				break;
			step = 0;
			break;
		default:
			break;
	}
}



/**************************************************
**	函数名称:
**	功能描述:
该函数判断GNSS定位模块有关异常，使用蓝色LED灯来测试。
**************************************************/

void ProcessGnssLed(void)
{
	static unsigned char step = 0;
	static struct stopwatch16 sw;

	static unsigned char On,Off;
	
	GNSSSTR *p;
	SysConfigStr *pCfg;
	
	p = GetGnssDataSpace();
	pCfg = GetSysCfgSpace();
	
	switch(step)
	{
		case 0:                           //
			if(p->status == 'A' && pCfg->CfgFlag != 0x55)
			{
				init_stopwatch16(&sw);
				step++;
				GnssLedOn();
				On = 2;
				Off = 5;
				break;
			}
			else if(p->status == 'A')
			{
				step = 3;
				
				break;
			}
			
			if(p->GnssModeStatus == 1)
			{
				init_stopwatch16(&sw);
				p->GnssModeStatus = 0;
				step++;
				GnssLedOn();
				On = 15;
				Off = 25;
				break;
			}
			else 
			{
				GnssLedOff();
				break;
			}
		case 1:
			if(read_stopwatch16_value(&sw) < On)
				break;
			step++;
			GnssLedOff();
			init_stopwatch16(&sw);
			break;
		case 2:
			if(read_stopwatch16_value(&sw) < Off)
				break;
			step = 0;
			break;
		case 3:
			if(p->status == 'V' || pCfg->CfgFlag != 0x55)
				step = 0;
			GnssLedOn();
			break;
		default:
			step = 0;
			break;
	}
	
	
}

/***********************File End********************/

