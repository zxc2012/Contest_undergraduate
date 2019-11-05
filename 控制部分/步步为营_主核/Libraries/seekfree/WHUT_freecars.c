/*
 * @file       Freecars��λ��Э��β�
 * @brief      whut2
 * @author     whut_ycy
 * @version    v2.0
 * @date       2019.1.14
 * @readme     �����NRF����Э��,��Ҫ������ɿ�
 */
#include "headfile.h"



/************�����Ǹ���FreeCars2.0Э�������ģ��û�����ı�**************/
uint8 uSendBuf[UartDataNum*2]={0};
uint8 FreeCarsDataNum=UartDataNum*2;

double UartData[9] = {0};
SerialPortType SerialPortRx;
extern uint32 pulse;
extern int WangToNum;
/*
��λ�����ز������ô˺�����ֵ����Ҫ�ı����� ��Kp
*/
void UartDebug(void)
{
//                   SysPID[Head_Angle].p=UartData[0];
//                   SysPID[Head_Angle].d=UartData[1];
//                Y_Speed=UartData[2];
//                X_Speed=UartData[3];
//                PID_K[2]=UartData[2];

//                SysPID[position].p = UartData[0];		//����P
//                SysPID[position].d = UartData[1];		//����D
                CarEnterOk= UartData[2];
//                WangToNum=UartData[3];

}

void freecars_push(void)
{
//   push(0,middle_Error[0]);
//   push(1,Direction_Control_Out);
//   push(2,kp_out);
//   push(3,kd_out);
//   push(4,car_v);
//   push(5,Speed_Set);
//   push(0,YH_Crossroad);
//   push(1,you_zhixian);
//   push(2,break_row);
//   push(3,chengxu_flag);
//   push(4,third_point[0]);
//   push(5,third_point[1]);
   tofreecars();
}


/*
������պ���
CmdNum��0~255������
DATA  ��0~255����������Ӧ��������
CmdNum    Data          ��Ӧ��ݼ�
  1         6               F6
  1         7               F7
  1         8               F8
  1         9               F9
  1         10               F10
  1         11               F11
  1         12               F12
  2         100             Pause
  2         101             Home
  2         102             Pg up
  2         103             Pg dn
  2         104             end
*/


void UartCmd(uint8 CmdNum,uint8 Data)
{
  if(CmdNum == 1 && Data == 6)
  {
//       NVIC_DisableIRQ(PIT0_IRQn);
//            ftm_pwm_duty(ftm0,ftm_ch1,5000);              //˫����PWM�������
//            ftm_pwm_duty(ftm0,ftm_ch0,5000);
//
//            ftm_pwm_duty(ftm0,ftm_ch3,5000);
//            ftm_pwm_duty(ftm0,ftm_ch2,5000);
  }
  if(CmdNum == 1 && Data == 7)
  {
    NVIC_EnableIRQ(PIT0_IRQn);
  }
//  if(CmdNum == 1 && Data == 8)
//  {
//    push(1,27);
//  }
//  if(CmdNum == 1 && Data == 9)
//  {
//    push(1,27);
//  }
//  if(CmdNum == 1 && Data == 10)
//  {
//    push(1,27);
//  }
//  if(CmdNum == 1 && Data == 11)
//  {
//    push(1,27);
//  }
}

