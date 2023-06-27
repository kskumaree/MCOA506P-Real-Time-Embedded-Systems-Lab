#include "stm32f4xx.h"
void msDelay(volatile int);
void ADC_Init(void);


unsigned int value,value1;
float stepSize=0.01137,voltage,voltage1;
void ADC_Init (void)
{
//High speed internal clock is 16MHz
//1. Enable ADC and GPIO clock
	RCC->APB2ENR |= (1<<8);  // enable ADC1 clock
	RCC->AHB1ENR |= (1<<0);  // enable GPIOA clock
	RCC->CFGR |= 6<<13; // AHB clock divided by 8; set APB2 = 2 MHz

//2. Set the pre-scalar in the Common Control Register (CCR)
	ADC->CCR |= 0<<16; //00: PCLK2 divided by 2, works at 1MHz

//3. Set the Resolution in the Control Register 1 (CR1)
	//ADC1->CR1 &= ~(1<<8);    // SCAN mode disabled, enable for multichannel use
	ADC1->CR1 |= (2<<24);   // 8 bit RES

//4. Set the Continuous Conversion, EOC, and Data Alignment in Control Reg 2 (CR2)
		//ADC1->CR2 |= (1<<1);     // enable continuous conversion mode
		ADC1->CR2 |= (1<<10);    // EOC after each conversion
		ADC1->CR2 &= ~(1<<11);   // Data Alignment RIGHT
//5. Set the Sampling Time for the channels
	//ADC1->SMPR2 &= ~(1<<0);  // Sampling time of 3 cycles for channel 0
	ADC1->SMPR2|=(7<<0); // 480+8cycles (number of bits)=488 cycles
//6. Set the Regular channel sequence length in ADC_SQR1
	ADC1->SQR1 |= (0<<20);   // SQR1_L =0 for 1 conversion

//7. Set the Respective GPIO PIN in the Analog Mode
	GPIOA->MODER |= (3<<0);  // analog mode for PA 0 (channel 0)
	GPIOA->MODER |= (3<<2);  // analog mode for PA 1 (channel 0)
	GPIOA->MODER |= (3<<4);  // analog mode for PA 1 (channel 0)
}
void msDelay(int msTime)
{
//Assume for loop take 12 clock cycles and system clock is 16MHz
int Time=msTime*1333;
	for(int i=0;i<Time;i++);

}

int main ()
{
//FPU enable
	SCB->CPACR|=(0xF<<20); //Co-processor (core peripheral) access control register
	ADC_Init ();
	ADC1->CR2 |= 1<<0;   // ADON =1 enable ADC1
	msDelay(500);

	//ADC1->CR2 |= (1<<30);  // start the conversion
	while (1)
	{
			//Channel 1
		    ADC1->SQR3=0;
		    ADC1->SQR3|=(2<<0);
		    ADC1->SR = 0;        // clear the status register
		    ADC1->CR2 |= (1<<30);  // start the conversion
			while (!(ADC1->SR & (1<<1)));  // wait for EOC flag to set
			value=ADC1->DR;
			voltage=value*stepSize;
			//Channel 2
			ADC1->SQR3=0;
			ADC1->SQR3|=(1<<0);
			ADC1->SR = 0;        // clear the status register
			ADC1->CR2 |= (1<<30);  // start the conversion
			while (!(ADC1->SR & (1<<1)));  // wait for EOC flag to set
			value1=ADC1->DR;
			voltage1=value1*stepSize;
	}
}

