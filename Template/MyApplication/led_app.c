#include "led_app.h"

static const uint8_t LED_ON = 0xFF;     /* 0xFF 表示全亮 */
static const uint8_t LED_OFF = 0x00;    /* 0x00 表示全灭 */
static const uint8_t LED_ODD = 0xAA;    /* 0xAA 表示奇数位置亮 */
static const uint8_t LED_EVEN = 0x55;   /* 0x55 表示偶数位置亮 */
static const uint8_t LED_PIN_SHIFT = 0x08;

/* 底层 */
/**
 * @brief       LED 显示函数
 * @param       led: 要控制 LED 的位置，取值范围：0x00~0xFF
 * @retval      无
 */
static void led_display(uint8_t led)
{
    /* 关闭所有的 LED */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                    GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |
                    GPIO_PIN_14 |GPIO_PIN_15, GPIO_PIN_SET);

    /* 根据 ucLed 的值点亮相应的灯 */
    HAL_GPIO_WritePin(GPIOC, (uint16_t)(led << LED_PIN_SHIFT), GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

/* 逻辑层 */
void led_proc1()
{
    led_display(LED_ON);
}

void led_proc2()
{
    led_display(LED_OFF);
}

void led_proc3()
{
    led_display(LED_ODD);
}

void led_proc4()
{
    led_display(LED_EVEN);
}

void led_proc5()
{
    static uint8_t led = 0x01;

    for (uint8_t i = 0; i < 8; i++) {
        led_display(led << i);
        HAL_Delay(100);
    }
}
