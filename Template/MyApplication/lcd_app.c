#include <stdio.h>
#include <stdarg.h>
#include "lcd.h"
#include "bsp_system.h"

/* 底层 */
/**
 * @brief       在 lcd 指定行显示格式化后的字符串
 * @param       line 要显示字符串的 LCD 行号
 * @param       format 格式化字符串，后跟要格式化的参数
 * @param       ...
 * @retval      无
 * @note        该函数接受一个行号和一个格式化字符串（类似于 printf），格式化字符串后，将其显示在 LCD 的指定行上。
 * @code
 * lcd_sprintf(0, "Temperature: %d", temperature);
 * @endcode
 */
static void lcd_sprintf(uint8_t line, char *format, ...)
{
    char string[21] = {0};
    va_list arg;
    va_start(arg, format);
    vsprintf(string, format, arg);
    va_end(arg);
    LCD_DisplayStringLine(line, (u8 *)string);
}

/* 逻辑层 */
/**
 * @brief       lcd 测试程序
 * @param       无
 * @retval      无
 */
void lcd_proc()
{
    static uint32_t count = 0;
    lcd_sprintf(Line0, "Test LCD!!!");
    lcd_sprintf(Line1, "Count:%d", count++);
    lcd_sprintf(Line2, "ADC1:%.3f", adc_value[0]);
    lcd_sprintf(Line3, "ADC2:%.3f", adc_value[1]);
}
