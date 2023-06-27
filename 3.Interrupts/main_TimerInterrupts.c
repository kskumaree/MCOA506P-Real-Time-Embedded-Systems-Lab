#include "stm32f4xx.h"
#define ARM_MATH_CM4

void TIM2_Init(void);
void configureLED(void);
void TIM2_Interrupt_Enable(void);


void TIM2_Init (void)
{

//1. Enable TIM@ and GPIO clock
	RCC->APB1ENR |= (1<<0);  // enable TIM2 clock
		RCC->CFGR |= 0<<10; // set APB1 = 16 MHz

//2. Timer pre-scalar and period configuration
	TIM2->CR1 &= ~(0x0010);    		//Set the mode to Count up
		TIM2->PSC = 16000-1;					//Set the Prescalar
		TIM2->ARR = 500-1; 						//Set period (Auto reload) to 400
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
			//while (!(TIM2->SR & (1<<0)));  // wait for Timer Overflow flag to set
			//GPIOD->ODR ^= (0xFUL<<12);
			//TIM2->SR &= ~(0x0001);				//Clear Update interrupt flag
			//}
}
}


void TIM2_IRQHandler( )
{
	GPIOD->ODR ^= (0x1UL<<12);
	TIM2->SR &= ~(0x0001);

}
