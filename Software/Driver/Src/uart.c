#include "uart.h"
void UartInit(void)		//115200bps@27.000MHz@定时器2
{
//	SCON = 0x50;		//8位数据,可变波特率
//	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
//	AUXR &= 0xFB;		//定时器2时钟为Fosc/12,即12T
//	T2L = 0xFB;		//设定定时初值
//	T2H = 0xFF;		//设定定时初值
//	AUXR |= 0x10;		//启动定时器2
//	ES=1;						//打开接收中断
//	EA=1;						//打开总中断
}

void send8bit(u8 dat)
 {
	SBUF=dat;
	while(!TI);
	TI=0;
 }
void send(u8 *str,u16 length)
 {		
	 int i=0;
   for(i=0;i<length;i++)
   {	   
   send8bit(*(str+i));
   }
}