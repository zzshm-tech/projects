

/******************************************
**	FileName:
**	Time:
**	¹¹½¨ÉÏ´«Êý¾Ý°ü
**
*******************************************/

#include "include.h"


static unsigned char 				TmpBuf[512];					//Êý¾Ý»º³åÇø
static unsigned char 				MsgLine;   						//ÐÐ±êÊ¶
static unsigned char 				MsgNum;							//ÏûÏ¢Ìå¼ÆÊýÆ

static unsigned short int 			MsgSerial;						//ÏûÏ¢ÐòÁÐÂë



/**********************************************************************************************
**	º¯ÊýÃû³Æ:unsigned short int BuildPositionPacket(unsigned char *source,unsigned char big)
**	¹¦ÄÜÃèÊö:
***********************************************************************************************/

unsigned short int BuildPositionPacket(unsigned char *source,unsigned char big)
{	
	static unsigned short int DataLen = 0;
	unsigned short int tmp = 0;
	
	MsgHeadStr 			TmpMsg;
	unsigned char 		buf[50];
	SysConfigStr 		*pSysCfg;
	
	MsgHeadStr 	 		*pMsgHead;
	MsgGnssStr   		*pMsgGnss;
	MsgFarmStr   		*pMsgFarm;
	MsgDeviceStr 		*pMsgDevice;

	GNSSSTR 	 		*pGnss;
	SysTimeStr 	 		*pTime;
	TerStateStr			*pTerState;     //ÏµÍ³Éè±¸×´Ì¬£¬²É¼¯µ½µÄÊý¾Ý
	CanDataStr 			*pCanData;
	
	pSysCfg = GetSysCfgSpace();
	pTerState = GetTerStateDataSpace();

	MsgLine++;
	if(MsgLine <= 1)
	{
		TmpMsg.frame_start[0] = 0xF1;
		TmpMsg.frame_start[1] = 0xF2;
		TmpMsg.frame_start[2] = 0xFF;
		TmpMsg.msg_id = 0x45;
		
		SL_Memcpy(&TmpMsg.device_id,pSysCfg->TerminalID,16);
		TmpMsg.blind_flag = 0x00;
		TmpMsg.msg_body_num = 0x00;
		TmpMsg.msg_len = 0x00;
		SL_Memcpy(TmpBuf,(unsigned char *)&TmpMsg,25);           			//
		DataLen = 25;

		pMsgDevice = (MsgDeviceStr *)buf;
		pMsgDevice->msg_device_type = 0x0001;               	 			//ÏûÏ¢ÌåÀàÐÍÂë(Éè±¸×´Ì¬ÏûÏ¢Ìå)
		pMsgDevice->msg_device_len = 7;                             		//ÏûÏ¢Ìå³¤¶È
 
		pMsgDevice->msg_device_manu_num 		= 0x01;                  	//Éè±¸Éú²ú³§¼ÒÃû³Æ
		pMsgDevice->msg_device_manu_type 		= 0x01;                 	//
		pMsgDevice->msg_device_user_num 		= 22;                   	//ÓÃ»§³§¼Ò±àºÅ
		pMsgDevice->msg_device_car_num 			= pSysCfg->CarType;         //³µÐÍ
		pMsgDevice->msg_device_protocol_ver 	= pTerState->McuFirNum;		//µ¥Æ¬»ú³ÌÐò°æ±¾
		pMsgDevice->msg_device_firware_ver 		= GetVerNum();              //¹Ì¼þ³ÌÐò°æ±¾ºÅ
		pMsgDevice->msg_device_hardware_ver 	= pSysCfg->HardWareNum;     //Ó²¼þ°æ±¾ºÅ
		
		SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgDeviceStr)); 
		DataLen += sizeof(MsgDeviceStr);
		MsgNum = 1;
		//MsgNum++;
	}

	//¶¨Î»ÏûÏ¢Ìå
	pMsgGnss = (MsgGnssStr *)buf;
	pGnss = GetGnssDataSpace();
	pTime = GetSysTimeSpace();
	
	pMsgGnss->msg_gps_type = 0x0100;                   				//   ÏûÏ¢ÀàÐÍÂë
	pMsgGnss->msg_gps_len = 26;                                     //	ÏûÏ¢Êý¾ÝÇøÓò³¤¶È
	pMsgGnss->msg_gps_latitude = pGnss->latitude * 1000000;         //   ¶¨Î»ÐÅÏ¢-¾­¶È
	pMsgGnss->msg_gps_longitude = pGnss->longitude * 1000000;      	//	¶¨Î»ÐÅÏ¢-Î¬¶È
	pMsgGnss->msg_gps_speed = pGnss->speed * 10;					//	¶¨Î»ÐÅÏ¢-ËÙ¶È
	pMsgGnss->msg_gps_azimuth  = pGnss->azimuth;         			//   ¶¨Î»ÐÅÏ¢-º½Ïò½Ç¶È
	pMsgGnss->msg_gps_altitude = pGnss->altitude;        			//	¶¨Î»ÐÅÏ¢-º£°Î¸ß¶È

	pMsgGnss->msg_gps_year = pTime->Year;							//   ¶¨Î»ÐÅÏ¢-Ê±¼ä
	pMsgGnss->msg_gps_mon = pTime->Mon;								//   
	pMsgGnss->msg_gps_day = pTime->MDay;							//
	pMsgGnss->msg_gps_hour = pTime->Hour;							//
	pMsgGnss->msg_gps_min = pTime->Min;								//
	pMsgGnss->msg_gps_sec = pTime->Sec;								//

	pMsgGnss->msg_gps_satellite_num = pGnss->satellite_num;			//  Ê¹ÓÃÎÀÐÇÊý¾Ý
	pMsgGnss->msg_gps_view_num = pGnss->satellite_view_num;			//  ¿ÉÊÓÎÀÐÇÊýÁ¿
	pMsgGnss->msg_gps_ghdop_v = pGnss->ghdop_v;						//  Ë®Æ½¶¨Î»Òò×Ó
	
	if(pGnss->status == 'A')                                    //¶¨Î»×´Ì¬±êÊ¶
		pMsgGnss->msg_gps_status = pGnss->status | 0x02;
	else
		pMsgGnss->msg_gps_status = pGnss->status & 0xFD;

	pMsgGnss->msg_gps_line = MsgLine;
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgGnssStr)); 
	DataLen += sizeof(MsgGnssStr);
	MsgNum++;

	//Éè±¸Ê±¼ä
	pMsgFarm = (MsgFarmStr *)buf;

	pCanData = GetCanDataSpace();
	pMsgFarm->MsgFarmType = 0x0980;                                 //ÏûÏ¢ÌåÀàÐÍÂë
	pMsgFarm->MsgFarmLen = 9;                                       //ÏûÏ¢ÌåÊý¾ÝÓò³¤¶È
	
	switch(pSysCfg->CarType)
	{
		case 0x09:
			pMsgFarm->MsgFarmBig = pCanData->Rotate1;       //Ë®µ¾»ú
			break;
		case 0x0f:
			pMsgFarm->MsgFarmBig = pCanData->EngineRotate / 8;       //Ë®µ¾»ú,ÀÏ¹úÈý
			break;
		case 0x03:
		case 0x0D:
			pMsgFarm->MsgFarmBig = pCanData->EngineRotate / 8;                        //ÍÏÀ­»ú 
			//SL_Print("(1)pMsgFarm->MsgFarmBig:%d\r\n",pMsgFarm->MsgFarmBig);
			break;
		default:
			pMsgFarm->MsgFarmBig = 0;
			break;
	}
	pMsgFarm->MsgTimeYear = pTime->Year;							//
	pMsgFarm->MsgTimeMon = pTime->Mon;								//
	pMsgFarm->MsgTimeDay = pTime->MDay;								//
	pMsgFarm->MsgTimeHour = pTime->Hour;							//
	pMsgFarm->MsgTimeMin = pTime->Min;                              //
	pMsgFarm->MsgTimeSec = pTime->Sec;                              //
	pMsgFarm->MsgLine = MsgLine;                                    //
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgFarmStr)); 
	DataLen += sizeof(MsgFarmStr);
	MsgNum++;

	if(MsgLine >= 10 || big > 0)    								//´ò°ü½áÊø                     
	{
		pMsgHead = (MsgHeadStr *)TmpBuf; 
		pMsgHead->msg_line = MsgLine;
		pMsgHead->msg_body_num = MsgNum;
		//SL_Print("BuildPositionPacket:%d\r\n",pMsgHead->msg_body_num);
		pMsgHead->msg_len = DataLen - 23; 	//¼ÓÉÏËæ»úÂë2¸ö×Ö½Ú
		MsgSerial++;
		SL_Memcpy(TmpBuf + DataLen,(unsigned char *)&MsgSerial,2);
		DataLen += 2;
		*(TmpBuf + DataLen) = BccVerify(TmpBuf + 3,DataLen - 3);
		DataLen++;
		*(TmpBuf + DataLen) = 0x0D;
		DataLen++;
		SL_Memcpy(source,TmpBuf,DataLen);
		tmp = DataLen;
		//SL_Print(">App:BuildCompletePacket 1...... %d\r\n",DataLen);
		MsgNum = 0;
		DataLen = 0;
		MsgLine = 0;
	}
	return tmp;
}	



