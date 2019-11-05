#include "headfile.h"

extern void Num_To_PidSetValue(uint8_t num,float *pid_set_x,float *pid_set_y);
extern int input_final[3];
extern uint8 speed_mode;
loop_t loop;
car_serch_action car_move_state;
void Duty_2ms()
{

}

void Duty_5ms()
{
}

void Duty_10ms()
{
}

void Duty_20ms()
{

//    key_IRQHandler();//�������
//    LCDMenu_Refresh();//OLEDˢ��
}

void Duty_50ms()
{
 
}

void Duty_1000ms()
{ 
//  powerVal=GetPowerAdc();
}
void Loop_check()  //TIME INTTERRUPT
{
	//loop.time++; //uint16_t
	loop.cnt_2ms++;
	loop.cnt_5ms++;
	loop.cnt_10ms++;
	loop.cnt_20ms++;
	loop.cnt_50ms++;
	loop.cnt_1000ms++;

}

void Duty_Loop()   					//�����������Ϊ1ms���ܵĴ���ִ��ʱ����ҪС��1ms��
{
	if(1)
	{		
		if( loop.cnt_2ms >= 2 )
		{
			loop.cnt_2ms = 0;
			Duty_2ms();						//����2ms������ 
		}
		if( loop.cnt_5ms >= 5 )
		{
			loop.cnt_5ms = 0;
			Duty_5ms();						//����5ms������ 
		}
		if( loop.cnt_10ms >= 10 )
		{
			loop.cnt_10ms = 0;
			Duty_10ms();					//����10ms������
		}
		if( loop.cnt_20ms >= 20 )
		{
			loop.cnt_20ms = 0;
			Duty_20ms();					//����20ms������
		}
		if( loop.cnt_50ms >= 50 )
		{
			loop.cnt_50ms = 0;
			Duty_50ms();					//����50ms������
		}
		if( loop.cnt_1000ms >= 1000 )
		{
			loop.cnt_1000ms = 0;
			Duty_1000ms();					//����50ms������
		}
	}
}





//void Read_Yaw(void)
//{
//    MPU6050_Data_Prepare();
//    if((mpu6050.Gyro.z>=0.5 || mpu6050.Gyro.z<-0.5 )&& mpu6050.Gyro_CALIBRATE == 1)
//        Yaww += (mpu6050.Gyro.z*0.05);
//}


