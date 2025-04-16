#include "usart_app.h"

/*******************串口中断+超时解析**************************/
// uint16_t uart_rx_index = 0;
// uint32_t uart_rx_ticks = 0;
// uint8_t uart_rx_buffer[128] = {0};

// /**
//  * @brief       中断回调函数（串口中断）
//  * 
//  * @param       huart1 uart1句柄
//  * @retval      无 
//  */
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//     if (huart->Instance == USART1)
//     {
//         uart_rx_ticks = HAL_GetTick();      // 记录接收到数据的时间
//         uart_rx_index++;                    // 指针往下偏移
//         HAL_UART_Receive_IT(&huart1, &uart_rx_buffer[uart_rx_index], 1);    // 使能接收中断
//     }
// }

// /**
//  * @brief       处理 USART 接收缓冲区中的数据（串口中断 + 超时解析）
//  * 
//  * @note        如果在 100ms 内没有接收到新的数据，将清空缓冲区
//  * @param       无
//  * @retval      无
//  */
// void uart_proc()
// {
//     if (uart_rx_index == 0)
//         return;   // 如果接收索引为 0，说明没有数据需要处理，直接返回
//     if (HAL_GetTick() - uart_rx_ticks > 100)
//     {
//         // printf("uart data: %s\n", uart_rx_buffer);
//         memset(uart_rx_buffer, 0, uart_rx_index);   // 清空缓冲区
//         uart_rx_index = 0;                          // 重置索引
//         huart1.pRxBuffPtr = uart_rx_buffer;         // 将缓存区指针重置成初始位置
//     }
// }



/****************************DMA+环形缓存区******************/
#define BUUFER_SIZE 128

uint8_t uart_rx_dma_buffer[128] = {0};  // 定义 DMA 接收缓冲区
ringbuffer_t usart_rb;                  // 定义环形缓存区
uint8_t usart_read_buffer[BUUFER_SIZE]; // 定义读取的数组


/**
 * @brief   UART DMA接收完成回调函数
 *          将接收到的数据写入环形缓冲区，并清空DMA缓冲区
 * @param   huart UART句柄
 * @param   Size 接收到的数据大小
 * @retval  无
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    /* 如果环形缓冲区未满 */
    if(!ringbuffer_is_full(&usart_rb))
    {
        /* 将 DMA 缓冲区中的数据写入环形缓冲区 */
        ringbuffer_write(&usart_rb, uart_rx_dma_buffer, Size);
    }
    /* 清空 DMA 缓冲区 */
    memset(uart_rx_dma_buffer, 0, sizeof(uart_rx_dma_buffer));
}

void uart_proc()
{
    /* 判断环形缓存区是否为空 */
    if(ringbuffer_is_empty(&usart_rb))
    {
        return;
    }

    /* 从环形缓冲区读取数据到接受缓冲区 */
    ringbuffer_read(&usart_rb, usart_read_buffer, usart_rb.itemCount);

    /* 打印接受缓冲区中的数据 */
    printf("ringbuffer data: %s\n", usart_read_buffer);

    /* 清空读取缓冲区 */
    memset(usart_read_buffer, 0, sizeof(uint8_t) * BUUFER_SIZE);
}
