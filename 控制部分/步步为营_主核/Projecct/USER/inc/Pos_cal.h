#ifndef __Pos_cal_H_
#define __Pos_cal_H_

#include "headfile.h"
/*
* ����������
*/
extern float forward_back,left_right;
extern float POS_actual_value;
extern float POS_set_value;

/*
* ����������
*/
void Pos_cal(uint8 now_position_x,uint8 now_positin_y,uint8 aim_position_x,uint8 aim_positiom_y);
void Pos_get(void);
void Pos_clear (void);


#endif