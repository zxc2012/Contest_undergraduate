#ifndef _Attitude_Calculation_H
#define _Attitude_Calculation_H
//#define  PI            3.1415926f
/*****�ǶȻ��ȸ����******/
//#define  PERIODHZ      (float)357.143       /////����Ƶ��
#define  PERIODS       (float)0.0029           ////��������
//#define  PERIODHZ      (float)333.333       /////����Ƶ��
//#define  PERIODS       (float)0.003           ////��������

#define Kp 0
#define Ki 1
#define Kd 2
#define Kt 3

typedef struct{
  float W;
  float X;
  float Y;
  float Z;
}QuaternionTypedef;

typedef struct{ 
  float Pitch;  //������
  float Yaw;    //ƫ����
  float Roll;   //������
}EulerAngleTypedef;


typedef struct{
  float Xdata;
  float Ydata;
  float Zdata;
}AttitudeDatatypedef;
typedef struct
{
  float GYROXdata;
  float GYROYdata;
  float GYROZdata;
  float ACCXdata;
  float ACCYdata;
  float ACCZdata;
  float MAGXdata;
  float MAGYdata;
  float MAGZdata;
}sensorDatatypedef;
typedef struct pid_type
{
    float errorsum;
    int32  lasterror;
    int32  preerror;



}pid_s;

typedef struct
{
     float vset;
     float angleset;
     float anglemax;
     float anglezero;

}coef_tag;

typedef struct
{
  float point;
  
  float point_p;
}mul_pid_s;
extern mul_pid_s camera_pid;
extern QuaternionTypedef    Quaternion;   //��Ԫ��
extern EulerAngleTypedef    EulerAngle;   //ŷ����
extern QuaternionTypedef    AxisAngle;    //���
extern EulerAngleTypedef    EulerAngleRate;//��ǰŷ�����ٶ�

extern QuaternionTypedef    MeaQuaternion;
extern EulerAngleTypedef    MeaEulerAngle;
extern QuaternionTypedef    MeaAxisAngle;
extern EulerAngleTypedef    SystemAttitude;            /////��̬��

extern QuaternionTypedef    ErrQuaternion;
extern EulerAngleTypedef    ErrEulerAngle;
extern QuaternionTypedef    ErrAxisAngle;
extern AttitudeDatatypedef         Acc;
extern AttitudeDatatypedef         Gyro;
extern float wy_coef[4],angle_coef[4],v_coef[4],wx_coef[4],dir_coef[4];
extern coef_tag other_coef;
extern void Quaternion_init();
extern u16 bz_time;
extern void Attitude_UpdateGyro(void);
extern u8 sensor;
extern void Attitude_UpdateAcc(void);
/*******pid****/
extern pid_s wy_pid,angle_pid,wx_pid,v_pid,dir_pid;
extern void Angle_get (void);
int32 PID_Position_angle(pid_s *pid,float *coef,int32 result,int32 point);
int32 PID_Position(pid_s *pid,float *coef,int32 result,int32 point);
int32 PID_Position_v(pid_s *pid,float *coef,int32 result,int32 point);
int32 PID_Increase(pid_s *pid,float *coef,int32 result,int32 point);
#endif
