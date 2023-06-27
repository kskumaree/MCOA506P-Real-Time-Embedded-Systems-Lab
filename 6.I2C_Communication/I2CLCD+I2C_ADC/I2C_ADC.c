#include "stm32f4xx.h"

#define GPIOBEN   (1U<<1)
#define I2CEN	(1U<<21)
#define I2C_100KHZ	80	//0B 0101 0000 = Decimal = 80
#define SD_MODE_MAX_RISE_TIME	17
#define CR1_PE	(1U<<0)
#define SR2_BUSY	(1U<<1)
#define CR1_START	(1U<<8)
#define SR1_SB	(1U<<0)
#define SR1_ADDR	(1U<<1)
#define SR1_TXE	(1U<<7)
#define CR1_ACK	(1U<<10)
#define CR1_STOP (1U<<9)
#define SR1_RXNE (1U<<6)
#define SR1_BTF (1U<<2) //Byte Transfer Finish
#define S_ADD (0x40)
#define BUS_ADD (0x02)
#define MSB_CON (0x02)
#define LSB_CON (0x00)
//PB8 SCL
//PB9 SDA
// Alternate function 4
//uint16_t final=0;
//uint8_t msb1=0;
//uint8_t lsb1=2;

void I2C1_init(void);
float I2C1_voltage(uint8_t saddr, uint8_t maddr);
void I2C1_Write(char saddr, char maddr, uint8_t msb, uint8_t lsb);
float I2C1_current(uint8_t saddr, uint8_t maddr);
void delay(volatile int delayw);

void I2C1_init(void)
{
	//Enable clock access to i2c1
	RCC->APB1ENR |=I2CEN;
	//enable clock access to gpiob AHB1
	RCC->AHB1ENR |=GPIOBEN;
	//set PB8 AND PB9 to altenate function
	GPIOB->MODER |=(2UL<<12);//PB6 to alternate function pin
	GPIOB->MODER |=(2UL<<14);//PB7 to alternate function pin
	GPIOB->PUPDR |=(0x5UL<<12);//set PB6 and 7 as pull up pins
	GPIOB->OTYPER |=(0x3UL<<6);//Set PB6 and 7 as open drain
	GPIOB->OSPEEDR |=(0xAUL<<12);//Set PB6 and 7 as high speed
	GPIOB->AFR[0] |= (0x44<<24);//Set PB6 and 7 to alternate function 4
	//ENTER RESET MODE
	I2C1->CR1 |= (1U<<15);
	//Come out of reset
	I2C1->CR1 &= ~(1U<<15);
	//Set peripheral clock frequency

	I2C1->CR2 = (1U<<4); //16MHz
	//I2C1->OAR1 |=(1UL<<14);//Should be set high
	//Set I2C to standard mode, 100kHz clock
	I2C1->CCR |=(0x50UL<<0);
	//Set rise time
	I2C1->TRISE |=(0x17UL<<0);//Configure maximum rise time
	//Enable I2C1 module
	I2C1->CR1 |= CR1_PE;
}

