/*************************************************************************
**	File Name:	
**	Time:

*******************************************************************************/
#ifndef INCLUDE_H
#define INCLUDE_H
/*********************************************************

*********************************************************/

//#define SL_EV_MMI_EV_BASE              0x01000000

#define EVTAPP_test 					0x01000100			 //应用开启编号
#define RXGPRSDATA 						(EVTAPP_test+1)		 //收到GPS数据任务编号
#define gprs_rxdata_index 				(rxgpsdata_index+1)	 //GPRS收到数据标志
#define sms_rxdata_index 				(gprs_rxdata_index+1)//收到SMS数据标志
#define SOFTWARE_VERSION_NUM   			20
/**********************************************************/

/*********************************************************
**                    S80 Lib Head file
**********************************************************/
#include "sl_os.h"
#include "sl_stdlib.h"
#include "sl_type.h"
#include "sl_debug.h"
#include "sl_error.h"
#include "sl_sms.h"
#include "sl_call.h"
#include "sl_timer.h"
#include "sl_system.h"
#include "sl_memory.h"
#include "sl_uart.h"
#include "sl_tcpip.h"
#include "sl_audio.h"
#include "sl_gpio.h"
#include "sl_lowpower.h"
#include "sl_timer.h"
#include "sl_filesystem.h"


/*****************************************************************

******************************************************************/

#include "UART.h"
#include "SysTickTime.h"
#include "GNSS.h"
#include "DataType.h"
#include "Led.h"
#include "ProData.h"
#include "Rtc.h"
#include "SysFile.h"
#include "AppEvent.h"
#include "SysFile.h"
#include "Gprs.h"
#include "SysProcess.h"
#include "Terminal.h"
#include "FiFoData.h"
#include "BuildDataPacket.h"
#include "TcpIpUpload.h"
#include "VersionNum.h"


/**************************Function*********************************/

void SL_AppSendMsg(HANDLE stTask, unsigned int ulMsgId, unsigned int ulParam);


#endif

/****************************File End*******************************/



