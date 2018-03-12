#ifndef __ADC12_H__
#define __ADC12_H__
#include<msp430f5529.h>
#include"header.h"

//==========================================

//==============ADC12采样通道==================
#ifndef __ADC12_INCH__
#define __ADC12_INCH__

#define ADC12_INCH0			(1)
#define ADC12_INCH1			(2)
#define ADC12_INCH2			(3)
#define ADC12_INCH3			(4)
#define ADC12_INCH4			(5)
#define ADC12_INCH5			(6)
#define ADC12_INCH6			(7)
#define ADC12_INCH7			(8)
#define ADC12_INCH8			(9)
#define ADC12_INCH9			(10)
#define ADC12_INCH10		(11)
#define ADC12_INCH11		(12)
#define ADC12_INCH12		(13)
#define ADC12_INCH13		(14)
#define ADC12_INCH14		(15)
#define ADC12_INCH15		(16)

#endif
//==========================================

/*****************功能函数*********************/
//单通道多次采样
//通道输入 inch
//参考电压 AVCC AVSS
//采样时钟 ADC12OSC
//触发信号 ADC12SC
//采样定时时间 64*ADC12CLK
//采样数据寄存器ADC12MEM(inch)
//Notes:
//	主函数中设置： IO端口
//			  ADC12中断向量中 ADC12IV ADC12MEMx
void ADC12_SET_SINGLE_REPEATED(uchar inch);

//多通道多次采样(2通道)
//通道输入 inch1 inch2
//参考电压 AVCC AVSS
//采样时钟 SMCLK
//触发信号 ADC12SC
//采样定时时间 16*ADC12CLK
//采样数据寄存器 ADC12MEM(inch1)  ADC12MEM(inch2)
//Notes:
//	主函数中设置： IO端口
//			  ADC12中断向量中 ADC12IV ADC12MEMxs
void ADC12_SET_SEQUENCE_REPEATED(uchar inch1,uchar inch2,uchar inch3);
void ADC12_IO_Init();                              //定义ADC采样管脚

#endif
