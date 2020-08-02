
/************************************************
**	FileName:
**	Time:
*************************************************/

#ifndef GPRS_H
#define GPRS_H

#define APP_CODE_MAXLEN     (310*1024)



void InitTcpIp(void);
TcpIPSocketStr *GetTcpIpSocketSpce(void);
void ProcessGprsRecv(void);
void ProcessSocketClient(void);
unsigned char AppSendTcpIpSocket(unsigned char *buf,unsigned short int len);
unsigned char ReadSendState(void);
void ReadIpAddrBack(void);
void ProcessCmd(void);



#endif



/**********************File End*********************/
