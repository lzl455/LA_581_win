/* ----------------------------------------------------------------------------
 * Copyright (c) 2020-2030 OnMicro Limited. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distributon.
 *   3. Neither the name of OnMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * -------------------------------------------------------------------------- */
/**
 * @file     main.c
 * @brief    main entry
 * @date     18 Sept. 2023
 * @author   OnMicro SW Team
 *
 * @version
 * Version 1.0
 *  - Initial release
 *
 * @{
 */
/*******************************************************************************
 * INCLUDES
 */
#include "om_driver.h"
#include "evt.h"
#include <string.h>

#include "mouse.h"
#include "ex2_app_mouse.h"
#include "main_usb.h"
#include "main_24g.h"
#include "ex2_hal.h"

//-----------------------------------------------------------------------------;
//常量定义区
//

//-----------------------------------------------------------------------------;
//变量定义区
//
bool                        brf_rate;
bool                        bmode_swtich_valid;

volatile uint8_t            ble_24g_ctrl;
volatile uint8_t            sys_ctrl;

uint8_t                     sys_type;
device_type_t               device_type;
device_type_t               bt_index;           // 硬件切换模式，蓝牙多通道情况会使用到

volatile uint32_t           sys_tick_count;
uint32_t                    rtc_init_value;
uint32_t                    rtc_cur_value;
uint32_t                    rtc_temp_value;

//----------------------------------------------
// 鼠标键盘数据相关寄存器
//
uint8_t                     m_data_flag;
uint8_t                     kb_led_status;
uint8_t                     hint_led_status;

uint8_t                     m_tx_count;
uint8_t                     key_tx_count;
uint8_t                     key_ex_tx_count;
uint8_t                     mmk_tx_count;
uint8_t                     def_tx_count;

uint8_t                     mouse_data_key;
uint16_t                    mouse_x;
uint16_t                    mouse_y;
uint8_t                     mouse_data_z;

uint8_t                     std_key[22];
uint8_t                     mmk_data_l;
uint8_t                     mmk_data_h;

uint8_t                     wheel_data;
//
//----------------------------------------------
// 鼠标ID
//
uint8_t                     kb_bd_addr[6];
extern uint8_t              mode_status_bak;
extern uint8_t              usb_mode_switch;
//-----------------------------------------------------------------------------;
//外部引用区
//
extern void rf_freq_make(void);
extern pmu_lpm_t pm_checker_callback(void);
extern uint32_t adv_interval_ms;
extern void bat_check(void);
/******************************************************************************
* 函数名称: encoder_init()
* 功能描述: 编码器初始化
* 输入参数: 无
* 输出参数: 无
* 说    明: 
******************************************************************************/
void encoder_init(void)
{
    drv_pmu_power_set(PMU_POWER_ID_RTC2M, PMU_POWER_ON);
}

