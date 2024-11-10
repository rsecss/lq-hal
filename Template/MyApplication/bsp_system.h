#ifndef __BSP_SYSTEM_H
#define __BSP_SYSTEM_H

#include "main.h"
#include "usart.h"
#include "lcd.h"
#include "stdio.h"   
#include "string.h"       
#include "stdarg.h"         /* 可变参数函数 */

#include "scheduler.h"      /* 调度器相关的函数 */
#include "led_app.h"        /* led 相关的函数 */
#include "key_app.h"        /* 四行代码版本 */
#include "lcd_app.h"        /* lcd 相关的函数 */
#include "usart_app.h"      /* 串口中断 + 超时解析 */
#include "adc_app.h"        /* adc 相关的函数 */

extern uint16_t uart_rx_index;
extern uint32_t uart_rx_ticks;
extern uint8_t uart_rx_buffer[128];
extern uint32_t adc_dma_buffer[2][50];
extern float adc_value[2];

#endif