/*
��ѯ������һ֡����
����ʱ�������ݳ����й�
�����Է����ж����������Ե���
����ʱ����㿴�����ĵ�
*/
void tofreecars(void)
{
  uint8 i,sum=0;
  //ʹ����ѯ�ķ�ʽ�������ݣ�������δ���ͣ�����ͣ�ڴ˴�ֱ���������
  uart_putchar(UARTPort,251);
  uart_putchar(UARTPort,109);
  uart_putchar(UARTPort,37);
  sum+=(251);      //ȫ�����ݼ���У��
  sum+=(109);
  sum+=(37);
  for(i=0;i<FreeCarsDataNum;i++)
  {
    uart_putchar(UARTPort,uSendBuf[i]);
    sum+=uSendBuf[i];         //ȫ�����ݼ���У��
  }
  uart_putchar(UARTPort,sum);
}
/*******nrf���ͺ���******/
//void tonrf(void)
//{
//  uint8 i,sum=0;
//  //ʹ����ѯ�ķ�ʽ�������ݣ�������δ���ͣ�����ͣ�ڴ˴�ֱ���������
////  UART_Put_Char(FreeCarsUARTPort,251);
////  UART_Put_Char(FreeCarsUARTPort,109);
////  UART_Put_Char(FreeCarsUARTPort,37);
//  NRF_TX_Buff[0] = 251;
//  NRF_TX_Buff[1] = 109;
//  NRF_TX_Buff[2] = 37;
//  sum+=(251);      //ȫ�����ݼ���У��
//  sum+=(109);
//  sum+=(37);
//  for(i=0;i<FreeCarsDataNum;i++)
//  {
//  //  UART_Put_Char(FreeCarsUARTPort,uSendBuf[i]);
//    NRF_TX_Buff[i+3]=uSendBuf[i];
//    sum+=uSendBuf[i];         //ȫ�����ݼ���У��
//  }
// // UART_Put_Char(FreeCarsUARTPort,sum);
//  NRF_Send_Packet(NRF_TX_Buff);
//  systick_delay_ms(2);
//}
//����һ��ͼ��Freecars��λ����   80*60
void freecars_img(uint8 imgData[],uint16 img_H,uint16 img_W)
{
  int16 i,j;
  uint16 offset;
  uart_putchar(UARTPort,0xFF);//FF,FE,FF,FE�ĸ�����ʾһ��ͼ��Ŀ�ʼ
  uart_putchar(UARTPort,0xFE);
  uart_putchar(UARTPort,0xFF);
  uart_putchar(UARTPort,0xFE);

//  for(i = 0;i < img_H;i++)
//  {
//    offset = i*img_W;
//    for(j = 0;j < CAMERA_W;j++)

  for(i = img_H - 1;i >=0;i--)
  {
    offset = i*img_W;
    for(j = img_W - 1;j >= 0;j--)
    {
      uint8 d = imgData[offset + j];
      if(d > 0xFD) d = 0xFD;            //�ܿ�У��λ
      uart_putchar(UARTPort,d);
    }
    uart_putchar(UARTPort,0xFE);//FE,FE 2������ʾ����
    uart_putchar(UARTPort,0xFE);
  }
  j=0;
}
/*
��ĳ��ͨ���������
adr��ͨ��
date������-32768~32767(2^8-1)
*/
void push(unsigned char adr,uint16 data)
{
  uSendBuf[adr*2]=data/256;
  uSendBuf[adr*2+1]=data%256;
}
//#if define longqiu
//void UART4_IRQHandler(void)
//#if define zhufei
void UART4_RX_TX_IRQHandler(void)

