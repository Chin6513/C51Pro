void delay(unsigned int xms)
//***********负责延迟，延迟xms毫秒***********//
{
	unsigned int i;
	for(;xms>0;xms--)
	for(i=110;i>0;i--);
}