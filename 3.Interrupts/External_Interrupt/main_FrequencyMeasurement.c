#include "stm32f4xx.h"

void EXT_Init(void);
void configureLED(void);
void External_Interrupt_Enable(void);
void TIM2_Init(void);
void TIM2_Interrupt_Enable(void);

unsigned int count=0, Freq=0;
void EXT_Init(void)
{


	RCC->APB1ENR |= (1<<0);  // enable PORTA clock
	EXTI->RTSR |=(1<<0);
	EXTI->IMR |=(1<<0);
}
void configureLED(void)
{
	RCC->AHB1ENR |=(1UL<<3);  //Enable GPIOD clock
	GPIOD->MODER &= ~(0xFFUL<<12*2);
	GPIOD->MODER |= (0x55UL<<12*2);
}

void External_Interrupt_Enable(void)
{
	NVIC->ISER[0] |= 1<<6;
}
void TIM2_Interrupt_Enable(void)
{
	NVIC->ISER[0] |= 1<<28;
	TIM2->DIER |=(1<<0);
}
void TIM2_Init (void)
{

//1. Enable TIM@ and GPIO clock
	RCC->APB1ENR |= (1<<0);  // enable TIM2 clock
		RCC->CFGR |= 0<<10; // set APB1 = 16 MHz

//2. Timer pre-scalar and period configuration
	TIM2->CR1 &= ~(0x0010);    		//Set the mode to Count up
		TIM2->PSC = 16000-1;					//Set the Prescalar
		TIM2->ARR = 1000-1; 						//Set period (Auto reload) to 400
		TIM2->SR &= ~(0x0001);				//Clear Update interrupt flag

}

int main ()
{
	TIM2_Init ();
	TIM2_Interrupt_Enable();
	EXT_Init();
	configureLED();
	External_Interrupt_Enable();
	TIM2->CR1 |= 1UL;
	while(1)
	{
		//GPIOD->ODR = (0x0UL<<12);
}
}


void EXTI0_IRQHandler( )
{
	count=count+1;
	EXTI->PR |= (1<<0);

}

void TIM2_IRQHandler( )
{
	Freq=count;
	count=0;
	//GPIOD->ODR ^= (0x1UL<<12);
	TIM2->SR &= ~(0x0001);

}
