/*---------------------------------------------------------------------------------------
                                     ���PID���Ƶײ�
���ܣ�����ٶȻ�PID����
���ߣ�NZX
ʱ�䣺2018.4.9
˵���������������ĸ�������ٶ�PID����
----------------------------------------------------------------------------------------*/
#include "headfile.h"

/*---------------------------
       ���������ز���
---------------------------*/




/*---------------------------------
	    PID��ʼ��
---------------------------------*/
void Moto_Pid_Init(void)
{
  /*----------A���-----------*/
  MOTO_PID_A.PID_P = 12.0f;
  MOTO_PID_A.PID_I = 0.2f;
  MOTO_PID_A.PID_D = 0.00;

  MOTO_PID_A.target_value = 0;


  /*----------B���-----------*/
  MOTO_PID_B.PID_P = 12.0f;
  MOTO_PID_B.PID_I = 0.2f;
  MOTO_PID_B.PID_D = 0.00f;

  MOTO_PID_B.target_value = 0;


  /*----------C���-----------*/
  MOTO_PID_C.PID_P = 12.0f;
  MOTO_PID_C.PID_I = 0.2f;
  MOTO_PID_C.PID_D = 0.00f;

  MOTO_PID_C.target_value = 0;


  /*----------D���-----------*/
  MOTO_PID_D.PID_P = 12.0f;
  MOTO_PID_D.PID_I = 0.2f;
  MOTO_PID_D.PID_D = 0.00f;

  MOTO_PID_D.target_value = 0;

}


/*---------------------------------
	   ������ƺ���
---------------------------------*/
void MOTO_CTRL(void)
{
  /*---��������ֵ---*/
  MOTO_PID_A.actual_value = MOTOR_get(MOTO_A);
  MOTO_PID_B.actual_value = MOTOR_get(MOTO_B);
//  MOTO_PID_C.actual_value = MOTOR_get(MOTO_C);
//  MOTO_PID_D.actual_value = MOTOR_get(MOTO_D);

//  if(MOTO_PID_A.actual_value > 160) MOTO_PID_A.actual_value = 160;
//  if(MOTO_PID_B.actual_value > 160) MOTO_PID_B.actual_value = 160;
//  if(MOTO_PID_C.actual_value > 160) MOTO_PID_C.actual_value = 160;
//  if(MOTO_PID_D.actual_value > 160) MOTO_PID_D.actual_value = 160;


  Speed_Value_A += MOTO_PID_A.actual_value;      //A�����������
  Speed_Value_B += MOTO_PID_B.actual_value;      //B���
  Speed_Value_C += MOTO_PID_C.actual_value;      //C���
  Speed_Value_D += MOTO_PID_D.actual_value;      //D���


  /*---PID����---*/
  Moto_Speed_Ctrl_A();
  Moto_Speed_Ctrl_B();
  Moto_Speed_Ctrl_C();
  Moto_Speed_Ctrl_D();

  /*---���PWM---*/
  Moto_Set_Pwm(MOTO_A,(int)MOTO_PID_A.PWM_OUT);
  Moto_Set_Pwm(MOTO_B,(int)MOTO_PID_B.PWM_OUT);
  Moto_Set_Pwm(MOTO_C,(int)MOTO_PID_C.PWM_OUT);
  Moto_Set_Pwm(MOTO_D,(int)MOTO_PID_D.PWM_OUT);
}



/*---------------------------------------------------------------------------------------------------*/



/*---------------------------------
	   A���PID�ٶȻ�
---------------------------------*/
void Moto_Speed_Ctrl_A(void)
{
  float PWM_P = 0.0f, PWM_I = 0.0f, PWM_D = 0.0f;

  MOTO_PID_A.error[0] = MOTO_PID_A.target_value - MOTO_PID_A.actual_value;


  PWM_P = MOTO_PID_A.PID_P * (MOTO_PID_A.error[0] - MOTO_PID_A.error[1]);
  PWM_I = MOTO_PID_A.PID_I * MOTO_PID_A.error[0];
  PWM_D = MOTO_PID_A.PID_D * (MOTO_PID_A.error[0] - 2 * MOTO_PID_A.error[1] + MOTO_PID_A.error[2]);
  MOTO_PID_A.PWM_OUT += PWM_P + PWM_I + PWM_D;


  MOTO_PID_A.error[2] = MOTO_PID_A.error[1];
  MOTO_PID_A.error[1] = MOTO_PID_A.error[0];


  MOTO_PID_A.PWM_OUT = MOTO_Scope_limit(MOTO_PID_A.PWM_OUT);



}

