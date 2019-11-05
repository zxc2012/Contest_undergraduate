/*---------------------------------------------------------------------------------------
                                     ����������ײ�
���ܣ���������ʼ���Լ�������ȡ��δ���ԣ�
���ߣ�WK
ʱ�䣺2018.4.16
˵�����������ȡ����������
----------------------------------------------------------------------------------------*/
#include "headfile.h"
Motor_St MotorStatus_t={0};//����״̬

/*------------------------
     �����������ʼ��
------------------------*/
void Encoder_Init(void)
{
	ftm_quad_init(ftm1);	//
	ftm_quad_init(ftm2);	//

}




/*------------------------------------------------------------------------------
 �ɼ�����ٶ�
------------------------------------------------------------------------------*/
void MOTOR_get(void)
{
	MotorStatus_t.LH_Spd=ftm_quad_get(ftm1);	
	ftm_quad_clean(ftm1);

	MotorStatus_t.RH_Spd=-ftm_quad_get(ftm2);
	ftm_quad_clean(ftm2);
        
        Speed_Value_LH+=MotorStatus_t.LH_Spd;
        Speed_Value_RH+=MotorStatus_t.RH_Spd;

}

void ALLEncoder_clear (void)
{
  ftm_quad_clean(ftm1);
  ftm_quad_clean(ftm2);
  Communication_send(1);
}

