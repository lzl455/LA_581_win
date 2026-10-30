/*******************************************************************************
* Copyright (c) 2013, SHENZHEN SEMITEK Co.,LTD.
* All rights reserved.
*
* 文件名称: _MATRIX_KEY_TBL_
*
* 功能说明:
* 修改说明:
*******************************************************************************/
#ifndef _MATRIX_KEY_WIN_FN_TBL_
#define _MATRIX_KEY_WIN_FN_TBL_

//=============================================================================;
// 配置引用区                                                                  ;
//=============================================================================;

//-----------------------------------------------------------------------------;
// FN按下，未锁定
//

// fn 键值表
unsigned char           const   matrixkey_win_fn_tbl[] =
{
// C2
    iMODE_KEY_BT1,      iKEY_TYPE_SPECIAL,
    iSTD_CAPITAL,       iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_S,             iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_QUROPE2,       iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_X,             iKEY_TYPE_STD|iKEY_PRESS,    
    iSTD_NULL,          iKEY_TYPE_INVALID,
    iMMK_MEDIA_SELECT_L,iKEY_TYPE_MMK|iMMK_MEDIA_SELECT_H,
    iSTD_2,             iKEY_TYPE_STD|iKEY_PRESS,
// C3
    iMODE_KEY_BT2,      iKEY_TYPE_SPECIAL,
    iMMK_VOLUMEUP_L,    iKEY_TYPE_MMK|iMMK_VOLUMEUP_H,
    iSTD_D,             iKEY_TYPE_STD|iKEY_PRESS,
    iMMK_MUTE_L,        iKEY_TYPE_MMK|iMMK_MUTE_H,
    iSTD_C,             iKEY_TYPE_STD|iKEY_PRESS,  
    iSTD_NULL,          iKEY_TYPE_INVALID,         
    iMMK_VOLUMEDOWN_L,  iKEY_TYPE_MMK|iMMK_VOLUMEDOWN_H, 
    iSTD_3,             iKEY_TYPE_STD|iKEY_PRESS,              
// C4
    iMODE_KEY_24G,      iKEY_TYPE_SPECIAL, 
    iMODE_KEY_USB,      iKEY_TYPE_SPECIAL,
    iSTD_F,             iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_G,             iKEY_TYPE_STD|iKEY_PRESS,    
    iSTD_V,             iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_B,             iKEY_TYPE_STD|iKEY_PRESS,  
    iSTD_5,             iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_4,             iKEY_TYPE_STD|iKEY_PRESS,   
// C5
    iSTD_U,             iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_Y,             iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_J,             iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_H,             iKEY_TYPE_STD|iKEY_PRESS,  
    iSTD_M,             iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_N,             iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_6,             iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_7,             iKEY_TYPE_STD|iKEY_PRESS,
// C6
    iSTD_I,             iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_SQUARE_BRACKET_RIGHT, iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_K,             iKEY_TYPE_STD|iKEY_PRESS,
    iMMK_PREV_TRACK_L,  iKEY_TYPE_MMK|iMMK_PREV_TRACK_H,
    iSTD_COMMA,         iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_KEYBOARD_RO,   iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_ADD,           iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_8,             iKEY_TYPE_STD|iKEY_PRESS,
// C7
    iSTD_O,             iKEY_TYPE_STD|iKEY_PRESS,
    iMMK_PLAY_PAUSE_L,  iKEY_TYPE_MMK|iMMK_PLAY_PAUSE_H,
    iSTD_L,             iKEY_TYPE_STD|iKEY_PRESS,   
    iSTD_NULL,          iKEY_TYPE_INVALID,
    iSTD_FULLSTOP,      iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_APP,           iKEY_TYPE_STD|iKEY_PRESS,
    iMMK_NEXT_TRACK_L,  iKEY_TYPE_MMK|iMMK_NEXT_TRACK_H,
    iSTD_9,             iKEY_TYPE_STD|iKEY_PRESS,    
// C8
    iSTD_P,             iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_SQUARE_BRACKET_LEFT, iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_SEMICOLON,     iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_QUOTATION_SIGN,iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_EUROPE1,       iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_SLASH,         iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_SUBTRACT,      iKEY_TYPE_STD|iKEY_PRESS,    
    iSTD_0,             iKEY_TYPE_STD|iKEY_PRESS,
// C9
    iSTD_SCROLL,        iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_NULL,          iKEY_TYPE_INVALID,
    iSTD_NULL,          iKEY_TYPE_INVALID,
    iSTD_NULL,          iKEY_TYPE_STD|iKEY_PRESS|iLEFT_ALT,
    iSTD_NULL,          iKEY_TYPE_INVALID,
    iSTD_NULL,          iKEY_TYPE_STD|iKEY_PRESS|iRIGHT_ALT,
    iSTD_NULL,          iKEY_TYPE_INVALID,
    iSTD_SNAPSHOT,      iKEY_TYPE_STD|iKEY_PRESS,
// C10
    iSTD_KEYBOARD_YEN,  iKEY_TYPE_STD|iKEY_PRESS,
    iKEY_FX_LOCK,       iKEY_TYPE_SPECIAL,
    iSTD_BACKSLASH,     iKEY_TYPE_STD|iKEY_PRESS,
    iKEY_KEY_LOCK,      iKEY_TYPE_SPECIAL,
    iSTD_ENTER,         iKEY_TYPE_STD|iKEY_PRESS,
    iMMK_CALCULATOR_L,  iKEY_TYPE_MMK|iMMK_CALCULATOR_H,
    iMMK_MAIL_L,        iKEY_TYPE_MMK|iMMK_MAIL_H,  
    iMMK_WWW_HOME_L,    iKEY_TYPE_MMK|iMMK_WWW_HOME_H,
// C11
    iSTD_NUMPAD7,       iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_NUMPAD4,       iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_NUMPAD1,       iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_SPACE,         iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_NUMLOCK,       iKEY_TYPE_STD|iKEY_PRESS,
    iLED_BRIGHT_SUB,    iKEY_TYPE_SPECIAL,
    iLED_DEL,           iKEY_TYPE_SPECIAL,
    iSTD_NULL,          iKEY_TYPE_INVALID,
// C12
    iSTD_NUMPAD8,       iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_NUMPAD5,       iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_NUMPAD2,       iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_NUMPAD0,       iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_NUMPAD_DIVIDE, iKEY_TYPE_STD|iKEY_PRESS,
    iLED_SPEED_ADD,     iKEY_TYPE_SPECIAL,
    iLED_INSET,         iKEY_TYPE_SPECIAL,  
    iSTD_NULL,          iKEY_TYPE_INVALID,
// C13
    iSTD_NUMPAD9,       iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_NUMPAD6,       iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_NUMPAD3,       iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_NUMPAD_FULLSTOP, iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_NUMPAD_MULTIPLY, iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_NUMPAD_SUBTRACT, iKEY_TYPE_STD|iKEY_PRESS,
    iLED_PAGEUP,        iKEY_TYPE_SPECIAL, 
    iLED_OFF,           iKEY_TYPE_SPECIAL,  
// C14
    iSTD_NUMPAD_ADD,    iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_KEYBOARD_NUMPAD_COMMA, iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_NUMPAD_ENTER,  iKEY_TYPE_STD|iKEY_PRESS,
    iLED_BRIGHT_ADD,    iKEY_TYPE_SPECIAL,
    iMMK_PLAY_PAUSE_L,  iKEY_TYPE_MMK|iMMK_PLAY_PAUSE_H,
    iLED_SPEED_SUB,     iKEY_TYPE_SPECIAL,
    iLED_HOME,          iKEY_TYPE_SPECIAL,    
    iLED_END,           iKEY_TYPE_SPECIAL,  
// C15
    iSTD_NULL,          iKEY_TYPE_INVALID,
    iSTD_NULL,          iKEY_TYPE_STD|iKEY_PRESS|iLEFT_SHIFT,
    iSTD_NULL,          iKEY_TYPE_STD|iKEY_PRESS|iRIGHT_SHIFT,
    iMMK_VOLUMEDOWN_L,  iKEY_TYPE_MMK|iMMK_VOLUMEDOWN_H,
    iMMK_VOLUMEUP_L,    iKEY_TYPE_MMK|iMMK_VOLUMEUP_H,
    iMMK_NEXT_TRACK_L,  iKEY_TYPE_MMK|iMMK_NEXT_TRACK_H,
    iMMK_PREV_TRACK_L,  iKEY_TYPE_MMK|iMMK_PREV_TRACK_H,
    iMMK_MUTE_L,        iKEY_TYPE_MMK|iMMK_MUTE_H,
// C16
    iSTD_PAUSE,         iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_KEYBOARD_LANG2,iKEY_TYPE_STD|iKEY_PRESS,//iSTD_NULL,          iKEY_TYPE_INVALID,  
    iSTD_KEYBOARD_LANG1,iKEY_TYPE_STD|iKEY_PRESS,//iSTD_NULL,          iKEY_TYPE_INVALID,        
    iMODE_KEY_FN,       iKEY_TYPE_SPECIAL,//iSTD_NULL,          iKEY_TYPE_INVALID,
    iSTD_NULL,          iKEY_TYPE_STD|iKEY_PRESS|iRIGHT_CTRL,
    iKEY_WIN_LOCK,      iKEY_TYPE_SPECIAL,
    iSTD_NULL,          iKEY_TYPE_STD|iKEY_PRESS|iLEFT_CTRL,
    iMMK_STOP_L,        iKEY_TYPE_MMK|iMMK_STOP_H,
// C17
    iMODE_KEY_BT0,      iKEY_TYPE_SPECIAL,
    iLED_MODE_CYC_ADD,  iKEY_TYPE_SPECIAL,
    iSTD_A,             iKEY_TYPE_STD|iKEY_PRESS,
    iKEY_DEFAULT,       iKEY_TYPE_SPECIAL,
    iSTD_Z,             iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_NULL,          iKEY_TYPE_INVALID, 
    iSTD_WAVE,          iKEY_TYPE_STD|iKEY_PRESS,
    iSTD_1,             iKEY_TYPE_STD|iKEY_PRESS
};
//-----------------------------------------------------------------------------;

#endif
