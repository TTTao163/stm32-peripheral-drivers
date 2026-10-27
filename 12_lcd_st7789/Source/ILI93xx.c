/*********************************************************************************************
* 文件: ili93xx.c
* 作者: zonesion 2016.12.22
* 说明: LCD显示相关函数
* 修改:
* 注释:
*********************************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include "string.h"
#include "ili93xx.h"
#include "font.h"

static char CMD_WR_RAM = 0x22;
unsigned int LCD_ID = 0x9325;

/*********************************************************************************************
* 名称: LCD_WR_REG
* 功能: 写寄存器命令
* 参数: regval:寄存器值
* 返回: 无
*********************************************************************************************/
void LCD_WR_REG(vu16 regval)
{
    regval=regval;
    ILI93xx_REG=regval;
}

/*********************************************************************************************
* 名称: LCD_WR_DATA
* 功能: 写LCD数据
* 参数: data:要写的值
* 返回: 无
*********************************************************************************************/
void LCD_WR_DATA(vu16 data)
{
    data=data;
    ILI93xx_DAT=data;
}

/*********************************************************************************************
* 名称: LCD_RD_DATA
* 功能: 读LCD数据
* 参数: 无
* 返回: 读到的值
*********************************************************************************************/
u16 LCD_RD_DATA(void)
{
    vu16 ram;
    ram=ILI93xx_DAT;
    return ram;
}

/*********************************************************************************************
* 名称: ILI93xx_WriteReg
* 功能: 向LCD指定寄存器写入数据
* 参数: r:寄存器地址; d:要写的值
* 返回: 无
*********************************************************************************************/
void ILI93xx_WriteReg(uint16_t r, uint16_t d)
{
    ILI93xx_REG = r;
    ILI93xx_DAT = d;
}

/*********************************************************************************************
* 名称: ILI93xx_ReadReg
* 功能: 读取寄存器的值
* 参数: r:寄存器地址
* 返回: 读到的值
*********************************************************************************************/
uint16_t ILI93xx_ReadReg(uint16_t r)
{
    uint16_t v;
    ILI93xx_REG = r;
    v = ILI93xx_DAT;
    return v;
}

/*********************************************************************************************
* 名称: BLOnOff
* 功能: 打开或关闭LCD背光
* 参数: st:1打开背光;0关闭背光
* 返回: 无
*********************************************************************************************/
void BLOnOff(int st)
{
#ifdef ZXBEE_PLUSE
    if (st)
    {
        GPIO_SetBits(GPIOD, GPIO_Pin_2);
    }
    else
    {
        GPIO_ResetBits(GPIOD, GPIO_Pin_2);
    }
#else
    if (st)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_15);
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_15);
    }
#endif
}

/*********************************************************************************************
* 名称: BLInit
* 功能: 背光IO口初始化
* 参数: 无
* 返回: 无
*********************************************************************************************/
void BLInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
#ifdef ZXBEE_PLUSE
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
#else
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
}

/*********************************************************************************************
* 名称: REST_Init
* 功能: 复位IO口初始化
* 参数: 无
* 返回: 无
*********************************************************************************************/
void REST_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_SetBits(GPIOD, GPIO_Pin_13);
    delay_ms(1);
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);
    delay_ms(10);
    GPIO_SetBits(GPIOD, GPIO_Pin_13);
    delay_ms(120);
}

