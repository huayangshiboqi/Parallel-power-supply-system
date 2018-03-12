//OLED的字库
//16*16
void OLED(void);
void Picture_shuzi(float p,int a,int b);  //浮点型小数   显示行数，显示列数。显示2.123形式
void Picture_hanzi(unsigned char *p[],int nn,int a,int b); //显示的汉字数组   显示汉字的个数   显示的行数   显示的列数
void Picture_shuzizheng(int p,int a,int b);                //整形数     显示的行数    显示的列数
void Picture_shuzi2(float p,int a,int b);//浮点型小数  显示行数   显示列数 。格式2.12形式
 