#include "key_app.h"

/**********************四行代码版本（初阶）*************************/
static uint8_t key_val = 0;        // 当前按键状态
static uint8_t key_old = 0;        // 上一次按键状态
static uint8_t key_down = 0;       // 按键按下标志
static uint8_t key_up = 0;         // 按键松开标志

/* 底层 */
/**
 * @brief           读取按键状态
 * 
 * @param           无
 * @return          uint8_t: 返回按键编号，0 表示无按键按下，1~4 表示相应按键编号
 */
static uint8_t key_read()
{
    uint8_t temp = 0;

    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET)
        temp = 1;           // 如果引脚状态为 RESET，按键一按下
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
        temp = 2;           // 如果引脚状态为 RESET，按键二按下
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET)
        temp = 3;           //如果引脚状态为 RESET，按键三按下
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
        temp = 4;           // 如果引脚状态为 RESET，按键四按下

    return temp;
}

/* 逻辑层 */
/**
 * @brief           按键控制处理函数（初阶——四行代码版本）
 * @param           无
 * @retval          无
 */
void key_proc()
{
    key_val = key_read();
    /* 计算按下的按键（当前按下状态与前一状态异或，并与当前状态相与）*/
    key_down = key_val & (key_val ^ key_old);
    /* 计算松开的按键（当前松开状态与前一状态异或，并与当前状态相与）*/
    key_up = ~key_val & (key_val ^ key_old);
    key_old = key_val;

    /* 按键的处理逻辑 */

}

/********************状态机版本（进阶）***************************/

///* 按键结构体定义 */
//typedef struct
//{
//  GPIO_TypeDef *gpiox;     // 指向 GPIO 端口的指针，例如 GPIOA、GPIOB 等。
//  uint16_t pin;            // 指定 GPIO 引脚，例如 GPIO_PIN_0、GPIO_PIN_1 等。
//  uint16_t ticks;          // 用于计时的变量，通常用于去抖动处理。
//  uint8_t level;           // 当前按键的电平状态，高电平或低电平。
//  uint8_t id;              // 按键的唯一标识符，可以用于区分不同的按键。
//  uint8_t state;           // 按键的当前状态，可能用于表示按键是否被按下或释放。
//  uint8_t debouce_cnt;     // 按键去抖动计数器，用于防止按键抖动引起的误触发。
//  uint8_t repeat;          // 按键重复按下的次数。
//} button;

//button btns[4]; // 按键数组

///**
// * @brief           按键初始化函数
// * 
// * @param           无
// * @retval          无
// */
//void key_init()
//{
//    /* 初始化第一个按键 */
//    btns[0].gpiox = GPIOB;    // 指定 GPIO 端口
//    btns[0].pin = GPIO_PIN_0; // 指定引脚
//    btns[0].level = 1;        // 设置初始电平
//    btns[0].id = 0;           // 设置按键 ID

//    /* 初始化第二个按键 */
//    btns[1].gpiox = GPIOB;
//    btns[1].pin = GPIO_PIN_1;
//    btns[1].level = 1;
//    btns[1].id = 1;

//    /* 初始化第三个按键 */
//    btns[2].gpiox = GPIOB;
//    btns[2].pin = GPIO_PIN_2;
//    btns[2].level = 1;
//    btns[2].id = 2;

//    /* 初始化第四个按键 */
//    btns[3].gpiox = GPIOA;
//    btns[3].pin = GPIO_PIN_0;
//    btns[3].level = 1;
//    btns[3].id = 3;
//}

///**
// * @brief           按键扫描函数
// * @param           button *btn 指向按键结构体的指针
// * @retval          无
// */
//void button_handler(button *btn)
//{
//    /* 读取按键当前电平 */
//    uint8_t gpio_level = HAL_GPIO_ReadPin(btn->gpiox, btn->pin);

//    /* 如果按键状态大于 0，则递增计时器 */
//    if (btn->state > 0)
//        btn->ticks++;

//    /* 如果当前电平与按键记录的电平不同，进行去抖动处理 */
//    if (btn->level != gpio_level)
//    {
//        if (++(btn->debouce_cnt) >= 3)  // 计数达到 3 次（试验测试表明 3 次时比较合理的），确认电平变化
//        {
//        btn->level = gpio_level;        // 更新电平
//        btn->debouce_cnt = 0;           // 重置去抖动计数器
//        }
//    }
//    else
//    {
//        btn->debouce_cnt = 0;           // 电平没有变化，重置去抖动计数器
//    }

//    /* 按键状态机 */
//    switch (btn->state)
//    {
//        case 0:                             // 初始状态
//            if (btn->level == 0)            // 等待按键按下
//            {
//                btn->ticks = 0;             // 重置计时器
//                btn->repeat = 1;            // 初始按键重复计数
//                btn->state = 1;             // 进入按键按下状态
//            }
//            break;
//        case 1:                             // 按键按下状态
//            if (btn->level != 0)            // 等待按键松开
//            {
//                btn->ticks = 0;             // 重置计时器
//                btn->state = 2;             // 进入按键释放状态
//            }
//            /* 按键长按，调度器按键是 10ms 运行一次，长按则为 10 * 30 = 300ms */
//            else if (btn->ticks >= 30)
//            {
//                /* 下面写长按逻辑：300ms 没松开，发生长按事件 */
//                switch(btn->id)
//                {
//                    case 0:
//                        printf("按键1长按\n");
//                        break;
//                    case 1:
//                        printf("按键2长按\n");
//                        break;
//                    case 2:
//                        printf("按键3长按\n");
//                        break;
//                    case 3:
//                        printf("按键4长按\n");
//                        break;
//                }
//                btn->ticks = 0;             // 重新计时
//                btn->repeat = 0;            // 防止释放的时候再次触发单击事件
//            }
//            break;
//        case 2:                             // 按键释放状态
//            if (btn->ticks >= 15)           // 计时器达到阈值
//            {
//                btn->state = 0;             // 返回初始状态

//                /* 下面写单击、双击、三击等逻辑 */
//                if(btn->repeat == 1)        // 单击
//                {
//                    switch(btn->id)
//                    {
//                        case 0:
//                            printf("按键1单击\n");
//                            break;
//                        case 1:
//                            printf("按键2单击\n");
//                            break;
//                        case 2:
//                            printf("按键3单击\n");
//                            break;
//                        case 3:
//                            printf("按键4单击\n");
//                            break;
//                    }
//                }
//                else if(btn->repeat == 2)   // 双击
//                {
//                    switch (btn->id)
//                    {
//                        case 0:
//                            printf("按键1双击\n");
//                            break;
//                        case 1:
//                            printf("按键2双击\n");
//                            break;
//                        case 2:
//                            printf("按键3双击\n");
//                            break;
//                        case 3:
//                            printf("按键4双击\n");
//                            break;
//                    }
//                }
//            }
//            else
//            {
//                if (btn->level == 0)        // 按键再次按下
//                {
//                    btn->repeat++;          // 递增重复计数
//                    btn->ticks = 0;         // 重置计时器
//                    btn->state = 1;         // 返回按键按下状态
//                }
//            }
//            break;
//    }
//}

///**
// * @brief           按键状态处理函数（放进）
// * @param           无
// * @retval          无
// */
//void key_state_proc()
//{
//    for (uint8_t i = 0; i < 4; i++) // 遍历所有按键
//    {
//        button_handler(&btns[i]); // 处理每个按键的状态
//    }
//}
