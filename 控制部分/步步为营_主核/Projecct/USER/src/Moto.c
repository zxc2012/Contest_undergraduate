/*---------------------------------------------------------------------------------------
                                     ����ײ�����
���ܣ�����ײ������Լ����ƽӿ�
���ߣ�NZX
ʱ�䣺2018.4.9
˵��������������ɵ�PWM���ڵ������
----------------------------------------------------------------------------------------*/
#include "headfile.h"

uint16 PWM_MAX = 10000;         //������ռ�ձ����ֵ����FTMx_PRECISON����




/*---------------------------------
	    �����ʼ��
---------------------------------*/
void Moto_Init(void)
{
  /*front_left */
  ftm_pwm_init(ftm0, ftm_ch0,20000, 0); 
  ftm_pwm_init(ftm0, ftm_ch1,20000, 0);

  /*front_right*/
  ftm_pwm_init(ftm0, ftm_ch2,20000, 0);  
  ftm_pwm_init(ftm0, ftm_ch3,20000, 0);

  /*back_left*/
  ftm_pwm_init(ftm0, ftm_ch4,20000, 0);  
  ftm_pwm_init(ftm0, ftm_ch5,20000, 0);

  /*back_right*/
  ftm_pwm_init(ftm0, ftm_ch6,20000, 0);  
  ftm_pwm_init(ftm0, ftm_ch7,20000, 0);

}

void StopAll(void)
{	
	Moto_Set_Pwm(LH,0);
	Moto_Set_Pwm(RH,0);
	Moto_Set_Pwm(LB,0);
	Moto_Set_Pwm(RB,0);
}

/*---------------------------------------------------------
  ���PWM����
---------------------------------------------------------*/
void Moto_Set_Pwm(Wheel_E wheel_e, int PWM_IN)
{
  if (PWM_IN >= 0)
  {
    switch(wheel_e)
    {
      case LH:
        ftm_pwm_duty(ftm0, ftm_ch0,(uint32)PWM_IN);
        ftm_pwm_duty(ftm0, ftm_ch1,0);break;
      case RH:
        ftm_pwm_duty(ftm0, ftm_ch2,(uint32)PWM_IN);
        ftm_pwm_duty(ftm0, ftm_ch3,0);break;
      case RB:
        ftm_pwm_duty(ftm0, ftm_ch6,(uint32)PWM_IN);
        ftm_pwm_duty(ftm0, ftm_ch7,0);break;
      case LB:
        ftm_pwm_duty(ftm0, ftm_ch4,(uint32)PWM_IN);
        ftm_pwm_duty(ftm0, ftm_ch5,0);break;
    }
  }
  else
  {
    switch(wheel_e)
    {
      case LH:
        ftm_pwm_duty(ftm0, ftm_ch1,(uint32)abs(PWM_IN));
        ftm_pwm_duty(ftm0, ftm_ch0,0);break;
      case RH:
        ftm_pwm_duty(ftm0, ftm_ch3,(uint32)abs(PWM_IN));
        ftm_pwm_duty(ftm0, ftm_ch2,0);break;
      case RB:
        ftm_pwm_duty(ftm0, ftm_ch7,(uint32)abs(PWM_IN));
        ftm_pwm_duty(ftm0, ftm_ch6,0);break;
      case LB:
        ftm_pwm_duty(ftm0, ftm_ch5,(uint32)abs(PWM_IN));
        ftm_pwm_duty(ftm0, ftm_ch4,0);break;
    }

  }
}

void Car_Move(void)  //speed N.4
{
    if(En_Update_PID == Yes)
    {
        Moto_Set_Pwm(LH,SysPID[LH_Spe].Out_Int16);
        Moto_Set_Pwm(RH,SysPID[RH_Spe].Out_Int16);
        Moto_Set_Pwm(LB,SysPID[LB_Spe].Out_Int16);
        Moto_Set_Pwm(RB,SysPID[RB_Spe].Out_Int16);
    }
    else
    {
        StopAll();
        SysPID[LH_Spe].Out_Int16 = 0;
        SysPID[RH_Spe].Out_Int16 = 0;
        SysPID[LB_Spe].Out_Int16 = 0;
        SysPID[RB_Spe].Out_Int16 = 0;
    }
}

#define A_PARAMETER          (36.5f)       //  ����֮�־� cm        
#define B_PARAMETER          (34f)       //  ����֮��� cm



