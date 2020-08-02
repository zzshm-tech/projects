
/*******************************************************************************
**	FileName:
**	Time:
********************************************************************************/

#include "include.h"

HANDLE SysAppEnterTask;

union task_state SysTask; //任务定义标志位

/*************************************************
**	��������:
**	��������:
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
**	��������:
**	��������:
************************************************/

void APP_ENTRY_START SL_Entry(void *pData)
{
	SL_EVENT	   		ev = {0};
	SL_TASK		   		stSltask;

	PSL_UART_DATA  		pUartData;     //���ڽ�������  

	unsigned int 		cache_data_len;
	unsigned char 		*ftp_data;
    unsigned int 		app_up_st = 0;
	
    SL_Memset(&ev, 0, sizeof(SL_EVENT));
	SL_Sleep(1000);									//
	
	SL_StopWatchDog();  							//ֹͣ���Ź�

    while(SL_CoreInitFinish() == FALSE)				//�ȴ���ʼ���ں�
    {
        SL_Print("SLAPP: core init");
        SL_Sleep(1000);								//
    }

	SL_UartSetAppTaskHandle(SL_UART_1, SL_GetAppTaskHandle());                             //��ʼ������1
    SL_UartSetAppTaskHandle(SL_UART_2, SL_GetAppTaskHandle());
	UART1_Init();  
	UART2_Init();
	Init_Led();
	
	SysAppEnterTask = SL_GetAppTaskHandle();
    stSltask.element[0] = SL_GetAppTaskHandle();	//��ȡӦ�����������������ʶ

	SL_GpioSetDir(SL_GPIO_4, SL_GPIO_OUT);			//����Gnss
	SL_Sleep(100);
	SL_GpioWrite(SL_GPIO_4, SL_PIN_HIGH);			//����GNSS
	SL_AppSendMsg(stSltask.element[0], EVT_APP_READY, 0);
	//SL_Print("The Sys Ver:1.1\r\n");
	SysTask.value = 0xFF;
	while(1)
    {
        SL_FreeMemory((VOID*)ev.nParam1);
        SL_GetEvent(stSltask, &ev);
		
        switch (ev.nEventId)
        {
        	case SL_APP_UPDATE_REQ:                 //��������
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
                    	SaveSendQueue();                //
                        SL_Print("---> SL App update condition check ok, reset system and update ... \n");
						SL_Sleep(10000);				//
						SL_HstSendEvt(0x20161034);      //
                        SL_PowerDown(0); 				//
                    }
                    
                }
                else
                {
                    SL_Print("---> FTP get file size overflow:%d, do not download!\n", cache_data_len);

                }
                
				break;
			case EVT_APP_GSM_RECV:                //
				ProcessSMSRecv();
				break;
           	case EVT_APP_GPRS_RECV:                 //
				ProcessGprsRecv();                  //    
		   		break;
			case EVT_APP_READY:                    				//   
				InitSysConfig();                    			// 
				SL_Sleep(2000);	                                //
				InitSysTimer();                     			// 
				ReadLocalLogFile();
				SL_Sleep(1000);	
				LoadSendQueue();								//
				SL_Sleep(2000);	
				ReadSysRunData();                               //
				SL_Sleep(1000);
				ReadIpAddrBack();                              //
				InitTcpIp();    				 				// 
				InitSms();
				SL_Print(">App:App Ready...........OK\r\n");  		//
				break;
            case SL_EV_TIMER:                     	//
				if(ev.nParam1 == 3)				  	//1秒定时器
                {
					if(SysTask.bit.task_gprs == 1)
						ProcessSocketClient();     		//

					ProcessSysSleep();       		//

					if(SysTask.bit.task_Up == 1)
						ProcessTcpIpUpLoad();      		//
					if(SysTask.bit.task_Run == 1)
						ProcessRunData();				// 

					ProcessCmd();
				}
				
				if(ev.nParam1 == 4)
				{
					TickAdd();      			//
					ProcessRtc();  
					
					if(SysTask.bit.task_gnss == 1)             //
						GnssDataParse();            //

					if(SysTask.bit.task_Led == 1)
						ProcessGsmLed();            //

					ProTermRecvData();          //

					ProTerminalData();          //

					if(SysTask.bit.task_SysPro == 1)
						SysProcess();             	//

					ProcessGnssLed();			//
				}
				
            	break;
			case SL_EV_UART_RECEIVE_DATA_IND:         //
				if(ev.nParam2 == 1)
				{	
					pUartData = (PSL_UART_DATA)ev.nParam1;
					UART1_Recv_Data(pUartData->aucDataBuf,pUartData->ulDataLen);   //
				}
				else
				{
					pUartData = (PSL_UART_DATA)ev.nParam1;
					UART2_Recv_Data(pUartData->aucDataBuf,pUartData->ulDataLen);    //
				
}
				break;
            default:
            	break;
        }
    }
}





/**********************************File End**************************************/

