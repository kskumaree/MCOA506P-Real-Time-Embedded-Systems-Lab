#include "stm32f4xx.h"
#define ARM_MATH_CM4

void TIM2_Init(void);
void configureLED(void);
void TIM2_Interrupt_Enable(void);
void msDelay(int msTime);

void TIM2_Init (void)
{

//1. Enable TIM@ and GPIO clock
	RCC->APB1ENR |= (1<<0);  // enable TIM2 clock
		RCC->CFGR |= 0<<10; // set APB1 = 16 MHz

//2. Timer pre-scalar and period configuration
	TIM2->CR1 &= ~(0x0010);    		//Set the mode to Count up
		TIM2->PSC = 16000-1;					//Set the Prescalar
		TIM2->ARR = 200; 						//Set period (Auto reload) to 400
		TIM2->SR &= ~(0x0001);				//Clear Update interrupt flag

}
void configureLED(void)
{
	RCC->AHB1ENR |=(1UL<<3);  //Enable GPIOD clock
	GPIOD->MODER &= ~(0xFFUL<<12*2);
	GPIOD->MODER |= (0x55UL<<12*2);
}

void TIM2_Interrupt_Enable(void)
{
	NVIC->ISER[0] |= 1<<28;
	TIM2->DIER |=(1<<0);
}


int main ()
{

	TIM2_Init ();
	configureLED();
	TIM2_Interrupt_Enable();
	TIM2->CR1 |= 1UL;
while(1)
	{
	GPIOD->ODR ^= (0x2UL<<12);
			msDelay(500);
}
}


void TIM2_IRQHandler( )
{
	GPIOD->ODR ^= (0x1UL<<12);
	TIM2->SR &= ~(0x0001);

}

void msDelay(int msTime)
{
	/* For loop takes 4 clock cycles to get executed. Clock frequency of stm32f407 by default is 16MHz
	So, 16MHz/4=4MHz. If we want 1000ms delay, 4MHz/1000=4000, so we have to multiply by 4000 to get a delay of 1s
	*/
	for(int i=0;i<msTime*4000;i++)
	{
		__NOP();
	}

}