{
  int i,b,d1;
  uint32 d;

  //SerialPortRx.Data=uart_getchar(UARTPort);
  uart_getchar(UARTPort,&SerialPortRx.Data);


  if( SerialPortRx.Stack < UartRxBufferLen )
  {
    SerialPortRx.Buffer[SerialPortRx.Stack++] = SerialPortRx.Data;

    if(   SerialPortRx.Stack >= UartRxDataLen  //UartRxDataLen����Ϊһ֡
       && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen]  ==0xff //У����ͷ
         && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+1]==0x55
           && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+2]==0xaa
             && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+3]==0x10 )
    {   //double data 9��ͨ������У��
      SerialPortRx.Check = 0;
      b = SerialPortRx.Stack - UartRxDataLen; //��ʼλ
      for(i=b; i<SerialPortRx.Stack-1; i++)  //��У��λ���λ����У��
      {
        SerialPortRx.Check += SerialPortRx.Buffer[i];//У��
      }

      if( SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack-1] )
      {   //У��ɹ����������ݽ���
        for(i = 0; i<9; i++)
        {
          d = SerialPortRx.Buffer[b+i*4+4]*0x1000000L + SerialPortRx.Buffer[b+i*4+5]*0x10000L + SerialPortRx.Buffer[b+i*4+6]*0x100L + SerialPortRx.Buffer[b+i*4+7];
          if(d>0x7FFFFFFF)
          {
            d1 = 0x7FFFFFFF - d;
          }
          else
          {
            d1 = d;
          }
          UartData[i]=d1;
          UartData[i]/=65536.0;
        }
        UartDebug();  //תȥ�������ܵ������ݸ�������
      }
      SerialPortRx.Stack = 0;
    }
    else if(   SerialPortRx.Stack >= UartRxCmdLen //UartRxDataLen����Ϊһ֡
            && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen]  ==0xff
              && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen+1]==0x55
                && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen+2]==0xaa
                  && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen+3]==0x77 )//cmd
    {
      SerialPortRx.Check = 0;
      b = SerialPortRx.Stack - UartRxCmdLen; //��ʼλ
      for(i=b; i<SerialPortRx.Stack-1; i++)  //��У��λ���λ����У��
      {
        SerialPortRx.Check += SerialPortRx.Buffer[i];//У��
      }
      if( SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack-1] )
      {   //У��ɹ�
        UartCmd(UartCmdNum,UartCmdData);//������յ����������MCU�������
      }
      SerialPortRx.Stack = 0;
    }
  }
  else
  {
    SerialPortRx.Stack = 0;
  }
}

void freecars_init(void)
{
  uart_init(UARTPort,BAUD*2);//�����ô���
  uart_rx_irq_en(UARTPort);//ʹ�ܽ����ж�
  set_irq_priority(UART4_RX_TX_IRQn,0);
  //set_vector_handler(FreeCarsVECTORn,Freecars_IRQHandler);//�����жϺ���
}
/*!
 *  @brief      ɽ��๦�ܵ���������λ��������ͷ��ʾ����
 *  @param      imgaddr    ͼ����ʼ��ַ
 *  @param      imgsize    ͼ��ռ�ÿռ�Ĵ�С
 *  @since      v5.0
*  Sample usage:
             �����÷��ο�������:
            ��ɽ������ͷ��ӥ����λ�����̺�΢��Ч�� - ���ܳ�������
             http://vcan123.com/forum.php?mod=viewthread&tid=6242&ctid=27
 */
void vcan_sendimg(void *imgaddr, uint32_t imgsize)
{
#define CMD_IMG     1
    uint8 cmdf[2] = {CMD_IMG, ~CMD_IMG};    //ɽ����λ�� ʹ�õ�����
    uint8 cmdr[2] = {~CMD_IMG, CMD_IMG};    //ɽ����λ�� ʹ�õ�����

    uart_putbuff(UARTPort, cmdf, sizeof(cmdf));    //�ȷ�������

    uart_putbuff(UARTPort, (uint8 *)imgaddr, imgsize); //�ٷ���ͼ��

    uart_putbuff(UARTPort, cmdr, sizeof(cmdr));    //�ȷ�������
}


/////����ͼ������
////�����Ʒ��͵���һλͼ���ǰ�λͼ��ͼ���ʽ����������λ��������
void SendPicData(UARTn uartn, uint8 * data, uint16 size)
{   uint8 SendHead[4] = {0xFF, 0xFA, 0xFD, 0xF6};
  uint8 psize[2] = {0};
  if(size >= 256)
    psize[0] = size / 256;
  psize[1] = size - 256 * (uint16)psize[0];

  uart_putbuff(uartn, SendHead, 4);
  uart_putchar(uartn, 250);
  uart_putbuff(uartn, psize, sizeof(psize));
  uart_putbuff(uartn, data, size);
  uart_putchar(uartn, 0x0A);  ////���һλУ��
}
