/*
 * This code computes speed of the motor using just channel A of the encoder.
 * Hence, for every cycle of signal A, there are 2 pulses generated. PPR = 2*CPR.
 * Using external interrupts, the number of cycles is determined as pulseCount.
 * Every 1s, pulseCount is read, speed is computed and pulseCount is reset by TIM2's ISR.
 * Give input in PA0
 */

#include "stm32f4xx.h"


#define ARM_MATH_CM4
#define CPR 1024
#define PPR 2*1024
#define PI 3.14

void EXT_Init(void);
void External_Interrupt_Enable(void);

unsigned int pulseCount = 0;
float positionDEG;
float positionRAD;
float speedRPS;
float speedRADS;

void EXT_Init(void)
{
	RCC->APB1ENR |= (1<<0);  // enable PORTA clock
	EXTI->RTSR |=(1<<0);
	EXTI->IMR |=(1<<0);
}


/*Configuring TIM2 for an interrupt every 1s*/
void configTIM2(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;			//TIM2 clock enable
	TIM2->CR1 &= ~(0x0010);    					//Set the mode to Count up
	TIM2->PSC = 16000-1;						//Set the Prescalar
	TIM2->ARR = 1000-1; 					    //Set period (Auto reload)
	TIM2->SR &= ~(0x0001);						//Clear Update interrupt flag
	NVIC->ISER[0] |= 1<<28;
	TIM2->DIER |=(1<<0);
}

void External_Interrupt_Enable(void)
{
	NVIC->ISER[0] |= 1<<6;
}

int main ()
{

	SCB->CPACR|=(0xF<<20); //Co-processor (core peripheral) access control register
	EXT_Init();

	External_Interrupt_Enable();
	configTIM2();
	TIM2->CR1 |= (1<<0);			//Enable TIM2
	while(1)
	{

	}
}


void EXTI0_IRQHandler( )
{
	pulseCount++;
	EXTI->PR |= (1<<0);
}


void TIM2_IRQHandler( )
{
	positionDEG  = 360*pulseCount/(PPR*1.0);
	positionRAD  = 2*PI*pulseCount/(PPR*1.0);
	speedRPS  = pulseCount/(PPR*1.0);
	speedRADS = 2*PI*pulseCount/(PPR*1.0);

	pulseCount = 0;
	TIM2->SR &= ~(0x0001);
}
