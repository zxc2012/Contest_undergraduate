#ifndef _EDGE_EXTRACT_H_
#define _EDGE_EXTRACT_H_

#include "headfile.h"


/*-------------------------�궨����-------------------------------------*/
#define COL     128//ͼ����   ��Χ1-752     K60�ɼ���������188
#define ROW     64//ͼ��߶�	��Χ1-480

#define sWHITE 255
#define sBLACK 0
#define AG_max 180
#define diagonal_line 143    //ͼ��Խ��߳���



/*----------------------------------------------------------------------*/

/*------------------------------����������------------------------------*/
extern uint8 temp[ROW][COL];
extern uint8 SL_img[ROW][COL];
extern double sin_value[180];
extern double cos_value[180];
extern uint8 hough_acc[AG_max][diagonal_line];
extern int16 left_edge[ROW];
extern int16 right_edge[ROW];
extern int16 heng_edge[COL];
extern int16 midline[ROW];
extern uint8 left_flag[ROW],right_flag[ROW],midline_flag[ROW],heng_flag[COL];
extern uint8 break_row;


/*------------------------------����������------------------------------*/
extern void edge_extract(void);
extern void trigonometric_function_int(void);
extern void Hough_Transfom(int Hough_Threshold);

#endif