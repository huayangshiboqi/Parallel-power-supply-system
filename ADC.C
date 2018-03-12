#include"ADC12.h"
#include"msp430.h"
//==============��������===================
#define Num_of_Results 10
volatile unsigned int a[Num_of_Results]={0};
volatile unsigned int A3results[Num_of_Results];    //˫ͨ��A3 A4ȡ����ֵ������
volatile unsigned int A4results[Num_of_Results];
volatile unsigned int A5results[Num_of_Results];    //A3 P6.3��˥����A4 P6.4�ӷŴ�

//float temp1[5]={0.0},temp2[5]={0.0},temp3[5]={0.0};//��·�������
//unsigned int M=0;
float temp1=0.0,temp2=0.0,temp3=0.0;//��·�������

void ADC(void)
{
		    ADC12_IO_Init();                              //����ADC������
			ADC12_SET_SEQUENCE_REPEATED(ADC12_INCH0,ADC12_INCH1,ADC12_INCH2);//��ͨ��ѭ�����
			ADC12CTL0 |=0x0003;

			_EINT();									// Enable interrupt
			_NOP();										// For debugger
			 _enable_interrupts();                 //ʹ�����ж�

}

//=============��������===================
/**********ADC12 IO�˿�����***************/
void ADC12_IO_Init()
{
	P6SEL |= 0x07;								// Enable A/D channel
}




//=============�ж�����====================
/**************ADC12�ж�*****************/
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{

    static unsigned char index = 0;
	int i;
	//ע�⣺�������͵�ѡ������Ʊ�����ȡֵ��Χ���ڴ�index�ķ�Χ0~255����������ĸ������ܳ���255��������Ҫ��ȫ��Щ����ʱ�ᷢ�����㲻��Ҫ�Ľ��
	switch(__even_in_range(ADC12IV,36))
	{
		case  0: break;                           	// Vector  0:  No interrupt
	  	case  2: break;                           	// Vector  2:  ADC overflow
	  	case  4: break;                           	// Vector  4:  ADC timing overflow
	  	case  6: break;                           	// Vector  6:  ADC12IFG0
	  	case  8: break;                           	// Vector  8:  ADC12IFG1
	  	case 10:
	  		A3results[index] = ADC12MEM0;   A4results[index] = ADC12MEM1; A5results[index] = ADC12MEM2;       	// Move results
	  		if(index==0)
	  		{
	  			a[1]=a[2]=a[3]=0;
	  		}
			if(index==9)                          //ע�⣺��Χ��ȷ������Ȼ��Զ���������
			{
				for(i=0;i<10;i++)
				{
					a[1]+=A3results[i];
					a[2]+=A4results[i];
					a[3]+=A5results[i];
				}
				temp1=(a[1])/12409.091;
				temp2=(a[2])/12409.091/1.143;
				temp3=(a[3])/12409.091/1.143;
			}		                             	// Increment results index, modulo; Set Breakpoint1 here
	  		break;                           	// Vector 10:  ADC12IFG2
	  	case 12:break;                            	// Vector 12:  ADC12IFG3
	  	case 14:break;                              // Vector 14:  ADC12IFG4
	  	case 16: break;                           	// Vector 16:  ADC12IFG5
	 	case 18: break;                           	// Vector 18:  ADC12IFG6
	 	case 20: break;                           	// Vector 20:  ADC12IFG7
	 	case 22: break;                           	// Vector 22:  ADC12IFG8
	 	case 24: break;                           	// Vector 24:  ADC12IFG9
	 	case 26: break;                           	// Vector 26:  ADC12IFG10
	 	case 28: break;                           	// Vector 28:  ADC12IFG11
	 	case 30:break;                                  // Vector 30:  ADC12IFG12
	 	case 32: break;                           	// Vector 32:  ADC12IFG13
	 	case 34:break;                           	// Vector 34:  ADC12IFG14
	 	case 36:  break;                            // Vector 36:  ADC12IFG15
	 	default: break;
		}
		        index=index+1;                              	// Increment results index, modulo; Set Breakpoint1 here
	  	  		if (index == 10)                       //ע�⣺����ĸ�����������250.����index��ȡֵ��ΧΪ0~499��������ʵ���㷶Χʱһ��Ҫע�⡣
	  	  		{
	  	  			index = 0;
	  	  		}
//	  	  		if(M==5)
	  	  		   if(index==0)
	  	  		       ADC12CTL0 &=~ 0x0003;
//	            _delay_cycles(2);
}
