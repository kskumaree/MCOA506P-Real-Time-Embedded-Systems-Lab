#include "stm32f4xx.h"
#define ARM_MATH_CM4
#include "arm_math.h"
#include "arm_const_structs.h"
#include "arm_common_tables.h"


void ADC_Init(void);
void ADC_Enable(void);
void ADC_Start(int);
float32_t value[8500],value1[8500],sum,RMS,out,currentRMS;
uint8_t ADC_VAL[8500];
uint32_t len=8500;
float32_t step=1.5/128;
void ADC_Init (void)
{
	/************** STEPS TO FOLLOW *****************
	1. Enable ADC and GPIO clock
	2. Set the prescalar in the Common Control Register (CCR)
	3. Set the Resolution in the Control Register 1 (CR1)
	4. Set the Continuous Conversion, EOC, and Data Alignment in Control Reg 2 (CR2)
	5. Set the Sampling Time for the channels in ADC_SMPRx
	6. Set the Regular channel sequence length in ADC_SQR1
	7. Set the Respective GPIO PINs in the Analog Mode
	************************************************/

//1. Enable ADC and GPIO clock
	RCC->APB2ENR |= (1<<8);  // enable ADC1 clock
	RCC->AHB1ENR |= (1<<0);  // enable GPIOA clock
	RCC->CFGR |= 6<<13; // set APB2 = 2 MHz (16/8)

//2. Set the pre-scalar in the Common Control Register (CCR)
	ADC->CCR |= 0<<16;  		 // PCLK2 divide by 2

//3. Set the Resolution in the Control Register 1 (CR1)
	//ADC1->CR1 &= ~(1<<8);    // SCAN mode disabled, enable for multichannel use
	ADC1->CR1 |= (2<<24);   // 8 bit RES

//4. Set the Continuous Conversion, EOC, and Data Alignment in Control Reg 2 (CR2)
		//ADC1->CR2 |= (1<<1);     // enable continuous conversion mode
		ADC1->CR2 |= (1<<10);    // EOC after each conversion
		ADC1->CR2 &= ~(1<<11);   // Data Alignment RIGHT






//5. Set the Sampling Time for the channels
	//ADC1->SMPR2 &= ~(1<<0);  // Sampling time of 112 cycles for channel 0
	ADC1->SMPR2|=(5<<0);
//6. Set the Regular channel sequence length in ADC_SQR1
	//ADC1->SQR1 &= ~(1<<20);   // SQR1_L =0 for 1 conversion

//7. Set the Respective GPIO PIN in the Analog Mode
	GPIOA->MODER |= (3<<0);  // analog mode for PA 0 (channel 0)

}
void msDelay(uint32_t msTime)
{
	/* For loop takes 4 clock cycles to get executed. Clock frequency of stm32f407 by default is 16MHz
	So, 16MHz/4=4MHz. If we want 1000ms delay, 4MHz/1000=4000, so we have to multiply by 4000 to get a delay of 1s
	*/
	for(uint32_t i=0;i<msTime*3000;i++)
	{
		__NOP();
	}

}

int main ()
{

	ADC_Init ();
	ADC1->CR2 |= 1<<0;   // ADON =1 enable ADC1
	uint32_t delay = 10000;
	while (delay--);//Wait sometime for ADC to start

	//ADC1->CR2 |= (1<<30);  // start the conversion
	while (1)
	{

		for(int i=0;i<8500;i++)
		{
			ADC1->CR2 |= (1<<30);  // start the conversion
			ADC1->SR = 0;        // clear the status register
			//ADC1->CR2 |= (1<<30);  // start the conversion
			while (!(ADC1->SR & (1<<1)));  // wait for EOC flag to set
			ADC_VAL[i]=ADC1->DR;
		}
//((float)value[i]-128)*step
//float step=1.5/128
		for(int j=0;j<8500;j++)
				{
					value[j]=((float)ADC_VAL[j]);
					value1[j]=(((float32_t)ADC_VAL[j]*0.0118)-1.506);

				}

			arm_rms_f32(value1,len,&RMS);
		//sum=sum/8500;
		//arm_sqrt_f32(sum,&out);
currentRMS=RMS*1.658;
	}

}
