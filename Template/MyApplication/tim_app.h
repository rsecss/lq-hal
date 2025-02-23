#ifndef TIM_APP_H
#define TIM_APP_H

#include "bsp_system.h"

#ifdef __cplusplus
extern "C" {
#endif

void pwm_set_duty(float duty,uint8_t channel);
void pwm_set_frequency(int frequency, uint8_t channel);
void tim_ic_proc(void);

#ifdef __cplusplus
}
#endif

#endif /* TIM_APP_H */