/*********************************************************************************************
* 名称: ILI93xxInit
* 功能: LCD初始化
* 参数: 无
* 返回: 无
*********************************************************************************************/
void ILI93xxInit(void)
{
    int a, b, c, d;
    int i, j;
    unsigned int xStar, xEnd, yStar, yEnd;
    char sm, ss, gs;
    
    fsmc_init();
    delay_ms(50);
    REST_Init();
    BLInit();
    BLOnOff(1);
    LCD_ID = ILI93xx_ReadReg(0);
    if (LCD_ID == 0)
    {
        LCD_WR_REG(0Xd3);
        a=LCD_RD_DATA();
        b=LCD_RD_DATA();
        c=LCD_RD_DATA();
        d=LCD_RD_DATA();
        LCD_ID = (c << 8) | d;
    }

    if (LCD_ID == 0x72)
    {
        CMD_WR_RAM = 0x2C;
        ILI93xx_WriteReg(0x001A, 0x0002);
        ILI93xx_WriteReg(0x001B, 0x0088);
        ILI93xx_WriteReg(0x0023, 0x0000);
        ILI93xx_WriteReg(0x0024, 0x005F);
        ILI93xx_WriteReg(0x0025, 0x0015);
        ILI93xx_WriteReg(0x002D, 0x0003);
        ILI93xx_WriteReg(0x0018, 0x0004);
        ILI93xx_WriteReg(0x0019, 0x0001);
        ILI93xx_WriteReg(0x0001, 0x0000);
        ILI93xx_WriteReg(0x0017, 0x0005);
        ILI93xx_WriteReg(0x0036, 0x0013);
        ILI93xx_WriteReg(0x0040,0x0000);
        ILI93xx_WriteReg(0x0041,0x0045);
        ILI93xx_WriteReg(0x0042,0x0045);
        ILI93xx_WriteReg(0x0043,0x0004);
        ILI93xx_WriteReg(0x0044,0x0000);
        ILI93xx_WriteReg(0x0045,0x0008);
        ILI93xx_WriteReg(0x0046,0x0023);
        ILI93xx_WriteReg(0x0047,0x0023);
        ILI93xx_WriteReg(0x0048,0x0077);
        ILI93xx_WriteReg(0x0049,0x0040);
        ILI93xx_WriteReg(0x004A,0x0004);
        ILI93xx_WriteReg(0x004B,0x0000);
        ILI93xx_WriteReg(0x004C,0x0088);
        ILI93xx_WriteReg(0x004D,0x0088);
        ILI93xx_WriteReg(0x004E,0x0088);
        ILI93xx_WriteReg(0x001F,0x00D0);
        ILI93xx_WriteReg(0x0028,0x003F);
        ILI93xx_WriteReg(0x016,0x00);
        
        xStar = 0; xEnd=240; yStar = 0; yEnd=400;
        ILI93xx_WriteReg(0x02, xStar>>8);
        ILI93xx_WriteReg(0x03, xStar);
        ILI93xx_WriteReg(0x04, xEnd>>8);
        ILI93xx_WriteReg(0x05, xEnd);
        ILI93xx_WriteReg(0x06, yStar>>8);
        ILI93xx_WriteReg(0x07, yStar);
        ILI93xx_WriteReg(0x08, yEnd>>8);
        ILI93xx_WriteReg(0x09, yEnd);

        ILI93xx_REG = 0x22;
        for (i=0; i<240; i++)
        {
            for (j=0; j<400; j++)
            {
                ILI93xx_DAT = 0xf800;
            }
        }
    }
    else if ((LCD_ID == 0x9325) || (LCD_ID == 0x9328) || (LCD_ID == 0xD3D3))
    {
        CMD_WR_RAM = 0x22;
        sm = 0;
        ss = 1;
        gs = 1;
#if SCREEN_ORIENTATION_LANDSCAPE
        gs = 0;
#endif
        ILI93xx_WriteReg(0x00EC,0x108F);
        ILI93xx_WriteReg(0x00EF,0x1234);
        ILI93xx_WriteReg(0x0001,0x0000|(ss<<8) | (sm<<10));
        ILI93xx_WriteReg(0x0002,0x0700);
#if SCREEN_ORIENTATION_LANDSCAPE
        ILI93xx_WriteReg(0x0003,(1<<12)|(0<<5)|(1<<4)|(1<<3));
#else
        ILI93xx_WriteReg(0x0003,(1<<12)|(1<<5)|(1<<4)|(0<<3));
#endif
        ILI93xx_WriteReg(0x0004,0x0000);
        ILI93xx_WriteReg(0x0008,0x0202);
        ILI93xx_WriteReg(0x0009,0x0000);
        ILI93xx_WriteReg(0x000a,0x0000);
        ILI93xx_WriteReg(0x000c,0x0001);
        ILI93xx_WriteReg(0x000d,0x0000);
        ILI93xx_WriteReg(0x000f,0x0000);
        ILI93xx_WriteReg(0x0010,0x0000);
        ILI93xx_WriteReg(0x0011,0x0007);
        ILI93xx_WriteReg(0x0012,0x0000);
        ILI93xx_WriteReg(0x0013,0x0000);
        ILI93xx_WriteReg(0x0007,0x0001);
        delay_ms(50);
        ILI93xx_WriteReg(0x0010,0x1490);
        ILI93xx_WriteReg(0x0011,0x0227);
        delay_ms(50);
        ILI93xx_WriteReg(0x0012,0x008A);
        delay_ms(50);
        ILI93xx_WriteReg(0x0013,0x1a00);
        ILI93xx_WriteReg(0x0029,0x0006);
        ILI93xx_WriteReg(0x002b,0x000d);
        delay_ms(50);
        ILI93xx_WriteReg(0x0020,0x0000);
        ILI93xx_WriteReg(0x0021,0x0000);
        delay_ms(50);
        ILI93xx_WriteReg(0x0030,0x0000);
        ILI93xx_WriteReg(0x0031,0x0604);
        ILI93xx_WriteReg(0x0032,0x0305);
        ILI93xx_WriteReg(0x0035,0x0000);
        ILI93xx_WriteReg(0x0036,0x0C09);
        ILI93xx_WriteReg(0x0037,0x0204);
        ILI93xx_WriteReg(0x0038,0x0301);
        ILI93xx_WriteReg(0x0039,0x0707);
        ILI93xx_WriteReg(0x003c,0x0000);
        ILI93xx_WriteReg(0x003d,0x0a0a);
        delay_ms(50);
        ILI93xx_WriteReg(0x0050,0x0000);
        ILI93xx_WriteReg(0x0051,0x00ef);
        ILI93xx_WriteReg(0x0052,0x0000);
        ILI93xx_WriteReg(0x0053,0x013f);
        ILI93xx_WriteReg(0x0060,(gs<<15)|0x2700);
        ILI93xx_WriteReg(0x0061,0x0001);
        ILI93xx_WriteReg(0x006a,0x0000);
        ILI93xx_WriteReg(0x0080,0x0000);
        ILI93xx_WriteReg(0x0081,0x0000);
        ILI93xx_WriteReg(0x0082,0x0000);
        ILI93xx_WriteReg(0x0083,0x0000);
        ILI93xx_WriteReg(0x0084,0x0000);
        ILI93xx_WriteReg(0x0085,0x0000);
        ILI93xx_WriteReg(0x0090,0x0010);
        ILI93xx_WriteReg(0x0092,0x0600);
        ILI93xx_WriteReg(0x0007,0x0133);
#if SCREEN_ORIENTATION_LANDSCAPE
        LCDSetWindow(0, 320, 0, 240);
#else
        LCDSetWindow(0, 240, 0, 320);
#endif
    }
    else if (LCD_ID == 0x9341)
    {
        CMD_WR_RAM = 0x2C;
        LCD_WR_REG(0xCF);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0xC1);
        LCD_WR_DATA(0X30);
        LCD_WR_REG(0xED);
        LCD_WR_DATA(0x64);
        LCD_WR_DATA(0x03);
        LCD_WR_DATA(0X12);
        LCD_WR_DATA(0X81);
        LCD_WR_REG(0xE8);
        LCD_WR_DATA(0x85);
        LCD_WR_DATA(0x10);
        LCD_WR_DATA(0x7A);
        LCD_WR_REG(0xCB);
        LCD_WR_DATA(0x39);
        LCD_WR_DATA(0x2C);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x34);
        LCD_WR_DATA(0x02);
        LCD_WR_REG(0xF7);
        LCD_WR_DATA(0x20);
        LCD_WR_REG(0xEA);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x00);
        LCD_WR_REG(0xC0);
        LCD_WR_DATA(0x1B);
        LCD_WR_REG(0xC1);
        LCD_WR_DATA(0x01);
        LCD_WR_REG(0xC5);
        LCD_WR_DATA(0x30);
        LCD_WR_DATA(0x30);
        LCD_WR_REG(0xC7);
        LCD_WR_DATA(0XB7);
        LCD_WR_REG(0x36);
