//电子设计大赛2011级电源题。调节芯片用的是MCP41010，SPI通信
//这个题目电路设计部分，是通过分别改变控制两路的电流，然后将两路电流进行加和，再流过负载，从而测得负载两端的电压。
//问题主要是，在大电流3A以上 ，数据不稳，改进应该从第一步：在稳定8V的判断条件上改进，
//进行8V调节的条件。1.5~3.5A进行比例设置调节之后，电压电流的变动，和调节电阻时电流电压的变动，两者8V调节的区别
#include <msp430f5529.h>
#include"library.h"
/*#include只是将后面的文件，插入到当前的文本中。其实和.h无关。
而你将一个函数写在.h里面当多个文件均#include时，则这个函数对于多个文件，再编译器看来，每个文件都存在完全相同的函数（只要函数定义相同就会有冲突）。而C语言或C++，默认对每个函数是可作为外部函数的。此时，连接器就会混乱。究竟连接哪个文本（源文件）所编译对应的函数。
你可以把函数写在头文件里，但需要增加static 前缀。此时这个函数就会在每个#include该头文件的源文件中存在，由于是static，对于函数是局部的意思，此时，外部就无法调用到该函数，最多会出现”找不到该函数的连接错误“而不会说函数重复存在的问题。*/
/*对于这个项目问题出现在字库library.h定义的显示字符变量上，因为这些变量被多个模块使用，所以字符被加载到多个模块中，但是，最后main.h中融合了这些模块，这些相同名字的字符被加载到一起，
 * 此时就会出现重定义的问题，如果这些字符用static修饰，则即便library.h中的字符被插到其它模块，最后被整合到一起，也不会出现
 * 重定义额现象。因为这些字符只在哪些模块中使用，不被另外加载到主函数或其他模块
 * 同样的问题还有函数的static用法，把函数限制在只能一个模块中使用。不过函数声明多次没问题，虽然不能定义多次*/
//同时这个项目存在函数重入问题，多发生在主程序显示和中断中显示，调用同一个显示函数发生的错误
#include "adjust.h"
#include"OLED.H"
#include"juzhengjianpan.h"

void ADC(void);
//extern float temp1[5],temp2[5],temp3[5];//三路电流检测
//extern unsigned int M;//从ADC来的变量
extern float temp1,temp2,temp3;//三路电流检测

unsigned int R=0xd5;//写的CODE值，给数字电位器
int flag=0;//任意比例设置的标志位
int flag2=0;//不同档位切换后，对1.5~3.5A范围内默认比例的影响
//int flag3=0;//用于电压在调整到8V左右后不再改变，给电路一个信号
float aa=0.0;//按键任意比例设置数据的存放
int flag123[2000]={0};//用于做过流保护
int flag1234=0;//用于调节过程的检测
int flag12345=0;//用于调节过程1.5~3.5之间的检测
int flag23=0;//用于1.5~3.5A之间的电压比例调整
int flag4A=0;//调整4.0A时的电流比例标志
unsigned int bb=0;

