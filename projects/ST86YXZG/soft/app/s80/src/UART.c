
/********************************************************
**	FileName:UART.c
**	Time:2017.05.23
*********************************************************/

#include "include.h"

#define endof(array)	(array + sizeof(array))


#define NUM_OF_COM  2              			//UART


/**********************************************/


static unsigned char	 	com1_new_data;				
static int		 			com1_recv_size;			
static unsigned char	 	com1_recv_buf[1024];          //
static unsigned char		*front_com1_recv_buf = com1_recv_buf;	
static unsigned char		*rear_com1_recv_buf = com1_recv_buf;	

static unsigned char	 	com2_new_data;			
static int		 			com2_recv_size;			
static unsigned char	 	com2_recv_buf[1024];          //
static unsigned char		*front_com2_recv_buf = com2_recv_buf;	
static unsigned char		*rear_com2_recv_buf = com2_recv_buf;	


/*****************************************************/

/******************************************************
**	函数名称：
**	功能描述：
******************************************************/

void UART1_Init(void)
{
	SL_UartSetBaudRate(SL_UART_1, SL_UART_BAUD_RATE_115200);
	SL_UartSetDCBConfig(SL_UART_1, SL_UART_8_DATA_BITS,SL_UART_1_STOP_BIT, SL_UART_NO_PARITY);
	SL_UartSetFlowCtrl(0,0);
	SL_UartClrRxBuffer(SL_UART_1);
}


/*********************************************************
**	函数名称：
**	功能描述：
*********************************************************/

void UART2_Init(void)
{

	SL_UartSetBaudRate(SL_UART_2, SL_UART_BAUD_RATE_9600);
	SL_UartSetDCBConfig(SL_UART_2, SL_UART_8_DATA_BITS,SL_UART_1_STOP_BIT, SL_UART_NO_PARITY);
	SL_UartSetFlowCtrl(0,0);
	SL_UartClrRxBuffer(SL_UART_2);
}


/*******************************************************
**	函数名称：
**	功能描述：
*******************************************************/

void clear_com(unsigned char port)
{
	switch (port) 
	{
		case 1:
			com1_recv_size = 0;
			front_com1_recv_buf = rear_com1_recv_buf;
			break;
		case 2:
			com2_recv_size = 0;
			front_com2_recv_buf = rear_com2_recv_buf;
			break;
		default:
			break;
	}
}


/************************************************************
**	函数名称：
**	功能描述：
************************************************************/

unsigned char new_data_in_com(unsigned char port)
{
	switch (port) 
	{
		case 1:
			if (com1_new_data == 0) 
			{
				return 0;
			}	
			com1_new_data = 0;
			return 1;
		case 2:
			if (com2_new_data == 0) 
			{
				return 0;
			}	
			com2_new_data = 0;
			return 1;
		default:
			return 0;
	}
}


/************************************************************
**	函数名称：
**	功能描述：
************************************************************/

int datasize_in_com(unsigned char port)
{
	int	 rv;
	
	switch (port) 
	{
		case 1:
			rv = com1_recv_size;
			break;
		case 2:
			rv = com2_recv_size;
			break;
		default:
			rv = 0;
	}	
	return rv;
}



/*************************************************************
**	��������:
**	��������:
*************************************************************/

int read_com_buf(unsigned char port, unsigned char *dest, int n)
{
	int	 rv = 0;
	
	switch (port) 
	{
		case 1:
			if (n > com1_recv_size)
				n = com1_recv_size;
			rv = n;
			while (n > 0) 
			{
				front_com1_recv_buf++;
				if (front_com1_recv_buf == endof(com1_recv_buf))
					front_com1_recv_buf = com1_recv_buf;
				*dest++ = *front_com1_recv_buf;
				com1_recv_size--;
				n--;
			}
			break;
		case 2:
			if (n > com2_recv_size)
				n = com2_recv_size;
			rv = n;
			while (n > 0) 
			{
				front_com2_recv_buf++;
				if (front_com2_recv_buf == endof(com2_recv_buf))
					front_com2_recv_buf = com2_recv_buf;
				*dest++ = *front_com2_recv_buf;
				com2_recv_size--;
				n--;
			}
			break;
		default:
			break;
	}	
	return rv;
}


/******************************************************************
** 	��������:
**	��������:
*******************************************************************/

int ReadComPkt(unsigned char port, unsigned char *dest, int max_destsize)
{
	unsigned char			 index;
	static unsigned char		 step[NUM_OF_COM];
	int				 recv_size;
	static struct stopwatch16	 stopwatch[NUM_OF_COM];
	
	if (port > NUM_OF_COM)
		return 0;
	index = port - 1;
	switch (step[index]) 
	{
		case 0:
			if (new_data_in_com(port) == 0)
				return 0;
			init_stopwatch16(&stopwatch[index]);
			step[index]++;
			
			return 0;
		case 1:
			if (new_data_in_com(port) == 1)
				init_stopwatch16(&stopwatch[index]);
			if (read_stopwatch16_value(&stopwatch[index]) < 2)
				return 0;
			recv_size = datasize_in_com(port);
			step[index] = 0;
			if (recv_size > max_destsize) 
			{
				clear_com(port);
				return 0;
			}
			return read_com_buf(port, dest, recv_size);
		default:
			step[index] = 0;
		return 0;
	}
}




/******************************************************************
**	��������:void UART1_Recv_Data(unsigned char *dest,unsigned short int data_len)
**	��������:UART1���շ�����
******************************************************************/

void UART1_Recv_Data(unsigned char *dest,unsigned short int data_len)
{
	unsigned char *str;
	
	str = dest;
	
	if(data_len == 0)
		return;
	com1_new_data = 1;
	while(data_len > 0)
	{
		if(rear_com1_recv_buf != front_com1_recv_buf || com1_recv_size == 0)
		{
			if(++rear_com1_recv_buf == endof(com1_recv_buf))
				rear_com1_recv_buf = com1_recv_buf;
			*rear_com1_recv_buf = *str;
			com1_recv_size++;
		}
		else 
		{
			if(++front_com1_recv_buf == endof(com1_recv_buf))
				front_com1_recv_buf = com1_recv_buf;
			com1_recv_size--;
			if(++rear_com1_recv_buf == endof(com1_recv_buf))
				rear_com1_recv_buf = com1_recv_buf;
			*rear_com1_recv_buf = *str;
			com1_recv_size++;
		}	
		str++;
		data_len--;
	}
	
}


/******************************************************************
**	��������:void UART2_Recv_Data(unsigned char *dest,unsigned short int data_len)
**	��������:���յ��������
******************************************************************/

void UART2_Recv_Data(unsigned char *dest,unsigned short int data_len)
{
	unsigned char *str;
	
	str = dest;
	
	if(data_len == 0)
		return;
	com2_new_data = 1;
	while(data_len > 0)
	{
		if(rear_com2_recv_buf != front_com2_recv_buf || com2_recv_size == 0)
		{
			if(++rear_com2_recv_buf == endof(com2_recv_buf))
				rear_com2_recv_buf = com2_recv_buf;
			*rear_com2_recv_buf = *str;
			com2_recv_size++;
		}
		else 
		{
			if(++front_com2_recv_buf == endof(com2_recv_buf))
				front_com2_recv_buf = com2_recv_buf;
			com2_recv_size--;
			if(++rear_com2_recv_buf == endof(com2_recv_buf))
				rear_com2_recv_buf = com2_recv_buf;
			*rear_com2_recv_buf = *str;
			com2_recv_size++;
		}	
		str++;
		data_len--;
	}
	
}









/*********************************File End**********************************/