#if SCREEN_ORIENTATION_LANDSCAPE
        LCD_WR_DATA((1<<7)|(0<<6)|(1<<5)|SWAP_RGB);
#else
        LCD_WR_DATA((0<<7)|(0<<6)|(0<<5)|SWAP_RGB);
#endif
        LCD_WR_REG(0x3A);
        LCD_WR_DATA(0x55);
        LCD_WR_REG(0xB1);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x1A);
        LCD_WR_REG(0xB6);
        LCD_WR_DATA(0x0A);
        LCD_WR_DATA(0xA2);
        LCD_WR_REG(0xF2);
        LCD_WR_DATA(0x00);
        LCD_WR_REG(0x26);
        LCD_WR_DATA(0x01);
        LCD_WR_REG(0xE0);
        LCD_WR_DATA(0x0F);
        LCD_WR_DATA(0x2A);
        LCD_WR_DATA(0x28);
        LCD_WR_DATA(0x08);
        LCD_WR_DATA(0x0E);
        LCD_WR_DATA(0x08);
        LCD_WR_DATA(0x54);
        LCD_WR_DATA(0XA9);
        LCD_WR_DATA(0x43);
        LCD_WR_DATA(0x0A);
        LCD_WR_DATA(0x0F);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x00);
        LCD_WR_REG(0XE1);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x15);
        LCD_WR_DATA(0x17);
        LCD_WR_DATA(0x07);
        LCD_WR_DATA(0x11);
        LCD_WR_DATA(0x06);
        LCD_WR_DATA(0x2B);
        LCD_WR_DATA(0x56);
        LCD_WR_DATA(0x3C);
        LCD_WR_DATA(0x05);
        LCD_WR_DATA(0x10);
        LCD_WR_DATA(0x0F);
        LCD_WR_DATA(0x3F);
        LCD_WR_DATA(0x3F);
        LCD_WR_DATA(0x0F);
        LCD_WR_REG(0x2B);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0xef);
        LCD_WR_REG(0x2A);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x01);
        LCD_WR_DATA(0x3f);
        LCD_WR_REG(0x11);
        delay_ms(120);
        LCD_WR_REG(0x29);
