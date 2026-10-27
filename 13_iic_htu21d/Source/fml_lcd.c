#include "fml_lcd.h"
#include "ili93xx.h"
#if LCD_FONT_EN
#include "font_tft.h"
#include "font.h"
#include "gbk_st16.h"
#endif

lcd_dev_t lcd_dev = {
  &screen_dev
};

lcd_phy_t LCD = {
  st7789_PrepareFill,
  st7789_DrawPoint,
  LCD_ReadPoint,
  st7789_FillColor,
  st7789_FillData
};

void set_LCDili93xx(void)
{
  LCD.PrepareFill = LCDSetWindow;
  LCD.DrawPoint = LCD_DrawPixel;
  LCD.ReadPoint = NULL;
  LCD.FillRectangle = LCD_File;
  LCD.FillData = NULL;
}

void (*TouchIrq)(void) = NULL;

void TouchIrqSet(void (*func)(void))
{
  TouchIrq = func;
}


uint8_t LCD_DriverInit(void)
{
  if(st7789_init())
  {
    ILI93xxInit();
    set_LCDili93xx();
  }
  return 0;
}


void LCD_Clear(uint16_t color)
{
  LCD.FillRectangle(0,0,lcd_dev.screen->wide,lcd_dev.screen->high,color);
}


void LCD_FillColor(short x0,short y0,short x1,short y1,uint16_t color)
{
  LCD.FillRectangle(x0,y0,x1,y1,color);
}

/*==========================================================================================*/


/*********************************************************************************************
* 名称: LCD_DrawLine
* 功能: lcd画线函数
* 参数: x1,y1:起点坐标,x2,y2:终点坐标,color:线的颜色
* 返回:
* 修改:
* 注释:
*********************************************************************************************/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
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
    LCD.DrawPoint(uRow,uCol,color);
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


void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
  LCD_DrawLine(x1,y1,x2,y1,color);
  LCD_DrawLine(x1,y1,x1,y2,color);
  LCD_DrawLine(x1,y2,x2,y2,color);
  LCD_DrawLine(x2,y1,x2,y2,color);
}


void LCD_DrawCircle(u16 x0,u16 y0,u8 r,u16 color)
{
  int a,b;
  int di;
  a=0;
  b=r;
  di=3-(r<<1);
  while(a<=b)
  {
    LCD.DrawPoint(x0+a,y0-b,color);
    LCD.DrawPoint(x0+b,y0-a,color);
    LCD.DrawPoint(x0+b,y0+a,color);
    LCD.DrawPoint(x0+a,y0+b,color);
    LCD.DrawPoint(x0-a,y0+b,color);
    LCD.DrawPoint(x0-b,y0+a,color);
    LCD.DrawPoint(x0-a,y0-b,color);
    LCD.DrawPoint(x0-b,y0-a,color);
    a++;
    if(di<0)di +=4*a+6;
    else
    {
      di+=10+4*(a-b);
      b--;
    }
  }
}


#if LCD_FONT_EN

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode,u16 color,u16 backColor)
{
  u8 temp,t1,t;
  u16 y0=y;
  u8 csize=(size/8+((size%8)?1:0))*(size/2);
  num=num-' ';
  for(t=0; t<csize; t++)
  {
    if(size==12)temp=tft_asc2_1206[num][t];
    else if(size==16)temp=tft_asc2_1608[num][t];
    else if(size==24)temp=tft_asc2_2412[num][t];
    else return;
    for(t1=0; t1<8; t1++)
    {
      if(temp&0x80)LCD.DrawPoint(x,y,color);
      else if(mode==0)LCD.DrawPoint(x,y,backColor);
      temp<<=1;
      y++;
      if(y>=LCD_HEIGHT_MAX)return;
      if((y-y0)==size)
      {
        y=y0;
        x++;
        if(x>=LCD_WIDTH_MAX)return;
        break;
      }
    }
  }
}


void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u16 color,u16 backColor)
{
  u8 x0=x;
  width+=x;
  height+=y;
  while((*p<='~')&&(*p>=' '))
  {
    if(x>=width)
    {
      x=x0;
      y+=size;
    }
    if(y>=height)break;
    LCD_ShowChar(x,y,*p,size,0,color,backColor);
    x+=size/2;
    p++;
  }
}


