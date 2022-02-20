//***********负责延迟，延迟xms毫秒***********//
void delay(unsigned int xms)
{
	unsigned int i;
	for(;xms>0;xms--)
	for(i=110;i>0;i--);
}