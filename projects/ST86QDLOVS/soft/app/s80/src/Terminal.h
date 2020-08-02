
/**********************************************************************
**	FileName:
**	Time:
**********************************************************************/

#ifndef TERMINAL_H
#define TERMINAL_H

#include "DataType.h"


void ProTerminalData(void);
unsigned char ProTermRecvData(void);
void SendVerifyTimeCmd(SysTimeStr *pTime);
TerStateStr	*GetTerStateDataSpace(void);
unsigned char ReadAccState(void);
void SendTerSleepCmd(void);
void SetTerCanProNum(void);
SysTimeStr *GetTerTimeSpace(void);
UpgradeStatusStr *GetUpgradeDataSpace(void);
void WriteRunMode(unsigned char n);
UpgradeStatusStr *GetUpgradeDataSpace(void);
CanDataStr *GetCanDataSpace(void);
unsigned char ReadRunMode(void);
LockCarStr *GetLockCarSpace(void);


#endif



/**********************************File End******************************/
