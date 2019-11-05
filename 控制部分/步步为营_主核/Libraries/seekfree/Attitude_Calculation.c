/*********************************************************************************************************************
 *
 * @file       		Attitude_Calculation.c
 * @author     		Alex
 * @version    		v1.0
 * @Software 		IAR 8.1
 * @date       		2016-11-9
 ********************************************************************************************************************/

#include "headfile.h"
#include "Attitude_Calculation.h"

AttitudeDatatypedef         Acc;
AttitudeDatatypedef         Gyro;
u8 sensor=0;
#if (sensor==1)//BMX055
#define  XA    (-Acc.Xdata)
#define  YA     (Acc.Zdata)  
#define  ZA    (-Acc.Ydata)

#define  XG    (Gyro.Xdata)
#define  YG   (-Gyro.Zdata)
#define  ZG   (Gyro.Ydata)

#elif (sensor==0)//FXAS2100_FXOS8700
#define  XA    (-Acc.Xdata)
#define  YA     (Acc.Zdata)  
#define  ZA    (-Acc.Ydata)

#define  XG    (-Gyro.Xdata)
#define  YG   (-Gyro.Zdata)
#define  ZG   (-Gyro.Ydata)
#endif
pid_s wy_pid,angle_pid,wx_pid,v_pid,dir_pid;
#define  ATTITUDE_COMPENSATE_LIMIT   ((float)PI*PERIODS / 180  )


QuaternionTypedef    Quaternion;
EulerAngleTypedef    EulerAngle;
QuaternionTypedef    AxisAngle;
EulerAngleTypedef    EulerAngleRate;

QuaternionTypedef    MeaQuaternion;
EulerAngleTypedef    MeaEulerAngle;
QuaternionTypedef    MeaAxisAngle;

QuaternionTypedef    ErrQuaternion;
EulerAngleTypedef    ErrEulerAngle;
QuaternionTypedef    ErrAxisAngle;
coef_tag other_coef;
float angle_coef[4]={0,0,0,0};
float v_coef[4]={0,0,0,0};
float wx_coef[4]={0,0,0,0};
float wy_coef[4]={0,0,0,0};
float dir_coef[4]={0,0,0,0};
float XN, XE, XD;
float YN, YE, YD;
float ZN, ZE, ZD;


vuint8                 IsGyroOffsetReset =1;     /////�����Ҫ������������Ʈ�����򽫸ı�����Ϊ   1
sensorDatatypedef      sensor_data;
EulerAngleTypedef      SystemAttitude;            /////��̬��
EulerAngleTypedef      SystemAttitudeRate;        /////��̬���ٶ�
AttitudeDatatypedef    GyroOffset;
float a_angle=0;                      //���ٶȼ������������
float car_angle=0;
float SysYaw_last;
int32 car_diroutcore=0;               

void Quaternion_Normalize(QuaternionTypedef *Qu)       //��Ԫ����һ��
{
	float Normal = 0;
	Normal = sqrtf(Qu->W * Qu->W + Qu->X * Qu->X + Qu->Y * Qu->Y + Qu->Z * Qu->Z);
	if (isnan(Normal) || Normal <= 0)
	{
		Qu->W = 1;
		Qu->X = 0;
		Qu->Y = 0;
		Qu->Z = 0;
	}
	else
	{
		Qu->W /= Normal;
		Qu->X /= Normal;
		Qu->Y /= Normal;
		Qu->Z /= Normal;
	}
}


// ��Ԫ������, p=q^-1=q*/(|q|^2), ԭʼ��Ԫ����Ϊ��λ��Ԫ��
void Quaternion_Invert(QuaternionTypedef *p, const QuaternionTypedef *q)
{
	p->W = q->W;
	p->X = -q->X;
	p->Y = -q->Y;
	p->Z = -q->Z;
}


