/*********************************************************************************************
* 文件: iic.c
* 说明: 软件I2C驱动
*********************************************************************************************/
#include "iic.h"
#include "delay.h"

/* SDA设置为输出模式 */
static void sda_out(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = PIN_SDA;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
}

/* SDA设置为输入模式 */
static void sda_in(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = PIN_SDA;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
}

/*********************************************************************************************
* 名称：iic_init()
* 功能：I2C初始化
*********************************************************************************************/
void iic_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* 使能GPIOA时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    /* 配置SCL引脚 - 开漏输出 */
    GPIO_InitStructure.GPIO_Pin = PIN_SCL;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
    
    /* 配置SDA引脚 - 开漏输出 */
    GPIO_InitStructure.GPIO_Pin = PIN_SDA;
    GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
    
    /* 空闲状态：SCL和SDA都为高 */
    GPIO_SetBits(I2C_GPIO, PIN_SCL);
    GPIO_SetBits(I2C_GPIO, PIN_SDA);
}


/*********************************************************************************************
* 名称：iic_start()
* 功能：I2C起始信号
*********************************************************************************************/
void iic_start(void)
{
    sda_out();
    GPIO_SetBits(I2C_GPIO, PIN_SDA);    /* 拉高数据线 */
    GPIO_SetBits(I2C_GPIO, PIN_SCL);    /* 拉高时钟线 */
    delay_us(5);
    GPIO_ResetBits(I2C_GPIO, PIN_SDA);  /* 拉低数据线 */
    delay_us(5);
    GPIO_ResetBits(I2C_GPIO, PIN_SCL);  /* 钳住I2C总线 */
}

/*********************************************************************************************
* 名称：iic_stop()
* 功能：I2C停止信号
*********************************************************************************************/
void iic_stop(void)
{
    sda_out();
    GPIO_ResetBits(I2C_GPIO, PIN_SDA);  /* 拉低数据线 */
    GPIO_SetBits(I2C_GPIO, PIN_SCL);    /* 拉高时钟线 */
    delay_us(5);
    GPIO_SetBits(I2C_GPIO, PIN_SDA);    /* 拉高数据线 */
    delay_us(5);
}

/*********************************************************************************************
* 名称：iic_send_ack()
* 功能：I2C发送应答信号
* 参数：ack: 0-产生ACK应答, 1-不产生ACK应答
*********************************************************************************************/
void iic_send_ack(char ack)
{
    GPIO_ResetBits(I2C_GPIO, PIN_SCL);
    sda_out();
    if(ack)
        GPIO_SetBits(I2C_GPIO, PIN_SDA);    /* 不产生ACK */
    else
        GPIO_ResetBits(I2C_GPIO, PIN_SDA);  /* 产生ACK */
    delay_us(5);
    GPIO_SetBits(I2C_GPIO, PIN_SCL);
    delay_us(5);
    GPIO_ResetBits(I2C_GPIO, PIN_SCL);
}

/*********************************************************************************************
* 名称：iic_recv_ack()
* 功能：I2C接收应答
* 返回：应答信号 0-ACK, 1-NACK
*********************************************************************************************/
unsigned char iic_recv_ack(void)
{
    unsigned char ack = 0;
    sda_in();
    GPIO_SetBits(I2C_GPIO, PIN_SDA);
    GPIO_SetBits(I2C_GPIO, PIN_SCL);
    delay_us(5);
    ack = SDA_R;
    GPIO_ResetBits(I2C_GPIO, PIN_SCL);
    return ack;
}

/*********************************************************************************************
* 名称：iic_write_byte()
* 功能：I2C写一个字节
* 参数：data - 要写的数据
* 返回：0-收到ACK, 1-收到NACK
*********************************************************************************************/
unsigned char iic_write_byte(unsigned char data)
{
    unsigned char i;
    sda_out();
    GPIO_ResetBits(I2C_GPIO, PIN_SCL);
    
    for(i = 0; i < 8; i++) {
        if(data & 0x80)
            GPIO_SetBits(I2C_GPIO, PIN_SDA);
        else
            GPIO_ResetBits(I2C_GPIO, PIN_SDA);
        delay_us(5);
        GPIO_SetBits(I2C_GPIO, PIN_SCL);
        delay_us(5);
        data <<= 1;
        GPIO_ResetBits(I2C_GPIO, PIN_SCL);
    }
    
    /* 接收应答 */
    sda_in();
    GPIO_SetBits(I2C_GPIO, PIN_SDA);
    GPIO_SetBits(I2C_GPIO, PIN_SCL);
    delay_us(5);
    if(SDA_R) {
        GPIO_ResetBits(I2C_GPIO, PIN_SCL);
        return 1;  /* NACK */
    }
    GPIO_ResetBits(I2C_GPIO, PIN_SCL);
    return 0;  /* ACK */
}

/*********************************************************************************************
* 名称：iic_read_byte()
* 功能：I2C读一个字节
* 参数：ack: 0-发送ACK, 1-发送NACK
* 返回：读取的字节
*********************************************************************************************/
unsigned char iic_read_byte(unsigned char ack)
{
    unsigned char i, data = 0;
    sda_in();
    
    for(i = 0; i < 8; i++) {
        GPIO_ResetBits(I2C_GPIO, PIN_SCL);
        delay_us(5);
        GPIO_SetBits(I2C_GPIO, PIN_SCL);
        data <<= 1;
        if(SDA_R) data++;
        delay_us(5);
    }
    
    iic_send_ack(ack);
    return data;
}
