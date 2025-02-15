#ifndef BSP_SYSTEM_H
#define BSP_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

/* 头文件 */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/* 应用层头文件 */
#include "main.h"
#include "lcd.h"
#include "usart.h"
#include "system.h"
#include "scheduler.h"
#include "led_app.h"        /* led 相关的函数 */
#include "key_app.h"        /* 四行代码版本 */
#include "lcd_app.h"        /* lcd 相关的函数 */
#include "usart_app.h"      /* 串口中断 + 超时解析 */
#include "adc_app.h"        /* adc 相关的函数 */


/* 全局变量，数组声明 */
extern uint16_t uart_rx_index;
extern uint32_t uart_rx_ticks;
extern uint8_t uart_rx_buffer[128];
extern uint32_t adc_dma_buffer[2][50];
extern float adc_value[2];

#ifdef __cplusplus
}
#endif

#endif /* BSP_SYSTEM_H */
