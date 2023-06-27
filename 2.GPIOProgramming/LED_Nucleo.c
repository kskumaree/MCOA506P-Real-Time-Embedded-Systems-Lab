#include "stm32l4xx.h"

void configureLED(void);
void msDelay(volatile int msTime);
int main(void)
{
	//Configure LED
	configureLED();
	//Define Delay function
	msDelay(1000);
	//GPIOD->ODR = (0x5UL<<12);
	while(1)
	{
		GPIOA->ODR ^= (0x1UL<<5);
		msDelay(200);
	}
}
void configureLED(void)
{
	RCC->AHB2ENR |=(1UL<<0);
	GPIOA->MODER &= ~(0xFFUL<<5*2);
	GPIOA->MODER |= (0x01UL<<5*2);
}
void msDelay(int msTime)
{
	//Assume for loop take 12 clock cycles and system clock is 16MHz
int Time=msTime*1333;
	for(int i=0;i<Time;i++);

}

