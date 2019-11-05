/*
 * @file       Freecars��λ��Э��β�
 * @brief      2017nxp
 * @author     liwei
 * @version    v0.1
 * @date       2016.5.10
 */
#ifndef __Freecars_H_
#define __Freecars_H_

#define UARTPort uart4
#define BAUD 115200
#define UartDataNum      17	//FreeCars��λ�� ����������ʾ�� ����ͨ������������λ�����øı�

#define UartRxBufferLen  100
#define UartRxDataLen    41           //FreeCars��λ�����͸�������MCU���գ���Ҫ��
#define UartRxCmdLen     7	      //FreeCars��λ�������������ݳ��ȣ���Ҫ��

#define UartCmdNum  SerialPortRx.Buffer[SerialPortRx.Stack-3]//�����
#define UartCmdData SerialPortRx.Buffer[SerialPortRx.Stack-2]//��������


//�������ݵĳ���ֻҪ�����鳤��Ϊ26=22+3+1������Ϊ���뷢���ַ���ȡ��ͳһ
//ȡ���ݵĳ������ַ����ĳ�����ȣ������ڷ��������ǻ�෢����һЩ
//��Чλ������Ӱ�첻���
#include "common.h"
#include "MK60_uart.h"
typedef struct
{
  int Stack;
  uint8 Data;
  uint8 PreData;
  uint8 Buffer[UartRxBufferLen];
  uint8 Enable;
  uint8 Check;
}SerialPortType;

extern double UartData[9];


extern void UartDebug(void);
extern void UartCmd(uint8 CmdNum,uint8 Data);
extern void freecars_init(void);
extern void tofreecars(void);
extern void push(uint8 chanel,uint16 data);
extern void freecars_img(uint8 imgData[],uint16 img_H,uint16 img_W);
extern void freecars_push(void);

#endif
