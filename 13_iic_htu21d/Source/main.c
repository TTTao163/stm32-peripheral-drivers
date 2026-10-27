/*********************************************************************************************
* 文件: main.c
* 说明: HTU21D温湿度传感器数据读取，LCD显示并通过串口发送到PC
*********************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "usart.h"
#include "htu21d.h"

int main(void)
{
    int temp_raw, humi_raw;
    float temp, humi;
    int temp_int, temp_dec, humi_int, humi_dec;
    char temp_str[20];
    char humi_str[20];
    
    delay_init(168);
    led_init();
    usart_init(115200);
    htu21d_init();
    
    /* 初始化LCD */
    lcd_init(IIC1);
    
    /* 显示标签 */
    LCDDrawFont24(20, 100, "Temp:", 0x0000, 0xFFFF);
    LCDDrawFont24(20, 140, "Humi:", 0x0000, 0xFFFF);
    
    while(1)
    {
        /* 读取原始数据 */
        temp_raw = htu21d_read_temp();
        humi_raw = htu21d_read_humi();
        
        /* 清除状态位(后两位置0) */
        temp_raw = temp_raw & 0xFFFC;
        humi_raw = humi_raw & 0xFFFC;
        
        /* 转换公式: T = -46.85 + 175.72 * ST / 65536 */
        temp = -46.85f + 175.72f * temp_raw / 65536.0f;
        
        /* 转换公式: RH = -6 + 125 * SRH / 65536 */
        humi = -6.0f + 125.0f * humi_raw / 65536.0f;
        
        /* 分离整数和小数部分 */
        temp_int = (int)temp;
        temp_dec = (int)((temp - temp_int) * 100);
        if(temp_dec < 0) temp_dec = -temp_dec;
        
        humi_int = (int)humi;
        humi_dec = (int)((humi - humi_int) * 100);
        if(humi_dec < 0) humi_dec = -humi_dec;
        
        /* 格式化数据 */
        sprintf(temp_str, "%d.%02d C  ", temp_int, temp_dec);
        sprintf(humi_str, "%d.%02d %%  ", humi_int, humi_dec);
        
        /* LCD显示转换后的数据 */
        LCDDrawFont24(100, 100, temp_str, 0x001F, 0xFFFF);
        LCDDrawFont24(100, 140, humi_str, 0x001F, 0xFFFF);
        
        /* 串口打印原始数据 */
        printf("Raw Temp:%d Raw Humi:%d\r\n", temp_raw, humi_raw);
        printf("Temp:%d.%02d C  Humi:%d.%02d %%RH\r\n", temp_int, temp_dec, humi_int, humi_dec);
        
        /* LED指示 */
        turn_on(D3);
        turn_on(D4);
        delay_ms(500);
        turn_off(D3);
        turn_off(D4);
        delay_ms(500);
    }
}
