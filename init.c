#include<reg52.h>
void total()
{
	EA=1;
}

void out()
{
	EX0=1;		//允许外部中断
	IT0=1;
}

void time()
{
	TMOD=0x01;	//设置定时器工作方式为16位定时器
	TH0=(65536-45872)/256;
	TL0=(65536-45872)%256;
	ET0=1;		//定时器中断开启
	TR0=1;		//开启定时器
}