// ��Ԫ���˷�, result=pq
void Quaternion_Multi(QuaternionTypedef *result, const QuaternionTypedef *p, const QuaternionTypedef *q)
{
	result->W = p->W * q->W - p->X * q->X - p->Y * q->Y - p->Z * q->Z;
	result->X = p->W * q->X + p->X * q->W - p->Y * q->Z + p->Z * q->Y;
	result->Y = p->W * q->Y + p->X * q->Z + p->Y * q->W - p->Z * q->X;
	result->Z = p->W * q->Z - p->X * q->Y + p->Y * q->X + p->Z * q->W;
}



void Quaternion_ToEulerAngle(const QuaternionTypedef *q, EulerAngleTypedef *e)  //��Ԫ��תŷ����
{
	e->Roll = atan2f(2 * (q->W * q->X + q->Y * q->Z), 1 - 2 * (q->X * q->X + q->Y * q->Y));
	float k = 2 * (q->W * q->Y - q->Z * q->X);
	if (k > 1) k = 1;
	else if (k < -1) k = -1;
	e->Pitch = asinf(k);
	e->Yaw = atan2f(2 * (q->W * q->Z + q->X * q->Y), 1 - 2 * (q->Y * q->Y + q->Z * q->Z));
}


void Quaternion_FromEulerAngle(QuaternionTypedef *q, const EulerAngleTypedef *e) //ŷ����ת��Ԫ��
{
	float cosx = cosf(e->Roll / 2);
	float sinx = sinf(e->Roll / 2);
	float cosy = cosf(e->Pitch / 2);
	float siny = sinf(e->Pitch / 2);
	float cosz = cosf(e->Yaw / 2);
	float sinz = sinf(e->Yaw / 2);
	q->W = cosx * cosy * cosz + sinx * siny * sinz;
	q->X = sinx * cosy * cosz - cosx * siny * sinz;
	q->Y = cosx * siny * cosz + sinx * cosy * sinz;
	q->Z = cosx * cosy * sinz - sinx * siny * cosz;
}

void Quaternion_ToAxisAngle(const QuaternionTypedef *q, QuaternionTypedef *a) //��Ԫ��ת���
{
	a->W = 0;
	a->X = q->X;
	a->Y = q->Y;
	a->Z = q->Z;
	Quaternion_Normalize(a);
	a->W = acosf(q->W) * 2;
}

// ���ת��Ϊ��Ԫ��
void Quaternion_FromAxisAngle(QuaternionTypedef *q, const QuaternionTypedef *a)
{
	float c = cosf(a->W / 2);
	float s = sinf(a->W / 2);
	q->W = 0;
	q->X = a->X;
	q->Y = a->Y;
	q->Z = a->Z;
	Quaternion_Normalize(q);
	q->W = c;
	q->X *= s;
	q->Y *= s;
	q->Z *= s;
}



//���ٶ�����ת����Ԫ������
void Quaternion_FromGyro(QuaternionTypedef *q, float wx, float wy, float wz, float dt)
{
	q->W = 1;
	q->X = wx * dt / 2;
	q->Y = wy * dt / 2;
	q->Z = wz * dt / 2;
	Quaternion_Normalize(q);
}


// ʹ�ý��ٶȸ�����Ԫ��(һ�����������Runge-Kunta, �ȼ���ʹ�ýǶ�����)
void Quaternion_UpdateFromGyro(QuaternionTypedef *q, float x, float y, float z, float dt)
{
	float dW = 0.5 * (-q->X * x - q->Y * y - q->Z * z) * dt;
	float dX = 0.5 * (q->W * x + q->Y * z - q->Z * y) * dt;
	float dY = 0.5 * (q->W * y - q->X * z + q->Z * x) * dt;
	float dZ = 0.5 * (q->W * z + q->X * y - q->Y * x) * dt;
	q->W += dW;
	q->X += dX;
	q->Y += dY;
	q->Z += dZ;
	Quaternion_Normalize(q);
}



