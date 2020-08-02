/*********************************************************************************
 **                        Edit History
 ** -------------------------------------------------------------------------
 ** DATE           NAME             DESCRIPTION
 ** 2011/05/03     Fan Xiaoyue        Create.
 *********************************************************************************/
#ifndef _SL_OS_H_
#define _SL_OS_H_
#include "sl_type.h"
#include "sl_error.h"
/****************************************************
                                   macro define
****************************************************/
#define SL_APP_TASK_PRIORITY_BEGIN               (233)
#define SL_APP_TASK_PRIORITY_END                 (244)
typedef void (*SL_TASK_ENTRY)(PVOID pParameter);
typedef struct{
	HANDLE element[4/4];
}SL_TASK;
typedef struct
{
	U32 nEventId;
	U32 nParam1;
	U32 nParam2;
	U32 nParam3;
}SL_EVENT;
typedef struct{
	HANDLE element[4/4];
}SL_SEMAPHORE;
typedef enum
{
	SL_TASK_MAX_PRIORITY = 20,		/*!< Max priority in OS */
	SL_NO_WAIT = 0,					/*!< Returns immediately */
	SL_INFINITE = 0xFFFFFFFF,		/*!< Would not return until condition arrival */
	SL_EVENT_PRI_NORMAL = 0,		/*!< Normal event */
	SL_EVENT_PRI_URGENT = 1,		/*!< Urgent event */
}SL_OPTION;
/****************************************************
                                   function
****************************************************/
#include "sl_app.h"
#define SL_CreateTask AA.SL_CreateTask_p
#define SL_GetAppTaskHandle AA.SL_GetAppTaskHandle_p
#define SL_CreateSemaphore AA.SL_CreateSemaphore_p
#define SL_DeleteSemaphore AA.SL_DeleteSemaphore_p
#define SL_TakeSemaphore AA.SL_TakeSemaphore_p
#define SL_GiveSemaphore AA.SL_GiveSemaphore_p
#define SL_SendEvents AA.SL_SendEvents_p
#define SL_GetEvent AA.SL_GetEvent_p
#define SL_CreateMutex AA.SL_CreateMutex_p
#define SL_TakeMutex AA.SL_TakeMutex_p
#define SL_GiveMutex AA.SL_GiveMutex_p
#define SL_DeleteMutex AA.SL_DeleteMutex_p
#define SL_GetCurrentTaskPriority AA.SL_GetCurrentTaskPriority_p
#define SL_SysRuntime AA.SL_SysRuntime_p
#define SL_EnterCriticalSection AA.SL_EnterCriticalSection_p
#define SL_ExitCriticalSection AA.SL_ExitCriticalSection_p
#endif