/*---------------------------------
	   B���PID�ٶȻ�
---------------------------------*/
void Moto_Speed_Ctrl_B(void)
{
  float PWM_P = 0.0f, PWM_I = 0.0f, PWM_D = 0.0f;

  MOTO_PID_B.error[0] = MOTO_PID_B.target_value - MOTO_PID_B.actual_value;


  PWM_P = MOTO_PID_B.PID_P * (MOTO_PID_B.error[0] - MOTO_PID_B.error[1]);
  PWM_I = MOTO_PID_B.PID_I * MOTO_PID_B.error[0];
  PWM_D = MOTO_PID_B.PID_D * (MOTO_PID_B.error[0] - 2 * MOTO_PID_B.error[1] + MOTO_PID_B.error[2]);
  MOTO_PID_B.PWM_OUT += PWM_P + PWM_I + PWM_D;


  MOTO_PID_B.error[2] = MOTO_PID_B.error[1];
  MOTO_PID_B.error[1] = MOTO_PID_B.error[0];


  MOTO_PID_B.PWM_OUT = MOTO_Scope_limit(MOTO_PID_B.PWM_OUT);

}

/*---------------------------------
	   C���PID�ٶȻ�
---------------------------------*/
void Moto_Speed_Ctrl_C(void)
{
  float PWM_P = 0.0f, PWM_I = 0.0f, PWM_D = 0.0f;

  MOTO_PID_C.error[0] = MOTO_PID_C.target_value - MOTO_PID_C.actual_value;


  PWM_P = MOTO_PID_C.PID_P * (MOTO_PID_C.error[0] - MOTO_PID_C.error[1]);
  PWM_I = MOTO_PID_C.PID_I * MOTO_PID_C.error[0];
  PWM_D = MOTO_PID_C.PID_D * (MOTO_PID_C.error[0] - 2 * MOTO_PID_C.error[1] + MOTO_PID_C.error[2]);
  MOTO_PID_C.PWM_OUT += PWM_P + PWM_I + PWM_D;


  MOTO_PID_C.error[2] = MOTO_PID_C.error[1];
  MOTO_PID_C.error[1] = MOTO_PID_C.error[0];


  MOTO_PID_C.PWM_OUT = MOTO_Scope_limit(MOTO_PID_C.PWM_OUT);

}

/*---------------------------------
	   D���PID�ٶȻ�
---------------------------------*/
void Moto_Speed_Ctrl_D(void)
{
  float PWM_P = 0.0f, PWM_I = 0.0f, PWM_D = 0.0f;

  MOTO_PID_D.error[0] = MOTO_PID_D.target_value - MOTO_PID_D.actual_value;


  PWM_P = MOTO_PID_D.PID_P * (MOTO_PID_D.error[0] - MOTO_PID_D.error[1]);
  PWM_I = MOTO_PID_D.PID_I * MOTO_PID_D.error[0];
  PWM_D = MOTO_PID_D.PID_D * (MOTO_PID_D.error[0] - 2 * MOTO_PID_D.error[1] + MOTO_PID_D.error[2]);
  MOTO_PID_D.PWM_OUT += PWM_P + PWM_I + PWM_D;


  MOTO_PID_D.error[2] = MOTO_PID_D.error[1];
  MOTO_PID_D.error[1] = MOTO_PID_D.error[0];


  MOTO_PID_D.PWM_OUT = MOTO_Scope_limit(MOTO_PID_D.PWM_OUT);
}


uint16 MAX_ARR = 10000;
/*---------------------------------
	    �޷�����
---------------------------------*/
float MOTO_Scope_limit(float data)
{
  return data = fabsf(data) > (float)MAX_ARR? fabsf(data) / data * MAX_ARR : data;
}











