//LCD display using PCF8574A (Remote 8-bit I/O expander for I2C-bus with interrupt)
//Communication protocol used I2C
//Floating point number Display
#include "stm32f4xx.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

//Pin configuration G
// GND to GND of board
// VCC to 5V
// SCL to PB6
// SDA to PB7

unsigned int temp;
char a[10]={0}; // To store the digits before decimal
char b[10]={0}; // To store the digits after decimal
float k = 23.23456; // The floating point decimal.

int j;
float f;int q;
int n1 = 0; //To store the length of digits before decimal
int n2 = 0; //To store the length of digits after decimal
char w;char t;


void msDelay(uint32_t msTime); // To provide Delay in miliseconds.
void conv(char *a,char *b,float k);
void I2C1_init(void);
void I2C_start(void);
void I2C_address(void);
void I2C_stop(void);
void lcdcommand (char cmd);
void lcddata (char data);
void lcd_write_i2c(uint8_t *data, uint8_t length);
void configure_GPIOB(void);
void configure_I2C(void);
void configGPIO(void);
void Transmit(char * add, int l);
int lengtho(int k);


void msDelay(uint32_t msTime){							//function for software delay
	// "For loop" takes 4 clock cycles to get executed. Clock frequency is 16MHz
	// 16MHz/4=4MHz. If we want 1000ms (1second) delay,
	// 4MHz/1000=4000, so we have to multiply by 4000 to get a delay of 1s
	for (uint32_t i=0;i<msTime*4000;i++){
		__NOP();
	}
}
void configure_GPIOB(void){							//to configure the GPIO pins PB6 and PB7
	//SDA - PB7, SCL - PB6
	RCC -> AHB1ENR |=(1UL<<1);						//Enable clock for port B
	GPIOB -> MODER |=(2UL<<(6*2));					//PB6 to alternate function pin "10"
	GPIOB -> MODER |=(2UL<<(7*2));					//PB7 to alternate function pin "10"
	GPIOB -> PUPDR |=(0x5UL<<(6*2));				//set PB6 and PB7 as pull up pins "01"
	GPIOB -> OTYPER |=(0x3UL<<6);					//Set PB6 and PB7 as open drain "1"
	GPIOB -> OSPEEDR |=(0xAUL<<(6*2));				//Set PB6 and PB7 as high speed "10"
	GPIOB -> AFR[0] |= (0x44<<(6*4));				//Set PB6 and PB7 to alternate function 4 "0100"
}

void configure_I2C(void){							// To configure the I2C registers
	RCC -> APB1ENR |=(1UL<<21);						// Enable I2C clock
	I2C1 -> CR1 |= (1UL<<15);						// Reset I2C
	I2C1 -> CR1 &= ~(1UL<<15);						// set I2C
	I2C1 -> CR2 |=(16UL<<0);						// Set peripheral clock at 16MHz
	I2C1 -> OAR1 |=(1UL<<14);						// Should be set high
	I2C1 -> CCR |=(0x50UL<<0);						// Set SCL as 100KHz
	I2C1 -> TRISE |=(17UL<<0);						// Configure maximum rise time
	I2C1 -> CR1 |= (1UL<<0);						// Enable I2C
}

void I2C_start(void){								// To send the I2C start bit
	I2C1 -> CR1 |= (1<<10);							// Enable the ACK Bit
	I2C1 -> CR1 |= (1<<8);							// Send the start bit
	while (!(I2C1->SR1 & (1<<0)));						// Wait for SB bit to set
}
void I2C_address(void){								// To send the I2C address
	I2C1 -> DR = 0x7E;  				// Send the slave address      // change according to the device
	while (!(I2C1->SR1 & (1<<1))); 						// Wait for ADDR bit to set
	temp = ((I2C1->SR1) | (I2C1->SR2));					// Read SR1 and SR2 to clear the ADDR bit
}
void I2C_stop(void){								// To send the I2C stop bit
	I2C1 -> CR1 |= (1<<9);  						// Stop I2C
}

// It has two registers you can write in IR (Instruction register) Data register
// RS = 0 for the command
// RS = 1 for Data register

