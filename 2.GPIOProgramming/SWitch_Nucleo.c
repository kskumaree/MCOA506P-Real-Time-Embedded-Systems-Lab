#include "stm32l4xx.h"

void configureLED(void)
{
	RCC->AHB2ENR |=(1UL<<0);
	GPIOA->MODER &= ~(0xFFUL<<5*2);
	GPIOA->MODER |= (0x01UL<<5*2);
}


void configurePB(void)
{
	RCC->AHB2ENR |=(0x1UL<<2);
	GPIOC->MODER &= ~(0x3UL<<13*2);

}

void msDelay(int msTime)
{
	//Assume for loop take 12 clock cycles and system clock is 16MHz
int Time=msTime*1333;
	for(int i=0;i<Time;i++);

}


int main ()
{
	configureLED();
	configurePB();
	unsigned int PB_state;


	while(1)
	{
	PB_state=(GPIOC->IDR&(0x1UL<<13));
			if(PB_state!=0)
			{
				GPIOA->ODR &=~ (0x1UL<<5);
				//msDelay(500);
			}
			else
			{
				GPIOA->ODR |= (0x1UL<<5);
				msDelay(50);
			}
}

}



