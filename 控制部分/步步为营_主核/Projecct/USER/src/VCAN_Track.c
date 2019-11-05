#include "headfile.h"




void Speed_ResolveXY(uint16 speed);

uint8 queen_set_flag = 8;;
float DiagonalDis;
float x_error;
float y_error;
float WALK_SPEED = 20;
float WALK_SPEED_SET = 50;

varBufferClass varBufferClass_t={0};    //��������
uint8 cal_n = 0;                      //�ж��ߵĲ���
uint8 updown_step = 1;                //�����Ӳ���
uint8 updown_flag = 0;                //�����ӱ�־
int WangToNum;                      //���ƶ�ȥ��λ��
Spd_Action_E SpdAct=Car_straight;       //Ĭ��Ϊ����״̬
uint8 CarEnterOk = 0;

uint8 SetOkError = 2;
uint8 send_flag = 1;
uint8 ucn=0;
extern xy_t S_s[23];
extern char str[5];
extern uint8 speed_mode;

void Track_EightQueen()
{

    switch(SpdAct)
    {
        
        case Car_setpoint://����׶�
        {
           SysPID[position].EnPid_Out = Yes;    //��λ�û�pid  
          /***״̬�л�****/
          if(SysPID[position].err_abs<=50)   //�Ƿ񶨵����
          {
                if(car_move_state==Car_serching)
                {
                  if(cal_n_serch==serch_step)
                  {
                      SpdAct = Car_stop;  
                      En_Update_PID = Yes;
                      break;               //�л�״̬��������ǰcase
                  }
                  TransferToXY_One(WangToNum,&now_position);//�ȸ������ڵ�λ��
                  TransferToNum(S_s[cal_n_serch++],&WangToNum);   //���½�Ҫȥ��λ��
                }
                else if(car_move_state==Car_chessing)
                {
                   if(cal_n == (8-optimal_n)*2)//�Ƿ񵽴��յ�
                   {
                      SpdAct = Car_stop;  
                      En_Update_PID = Yes;
                      break;               //�л�״̬��������ǰcase
                   }
                   TransferToXY_One(WangToNum,&now_position);//�ȸ������ڵ�λ��
//                   TransferToNum(S_s[++cal_n],&WangToNum);   //���½�Ҫȥ��λ��
                }
                
                SysPID[LH_Spe].set = 0;
                SysPID[LB_Spe].set = 0;
                SysPID[RH_Spe].set = 0;
                SysPID[RB_Spe].set = 0;
                SysPID[position].EnPid_Out = No;  //��λ�û�pid
              
                Pos_clear();       //��ձ������ۼ�ֵ
                SpdAct=Car_move_q;      //ʰȡ���������
          }
        }break;
        
        case Car_move_q:          //�����������
        {
          if(car_move_state==Car_serching)
          {
            ServoState[0]=Servo_Up;
            En_Update_PID = Yes;//����PID���ƶ�ȥ�¸�λ��
                
            Pos_clear();       //��ձ������ۼ�ֵ
                
            SpdAct = Car_straight; 
          }
          else if(car_move_state==Car_chessing)
          {
//            Judge_Queen_UpDown();  //��ʱ�ص�
            updown_step =2;          //��ʱ���
            if(updown_step == 2)   //�Ƿ����
            {   
                ucn++;
                if(ucn>=100)
                {
                  ucn=0;
                updown_step = 1;
                En_Update_PID = Yes;//����PID���ƶ�ȥ�¸�λ��
                send_flag = 1;
                
                Pos_clear();       //��ձ������ۼ�ֵ
                
                SpdAct = Car_straight;  
                }
            }
          }
                    
        }break; 
        
        case Car_straight://ֱ�߽׶�
        {
            
            SysPID[position].EnPid_Out = No;  //��λ�û�pid
            
            SendBeforeOrAfter();//������ݮ��ʹ��ǰ�������ͷ
            
            Speed_ResolveXY(WALK_SPEED);
            
            /***״̬�л�****/
            if(IsCarFindPoint()==Yes)//�ҵ�����  
            {   
                SpdAct=Car_setpoint;
                WALK_SPEED = MIN_SPEED;
                WALK_SPEED_SET = WALK_SPEED;
                
            }
                          
        }break;  
        
        case Car_stop://���
        {
            CtrRelay(RelayIdDown,Relay_No);
//            CtrRelay(RelayIdFrong,Relay_No);
//            CtrRelay(RelayIdLeft,Relay_No);

//            if(speed_mode!=4)
//                Out_Of_Board();         
        }break; 

    }
}

