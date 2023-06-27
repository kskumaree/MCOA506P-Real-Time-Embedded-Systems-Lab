#include "stm32f4xx.h"

void GPIO_Config(void);
void TIM_Config(void);

int main(void)
{
	GPIO_Config();
	TIM_Config();
	//Start Timer2
	TIM4->CR1 |= 1UL;
	while(1)
	{
	}
}

void GPIO_Config(void)
{
	//GPIOD clock enable
	RCC->AHB1ENR |= (1UL << 3);
	//Set GPIOD pins 12 - 15 to alternate mode
	GPIOD->MODER |= (2UL << 12*2) | (2UL << 13*2) | (2UL << 14*2) | (2UL << 15*2);
	//Set alternate function to AF2 (TIM4)
	GPIOD->AFR[1] |= (2UL << 4*4) | (2UL << 5*4) | (2UL << 6*4) | (2UL << 7*4);
}
void TIM_Config(void)
{
	// Basic timer configuration
	RCC->APB1ENR |= (1UL << 2);  	//Enable TIM4 clock
	TIM4->CR1 &= ~(0x0010);    		//Set the mode to Count up
	TIM4->PSC = 160-1;					//Set the Prescalar
	TIM4->ARR = 1000-1; 						//Set period (Auto reload) 10ms duration of total pulse

	//Channel 1 config
	TIM4->CCMR1 |= (6UL << 4);    //Set OutputCompare mode to PWM
	TIM4->CCMR1 &= ~(3UL << 0);   //CC to output
	TIM4->CCER &= ~(1UL << 1);   	//Output Compare polarity to active high
	TIM4->CCER |= (1UL << 0);   	//Capture compare output enable
	//Set the Pulse width
	TIM4->CCR1 = 100;//10% duty cycle

	//Channel 2 config
	TIM4->CCMR1 |= (6UL << (4+8));    //Set OutputCompare mode to PWM
	TIM4->CCMR1 &= ~(3UL << 8);   //CC to output
	TIM4->CCER &= ~(1UL << (1+4));   	//Output Compare polarity to active high
	TIM4->CCER |= (1UL << 4);   	//Capture compare output enable
	//Set the Pulse width
	TIM4->CCR2 = 200;//20% duty cycle

	//Channel 3 config
	TIM4->CCMR2 |= (6UL << 4);    //Set OutputCompare mode to PWM
	TIM4->CCMR2 &= ~(3UL << 0);   //CC to output
	TIM4->CCER &= ~(1UL << 9);   	//Output Compare polarity to active high
	TIM4->CCER |= (1UL << 8);   	//Capture compare output enable
	//Set the Pulse width
	TIM4->CCR3 = 300;//30% duty cycle

	//Channel 4 config
	TIM4->CCMR2 |= (6UL << (4+8));    //Set OutputCompare mode to PWM
	TIM4->CCMR2 &= ~(3UL << 8);   //CC to output
	TIM4->CCER &= ~(1UL << 13);   	//Output Compare polarity to active high
	TIM4->CCER |= (1UL << 12);   	//Capture compare output enable
	//Set the Pulse width
	TIM4->CCR4 = 400;//40% duty cycle
}
