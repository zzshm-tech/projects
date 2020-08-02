
/**********************************************
**	File Name:
**	Time:
***********************************************/
#include "include.h"


/**********************************************
**函数名称:void Init_Led(void)
**功能描述:初始化IO LedIO口
**********************************************/

void Init_Led(void)
{
	SL_GpioSetDir(SL_GPIO_3, SL_GPIO_OUT);        //GSM Mode Run Status
	SL_GpioWrite(SL_GPIO_3, SL_PIN_LOW);          //
	SL_GpioSetDir(SL_GPIO_1, SL_GPIO_OUT);        //GPS Status 
	SL_GpioWrite(SL_GPIO_1, SL_PIN_LOW);          //
}


/***********************************************
**	函数名称:
**	功能描述:
************************************************/

void GnssLedOn(void)
{
	SL_GpioWrite(SL_GPIO_1, SL_PIN_HIGH); 
}


/************************************************
**	函数名称:
**	功能描述:
************************************************/

void GnssLedOff(void)
{
	SL_GpioWrite(SL_GPIO_1, SL_PIN_LOW); 
}


/************************************************
**	函数名称:
** 	功能描述:
************************************************/

static void GsmLedOn(void)
{
	SL_GpioWrite(SL_GPIO_3, SL_PIN_HIGH); 
}



/************************************************
**	函数名称:
**	功能描述:
************************************************/

static void GsmLedOff(void)
{
	SL_GpioWrite(SL_GPIO_3, SL_PIN_LOW); 
}


/************************************************
**	函数名称:
** 	功能描述:
************************************************/

/*
void ProcessGsmLed(void)
{
	static unsigned char step = 0;
	static struct stopwatch16 sw;

	static unsigned short int CyclOn = 4;
	static unsigned short int CyclOff = 4;

	TcpIPSocketStr *p;

	p = GetTcpIpSocketSpce();

	if(p->LinkState > 0)       //如果连接网络成功
	{
		CyclOn = 12;
		CyclOff = 100;
	}
	else
	{
		CyclOn = 12;
		CyclOff = 28;
	}
	if(p->SendState == 1)	  //发送完成之后
	{
		CyclOn = 2;
		CyclOff = 5;
	}
	if(p->FtpLoadFig == 1)
	{
		CyclOn = 5;
		CyclOff = 5;
	}
	
	switch(step)
	{
		case 0:
			init_stopwatch16(&sw);
			step++;
			GsmLedOn();
			break;
		case 1:
			if(read_stopwatch16_value(&sw) < CyclOn)
				break;
			GsmLedOff();
			init_stopwatch16(&sw);
			step++;
			break;
		case 2:
			if(read_stopwatch16_value(&sw) < CyclOff)
				break;
			step = 0;
			break;
		default:
			break;
	}
}

*/


/*******************************************************
**	函数名称:
**	功能描述:
*******************************************************/

void ProcessGsmLed(void)
{
	static struct stopwatch16    sw;
	static unsigned char         step = 0;

	switch(step)
	{
		case 0:
			init_stopwatch16(&sw);
			GsmLedOn();
			//SL_Print(">App:Run This...........1\r\n");
			step++;
			break;
		case 1:
			if(read_stopwatch16_value(&sw) < 10)
				break;
			GsmLedOff();
			init_stopwatch16(&sw);
			//SL_Print(">App:Run This...........1\r\n");
			step++;
			break;
		case 2:
			if(read_stopwatch16_value(&sw) < 30)
				break;
			step = 0;
			break;
		default:
			break;
	}
}


/***********************File End********************/

