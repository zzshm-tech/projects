/*!
 *	\file sl_os.h
 *	\brief A set of OS service supplied by SL1360
 *
 *
 *	\n
 * 	<h3>OS Interfaces</h3>
 *	 Our RTOS support 255 levels tasks. GSM, GPRS and GPS have used part of them,
 *     and some of them must run as higher level task (snall number represent higher level task priority),
 *     User can use SL_Create_Task to create task with priority from \b 0 to \b 31, which represent
 *     real task priority from 0x88 to 0xA7.
 *	you can create task vy call SL_Create_Task(), create Queue by SL_Create_Queue(),
 *	create semaphore by SL_Create_Semaphore(), and create event by SL_Create_Event_Group()
 *	\n
 *	 Memory Pool: you can create memory pool by SL_Create_Memory_Pool. The maximum size for
 *	 memory pool is 96K. (The size dependent on the codes and data your application used)
 *
 *
 *	\version 		0.1
 *	\author 		SimpLight
 *	\date 		2011-04-15
 * 	\copyright
 *  Copyright (C) 2006-2011 by SimpLight Nanoelectronics, Ltd.\n
 *                 All rights reserved.\n
 * This material constitutes the trade secrets and confidential,\n
 * proprietary information of SimpLight.  This material is not to\n
 * be disclosed, reproduced, copied, or used in any manner  not\n
 * permitted under license from SimpLight Nanoelectronics Ltd.\n
 *
 *	\example ex_task.c
 *	\example ex_mempool.c
 *	\example ex_queue.c
 */
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

/*! \struct SL_TASK
 *   \brief Task control struct 
 */
typedef struct{
	HANDLE element[4/4];
}SL_TASK;

/*! \struct SL_EVENT
 *   \brief Task event struct 
 */
typedef struct
{
	U32 nEventId;
	U32 nParam1;
	U32 nParam2;
	U32 nParam3;
}SL_EVENT;

/*! \struct SL_SEMAPHORE
*   \brief Semaphore control struct
*/
typedef struct{
	HANDLE element[4/4];
}SL_SEMAPHORE;

/*! \enum SL_OPTION
*   \brief Define constants for use in service parameters 
*/
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
HANDLE SL_CreateTask(SL_TASK_ENTRY pTaskEntry, U16 usStackSize, U8 ucPriotiry, PCSTR pTaskName);

HANDLE SL_GetAppTaskHandle(VOID);

/*!
 * \fn SL_Create_Semaphore(SL_SEMAPHORE *semaphore, UINT32 initial_count)
 * \brief This service creates a counting semaphore.
 * \param[in] semaphore Semaphore control block
 * \param[in] initial_count Specifies the initial count of the semaphore.
 * \return	  The semaphore is created or not
 * \retval    SL_OK Indicates successful completion of the service
 * \retval    SL_ERR Error
 * \see SL_Obtain_Semaphore
 */
S32 SL_CreateSemaphore(SL_SEMAPHORE *semaphore, U32 initial_count);

/*!
 * \fn SL_Delete_Semaphore(SL_SEMAPHORE semaphore)
 * \brief This service deletes the semaphore specified by semaphore
 * \param[in] semaphore Semaphore control block
 * \return	  The request operation successfully or not
 * \retval    SL_OK Indicates successful completion of the service
 * \retval    SL_ERR Error
 * \see SL_Obtain_Semaphore \n SL_Create_Semaphore
 */
S32 SL_DeleteSemaphore(SL_SEMAPHORE semaphore);

/*!
 * \fn SL_Obtain_Semaphore(SL_SEMAPHORE semaphore, UINT32 timeout)
 * \brief This service obtains an instance of the specified semaphore. 
 * \details Since instances are implemented with an internal counter, obtaining a semaphore \n
 *             translates into decrementing the semaphores internal counter by one. If the semaphore \n
 *             counter is zero before this call, the service cannot be immediately satisfied.
 * \param[in] semaphore Semaphore control block
 * \param[in] timeout  The time-out value.
 * \return	  Semaphore is acquired successfully or not
 * \retval    SL_OK Indicates successful completion of the service
 * \retval    SL_ERR Error
 * \see SL_Create_Semaphore
 */
S32 SL_TakeSemaphore(SL_SEMAPHORE semaphore, U32 timeout);

/*!
 * \fn SL_STATUS SL_Release_Semaphore(SL_SEMAPHORE semaphore)
 * \brief This service releases an instance of the semaphore specified by the parameter  semaphore. 
 * \details If there are any tasks waiting to obtain the same semaphore, the first task waiting is \n 
 *            given this instance of the semaphore. Otherwise, if there are no tasks waiting for this \n
 *            semaphore, the internal semaphore counter is incremented by one.
 * \param[in] semaphore Semaphore control block
 * \return	   Semaphore is released successfully or not
 * \retval    SL_OK Indicates successful completion of the service
 * \retval    SL_ERR Error
 * \see SL_Create_Semaphore
 */
S32 SL_GiveSemaphore(SL_SEMAPHORE semaphore);

/*!
 * \fn SL_STATUS SL_Set_Events(SL_TASK hTask, SL_EVENT *pEvent, UINT32 timeout, SL_OPTION option);
 * \brief This service sets the specified event flags in the specified event.
 * \details Any task waiting on the event whose event flag request is satisfied by this service is resumed.
 * \param[in] hTask The specified task handle.
 * \param[in] pEvent Specify the event structure to retrieve the event.
 * \param[in] timeout Specified timeout value. There are two operation options available:SL_NO_WAIT and SL_INFINITE
 * \param[in] operation There are two operation options available: SL_EVENT_PRI_NORMAL and SL_EVENT_PRI_URGENT
 * \return	  Event is seted successfully or not
 * \retval    SL_OK Indicates successful completion of the service
 * \retval    SL_ERR Error
 * \see SL_Retrieve_Events
 */
S32 SL_SendEvents(SL_TASK hTask, SL_EVENT *pEvent);

/*!
 * \fn SL_Retrieve_Events(SL_TASK hTask, SL_EVENT *pEvent)
 * \brief This service retrieves the specified event-flag combination from the specified event-flag. 
 * \details If the combination is present, the service completes immediately.
 * \param[in] pEvent Specify the event structure to retrieve the event.
 * \return	   Event is successfully retrieved or not
 * \see SL_Set_Events
 */
void SL_GetEvent(SL_TASK hTask, SL_EVENT* pstEvent);

U32 SL_CreateMutex(void);
U8 SL_TakeMutex(U8 ucMutexId);
void SL_GiveMutex(U8 ucMutexId, U8 ucUserId);
void SL_DeleteMutex(U8 ucMutexId);
U32 SL_GetCurrentTaskPriority(void);

/*!
 * \fn int SL_SysRuntime(VOID)
 * \brief This service returns total seconds from system boot-up
 * \return
 *      Total seconds from system boot-up
 */
U32 SL_SysRuntime(VOID);
HANDLE SL_EnterCriticalSection(void);
BOOL SL_ExitCriticalSection(HANDLE hSection);

#endif