void lcdcommand (char cmd) //To send command to the LCD
{
  char upper_4, lower_4;
	uint8_t datac[4]; //total 8 bits divided into two 4 bits each for bit divided into 2 with en = 0/1
	upper_4 = (cmd&0xf0);
	lower_4 = ((cmd<<4)&0xf0);
	datac[0] =	upper_4|0x0C;  //en=1, rs=0 // taking enable from high to low
	datac[1] = upper_4|0x08;  //en=0, rs=0
	datac[2] = lower_4|0x0C;  //en=1, rs=0 // taking enable from high to low
	datac[3] =	lower_4|0x08;  //en=0, rs=0
	lcd_write_i2c((uint8_t *)datac,4);
}

void lcddata (char data) //To send the data to be displayed on the LCD
{
	char upper_4, lower_4;
	uint8_t data_d[4];  //total 8 bits divided into two 4 bits each for bit divided into 2 with en = 0/1
	upper_4 = (data&0xf0);
	lower_4 = ((data<<4)&0xf0);
	data_d[0] = upper_4|0x0D; //en=1,rs=1
	data_d[1] = upper_4|0x09; //en=0,rs=1
	data_d[2] = lower_4|0x0D; //en=1,rs=1
	data_d[3] = lower_4|0x09; //en=0,rs=1
	lcd_write_i2c((uint8_t *)data_d,4);
}

void lcd_write_i2c(uint8_t *data, uint8_t length) // Write function I2C
{
	I2C_start();
	I2C_address();
	//sending the data
	while (!(I2C1->SR1 & (1<<7))); //Wait till the data register is empty TxE
	for (uint8_t i=0;i<length;i++)
	{
		I2C1->DR=data[i];
		while (!(I2C1->SR1 & (1<<2))); // Wait till the byte transfer is finished
	}
	I2C_stop();
}
void LCD_init(void)
{
		// Initial commands to set up the LCD in 4 bit mode
		lcdcommand(0x30);							//to clear the screen

		lcdcommand(0x30);							//to clear the screen

		lcdcommand(0x30);							//to clear the screen

		lcdcommand(0x20);							//to turn on the screen

		// Set LCD to 4 bit mode, 2 lines, 5x8 font
		lcdcommand(0x28);

		// Turn the display on with blinking cursor
		lcdcommand(0x0C);

		// Clear the contents on the screen
		lcdcommand(0x01);
		msDelay(2); // 1.57ms time required.
		// Set LCD to write from left to right
		lcdcommand(0x06);
}

void Transmit(char * add, int l) // Transmit data, First parameter Character array send length of the data to transmit
{
	char data;
	for(int i=0;i<l;i++)
	{
	data=add[i];
	lcddata(data);
	msDelay(1);
		}
}
int lengtho(int x) //To calculate the length of the digits.
{
	int count = 0;
	while(x>0)
	{
		count++;
		x=x/10;
	}
	return count; //returning the length
}

void conv(char *a,char *b,float k) // Converting the floating point number to character array

{
	GPIOD->ODR |=(0x1UL<<13); // set led high to indicate conversion start
	// Digits before the decimal
	j = (int)k;
	n1 = lengtho(j); //length of the digits before the decimal
	int g;int j2;
	for(g=0;j>0;g++)
	{
		a[n1-g-1] = j%10 +'0';
		j=j/10;
	}
	a[n1] = '\0';
	//digits before the decimal stored inside a[]

	//Digits after the decimal
	j=(int)k;
	f = k-j;
	q = (f*1000); // multiply by 100 to get to 2 decimal place and by 1000 to get upto 3 decimal place
	n2 = lengtho(q); //length of the digits after decimal
	int o;int e;
	for(o=0;q>0;o++)
	{
	b[n2-o-1] = q%10 +'0';
	q=q/10;
	}
	b[n2]='\0';
	//digits stored inside b[]

}
void configGPIO(void) //configuring GPIO pins for LED
{
	RCC->AHB1ENR |=(1UL<<3);
	GPIOD->MODER &= ~(0xFFUL<<12*2); //GPIOD 12,13,14,15 in output mode
	GPIOD->MODER |= (0x55UL<<12*2);
}

int main(void)
{
		//enable FPU
		SCB->CPACR |= (0xF<<20);
		//configuring GPIOB for I2C communication
		configure_GPIOB();
		//configuring GPIOD for LED
		configGPIO();
		//Configure I2C
		configure_I2C();
		msDelay(1);
		//Initializing the LCD
		LCD_init();
		//Converting float input into character array
		conv(a,b,k);
		//Concatenating the strings
		strcat(a,".");
		strcat(a,b);
		Transmit(a,(n1+n2+1));
		while (1)
		{

		}
}
