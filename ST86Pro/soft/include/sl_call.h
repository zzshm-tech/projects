/*********************************************************************************
 ** File Name:     sl_call.h                                                *
 ** Author:         Wang Ya Feng                                                  *
 ** DATE:           2013/01/09                                                   *
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
 ** 2013/01/09     Wang Ya Feng       Create
 *********************************************************************************/
#ifndef _SL_CALL_H_
#define _SL_CALL_H_

#include "sl_type.h"


/*************************
           type define
*************************/
typedef void (*SL_CALL_DIAL)(S32 slResult); 
typedef void (*SL_CALL_RING)(U8* pucComingNum);
typedef void (*SL_CALL_HANGUP)(S32 slResult);
typedef void (*SL_CALL_RELEASE)(S32 slResult);
typedef void (*SL_CALL_ACCEPT)(S32 slResult);

typedef enum{
    SL_CALL_VOICECALL,
    SL_CALL_GPRSCALL,
    SL_CALL_INVALID
}SL_CALL_TYPE;

typedef enum
{
	SL_CC_STATE_NULL 		= 0x00,
	SL_CC_STATE_ACTIVE   	= 0x01,   
	SL_CC_STATE_HOLD 		= 0x02,   
	SL_CC_STATE_WAITING 	= 0x04,   
	SL_CC_STATE_INCOMING 	= 0x08,   
	SL_CC_STATE_DIALING 	= 0x10,   
	SL_CC_STATE_ALERTLING 	= 0x20,   
	SL_CC_STATE_RELEASE 	= 0x40, 
}SL_CALL_STATUS;

typedef enum
{
    SL_ACTIVE_STATE_READY,
    SL_ACTIVE_STATE_UNAVAILABLE,
    SL_ACTIVE_STATE_UNKNOWN,
    SL_ACTIVE_STATE_RING,
    SL_ACTIVE_STATE_CALL_IN_PROGRESS,
    SL_ACTIVE_STATE_SLEEP
}SL_CALL_ACTIVE_STATE;

typedef struct{
    SL_CALL_DIAL pstSlCallDial;
    SL_CALL_RING pstSlCallRing;
    SL_CALL_HANGUP pstSlCallHangup;
    SL_CALL_RELEASE pstSlCallRelease;
    SL_CALL_ACCEPT pstSlCallAccept;
}SL_CALL_CALLBACK;

/*************************
           function
*************************/
S32 SL_CallInit(SL_CALL_CALLBACK* cb);
S32 SL_CallDial(SL_CALL_TYPE enCallType, char* callNo);
S32 SL_CallAnswer(void);
S32 SL_CallHangup(void);
SL_CALL_ACTIVE_STATE SL_CallGetActiveStatus(void);

#endif
