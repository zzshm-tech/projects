
/**********************************************
**	File Name:
**	Time:
***********************************************/
#include "include.h"


/**********************************************
**��������:void Init_Led(void)
**��������:��ʼ��IO LedIO��
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
**	��������:void GnssLedOn(void)
**	��������:
************************************************/

void GnssLedOn(void)
{
	SL_GpioWrite(SL_GPIO_2, SL_PIN_HIGH); 
}


/************************************************
**	��������:
**	��������:
************************************************/

void GnssLedOff(void)
{
	SL_GpioWrite(SL_GPIO_2, SL_PIN_LOW); 

}


/************************************************
**	��������:
** 	��������:
************************************************/

static void GsmLedOn(void)
{

	SL_GpioWrite(SL_GPIO_4, SL_PIN_HIGH); 
}



/************************************************
**	��������:
**	��������:
************************************************/

static void GsmLedOff(void)
{
	SL_GpioWrite(SL_GPIO_4, SL_PIN_LOW); 
}


/************************************************
**	��������:
** 	��������:
************************************************/

void ProcessGsmLed(void)
{
	static unsigned char step = 0;
	static struct stopwatch16 sw;

	static unsigned short int CyclOn = 4;
	static unsigned short int CyclOff = 4;

	TcpIPSocketStr *p;

	p = GetTcpIpSocketSpce();

	if(p->GprsNetActive != 1)
	{
		GsmLedOn();
		return;
	}
	

	if(p->LinkState > 0)       //�����������ɹ�
	{
		CyclOn = 12;
		CyclOff = 100;
	}
	else
	{
		CyclOn = 12;
		CyclOff = 28;
	}
	if(p->SendState == 1)	  //�������֮��
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



/**************************************************
**	:
**	:
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

