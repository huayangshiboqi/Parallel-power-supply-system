#include<msp430.h>
#include"header.h"
//��������
#define CS1 P3OUT|=BIT5
#define CS0 P3OUT&=~BIT5
#define CLK1 P3OUT|=BIT6
#define CLK0 P3OUT&=~BIT6
#define SI1 P3OUT|=BIT7
#define SI0 P3OUT&=~BIT7

#define CS11 P3OUT|=BIT2
#define CS00 P3OUT&=~BIT2
#define CLK11 P3OUT|=BIT3
#define CLK00 P3OUT&=~BIT3
#define SI11 P3OUT|=BIT4
#define SI00 P3OUT&=~BIT4

////////////////////////
//��������
uint i;
//��������
void write_word(uint T)
{
  for(i=0;i<8;i++)
  {
	  CLK0;
      if(T&0x80)
    	  SI1;
      else
          SI0;
      _delay_cycles(1);
   CLK1;
   T=T<<1;
//   _delay_cycles(1);
  }
}
////////////////////////
//����ת��
void yunsuan(uint r)
{
 write_word(r);
}
//����
void gengxin(uint r)
{
     CS1;
   CLK0;
   _delay_cycles(1);
   CS0;
   CLK0;
   _delay_cycles(1);
//   _delay_cycles(100);
   write_word(0x11);
   yunsuan(r);
//   _delay_cycles(100);
   CS1;
}

//�ڶ������ֵ�λ��
void write_word1(uint T)
{
  for(i=0;i<8;i++)
  {
	  CLK00;
      if(T&0x80)
    	  SI11;
      else
          SI00;
      _delay_cycles(1);
   CLK11;
   T=T<<1;
   _delay_cycles(1);
  }
}
////////////////////////
//����ת��
void yunsuan1(uint r)
{
 write_word1(r);
}
//����
void gengxin1(uint r)
{
     CS11;
   CLK00;
   _delay_cycles(1);
   CS00;
   CLK00;
   _delay_cycles(1);
//   _delay_cycles(100);
   write_word1(0x11);
   yunsuan1(r);
//   _delay_cycles(100);
   CS11;
}
/////////////////////
//������
void adjust1(uint R)
 {
       gengxin(R);
//       _delay_cycles(100);
}
void adjust2(uint R)
{
    gengxin1(R);
//    _delay_cycles(100);
}
