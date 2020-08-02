
#include "include.h"


#define endof(array)	(array + sizeof(array))


static unsigned char 		GnssDataBuf[1000];    //解析邋邋GNSS数据缓冲空间

static GNSSSTR 				GnssData;

static unsigned char        GnssDataErro;                //

static double               TempLatitude;                //

static double 				TempLongitude;				 //

static GNSSSTR 				TempGnssData;                //

static unsigned char        TempBuf[20];                 //

static unsigned char 		GnssStatusBack = 'V';   //作为Gnss模块的定位状态


/************************************************************************************
**	函数名称:
**	功能描述:
************************************************************************************/

static unsigned char GnssDataVerfy(unsigned char *source)	
{
	unsigned char i,len,data_sum;
  	unsigned char *p;

  	p = source;

	for(i = 0;i < 90;i++)
	{
		if(*(p + i) == '*')
			break;
	}
	if(i > 90)
		return 0;
	
  	len = i;
  	data_sum = 0;
	
	for(i = 0;i < len;i++)
	{
		data_sum = data_sum ^ *p;
		p++;
	}
	p++;
	i =	AsciiToHex(*p,*(p + 1));

	if(data_sum	== i)
		return len;
	else
		return 0;
}




/*******************************************************************************
**	函数名称:
**	功能描述:
*******************************************************************************/

unsigned char GetComma(unsigned char num,unsigned char *str,unsigned char str_len)
{
	unsigned char i,j = 0;

	for(i = 0;i < str_len;i ++)
	{
		if(str[i] == ',')
			j++;
		if(j == num)
			return i + 1;	
	}

	return 0;	
}




/********************************************************************************
**	函数名称:
** 	功能描述:字符串转double型
********************************************************************************/

double StrToDouble(unsigned char *buf)
{
	double rev = 0;
	double dat = 0;
	int integer = 1;
	unsigned char *str = buf;
	
	int i;
	
	i = 0;
	while(*str != '\0')
	{
		switch(*str)
		{
			case '0':
				dat = 0;
				break;
			case '1':
				dat = 1;
				break;
			case '2':
				dat = 2;
				break;		
			case '3':
				dat = 3;
				break;
			case '4':
				dat = 4;
				break;
			case '5':
				dat = 5;
				break;
			case '6':
				dat = 6;
				break;
			case '7':
				dat = 7;
				break;
			case '8':
				dat = 8;
				break;
			case '9':
				dat = 9;
				break;
			case '.':
				dat = '.';
				break;
		}
		if(dat == '.')
		{
			integer = 0;
			i = 1;
			str ++;
			continue;
		}
		if( integer == 1 )
		{
			rev = rev * 10 + dat;
		}
		else
		{
			rev = rev + dat / (10 * i);
			i = i * 10 ;
		}
		str ++;
	}
	return rev;
}




/***********************************************************************************
**	函数名称:
**	功能描述:
************************************************************************************/
unsigned char GetDataStr(unsigned char n,unsigned char m,unsigned char *str,unsigned char *source,unsigned char str_len)
{
	unsigned char a,b;
	unsigned char *p;
	
	a = GetComma(n,str,str_len);
	b = GetComma(m,str,str_len);
	
	if(a == 0 || b == 0)
		return 0;
	
	p = str + a;
	a = b - a - 1;
	if(a > 50)
		return 0;
	
	SL_Memcpy(source,p,a);
	
	return a;
}



/******************************************************
** 	函数名称:
**	功能描述:GNSS定位数据过滤
*******************************************************/

static unsigned char GnssDataFilter(void)
{
	double tmp;
	unsigned int Tmp1;
	tmp = ComputeDistance(TempGnssData.latitude, TempGnssData.longitude, TempLatitude, TempLongitude) * 1000;

	if(tmp > 70)             //120Km/h  
	{
		Tmp1 = tmp * 10;
		//SL_Print("ComputeDistance:%d\r\n",Tmp1);
		return 0;
	}
	
	return 1;
}



