#ifndef ADC_APP_H
#define ADC_APP_H

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t adc_dma_buffer[2][50];
extern float adc_value[2];

void adc_proc(void);        /* ADC 调度函数 */

#ifdef __cplusplus
}
#endif

#endif /* ADC_APP_H */
