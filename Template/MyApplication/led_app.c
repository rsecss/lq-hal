#include "led_app.h"

#define LED_ON 0xFF    /* 0xFF 表示全亮,可以根据需求改写 */
#define LED_OFF 0x00

/* 底层 */
/**
 * @brief       LED 显示函数
 * @param       ucled: 要控制 LED 的位置，取值范围：0~8
 * @retval      无
 */
void led_disp(uint8_t ucled)
{
    /* 关闭所有的 LED */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
						GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |GPIO_PIN_15, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

    /* 根据 ucLed 的值点亮相应的灯 */
    HAL_GPIO_WritePin(GPIOC, ucled << 8, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

/* 逻辑层 */
void led_proc1()
{
    led_disp(LED_ON);
}

void led_proc2()
{
	led_disp(LED_OFF);
}