void Track_PlaceObstacle()    //�����ϰ�
{
    static uint8 cb_step = 0;
    static uint8 cb_cnt = 0;
    
    switch(SpdAct)
    {
        case Car_setpoint://����׶�
        {
           SysPID[X_Dir].EnPid_Out = Yes;
           SysPID[Y_Dir].EnPid_Out = Yes;                    
          /***״̬�л�****/
          if(IsCarSetOK_Obstacle()==Yes)   //�Ƿ񶨵����
          {     
                StopAll();               //ͣ�£�
                En_Update_PID = No;      //��PID       
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
                TransferToNum(S_s[++cal_n],&WangToNum);   //���½�Ҫȥ��λ��
                SpdAct=Car_place_obstacle;        //ʰȡ���������
          }
        }break;
        
        case Car_place_obstacle://���µ���
        {	
            if(cb_step == 0 && ServoState[userNow] == OK)
            {
                cb_step = 1;
            }          
            else if(cb_step == 1 && ServoState[userNow] == OK)
            {
                cb_cnt++;
                if(cb_cnt>=50)
                {
                    ServoState[userNow] = EleIron_OFF;
                    if(cb_cnt>=70)
                    {
                        cb_cnt = 0;
                        cb_step = 2;    
                    }
                }
            }
            else if(cb_step == 2)  //userNow��0 ǰ��ı�
            {                                                                    //         1 ��ߵı�
                ServoState[userNow] = Servo_Up; 

                if(cal_n == (8-optimal_n)*2+1)//�Ƿ񵽴��յ�
                {
                   ServoState[0] = Servo_Mid; 
                   ServoState[1] = Servo_Mid; 
                   SpdAct = Car_stop;   
                   En_Update_PID = Yes;
                   TransferToNum(S_s[--cal_n],&WangToNum);
                   break;               //�л�״̬��������ǰcase
                }
                En_Update_PID = Yes;
                send_flag = 1;
                SpdAct = Car_straight;   
                cb_step = 0;  
            } 
            else if(cb_step == 3 && (ServoState[userNow] == OK || ServoState[userNow] == Servo_Mid) && userNow == 0)
            {
                if(cal_n == (8-optimal_n)*2+1)//�Ƿ񵽴��յ�
                {
                   SpdAct = Car_stop;  
                   En_Update_PID = Yes;
                   TransferToNum(S_s[--cal_n],&WangToNum);
                   break;               //�л�״̬��������ǰcase
                }
                send_flag = 1;
                SpdAct = Car_straight;  
                En_Update_PID = Yes;
                cb_step = 0;
            }
                    
        }break; 
        
        case Car_straight://ֱ�߽׶�
        {
            
            SendBeforeOrAfter();//������ݮ��ʹ��ǰ�������ͷ
           
//            Speed_ResolveXY(WALK_SPEED);
            
            /***״̬�л�****/
            if(IsCarFindPoint()==Yes)//�ҵ�����
            {   
                SpdAct=Car_setpoint;
                WALK_SPEED_SET = MIN_SPEED;
            }
                          
        }break;   
        
        case Car_stop://���
        {
            CtrRelay(RelayIdFrong,Relay_No);
            CtrRelay(RelayIdLeft,Relay_No);
            
            if(speed_mode!=4)
                Out_Of_Board();         
        }break; 

    }
}

void Speed_ResolveXY(uint16 speed)
{
    
    WALK_SPEED_SET = GetSpeedByDistance(SysPID[position].err);         
    
    X_Speed = (int16)((x_error * speed) / DiagonalDis);
    Y_Speed = (int16)((y_error * speed) / DiagonalDis);
}


void SendBeforeOrAfter(void)
{
 //   if(send_flag == 1)
 //   {
        if(WangToNum>=24 )
        {
            Walk_Ctrl.H_B_Flag = 0;  //ǰ��
  //          send_flag = 0;
        }
        else if(WangToNum< 24)  
        {
            Walk_Ctrl.H_B_Flag = 1;  //����
  //          send_flag = 0;
        }
 //   }
}    
 


uint16 max_speed_y,min_speed_y,max_speed_x,min_speed_x;


void Judge_Queen_UpDown(void)
{
    static uint8 down_cnt = 0;
    
    if(updown_flag == 0)     //������
    {
        
        if(updown_step == 1&&ServoState[0]!=Servo_Up&&ServoState[0] != OK)
        {
            ServoState[0]=Servo_Down;
        }
        
        if(ServoState[0] == OK)   //�Ƿ����
        {                 
            updown_step = 2;
            updown_flag = 1;
        }
        
    }
    else                 //������
    { 
        down_cnt++;
        ServoState[0]=EleIron_OFF;
        if(down_cnt>=1)
        {
            down_cnt=0;
            updown_step = 2;
            updown_flag = 0;
        }
        
    }
    
}



/*
*���ܣ��Ƿ��ҵ�ת�۵�
*���أ�YES����NO��
*/
YesNo_t IsCarFindPoint()
{      
    static uint8 flag = 0;;
//    if((SysPID[X_Dir].err_abs<StartPid) && (SysPID[Y_Dir].err_abs <StartPid) && SysPID[Head_Angle].err_abs<10) //�ж����
    if((ABS(POS_set_value-POS_actual_value))/100< StartPid&& SysPID[Head_Angle].err_abs<10) //�ж����
    {	
        flag++;
        if(flag>3)
        {
            flag = 0;
            return Yes;
        }
        else
            return No;
    }    
    else 
    {
        flag = 0;
        return No;
    }
}

