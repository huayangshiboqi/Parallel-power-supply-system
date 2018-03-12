#include "msp430.h"
#include"OLED.H"
#include"math.h"
#include"library.h"
unsigned int key=0;
 unsigned int temp;
 int flagg=0;
void keyscan()
{
	flagg=0;
     P2DIR=0xf0;
     P2REN=0xff;
     P2OUT=0x0f;
     temp=P2IN;
     temp=temp&0x0f;
     while(temp==0x0f)
     {
		 temp=P2IN;
		 temp=temp&0x0f;
     }
     if(temp!=0x0f)
     {
    	 _delay_cycles(3);//消抖
         temp=P2IN;
         temp=temp&0x0f;
         if(temp!=0x0f)//再次判断是否按下
         {
        	 flagg=1;
//             temp=P2IN;
             switch(temp)
             {
                 case 0x07:key=0;break;
                 case 0x0b:key=1;break;
                 case 0x0d:key=2;break;
                 case 0x0e:key=3;break;
                 default:break;
             }
         }
     }

     P2DIR=0x0f;
     P2REN=0xff;
     P2OUT=0xf0;
     temp=P2IN;
     temp=temp&0xf0;
     if(temp!=0xf0)
     {
    	 _delay_cycles(3);
         temp=P2IN;
         temp=temp&0xf0;
         if(temp!=0xf0)
         {
//             temp=P2IN;
             switch(temp)
             {
                 case 0x70:key=key;break;
                 case 0xb0:key=key+4;break;
                 case 0xd0:key=key+8;break;
                 case 0xe0:key=key+12;break;
                 default:break;
             }
         }
     }
     while((P2IN&0xf0)!=0xf0);//判断是否松手
}
float juzhengjianpan( void )
{
	int b[5]={0};
	int i=0,c=0,d=0;float a=0.0;
	unsigned char * bbc[]={xiaoshudian};
	while(1)
    {
        keyscan();
        while(flagg==1)
        {
				b[i]=(int)key;
				switch(key)
				{
					case 0:Picture_shuzizheng(0,6,i+3);break;
					case 1:Picture_shuzizheng(1,6,i+3);break;
					case 2:Picture_shuzizheng(2,6,i+3);break;
					case 3:Picture_shuzizheng(3,6,i+3);break;
					case 4:Picture_shuzizheng(4,6,i+3);break;
					case 5:Picture_shuzizheng(5,6,i+3);break;
					case 6:Picture_shuzizheng(6,6,i+3);break;
					case 7:Picture_shuzizheng(7,6,i+3);break;
					case 8:Picture_shuzizheng(8,6,i+3);break;
					case 9:Picture_shuzizheng(9,6,i+3);break;
					case 12:Picture_hanzi(bbc,1,6,i+3);break;
					case 15:break;
					default:break;
				}
                 i++;

				flagg=0;
        }
        if(i!=0)
            if(b[i-1]==15)
        	    break;           //如果设置完，则退出设置

    }
	for(c=0;c<i;c++)
	{
		if(b[c]==12)
			d=c;                     //找出小数点的位置
	}
	for(c=d-1;(c>0)||(c==0);c--)     //整数部分加和
	{
		a=a+(float)(b[c]*pow(10,d-1-c));
	}
	for(c=i-2;c>d;c--)               //小数部分加和
	{
		a=a+(float)(b[c]*pow(10,d-c));
	}
	Picture_shuzi2(a,6,3);
	_delay_cycles(12);              //这个显示，没加等待时，会出现乱码的情况，加等待后，依据等待时间乱码的情况依次后移
	return a;
}

