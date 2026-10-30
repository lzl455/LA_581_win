/*******************************************************************************
* Copyright (c) 2013, SHENZHEN SEMITEK Co.,LTD.
* All rights reserved.
*
* 文件名称: _LED_TBL_
*
* 功能说明:
* 修改说明:
*******************************************************************************/
#ifndef _LED_LOGO_TBL_
#define _LED_LOGO_TBL_
#include "led.h"

//=============================================================================;
// 配置引用区                                                                  ;
//=============================================================================;

//-----------------------------------------------------------------------------;
//配置常量定义区
//
#define NULL_0                  127
#define NULL_1                  126
#define NULL_2                  125
#define NULL_3                  124
#define NULL_4                  123
#define NULL_5                  122
#define NULL_6                  121
#define NULL_7                  120

//-----------------------------------------------------------------------------;
//配置区
//
#define RGB_COLOR_MAX         8
#define RGB_COLOR_INIT        7

unsigned char       const   rgb_en_tbl[]={
    iPWM_R,
    iPWM_G,
    iPWM_B,
    iPWM_R|iPWM_B,
    iPWM_R|iPWM_G,
    iPWM_G|iPWM_B,
    iPWM_W,
    iPWM_R|iPWM_G|iPWM_B,

    iPWM_R,
    iPWM_G,
    iPWM_B,
    iPWM_R|iPWM_B,
    iPWM_R|iPWM_G,
    iPWM_G|iPWM_B,
    iPWM_W,
    iPWM_R|iPWM_G|iPWM_B,

    0x0F,
};

unsigned char       const   signle_en_tbl[]={
    iPWM_R,
    iPWM_R|iPWM_G,
    iPWM_G,
    iPWM_G|iPWM_B,
    iPWM_B,
    iPWM_R|iPWM_B,
    iPWM_W,
    iPWM_R|iPWM_G|iPWM_B,

    iPWM_R,
    iPWM_R|iPWM_G,
    iPWM_G,
    iPWM_G|iPWM_B,
    iPWM_B,
    iPWM_R|iPWM_B,
    iPWM_W,
    iPWM_R|iPWM_G|iPWM_B,

    0x0F,
};

