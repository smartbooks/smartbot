/*-----------------------------------------------
  名称：IO口高低电平控制
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：点亮P1口的一个LED灯
        该程序是单片机学习中最简单最基础的，
		通过程序了解如何控制端口的高低电平
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，
                  //头文件包含特殊功能寄存器的定义


void Delay50ms() {

	unsigned char i, j, k;

	i = 3;
	j = 22;
	k = 124;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);

}

sbit LED=P1^0;// 用sbit 关键字 定义 LED到P1.0端口，
              //LED是自己任意定义且容易记忆的符号

/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void) {

	while (1) {
		
		LED=1;            //将P1.0口赋值 1，对外输出高电平
		Delay50ms();
		
		LED=0;            //将P1.0口赋值 0，对外输出低电平
		Delay50ms();
	
	}

}