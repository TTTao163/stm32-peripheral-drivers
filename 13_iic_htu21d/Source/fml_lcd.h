#ifndef _FML_LCD_H_
#define _FML_LCD_H_

#include "stdio.h"
#include "stm32f4xx.h"
#include "st7789.h"

#define LCD_FONT_EN 1

#define LCD_WIDTH_MAX       320
#define LCD_HEIGHT_MAX      240

#define RGB888toRGB565(c24)  ((unsigned short)(((c24&0x0000f8)>>3)+((c24&0x00fc00)>>5)+((c24&0xf80000)>>8)))

#define LCD_COLOR_BLACK             0x0000
#define LCD_COLOR_WHITE             0xFFFF

#define LCD_COLOR_GRAY_1            RGB888toRGB565(0xEFEFEF)
#define LCD_COLOR_GRAY_2            RGB888toRGB565(0xDFDFDF)
#define LCD_COLOR_GRAY_3            RGB888toRGB565(0xBFBFBF)
#define LCD_COLOR_GRAY_4            RGB888toRGB565(0x7F7F7F)

#define LCD_COLOR_RED               RGB888toRGB565(0xFF0000)
#define LCD_COLOR_LIGHT_RED         RGB888toRGB565(0xFF8080)
#define LCD_COLOR_DARK_RED          RGB888toRGB565(0x800000)

#define LCD_COLOR_GREEN             RGB888toRGB565(0x00FF00)
#define LCD_COLOR_LIGHT_GREEN       RGB888toRGB565(0x80FF80)
#define LCD_COLOR_DARK_GREEN        RGB888toRGB565(0x008000)

#define LCD_COLOR_BLUE              RGB888toRGB565(0x0000FF)
#define LCD_COLOR_LIGHT_BLUE        RGB888toRGB565(0x8080FF)
#define LCD_COLOR_DARK_BLUE         RGB888toRGB565(0x000080)

#define LCD_COLOR_YELLOW            RGB888toRGB565(0xFFFF00)
#define LCD_COLOR_LIGHT_YELLOW      RGB888toRGB565(0xFFFF80)
#define LCD_COLOR_DARK_YELLOW       RGB888toRGB565(0x808000)

#define LCD_COLOR_PURPLE            RGB888toRGB565(0xFF00FF)
#define LCD_COLOR_LIGHT_PURPLE      RGB888toRGB565(0xFF80FF)
#define LCD_COLOR_DARK_PURPLE       RGB888toRGB565(0x800080)

#define LCD_COLOR_CYAN              RGB888toRGB565(0x00FFFF)
#define LCD_COLOR_LIGHT_CYAN        RGB888toRGB565(0x80FFFF)
#define LCD_COLOR_DARK_CYAN         RGB888toRGB565(0x008080)

#define LCD_COLOR_BROWN             RGB888toRGB565(0x804000)
#define LCD_COLOR_LIGHT_BROWN       RGB888toRGB565(0xFF8000)

#define LCD_COLOR_ORANGE            RGB888toRGB565(0xFF8000)


typedef struct
{
  screen_dev_t* screen;
}lcd_dev_t;

typedef struct
{
  void (*PrepareFill)(short x1, short y1, short x2, short y2);
  void (*DrawPoint)(short x,short y,uint32_t color);
  uint32_t (*ReadPoint)(short x,short y);
  void (*FillRectangle)(short x1,short y1,short x2,short y2,uint16_t color);
  void (*FillData)(short x1,short y1,short x2,short y2,unsigned short* dat);
}lcd_phy_t;


extern lcd_dev_t lcd_dev;
extern lcd_phy_t LCD;
extern void (*TouchIrq)(void);


void TouchIrqSet(void (*func)(void));

uint8_t LCD_DriverInit(void);
void LCD_Clear(uint16_t color);
void LCD_FillColor(short x0,short y0,short x1,short y1,uint16_t color);

//draw
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void LCD_DrawCircle(u16 x0,u16 y0,u8 r,u16 color);

//font
void LCDShowFont16(u16 x,u16 y,char* p,u16 width,u16 color,u16 Bcolor);
void LCD_ShowFontString(u16 x,u16 y,char* text,u16 textSize,u16 showLen,u16 textColor,u16 backColor);
void LCDDrawFont16_Next(int x, int y, int xs, int xe,char *str, int color, int bc);
void LCDDrawFont24(int x, int y, char *str, int color, int bc);

#endif
