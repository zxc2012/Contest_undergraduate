#include "headfile.h"
//extern uint8 Data_State;/*���ӻ���������״̬*/
uint8 remain_x;
uint8 remain_y;
void Connunication_init(void)   //˫��ͨ��
{
	uart_init (uart2, 115200);
	set_irq_priority(UART2_RX_TX_IRQn,0);	//�ж����ȼ�0						
	uart_rx_irq_en(uart2);
}

/*�������մӻ���������ֵ���������ڴ����ж�����*/    
/*�����Ҫ���ӽ��յ����ݣ��޸Ģ٢ڢۢܢݢ޵ط�*/
void UART2_RX_ISR(void)              
{
	static uint8 Temp[8]={0};  //�������ݱ���												//��  ����Ҫ�������ݣ���Temp�����С
	static uint8 state = 0;/* ����״̬ */
	static uint8 _data_len = 6,_data_cnt = 0;                       //��  ����Ҫ�������ݣ���_data_len�Ĵ�С
	uint8 byteReceived;
	uart_getchar(uart2,&byteReceived);
	if(byteReceived == 0xfe && state==0)                              //��־λ��ӻ���Ӧ
	{
		state = 1;
		Temp[0] = byteReceived;
	}			
	else if(byteReceived == 0xfa && state==1)													 //��־λ��ӻ���Ӧ
	{
		state=2;
		Temp[1] = byteReceived;
	}
	else if(state == 2&&_data_len>0)
	{
		_data_len--;
		Temp[2+_data_cnt++] = byteReceived;
		if(_data_len==0)
			state = 3;
	}
	else if(state == 3)
	{
		if(byteReceived == 0xff)  																			 //��־λ��ӻ���Ӧ
		{
			//������
			MotorStatus_t.LB_Spd = ((int)(Temp[2]<<8)&0xff00)|Temp[3];         //�� ����Ҫ�������ݣ��Ĵ�����������
			MotorStatus_t.RB_Spd = ((int)(Temp[4]<<8)&0xff00)|Temp[5];	
			remain_x = Temp[6];
			remain_y = Temp[7];
                        
			if(MotorStatus_t.LB_Spd >30000)     //���������������ܳ���30000
				MotorStatus_t.LB_Spd = MotorStatus_t.LB_Spd-65535;	
			if(MotorStatus_t.RB_Spd >30000)
				MotorStatus_t.RB_Spd = MotorStatus_t.RB_Spd-65535;
			
			state = 0;
			_data_len = 6;                                                 //������Ҫ�������ݣ���_data_len�Ĵ�С
			_data_cnt = 0;
                        
                        Speed_Value_LB+=MotorStatus_t.LB_Spd;
                        Speed_Value_RB+=MotorStatus_t.RB_Spd;
			return;
		}
		else    //������������ͷ����
		{
			_data_cnt = 0;
			_data_len = 6;																							  //������Ҫ�������ݣ���_data_len�Ĵ�С
			state = 0;
			return;
		}

	}
	else
	{
		state = 0;
		_data_len = 6;                                                  //������Ҫ�������ݣ���_data_len�Ĵ�С
		_data_cnt = 0;
	}
}


/*
*  �������ݸ����ˣ�ִ�ж�Ӧ����
*  1�������������  2������һ���ٶ�
*/
void Communication_send(uint8 num)
{
    uint8 data_to_send[4];                             //�����������ݣ������СҪ�޸�
    data_to_send[0] = 0XFE;
	  data_to_send[1] = 0XFA;
		data_to_send[2] = num;     //�����������ݣ���������Ҫ�޸�	
                      data_to_send[3] = 0xff;	
   uart_putbuff(uart2, (uint8_t *)data_to_send, 4);    //�����������ݣ������ֽ���Ҫ�޸�
}