void QuaternionFromAcc(QuaternionTypedef *Qu, float ax, float ay, float az, float mx, float my, float mz)//�ɼ����ٶȼƻ����Ԫ��
{
	float Normal = 0;
	XD = ax;
	YD = ay;
	ZD = az;      //ȡ��������
	Normal = sqrtf(XD * XD + YD * YD + ZD * ZD);
	XD /= Normal;
	YD /= Normal;
	ZD /= Normal;                //��һ��


	XN = -mx; YN = -my; ZN = -mz;              //ȡ����������Ϊ��
	Normal = XD * XN + YD * YN + ZD * ZN;      //��
	XN -= Normal*XD;
	YN -= Normal*YD;
	ZN -= Normal*ZD;                       //������
	Normal = sqrtf(XN * XN + YN * YN + ZN * ZN);
	XN /= Normal;
	YN /= Normal;
	ZN /= Normal;                       //��һ��

	XE = YD*ZN - YN*ZD;                     //��//������
	YE = ZD*XN - ZN*XD;
	ZE = XD*YN - XN*YD;
	Normal = sqrtf(XE*XE + YE*YE + ZE*ZE);
	XE /= Normal;
	XE /= Normal;
	XE /= Normal;                   //��һ��

	Qu->W = 0.5 * sqrtf(XN + YE + ZD + 1);   //��ת����ת����Ԫ��
	Qu->X = (YD - ZE) / (4 * Qu->W);
	Qu->Y = (ZN - XD) / (4 * Qu->W);
	Qu->Z = (XE - YN) / (4 * Qu->W);
	Quaternion_Normalize(Qu);
	return;
}


void Quaternion_init()
{
	if (XA != 0 || YA != 0 || ZA != 0)
	{
		QuaternionFromAcc(&Quaternion, XA, YA, ZA, -1, 0, 0);
	}
	else
	{
		Quaternion.W = 1;
		Quaternion.X = 0;
		Quaternion.Y = 0;
		Quaternion.Z = 0;
	}
	Quaternion_ToEulerAngle(&Quaternion, &EulerAngle);
	Quaternion_ToAxisAngle(&Quaternion, &AxisAngle);
	EulerAngleRate.Pitch = 0;
	EulerAngleRate.Roll = 0;
	EulerAngleRate.Yaw = 0;
}


void Attitude_UpdateAcc(void)//����ںϸ���
{
	QuaternionTypedef    EstQuaternion;
	EulerAngleTypedef    EstEulerAngle;
	QuaternionTypedef    DivQuaternion;
	QuaternionTypedef    ComAxisangle;
	QuaternionTypedef    Compensate;
	QuaternionTypedef    Last;


	QuaternionFromAcc(&MeaQuaternion, 0, YA, ZA, -1, 0, 0);
	Quaternion_ToEulerAngle(&MeaQuaternion, &MeaEulerAngle);
	Quaternion_ToAxisAngle(&MeaQuaternion, &MeaAxisAngle); //���㵱ǰ���ٶȼ���̬

	EstEulerAngle.Roll = EulerAngle.Roll;
	EstEulerAngle.Pitch = EulerAngle.Pitch;
	EstEulerAngle.Yaw = 0;

	Quaternion_FromEulerAngle(&EstQuaternion, &EstEulerAngle);  //����ŷ����ת��Ԫ��

	   //�������������Ԫ��ƫ��
	Quaternion_Invert(&DivQuaternion, &EstQuaternion);
	Quaternion_Multi(&ErrQuaternion, &DivQuaternion, &MeaQuaternion);
	Quaternion_Normalize(&ErrQuaternion);
	Quaternion_ToEulerAngle(&ErrQuaternion, &ErrEulerAngle);
	Quaternion_ToAxisAngle(&ErrQuaternion, &ErrAxisAngle);


	//���У���޷�

	memcpy(&ComAxisangle, &ErrAxisAngle, sizeof(QuaternionTypedef));
	if (ComAxisangle.W > ATTITUDE_COMPENSATE_LIMIT)
	{
		ComAxisangle.W = ATTITUDE_COMPENSATE_LIMIT;
	}
	Quaternion_FromAxisAngle(&Compensate, &ComAxisangle);


	//ִ��У��

	memcpy(&Last, &EstQuaternion, sizeof(QuaternionTypedef));
	Quaternion_Multi(&EstQuaternion, &Last, &Compensate);


	Quaternion_ToEulerAngle(&EstQuaternion, &EstEulerAngle);
	EstEulerAngle.Yaw = EulerAngle.Yaw;//��ʹ�ü��ٶȼƲ�ƫ����
	Quaternion_FromEulerAngle(&Quaternion, &EstEulerAngle);
	Quaternion_ToEulerAngle(&Quaternion, &EulerAngle);
	Quaternion_ToAxisAngle(&Quaternion, &AxisAngle);
}


