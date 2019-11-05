#include "headfile.h"


uint16 a_Speed=540;

int16 SpeedBuff[C_Point-StartPid];	//�ٶȴ洢

/*
*���ݾ��뷵���ٶ��趨ֵ
*���룺����
*������ٶ��趨ֵ
*/

int16 GetSpeedByDistance(int16 distance)
{
    static uint32_t m_spd_2=MIN_SPEED*MIN_SPEED;//��С�ٶ�ƽ��
    int16 setSpeed=0;
	if(distance<0)		//����ֵ
	   distance=-distance;
        distance/=100;
        
	if(distance>=C_Point )//�ֶκ���
	{
		return MAX_SPEED; 
	}
	else if(distance<=StartPid)
	{
		return MIN_SPEED ;
	}
        
	return SpeedBuff[distance-StartPid];
	

}

/*
*�����ٶ������ı仯ֵ
*/
void InitSpeedWithDistance()
{

  uint16 i=0;
  float k=(float)(MAX_SPEED-MIN_SPEED)/(C_Point-StartPid);
  float b=MIN_SPEED-k*StartPid;
  int16 dis=0;
    for(i=0;i<(C_Point-StartPid);i++)
    {
            dis=i+StartPid;

    //             SpeedBuff[i]=-0.00807*dis*dis+3.426*dis-62.456;   //���٣��ʼ��
            SpeedBuff[i]=(int16)(k*dis+b);   //
    }

}

//
//void CtrSpeedPara(int16 distance,uint16 CurrentSpeed)
//{
//   if(distance<0)		//����ֵ
//	distance=-distance;
//   C_Point=( CurrentSpeed*CurrentSpeed-MIN_SPEED*MIN_SPEED)/a_Speed+StartPid;
//   InitSpeedWithDistance();
//    
//}