#if SCREEN_ORIENTATION_LANDSCAPE
        LCDSetWindow(0, 320, 0, 240);
#else
        LCDSetWindow(0, 240, 0, 320);
#endif
    }
    LCDClear(LCD_COLOR_BLACK);
}


/*********************************************************************************************
* 名称: LCDSetWindow
* 功能: 设置窗口
* 参数: x:窗口起始X坐标,xe:窗口终点X坐标,y:窗口起始Y坐标,ye:窗口终点Y坐标
* 返回: 无
*********************************************************************************************/
void LCDSetWindow(short x, short xe, short y, short ye)
{
    if (LCD_ID == 0x9341)
    {
        LCD_WR_REG(0X2A);
        LCD_WR_DATA(x>>8);
        LCD_WR_DATA(x&0XFF);
        LCD_WR_DATA(xe>>8);
        LCD_WR_DATA(xe&0XFF);
        LCD_WR_REG(0X2B);
        LCD_WR_DATA(y>>8);
        LCD_WR_DATA(y&0XFF);
        LCD_WR_DATA(ye>>8);
        LCD_WR_DATA(ye&0XFF);
    }
    else
    {
#if SCREEN_ORIENTATION_LANDSCAPE
        ILI93xx_WriteReg(0x52, x);
        ILI93xx_WriteReg(0x53, xe);
        ILI93xx_WriteReg(0x50, y);
        ILI93xx_WriteReg(0x51, ye);
#else
        ILI93xx_WriteReg(0x52, y);
        ILI93xx_WriteReg(0x53, ye);
        ILI93xx_WriteReg(0x50, x);
        ILI93xx_WriteReg(0x51, xe);
#endif
    }
}

