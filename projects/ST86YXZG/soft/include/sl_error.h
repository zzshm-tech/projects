/*********************************************************************************
** File Name:      sl_error.h                                                *
** Author:         Wang Ya Feng                                                  *
** DATE:           2013/01/07                                                   *
** Copyright:      2013 by SimpLight Nanoelectronics,Ltd. All Rights Reserved.  *
*********************************************************************************

*********************************************************************************
** Description:    Implement for common function of API layer                   *
** Note:           None                                                         *
*********************************************************************************

*********************************************************************************
**                        Edit History                                          *
** -------------------------------------------------------------------------    *
** DATE           NAME             DESCRIPTION                                  *
** 2013/01/07    Wang Ya Feng       Create
*********************************************************************************/
#ifndef _SL_ERROR_H_
#define _SL_ERROR_H_

/*!
*  \brief Error code defined
*/
typedef enum
{
    /* common */
    SL_RET_BLOCK = 1,
    SL_RET_OK = 0,
    SL_RET_ERR = -1,
    SL_RET_PARAM_INVALID = -2,
    SL_RET_MEM_FULL = -3,
    SL_RET_OPERATION_NOT_ALLOWED = -4,
    SL_RET_NO_FREE_UTIL = -5,
    SL_RET_HANDLE_INVALID = -6,
    SL_RET_EXE_NOT_SUPPORT = -7,
    SL_RET_CMS_ERR_IN_MS = -8,
    SL_RET_UNKNOWN_ERR = -255,
    /*call */
    SL_CC_INVALID_CHAR_INDIAL = -1001,
    SL_CC_STATUS_ERR = -1002,
    SL_CC_OPERATION_NOT_ALLOWED = -1003,
    SL_CC_NETWORK_TIMEOUT = -1004,
    SL_CC_NO_DIALTONE = -1005,
    SL_CC_NO_ANSWER = -1006,
    SL_CC_NO_CARRIER = -1007,
    SL_CC_USER_BUSY = -1008,
    SL_CC_ERROR = -1009,
    /*sms*/
    SL_SMS_INVALID_PARAM = -10001,
    SL_SMS_INIT_ERR = -10002,
    SL_SMS_GET_STORAGE_INFO_ERR = -10003,
    SL_SMS_READ_ERR = -10004,
    SL_SMS_SEND_ERR = -10005,
    SL_SMS_DEL_ERR = -10006,
    SL_SMS_LIST_ERR = -10007,
    SL_SMS_SET_ERR = -10008,
    SL_SMS_INVALID_STATUS = -10009,
    SL_SMS_CHAR_CONVERT_ERR = -10010,
    SL_SMS_LENTH_INVALID = -10010,
    SL_SMS_INVALID_PDU_CHAR = -10010,
    SL_SMS_MEM_FAIL = -10011,
    SL_SMS_NULL = -10012,
    SL_SMS_INVALID_MEM_INDEX = -10013,
    SL_SMS_TEMP_FAIL = -10014,
    SL_SMS_MEM_FULL = -10015,
    SL_SMS_OPER_NOT_ALLOWED = -10016,
    SL_SMS_TMEP_FAIL = -10017,
    SL_SMS_MEM_CAP_EXCCEEDED = -10018,
    SL_SMS_SCA_ADDR_UNKNOWN = - 10019,
    SL_SMS_INVALID_ADDR_CHAR = -10020,
    SL_SMS_INVALID_PDU_PARAM = -10021,
    SL_SMS_INVALID_TXT_PARAM = -10022,
    SL_SMS_TP_VPF_NOT_SUPP = -10023,
    SL_SMS_RFQ_FAC_NOT_IMP = -10024,
    SL_SMS_GET_PARAM_ERR = -10025,
    SL_SMS_SET_PARAM_ERR = -10026,
    SL_SMS_UNKNOWN_ERR = -10027,
    /*pbk*/
    SL_PBK_INVALID_PARAM = -20001,
    SL_PBK_INIT_ERR = -20002,
    SL_PBK_GET_STORAGE_INFO_ERR = -20003,
    SL_PBK_STORAGE_ERR = -20004,
    SL_PBK_LIST_ENTRY_ERR = -20005,
    SL_PBK_WRITE_ENTRY_ERR = -20006,
    SL_PBK_DELETE_ENTRY_ERR = -20007,
    SL_PBK_UNKNOWN_ERR = -20008,
    /*tcpip*/
    SL_TCPIP_GET_GPRS_STATE_ERR = -100001,
    SL_TCPIP_SET_PDP_ERR = -100002,
    SL_TCPIP_PARAM_INVALID = -100003,
    SL_TCPIP_STATE_ERR = -100004,
    SL_TCPIP_GPRS_ATT_ERR = -100005,
    SL_TCPIP_GPRS_ACT_ERR = -100006,
    SL_TCPIP_SOCKET_INVALID = -100007,
    SL_TCPIP_SOCKET_CLOSE_ERR = -100008,
    SL_TCPIP_SOCKET_CONN_ERR = -100009,
    SL_TCPIP_SOCKET_SEND_ERR = -100010,
    SL_TCPIP_SOCKET_RECV_ERR = -100011,
    SL_TCPIP_SOCKET_BIND_ERR = -100012,
    SL_TCPIP_SOCKET_SERVER_DISCONNECT = -100013,
    SL_TCPIP_SOCKET_CANNOT_CONNECT = -100014,
    SL_TCPIP_SOCKET_RECV_DATA_NULL= -100015,
    SL_TCPIP_SOCKET_ABORT = -100016,
    /*gprs*/
    SL_GPRS_GET_STATE_ERR = -200001,
    SL_GPRS_ATT_ERR = -200002,
    SL_GPRS_SET_PDP_ERR = -200003,
    SL_GPRS_ACT_ERR = -200004,
    /*tts*/
    SL_TTS_ERR_NONE = -1000000,				//	无错误
    SL_TTS_ERR_TIME_EXPIRED = -1000001,		//	授权时间过期
    SL_TTS_ERR_LICENCE = -1000002,			//	授权错误
    SL_TTS_ERR_INPUT_PARAM = -1000003,		//	传入参数错误
    SL_TTS_ERR_TOO_MORE_TEXT = -1000004,	//	输入文本太长
    SL_TTS_ERR_NOT_INIT = -1000005,			//	引擎没有初始化，或者没有正确初始化
    SL_TTS_ERR_OPEN_DATA = -1000006,		//	打开资源数据错误
    SL_TTS_ERR_NO_INPUT = -1000007,			//	没有输入文本
    SL_TTS_ERR_MORE_TEXT = -1000008,		//	文本没有合成完毕
    SL_TTS_ERR_INPUT_MODE = -1000009,		//	输入方式错误
    SL_TTS_ERR_ENGINE_BUSY = -1000010,		//  引擎在工作中
	/*audio*/
    SL_MCI_ERR_OK = -2000000,
    SL_MCI_ERR_UNKNOWN_FORMAT = -2000001, 
    SL_MCI_ERR_BUSY = -2000002,
    SL_MCI_ERR_INVALID_PARAMETER = -2000003,
    SL_MCI_ERR_ACTION_NOT_ALLOWED = -2000004,
    SL_MCI_ERR_OUT_OF_MEMORY = -2000005,
    SL_MCI_ERR_CANNOT_OPEN_FILE = -2000006,         		           
    SL_MCI_ERR_END_OF_FILE = -2000007,	     
    SL_MCI_ERR_TERMINATED = -2000008,		   
    SL_MCI_ERR_BAD_FORMAT = -2000009,	      
    SL_MCI_ERR_INVALID_FORMAT = -2000010,   
    SL_MCI_ERR_UNKNOWN = -2000011,	    
    /*audio record*/
    SL_AUDIOREC_OK = -3000000,
    SL_AUDIOREC_ERR = -3000001,
    SL_AUDIOREC_PLAYING = -3000002,
    SL_AUDIOREC_RECORDING  = -3000003,
    SL_AUDIOREC_UNRECORD  = -3000004,
    SL_AUDIOREC_UNPLAY  = -3000005,
    SL_AUDIOREC_BUSY  = -3000006,
    SL_AUDIOREC_OVERFLOW  = -3000007,
    /*file system*/
    SL_FS_IS_DIRECTORY = -4200001,
    SL_FS_NOT_DIRECTORY = -4200002,
    SL_FS_NO_DIR_ENTRY = -4200003,
    SL_FS_OPERATION_NOT_GRANTED = -4200005,
    SL_FS_DIR_NOT_EMPTY = -4200006,
    SL_FS_FDS_MAX = -4200007,
    SL_FS_PROCESS_FILE_MAX = -4200008,
    SL_FS_FILE_EXIST = -4200009,
    SL_FS_NO_BASENAME = -4200011,
    SL_FS_BAD_FD = -4200012,
    SL_FS_NO_MORE_FILES = -4200013,
    SL_FS_UNKNOWN_FILESYSTEM = -4200016,
    SL_FS_INVALID_PARAMETER = -4200019,
    SL_FS_NOT_SUPPORT = -4200020,
    SL_FS_NO_MORE_MEMORY = -4200025,
    SL_FS_PATHNAME_PARSE_FAILED = -4200041,
    SL_FS_READ_DIR_FAILED = -4200048,
    SL_FS_NOT_REGULAR = -4200100,
    /*fm*/
    SL_FM_ERR_NO = -5000000,
    SL_FM_ERR_RESOURCE_BUSY = -5000001,
    SL_FM_ERR_ALREADY_OPENED = -5000002,
    SL_FM_ERR_RESOURCE_TIMEOUT = -5000003,
    SL_FM_ERR_BAD_PARAMETER = -5000004,
    SL_FM_ERR_NO_ITF = -5000005,
    /*bus*/
    SL_BUS_ERR_NO = -6000000,
    SL_BUS_ERR_RESOURCE_RESET = -6000001,
    SL_BUS_ERR_RESOURCE_BUSY = -6000002,
    SL_BUS_ERR_RESOURCE_TIMEOUT = -6000003,
    SL_BUS_ERR_RESOURCE_NOT_ENABLED = -6000004,
    SL_BUS_ERR_BAD_PARAMETER = -6000005,
    SL_BUS_ERR_UART_RX_OVERFLOW = -6000006,
    SL_BUS_ERR_UART_TX_OVERFLOW = -6000007,
    SL_BUS_ERR_UART_PARITY = -6000008,
    SL_BUS_ERR_UART_FRAMING = -6000009,
    SL_BUS_ERR_UART_BREAK_INT = -6000010,
    SL_BUS_ERR_TIM_RTC_NOT_VALID = -6000011,
    SL_BUS_ERR_TIM_RTC_ALARM_NOT_ENABLED = -6000012,
    SL_BUS_ERR_TIM_RTC_ALARM_NOT_DISABLED = -6000013,
    SL_BUS_ERR_COMMUNICATION_FAILED = -6000014,
    /*lbs*/
    SL_LBS_ERR_NO = -7000000,
    SL_LBS_ERR_PARAM_NOT_ENOUGH = -7000001,
    SL_LBS_ERR_PARAM_ERROR = -7000002,
    SL_LBS_ERR_NO_AUTHORITY = -7000003,
    SL_LBS_ERR_DATA_NON_EXIST = -7000004,
    SL_LBS_ERR_SERVER_ERROR = -7000005,
}SL_ERRCODE;

#endif

