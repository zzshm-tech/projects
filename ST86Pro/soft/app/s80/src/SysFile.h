
/***********************************************
** 	FileName:
**	Time:
***********************************************/

#ifndef SYSFILE_H
#define SYSFILE_H


SysConfigStr *GetSysCfgSpace(void);
void InitSysConfig(void);
void InitSysTimer(void);

void ReadSysCfg(void);
void SaveSysCfg(void);
unsigned int FileCheckOrGreate(unsigned char *FileName, unsigned char *Pucbuff, unsigned int ulLenth,unsigned char WRSign);




#endif



/****************************File End******************************/