/*********************************************************************************************
* 名称: LCDSetCursor
* 功能: 设置光标
* 参数: x,y:坐标
* 返回: 无
*********************************************************************************************/
void LCDSetCursor(int x, int y)
{
    if (LCD_ID == 0x9341)
    {
        LCD_WR_REG(0X2A);
        LCD_WR_DATA(x>>8);
        LCD_WR_DATA(x&0XFF);
        LCD_WR_REG(0X2B);
        LCD_WR_DATA(y>>8);
        LCD_WR_DATA(y&0XFF);
    }
    if ((LCD_ID == 0x9325) || (LCD_ID == 0x9328))
    {
#if SCREEN_ORIENTATION_LANDSCAPE
        ILI93xx_WriteReg(0x21, x);
        ILI93xx_WriteReg(0x20, y);
#else
        ILI93xx_WriteReg(0x21, y);
        ILI93xx_WriteReg(0x20, x);
#endif
    }
}

/*********************************************************************************************
* 名称: LCDWriteData
* 功能: 写固定长度数据
* 参数: dat:数据,len:数据长度
* 返回: 无
*********************************************************************************************/
void LCDWriteData(uint16_t *dat, int len)
{
    int i;
    ILI93xx_REG = CMD_WR_RAM;
    for (i=0; i<len; i++)
    {
        ILI93xx_DAT = dat[i];
    }
}

/*********************************************************************************************
* 名称: LCDClear
* 功能: LCD清屏
* 参数: color:清屏颜色
* 返回: 无
*********************************************************************************************/
void LCDClear(uint16_t color)
{
    long i;
    LCDSetCursor(0,0);
    ILI93xx_REG = CMD_WR_RAM;
    for (i=0; i<LCD_WIDTH*LCD_HEIGHT; i++)
    {
        ILI93xx_DAT = color;
    }
}

/*********************************************************************************************
* 名称: LCD_DrawPixel
* 功能: 画点
* 参数: x,y:坐标,color:点的颜色
* 返回: 无
*********************************************************************************************/
void LCD_DrawPixel(short x, short y, uint32_t color)
{
    LCDSetCursor(x, y);
    ILI93xx_REG = CMD_WR_RAM;
    ILI93xx_DAT = color;
}

/*********************************************************************************************
* 名称: LCDDrawGB_24_1
* 功能: 显示一个汉字(24*24)
* 参数: x,y:显示坐标,gb2:汉字字符串,color:字符颜色,bc:背景色
* 返回: 无
*********************************************************************************************/
void LCDDrawGB_24_1(int x, int y, char *gb2, int color, int bc)
{
    char dot;
    int i, j, k, m;

    for (i=0; i<GB_24_SIZE; i++)
    {
        if (gb2[0] == GB_24[i].Index[0] && gb2[1] == GB_24[i].Index[1])
        {
            for (j=0; j<24; j++)
            {
                for (k=0; k<3; k++)
                {
                    dot = GB_24[i].Msk[j*3+k];
                    for (m=0; m<8; m++)
                    {
                        if (dot & 1<<(7-m))
                        {
                            LCD_DrawPixel(x+k*8+m, y+j, color);
                        }
                        else if (bc > 0)
                        {
                            LCD_DrawPixel(x+k*8+m, y+j, bc);
                        }
                    }
                }
            }
            break;
        }
    }
}

/*********************************************************************************************
* 名称: LCDDrawAsciiDot12x24_1
* 功能: 显示一个ASCII(12*24)
* 参数: x,y:显示坐标,ch:显示字符,color:字符颜色,bc:背景色
* 返回: 无
*********************************************************************************************/
void LCDDrawAsciiDot12x24_1(int x, int y, char ch, int color, int bc)
{
    char dot;
    int i, j, k;
    if (ch<0x20 || ch > 0x7e) ch = 0x20;
    ch -= 0x20;
    for (i=0; i<3; i++) {
        for (j=0; j<12; j++) {
            dot = nAsciiDot12x24[ch*36+i*12+j];
            for (k=0; k<8; k++) {
                if (dot&1)LCD_DrawPixel(x+j, y+(i*8)+k, color);
                else if (bc > 0) LCD_DrawPixel(x+j, y+(i*8)+k, bc&0xffff);
                dot >>= 1;
            }
        }
    }
}