// 按键 -> 扩散(LYKS)涟漪起点灯号: 按键盘文版各键横向位置把整板等分6段, 5=最左(Esc/Tab) ... 0=数字区
// 下标顺序同 matrixkey_win_tbl(C2~C17 各8键); 0xFF = 空位/本键盘无此键, 不起涟漪
unsigned char       const       led_single_table[] =
{
// C2
    5,// iSTD_W, iKEY_TYPE_STD|iKEY_PRESS
    5,// iSTD_CAPITAL, iKEY_TYPE_STD|iKEY_PRESS
    5,// iSTD_S, iKEY_TYPE_STD|iKEY_PRESS
    5,// iSTD_QUROPE2, iKEY_TYPE_STD|iKEY_PRESS
    5,// iSTD_X, iKEY_TYPE_STD|iKEY_PRESS
    0xFF,// iSTD_NULL, iKEY_TYPE_INVALID
    5,// iSTD_F1, iKEY_TYPE_STD|iKEY_PRESS
    5,// iSTD_2, iKEY_TYPE_STD|iKEY_PRESS
// C3
    5,// iSTD_E, iKEY_TYPE_STD|iKEY_PRESS
    4,// iSTD_F3, iKEY_TYPE_STD|iKEY_PRESS
    5,// iSTD_D, iKEY_TYPE_STD|iKEY_PRESS
    4,// iSTD_F4, iKEY_TYPE_STD|iKEY_PRESS
    4,// iSTD_C, iKEY_TYPE_STD|iKEY_PRESS
    0xFF,// iSTD_NULL, iKEY_TYPE_INVALID
    5,// iSTD_F2, iKEY_TYPE_STD|iKEY_PRESS
    5,// iSTD_3, iKEY_TYPE_STD|iKEY_PRESS
// C4
    4,// iSTD_R, iKEY_TYPE_STD|iKEY_PRESS
    4,// iSTD_T, iKEY_TYPE_STD|iKEY_PRESS
    4,// iSTD_F, iKEY_TYPE_STD|iKEY_PRESS
    4,// iSTD_G, iKEY_TYPE_STD|iKEY_PRESS
    4,// iSTD_V, iKEY_TYPE_STD|iKEY_PRESS
    4,// iSTD_B, iKEY_TYPE_STD|iKEY_PRESS
    4,// iSTD_5, iKEY_TYPE_STD|iKEY_PRESS
    4,// iSTD_4, iKEY_TYPE_STD|iKEY_PRESS
// C5
    3,// iSTD_U, iKEY_TYPE_STD|iKEY_PRESS
    4,// iSTD_Y, iKEY_TYPE_STD|iKEY_PRESS
    3,// iSTD_J, iKEY_TYPE_STD|iKEY_PRESS
    4,// iSTD_H, iKEY_TYPE_STD|iKEY_PRESS
    3,// iSTD_M, iKEY_TYPE_STD|iKEY_PRESS
    3,// iSTD_N, iKEY_TYPE_STD|iKEY_PRESS
    4,// iSTD_6, iKEY_TYPE_STD|iKEY_PRESS
    4,// iSTD_7, iKEY_TYPE_STD|iKEY_PRESS
// C6
    3,// iSTD_I, iKEY_TYPE_STD|iKEY_PRESS
    2,// iSTD_SQUARE_BRACKET_RIGHT, iKEY_TYPE_STD|iKEY_PRESS
    3,// iSTD_K, iKEY_TYPE_STD|iKEY_PRESS
    3,// iSTD_F6, iKEY_TYPE_STD|iKEY_PRESS
    3,// iSTD_COMMA, iKEY_TYPE_STD|iKEY_PRESS
    0xFF,// iSTD_KEYBOARD_RO, iKEY_TYPE_STD|iKEY_PRESS  (本键盘无此键)
    2,// iSTD_ADD, iKEY_TYPE_STD|iKEY_PRESS
    3,// iSTD_8, iKEY_TYPE_STD|iKEY_PRESS
// C7
    3,// iSTD_O, iKEY_TYPE_STD|iKEY_PRESS
    3,// iSTD_F7, iKEY_TYPE_STD|iKEY_PRESS
    3,// iSTD_L, iKEY_TYPE_STD|iKEY_PRESS
    0xFF,// iSTD_NULL, iKEY_TYPE_INVALID
    3,// iSTD_FULLSTOP, iKEY_TYPE_STD|iKEY_PRESS
    2,// iSTD_APP, iKEY_TYPE_STD|iKEY_PRESS
    3,// iSTD_F8, iKEY_TYPE_STD|iKEY_PRESS
    3,// iSTD_9, iKEY_TYPE_STD|iKEY_PRESS
// C8
    3,// iSTD_P, iKEY_TYPE_STD|iKEY_PRESS
    2,// iSTD_SQUARE_BRACKET_LEFT, iKEY_TYPE_STD|iKEY_PRESS
    3,// iSTD_SEMICOLON, iKEY_TYPE_STD|iKEY_PRESS
    2,// iSTD_QUOTATION_SIGN, iKEY_TYPE_STD|iKEY_PRESS
    2,// iSTD_EUROPE1, iKEY_TYPE_STD|iKEY_PRESS
    2,// iSTD_SLASH, iKEY_TYPE_STD|iKEY_PRESS
    3,// iSTD_SUBTRACT, iKEY_TYPE_STD|iKEY_PRESS
    3,// iSTD_0, iKEY_TYPE_STD|iKEY_PRESS
// C9
    1,// iSTD_SCROLL, iKEY_TYPE_STD|iKEY_PRESS
    0xFF,// iSTD_NULL, iKEY_TYPE_INVALID
    0xFF,// iSTD_NULL, iKEY_TYPE_INVALID
    5,// iSTD_NULL, iKEY_TYPE_STD|iKEY_PRESS|iLEFT_ALT
    0xFF,// iKEY_WHEEL, iKEY_TYPE_SPECIAL  (本键盘无此键)
    3,// iSTD_NULL, iKEY_TYPE_STD|iKEY_PRESS|iRIGHT_ALT
    0xFF,// iSTD_NULL, iKEY_TYPE_INVALID
    1,// iSTD_SNAPSHOT, iKEY_TYPE_STD|iKEY_PRESS
// C10
    0xFF,// iSTD_KEYBOARD_YEN, iKEY_TYPE_STD|iKEY_PRESS  (本键盘无此键)
    2,// iSTD_BACKSPACE, iKEY_TYPE_STD|iKEY_PRESS
    0xFF,// iSTD_BACKSLASH, iKEY_TYPE_STD|iKEY_PRESS  (本键盘无此键)
    2,// iSTD_F11, iKEY_TYPE_STD|iKEY_PRESS
    2,// iSTD_ENTER, iKEY_TYPE_STD|iKEY_PRESS
    2,// iSTD_F12, iKEY_TYPE_STD|iKEY_PRESS
    3,// iSTD_F9, iKEY_TYPE_STD|iKEY_PRESS
    2,// iSTD_F10, iKEY_TYPE_STD|iKEY_PRESS
// C11
    0,// iSTD_NUMPAD7, iKEY_TYPE_STD|iKEY_PRESS
    0,// iSTD_NUMPAD4, iKEY_TYPE_STD|iKEY_PRESS
    0,// iSTD_NUMPAD1, iKEY_TYPE_STD|iKEY_PRESS
    4,// iSTD_SPACE, iKEY_TYPE_STD|iKEY_PRESS
    0,// iSTD_NUMLOCK, iKEY_TYPE_STD|iKEY_PRESS
    1,// iSTD_DOWN, iKEY_TYPE_STD|iKEY_PRESS
    1,// iSTD_DELETE, iKEY_TYPE_STD|iKEY_PRESS
    0xFF,// iSTD_NULL, iKEY_TYPE_INVALID
// C12
    0,// iSTD_NUMPAD8, iKEY_TYPE_STD|iKEY_PRESS
    0,// iSTD_NUMPAD5, iKEY_TYPE_STD|iKEY_PRESS
    0,// iSTD_NUMPAD2, iKEY_TYPE_STD|iKEY_PRESS
    0,// iSTD_NUMPAD0, iKEY_TYPE_STD|iKEY_PRESS
    0,// iSTD_NUMPAD_DIVIDE, iKEY_TYPE_STD|iKEY_PRESS
    1,// iSTD_RIGHT, iKEY_TYPE_STD|iKEY_PRESS
    1,// iSTD_INSERT, iKEY_TYPE_STD|iKEY_PRESS
    0xFF,// iSTD_NULL, iKEY_TYPE_INVALID
// C13
    0,// iSTD_NUMPAD9, iKEY_TYPE_STD|iKEY_PRESS
    0,// iSTD_NUMPAD6, iKEY_TYPE_STD|iKEY_PRESS
    0,// iSTD_NUMPAD3, iKEY_TYPE_STD|iKEY_PRESS
    0,// iSTD_NUMPAD_FULLSTOP, iKEY_TYPE_STD|iKEY_PRESS
    0,// iSTD_NUMPAD_MULTIPLY, iKEY_TYPE_STD|iKEY_PRESS
    0,// iSTD_NUMPAD_SUBTRACT, iKEY_TYPE_STD|iKEY_PRESS
    1,// iSTD_PAGEUP, iKEY_TYPE_STD|iKEY_PRESS
    1,// iSTD_PAGEDOWN, iKEY_TYPE_STD|iKEY_PRESS
// C14
    0,// iSTD_NUMPAD_ADD, iKEY_TYPE_STD|iKEY_PRESS
    0xFF,// iSTD_KEYBOARD_NUMPAD_COMMA, iKEY_TYPE_STD|iKEY_PRESS  (本键盘无此键)
    0,// iSTD_NUMPAD_ENTER, iKEY_TYPE_STD|iKEY_PRESS
    1,// iSTD_UP, iKEY_TYPE_STD|iKEY_PRESS
    4,// iMMK_PLAY_PAUSE_L, iKEY_TYPE_MMK|iMMK_PLAY_PAUSE_H
    1,// iSTD_LEFT, iKEY_TYPE_STD|iKEY_PRESS
    1,// iSTD_HOME, iKEY_TYPE_STD|iKEY_PRESS
    1,// iSTD_END, iKEY_TYPE_STD|iKEY_PRESS
// C15
    0xFF,// iSTD_NULL, iKEY_TYPE_INVALID
    5,// iSTD_NULL, iKEY_TYPE_STD|iKEY_PRESS|iLEFT_SHIFT
    2,// iSTD_NULL, iKEY_TYPE_STD|iKEY_PRESS|iRIGHT_SHIFT
    4,// iMMK_VOLUMEDOWN_L, iKEY_TYPE_MMK|iMMK_VOLUMEDOWN_H
    3,// iMMK_VOLUMEUP_L, iKEY_TYPE_MMK|iMMK_VOLUMEUP_H
    4,// iMMK_NEXT_TRACK_L, iKEY_TYPE_MMK|iMMK_NEXT_TRACK_H
    4,// iMMK_PREV_TRACK_L, iKEY_TYPE_MMK|iMMK_PREV_TRACK_H
    3,// iMMK_MUTE_L, iKEY_TYPE_MMK|iMMK_MUTE_H
// C16
    1,// iSTD_PAUSE, iKEY_TYPE_STD|iKEY_PRESS
    0xFF,// iSTD_KEYBOARD_LANG2, iKEY_TYPE_STD|iKEY_PRESS  (本键盘无此键)
    0xFF,// iSTD_KEYBOARD_LANG1, iKEY_TYPE_STD|iKEY_PRESS  (本键盘无此键)
    2,// iMODE_KEY_FN, iKEY_TYPE_SPECIAL
    2,// iSTD_NULL, iKEY_TYPE_STD|iKEY_PRESS|iRIGHT_CTRL
    5,// iSTD_NULL, iKEY_TYPE_STD|iKEY_PRESS|iLEFT_GUI
    5,// iSTD_NULL, iKEY_TYPE_STD|iKEY_PRESS|iLEFT_CTRL
    4,// iSTD_F5, iKEY_TYPE_STD|iKEY_PRESS
// C17
    5,// iSTD_Q, iKEY_TYPE_STD|iKEY_PRESS
    5,// iSTD_TAB, iKEY_TYPE_STD|iKEY_PRESS
    5,// iSTD_A, iKEY_TYPE_STD|iKEY_PRESS
    5,// iSTD_ESCAPE, iKEY_TYPE_STD|iKEY_PRESS
    5,// iSTD_Z, iKEY_TYPE_STD|iKEY_PRESS
    0xFF,// iSTD_NULL, iKEY_TYPE_INVALID
    5,// iSTD_WAVE, iKEY_TYPE_STD|iKEY_PRESS
    5 // iSTD_1, iKEY_TYPE_STD|iKEY_PRESS
};

unsigned char       const       led_liuguang_l_r_tbl[]={
    63, 32,  0, 32, 63, 32,  0, 32, 63, 32,  0, 32,63, 32, 0, 32,
};
unsigned char       const       led_liuguang_rgb_tbl[]={
      1,  2,  3,  4,  6,  7,  1,  2,  3,  4,  6,  7,  1,  2, 3,  4,
};

unsigned char       const   sbzl_r_tbl[]={
 60, 40, 20,  0,  0,  0,  0, 20, 40, 60, 40, 20,  0,  0,  0,  0,};
unsigned char       const   sbzl_g_tbl[]={
  0, 20, 40, 60, 40, 20,  0,  0,  0,  0, 20, 40, 60, 40, 20,  0,};
unsigned char       const   sbzl_b_tbl[]={
  0,  0,  0,  0, 20, 40, 60, 40, 20,  0,  0,  0,  0, 20, 40, 60,};
unsigned char       const   sbzl_ctrl_tbl[]={
  0,  0,  0,  1,  1,  1,  2,  2,  2,  0,  0,  0,  1,  1,  1,  2,};

//-----------------------------------------------------------------------------;

#endif