//==============主函数====================
void main()
 {
//	unsigned char * ba[]={dang,qian,dian,liu};  //指针型数组，ba[0]里面存的是一个指针型变量
//	int i=0;                                    //i自由变量，r作为code值的增量
	int a=120,b=120;                                     //作为调整电流，暂时存放变量
	float x=0.0,y=0.0,z=0.0,w[2]={0.0},RR=0.0;           //分别为：电压，I1，I2，总电流
	float bili=0.0;                             //用于存放I1：I2的值
	WDTCTL = WDTPW+WDTHOLD;					    // Stop watchdog timer
//	P1DIR|=BIT5;P1DIR |=BIT6;P1DIR|=BIT4;        //数字电位器
    P3DIR|=0x00ff;                              //数字电位器
    P1DIR|=BIT0;P4DIR|=BIT7;                    //两个指示灯
    P1OUT&=~BIT0;P4OUT&=~BIT7;

    ADC();
    OLED();

    P1DIR&=~BIT1;                               //将P1.1脚设置为输入方式
    _enable_interrupts();                       //使能总中断
    P1REN|=BIT1;                                //使能P1.1上拉、下拉寄存器
    P1OUT |=BIT1;                               //将P1.1设置为上拉输入
    P1IES |=BIT1;                               //将P1.1设置下降沿触发中断
    P1IFG&=~BIT1;                               //清除P1.1中断标志位

//    Picture_hanzi(ba,3,4,0);                  //要显示的汉字，显示汉字的个数，显示行数，显示列数
//    Picture_shuzi(1.23,4,4);                  //显示的数字，显示的行数，显示的列数。

	adjust1(R);                                 //给第一支路电路一个初始值。
	adjust2(R);                                 //给第二支路一个初始值
	while(1)
	{
	   x=temp1;                                 //母线电压
	   y=temp2;                                 //第一支路电流
	   z=temp3;                                 //第二支路电流*/
       w[1]=y+z;                                   //母线电流
       x=x*4;
       Picture_shuzi(x,0,3);
       Picture_shuzi(y,2,3);
       Picture_shuzi(z,4,3);

       if((x<7.60)||(x>8.40)||(fabs(w[0]-w[1])>0.08))                   //第一步，调节电压到八幅
       {
    	   if(flag4A==0)
    	   {
    	      RR=x/w[1];
    	      R=(int)(256-256*8/RR/2/2.82);
    	      adjust1(R);
              adjust2(R);
    	   }
    	   if(flag4A==1)     //当flag4A=1时，对应于4A状态时电路code值调整，用加一减一的形式，不用公式，是因为系数问题，跳动较大
    	   {
    			          if(x<7.0)
    			       	   {
    			       			   R=R-(int)(3*(8.0-x));
    			       			   adjust1(R);adjust2(R);
    			       	   }
    			       	   if(x<7.90)
    			       	   {
    			       		   if(R!=0)
    			       		   {
    			   				   R=R-1;
    			   				   adjust1(R);
    			   				   adjust2(R);
    			   				   a=b=R;                             //暂存变量，用于后续电流的调整
    			       	       }
    			       		   else
    			       		   {
    			                      adjust1(R);
    			                      adjust2(R);
    			                      a=b=R;                             //暂存变量，用于后续电流的调整
    			       		   }
    			       	   }

    			       		   if(x>9.0)
    			       		   {
    			       			   R=R+(int)(3*(x-8.0));
    			       			   adjust1(R);adjust2(R);
    			       		   }
    			       		   if(x>8.10)
    			       	       {
    			       		   if(R!=0xff)
    			       		   {
    			   				   R=R+1;
    			   				   adjust1(R);
    			   				   adjust2(R);
    			   				   a=b=R;                             //暂存变量，用于后续电流的调整
    			       	       }
    			       		   else
    			       		   {
    			                      adjust1(R);
    			                      adjust2(R);
    			                      a=b=R;                             //暂存变量，用于后续电流的调整
    			       		   }

    			       	       }

    	   }

    	   a=b=R;
    	   flag1234=0;
    	   flag12345=0;

    	   w[0]=w[1];//更新总电流
       }

       if((x>7.60)&&(x<8.40))                   //在稳定电压下调节电流
       {
    	   flag4A=0;
//    	   w[0]=w[1];                           //更新总电流的值
    	   if(bb!=0)                            //用于过流波爱护>4.5A的调整，在过一次流的情况下才有作用
    	   {
    	        flag123[bb]=0;
    	        bb++;
    	        if(bb==2000)
    		        bb=0;
    	   }
		   if((bb!=0)&&(flag123[bb-2]==flag123[bb-1]))   //如果等待一次延迟或已经重新开启调整后还是没有恢复，则继续等待
		   {
			   P1OUT&=~BIT0;P4OUT&=~BIT7;                //恢复正常电路
			   bb=0;
		   }
    	   bili=(float)(z/1.0/y);              //在调整电流时先计算出电流比例
    	                                       //强制转换，是对紧跟其后的变量进行强制转换，所以，后面需要加括号
//    	   Picture_shuzi(bili,4,1);            //同步显示出当前比例
    	   if((w[1]>0)&&(w[1]<1.40))               //当电流是1A时，
    	   {
    		   Picture_shuzi2(1.0,6,3);         //显示当前需要调整的电流比例
                                               //第一步：不用变，因为CODE值就是相等的
    		   flag2=0;flag1234=0;flag12345=0;
    		   P1IE&=~BIT1;flag=0;
    	   }
    	   else if((w[1]>1.40)&&(w[1]<3.8))        //电流在1.5A~3.5A之间
    	   {
 //   		   flag2=1;
    	       if(flag==0)                         //由flag确定整个程序只能设置一次，除非此次调整完成
    	       {                                   //第一次设置，有用
    	    	   P1IE|=BIT1;                           //使能P1.1中断//在中断中完成比例的设置
//    	    	   if(flag2==0)
    	    	      aa=0.5;                           //如果没有手动设置，则按照上一次比例进行调整
//    	    	   else                                 //默认比例，0.5
//    	    		   aa=1.0;
    	    	   P4OUT|=BIT7;
    			   _delay_cycles(50000);//提示要进入设置界面,P4.7灯闪烁
                   P4OUT&=~BIT7;
    	       }                                     //进行电流比例的配置,设置完才会退出循环

    	       if(aa!=0.5)
    	       {
    	    	    if(flag12345==0)//进行一次调整的标志位
                     if((bili<aa-0.2)||(bili>aa+0.2))
                     {
                    	 Picture_shuzi2(aa,6,3);
                    	 b=(int)(256-256*w[1]*aa/2.82/(aa+1));
    	    	         a=(int)(256-256*w[1]/2.82/(aa+1));
                         adjust1(a);
                         adjust2(b);
                         flag12345=1;
                         _delay_cycles(20);_delay_cycles(20);_delay_cycles(20);
                         ADC12CTL0 |=0x0003;
                         _delay_cycles(20);_delay_cycles(20);

                         w[0]=temp2+temp3;
                         flag23=0;
                     }
//                    if((bili>aa-0.1)&&(bili<aa+0.1))
//         		     {
          			   P4OUT|=BIT7;               //调整完全,P4.7灯闪烁，提示完成，可进行下一次调整
          			   _delay_cycles(20000);
          			   P4OUT&=~BIT7;
//          			   flag=0;                   //调增完后，等待下一次调整
          			   P1IE|=BIT1;
//         		   }
    	        }
    	       else
    	       {   if(flag1234==0)//进行一次调整的标志位
    	       {
                   if((bili<aa-0.2)||(bili>aa+0.2))
                   {
                	   Picture_shuzi2(0.5,6,3);
                  	   b=(int)(256-256*w[1]*aa/2.82/(aa+1));
  	    	           a=(int)(256-256*w[1]/2.82/(aa+1));
                       adjust1(a);
                       adjust2(b);
                       flag1234=1;
                       _delay_cycles(20);_delay_cycles(20);_delay_cycles(20);
                       ADC12CTL0 |=0x0003;
                       _delay_cycles(20);_delay_cycles(20);

                       w[0]=temp2+temp3;
                       flag23=0;
                   }
    	       }
    	       }
    	   }
    	   else if((w[1]>=3.80)&&(w[1]<4.4))            //第三梯度：电流为4.0A
    	   {
    		   flag4A=1;
    		   flag2=0;//当电压从4A范围变到1.5~3.5A范围时，1.5~3.5A内的比例由这一次比例1:1决
    		   flag1234=0;//当从4A回到1.5~3.5A范围时，可以对1.5~3.5A范围内的电流进行再一次按比例分配
    		   flag12345=0;
    		   flag=0;
    		   flag2=1;
    		   P1IE&=~BIT1;
    		   Picture_shuzi2(1.0,6,3);          //电流不需调整   1:1
    	   }
    	   else if(w[1]>4.5)                       //关断
    	   {
    		   P1OUT|=BIT0;P4OUT|=BIT7;
    		   flag123[bb]=1;
    		   bb++;
    		   if(bb==2000)
    			   bb=0;
    		   adjust1(0xff);                        //用于给电路数值最小，        另一种方法是给那个傻瓜芯片一个电平
    		   adjust2(0xff);
			   while(1)
			   {
				    _delay_cycles(500000);             //隔一段时间重启一次，看是否恢复
				    _delay_cycles(500000);  _delay_cycles(500000);
				    _delay_cycles(500000);
					 break;
				}
    	   }
       }


//		_delay_cycles(10000);                    //等待电路调整
		_delay_cycles(1000);
		ADC12CTL0 |=0x0003;                     //每循环完一次，就重新采样
		_delay_cycles(20);
	}
}






