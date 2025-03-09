#include "tim_app.h"

/**
 * @brief   设置 PWM 占空比
 * 
 * @note    此函数用于设置 TIMX 的 CHX 通道的 PWM 占空比。占空比的范围为 0%~100%
 * 
 * @param   Duty 占空比，范围为 0.0~100.0
 * @param   channel 定时器通道号（1 对应 htim16，2 对应 htim17）
 * @retval  无
 */
void pwm_set_duty(float duty,uint8_t channel)
{
    switch(channel)
    {
        case 1:
            TIM16->CCR1 = (TIM16->ARR + 1) * (duty / 100.0f);
            break;
        case 2:
            TIM17->CCR1 = (TIM17->ARR + 1) * (duty / 100.0f);
            break;
        default:
            break;
    }
}

/**
 * @brief   设置 PWM 频率
 * 
 * @param   frequency 目标频率（Hz）
 * @param   channel 定时器通道号（1 对应 htim16，2 对应 htim17）
 */
void pwm_set_frequency(int frequency, uint8_t channel)
{
    uint32_t clk = HAL_RCC_GetPCLK2Freq();  // 获取 PCLK2 时钟频率
    if ((RCC->CFGR & RCC_CFGR_PPRE2) != RCC_CFGR_PPRE2_DIV1)
    {
        clk *= 2;
    }
    
    float duty = 0;
    uint32_t prescaler = 1;
    uint32_t arr_value = 0;
    
    // 计算ARR和预分频器
    arr_value = clk / (frequency * prescaler) - 1;
    
    // 如果 ARR 超出范围，调整预分频器
    if (arr_value > 65535)
    {
        prescaler = (arr_value + 65535) / 65535;
        arr_value = clk / (frequency * prescaler) - 1;
        // printf("Using prescaler = %d\r\n", prescaler);
    }

    // printf("Target Frequency = %d Hz\r\n", frequency);
    // printf("Clock = %d Hz\r\n", clk);
    // printf("ARR = %d\r\n", arr);
    
    switch (channel)
    {
        case 1:
            /* 保存原占空比 */
            duty = (float)TIM16->CCR1 / (TIM16->ARR + 1);
            // printf("Original duty for channel 1 = %.3f\r\n", duty);
            
            /* 设置新的预分频值和 ARR */
            __HAL_TIM_SET_PRESCALER(&htim16, prescaler-1);
            TIM16->ARR = arr_value;
            // printf("New TIM16->ARR = %d\r\n", TIM16->ARR);
            
            /* 保持占空比不变 */
            TIM16->CCR1 = (TIM16->ARR + 1) * duty;
            // printf("New TIM16->CCR1 = %d\r\n", TIM16->CCR1);
            
            /* 更新寄存器 */
            TIM16->EGR = TIM_EGR_UG;
            break;
            
        case 2:
            /* 保存原占空比 */
            duty = (float)TIM17->CCR1 / (TIM17->ARR + 1);
            // printf("Original duty for channel 2 = %.3f\r\n", duty);
            
            /* 设置新的预分频值和 ARR */
            __HAL_TIM_SET_PRESCALER(&htim17, prescaler-1);
            TIM17->ARR = arr_value;
            // printf("New TIM17->ARR = %d\r\n", TIM17->ARR);
            
            /* 保持占空比不变 */
            TIM17->CCR1 = (TIM17->ARR + 1) * duty;
            // printf("New TIM17->CCR1 = %d\r\n", TIM17->CCR1);
            
            /* 更新寄存器 */
            TIM17->EGR = TIM_EGR_UG;
            break;
        default:
            printf("Invalid channel number\r\n");
            break;
    }
}

uint32_t tim_ic_buffer[64] = {0};            // 定义存储输入捕获值的缓冲区
uint32_t tim_ic_val = 0;            // 最终计算得到的输入捕获值
static const float unit_time = 1000000.0f;   // 单位时间 1s
/**
 * @brief       TIM_IC 输入捕获处理函数  
 * 
 * @param       无
 * @retval      无
 */
void tim_ic_proc()
{
    uint32_t tim_ic_temp = 0;           // 临时存储输入捕获计算的中间值

    /* 计算缓冲区捕获值的总和 */
    for (uint8_t i = 0; i < 64; i++)
    {
        tim_ic_temp += tim_ic_buffer[i];
    }

    /* 计算平均值 */
    tim_ic_temp /= 64;

    /* 计算频率值，单位为 Hz */
    tim_ic_val = (int)(unit_time / (float)tim_ic_temp);

    /* 将计算得到的频率值限制在 500 到 20000 之间 */
    limit_filter(&tim_ic_val, 1, 500, 20000);
}