/*----------------------------------------------------------------------------------------------------
                                          λ�û����
-------------------------------------------------------------------------------------------------- */

PID_STR POSITION_PID_STR= {0};
PID_STR POSITION_PID_STR_A = {0};
PID_STR POSITION_PID_STR_B = {0};
PID_STR POSITION_PID_STR_C = {0};
PID_STR POSITION_PID_STR_D = {0};


uint8 POSITION_PID_FLAG = 0;
uint8 POSITION_ARRIVE_FLAG=0;

void Position_Pid_Init(void)
{
  POSITION_PID_STR_A.PID_P = 0.5f;
  POSITION_PID_STR_A.PID_I = 0.006f;
  POSITION_PID_STR_A.target_value = 0.0f;
  POSITION_PID_STR_A.Intergral = 0;

  POSITION_PID_STR_B.PID_P = 0.5f;
  POSITION_PID_STR_B.PID_I = 0.006f;
  POSITION_PID_STR_B.target_value = 0.0f;
  POSITION_PID_STR_B.Intergral = 0;

  POSITION_PID_STR_C.PID_P = 0.5f;
  POSITION_PID_STR_C.PID_I = 0.006f;
  POSITION_PID_STR_C.target_value = 0.0f;
  POSITION_PID_STR_C.Intergral = 0;

  POSITION_PID_STR_D.PID_P = 0.5f;
  POSITION_PID_STR_D.PID_I = 0.006f;
  POSITION_PID_STR_D.target_value = 0.0f;
  POSITION_PID_STR_D.Intergral = 0;

  Speed_Value_A = 0;
  Speed_Value_B = 0;
  Speed_Value_C = 0;
  Speed_Value_D = 0;
}


void Position_Ctrl(void)
{
  /* ���� */
  POSITION_PID_STR_A.actual_value = Speed_Value_A;
  POSITION_PID_STR_B.actual_value = Speed_Value_B;
  POSITION_PID_STR_C.actual_value = Speed_Value_C;
  POSITION_PID_STR_D.actual_value = Speed_Value_D;


  /* PID���� */
  PositionA_Pid_Ctrl();
  PositionB_Pid_Ctrl();
  PositionC_Pid_Ctrl();
  PositionD_Pid_Ctrl();

  /* ���ڻ����γɴ���PID */
  MOTO_PID_A.target_value = POSITION_PID_STR_A.PWM_OUT;
  MOTO_PID_B.target_value = POSITION_PID_STR_B.PWM_OUT;
  MOTO_PID_C.target_value = POSITION_PID_STR_C.PWM_OUT;
  MOTO_PID_D.target_value = POSITION_PID_STR_D.PWM_OUT;

  if(fabs(POSITION_PID_STR_A.error[0]) < 100 && fabs(POSITION_PID_STR_B.error[0]) < 100
  &&fabs( POSITION_PID_STR_C.error[0]) < 100 && fabs(POSITION_PID_STR_D.error[0]) < 100  ){
    MOTO_PID_A.target_value = 0;
    MOTO_PID_B.target_value = 0;
    MOTO_PID_C.target_value = 0;
    MOTO_PID_D.target_value = 0;
    POSITION_PID_FLAG = 0;



  }
}

void PositionA_Pid_Ctrl(void)
{
 float PWM_P = 0.0f, PWM_I = 0.0f, PWM_D = 0.0f;

  POSITION_PID_STR_A.error[0] = POSITION_PID_STR_A.target_value - POSITION_PID_STR_A.actual_value;


  PWM_P = POSITION_PID_STR_A.PID_P * (POSITION_PID_STR_A.error[0] - POSITION_PID_STR_A.error[1]);
  PWM_I = POSITION_PID_STR_A.PID_I * POSITION_PID_STR_A.error[0];
  PWM_D = POSITION_PID_STR_A.PID_D * (POSITION_PID_STR_A.error[0] - 2 * POSITION_PID_STR_A.error[1] + POSITION_PID_STR_A.error[2]);
  POSITION_PID_STR_A.PWM_OUT += PWM_P + PWM_I + PWM_D;


  POSITION_PID_STR_A.error[2] = POSITION_PID_STR_A.error[1];
  POSITION_PID_STR_A.error[1] = POSITION_PID_STR_A.error[0];


  POSITION_PID_STR_A.PWM_OUT = MAX_POSITION_Scope_limit(POSITION_PID_STR_A.PWM_OUT);
}



