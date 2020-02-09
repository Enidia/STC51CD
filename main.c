#include "reg52.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include<intrins.h>		//��ΪҪ�õ������ƺ��������Լ������ͷ�ļ�

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

sbit k1=P3^0;	 //����P30����k1
sbit k2=P3^1;	 //����P31����k2
sbit k3=P3^2;	 //����P32����k3
sbit k4=P3^4;           //����P34����k4

sbit beep=P3^5;	

u8 code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//��ʾ0~F
u8 flag;
u8 mont31[7]={1,3,5,7,8,10,12};

u8 mode,edit,al_state;
u8 ssec,sec,maxday,t_hour,t_min;

u8 DisplayData[8];

u16 year=2017;
u8 month=1;
u8 day=12;
u8 hour=9;
u8 min=10;

void delay(u16 i)
{
	while(i--);	
}

void keypros()
{
	if(k1==0)		  //��ⰴ��K1�Ƿ���
	{	
		delay(1000);   //�������� һ���Լ10ms
		if(k1==0)	 //�ٴ��жϰ����Ƿ���
		{
			mode++;
			edit=0;
			if(mode>2) mode = 0;
		}
		while(!k1);	 //��ⰴ���Ƿ��ɿ�
	}		
}

void editpros()
{
	if(k2==0)
	{	
		delay(1000);   //�������� һ���Լ10ms
		if(k2==0)
		{
			edit++;flag=1;
			if(mode==0&&edit>3) edit = 0;
			if(mode==1&&edit>3) edit = 0;
			if(mode==2&&edit>2) edit = 0;
		}
		while(!k2);	 //��ⰴ���Ƿ��ɿ�
	}		
}

void addpros()
{
	if(k3==0)
	{	
		delay(1000);   //�������� һ���Լ10ms
		if(k3==0)
		{               
			if(mode==0)
				flag=0;
				switch(edit)
				{
				case 0:
					al_state=0;break;
				case 1:
					hour++;
					if(hour>23) 
						hour=0;break;
				case 2:
					min++;
					if(min>60) 
						min=0;break;
				case 3:
					sec++;
					if(sec>60) 
						sec=0;break;
				}
			else if(mode==1)
				flag=0;
				switch(edit)
				{
				case 0:
					al_state=0;break;
				case 1:
					year++;
					if(year>9999) 
						year=1999;break;
				case 2:
					month++;
					if(month>12)
						month=1;
					if(month==2)
						maxday = 28;
					else
					{
						int i=0;
						maxday = 30;
						for(i=0; i<7; i++)
							if(month == mont31[i])
								maxday = 31;
					}
					break;
				case 3:
					day++;
					if(day>=maxday)
						day=1;
					break;
				}
			else if(mode==2)
 				flag=0;
				switch(edit)
				{
				case 1:
					t_hour++;
					if(t_hour>23) 
						t_hour=0;break;
				case 2:
					t_min++;
					if(t_min>60) 
						t_min=0;break;
				}
				
		}
		while(!k3);	 //��ⰴ���Ƿ��ɿ�
	}		
}
void decpros()
{
	if(k4==0)
	{	
		delay(1000);   //�������� һ���Լ10ms
		if(k4==0)
		{
			if(mode==0)
 				flag=0;
				switch(edit)
				{
				case 0:
					al_state=0;break;
				case 1:
					hour--;
					if(hour<0) 
						hour=23;break;
				case 2:
					min--;
					if(min<0) 
						min=59;break;
				case 3:
					sec--;
					if(sec<0) 
						sec=59;break;
				}
			else if(mode==1)
				flag=0;
				switch(edit)
				{
				case 0:
					al_state=0;break;
				case 1:
					year--;
					if(year<0) 
						year=1999;break;
				case 2:
					month--;
					if(month<1)
						month=12;
					if(month==2)
						maxday = 28;
					else
					{
						int i=0;
						maxday = 30;
						for(i=0; i<7; i++)
							if(month == mont31[i])
								maxday = 31;
					}
					break;
				case 3:
					day--;
					if(day>=maxday)
						day=1;
					break;
				}
			else if(mode==2)
				flag=0;
				switch(edit)
				{
				case 1:
					t_hour--;
					if(t_hour<0) 
						t_hour=23;break;
				case 2:
					t_min--;
					if(t_min<0) 
						t_min=59;break;
				}
				
		}
		while(!k3);	 //��ⰴ���Ƿ��ɿ�
	}		
}

