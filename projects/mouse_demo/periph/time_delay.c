/*******************************************************************************
* Copyright (c) 2013, SHENZHEN SEMITEK Co.,LTD.
* All rights reserved.
*
* 文件名称: Time.c
*
* 功能说明: 
* 修改说明:
*******************************************************************************/
#include "om_driver.h"
#include "main.h"
#include "stdio.h"
//=============================================================================;
// 代码区                                                                      ;
//=============================================================================;

/******************************************************************************
* 函数名称: delayus()
* 功能描述: 微秒延时
* 输入参数: 延时时间
* 输出参数: 无
* 说    明: 
******************************************************************************/
__RAM_CODE void delayus(volatile uint32_t time)
{
    if(iDEVICE_TYPE_BT2 < device_type)
        time<<=2;

    do
    {
        __NOP();
        time--;
    }
    while(time);
}

__RAM_CODE void uart_delay(void)
{
    uint8_t time= 65;

    if(iDEVICE_TYPE_BT2 >= device_type)
        time= 14;
    do
    {
        __NOP();
    }
    while(--time);
}
/****************************************************************************
* 函数名称: UART_SendData()
* 功能说明:	串口发送函数   
* 输    入: UART_TypeDef * UARTx     UART0   UART1   UART2
*			uint8_t data    待发送的数据
* 输    出: 无
* 注意事项: 无
*****************************************************************************/
#if 1
__RAM_CODE void uart_tx(uint8_t data)
{
    OM_CRITICAL_BEGIN();

    OM_GPIO0->DATAOUTCLR = (1<<UART_PIN);
    uart_delay();
    if(data&(1<<0))
        OM_GPIO0->DATAOUTSET = (1<<UART_PIN);
    else
        OM_GPIO0->DATAOUTCLR = (1<<UART_PIN);
    uart_delay();

    if(data&(1<<1))
        OM_GPIO0->DATAOUTSET = (1<<UART_PIN);
    else
        OM_GPIO0->DATAOUTCLR = (1<<UART_PIN);
    uart_delay();

    if(data&(1<<2))
        OM_GPIO0->DATAOUTSET = (1<<UART_PIN);
    else
        OM_GPIO0->DATAOUTCLR = (1<<UART_PIN);
    uart_delay();

    if(data&(1<<3))
        OM_GPIO0->DATAOUTSET = (1<<UART_PIN);
    else
        OM_GPIO0->DATAOUTCLR = (1<<UART_PIN);
    uart_delay();

    if(data&(1<<4))
        OM_GPIO0->DATAOUTSET = (1<<UART_PIN);
    else
        OM_GPIO0->DATAOUTCLR = (1<<UART_PIN);
    uart_delay();

    if(data&(1<<5))
        OM_GPIO0->DATAOUTSET = (1<<UART_PIN);
    else
        OM_GPIO0->DATAOUTCLR = (1<<UART_PIN);
    uart_delay();

    if(data&(1<<6))
        OM_GPIO0->DATAOUTSET = (1<<UART_PIN);
    else
        OM_GPIO0->DATAOUTCLR = (1<<UART_PIN);
    uart_delay();

    if(data&(1<<7))
        OM_GPIO0->DATAOUTSET = (1<<UART_PIN);
    else
        OM_GPIO0->DATAOUTCLR = (1<<UART_PIN);
    uart_delay();

    OM_GPIO0->DATAOUTSET = (1<<UART_PIN);
    
    OM_CRITICAL_END();
    uart_delay();
}

int fputc(int ch, FILE *f)
{
    uart_tx((uint8_t)ch);
    return ch;
}
#endif

