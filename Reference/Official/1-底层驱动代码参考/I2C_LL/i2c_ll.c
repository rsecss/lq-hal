/*
  程序说明: CT117E-M4嵌入式竞赛板GPIO模拟I2C总线驱动程序
  软件环境: MDK-ARM LL库
  硬件环境: CT117E-M4嵌入式竞赛板
  日    期: 2020-3-1
*/
#include "i2c_ll.h"

#define DELAY_TIME	20


/**
  * @brief SDA线输入模式配置
  * @param None
  * @retval None
  */
void SDA_Input_Mode()
{
    LL_GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = LL_GPIO_PIN_7;
    GPIO_InitStructure.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStructure.Speed = LL_GPIO_SPEED_FAST;
    LL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief SDA线输出模式配置
  * @param None
  * @retval None
  */
void SDA_Output_Mode()
{
    LL_GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = LL_GPIO_PIN_7;
    GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;
		GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStructure.Speed = LL_GPIO_SPEED_FAST;
    LL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief SDA线输出一个位
  * @param val 输出的数据
  * @retval None
  */
void SDA_Output( uint16_t val )
{
    if ( val )
    {
        GPIOB->BSRR |= LL_GPIO_PIN_7;
    }
    else
    {
        GPIOB->BRR |= LL_GPIO_PIN_7;
    }
}

/**
  * @brief SCL线输出一个位
  * @param val 输出的数据
  * @retval None
  */
void SCL_Output( uint16_t val )
{
    if ( val )
    {
        GPIOB->BSRR |= LL_GPIO_PIN_6;
    }
    else
    {
        GPIOB->BRR |= LL_GPIO_PIN_6;
    }
}

/**
  * @brief SDA输入一位
  * @param None
  * @retval GPIO读入一位
  */
uint8_t SDA_Input(void)
{
	if(LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_7)){
		return 1;
	}else{
		return 0;
	}
}


/**
  * @brief I2C的短暂延时
  * @param None
  * @retval None
  */
static void delay1(unsigned int n)
{
    uint32_t i;
    for ( i = 0; i < n; ++i);
}

/**
  * @brief I2C起始信号
  * @param None
  * @retval None
  */
void I2CStart(void)
{
    SDA_Output(1);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SDA_Output(0);
    delay1(DELAY_TIME);
    SCL_Output(0);
    delay1(DELAY_TIME);
}

/**
  * @brief I2C结束信号
  * @param None
  * @retval None
  */
void I2CStop(void)
{
    SCL_Output(0);
    delay1(DELAY_TIME);
    SDA_Output(0);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SDA_Output(1);
    delay1(DELAY_TIME);

}

/**
  * @brief I2C等待确认信号
  * @param None
  * @retval None
  */
unsigned char I2CWaitAck(void)
{
    unsigned short cErrTime = 5;
    SDA_Input_Mode();
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    while(SDA_Input())
    {
        cErrTime--;
        delay1(DELAY_TIME);
        if (0 == cErrTime)
        {
            SDA_Output_Mode();
            I2CStop();
            return ERROR;
        }
    }
    SDA_Output_Mode();
    SCL_Output(0);
    delay1(DELAY_TIME);
    return SUCCESS;
}

/**
  * @brief I2C发送确认信号
  * @param None
  * @retval None
  */
void I2CSendAck(void)
{
    SDA_Output(0);
    delay1(DELAY_TIME);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SCL_Output(0);
    delay1(DELAY_TIME);

}

/**
  * @brief I2C发送非确认信号
  * @param None
  * @retval None
  */
void I2CSendNotAck(void)
{
    SDA_Output(1);
    delay1(DELAY_TIME);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SCL_Output(0);
    delay1(DELAY_TIME);

}

/**
  * @brief I2C发送一个字节
  * @param cSendByte 需要发送的字节
  * @retval None
  */
void I2CSendByte(unsigned char cSendByte)
{
    unsigned char  i = 8;
    while (i--)
    {
        SCL_Output(0);
        delay1(DELAY_TIME);
        SDA_Output(cSendByte & 0x80);
        delay1(DELAY_TIME);
        cSendByte += cSendByte;
        delay1(DELAY_TIME);
        SCL_Output(1);
        delay1(DELAY_TIME);
    }
    SCL_Output(0);
    delay1(DELAY_TIME);
}

/**
  * @brief I2C接收一个字节
  * @param None
  * @retval 接收到的字节
  */
unsigned char I2CReceiveByte(void)
{
    unsigned char i = 8;
    unsigned char cR_Byte = 0;
    SDA_Input_Mode();
    while (i--)
    {
        cR_Byte += cR_Byte;
        SCL_Output(0);
        delay1(DELAY_TIME);
        delay1(DELAY_TIME);
        SCL_Output(1);
        delay1(DELAY_TIME);
        cR_Byte |=  SDA_Input();
    }
    SCL_Output(0);
    delay1(DELAY_TIME);
    SDA_Output_Mode();
    return cR_Byte;
}

//
void I2CInit(void)
{
	LL_GPIO_InitTypeDef GPIO_Initure;
	
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
	
	GPIO_Initure.Pin = LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
	GPIO_Initure.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_Initure.Pull = LL_GPIO_PULL_UP;
	GPIO_Initure.Speed = LL_GPIO_SPEED_FAST;
	LL_GPIO_Init(GPIOB, &GPIO_Initure);	
}
