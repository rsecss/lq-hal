#include "system.h"

/**
 * @brief   系统初始化函数
 * 
 * @details 初始化系统的 GPIO 设置，包括对 GPIOC 和 GPIOD 
 * @param   无
 * @retval  无
 */
void system_init()
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8
                            |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}
