#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>

#define ARM_MATH_CM4

void USART2_init(void);
void GPIO_init(void);
void USART_read (volatile uint8_t *buffer,volatile uint32_t nBytes);
void configureLED(void);
void msDelay(int msTime);

void USART2_init(void){

//USART DISABLE
USART2->CR1 &= ~(1<<13);

//SETTING 8 BIT DATA TRANSMISSION
USART2->CR1 &= ~(1<<12);

//SETTING ONE STOP BIT
USART2->CR2  |= (0<<12);

//SET NO PARITY
USART2->CR1 &= ~(1<<10);

//OVERSAMPLING BY 16
USART2->CR1 &= ~(1<<15);

//SETTING BAUD RATE TO 9600
USART2->BRR = 0x683;

//ENABLE TRANSMSSION AND RECEPTION
USART2->CR1 |= (3<<2);

USART2->SR &= ~(1<<7);  //CLEARING TXE FLAG
USART2->SR &= ~(1<<6);  //CLEARING TC FLAG
USART2->SR &= ~(1<<5);  //CLEARING RXE FLAG

//ENABLE USART
USART2->CR1 |= (1<<13);

USART2->DR = 0x0;
}


void USART_read (volatile uint8_t *buffer, volatile uint32_t nBytes) {
int i;
for (i = 0; i < nBytes; i++) {
while (!(USART2->SR & (1<<5))); // Wait until hardware sets RXNE
//buffer[i] = USART2->DR&(0x1FF); // Reading DR clears RXNE
buffer[i] = USART2->DR&(0x1FF); // Reading DR clears RXNE

}
}

void GPIO_init(void){
//ENABLE CLOCK TO GPIOD
RCC->AHB1ENR |= (1<<3);

//ENABLING CLOCK OF USART 2 and 3
RCC->APB1ENR |= (3<<17);

//SETTING ALTERNATE FUNCTION
GPIOD->MODER &= ~(0x0F<<(2*5));
GPIOD->MODER |= (0x0A<<(2*5));
GPIOD->MODER &= ~(0x0F<<(2*8));
GPIOD->MODER |= (0x0A<<(2*8));

//SETTING ALTERNATE FUCTION
GPIOD->AFR[0] |= (0x77<<(4*5));
GPIOD->AFR[1] |= (0x77<<(4*0));
//GPIO SPEED HIGH
GPIOD->OSPEEDR |= (0xF<<(2*5));
GPIOD->OSPEEDR |= (0xF<<(2*8));
//SELECT PULL UP RESISTOR
GPIOD->PUPDR &= ~(0xF<<(2*5));
GPIOD->PUPDR |= (0x5<<(2*5));
GPIOD->PUPDR &= ~(0xF<<(2*8));
GPIOD->PUPDR |= (0x5<<(2*8));

//SELECT PUSH PULL
GPIOD->OTYPER &= ~(0x3<<5);
GPIOD->OTYPER &= ~(0x3<<8);
}

int main(void){

GPIO_init();
USART2_init();
configureLED();
volatile uint8_t *buffer_r;
volatile uint8_t data_r;

volatile uint32_t nBytes = (volatile uint32_t)0x01;
int sec = 1000;
while(1){
buffer_r = &data_r;

USART_read(buffer_r,nBytes);

if (data_r == 0x10){
GPIOD->ODR ^= (0xFUL<<12);
void msDelay(sec);

}
void msDelay(sec);

}
}

void msDelay(int msTime)
{
//Assume for loop take 12 clock cycles and system clock is 16MHz
int Time=msTime*1333;
for(int i=0;i<Time;i++);

}

void configureLED(void)

{

RCC->AHB1ENR |=(1UL<<3);//Enable GPIOD clock

GPIOD->MODER &= ~(0xFFUL<<12*2);

GPIOD->MODER |= (0x55UL<<12*2);

}