void Timer0Init()
{
	TMOD|=0X01;//ѡ��Ϊ��ʱ��0ģʽ��������ʽ1������TR0��������

	TH0=0Xd8;	//����ʱ������ֵ����ʱ10ms
	TL0=0Xf0;	
	ET0=1;//�򿪶�ʱ��0�ж�����
	EA=1;//�����ж�
	TR0=1;//�򿪶�ʱ��			
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

void datapros()
{
          if(edit==3&&flag=1)
            {
               	DisplayData[7]=0x40;
	DisplayData[6]=0x40;
            }
            else
            {
	DisplayData[7]=smgduan[sec%10];
	DisplayData[6]=smgduan[sec/10];
             }
	DisplayData[5]=0x40;
             if(edit==2&&flag=1)	
            {
                DisplayData[4]=0x40;
	DisplayData[3]=0x40;
            }
            else
            {
                DisplayData[4]=smgduan[min%10];
	DisplayData[3]=smgduan[min/10];
            }
	DisplayData[2]=0x40;
            if(edit==1&&flag==1)
	{
	DisplayData[1]=0x40;
	DisplayData[0]=0x40;
	}
	else
	{
	DisplayData[1]=smgduan[hour%10];
	DisplayData[0]=smgduan[hour/10];
	}
}

void datedisplay()
{
 	if(edit==3&&flag=1)
	{
	DisplayData[7]=0x40;
	DisplayData[6]=0x40;
	}
	else
	{
	DisplayData[7]=smgduan[day%10];
	DisplayData[6]=smgduan[day/10];
	}
	 if(edit==2&&flag=1)
	{
	DisplayData[5]=0x40;	
	DisplayData[4]=0x40;
	}
	else
	{
	DisplayData[5]=smgduan[month%10];	
	DisplayData[4]=smgduan[month/10];
	}
	 if(edit==1&&flag=1)
	{
	DisplayData[3]=0x40;
	DisplayData[2]=0x40;
	DisplayData[1]=0x40;
	DisplayData[0]=0x40;
	}
	else
	{
	DisplayData[3]=smgduan[year%10];
	DisplayData[2]=smgduan[year/10%10];
	DisplayData[1]=smgduan[year/100%10];
	DisplayData[0]=smgduan[year/1000];
	}
}

void alarmdisplay()
{
	DisplayData[7]=0x40;	
	 if(edit==2&&flag=1)
	{
	DisplayData[6]=0x40;
	DisplayData[5]=0x40;
	}
	else
	{
	DisplayData[6]=smgduan[t_min%10];
	DisplayData[5]=smgduan[t_min/10];
	}
	DisplayData[4]=0x40;
 	if(edit==1&&flag=1)
	{
	DisplayData[3]=0x40;
	DisplayData[2]=0x40;
	}
	else
	{
	DisplayData[3]=smgduan[t_hour%10];
	DisplayData[2]=smgduan[t_hour/10];
	}
	DisplayData[1]=0x38;
	DisplayData[0]=0x77;
}

void alarm()
{
	if(al_state==1)
	{
		beep=~beep;
		delay(30);
	}
	else beep=0;
}

void main()
{	
	Timer0Init();  //��ʱ��0��ʼ��
	beep=0;
	while(1)
	{
		keypros();
		editpros();
		addpros();
                                decpros();
		alarm();
		if(mode == 0)
			datapros();
		else if(mode == 1)
			datedisplay();
		else if(mode == 2)
			alarmdisplay();
		DigDisplay();
	}		
}


void Timer0() interrupt 1
{
	TH0=0Xd8;	//����ʱ������ֵ����ʱ10ms
	TL0=0Xf0;
	ssec++;
	if(ssec>=100)  //1s
	{
		ssec=0;
		sec++;
		if(sec>=60)
		{
			sec=0;
			min++;
			if(min>=60)
			{
				min=0;
				hour++;
				if(hour>=24)
				{
					hour=0;
					day++;
					if(month==2)
						maxday = 28;
					else
					{
						int i=0;
						maxday = 30;
						for(i=0; i<7; i++)
							if(month == mont31[i])
								maxday = 31;
					}
					if(day>=maxday)
					{
						day=1;
						month++;
						if(month>=12)
						{
							month=1;
							year++;
							if(year>=9999)
							year=0;
					    }
					}
				}
			}
			if(t_hour==hour&&t_min==min)
					al_state=1;
				else al_state=0;
		}	
	}	
}

