#include "adc_app.h"

uint32_t adc_dma_buffer[2][50];
float adc_value[2];

/**
 * @brief       ADC 转换调度函数
 * @param       无
 * @retval      无
 */
void adc_proc()
{
    for (uint8_t i = 0; i < 50; i++)
    {
        adc_value[0] += (float)adc_dma_buffer[0][i];
        adc_value[1] += (float)adc_dma_buffer[1][i];
    }

    adc_value[0] = adc_value[0] / 50 * 3.3f / 4096;
    adc_value[1] = adc_value[1] / 50 * 3.3f / 4096;
}