/*********************************************************
**	函数名称:
**	功能描述:
**********************************************************/

unsigned char GnssDataParse(void)
{
	static unsigned char step = 0,counter = 0;
	static struct stopwatch16  sw;
	static unsigned char step1 = 0;
	
	SysTimeStr					tmp_time;               //系统时间
	unsigned short int 			m_data_len;
	unsigned char 				m_str_len;
	unsigned char 				*p;
	int 						index;

	//unsigned int tmp1 = 0,tmp2 = 0;      //调试使用
	
	m_data_len =  ReadComPkt(1,GnssDataBuf,sizeof(GnssDataBuf));   //
	
	switch(step1)            //该
	{
		case 0:
			if(TempGnssData.status == 'A')
				break;
			init_stopwatch16(&sw);
			//GnssLedOff();
			step1++;
			break;
		case 1:
			if(TempGnssData.status == 'A')
			{
				step1 = 0;
				break;
			}
			//SL_Print("Run This.......\r\n");
			if(read_stopwatch16_value(&sw) < 4800)    //     2分钟不定位，重启
				break;
			SL_GpioWrite(SL_GPIO_6, SL_PIN_LOW);       //关闭GNSS模块电源  
			SL_Print("Close GNSS Mode.......\r\n");
			init_stopwatch16(&sw);
			step1++;
			break;
		case 2:
			if(read_stopwatch16_value(&sw) < 200)    //     5秒钟
				break;
			SL_GpioWrite(SL_GPIO_6, SL_PIN_HIGH);
			SL_Print("OPen GNSS Mode.......\r\n");        //调试信息
			step1 = 0;
			break;
		default:
			break;
	}
		
	counter++;
	if(counter > 100)
	{
		TempGnssData.status = 'V';
		GnssData.status = 'V';
		GnssData.satellite_num = 0;
		GnssData.speed = 0; 
		GnssStatusBack = 'V';
		//GnssLedOff();
		counter--;
	}
	
	if(m_data_len == 0)
		return 0;

	counter = 0;
	GnssData.GnssModeStatus = 1;                    //
	TempGnssData.GnssModeStatus = 1;
	tmp_time.Year = 0;       //
	tmp_time.Mon = 0;		  //
	tmp_time.MDay = 0;        //
	tmp_time.Hour = 0;       //
	tmp_time.Min = 0;         //
	tmp_time.Sec = 0;         //
	
	index = LookForStr(GnssDataBuf,(unsigned char *)"GNRMC",m_data_len);         //
	p = GnssDataBuf + index;
	if((index > 0) && (p + 100) < endof(GnssDataBuf))     //
	{
		m_str_len = GnssDataVerfy(GnssDataBuf + index);
			
		if(m_str_len > 0)                                 								//
		{
				if(GetDataStr(2,3,p,TempBuf,m_str_len) > 0)    							//
				{
					
					if(TempBuf[0] != 'A')                                       	//
					{
						GnssData.status = 'V';
						GnssData.satellite_num = 0;
						GnssData.speed = 0; 
						TempGnssData.status = 'V';
						GnssStatusBack = 'V';
						//GnssLedOff();
					}
					else
					{
						TempGnssData.status = 'A';
					}
					
				}
				if(GetDataStr(1,2,p,TempBuf,m_str_len) > 0)    					   //
				{
					tmp_time.Hour = (TempBuf[0] - '0') * 10 + (TempBuf[1] - '0');  //
					tmp_time.Min =(TempBuf[2] - '0') * 10 + (TempBuf[3] - '0');    //
					tmp_time.Sec = (TempBuf[4] - '0') * 10 + (TempBuf[5] - '0');   //
				}
				if((index = GetDataStr(3,4,p,TempBuf,m_str_len)) > 0)    		//
				{
					TempBuf[index] = '\0';
					//SL_Print("The Gnss Latitude:%s\r\n",TempBuf);
					TempGnssData.latitude = StrToDouble(TempBuf);             // 
				}
				if(GetDataStr(4,5,p,TempBuf,m_str_len) > 0)    							//
				{
					if(TempBuf[0] == 'N')
						TempGnssData.latitude_ns = NORTH_LATITUDE;                 //维度
					else if(TempBuf[0] == 'S')
						TempGnssData.latitude_ns = SOUTH_LATITUDE;  								//
				}
				if((index = GetDataStr(5,6,p,TempBuf,m_str_len)) > 0)    		//经度
				{
					TempBuf[index] = '\0';
					//SL_Print("The Gnss longitude:%s\r\n",TempBuf);
					TempGnssData.longitude = StrToDouble(TempBuf); 
					//tmp1 = TempGnssData.longitude * 1000000;
					//SL_Print("The Gnss longitude:%d\r\n",tmp1);
				}
				if(GetDataStr(6,7,p,TempBuf,m_str_len) > 0)    							//
				{
					if(TempBuf[0] == 'E')
						TempGnssData.longitude_ew = EAST_LONGTITUDE;               //
					else if(TempBuf[0] == 'W')
						TempGnssData.longitude_ew = WEST_LONGTITUDE;  						//
				}
				if((index = GetDataStr(7,8,p,TempBuf,m_str_len)) > 0)    	  //
				{
					TempBuf[index] = '\0';
					TempGnssData.speed = (float)StrToDouble(TempBuf);         //  注意这个地方  
				}
				if((index = GetDataStr(8,9,p,TempBuf,m_str_len)) > 0)    		//
				{
					TempBuf[index] = '\0';
					TempGnssData.azimuth = (unsigned short int)StrToDouble(TempBuf);
				}
				if(GetDataStr(9,10,p,TempBuf,m_str_len) > 0)    							//
				{
					tmp_time.Year = (TempBuf[4] - '0') * 10 + (TempBuf[5] - '0');  			//
					tmp_time.Mon = (TempBuf[2] - '0') * 10 + (TempBuf[3] - '0');      //
					tmp_time.MDay= (TempBuf[0] - '0') * 10 + (TempBuf[1] - '0');      //
				}
			}
		}
		
		index = LookForStr(GnssDataBuf,(unsigned char *)"GNGGA",m_data_len);
		
		p = GnssDataBuf + index;
		
		if((index > 0) && (p + 100) < endof(GnssDataBuf))     	//
		{
			m_str_len = GnssDataVerfy(p);
			if(m_str_len > 0)                                 					//
			{
				if((index = GetDataStr(7,8,p,TempBuf,m_str_len)) > 0)    //
				{
					TempBuf[index] = '\0';
					TempGnssData.satellite_num = (unsigned char)StrToDouble(TempBuf);   //
				}
				if((index = GetDataStr(8,9,p,TempBuf,m_str_len)) > 0)    //
				{
					TempBuf[index] = '\0';
					TempGnssData.ghdop_v = (float)StrToDouble(TempBuf);  //
				}
				if((index = GetDataStr(9,10,p,TempBuf,m_str_len)) > 0)    //
				{
					TempBuf[index] = '\0';
					TempGnssData.altitude = (unsigned short int)StrToDouble(TempBuf);     //
				}
			}
		}
		
		index = LookForStr(GnssDataBuf,(unsigned char *)"GPGSV",m_data_len);  
		
		p = GnssDataBuf + index;
		
		if((index > 0) && (p + 100) < endof(GnssDataBuf))     //
		{
			m_str_len = GnssDataVerfy(p);
			if(m_str_len > 0)                                 //
			{
				if((index = GetDataStr(3,4,p,TempBuf,m_str_len)) > 0)    //
				{
					TempBuf[index] = '\0';
					TempGnssData.satellite_view_num = (unsigned char)StrToDouble(TempBuf);
				}
			}
		}
		
		index = LookForStr(GnssDataBuf,(unsigned char *)"BDGSV",m_data_len);
		p = GnssDataBuf + index;
		
		if((index > 0) && (p + 100) < endof(GnssDataBuf))     //
		{
			m_str_len = GnssDataVerfy(p);
			if(m_str_len > 0)                                 						//
			{
				if((index = GetDataStr(3,4,p,TempBuf,m_str_len)) > 0)   			 //
				{
					TempBuf[index] = '\0';
					TempGnssData.satellite_view_num += (unsigned char)StrToDouble(TempBuf);     // 
				}
			}
		}
		
		TempGnssData.TYear = tmp_time.Year;       //
		TempGnssData.TMon = tmp_time.Mon;		  //
		TempGnssData.TDay = tmp_time.MDay;        //
		TempGnssData.THour = tmp_time.Hour;       //
		TempGnssData.TMin = tmp_time.Min;         //
		TempGnssData.TSec = tmp_time.Sec;         //
		TempGnssData.latitude = ((unsigned int)(TempGnssData.latitude / 100) + (TempGnssData.latitude - (unsigned int)(TempGnssData.latitude / 100 ) * 100) / 60);
		TempGnssData.longitude = ((unsigned int)(TempGnssData.longitude / 100) + (TempGnssData.longitude - (unsigned int)(TempGnssData.longitude / 100) * 100) / 60);
		TempGnssData.gnss_time = CalendarToUnix(tmp_time) + 28800;          //
		TempGnssData.speed *= 1.85;                                         //   
		//SL_Print("\r\n");
		SL_Memcpy((void *)&GnssData,(void *)&TempGnssData,sizeof(TempGnssData));

		//tmp1 = GnssData.latitude * 1000000;
		//tmp2 = GnssData.longitude * 100000;
		
		//SL_Print("The Gnss Latitude:%d The Gnss Longitude:%d\r\n",tmp1,tmp2);
		
		if(TempGnssData.status == 'A')
		{
			//SL_Print("The GnssTimeUnix:%d,%d,%d-%d:%d:%d(%u)\r\n",GnssData.TYear,GnssData.TMon,GnssData.TDay,GnssData.THour,GnssData.TMin,GnssData.TSec,GnssData.gnss_time);
				
			//TempGnssData.latitude = ((unsigned int)(TempGnssData.latitude / 100) + (TempGnssData.latitude - (unsigned int)(TempGnssData.latitude / 100 ) * 100) / 60);
			//TempGnssData.longitude = ((unsigned int)(TempGnssData.longitude / 100) + (TempGnssData.longitude - (unsigned int)(TempGnssData.longitude / 100) * 100) / 60);
			switch(step)
			{
				case 0:                                         //
					TempLatitude  = TempGnssData.latitude;
					TempLongitude = TempGnssData.longitude;    //
					step++;
					break;
				case 1:
				case 2:
				case 3:
					if(GnssDataFilter() > 0)
					{
						TempLatitude  = TempGnssData.latitude;
						TempLongitude = TempGnssData.longitude;
						step++;
					}
					else
					{
						step = 0;
					}
					break;
				case 4:
					if(GnssDataFilter() > 0)
					{
						TempLatitude  = TempGnssData.latitude;
						TempLongitude = TempGnssData.longitude;
						GnssData.status = TempGnssData.status;
						
						if(GnssStatusBack == 'V')
						{
							GnssStatusBack = GnssData.status;
							ResetNextTime();
							SaveSysRunData(3);
						}
						GnssLedOn();
						GnssDataErro = 0;
						break;
					}
					else
					{
						if(GnssDataErro++ > 3)
							step = 0;
					}
					break;
				default:
					step = 0;
					break;
			}
		}
	return 1;
}


/******************************************************
**	函数名称:
**	功能描述:
******************************************************/

GNSSSTR *GetGnssDataSpace(void)
{
	return &GnssData;
}


/*************************File End************************/







