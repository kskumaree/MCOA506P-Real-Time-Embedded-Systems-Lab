/*
TIM1 counts the pulses using input capture mode, TIM2 checks the TIM1->CNT value and calculates speed.
*/

#include "stm32f4xx.h"
#include <stdio.h>
#include<math.h>
#define ARM_MATH_CM4

float speed;     // in rad/sec
uint16_t ticks;


/*Enabling Clocks*/
void clocksEnable(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;  //GPIO Port E for PE9, PE11
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;      //TIM1
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;      //TIM2
}


/*Configuring GPIO for input capture*/
void configGPIO(void)
{
	GPIOD->MODER |= (1<<22);
	GPIOE->MODER |= ((2<<18)|(2<<22));      //AF for PE9, PE11
	GPIOE->AFR[1] |= ((1<<4)|(1<<12));      //AF1 for TIM1_CH1, TIM1_CH2
}


/*Configuring TIM1 in Encoder Interface Mode*/
void configTIM1(void)
{
	TIM1->PSC = 0x0000;             //Prescalar = 0
	TIM1->ARR = 0xFFFF;             //Period = 65536
	TIM1->CR1 &= ~(1<<4);           //Upcounting Mode

	TIM1->SMCR |= (3<<0);	//Encoder mode 3, both TI1 and TI2 i.e. channel A and B are captured

	/*
	 * IC1F=IC2F = 0b0000, IC1PSC=IC2PSC = 0b00, CC1S=CC2S = 0b01
	 * TIM1->CCMR1 = 0b0000000100000001;
	 */

	TIM1->CCMR1 |= ((0x1<<0)|(0x1<<8));	    //CC1 and CC2 are input. CC1 = TI1, CC2 = TI2
	TIM1->CCMR1 &= ~((0x3<<2)|(0x3<<10));	//Prescalars are 0 for IC1PSC and IC2PSC
	TIM1->CCMR1 &= ~((0xF<<4)|(0xF<<12));	//Filters are 0 for IC1F and IC2F
	TIM1->CCER |= ((0x1<<0)|(0x1<<4));		//CC1E and CC2E are enabled
}


/*Configuring TIM2 for an interrupt every 1s*/
void configTIM2(void)
{
	TIM2->CR1 &= ~(0x0010);    		//Set the mode to Count up
	TIM2->PSC = 16000-1;			//Set the Prescalar
	TIM2->ARR = 1000-1; 			//Set period (Auto reload) to 1000
	TIM2->SR &= ~(0x1<<0);			//Clear Update interrupt flag

}


int main(void)
{
	SCB->CPACR|=(0xF<<20);			//FPU enable
	clocksEnable();
	configGPIO();
	configTIM1();
	configTIM2();

	NVIC->ISER[0] |= 1<<28;
	TIM2->DIER |=(1<<0);

	TIM1->CR1 |= (1<<0);			//Enable TIM1
	TIM2->CR1 |= (1<<0);			//Enable TIM2

	while(1)
	{
		//Nothing here, ISR handles it all
	}
}

void TIM2_IRQHandler()
{
	/*
	   Speed in rad/s is 2*pi*ticks/(PPR*"how often the ISR happens in seconds in float")
	   for 1 channel, PPR = 2*CPR, for 2 channels, PPR = 4*CPR.
	   CPR = cycles per revolution, PPR is pulses per revolution.
	   This is calculated every 1ms because that's how often this ISR happens.
	   You can change how often this ISR happens in configTIM2(). 
	   If you do, be sure to change the denominator for speed calculation.
	*/

	ticks = TIM1->CNT;
	speed = (6.280*ticks)/(4*1024*1.000);
	TIM1->CNT=0;
	TIM2->SR &= ~(0x1<<0);
}