float I2C1_voltage(uint8_t saddr, uint8_t maddr)
{
	uint16_t MSB=0;
	uint16_t LSB=0;
	uint16_t final=0;
	volatile int temp;
	//wait until bus not busy
	while(I2C1->SR2 &(SR2_BUSY)){}
	//I2C1->CR1 |= (1<<10);//Enable the ACK Bit
	//Generate the start
	I2C1->CR1 |=CR1_START;
	//Wait until start flag is set
	while(!(I2C1->SR1 & SR1_SB)){}
	//Trasnmit the slave address + write
	I2C1->DR = saddr<<1;
	//wait until addr flag is set
	while(!(I2C1->SR1 & (1U<<1))){}
	temp = I2C1->SR1|I2C1->SR2;
	//wait until transmitter empty
	while(!(I2C1->SR1 & SR1_TXE)){}
	//Send memory address
	I2C1->DR = maddr;
	//Wait until transmitter empty
	while(!(I2C1->SR1 & SR1_TXE)){}
	//Generate Stop
	I2C1->CR1 |= CR1_STOP;
	//Generate restart
	//I2C1->CR1 |= (1<<10);//Enable the ACK Bit
	I2C1->CR1 |=CR1_START;
	//Wait until start flag is set
	while(!(I2C1->SR1 & SR1_SB)){}
	//Transmit slave address + Read
	I2C1->DR = saddr<<1|1;
	//Wait until addr flag is set
	while(!(I2C1->SR1 & (SR1_ADDR))){}
	//clear addr flag;
	temp =  I2C1->SR1|I2C1->SR2;
	//Enable Acknowledge
	I2C1->CR1 |= CR1_ACK;
	//wait until RXNE flag is set
	while(!(I2C1->SR1 & SR1_RXNE)){}
	//read data from DR
	MSB = I2C1->DR;
	final=MSB;
	//Disable the Acknowledge
	I2C1->CR1 &= ~CR1_ACK;
	//Genearte Stop
	I2C1->CR1 |= CR1_STOP;
	//Wait for RXNE flag set
	while(!(I2C1->SR1 & SR1_RXNE)){}
	//READ DATA FROM DR
	LSB = I2C1->DR;
	final=(final<<8|(LSB));
	final=final>>3;
	float fr=0;
	fr=final*0.004;
	return fr;
}
void I2C1_Write(char saddr, char maddr, uint8_t msb, uint8_t lsb)
{
	volatile int tmp;
	//Wait until bus not busy
	while(I2C1->SR2 & (SR2_BUSY)){}
	//Generate start
	I2C1->CR1 |= CR1_START;
	//wait until start flag is set
	while(!(I2C1->SR1 & (SR1_SB))){}
	//transmit slave address
	I2C1->DR = saddr <<1;
	//wait until addr flag is set
	while(!(I2C1->SR1 &(SR1_ADDR))){}
	//clear addr flag
	tmp = I2C1->SR2;
	//wait until data register empty
	while(!(I2C1->SR1 &(SR1_TXE))){}
	//send memory address
	I2C1->DR = maddr;
	//wait until data register empty
	while(!(I2C1->SR1 &(SR1_TXE))){}
	//transmit memory address
	I2C1->DR = msb;
	//wait until data register empty
	while(!(I2C1->SR1 &(SR1_TXE))){}
	//transmit memory address
	I2C1->DR = lsb;
	//wait until transfer finished
	while(!(I2C1->SR1 &(SR1_BTF))){}
	//Generate Stop
	I2C1->CR1 |= CR1_STOP;
}
float I2C1_current(uint8_t saddr, uint8_t maddr)
{
	uint16_t MSB=0;
	uint16_t LSB=0;
	uint16_t final=0;
	volatile int temp;
	//wait until bus not busy
	while(I2C1->SR2 &(SR2_BUSY)){}
	//I2C1->CR1 |= (1<<10);//Enable the ACK Bit
	//Generate the start
	I2C1->CR1 |=CR1_START;
	//Wait until start flag is set
	while(!(I2C1->SR1 & SR1_SB)){}
	//Trasnmit the slave address + write
	I2C1->DR = saddr<<1;
	//wait until addr flag is set
	while(!(I2C1->SR1 & (1U<<1))){}
	temp = I2C1->SR1|I2C1->SR2;
	//wait until transmitter empty
	while(!(I2C1->SR1 & SR1_TXE)){}
	//Send memory address
	I2C1->DR = maddr;
	//Wait until transmitter empty
	while(!(I2C1->SR1 & SR1_TXE)){}
	//Generate Stop
	I2C1->CR1 |= CR1_STOP;
	//Generate restart
	//I2C1->CR1 |= (1<<10);//Enable the ACK Bit
	I2C1->CR1 |=CR1_START;
	//Wait until start flag is set
	while(!(I2C1->SR1 & SR1_SB)){}
	//Transmit slave address + Read
	I2C1->DR = saddr<<1|1;
	//Wait until addr flag is set
	while(!(I2C1->SR1 & (SR1_ADDR))){}
	//clear addr flag;
	temp =  I2C1->SR1|I2C1->SR2;
	//Enable Acknowledge
	I2C1->CR1 |= CR1_ACK;
	//wait until RXNE flag is set
	while(!(I2C1->SR1 & SR1_RXNE)){}
	//read data from DR
	MSB = I2C1->DR;
	final=MSB;
	//Disable the Acknowledge
	I2C1->CR1 &= ~CR1_ACK;
	//Genearte Stop
	I2C1->CR1 |= CR1_STOP;
	//Wait for RXNE flag set
	while(!(I2C1->SR1 & SR1_RXNE)){}
	//READ DATA FROM DR
	LSB = I2C1->DR;
	final=(final<<8|(LSB));
	float fr=0;
	fr=(final*0.8);
	return fr;
}
void delay(volatile int delayw)
{
	int time = 1333*delayw;
	for(int i=0;i<time;i++){}
}
float v_bus=0;
float i_bus=0;
int main(void)
{
	SCB->CPACR |= (0xF<<20);
	I2C1_init();
	I2C1_Write(S_ADD, (0x05), MSB_CON, LSB_CON);

    while(1)
    {
    	v_bus=I2C1_voltage(S_ADD, BUS_ADD);
    	delay(10);
    	i_bus=I2C1_current(S_ADD, (0x04));



    }
}
