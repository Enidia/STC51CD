

#include "reg52.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;


#define GPIO_KEY P1

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
sbit bee=P3^5;

u8 code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//��ʾ0~F
u8 code signal[6]={0x00,0x21,0x40,0x76,0x0e,0x79};//+ - * /

u8 KeyValue = 20;
u8 cursign;
u8 presign;
long int dispnumb=0;
long int prenumb;
u8 DisplayData[8];
void delay(u16 i)
{
	while(i--);	
}
void KeyDown(void)
{
	char a=0;
	GPIO_KEY=0x0f;
	if(GPIO_KEY!=0x0f)//��ȡ�����Ƿ���
	{
		delay(1000);//��ʱ10ms��������
		if(GPIO_KEY!=0x0f)//�ٴμ������Ƿ���
		{	
			//������
			GPIO_KEY=0X0F;
			switch(GPIO_KEY)
			{
				case(0X07):	KeyValue=0;break;
				case(0X0b):	KeyValue=1;break;
				case(0X0d): KeyValue=2;break;
				case(0X0e):	KeyValue=3;break;
			}
			//������
			GPIO_KEY=0XF0;
			switch(GPIO_KEY)
			{
				case(0X70):	KeyValue=KeyValue;break;
				case(0Xb0):	KeyValue=KeyValue+4;break;
				case(0Xd0): KeyValue=KeyValue+8;break;
				case(0Xe0):	KeyValue=KeyValue+12;break;
			}
			while((a<50)&&(GPIO_KEY!=0xf0))	 //��ⰴ�����ּ��
			{
				delay(1000);
				a++;
			}
		}
	}
}

void DigDisplay()
{
	u8 i;
	for(i=0;i<8;i++)
	{
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;//��ʾ��0λ
			case(1):
				LSA=1;LSB=0;LSC=0; break;//��ʾ��1λ
			case(2):
				LSA=0;LSB=1;LSC=0; break;//��ʾ��2λ
			case(3):
				LSA=1;LSB=1;LSC=0; break;//��ʾ��3λ
			case(4):
				LSA=0;LSB=0;LSC=1; break;//��ʾ��4λ
			case(5):
				LSA=1;LSB=0;LSC=1; break;//��ʾ��5λ
			case(6):
				LSA=0;LSB=1;LSC=1; break;//��ʾ��6λ
			case(7):
				LSA=1;LSB=1;LSC=1; break;//��ʾ��7λ	
		}
		P0=DisplayData[i];//���Ͷ���
		delay(100); //���һ��ʱ��ɨ��	
		P0=0x00;//����
	}
}

void equel()
{
	switch(cursign)
	{
	case 1:
		dispnumb=prenumb+dispnumb;
		break;
	case 2:	
		dispnumb=prenumb-dispnumb;
		break;
	case 3:
		dispnumb=prenumb*dispnumb;
		break;
	case 4:
		if(dispnumb==0){
			cursign=5;
			return;
		}
		else dispnumb=prenumb/dispnumb;
		break;
	}
	cursign=0;
}

void datedisplay()
{
	DisplayData[7]=smgduan[dispnumb%10];
	DisplayData[6]=smgduan[dispnumb/10%10];
	DisplayData[5]=smgduan[dispnumb/100%10];	
	DisplayData[4]=smgduan[dispnumb/1000%10];
	DisplayData[3]=smgduan[dispnumb/10000%10];
	DisplayData[2]=smgduan[dispnumb/100000%10];
	DisplayData[1]=smgduan[dispnumb/1000000%10];
	DisplayData[0]=signal[cursign];
}


void convertkey()
{
	u8 numb=20;
	switch(KeyValue)
	{
	case 0:
		numb = 7;break;
	case 1:
		numb = 8;break;
	case 2:
		numb = 9;break;		
	case 3:
		cursign = 4;
		prenumb = dispnumb;
		dispnumb = 0;break;	//  /	
	case 4:
		numb = 4;break;		
	case 5:
		numb = 5;break;		
	case 6:
		numb = 6;break;		
	case 7:
		cursign = 3;
		prenumb = dispnumb;
		dispnumb = 0;break;   //  *
	case 8:
		numb = 1;break;
	case 9:
		numb = 2;break;
	case 10:
		numb = 3;break;
	case 11:
		cursign = 2;
		prenumb = dispnumb;
		dispnumb = 0;break;    // -
	case 12:
		numb = 0;break;
	case 13:
		dispnumb=dispnumb/10;break;//   tuige
	case 14:
		equel();
		break;//         denyu
	case 15:
		cursign = 1;
		prenumb = dispnumb;
		dispnumb = 0;break;   // +
	case 20:
		return;
	}
	if(numb != 20&&(numb != 0||dispnumb != 0))
	{
		dispnumb=dispnumb*10+numb;
		if(dispnumb/10000000>0)
			dispnumb = dispnumb/10;
	}
	KeyValue = 20;
}

void main()
{	bee=0;
	while(1)
	{
		KeyDown();
		convertkey();
		datedisplay();	
		DigDisplay();	
	}		
}