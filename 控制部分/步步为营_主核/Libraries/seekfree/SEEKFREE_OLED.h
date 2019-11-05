/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		SEEKFREE_OLED.h
 * @brief      		0.96OLED������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 * @note
					NRF24L01���߶��壺
					------------------------------------
					    OLEDҺ��    ��Ƭ��
    					D0          A15
    					D1          A16
    					RES         A14
    					DC          B16
					------------------------------------
 ********************************************************************************************************************/



#ifndef _SEEKFREE_OELD_H
#define _SEEKFREE_OELD_H

#include "headfile.h"


/************************************************************************
     OLED һ�п�����ʾ16���ַ���������ʾ8�С�
************************************************************************/

#define OLED_SCL  A8  //D0
#define OLED_SDA  A7  //D1
#define OLED_RST  A6
#define OLED_DC   A5
#define X_WIDTH 128
#define Y_WIDTH 64

/*��������������������No Rewritten or error�� ��������������������*/
#define OLED_SCLH  gpio_set(OLED_SCL,1)//HIGH)// ʱ�Ӷ���
#define OLED_SCLL  gpio_set(OLED_SCL,0)//LOW)

#define OLED_SDAH  gpio_set(OLED_SDA,1)//HIGH)//���ݿ�D0
#define OLED_SDAL  gpio_set(OLED_SDA,0)//LOW)

#define OLED_RSTH  gpio_set(OLED_RST,1)//HIGH)//��λ���ܵ�ƽ
#define OLED_RSTL  gpio_set(OLED_RST,0)//LOW)

#define OLED_DCH   gpio_set(OLED_DC,1)//HIGH)
#define OLED_DCL   gpio_set(OLED_DC,0)//LOW)//ƫ�ó���

#define OLED_CSH   gpio_set(OLED_CS,1)//HIGH)
#define OLED_CSL   gpio_set(OLED_CS,0)//LOW)

#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xCF

/*****************************************************************/

extern uint8 TESLA_BMP[];
extern const uint8 F6x8[][6];
//�ڲ���������
extern void OLED_Set_Pos(uint8 x, uint8 y);
extern void OLED_WrDat(uint8 data);
extern void OLED_WrCmd(uint8 cmd);

//�ⲿ��������
extern void initOLED(void);
extern void OLED_CLS(void);
extern void OLED_P6x8Str(uint8 x,uint8 y,uint8 ch[]);
extern void OLED_P8x16Str(uint8 x,uint8 y,uint8 ch[]);
extern void OLED_BMP(uint8 bmp[]);
extern char OLED_int16(uint16 x,uint16 y,int16 num);
extern void OLED_double(uint16 a,uint16 b,double number,char precision);
extern void Draw_Picture(uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint8 bmp[]);
extern void OLED_image1(void);
void OLED_image2(void);


#endif