void Attitude_UpdateGyro(void)  //���ٸ���
{

	QuaternionTypedef   g1, tmp;
	EulerAngleTypedef   LastEulerAngle;
	QuaternionTypedef   LastQuanternion;

	//������һ�ε�ŷ���Ǻ���Ԫ��
	memcpy(&LastEulerAngle, &EulerAngle, sizeof(EulerAngleTypedef));
	memcpy(&LastQuanternion, &Quaternion, sizeof(QuaternionTypedef));

	//������̬����
	float gx = XG / 180 * PI;
	float gy = YG / 180 * PI;
	float gz = ZG / 180 * PI;

	Quaternion_UpdateFromGyro(&Quaternion, gx, gy, gz, PERIODS);
	Quaternion_ToEulerAngle(&Quaternion, &EulerAngle);
	Quaternion_ToAxisAngle(&Quaternion, &AxisAngle);

	// ����ŷ�����ٶ�
	// YawΪƫ�����ٶ�,Ϊ��NED�е�D��(Z��)��ת�Ľ��ٶ�,ʹ����Ԫ������

	g1.W = 0; g1.X = gx; g1.Y = gy; g1.Z = gz;

	Quaternion_Invert(&LastQuanternion, &LastQuanternion);
	Quaternion_Multi(&tmp, &LastQuanternion, &g1);
	Quaternion_Invert(&LastQuanternion, &LastQuanternion);
	Quaternion_Multi(&g1, &tmp, &LastQuanternion);

	EulerAngleRate.Yaw = g1.Z;
	//PitchΪ�������ٶ�, Ϊ��Y����ת�Ľ��ٶ�, ʹ��???����
	if (fabs(LastEulerAngle.Pitch - EulerAngle.Pitch) < PI / 2)
	{
		EulerAngleRate.Pitch = EulerAngle.Pitch - LastEulerAngle.Pitch;
	}
	else if (EulerAngle.Pitch - LastEulerAngle.Pitch > PI / 2)
	{
		EulerAngleRate.Pitch = -PI + (EulerAngle.Pitch - LastEulerAngle.Pitch);
	}
	else if (EulerAngle.Pitch - LastEulerAngle.Pitch < -PI / 2)
	{
		EulerAngleRate.Pitch = PI + (EulerAngle.Pitch - LastEulerAngle.Pitch);
	}

	EulerAngleRate.Pitch /= PERIODS;
	//RollΪ������ٶ�, ��X''����ת�Ľ��ٶ�, ֱ��ʹ������������
	EulerAngleRate.Roll = gx;
}
int32 PID_Position_angle(pid_s *pid,float *coef,int32 result,int32 point)
{
       int32 realize;
       
       int32 error;

       error=point-result;

       //pid->errorsum+=error*coef[Ki];

       //pid->errorsum=XIANFU(pid->errorsum,-coef[Kt],coef[Kt]);

       realize=(int32)(error*coef[0]-SystemAttitudeRate.Pitch*100*coef[1]);//û��I�Ϊ��UI�������

       pid->lasterror=error;

       return realize;

}



