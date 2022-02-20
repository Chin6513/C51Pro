/*******************************************************************************
安徽工程大学电子科技协会大作业
*专业：集成电路设计与集成系统
*姓名：秦煜杰
*题目：让led灯每隔0.5秒依次序点亮并且依次序熄灭；
*	让数码管每隔10毫秒从0.00加0.01至9999
*	有0去0，小数点前满了小数点后移但是速率不变
*	利用按键可以暂停和解除这两个过程
*解析：速率不变理解为加0.01的速率不变，有0去0理解为有效数字，
*	led依次序亮灭理解为依次序亮满，再依次序全灭，但是题目未说明方向
*******************************************************************************/
#include<reg52.h>
#include"init.h"
#include"delay.h"
#define uint unsigned int
#define uchar unsigned char

sbit N1=P2^3;
sbit N2=P2^4;
sbit N3=P2^5;
sbit N4=P2^6;
sbit key0=P3^3;
sbit judge=P1^7;		//led判断变量，通过该变量确定整体状态，实现led的循环亮灭

uchar a[]={0x05,0x7d,0x46,0x54,0x3c,0x94,0x84,0x5d,0x04,0x14};//数码管'0'至'9'
uchar b[]={0x01,0x79,0x42,0x50,0x38,0x90,0x80,0x59,0x00,0x10};//数码管'0.'至'9.'

uint x,y,z;		//x储存整数部分，y储存第一个小数，z储存第二个小数
uchar num,kc=1;//num负责定时器计数，kc用来控制外部中断

void main()
{
	void display(uint x,uint y,uint z);	//显示函数
	void count();								//计数函数
	void case1(uint x);
	void case2(uint x,uint y);
	void case3(uint x,uint y,uint z);
	void fourth(uint k);
	void third(uint k);
	void thirds(uint k);
	void second(uint k);
	void seconds(uint k);
	void first(uint k);	
	total();
	out();
	time();
	count();
}

//****负责计数，将要现实的数进行拆分为xyz****//
void count()
{
	x=0;y=0;z=0;
	while(x<=9999)
	{
		display(x,y,z);
		P0=0xff;		//消除残影
		delay(10);
		z++;			//延迟10秒之后加0.01
		if(z==10){y++;z=0;}
		if(y==10){x++;y=0;}
	}
}

//***负责将拆分的数字再划分，分配到下一过程***//
void display(uint x,uint y,uint z)
{
	uint p,q,m,n;
	p=x/1000;	//千位
	q=x/100%10;	//百位
	m=x/10%10;	//十位
	n=x%10;		//个位
	
	if(y==0&&z==0)		//0~9999
		case1(x);
	else if(z==0)		//x.1~x.9;特殊:8888.9
		case2(x,y);
	else					//x.01~x.y9;特殊:8888.99#888.99
		case3(x,y,z);
}

//************负责0~9999的显示分配***********//
void case1(uint x)
{
	uint p,q,m,n;
	p=x/1000;	//千位
	q=x/100%10;	//百位
	m=x/10%10;	//十位
	n=x%10;		//个位
	
	if(p!=0)			//四位数显示分配
	{fourth(p);third(q);second(m);first(n);}
	else if(q!=0)	//三位数显示分配
	{third(q);second(m);first(n);}
	else if(m!=0)	//两位数显示分配
	{second(m);first(n);}
	else				//个位数显示分配
	{first(n);}
}

//*********负责小数点后一位的显示分配*********//
void case2(uint x,uint y)
{
	uint p,q,m,n;
	p=x/1000;	//千位
	q=x/100%10;	//百位
	m=x/10%10;	//十位
	n=x%10;		//个位
	
	if(p!=0)
		case1(x);												//pqmn.y->pqmn
	else if(q!=0)
		{fourth(q);third(m);seconds(n);first(y);}		//0qmn.y->qmn.y
	else if(m!=0)
		{third(m);seconds(n);first(y);}					//00mn.y->_xx.x
	else
		{seconds(n);first(y);}								//000n.y->__n.y
}

//*********负责小数点后两位的显示分配************//
void case3(uint x,uint y,uint z)
{
	uint p,q,m,n;
	p=x/1000;	//千位
	q=x/100%10;	//百位
	m=x/10%10;	//十位
	n=x%10;		//个位
	
	if(p!=0)
		case1(x);												//pqmn.yz->pqmn
	else if(q!=0)
		case2(x,y);												//_qmn.yz->qmn.y
	else if(m!=0)
		{fourth(m);thirds(n);second(y);first(z);}		//__mn.yz->mn.yz
	else
		{thirds(n);second(y);first(z);}					//n.yz->_n.yz
}

//********不带小数点****第一个数码管显示*********//
void fourth(uint k)
{
	N1=0;
	P0=a[k];
	delay(1);
	N1=1;
}

//********不带小数点****第二个数码管显示*********//
void third(uint k)
{
	N2=0;
	P0=a[k];
	delay(1);
	N2=1;
}

//********带小数点****第二个数码管显示*********//
void thirds(uint k)
{
	N2=0;
	P0=b[k];
	delay(1);
	N2=1;
}

//********不带小数点****第三个数码管显示*********//
void second(uint k)
{
	N3=0;
	P0=a[k];
	delay(1);
	N3=1;
}

//********带小数点****第三个数码管显示*********//
void seconds(uint k)
{
	N3=0;
	P0=b[k];
	delay(1);
	N3=1;
}

//********不带小数点****第一个数码管显示*********//
void first(uint k)
{
	N4=0;
	P0=a[k];
	delay(1);
	N4=1;
}

//*********外部中断*****按键控制显示状态**********//
//实现方法:通过不断扫描按键状态结束中断
void ext_0() interrupt 0
{
	kc=1;
	while(kc==1)
	{
		display(x,y,z);
		if(!key0)		//按一下执行
		{kc=!kc;}
	}
}

//********定时器中断*****LED灯的显示*******//
//实现方法:通过判断led8的接口状态确定移入led1的状态，整体左移
void T0_time() interrupt 1
{
	TH0=(65536-45872)/256;
	TL0=(65536-45872)%256;
	num++;
	if(num==10)
	{
		num=0;
		if(judge==0)			//当最下端灯灭时
		P1=((P1<<1)+1);		//左移后最上端亮
		else						//否则
		P1=P1<<1;				//左移后最上端灭
	}
}