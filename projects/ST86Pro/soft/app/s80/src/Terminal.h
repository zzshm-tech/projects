
/**********************************************************************
**	FileName:
**	Time:
**********************************************************************/

#ifndef TERMINAL_H
#define TERMINAL_H

#include "DataType.h"



CanWheatStr *GetCanWheatSpace(void);    //Ð¡Âó»ú 
CanPaddyStr	*GetCanPaddySpace(void) ;
CanMaizeStr *GetCanMaizeSpace(void) ;
CanTractorStr *GetCanTractorSpace(void);
void ProTerminalData(void);
unsigned char ProTermRecvData(void);
void SendLockVehicleCmd(void);
void SendVerifyTimeCmd(SysTimeStr *pTime);
TerStateStr	*GetTerStateDataSpace(void);
unsigned char ReadAccState(void);
void SendTerSleepCmd(void);
void SetTerCanProNum(void);
SysTimeStr *GetTerTimeSpace(void);
UpgradeStatusStr *GetUpgradeDataSpace(void);
void WriteRunMode(unsigned char n);
UpgradeStatusStr *GetUpgradeDataSpace(void);



#endif



/**********************************File End******************************/
