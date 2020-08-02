/*********************************************************************************
** File Name:     sl_uart.h                                                *
** Author:         Wang Ya Feng                                                  *
** DATE:           2013/01/17                                                   *
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
** 2013/01/17     Wang Ya Feng       Create
*********************************************************************************/
#ifndef _SL_UART_H_
#define _SL_UART_H_
#include "sl_type.h"
/******************************************
                      type define
******************************************/
typedef enum _SL_UART_BAUDRATE
{
  SL_UART_BAUD_RATE_2400 = 2400,
  SL_UART_BAUD_RATE_4800 = 4800,
  SL_UART_BAUD_RATE_9600 = 9600,
  SL_UART_BAUD_RATE_14400 = 14400,
  SL_UART_BAUD_RATE_19200 = 19200,
  SL_UART_BAUD_RATE_28800 = 28800,
  SL_UART_BAUD_RATE_33600 = 33600,
  SL_UART_BAUD_RATE_38400 = 38400,
  SL_UART_BAUD_RATE_57600 = 57600,
  SL_UART_BAUD_RATE_115200 = 115200,
  SL_UART_BAUD_RATE_230400 = 230400,
  SL_UART_BAUD_RATE_460800 = 460800,
  SL_UART_BAUD_RATE_921600 = 921600,
  SL_UART_BAUD_RATE_1843200 = 1843200,
} SL_UART_BAUDRATE;
/* UART number */
typedef enum _SL_UART_ID
{
  SL_UART_1 = 1,
  SL_UART_2 = 2,
  SL_UART_QTY
} SL_UART_ID;
typedef enum _SL_UART_DATABITS
{
  SL_UART_7_DATA_BITS,
  SL_UART_8_DATA_BITS,
} SL_UART_DATABITS;
typedef enum _SL_UART_STOPBITS
{
  SL_UART_1_STOP_BIT,
  SL_UART_2_STOP_BITS,
} SL_UART_STOPBITS;
typedef enum _SL_UART_PARITY
{
  SL_UART_NO_PARITY,
  SL_UART_ODD_PARITY,
  SL_UART_EVEN_PARITY,
  SL_UART_SPACE_PARITY,
  SL_UART_MARK_PARITY,
} SL_UART_PARITY;
typedef struct
{
  U32 ulDataLen;
  U8 aucDataBuf[0];
} SL_UART_DATA, *PSL_UART_DATA;
/******************************************
                          function
******************************************/
#include "sl_app.h"
#define SL_UartOpen AA.SL_UartOpen_p
#define SL_UartClose AA.SL_UartClose_p
#define SL_UartSendData AA.SL_UartSendData_p
#define SL_UartSetBaudRate AA.SL_UartSetBaudRate_p
#define SL_UartSetDCBConfig AA.SL_UartSetDCBConfig_p
#define SL_UartSetFlowCtrl AA.SL_UartSetFlowCtrl_p
#define SL_UartGetTxRoomLeft AA.SL_UartGetTxRoomLeft_p
#define SL_UartGetTxRestBytes AA.SL_UartGetTxRestBytes_p
#define SL_UartGetBytesAvail AA.SL_UartGetBytesAvail_p
#define SL_UartClrRxBuffer AA.SL_UartClrRxBuffer_p
#define SL_UartClrTxBuffer AA.SL_UartClrTxBuffer_p
#define SL_UartSetAppTaskHandle AA.SL_UartSetAppTaskHandle_p
#define SL_UartGetAppTaskHandle AA.SL_UartGetAppTaskHandle_p
#define SL_UartSetDlMode AA.SL_UartSetDlMode_p
#define SL_UartGetDlMode AA.SL_UartGetDlMode_p
#endif