/*-----------------------------------------------------------------------------------------
* PID_Position
* λ��ʽPID
---------------------------------------------------------------------------------------------*/

int32 PID_Position(pid_s *pid,float *coef,int32 result,int32 point)
{
       int32 error,realize;

       error=point-result;

       pid->errorsum+=error*coef[Ki];

       pid->errorsum=XIANFU(pid->errorsum,-coef[Kt],coef[Kt]);

     //  realize=error*coef[Kp]+(error-pid->lasterror)*coef[Kd]+pid->errorsum;
        realize=error*coef[Kp]+(error-pid->lasterror)*coef[Kd];
       pid->lasterror=error;

       return realize;

}

int32 PID_Position_v(pid_s *pid,float *coef,int32 result,int32 point)
{
       int32 error,realize;

       error=point-result;

       pid->errorsum+=error*coef[Ki];

       pid->errorsum=XIANFU(pid->errorsum,-coef[Kt],coef[Kt]);

       realize=error*coef[Kp];

       pid->lasterror=error;

       return realize;

}
/*-----------------------------------------------------------------------------------------
* PID_Increase
* ����ʽPID
---------------------------------------------------------------------------------------------*/

int32 PID_Increase(pid_s *pid,float *coef,int32 result,int32 point)
{
     int32 error,increase;

     error=point-result;

     increase=coef[Kp]*(error-pid->lasterror)+coef[Ki]*error+coef[Kd]*(error-2*pid->lasterror+pid->preerror);

     pid->preerror=pid->lasterror;

     pid->lasterror=error;

     return increase;

}
  //ת���ڻ����
  //static uint16 Count = 0;
void GyroOffset_init(void)      /////////��������Ʈ��ʼ��
{
 static uint16 Count = 0;
  if(Count == 0)
  {
    GyroOffset.Xdata = 0;
    GyroOffset.Ydata = 0;
    GyroOffset.Zdata = 0;
    /***********
                    1/22
        xdata:3225734349  3226879394
        ydata:3230373249  3229236593   
        zdata:1072416424  1072718414

       *********/
  }
  
  if(Count == 1000)
  {
    GyroOffset.Xdata /= 1000;
    GyroOffset.Ydata /= 1000;
    GyroOffset.Zdata /= 1000;
    IsGyroOffsetReset = 0;
    DataSave();
    Count = 0;
  }
  else
  {
    if(sensor==1)
    BMX055_DataRead(&sensor_data, 0);
    else if(sensor==0)
    FXAS2100_FXOS8700_DataRead(&sensor_data, 0);
    GyroOffset.Xdata += sensor_data.GYROXdata;
    GyroOffset.Ydata += sensor_data.GYROYdata;
    GyroOffset.Zdata += sensor_data.GYROZdata;
    Count++;
  }
}


