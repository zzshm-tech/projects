/*********************************************************************************
** File Name:      sl_lowpower.h                                                *
** Author:         David                                                  *
** DATE:           2013/03/14                                                   *
** Copyright:      2013 by SimpLight Nanoelectronics,Ltd. All Rights Reserved.  *
*********************************************************************************
*********************************************************************************
** Description:    Implement for common function of API layer                   *
** Note:           None                                                         *
*********************************************************************************
*********************************************************************************
**                        Edit History                                          *
** -------------------------------------------------------------------------    *
** DATE                   NAME             DESCRIPTION                                  *
** 2013/03/14             David            Create
*********************************************************************************/
#ifndef _SL_LOWPOWER_H_
#define _SL_LOWPOWER_H_
#include "sl_type.h"
typedef	enum {
	SL_LPWR_DSLEEP,
	SL_LPWR_WAKEUP
} SL_LPWR_STATUS_t;
typedef enum {
	SL_WKUP_UART_1,
	SL_WKUP_UART_2,
	SL_WKUP_UART_ALL,
	SL_WKUP_NONE
} SL_WAKEUP_TYPE_t;
#include "sl_app.h"
#define SL_LpwrEnterDSleep AA.SL_LpwrEnterDSleep_p
#define SL_LpwrEnterWakeup AA.SL_LpwrEnterWakeup_p
#define SL_LpwrGetCurrSt AA.SL_LpwrGetCurrSt_p
#endif