/*********************************************************************************************
* 名称: LCDDrawFnt24
* 功能: 显示多个汉字(24*24)
* 参数: x,y:显示坐标,str:汉字字符串,color:字符颜色,bc:背景色
* 返回: 无
*********************************************************************************************/
void LCDDrawFnt24(int x, int y, char *str, int color, int bc)
{
    while (*str != '\0')
    {
        if (*str & 0x80)
        {
            if (str[1] != '\0')
            {
                LCDDrawGB_24_1(x, y, str, color, bc);
                str += 2;
                x+= 24;
            }
            else break;
        }
        else
        {
            LCDDrawAsciiDot12x24_1(x, y, *str, color, bc);
            str ++;
            x += 12;
        }
    }
}

/*********************************************************************************************
* 名称: LCD_File
* 功能: 在指定大小清屏幕
* 参数: x1,y1:起始坐标值,x2,y2:终点坐标值,color:屏幕颜色
* 返回: 无
*********************************************************************************************/
void LCD_File(short x1,short y1,short x2,short y2,uint16_t color)
{
    int i, j;
    LCDSetWindow(x1,x2,y1,y2);
    LCDSetCursor(x1, y1);
    ILI93xx_REG = CMD_WR_RAM;
    for(i=x1; i<=x2; i++)
        for(j=y1; j<=y2; j++)
        {
            ILI93xx_DAT = color;
        }
#if SCREEN_ORIENTATION_LANDSCAPE
    LCDSetWindow(0, 320, 0, 240);
#else
    LCDSetWindow(0, 240, 0, 320);
#endif
}

/*********************************************************************************************
* 名称: LCD_Color_Fill
* 功能: 在指定区域显示指定颜色块
* 参数: (sx,xy)(ex,ey):矩形对角坐标,color:填充颜色
* 返回: 无
*********************************************************************************************/
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u8 *color)
{
    u16 color_temp=0;
    u16 height,width;
    u16 x,y;
    width=ex-sx;
    height=ey-sy;

    for(y=0; y<height; y+=1)
    {
        for(x=0; x<width; x+=1)
        {
            color_temp = color[2*(y*width+x)]<<8;
            color_temp |= color[2*(y*width+x)+1];
            LCD_DrawPixel(sx+x,sy+y,color_temp);
        }
    }
}

/*********************************************************************************************
* 名称: ILI93xx_LCD_DrawLine
* 功能: lcd画线函数
* 参数: x1,y1:起点坐标,x2,y2:终点坐标,color:线的颜色
* 返回: 无
*********************************************************************************************/
void ILI93xx_LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
    u16 t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1;
    delta_y=y2-y1;
    uRow=x1;
    uCol=y1;

    if(delta_x>0)incx=1;
    else if(delta_x==0)incx=0;
    else
    {
        incx=-1;
        delta_x=-delta_x;
    }

    if(delta_y>0)incy=1;
    else if(delta_y==0)incy=0;
    else
    {
        incy=-1;
        delta_y=-delta_y;
    }

    if( delta_x>delta_y)distance=delta_x;
    else distance=delta_y;
    for(t=0; t<=distance+1; t++ )
    {
        LCD_DrawPixel(uRow,uCol,color);
        xerr+=delta_x ;
        yerr+=delta_y ;
        if(xerr>distance)
        {
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            uCol+=incy;
        }
    }
}

void ILI93xx_LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
    ILI93xx_LCD_DrawLine(x1,y1,x2,y1,color);
    ILI93xx_LCD_DrawLine(x1,y1,x1,y2,color);
    ILI93xx_LCD_DrawLine(x1,y2,x2,y2,color);
    ILI93xx_LCD_DrawLine(x2,y1,x2,y2,color);
}

void ILI93xx_LCD_Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
    int a,b;
    int di;
    a=0;
    b=r;
    di=3-(r<<1);
    while(a<=b)
    {
        LCD_DrawPixel(x0+a,y0-b,color);
        LCD_DrawPixel(x0+b,y0-a,color);
        LCD_DrawPixel(x0+b,y0+a,color);
        LCD_DrawPixel(x0+a,y0+b,color);
        LCD_DrawPixel(x0-a,y0+b,color);
        LCD_DrawPixel(x0-b,y0+a,color);
        LCD_DrawPixel(x0-a,y0-b,color);
        LCD_DrawPixel(x0-b,y0-a,color);
        a++;
        if(di<0)di +=4*a+6;
        else
        {
            di+=10+4*(a-b);
            b--;
        }
    }
}