/******************************************************************************
* 函数名称: mouse_periph_wakeup_enable()
* 功能描述: 外设唤醒设置
* 输入参数: 唤醒屏蔽位
* 输出参数: 无
* 说    明: 
******************************************************************************/
void mouse_periph_wakeup_enable(uint8_t mask)
{
    OM_CRITICAL_BEGIN();
    drv_gpio_set_trig(OM_GPIO0, (1<<KEY_R0_PIN)|(1<<KEY_R1_PIN)|(1<<KEY_R2_PIN)|(1<<KEY_R3_PIN)| \
                                (1<<KEY_R4_PIN)|(1<<KEY_R5_PIN)|(1<<KEY_R6_PIN)|(1<<KEY_R7_PIN), \
                                 GPIO_TRIG_NONE);
    NVIC_ClearPendingIRQ(WAKEUP_GPIO0_IRQn);
    drv_pmu_clear_wakeup();

    if(mask & WAKEUP_BTN_MASK) { /* button only faling edge wakeup*/
        mLED_VDD_DIS;
        MATRX_ALL_LOW;
        MATRX_C18_LOW;                          // C18/C19 也是矩阵列: 原来没拉低, 下面又去掉了上拉, 处于浮空输入(无模拟输入模式会漏电), 且这两列按键无法唤醒
        MATRX_C19_LOW;                          // LED_VDD已关, NUM/CAPS灯不会亮
        KEY_EX_LOW;
        OM_PMU->PAD_PU_CTRL_1 = 0x80000000;     // 仅P37上拉(输入高); 空脚输出低不加上拉

        drv_gpio_set_trig(OM_GPIO0, (1<<KEY_R0_PIN)|(1<<KEY_R1_PIN)|(1<<KEY_R2_PIN)|(1<<KEY_R3_PIN)| \
                                    (1<<KEY_R4_PIN)|(1<<KEY_R5_PIN)|(1<<KEY_R6_PIN)|(1<<KEY_R7_PIN), \
                                     GPIO_TRIG_FALLING_EDGE);
        drv_pmu_wakeup_pin_set(KEY_R0_PIN, PMU_PIN_WAKEUP_TYPE_FALL);
        drv_pmu_wakeup_pin_set(KEY_R1_PIN, PMU_PIN_WAKEUP_TYPE_FALL);
        drv_pmu_wakeup_pin_set(KEY_R2_PIN, PMU_PIN_WAKEUP_TYPE_FALL);
        drv_pmu_wakeup_pin_set(KEY_R3_PIN, PMU_PIN_WAKEUP_TYPE_FALL);
        drv_pmu_wakeup_pin_set(KEY_R4_PIN, PMU_PIN_WAKEUP_TYPE_FALL);
        drv_pmu_wakeup_pin_set(KEY_R5_PIN, PMU_PIN_WAKEUP_TYPE_FALL);
        drv_pmu_wakeup_pin_set(KEY_R6_PIN, PMU_PIN_WAKEUP_TYPE_FALL);
        drv_pmu_wakeup_pin_set(KEY_R7_PIN, PMU_PIN_WAKEUP_TYPE_FALL);

        NVIC_SetPriority(WAKEUP_GPIO0_IRQn, RTE_GPIO0_IRQ_PRIORITY);
        NVIC_EnableIRQ(WAKEUP_GPIO0_IRQn);
    }
    else {
        drv_pmu_wakeup_pin_set(KEY_R0_PIN, PMU_PIN_WAKEUP_TYPE_DISABLE);  
        drv_pmu_wakeup_pin_set(KEY_R1_PIN, PMU_PIN_WAKEUP_TYPE_DISABLE);
        drv_pmu_wakeup_pin_set(KEY_R2_PIN, PMU_PIN_WAKEUP_TYPE_DISABLE);
        drv_pmu_wakeup_pin_set(KEY_R3_PIN, PMU_PIN_WAKEUP_TYPE_DISABLE);
        drv_pmu_wakeup_pin_set(KEY_R4_PIN, PMU_PIN_WAKEUP_TYPE_DISABLE);
        drv_pmu_wakeup_pin_set(KEY_R5_PIN, PMU_PIN_WAKEUP_TYPE_DISABLE);
        drv_pmu_wakeup_pin_set(KEY_R6_PIN, PMU_PIN_WAKEUP_TYPE_DISABLE);
        drv_pmu_wakeup_pin_set(KEY_R7_PIN, PMU_PIN_WAKEUP_TYPE_DISABLE);

        NVIC_DisableIRQ(WAKEUP_GPIO0_IRQn);

        OM_PMU->PAD_PU_CTRL_1 = 0x800AAA20;     // P37上拉(输入高); 空脚P20/P21/P36输出低, 去掉上拉
        MATRX_ALL_IN;
        led_status_hint();
    }

    OM_CRITICAL_END();
}