/*
*���ܣ��Ƿ񶨵����
*���أ�YES���ǣ�NO����
*/
YesNo_t IsCarSetOK()
{
    static uint8 flag = 0;
    if(SysPID[X_Dir].err_abs<SetOkError &&SysPID[Y_Dir].err_abs <SetOkError && SysPID[Head_Angle].err_abs<SetOkError) //�ж����
    {	
        flag++;
        if(flag>=queen_set_flag)
        {
            flag = 0;
            return Yes;
        }
        else
            return No;
    }    
    else 
    {
        flag = 0;
        return No;
    }
}

YesNo_t IsCarSetOK_Obstacle()
{
    static uint8 flag = 0;;
    if(( (SysPID[X_Dir].err_abs<10 &&SysPID[Y_Dir].err_abs <2&&userNow==0) || 
          (SysPID[X_Dir].err_abs<2 &&SysPID[Y_Dir].err_abs <10&&userNow==1) ) && SysPID[Head_Angle].err_abs<2) //�ж����
    {	
        flag++;
        if(flag>8)
        {
            flag = 0;
            return Yes;
        }
        else
            return No;
    }    
    else 
    {
        flag = 0;
        return No;
    }
}



/**
 * @brief  �������庯�����ɴӵ�ǰֵ�𲽱仯��Ŀ��ֵ ÿ�����ڱ仯һ�� �˺���������ִ��
 *           
 * \code
 *   varBufferClass SpeedBuffer= {.isOKFlag=0,.times=50};//��ʼ���ṹ��
 *   SpeedBuffer.isOKFlag = 0;//��־λ��0 ��ʼ����  �൱�ں���ģ�鸴λ ����Ҫ
 *   variableBuffer(nowSpeed,setSpeed,&Speed,&SpeedBuffer);
 * \endcode
 * @param[in]  inValue     ��ʼֵ
 *             setValue    Ŀ��ֵ
 *             *variable   ����ָ��
 *             *varBuffer  �ṹ��ָ�� 
 * 
 * @retval     1   ���ڲ�����
 *             2   �������  �ȴ�
 */

uint8 variableBuffer(float inValue,float setValue,float *variable,varBufferClass *varBuffer)
{
	if(varBuffer->isOKFlag==0)
	{
		varBuffer->delt = 0;
		varBuffer->count = 0;
		varBuffer->delt = (setValue - inValue)/varBuffer->times;	
		varBuffer->isOKFlag = 1;
	}
	if(varBuffer->isOKFlag==1)
	{
		*variable += varBuffer->delt;
		varBuffer->count++;
		if(varBuffer->count >= varBuffer->times) 
		{
				varBuffer->isOKFlag = 2;
		}
	}
	return varBuffer->isOKFlag;
}

void Out_Of_Board(void)
{
    uint16 num;
    xy_t end_q;
    static uint16 delay_cnt = 0;
    
    if(delay_cnt>300)
    {
        StopAll();
        En_Update_PID = No;
    }
    else
    {
        delay_cnt++;

    }
    
    end_q.y = (WangToNum-1)/8;
    end_q.x = (WangToNum-1)%8;

    num = WangToNum;


    if(end_q.x>=4)                         //����Xһ��Ϊ��
    {
        if(end_q.y>=4)
        {
            if((7-end_q.y)<=(7-end_q.x))  //��Y ��
            {
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
            
                SysPID[X_Dir].Out_Int16 = (int16)0;
                SysPID[Y_Dir].Out_Int16 = (int16)300;
                return;
            }
            else                         //��X ��
            {
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
            
                SysPID[X_Dir].Out_Int16 = (int16)300;
                SysPID[Y_Dir].Out_Int16 = (int16)0;
                return;
            }
        }
        else
        {
            if((end_q.y-0)<=(7-end_q.x))  //��Y ǰ
            {
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
            
                SysPID[X_Dir].Out_Int16 = (int16)0;
                SysPID[Y_Dir].Out_Int16 = (int16)-300;
                return;
            }
            else                         //��X ��
            {
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
            
                SysPID[X_Dir].Out_Int16 = (int16)300;
                SysPID[Y_Dir].Out_Int16 = (int16)0;
                return;
            }
        }
    }
    else                                                //����Xһ��Ϊ��
    {
        if(end_q.y>=4)
        {
            if((7-end_q.y)<=(end_q.x))  //��Y ��
            {
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
            
                SysPID[X_Dir].Out_Int16 = (int16)0;
                SysPID[Y_Dir].Out_Int16 = (int16)300;
                return;
            }
            else                         //��X ��
            {
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
            
                SysPID[X_Dir].Out_Int16 = (int16)-300;
                SysPID[Y_Dir].Out_Int16 = (int16)0;
                return;
            }
        }
        else
        {
            if((end_q.y-0)<=(end_q.x))  //��Y ǰ
            {
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
            
                SysPID[X_Dir].Out_Int16 = (int16)0;
                SysPID[Y_Dir].Out_Int16 = (int16)-300;
                return;
            }
            else                         //��X ��
            {
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
            
                SysPID[X_Dir].Out_Int16 = (int16)-300;
                SysPID[Y_Dir].Out_Int16 = (int16)0;
                return;
            }
        }    
    }

    
}