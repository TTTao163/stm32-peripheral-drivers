/*********************************************************************************************
* 文件: htu21d.h
* 说明: HTU21D温湿度传感器驱动头文件
*********************************************************************************************/
#ifndef _HTU21D_H_
#define _HTU21D_H_

void htu21d_init(void);
void htu21d_reset(void);
int htu21d_read_temp(void);
int htu21d_read_humi(void);
float htu21d_t(void);
float htu21d_h(void);

#endif