#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)//进行按键比例的设置
{
	P1IE&=~BIT1;
    unsigned int i,PUSHKEY=0;
    unsigned char *bbb[]={she,zhi,maohao,kong,kong,kong,maohao,YI};
    PUSHKEY=P1IFG&(~P1DIR);

    for(i=0;i<5536;i++);for(i=0;i<5536;i++);for(i=0;i<5536;i++);
    for(i=0;i<136;i++);
    if((PUSHKEY&BIT1) == BIT1)
    {
        Picture_hanzi(bbb,8,6,0);           //设置界面
        P1OUT|=BIT0;
    	aa=juzhengjianpan();                //设置当前比例值//0.5~2  :1

    }
    for(i=0;i<5536;i++);for(i=0;i<5536;i++);for(i=0;i<5536;i++);
    for(i=0;i<136;i++);for(i=0;i<5536;i++);for(i=0;i<5536;i++);for(i=0;i<5536;i++);
    for(i=0;i<15536;i++);
    if(aa==0.5)
    	flag1234=0;
    flag=1;
    flag12345=0;
    Picture_shuzi2(aa,6,3);
    P1OUT&=~BIT0;P4OUT&=~BIT7;                         //设置完后灯灭
    P1IFG &= ~BIT1;                          // Clear P2.1 IFG
}
