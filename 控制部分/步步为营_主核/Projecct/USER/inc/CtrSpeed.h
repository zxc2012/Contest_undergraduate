#ifndef _CTRSPEED_H_
#define _CTRSPEED_H_


#define C_Point   110	//ת�۵�
#define MAX_SPEED 70	//����ٶ�
#define MIN_SPEED 30	//��С�ٶ�
#define StartPid  60  //����PID��λ��


int16 GetSpeedByDistance(int16 distance);
void InitSpeedWithDistance();

#endif