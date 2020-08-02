/*********************************************************************************
** File Name:     sl_lbs.h                                                *
** Author:         Wang Ya Feng                                                  *
** DATE:           2015/05/06                                                   *
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
** 2015/05/06     Wang Ya Feng       Create
*********************************************************************************/
#ifndef _SL_LBS_H_
#define _SL_LBS_H_

#include "sl_type.h"

/****************************************************
                    macro define

****************************************************/
#define LBS_ADDR_LENTH_MAX     80
#define LBS_ROAD_LENTH_MAX     60
#define LBS_COUNTY_CODE_LENTH_MAX  20
#define LBS_TOWN_CODE_LENTH_MAX  20

typedef enum
{
    SL_LBS_MAP_GPS = 1,
	SL_LBS_MAP_GOOGLE = 2,
	SL_LBS_MAP_BAIDU = 4,
	SL_LBS_MAP_GAODE = 8,
	SL_LBS_MAP_TOTAL
}SL_LBS_MAP_INFO_TYPE;

typedef struct
{
    U32 ulLongi; //输入gps经度
	U32 ulLati; //输入gps纬度
	BOOL blLongLatValidFlag; // 输入gps经纬度有效标志
	SL_LBS_MAP_INFO_TYPE enMapType;  //返回地图坐标类型 1=GPS 2=Google 4=Baidu 8=QQ(高德) 多选相加 不填默认为1
    BOOL bAddr;  //是否返回位置信息
	BOOL bRoad;  //是否返回道路信息
	BOOL bRadius;  //是否返回覆盖度信息
	BOOL bContyDivCode; // 是否返回县级行政区划代码
	BOOL bTownDivCode;  // 是否返回乡镇级行政区划代码
}SL_LBS_REQ_INFO_STRUCT;

typedef struct
{
    U32 ulLongi; //返回gps经度
	U32 ulLati; //返回gps纬度
    U32 ulLongiGoogle; // 返回google地图经度
	U32 ulLatiGoogle; // 返回google地图纬度
    U32 ulLongiBaidu; // 返回baidu地图经度
	U32 ulLatiBaidu; // 返回baidu地图纬度
    U32 ulLongiGaode; // 返回高德地图经度
	U32 ulLatiGaode; // 返回高德地图纬度
	U32 ulRadius; // 返回覆盖度信息
    U8 aucAddr[LBS_ADDR_LENTH_MAX]; // 返回地址信息字符串
	U8 aucRoad[LBS_ROAD_LENTH_MAX]; // 返回道路信息字符串
	U8 aucCountyDivCode[LBS_COUNTY_CODE_LENTH_MAX]; // 返回县级行政区划代码字符串
	U8 aucTownDivCode[LBS_TOWN_CODE_LENTH_MAX]; // 返回乡镇级行政区划代码字符串
}SL_LBS_RSP_INFO_STRUCT;

/****************************************************
                                   function

****************************************************/
VOID SL_LbsInit(VOID);
S32 SL_LbsSendReq(SL_LBS_REQ_INFO_STRUCT* pstLbsReqInfo);
S32 SL_LbsRspInfoParse(S8* scRcvInfo, SL_LBS_RSP_INFO_STRUCT* pstRspInfo);

#endif
