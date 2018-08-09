#include "delay.h"


volatile uint16_t ntime;

uint32_t TimingDelay;

void Delay_pre(volatile uint32_t nTime)
{ 
	  TimingDelay = nTime;
	  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
		TimingDelay--;
}

void delay_ms(uint16_t nms)
{	 		  	   
//	ntime=nms;
	SysTick_Config(SystemCoreClock / 1000);//1ms产生一次中断
	TimingDelay = nms;
	while(TimingDelay != 0);
//	while(ntime);
//	SysTick->CTRL=0x00;			  	    
}   
		    								   
void delay_us(uint32_t nus)
{		
	ntime=nus;
	SysTick_Config(SystemCoreClock / 1000000);//1us产生一次中断
	TimingDelay = nus;
	while(TimingDelay != 0);
}

void delay_short(uint32_t count)
{
	while(count--);
}

void delay_long(uint32_t count)
{
	uint16_t n;
	uint32_t thecount;
	for(n=0;n<1000;n++)
	{
		thecount = count;
		while(thecount--);
	}
}