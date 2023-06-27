#include "stm32f4xx.h"
void configureLED(void);
void msDelay(volatile int msTime);
int main(void)
{
	RCC->CFGR |= (0x8<<4); // system clock divided by 2 (AHB prescaler)
	//Configure LED
	configureLED();
	//Define Delay function
	msDelay(1000);

	while(1)
	{
		GPIOD->ODR ^= (0xFUL<<12);
		msDelay(500);
	}
}
void configureLED(void)
{
	RCC->AHB1ENR |=(1UL<<3);
	GPIOD->MODER &= ~(0xFFUL<<12*2);
	GPIOD->MODER |= (0x55UL<<12*2);
}
void msDelay(int msTime)
{
	//Assume for loop take 12 clock cycles and system clock is 16MHz
int Time=msTime*1333;
	for(int i=0;i<Time;i++);

}
