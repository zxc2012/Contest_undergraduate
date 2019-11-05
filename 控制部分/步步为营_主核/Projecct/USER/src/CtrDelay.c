#include "headfile.h"

#define DelayPort_Down D13 //�̵����˿�
#define DelayPort_FRONG D12 //�̵����˿�
#define DelayPort_Left D14 //�̵����˿�

Relay_t RelayID;

/*
*��ʼ�����Ƽ̵����Ķ˿�
*/
void InitRelay()
{
    gpio_init (DelayPort_Down, GPO,0);    //��ʼ�� �ܽ�Ϊ���
//    gpio_init (DelayPort_FRONG, GPO,0);    //��ʼ�� �ܽ�Ϊ���
//    gpio_init (DelayPort_Left, GPO,0);    //��ʼ�� �ܽ�Ϊ���
    
    CtrRelay(RelayIdDown,Relay_No);
//    CtrRelay(RelayIdFrong,Relay_No);
//    CtrRelay(RelayIdLeft,Relay_No);
}

/*
*ctrSign��1�����̵���
*		  0�رռ̵���
*/
void CtrRelay(Relay_t relay_id,uint8 ctrSign)
{
    if(relay_id == RelayIdDown)
        gpio_set (DelayPort_Down, ctrSign);    //  �ܽ� ��� 1
    else if(relay_id == RelayIdFrong)
        gpio_set (DelayPort_FRONG, ctrSign);  
    else if(relay_id == RelayIdLeft)
        gpio_set (DelayPort_Left, ctrSign);  
}

