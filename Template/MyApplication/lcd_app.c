#include "lcd_app.h"

/* 底层 */
/**
 * @brief       在 lcd 指定行显示格式化后的字符串
 * @param       line 
 * @param       format 
 * @param       ... 
 * @retval      无
 * @note        示例用法:
 * @code
 * lcd_sprintf(0, "Temperature: %d", temperature);
 * @endcode
 */
void lcd_sprintf(uint8_t Line, char* format, ...)
{
    char string[21];
    va_list arg;
    va_start(arg, format);
    vsprintf(string, format, arg);
    va_end(arg);
    LCD_DisplayStringLine(Line, (u8*)string);
}

/* 逻辑层 */
/**
 * @brief       lcd 测试程序
 * @param       无
 * @retval      无
 */
void lcd_proc()
{
    static uint8_t count = 0;
    lcd_sprintf(Line0, "Test LCD");
    lcd_sprintf(Line1, "Count: %d", count++);
    lcd_sprintf(Line2, "ADC1:%.2f",adc_value[0]);
    lcd_sprintf(Line3, "ADC2:%.2f",adc_value[1]);
}
