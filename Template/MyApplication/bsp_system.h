#ifndef BSP_SYSTEM_H
#define BSP_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

/* 应用层模块头文件 */
#include "lcd.h"
#include "led_app.h"        /* led 相关的函数 */
#include "key_app.h"        /* 四行代码版本 */
#include "lcd_app.h"        /* lcd 相关的函数 */
#include "usart_app.h"      /* 串口中断 + 超时解析 */
#include "adc_app.h"        /* adc 相关的函数 */

#ifdef __cplusplus
}
#endif

#endif /* BSP_SYSTEM_H */
