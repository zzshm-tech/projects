
/*******************************************************************************
**	FileName:
**	Time:
********************************************************************************/

#include "include.h"

HANDLE SysAppEnterTask;




void ProcessTest(void)
{
	static unsigned char step = 0;
	static unsigned short int counter;
	
	unsigned char tmp;
	counter++;
	switch(step)
	{
		case 0:
			tmp = SL_FileSysFormat(SL_FS_TYPE_FAT,SL_FS_FORMAT_NORMAL);
			if(tmp == SL_RET_OK)
				SL_Print("FileSysFormat OK...........\r\n");
			step++;
			break;
		case 1:
			SL_Print("Test Task Counter:%d\r\n",counter);
			break;
		default:
			break;
	}
}




/*************************************************
**	函数名称:
**	功能描述:
**************************************************/

void SL_AppSendMsg(HANDLE stTask, unsigned int ulMsgId, unsigned int ulParam)
{
    SL_EVENT 					stEvnet;
    SL_TASK 					hTask;
	
    SL_Memset(&stEvnet, 0, sizeof(SL_EVENT));
    stEvnet.nEventId = ulMsgId;
    stEvnet.nParam1 = ulParam;
    hTask.element[0] = stTask;
    SL_SendEvents(hTask, &stEvnet);
}


/************************************************
**	函数名称:
**	功能描述:
************************************************/

void APP_ENTRY_START SL_Entry(void *pData)
{
	SL_EVENT	   		ev = {0};
	SL_TASK		   		stSltask;

	PSL_UART_DATA  		pUartData;     //串口接收数据  

	unsigned int 		cache_data_len;
	unsigned char 		*ftp_data;
    unsigned int 		app_up_st = 0;
	
    SL_Memset(&ev, 0, sizeof(SL_EVENT));
	SL_Sleep(1000);									//
	
	SL_StopWatchDog();  							//停止看门狗

    while(SL_CoreInitFinish() == FALSE)				//等待初始化内核
    {
        SL_Print("SLAPP: core init");
        SL_Sleep(1000);								//
    }

	SL_UartSetAppTaskHandle(SL_UART_1, SL_GetAppTaskHandle());                             //初始化串口1
    SL_UartSetAppTaskHandle(SL_UART_2, SL_GetAppTaskHandle());
	UART1_Init();  
	UART2_Init();
	Init_Led();
	
	SysAppEnterTask = SL_GetAppTaskHandle();
    stSltask.element[0] = SL_GetAppTaskHandle();	//获取应用主任务的任务句柄标识

	SL_GpioSetDir(SL_GPIO_6, SL_GPIO_OUT);			//开启Gnss
	SL_Sleep(100);
	SL_GpioWrite(SL_GPIO_6, SL_PIN_HIGH);			//GPS_ON;  
	SL_AppSendMsg(stSltask.element[0], EVT_APP_READY, 0);
	//SL_Print("The Sys Ver:1.1\r\n");
	while(1)
    {
        SL_FreeMemory((VOID*)ev.nParam1);
        SL_GetEvent(stSltask, &ev);
		
        switch (ev.nEventId)
        {
        	case SL_APP_UPDATE_REQ:                 //更新引用
        		cache_data_len = ev.nParam2;
				
                SL_Print("---> FTP get file len:%d\n", cache_data_len);
				
                if (cache_data_len < APP_CODE_MAXLEN) //app max 310K byte
                {
                    SL_Print("---> FTP get data in ram\n");
                    ftp_data = (unsigned char *)ev.nParam1;
                    app_up_st = SL_AppUpdateInit(ftp_data, cache_data_len);
                    SL_Print("---> SL APP update init ret:%d\n", app_up_st);
					
                    if (SL_APP_UPDATE_OK != app_up_st)
                    {
                        SL_Print("---> SL App update condition check fail, do not update!\n");
                    }
                    else
                    {
                    	SaveSendQueue();                //保存数据发送队列数据
                        SL_Print("---> SL App update condition check ok, reset system and update ... \n");
						SL_Sleep(10000);				//等待10秒
						SL_HstSendEvt(0x20161034);      //更新完成   重启设备
                        SL_PowerDown(0); 				//重启
                    }
                    
                }
                else
                {
                    SL_Print("---> FTP get file size overflow:%d, do not download!\n", cache_data_len);

                }
                
				break;
           	case EVT_APP_GPRS_RECV:                 //处理GPRS接收数据事件
				//ProcessGprsRecv();                  //处理GPRS接收数据包     
		   		break;
			case EVT_APP_READY:                    				//应用程序准备完成事件   
				InitSysConfig();                    			//系统配置及默认值初始化  
				SL_Sleep(2000);	                                //
				InitSysTimer();                     			//初始化系统定时器  
				SL_Sleep(1000);	
				LoadSendQueue();								//
				SL_Sleep(2000);	
				ReadSysRunData();                               //
				SL_Sleep(1000);
				ReadIpAddrBack();                              //
				InitTcpIp();    				 				//初始化TCP相关操作回调函函数 
				SL_Print("App Ready...........OK\r\n");  		//输出调试信息 
				break;
            case SL_EV_TIMER:                     	//定时器时间到事件
				if(ev.nParam1 == 3)				  	//一秒定时器
                {
					ProcessTest();
				}
				
				if(ev.nParam1 == 4)
				{
					TickAdd();      			//应用程序定时器   计数器
					ProcessGsmLed();
				}
				
            	break;
			case SL_EV_UART_RECEIVE_DATA_IND:         //串口接收事件
				if(ev.nParam2 == 1)
				{	
					pUartData = (PSL_UART_DATA)ev.nParam1;
					UART1_Recv_Data(pUartData->aucDataBuf,pUartData->ulDataLen);   //接收数据 串口1
				}
				else
				{
					pUartData = (PSL_UART_DATA)ev.nParam1;
					UART2_Recv_Data(pUartData->aucDataBuf,pUartData->ulDataLen);    //接收数据串口2
				}
				break;
            default:
            	break;
        }
    }
}





/**********************************File End**************************************/