void LCD_ShowFont16(u16 x,u16 y,u8 *data,u16 Color,u16 BColor)
{
  unsigned int i,j,b,index;
  u8 tmp_char;
  for(b=0; b<1; b++)
  {
    index=(94*(data[b*2] - 0xa1)+(data[b*2+1] - 0xa1));
    for(i=0; i<16; i++)
    {
      for(j=0; j<8; j++)
      {
        tmp_char=gbk_st16[index*32+i*2];
        if ( (tmp_char << j) & 0x80)
        {
          LCD.DrawPoint(x+j,y+i,Color);
        }
        else
        {
          LCD.DrawPoint(x+j,y+i,BColor);
        }
      }
      for(j=0; j<8; j++)
      {
        tmp_char=gbk_st16[index*32+i*2+1];
        if ( (tmp_char << j) & 0x80)
        {
          LCD.DrawPoint(x+j+8,y+i,Color);
        }
        else
        {
          LCD.DrawPoint(x+j+8,y+i,BColor);
        }
      }
    }
  }
}

void LCDDrawAsciiDot12x24(int x, int y, char ch, int color, int bc)
{
  char dot;
  int i, j, k;
  if (ch<0x20 || ch > 0x7e) ch = 0x20;
  ch -= 0x20;
  for (i=0; i<3; i++) {
    for (j=0; j<12; j++) {
      dot = nAsciiDot12x24[ch*36+i*12+j];
      for (k=0; k<8; k++) {
        if (dot&1)LCD.DrawPoint(x+j, y+(i*8)+k, color);
        else if (bc > 0) LCD.DrawPoint(x+j, y+(i*8)+k, bc&0xffff);
        dot >>= 1;
      }
    }
  }
}

void LCDDrawGB_24(int x, int y, char *gb2, int color, int bc)
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
                            LCD.DrawPoint(x+k*8+m, y+j, color);
                        }
                        else  if (bc > 0)
                        {
                            LCD.DrawPoint(x+k*8+m, y+j, bc);
                        }
                    }
                }
            }
            break;
        }
    }
}

void LCDShowFont16(u16 x,u16 y,char* p,u16 width,u16 color,u16 Bcolor)
{
    u8* temp=(u8*)p;
    u16 x1=x;

    while(x1<(x+width))
    {
        if(x1>LCD_WIDTH_MAX) return;
        
        if(*temp!='\0')
        {
            if (*temp & 0x80)
            {
                LCD_ShowFont16(x1,y,temp,color,Bcolor);
                temp+=2;
                x1+=16;
            }
            else
            {
                LCD_ShowChar(x1,y,*temp,16,0,color,Bcolor);
                temp ++;
                x1 += 8;
            }
        }
        else
        {
            LCD_ShowChar(x1,y,' ',16,0,color,Bcolor);
            x1 += 8;
        }
    }
}

void LCD_ShowFontString(u16 x,u16 y,char* text,u16 textSize,u16 showLen,u16 textColor,u16 backColor)
{
  u8* temp=(u8*)text;
  u16 x1=x;
  
  while(x1<(x+showLen))
  {
    if(*temp!='\0')
    {
      if (*temp & 0x80)
      {
        LCD_ShowFont16(x1,y,temp,textColor,backColor);
        temp+=2;
        x1+=textSize;
      }
      else
      {
        LCD_ShowChar(x1,y,*temp,16,0,textColor,backColor);
        temp ++;
        x1 += (textSize/2);
      }
    }
    else
    {
      LCD_ShowChar(x1,y,' ',16,0,textColor,backColor);
      x1 += 8;
    }
  }
}

void LCDDrawFont16_Next(int x, int y, int xs, int xe,char *str, int color, int bc)
{
  while (*str != '\0') {
    if (*str & 0x80) {
      if (str[1] != '\0') {
        LCD_ShowFont16(x, y, (unsigned char*)str, color, bc);
        str += 2;
        x+= 16;
        if(x > (xe-16)){
          x = xs;
          y = y + 20;
        }
      } else break;
    } else {
      LCD_ShowChar(x, y, *str, 16, 0, color, bc);
      str ++;
      x += 8;
      if(x > (xe-8)){
          x = xs;
          y = y + 20;
        }
    }
  }
}

void LCDDrawFont24(int x, int y, char *str, int color, int bc)
{
  while (*str != '\0')
    {
        if (*str & 0x80)
        {
            if (str[1] != '\0')
            {
                LCDDrawGB_24(x, y, str, color, bc);
                str += 2;
                x+= 24;
            }
            else break;
        }
        else
        {
            LCDDrawAsciiDot12x24(x, y, *str, color, bc);
            str ++;
            x += 12;
        }
    }
}

#endif