/******************************************************************************
* 函数名称: main()
* 功能描述: 主程序
* 输入参数: 无
* 输出参数: 无
* 说    明: 
******************************************************************************/
int main(void)
{
    /* Initializing RCC */
    drv_rcc_init();

    drv_wdt_init(OM_WDT);                      
    drv_wdt_keep_alive(OM_WDT);

    drv_flash_read(CFG_START_SYNC_WORD_ADDR, kb_bd_addr+2, 4);
    kb_bd_addr[0] = iVERSION_ID_L;
    kb_bd_addr[1] = iVERSION_ID_H;
#if (0)
    kb_bd_addr[2]=0x02;                      // 调试使用，固定ID，在接收器也固定ID
    kb_bd_addr[3]=0x02;                      // 直接通信避免串码
    kb_bd_addr[4]=0x02;
#endif

    #if (RTE_ICACHE)
    drv_icache_enable();
    #endif

    OM_RCC->CLKGATE |= RCC_DEV_CLK_GATE_GPIO_CLK_GATE_MASK;
//------------------------------------------------------------------------------
// 不同个案，需要根据实际原理图设置初始化IO状态
    OM_PMU->PAD_PU_CTRL = 0x00AAAA00;
    OM_PMU->PAD_PU_CTRL_1 = 0x800AAA20;         // P37上拉(输入高); 空脚P20/P21/P36改输出低, 去掉上拉
    OM_PMU->PAD_PD_CTRL = 0x3000000C;
    OM_PMU->PAD_BUF_EN &= ~LVD_PIN_MASK;

    OM_GPIO0->DATAOUT = 0x9FF4FFFF;             // 空脚P20/P21/P35/P36 输出低
    OM_GPIO0->OUTENSET = 0x640F0003;
    OM_GPIO0->OUTENCLR = ~0x640F0003;
    device_type = iDEVICE_TYPE_ERR;

                                               
    OM_RCC->CLKGATE &= ~RCC_DEV_CLK_GATE_SWD_CLK_GATE_MASK; // disable jtag
    hint_led_status = 0;
    kb_led_status = 0;
    mouse_periph_wakeup_enable(WAKEUP_ALL_DISABLE);
    if(RCC_RST_TYPE_DEEP_SLEEP == (OM_RCC->RSTCTRL & 0x07))
    {
        if(OM_PMU->SW_RSVD1 & PMU_SW_RSVD1_POWER_OFF)
        {                                     
            while(1)
            {                                  
                if(0 != USB_5V_PIN)             
                    break;

                hint_led_status = bled_low;
                led_status_hint();
                delayus(500000);
                mLED_VDD_DIS;
                mouse_periph_wakeup_enable(WAKEUP_SLEEP_LEVEL2);
                drv_pmu_set_low_power_mode(PMU_LPM_DEEP_SLEEP);
            }
            OM_PMU->SW_RSVD1 &= ~PMU_SW_RSVD1_POWER_OFF;
        }
    }

    ble_24g_ctrl &= ~bom6239_sync_ok;
    rtc_init_value = 0;

    encoder_init();
    bat_init();

    bargb_valid = false;
    laod_flash_cfg();

    scan_mode_swtich(iMODE_CHECK_POWERUP);

    memset(matrix_status, 0x00, iMATRIX_COL_NUM);
    memset(key_fn_status, 0x00, 128);

    #if(RF_RATE_2MHz==RF_RATE)                  
        brf_rate = true;
    #else
        brf_rate = false;
    #endif

    if((RCC_RST_TYPE_POWER_ON == (OM_RCC->RSTCTRL & 0x07))
        || (iSCRATCH_BOOT_POWER == OM_PMU->SW_RSVD7))
    {
        OM_RCC->CLKGATE |= RCC_DEV_CLK_GATE_SWD_CLK_GATE_MASK; // disable jtag
        delayus(50000);                       

        sys_ctrl = 0;
        OM_PMU->SW_RSVD7 = 0;
        OM_RCC->CLKGATE &= ~RCC_DEV_CLK_GATE_SWD_CLK_GATE_MASK; // disable jtag

        //OM_PMU->SW_RSVD1 = 0;
        OM_PMU->SW_RSVD1 = (device_type&PMU_SW_RSVD1_RF_MDOE_MASK);
        //OM_PMU->SW_RSVD1 |= iDEVICE_TYPE_BT0; 
        //OM_PMU->SW_RSVD1 |= iDEVICE_TYPE_USB;
        //OM_PMU->SW_RSVD1 |= iDEVICE_TYPE_24G;
        OM_PMU->SW_RSVD1 |= (0<<8);
        OM_PMU->SW_RSVD1 |= ((0&0x0F)<<24);

        OM_PMU->SW_RSVD2 = iSEARCH_SYNC_WORD;
        sync_word = iSEARCH_SYNC_WORD;
        rf_ctrl_reg = (bsearch_dongle|brf_sync_time|brf_sync_status);

        sys_tick_count = 0;
        ble_24g_ctrl &= ~bom6239_conning;
        OM_PMU->SW_RSVD1 &= ~RSVD1_RF_CONN_EN;
    }
    else
    {
soft_reset_process:
        sys_ctrl |= bmcu_reset;

        ble_24g_ctrl &= ~bom6239_conning;
        if(OM_PMU->SW_RSVD1 & RSVD1_RF_CONN_EN)
            ble_24g_ctrl |= bom6239_conning;
        OM_PMU->SW_RSVD1 &= ~RSVD1_RF_CONN_EN;

        if(iDEVICE_TYPE_24G == device_type)
        {
            if(iSEARCH_SYNC_WORD == OM_PMU->SW_RSVD2)
            {
                rf_ctrl_reg |= (bsearch_dongle|brf_sync_time|brf_sync_status);
                sync_word = iSEARCH_SYNC_WORD;
            }
            else
            {
                rf_ctrl_reg |= (brf_sync_time|brf_sync_status);
                sync_word = OM_PMU->SW_RSVD2;
                rf_freq_make();
            }
        }
    }

    while(1)
    {
        sys_tick_count = 0;
        bmode_swtich_valid = false;
        rtc_init();
        b24g_nromal = false;
        flash_load_ble_addr();
        device_led_on();

        key_ctrl &= ~bwin_mac_sataus;
        if(sys_mode&(1<<(device_type&0x07)))
            key_ctrl |= bwin_mac_sataus;
        
        bat_value = 100;
        if(iDEVICE_TYPE_USB == device_type)
        {
            mouse_usb_init();
            main_usb();
        }
        else if(iDEVICE_TYPE_24G == device_type)
        {
            usb_mode_switch |= (1<<0);
            flash_save_status();
            mMCU_MODE_NORMAL;
            mouse_24g_init();
            main_24g();
        }
        else //if(iDEVICE_TYPE_BT2 >= device_type)
        {
            usb_mode_switch |= (1<<0);
            flash_save_status();
            OM_RCC->CLKCTRL = RCC_DIV_CLK_SEL_4;
            __NOP();__NOP();__NOP();__NOP();   

            device_type &= 0x03;
            evt_init();

            if(0xFF == ble_id[device_type])
            {
                ble_24g_ctrl |= bom6239_conning;

                flash_read(iFLASH_BLE_ROLL_ADDR, 4);
                ble_id[device_type] = flash_fifo[device_type+1];
            }

            if(ble_24g_ctrl&bom6239_conning)
            {                                   // 蓝牙配对
                ble_24g_ctrl &= ~bom6239_conn_ok;
                ble_id[device_type]++;
                if(0xFF == ble_id[device_type])
                    ble_id[device_type]++;
            }
            kb_bd_addr[4] = ble_id[device_type];
            kb_bd_addr[5] &= 0xFC;
            kb_bd_addr[5] |= device_type;

            ex2_app_mouse_ble_init();
            while(1)
            {
                evt_schedule();
                if(true == bmode_swtich_valid)
                {
                    ex2_hal_uninit();
                    break;                      // 切换模式，返回主程序
                }

                OM_CRITICAL_BEGIN();
                // if no event, do power manage
                if (evt_get_all() == 0U)
                {
                    drv_pmu_set_low_power_mode(pm_checker_callback());
                }
                OM_CRITICAL_END();
            }
        }
    }
}

/** @} */
