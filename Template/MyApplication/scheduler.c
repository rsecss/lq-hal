#include "scheduler.h"

uint8_t task_num = 0;			/* 全局变量，用于存储用户任务数量 */

typedef struct {
    void (*task_func)(void);    /* 任务函数 */
    uint32_t rate_ms;           /* 任务执行周期（ms） */
    uint32_t last_run_time;     /* 上次执行时间（ms） */
}task_t;

/* 静态调度器任务列表，每个任务包括任务函数、执行周期（ms）、上次执行时间（ms） */
static task_t scheduler_task[] = {
    {led_proc1, 1, 0},       /* LED 任务，周期为 1ms，初始上次运行时间为 0 */
	{led_proc2, 1, 1},
    {key_proc, 10, 0},      /* 按键任务，周期为 10ms，初始上次运行时间为 0 */
    {lcd_proc, 100, 0},      /* LCD 任务，周期为 100ms，初始上次运行时间为 0 */
    {uart_proc, 10, 0},     /* UART 任务，周期为 10ms，初始上次运行时间为 0 */
    {adc_proc, 100, 0}      /* ADC 任务，周期为 100ms，初始上次运行时间为 0 */
};

/**
 * @brief       调度器初始化函数
 * @param       无
 * @retval      无
 */
void scheduler_init()
{
    task_num = sizeof(scheduler_task) / sizeof(task_t);
}

/**
 * @brief       调度器运行函数
 * @param       无
 * @retval      无
 */
void scheduler_run()
{
    uint8_t i;
    for (i = 0; i < task_num; i++) 
    {
        uint32_t now_time = HAL_GetTick();
		
        if (now_time >= scheduler_task[i].last_run_time + scheduler_task[i].rate_ms) 
        {
            scheduler_task[i].last_run_time = now_time;		/* 更新任务的上次运行时间为当前时间 */
            scheduler_task[i].task_func();      			/* 执行任务函数 */
        }
    }
}
