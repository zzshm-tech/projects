/*!
 *	\file sl_kpd.h
 *	\brief  A set of keypad message definations in SL1360
 *
 * 	\details The file defines keypad manipulation APIs to support development on SL1360.
 *	\n
 *
 *	\version 		0.1
 *	\author 		SimpLight
 *	\date 			2012-08-23
 * 	\copyright
 *  Copyright (C) 2006-2012 by SimpLight Nanoelectronics, Ltd.\n
 *                 All rights reserved.\n
 * This material constitutes the trade secrets and confidential,\n
 * proprietary information of SimpLight.  This material is not to\n
 * be disclosed, reproduced, copied, or used in any manner  not\n
 * permitted under license from SimpLight Nanoelectronics Ltd.\n
 *
 */

/**********************************************************************************
 **                        Edit History                                          *
 ** -------------------------------------------------------------------------    *
 ** DATE           NAME             DESCRIPTION                                  *
 ** 2012/08/23    Fan Xiaoyue         Created.                                   *
 *********************************************************************************/

#ifndef _SL_KPD_H_
#define _SL_KPD_H_
#include "sl_type.h"
#include "sl_os.h"


/*! \enum SL_KPDVAL
*   \brief Standard key definition.
*/
typedef enum
{
	VK_NONE              = 0x00, 
	
	VK_BACKSPACE         = 0x01, 
	VK_SPACE             = 0x02,
	VK_ESCAPE            = 0x03,
	VK_MENU              = 0x04,
	
	VK_HOME              = 0x10,
	VK_END               = 0x11,
	VK_INSERT            = 0x12,
	VK_DELETE            = 0x13, //Delete key   
	VK_PAGEUP            = 0x14,
	VK_PAGEDOWN          = 0x15,
	
	VK_UP                = 0x16, //UP arrow key   
	VK_DOWN              = 0x17, //DOWN arrow key   
	VK_LEFT              = 0x18, //LEFT arrow key   
	VK_RIGHT             = 0x19, //RIGHT arrow key   
	VK_SELECT            = 0x1A, //Select/action key, enters a carriage return in multiple-line fields by pressing the Select key.   
	
	VK_CTL_LEFT          = 0x20, //Left Soft control key   
	VK_CTL_RIGHT         = 0x21, //Right Soft control key
	
	VK_PHONE_SEND        = 0x30,
	VK_PHONE_END         = 0x31,
	
	VK_CONTRAST_DEC      = 0x40, 
	VK_CONTRAST_INC      = 0x41,
	VK_VOLUME_DEC        = 0x42, 
	VK_VOLUME_INC        = 0x43,
	VK_BRIGHTNESS_DEC    = 0x44,
	VK_BRIGHTNESS_INC    = 0x45,
	VK_BACKLIGHT_ON      = 0x46,
	VK_BACKLIGHT_OFF     = 0x47,   
	VK_DICTAPHONE_PLAY   = 0x48,    
	VK_DICTAPHONE_STOP   = 0x49,
	VK_DICTAPHONE_RECORD = 0x4A,  
	VK_POWER_OFF         = 0x4B,
	
	VK_NUM0              = 0x50,
	VK_NUM1              = 0x51,
	VK_NUM2              = 0x52,  
	VK_NUM3              = 0x53,
	VK_NUM4              = 0x54,  
	VK_NUM5              = 0x55,  
	VK_NUM6              = 0x56,  
	VK_NUM7              = 0x57,  
	VK_NUM8              = 0x58,  
	VK_NUM9              = 0x59,  
	VK_STAR              = 0x5A, //* key
	VK_HASH              = 0x5B, //# key
	
	VK_APP1              = 0x61,
	VK_APP2              = 0x62,
	VK_APP3              = 0x63,
	VK_APP4              = 0x64,
	VK_APP5              = 0x65,
	VK_APP6              = 0x66,
	VK_APP7              = 0x67,
	VK_APP8              = 0x68,
	VK_APP9              = 0x69,
	VK_APP10             = 0x6A,
	VK_APP11             = 0x6B,
	VK_APP12             = 0x6C,
	VK_APP13             = 0x6D,
	VK_APP14             = 0x6E,
	VK_APP15             = 0x6F,
	
	VK_ALL               = 0xFF,
}SL_KEYPAD_VK_t;

typedef enum
{
    SL_KEY_DOWN,
    SL_KEY_UP,
    SL_KEY_INVALID
}SL_KEY_STATUS;

typedef struct
{
    SL_KEY_STATUS enSlKeyStatus;
    U8 ucKeyValue;
}SL_KEY;

typedef void (*SL_KEY_CB_UPDW)(SL_KEY* pstSlKey);

typedef struct
{
    SL_KEY_CB_UPDW SL_KEY_UPDW;
}SL_KEY_CALLBACK;


/*!
 * \fn SL_RegisterKeypadHandler(SL_HANDLER khandler)
 * \brief Register keypad handler function
 * \param[in] khandler Specify keypad handler function as SL_HANDLER structure.
 * \return Register result
 * \retval SL_OK Register succeed.
 * \retval SL_ERR Register failed.
 */
//S32 SL_RegisterKeypadHandler(void* khandler);

void SL_KeyEventCallback(SL_KEY_CB_UPDW cb);

#endif