/******************************************************
**	º¯ÊýÃû³Æ:
**	¹¦ÄÜÃèÊö:
******************************************************/

unsigned short int BuildCompletePacket(unsigned char *source,unsigned char big)
{
	MsgHeadStr TmpMsg;
	unsigned short int DataLen;
	unsigned char buf[450];
	SysConfigStr *pSysCfg;

	//ÏûÏ¢ÌåÖ¸Õë
	MsgHeadStr 	 		*pMsgHead;              //ÏûÏ¢Í·
	MsgDeviceStr 		*pMsgDevice;            //Éè±¸×´Ì¬ÏûÏ¢Ìå
	MsgInputStr  		*pMsgInput;             //ÊäÈëÏûÏ¢Ìå
	MsgGnssStr   		*pMsgGnss;              //¶¨Î»ÏûÏ¢Ìå
	MsgFarmStr   		*pMsgFarm;              //×÷ÒµÏûÏ¢Ìå
	MsgCanPaddyStr		*pMsgCanPaddy;			//
	MsgCanTractorStr	*pMsgCanTractor;		//

	MsgOilStr			*pMsgOil;
	
	
	//Êý¾ÝÖ¸Õë
	GNSSSTR 			*pGnss;         //¶¨Î»Ä£¿éÊý¾Ý
	TerStateStr			*pTerState;     //ÏµÍ³Éè±¸×´Ì¬£¬²É¼¯µ½µÄÊý¾Ý
	SysTimeStr 			*pSysTime;      //ÏµÍ³Ê±¼ä
	CurrentFaultCodeStr	*pFaultCode1;   //µ±Ç°¹ÊÕÏÂë
	CanDataStr 			*pCanData;
	
	pSysCfg = GetSysCfgSpace();       	//»ñÈ¡Éè±¸ÅäÖÃÐÅÏ¢
	
	DataLen = 0;                     	//±êÊ¶Êý¾Ý³¤¶È
	big = 0;
	MsgNum = 0;
	
	//ÏûÏ¢Í·
	
	TmpMsg.frame_start[0] = 0xF1;
	TmpMsg.frame_start[1] = 0xF2;
	TmpMsg.frame_start[2] = 0xFF;
	TmpMsg.msg_id = 0x45;
	SL_Memcpy(&TmpMsg.device_id,pSysCfg->TerminalID,16);            //Éè±¸ºÅ
	TmpMsg.blind_flag = 0x00;                                       //
	TmpMsg.msg_body_num = 0x00;                                     //
	TmpMsg.msg_len = 0x00;                                          //
	SL_Memcpy(TmpBuf,(unsigned char *)&TmpMsg,25);          		//
	DataLen = 25;
	
	//Éè±¸×´Ì¬ÏûÏ¢Ìå:

	pTerState = GetTerStateDataSpace();
	
	pMsgDevice = (MsgDeviceStr *)buf;
	pMsgDevice->msg_device_type = 0x0001;               		 	//
	pMsgDevice->msg_device_len = 8;									//
	pMsgDevice->msg_device_manu_num = 0x01;                  		//
	pMsgDevice->msg_device_manu_type = 0x01;                  	 	//
	pMsgDevice->msg_device_user_num = 22;                    		//ÓÃ»§±àºÅ
	pMsgDevice->msg_device_car_num = pSysCfg->CarType;				//pSysCfg->CarType;            //³µÐÍ±àºÅ  
	pMsgDevice->msg_device_firware_ver = GetVerNum();               //ST86³ÌÐò
	pMsgDevice->msg_device_protocol_ver = pTerState->McuFirNum;     //µ¥Æ¬»ú³ÌÐò
	pMsgDevice->msg_device_hardware_ver = pSysCfg->HardWareNum;		//Ó²¼þ³ÌÐò
	
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgDeviceStr));    
	DataLen += sizeof(MsgDeviceStr); 
	*(TmpBuf + DataLen) = 1;
	DataLen += 1;
	MsgNum++;
	
	//ÊäÈë×´Ì¬Êý¾Ý
	
	pMsgInput = (MsgInputStr *)buf;
	
	pMsgInput->msg_input_type = 0x0004;                             //ÏûÏ¢ÌåÀàÐÍÂë(ÊäÈë×´Ì¬ÏûÏ¢Ìå)
	pMsgInput->msg_input_len = 24;                                  //ÏûÏ¢Ìå³¤¶È
	pMsgInput->msg_input_io = 0x00;
	pMsgInput->msgAccState = ReadAccState();						//ACC×´Ì¬  
	pMsgInput->msgMotoState = 0;
	pMsgInput->msgFrq1 = 0;
	pMsgInput->msgFrq2 = 0;
	pMsgInput->msgPWM1 = 0;
	pMsgInput->msgPWM2 =  0;
	pMsgInput->msgInputVol1 = 0;
	pMsgInput->msgInputVol2 = 0;
	pMsgInput->msgPowVol = pTerState->PowVol * 100;					//Íâ²¿¹©µçµçÑ¹
	pMsgInput->msgBatteryVol = pTerState->Battery * 100;			//ÄÚ²¿µç³ØµçÑ¹
	pMsgInput->msgWarningState = GetWarningValue();          			
	pMsgInput->msgLine = 1;
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgInputStr)); 	
	DataLen += sizeof(MsgInputStr);
	MsgNum++;
	
	
	//¶¨Î»ÏûÏ¢Ìå
	pGnss = GetGnssDataSpace();
	pSysTime = GetSysTimeSpace();
	pMsgGnss = (MsgGnssStr *)buf;
	pMsgGnss->msg_gps_type = 0x0100;                   								//
	pMsgGnss->msg_gps_len = 26;                                     				//
	pMsgGnss->msg_gps_latitude = pGnss->latitude * 1000000;        					//
	pMsgGnss->msg_gps_longitude = pGnss->longitude * 1000000;      					//  
	pMsgGnss->msg_gps_speed = pGnss->speed * 10;									//
	pMsgGnss->msg_gps_azimuth  = pGnss->azimuth;         							//
	pMsgGnss->msg_gps_altitude = pGnss->altitude;        							//
	pMsgGnss->msg_gps_year = pSysTime->Year;
	pMsgGnss->msg_gps_mon = pSysTime->Mon;
	pMsgGnss->msg_gps_day = pSysTime->MDay;
	pMsgGnss->msg_gps_hour = pSysTime->Hour;
	pMsgGnss->msg_gps_min = pSysTime->Min;
	pMsgGnss->msg_gps_sec = pSysTime->Sec;
	
	pMsgGnss->msg_gps_satellite_num = pGnss->satellite_num;
	pMsgGnss->msg_gps_view_num = pGnss->satellite_view_num;
	pMsgGnss->msg_gps_ghdop_v = pGnss->ghdop_v;

	if(pGnss->status == 'A')
		pMsgGnss->msg_gps_status = pGnss->status | 0x02;
	else
		pMsgGnss->msg_gps_status = pGnss->status & 0xFD;

	pMsgGnss->msg_gps_line = 1;
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgGnssStr)); 
	DataLen += sizeof(MsgGnssStr);
	MsgNum++;
	
	//Éè±¸Ê±¼ä
	pMsgFarm = (MsgFarmStr *)buf;

	pMsgFarm->MsgFarmType = 0x0980;
	pMsgFarm->MsgFarmLen = 9;

	pCanData = GetCanDataSpace();          //»ñÈ¡ËùÓÐ»úÐÍCANÊý¾Ý
	switch(pSysCfg->CarType)
	{
		case 0x09:
			pMsgFarm->MsgFarmBig = pCanData->Rotate1;       //Ë®µ¾»ú
			break;
		case 0x0f:
			pMsgFarm->MsgFarmBig = pCanData->EngineRotate / 8;       //Ë®µ¾»ú,ÀÏ¹úÈý
			break;
		case 0x03:                                           //ÐÂ¹úÈýMÆ½Ì¨ÍÏÀ­»ú
		case 0x0D:                                           //ÀÏ¹úÈýMÆ½Ì¨ÍÏÀ­»ú
			pMsgFarm->MsgFarmBig = pCanData->EngineRotate / 8;                        //ÍÏÀ­»ú  
			//SL_Print("(2)pMsgFarm->MsgFarmBig:%d\r\n",pMsgFarm->MsgFarmBig);
			break;
		default:
			pMsgFarm->MsgFarmBig = 0;
			break;
	}
	
	pMsgFarm->MsgTimeYear = pSysTime->Year;
	//SL_Print("The Year:%d\r\n",pMsgFarm->MsgTimeYear);
	pMsgFarm->MsgTimeMon = pSysTime->Mon;
	pMsgFarm->MsgTimeDay = pSysTime->MDay;
	pMsgFarm->MsgTimeHour = pSysTime->Hour;
	pMsgFarm->MsgTimeMin = pSysTime->Min;
	pMsgFarm->MsgTimeSec = pSysTime->Sec;
	pMsgFarm->MsgLine = 1;
	SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgFarmStr)); 
	DataLen += sizeof(MsgFarmStr);
	MsgNum++;
	switch(pSysCfg->CarType)
	{
		case 0x09:  							// Ë®µ¾»ú
			pMsgCanPaddy = (MsgCanPaddyStr *)buf;

			pMsgCanPaddy->MsgCanPaddyType = 0x0014;				   				//
			pMsgCanPaddy->MsgCanPaddyLen = sizeof(MsgCanPaddyStr) - 4;			//
			
			pMsgCanPaddy->MsgEngineRotate = pCanData->EngineRotate;     		//·¢¶¯»ú×ªËÙ
			pMsgCanPaddy->MsgWaterTemp = pCanData->WaterTemp;    				//ÀäÈ´Ë®ÎÂ¶È
			pMsgCanPaddy->MsgRotate1 = pCanData->Rotate1;        				//¹¤×÷ËÙ¶ÈÅÐ¶Ï
			pMsgCanPaddy->MsgRotate2 = pCanData->Rotate2;        				//¹ö¶¯ÂÖ×ªËÙ
			pMsgCanPaddy->MsgSysVol = pCanData->SysVol;            			//ÏµÍ³µçÑ¹
			pMsgCanPaddy->MsgEngineWorkTime = pCanData->EngineWorkTime;    	//·¢¶¯»ú¹¤×÷Ê±¼ä 
			pMsgCanPaddy->MsgWarnValue = pCanData->WarnValue1;         			//Á¸Âú±¨¾¯
			pMsgCanPaddy->MsgTravlledSpeed = pCanData->TravlledSpeed;     		//ÐÐÊ»ËÙ¶È
			pMsgCanPaddy->MsgTotalTravlled = pCanData->TotalTravlled;     		//×ÜÐÐÊ»Àï³Ì

			pMsgCanPaddy->MsgEngineOil =  pCanData->EngineOil; 			//»úÓÍÑ¹Á¦
			pMsgCanPaddy->MsgFuelPercent = pCanData->FuelPercent;			//È¼ÁÏ°Ù·Ö±È
//			SL_Print("Oil Data............%d\r\n",pMsgCanPaddy->MsgEngineOil);
			pMsgCanPaddy->MsgLine1 = 1;
			
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanPaddyStr)); 
			DataLen += sizeof(MsgCanPaddyStr);
			MsgNum++;

			pMsgOil = (MsgOilStr *)buf;

			pMsgOil->MsgOilType = 0x0013;
			pMsgOil->MsgOilLen = 9;

			pMsgOil->MsgOil = pCanData->OilConsumption;
			pMsgOil->MsgOilTotal = pCanData->TatalOilConsumption;
			pMsgOil->MsgLine = 1;
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgOilStr)); 
			DataLen += sizeof(MsgOilStr);
			//SL_Print("Oil Data.........................3\r\n");
			MsgNum++;
			break;
		case 0x0F:  //  ÀÏ¹úÈýË®µ¾»ú
			pMsgCanPaddy = (MsgCanPaddyStr *)buf;

			pMsgCanPaddy->MsgCanPaddyType = 0x000E;				   				//
			pMsgCanPaddy->MsgCanPaddyLen = sizeof(MsgCanPaddyStr) - 6;			//
			
			pMsgCanPaddy->MsgEngineRotate = pCanData->EngineRotate;     		//·¢¶¯»ú×ªËÙ
			pMsgCanPaddy->MsgWaterTemp = pCanData->WaterTemp;    				//ÀäÈ´Ë®ÎÂ¶È
			pMsgCanPaddy->MsgRotate1 = pCanData->Rotate1;        				//¹¤×÷ËÙ¶ÈÅÐ¶Ï
			pMsgCanPaddy->MsgRotate2 = pCanData->Rotate2;        				//¹ö¶¯ÂÖ×ªËÙ
			pMsgCanPaddy->MsgSysVol = pCanData->SysVol;            				//ÏµÍ³µçÑ¹
			pMsgCanPaddy->MsgEngineWorkTime = pCanData->EngineWorkTime;    		//·¢¶¯»ú¹¤×÷Ê±¼ä 
			pMsgCanPaddy->MsgWarnValue = pCanData->WarnValue1;         			//Á¸Âú±¨¾¯
			pMsgCanPaddy->MsgTravlledSpeed = pCanData->TravlledSpeed;     		//ÐÐÊ»ËÙ¶È
			pMsgCanPaddy->MsgTotalTravlled = pCanData->TotalTravlled;     		//×ÜÐÐÊ»Àï³Ì
			pMsgCanPaddy->MsgLine1 = 1;
			
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanPaddyStr) - 2);             //
			DataLen = DataLen + sizeof(MsgCanPaddyStr) - 2;
			MsgNum++;
			break;
		case 0x03:                                  //MÆ½Ì¨ÍÏÀ­»ú
			pMsgCanTractor = (MsgCanTractorStr *)buf;

			pMsgCanTractor->MsgCanTractorType = 0x000F; 				 		//ÏûÏ¢ÌåÀàÐÍÂë  
			pMsgCanTractor->MsgCanTractorLen = sizeof(MsgCanTractorStr) - 4;	//ÏûÏ¢Ìå³¤¶È
			
			pMsgCanTractor->MsgEngineRotate = pCanData->EngineRotate;		//·¢¶¯»ú×ªËÙ
			pMsgCanTractor->MsgWaterTemp =	pCanData->WaterTemp; 			//ÀäÈ´Ë®ÎÂ¶È
			pMsgCanTractor->MsgEngineOil =  pCanData->EngineOil; 			//»úÓÍÑ¹Á¦
			pMsgCanTractor->MsgFuelPercent = pCanData->FuelPercent;			//È¼ÁÏ°Ù·Ö±È
			pMsgCanTractor->MsgEngineWorkTime = pCanData->EngineWorkTime;	//·¢¶¯»ú¹¤×÷Ê±¼ä 
			pMsgCanTractor->MsgSysVol =	pCanData->SysVol;					//ÏµÍ³µçÑ¹
			pMsgCanTractor->MsgTravlledSpeed = pCanData->TravlledSpeed; 		//ÐÐÊ»ËÙ¶È
			pMsgCanTractor->MsgTotalTravlled =	pCanData->TotalTravlled; 	//×ÜÐÐÊ»Àï³Ì
			pMsgCanTractor->MsgWarnValue = pCanData->WarnValue1; 				//Á¸Âú±¨¾¯
			pMsgCanTractor->MsgLine = 1;
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanTractorStr)); 
			DataLen += sizeof(MsgCanTractorStr);
			MsgNum++;

			pMsgOil = (MsgOilStr *)buf;

			pMsgOil->MsgOilType = 0x0013;
			pMsgOil->MsgOilLen = 9;

			pMsgOil->MsgOil = pCanData->OilConsumption;
			pMsgOil->MsgOilTotal = pCanData->TatalOilConsumption;
			pMsgOil->MsgLine = 1;
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgOilStr)); 
			DataLen += sizeof(MsgOilStr);
			//SL_Print("Oil Data.........................3\r\n");
			MsgNum++;
			break;
		case 0x0D:                              //MÆ½Ì¨ÍÏÀ­»ú ÀÏ¹úÈý
			pMsgCanTractor = (MsgCanTractorStr *)buf;

			pMsgCanTractor->MsgCanTractorType = 0x000F; 				 		//ÏûÏ¢ÌåÀàÐÍÂë  
			pMsgCanTractor->MsgCanTractorLen = sizeof(MsgCanTractorStr) - 4;	//ÏûÏ¢Ìå³¤¶È
			
			pMsgCanTractor->MsgEngineRotate = pCanData->EngineRotate;		//·¢¶¯»ú×ªËÙ
			pMsgCanTractor->MsgWaterTemp =	pCanData->WaterTemp; 			//ÀäÈ´Ë®ÎÂ¶È
			pMsgCanTractor->MsgEngineOil =  pCanData->EngineOil; 			//»úÓÍÑ¹Á¦
			pMsgCanTractor->MsgFuelPercent = pCanData->FuelPercent;			//È¼ÁÏ°Ù·Ö±È
			pMsgCanTractor->MsgEngineWorkTime = pCanData->EngineWorkTime;	//·¢¶¯»ú¹¤×÷Ê±¼ä 
			pMsgCanTractor->MsgSysVol =	pCanData->SysVol;					//ÏµÍ³µçÑ¹
			pMsgCanTractor->MsgTravlledSpeed = pCanData->TravlledSpeed; 		//ÐÐÊ»ËÙ¶È
			pMsgCanTractor->MsgTotalTravlled =	pCanData->TotalTravlled; 	//×ÜÐÐÊ»Àï³Ì
			pMsgCanTractor->MsgWarnValue = pCanData->WarnValue1; 				//Á¸Âú±¨¾¯
			pMsgCanTractor->MsgLine = 1;
			SL_Memcpy(TmpBuf + DataLen,buf,sizeof(MsgCanTractorStr)); 
			DataLen += sizeof(MsgCanTractorStr);
			MsgNum++;
			break;
		default:
			break;
	}

	//ÒÔÏÂÊÇ¹ÊÕÏÂë
	pFaultCode1 = GetCurrentFaultCodeSpace();     //µ±Ç°¹ÊÕÏÂë

	if(pFaultCode1->FaultCodeNum > 0 && pFaultCode1->FaultCodeNum <= 160)     //ÓÐ¹ÊÕÏÂëÇÒÔÚµ±Ç°·¶Î§ÄÚ£¬½øÐÐ´ò°ü
	{
		IntToChar TmpInt16;

		TmpInt16.IntII = 0x0011;                      //ÀàÐÍ±àÂë
		buf[0] = TmpInt16.TTbyte[0];
		buf[1] = TmpInt16.TTbyte[1];

		TmpInt16.IntII = pFaultCode1->FaultCodeNum * 4 + 2;   //ÏûÏ¢Ìå³¤¶È  ÏûÏ¢Ìå³¤¶È¶¼ÊÇ8µÄ±¶Êý
				
		buf[2] = TmpInt16.TTbyte[0];
		buf[3] = TmpInt16.TTbyte[1];
				
		SL_Memcpy(TmpBuf + DataLen,buf,4);           //³¤¶ÈºÍÀàÐÍÂë 
		DataLen += 4;
				
		*(TmpBuf + DataLen) = pFaultCode1->FaultCodeNum;   //ÉÏ´«¹ÊÕÏÂëÊýÁ¿¡£¹ÊÕÏÂëÊýÁ¿Îª
		DataLen++;
				
		SL_Memcpy(TmpBuf + DataLen,pFaultCode1->FaultCode,pFaultCode1->FaultCodeNum * 4);
		DataLen += pFaultCode1->FaultCodeNum * 4;

		*(TmpBuf + DataLen) = 0x01;
		DataLen += 1;
		MsgNum++;
			
	}
			
	pMsgHead = (MsgHeadStr *)TmpBuf;
	pMsgHead->msg_len = DataLen - 23;          //¼ÓÉÏËæ»úÂë³¤¶È  
	
	pMsgHead->msg_body_num = MsgNum;  
	//SL_Print("BuildCompletePacket:%d\r\n",pMsgHead->msg_body_num);
	pMsgHead->msg_line = 1;
	MsgSerial++;
	SL_Memcpy(TmpBuf + DataLen,(unsigned char *)&MsgSerial,2); 
	
	DataLen += 2;
	
	
	*(TmpBuf + DataLen) = BccVerify(TmpBuf + 3,DataLen - 3);
	DataLen++;
	*(TmpBuf + DataLen) = 0x0D;
	DataLen++;
	SL_Memcpy(source,TmpBuf,DataLen);

	SL_Print(">App:BuildCompletePacket 2...... %d\r\n",DataLen);

	return DataLen;
	
}



/**************************File End***************************/






