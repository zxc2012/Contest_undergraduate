
#include "headfile.h"
extern int WangToNum;
void PIT0_IRQHandler(void)
{

    static uint16 cnt_1ms=0;//1ms����
    static uint16 cnt_2ms=0;//2ms����
    static uint16 cnt_3ms=0;//3ms����
    static uint16 cnt_5ms=0;//5ms����
    static uint16 cnt_10ms=0;//10ms����
    static uint16 cnt_15ms=0;//15ms����
    static uint16 cnt_50ms=0;//30ms����
    static uint16 cnt_20ms=0;//20ms����
    
    static uint16 cnt_30ms=0;//30ms����
    
    static uint16 cnt_xxms=0;//20ms����
    static uint16 cnt_1000ms=0;//1000ms����
    
    if(++cnt_1ms>=1)
    {
        cnt_1ms=0;
//        Loop_check();
//        CountTimeBase();//��ʱ���ܵ���
    }

    if(++cnt_2ms>=2)//2ms����
    {     
        cnt_2ms=0;//���־    
    }
    if(++cnt_3ms>=3)//3ms����
    {
      MOTOR_get();  //��ȡ�ٶ�
      Angle_get();  //��ȡ�Ƕ�
      cnt_3ms=0;//���־
    }
    if(++cnt_5ms>=5)//5ms����
    {   
        SysCtrl_Angle_PID(Angle_to_Ctro,SystemAttitude.Yaw,SysPID[Head_Angle].EnPid_Out);  //�ǶȻ�
        SysCtrl_Speed_PID_Update();//�ٶȼ��㼰PID����  //speed N.2
        Car_Move();//�ٶ�PID�����ֵ  //speed N.4

        cnt_5ms=0;//���־   
    }

    if(++cnt_10ms>=10)
    {
        if(CarEnterOk == 1)  //������֮��
        {
            position_pid(now_position,WangToNum);  //speed N.1
            if(SwitchMode == 1)
            {
                Track_PlaceObstacle();  //���ϰ�
                Servo_StateCtr();
            }
            else
            {
                Track_EightQueen();  //�˻ʺ�
//                Servo_StateCtr();
            }
        }
    
        cnt_10ms=0;	
    }

    if(++cnt_15ms>=15)
    {
        
        cnt_15ms=0;
    }

    if(++cnt_20ms>=20)
    {
       if(CarEnterOk == 1)
       {
         if(car_move_state==Car_serching)
           WALK_SPEED=30;
         else if(car_move_state==Car_chessing)
           TimeChangeFreq();
       }
            
        cnt_20ms=0; 
    }

    if(++cnt_50ms>=30)
    {
//        TimeChangeServoSetPwm();//������ٶ���
        cnt_50ms=0;
    }

    if(++cnt_xxms>=100)
    {

        cnt_xxms=0;   
            
    }
        
    if(++cnt_1000ms>=1000)
    {       	 
        cnt_1000ms=0;
    }
    
    
      PIT_FlAG_CLR(pit0);



}


/*
*��ʱ��20ms���ٸı�Ƶ��
*/
#define ChangePwmValue	(16)
void TimeChangeFreq(void)
{
	if(WALK_SPEED>WALK_SPEED_SET)//ֱ�Ӽ���
	  WALK_SPEED = WALK_SPEED_SET;
	else if(WALK_SPEED<WALK_SPEED_SET)//���ȼ���
	  WALK_SPEED += ChangePwmValue;
		
}



