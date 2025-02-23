/*
  ����˵��: CT117E-M4Ƕ��ʽ������GPIOģ��I2C������������
  �������: MDK-ARM HAL��
  Ӳ������: CT117E-M4Ƕ��ʽ������
  ��    ��: 2020-3-1
*/

#include "i2c_hal.h"

#define DELAY_TIME	20

/**
  * @brief SDA������ģʽ����
  * @param None
  * @retval None
  */
void SDA_Input_Mode()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = GPIO_PIN_7;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief SDA�����ģʽ����
  * @param None
  * @retval None
  */
void SDA_Output_Mode()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = GPIO_PIN_7;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief SDA�����һ��λ
  * @param val ���������
  * @retval None
  */
void SDA_Output( uint16_t val )
{
    if ( val )
    {
        GPIOB->BSRR |= GPIO_PIN_7;
    }
    else
    {
        GPIOB->BRR |= GPIO_PIN_7;
    }
}

/**
  * @brief SCL�����һ��λ
  * @param val ���������
  * @retval None
  */
void SCL_Output( uint16_t val )
{
    if ( val )
    {
        GPIOB->BSRR |= GPIO_PIN_6;
    }
    else
    {
        GPIOB->BRR |= GPIO_PIN_6;
    }
}

/**
  * @brief SDA����һλ
  * @param None
  * @retval GPIO����һλ
  */
uint8_t SDA_Input(void)
{
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_SET){
		return 1;
	}else{
		return 0;
	}
}


/**
  * @brief I2C�Ķ�����ʱ
  * @param None
  * @retval None
  */
static void delay1(unsigned int n)
{
    uint32_t i;
    for ( i = 0; i < n; ++i);
}

/**
  * @brief I2C��ʼ�ź�
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
  * @brief I2C�����ź�
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
  * @brief I2C�ȴ�ȷ���ź�
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
  * @brief I2C����ȷ���ź�
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
  * @brief I2C���ͷ�ȷ���ź�
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
  * @brief I2C����һ���ֽ�
  * @param cSendByte ��Ҫ���͵��ֽ�
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
  * @brief I2C����һ���ֽ�
  * @param None
  * @retval ���յ����ֽ�
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
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = GPIO_PIN_7 | GPIO_PIN_6;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void I2CStart(void);
void I2CStop(void);
unsigned char I2CWaitAck(void);
void I2CSendAck(void);
void I2CSendNotAck(void);
void I2CSendByte(unsigned char cSendByte);
unsigned char I2CReceiveByte(void);
void I2CInit(void);

/**
 * @brief     �� EEPROM д������
 * 
 * @param     eeprom_string ָ��Ҫд�� EEPROM �����ݵ�ָ��
 * @param     address EEPROM �ڴ��е���ʼ��ַ
 * @param     number Ҫд��������ֽ���
 * @retval    ��
 */
void eeprom_wirte(uint8_t *eeprom_string, uint8_t address, uint8_t number)
{
  I2CStart();                     // ������ʼ�ź�
  I2CSendByte(0xA0);              // ���� EEPROM д���������ַ + д����λ��
  I2CWaitAck();                   // �ȴ� EEPROM �� ACK �ź�

  I2CSendByte(address);           // ����Ҫд�� EEPROM �ڴ���ʼ��ַ
  I2CWaitAck();                   // �ȴ� EEPROM �� ACK �ź�

  /* ���ֽ�д�� EEPROM */
  while (number--) {
    I2CSendByte(*eeprom_string++);    // ���͵�ǰ�ֽڵ�����
    I2CWaitAck();                     // �ȴ� EEPROM �� ACK �ź�
    delay1(200);                      // ��ʱȷ������д��
  }

  I2CStop();                     // ����ֹͣ�ź�
  HAL_Delay(5);                  // ����д����Ҫ��ʱȷ������д��
}

/**
 * @brief     �� EEPROM ��ȡ����
 * 
 * @param     eeprom_string ָ��洢��ȡ���ݵĻ�������ָ��
 * @param     address EEPROM �ڴ��е���ʼ��ַ
 * @param     number Ҫ��ȡ�������ֽ���
 * @retval    ��
 */
void eeprom_read(uint8_t *eeprom_string, uint8_t address, uint8_t number)
{
  I2CStart();                     // ���� I2C ͨ��
  I2CSendByte(0xA0);              // ���� EEPROM д���������ַ + д����λ��
  I2CWaitAck();                   // �ȴ� EEPROM �� ACK �ź�

  I2CSendByte(address);           // ����Ҫ��ȡ EEPROM �ڴ���ʼ��ַ
  I2CWaitAck();                   // �ȴ� EEPROM �� ACK �ź�

  I2CStart();                     // �ٴ����� I2C ͨ�ţ�׼��������
  I2CSendByte(0xA1);              // ���� EEPROM �����������ַ + ������λ��
  I2CWaitAck();                   // �ȴ� EEPROM �� ACK �ź�

  while (number--) {
    *eeprom_string++ = I2CReceiveByte();    // ��ȡ��ǰ�ֽڵ�����
    if (number) {
      I2CSendAck();                         // �������Ҫ������ȡ,���� ACK �ź�
    } else {
      I2CSendNotAck();                      // �������Ҫ������ȡ,���� NACK �ź�
    }
  }

  I2CStop();                     // ֹͣ I2C ͨ��
}

/**
 * @brief     �� MCP4017 д������
 * 
 * @param     data Ҫд�� MCP4017 ������
 * @retval    ��
 */
void mcp4017_write(uint8_t data)
{
  I2CStart();             // ���� I2C ͨ��
  I2CSendByte(0x5E);      // ���� MCP4017 д���������ַ + д����λ��
  I2CWaitAck();           // �ȴ� MCP4017 �� ACK �ź�
  I2CSendByte(data);      // ����Ҫд�� MCP4017 ������
  I2CWaitAck();           // �ȴ� MCP4017 �� ACK �ź�
  I2CStop();              // ֹͣ I2C ͨ��
}

/**
 * @brief     �� MCP4017 ��ȡ����
 * 
 * @param     data ָ��洢��ȡ���ݵı�����ָ��
 * @retval    ��
 */
void mcp4017_read(uint8_t *data)
{
  I2CStart();                 // ���� I2C ͨ��
  I2CSendByte(0x5F);          // ���� MCP4017 �����������ַ + ������λ��
  I2CWaitAck();               // �ȴ� MCP4017 �� ACK �ź�
  *data = I2CReceiveByte();   // ��ȡ��ǰ�ֽڵ�����
  I2CSendNotAck();            // ���� NACK �ź�
  I2CStop();                  // ֹͣ I2C ͨ��
}
