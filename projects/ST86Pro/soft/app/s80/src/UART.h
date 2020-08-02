
/**************************************************
**	File Name:
**	Time:	
**************************************************/

#ifndef UART_H
#define UART_H




void UART1_Init(void);
void UART2_Init(void);

int ReadComPkt(unsigned char port, unsigned char *dest, int max_destsize);
void UART1_Recv_Data(unsigned char *dest,unsigned short int data_len);
void UART2_Recv_Data(unsigned char *dest,unsigned short int data_len);






#endif


/*************************File End**************************/