extern uint8_t first_flag ;
extern xy_t S_s[23];
extern uint8_t test_form ,test_to ;
void test_select()
{
    uint8_t i = 0;
    uint8_t sta_i = 0;
    int temp[30];
    
    for(i=0;i<8;i++)
         input_q[i] = 100;
    if(test_to == 8)
    {
       temp[0] = 1;temp[1] = 57;temp[2] = 1;temp[3] = 2;temp[4] = 58;temp[5] = 2;
       temp[6] = 3;temp[7] = 59;temp[8] = 3;temp[9] = 4;temp[10] = 60;temp[11] = 4;
       temp[12] = 5;temp[13] = 61;temp[14] = 5;temp[15] = 6;temp[16] = 62;temp[17] = 6;
       temp[18] = 7;temp[19] = 63;temp[20] = 7;temp[21] = 8;temp[22] = 64;temp[23] = 8;

           for(i=((test_form-1)*3);i<24;i++)
                ConversionToXYone(temp[i],&S_s[i-((test_form-1)*3)]);
           
           optimal_n = -4;//8-12
           Walk_Ctrl.start_point = test_form;

    }
    if(test_to == 57)
    {
       temp[0] = 1;temp[1] = 8;temp[2] = 1;temp[3] = 9;temp[4] = 16;temp[5] = 9;
       temp[6] = 17;temp[7] = 24;temp[8] = 17;temp[9] = 25;temp[10] = 32;temp[11] = 25;
       temp[12] = 33;temp[13] = 40;temp[14] = 33;temp[15] = 41;temp[16] = 48;temp[17] = 41;
       temp[18] = 49;temp[19] = 56;temp[20] = 49;temp[21] = 57;temp[22] = 64;temp[23] = 57;
       
       sta_i = (test_form/8)*3;
       for(i=sta_i;i<24;i++)
            ConversionToXYone(temp[i],&S_s[i-sta_i]);
           
       optimal_n = -4;//8-12
       Walk_Ctrl.start_point = test_form;
    }
}
extern int WangToNum;
void Select_MoveDir(void)
{
    static uint16_t delay_cnt=0;
    if(WangToNum<=8)
    {
        SysPID[X_Dir].EnPid_Out = No;
        SysPID[Y_Dir].EnPid_Out = No;
        
        SysPID[X_Dir].Out_Int16 = 0;
        SysPID[Y_Dir].Out_Int16 = (int16_t)100;
        En_Update_PID = Yes;
         
    }
    else if(WangToNum>=57)
    {
        SysPID[X_Dir].EnPid_Out = No;
        SysPID[Y_Dir].EnPid_Out = No;
        
        SysPID[X_Dir].Out_Int16 = (int16_t)0;
        SysPID[Y_Dir].Out_Int16 = -(int16_t)100;
        En_Update_PID = Yes;
      //  systick_delay_ms(900); 
    }
    else if(WangToNum==16||WangToNum==24||WangToNum==32||
            WangToNum==40||WangToNum==48||(WangToNum==56&& SwitchMode>1) )
    {
        SysPID[X_Dir].EnPid_Out = No;
        SysPID[Y_Dir].EnPid_Out = No;
        
        SysPID[X_Dir].Out_Int16 = -(int16_t)100 * 1;
        SysPID[Y_Dir].Out_Int16 = (int16_t)0;
        En_Update_PID = Yes;
      //  systick_delay_ms(900); 
    }
    else if(WangToNum==9||WangToNum==17||WangToNum==25||
            WangToNum==33||WangToNum==41|| (WangToNum==49&& SwitchMode>1) )
    {
        SysPID[X_Dir].EnPid_Out = No;
        SysPID[Y_Dir].EnPid_Out = No;
        
        SysPID[X_Dir].Out_Int16 = (int16_t)100 * 1;
        SysPID[Y_Dir].Out_Int16 = (int16_t)0;
        En_Update_PID = Yes;
     //   systick_delay_ms(900); 
    }
    else if(SwitchMode<=1 && (WangToNum==49||WangToNum==50||WangToNum==51||
             WangToNum==52||WangToNum==53||WangToNum==54||WangToNum==55||WangToNum==56))
    {
        SysPID[X_Dir].EnPid_Out = No;
        SysPID[Y_Dir].EnPid_Out = No;
        
        SysPID[X_Dir].Out_Int16 = (int16_t)0;
        SysPID[Y_Dir].Out_Int16 = -(int16_t)100;
        En_Update_PID = Yes;
    }
    else if(SwitchMode<=1 && (WangToNum==15||WangToNum==23||WangToNum==31||
             WangToNum==39||WangToNum==47) )
    {
        SysPID[X_Dir].EnPid_Out = No;
        SysPID[Y_Dir].EnPid_Out = No;
        
        SysPID[X_Dir].Out_Int16 = -(int16_t)100 * 1;
        SysPID[Y_Dir].Out_Int16 = (int16_t)0;
        En_Update_PID = Yes;
    }

    while(1)
    {
        delay_cnt++;
        if(delay_cnt>=120)
            break;
        if(WangToNum<=8)
        {
            if((Walk_Ctrl.H_B_Flag==1&&Walk_Ctrl.FoundNumber==(WangToNum+8))&&delay_cnt>=60)
            {
//                Walk_Ctrl.pos = NumPos_To_CarPos(1,(WangToNum+8),Walk_Ctrl.num_pos);     //����old_xy  
                break;
            }
            else
                systick_delay_ms(10);              
        }
        else if(WangToNum>=57)
        {
            if((Walk_Ctrl.H_B_Flag==0&&Walk_Ctrl.FoundNumber==(WangToNum)) &&delay_cnt<=5)
            {
//                Walk_Ctrl.pos = NumPos_To_CarPos(0,(WangToNum),Walk_Ctrl.num_pos);     //����old_xy  
                break;
            }
            else
                systick_delay_ms(10);
        }
        else if( (WangToNum>=49&&WangToNum<=55) && SwitchMode <=1)
        {
            if((Walk_Ctrl.H_B_Flag==0&&Walk_Ctrl.FoundNumber==(WangToNum)) &&delay_cnt<=5 )
            {
//                Walk_Ctrl.pos = NumPos_To_CarPos(0,(WangToNum),Walk_Ctrl.num_pos);     //����old_xy  
                break;
            }
            else
                systick_delay_ms(10);
        }
        else
        {
            if((Walk_Ctrl.H_B_Flag==0&&Walk_Ctrl.FoundNumber==(WangToNum-8)) || (Walk_Ctrl.H_B_Flag==1&&Walk_Ctrl.FoundNumber==(WangToNum+8)))
            {
                if(Walk_Ctrl.CameraId == 0)
                {
//                    Walk_Ctrl.pos = NumPos_To_CarPos(0,(WangToNum-8),Walk_Ctrl.num_pos);     //����old_xy  
                }
                else
                {
//                    Walk_Ctrl.pos = NumPos_To_CarPos(1,(WangToNum+8),Walk_Ctrl.num_pos);  
                }
                break;
            }
            else
                systick_delay_ms(10); 
        }
    }
     
    first_flag = 1;         //�򿪴��󱣻�  
    
    SysPID[X_Dir].EnPid_Out = No;
    SysPID[Y_Dir].EnPid_Out = No;
    
}
uint8_t min_n;
extern uint8_t stop_flag;
extern xy_t start_car[28];
extern xy_t start_car_set;