void PositionB_Pid_Ctrl(void)
{
 float PWM_P = 0.0f, PWM_I = 0.0f, PWM_D = 0.0f;

  POSITION_PID_STR_B.error[0] = POSITION_PID_STR_B.target_value - POSITION_PID_STR_B.actual_value;


  PWM_P = POSITION_PID_STR_B.PID_P * (POSITION_PID_STR_B.error[0] - POSITION_PID_STR_B.error[1]);
  PWM_I = POSITION_PID_STR_B.PID_I * POSITION_PID_STR_B.error[0];
  PWM_D = POSITION_PID_STR_B.PID_D * (POSITION_PID_STR_B.error[0] - 2 * POSITION_PID_STR_B.error[1] + POSITION_PID_STR_B.error[2]);
  POSITION_PID_STR_B.PWM_OUT += PWM_P + PWM_I + PWM_D;


  POSITION_PID_STR_B.error[2] = POSITION_PID_STR_B.error[1];
  POSITION_PID_STR_B.error[1] = POSITION_PID_STR_B.error[0];


  POSITION_PID_STR_B.PWM_OUT = MAX_POSITION_Scope_limit(POSITION_PID_STR_B.PWM_OUT);
}




void PositionC_Pid_Ctrl(void)
{
 float PWM_P = 0.0f, PWM_I = 0.0f, PWM_D = 0.0f;

  POSITION_PID_STR_C.error[0] = POSITION_PID_STR_C.target_value - POSITION_PID_STR_C.actual_value;


  PWM_P = POSITION_PID_STR_C.PID_P * (POSITION_PID_STR_C.error[0] - POSITION_PID_STR_C.error[1]);
  PWM_I = POSITION_PID_STR_C.PID_I * POSITION_PID_STR_C.error[0];
  PWM_D = POSITION_PID_STR_C.PID_D * (POSITION_PID_STR_C.error[0] - 2 * POSITION_PID_STR_C.error[1] + POSITION_PID_STR_C.error[2]);
  POSITION_PID_STR_C.PWM_OUT += PWM_P + PWM_I + PWM_D;


  POSITION_PID_STR_C.error[2] = POSITION_PID_STR_C.error[1];
  POSITION_PID_STR_C.error[1] = POSITION_PID_STR_C.error[0];


  POSITION_PID_STR_C.PWM_OUT = MAX_POSITION_Scope_limit(POSITION_PID_STR_C.PWM_OUT);
}



void PositionD_Pid_Ctrl(void)
{
 float PWM_P = 0.0f, PWM_I = 0.0f, PWM_D = 0.0f;

  POSITION_PID_STR_D.error[0] = POSITION_PID_STR_D.target_value - POSITION_PID_STR_D.actual_value;


  PWM_P = POSITION_PID_STR_D.PID_P * (POSITION_PID_STR_D.error[0] - POSITION_PID_STR_D.error[1]);
  PWM_I = POSITION_PID_STR_D.PID_I * POSITION_PID_STR_D.error[0];
  PWM_D = POSITION_PID_STR_D.PID_D * (POSITION_PID_STR_D.error[0] - 2 * POSITION_PID_STR_D.error[1] + POSITION_PID_STR_D.error[2]);
  POSITION_PID_STR_D.PWM_OUT += PWM_P + PWM_I + PWM_D;


  POSITION_PID_STR_D.error[2] = POSITION_PID_STR_D.error[1];
  POSITION_PID_STR_D.error[1] = POSITION_PID_STR_D.error[0];


  POSITION_PID_STR_D.PWM_OUT = MAX_POSITION_Scope_limit(POSITION_PID_STR_D.PWM_OUT);
}
uint16 MAX_POSITION = 100;
/*---------------------------------
	   λ�û��޷�����
---------------------------------*/
float MAX_POSITION_Scope_limit(float data)
{
  return data = fabsf(data) > (float)MAX_POSITION? fabsf(data) / data * MAX_POSITION : data;
}


