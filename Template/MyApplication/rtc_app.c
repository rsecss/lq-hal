#include "rtc_app.h"

RTC_TimeTypeDef time = {0};     // 定义时间结构体
RTC_DateTypeDef date = {0};     // 定义日期结构体

/**
 * @brief   RTC 模块初始化函数
 * 
 * @param   无
 * @retval  无
 */
void rtc_proc()
{
    HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);  // 获取当前的 RTC 时间
    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);  // 获取当前的 RTC 日期
}