void Cau_EnterPos()
{
    uint8_t j;
    uint16_t min_d = 10000;
    xy_t start_pos;

    if(SwitchMode==15)
    {
        for(j=0;j<28;j++)                  //��������볡λ��
        {
            Find_path(input_q,start_car[j]);  //start_car �������Խ�����28��λ��  input_q ���ӵİ˸�λ��
            if(S_Dis<min_d)
            {
                min_d = S_Dis;
                min_n = j;
            }
        }
//        if(speed_mode == 3)                 //ģʽ3Ϊ��Ϊ�趨�볡λ��
//        {
//            start_car[min_n].x = start_car_set.x;
//            start_car[min_n].y = start_car_set.y;
//            S_Dis = 10000;
//        }
        Find_path(input_q,start_car[min_n]);
    
    }
//    while(En_Update_PID == No)          //����������
//    {
////        Duty_Loop();
//          MOTOR_get();
//          Communication_send(2);
////        Switch_Mode();              //ѡ��ģʽ
////        sendDataToScope();
//        ALLEncoder_clear();//������������ֵ
//    }
    En_Update_PID = No;
    
    
    if(speed_mode != 4)                 //�����ʼλ������
      TransferToNum(start_car[min_n],&WangToNum);  
    now_position.x=9;
    now_position.y=5;
    SendBeforeOrAfter();
  //  first_flag = 1;         //�򿪴��󱣻�    
}



void Enter_FirstPos(void)
{

    Cau_EnterPos();     //�����볡���꣬·��

//    Select_MoveDir();   //���볡

}


extern uint8_t CarEnterOk;
void car_serch (void)
{
  En_Update_PID = No;
  car_move_state=Car_serching;
  now_position.x=-1;
  now_position.y=2;
  TransferToNum(S_s[cal_n_serch++],&WangToNum);
  CarEnterOk = 1;
  En_Update_PID=Yes;
  while(1)
  {}
}
void Car_EnterTo(void)
{
//    if(SwitchMode==15)
//    {
//        while(En_Update_PID == No)   //�������
//        {
//            Duty_Loop(); 
//            MotorStatus_t.LH_Spd = GetDmaConut(LH);
//            MotorStatus_t.RH_Spd = GetDmaConut(RH);
//            MotorStatus_t.LB_Spd = GetDmaConut(LB);
//            MotorStatus_t.RB_Spd = GetDmaConut(RB);
//            Switch_Mode();              //ѡ��ģʽ
//            sendDataToScope();
//            AllDmaValueClear();//����DMAֵ
//        }
//    }
    
    En_Update_PID = No;
    
    Enter_FirstPos();          //�����һ������
    En_Update_PID = No;
    
    TransferToXY_One(WangToNum,&now_position);//�ȸ������ڵ�λ��
    TransferToNum(S_s[++cal_n],&WangToNum);
    
    if(SwitchMode == 1)
        ServoState[userNow] = Servo_Up;
    //ServoState[1] = Servo_Up;

    CarEnterOk = 1; 
    En_Update_PID = Yes;       //
}