void Angle_get (void)
{
  static _Bool IsAttitudeinit = 0;
   // i=0;
  if(IsGyroOffsetReset)
  {
    GyroOffset_init();             ///////��ƫ���������в�������̬���㣬����֤��̬ģ�龲ֹ
    return;
  }
    if(sensor==1)
    BMX055_DataRead(&sensor_data, 0);
    else if(sensor==0)
    FXAS2100_FXOS8700_DataRead(&sensor_data, 0);
  if(sensor==1)
  {
    sensor_data.GYROXdata = (sensor_data.GYROXdata - GyroOffset.Xdata) * 0.030517578;   
    sensor_data.GYROYdata = (sensor_data.GYROYdata - GyroOffset.Ydata) * 0.030517578;
    sensor_data.GYROZdata = (sensor_data.GYROZdata - GyroOffset.Zdata) * 0.030517578;
      ///////1000 / 32768     //////BMX055������Ʈ�������Ժ��Բ��ƣ����ǰ�ȫ������ǽ���һ��
    sensor_data.ACCXdata *= 0.001953125;    ///////4 / 2048
    sensor_data.ACCYdata *= 0.001953125;
    sensor_data.ACCZdata *= 0.001953125;
  }
  else if(sensor==0)
  {
    sensor_data.GYROXdata = (sensor_data.GYROXdata - GyroOffset.Xdata) * 0.03060163;
    sensor_data.GYROYdata = (sensor_data.GYROYdata - GyroOffset.Ydata) * 0.03060163;
    sensor_data.GYROZdata = (sensor_data.GYROZdata - GyroOffset.Zdata) * 0.03060163;
    sensor_data.ACCXdata *= 0.00048828;
    sensor_data.ACCYdata *= 0.00048828;
    sensor_data.ACCZdata *= 0.00048828;
    sensor_data.ACCZdata = (sensor_data.ACCZdata + 0.95) / (0.95 + 1.05) * 2 - 1;
    sensor_data.ACCYdata = (sensor_data.ACCYdata + 1.04) / (0.96 + 1.04) * 2 - 1;
    sensor_data.ACCXdata = (sensor_data.ACCXdata + 0.97) / (0.97 + 1.03) * 2 - 1;
  }

  
  
  Acc.Xdata = sensor_data.ACCXdata;
  Acc.Ydata = sensor_data.ACCYdata;
  Acc.Zdata = sensor_data.ACCZdata;
  Gyro.Xdata = sensor_data.GYROXdata;
  Gyro.Ydata = sensor_data.GYROYdata;
  Gyro.Zdata = sensor_data.GYROZdata;

  if(IsAttitudeinit == 0)
  {
    Quaternion_init();                    ////��̬�����ʼ��        
    IsAttitudeinit = 1;
  }
  else
  {
    Attitude_UpdateGyro();                /////���ٸ���
    Attitude_UpdateAcc();                 /////����ںϸ���
    
    SystemAttitude.Pitch = -EulerAngle.Roll / PI * 180;            ////תΪ�Ƕ�   ������
    SystemAttitude.Roll = EulerAngle.Pitch / PI * 180;             ////������
    SystemAttitude.Yaw = EulerAngle.Yaw / PI * 180;                ////ƫ����
    SystemAttitudeRate.Pitch = -EulerAngleRate.Roll / PI * 180;   ////�������ٶ�
    SystemAttitudeRate.Yaw = EulerAngleRate.Yaw / PI * 180;       ////ƫ�����ٶ�
    SystemAttitudeRate.Roll=EulerAngleRate.Pitch/PI*180;          ////������ٶ�
        /////Ϊ�˸���׼�Ļ�ø����ǣ��ٴν��л����˲�
    SystemAttitudeRate.Yaw=XIANFU(SystemAttitudeRate.Yaw,-310,310);
    
    float AccZ, AccZAdjust;    
    AccZ = -Acc.Zdata;
    if (AccZ > 1)
	AccZ = 1;
    if (AccZ < -1)
	AccZ = -1;    
    a_angle = asinf(AccZ) * 180 / PI;
    car_angle = SystemAttitude.Pitch;
    AccZAdjust = (a_angle - SystemAttitude.Pitch);//Error
    SystemAttitude.Pitch += (-Gyro.Xdata + AccZAdjust) * PERIODS;//���õ�ǰ�Ƕ�
//    static float Yawoffset=0;
//    Yawoffset+=PERIODS;
//    SystemAttitude.Yaw=EulerAngle.Yaw / PI * 180+Yawoffset;
    

  /*****************************************************************/
  /*���Ƿֽ��ߣ���������Ԫ����̬����*/
  /******************************************************************/
  }
//      car_diroutcore=PID_Position(&wx_pid,wx_coef,SysYaw_last*100,SystemAttitude.Yaw*100);
//      SysYaw_last=SystemAttitudeRate.Yaw;
}