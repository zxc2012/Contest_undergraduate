#include "headfile.h"

uint8 flag1=0;
uint8 start_flag=0;

void all_init()
{
   camera_init();
//   IIC_init();         //ģ��IIC�˿ڳ�ʼ��
//   InitMPU6050();      //��ʼ��MPU6050
//   GPIO_init();          //���� ң�س�ʼ��
//   initOLED();
//   FLASH_Init();       //FLASH��ʼ��
//   Moto_Init();          //�����ʼ��
//   Encoder_Init();       //��������ʼ��
//   Connunication_init(); //˫��ͨѶ��ʼ��
   freecars_init();      //��λ����ʼ��

}


void GPIO_init()
{
//
//   //��Ƭ������ʼ��
//   gpio_init (G5,GPI,0);            //K0
//   gpio_init (G6,GPI,0);            //K1
//   gpio_init (G7,GPI,0);            //K2

//      key_init(KEY_MAX);            //���򿪹س�ʼ��
//
    gpio_init (B8, GPI,0);        //ң��
    gpio_init (B9, GPI,0);
    gpio_init (B10, GPI,0);
    gpio_init (B11, GPI,0);

//   gpio_init(A9,GPO,0);    //������


}



//---------------�жϳ�ʼ��------------------//
void ISR_init(void)
{
  
    pit_init_ms(pit0,1);                            //��ʼ��PIT0����ʱʱ��Ϊ�� xms
    set_irq_priority(PIT0_IRQn,2);
    enable_irq(PIT0_IRQn);                            //ʹ��PIT0�ж�
  

}



