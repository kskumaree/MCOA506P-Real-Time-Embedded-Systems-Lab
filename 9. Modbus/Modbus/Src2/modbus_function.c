#include "stm32f4xx.h"
#include "usart.h"
#include "modbus_crc.h"
#include <stdint.h>
#include <stdio.h>

typedef struct
{
	uint8_t slaveAddress;
	uint8_t functionCode;
	uint16_t regAdd;
	uint16_t nreg;
}readRegister_msg;

typedef struct
{
	uint8_t slaveAddress;
	uint8_t functionCode;
	uint16_t regAdd;
	uint16_t data;
}writeRegister_msg;


void read_Register(readRegister_msg read,uint16_t msg_r[]){
	GPIO_init();
	USART2_init();
	USART3_init();
	uint32_t nBytes = 8;
	uint8_t msg[nBytes];
	msg[0] = read.slaveAddress;
	msg[1] = read.functionCode;
	msg[2] = (read.regAdd>>8)&(0x0FF);
	msg[3] = (read.regAdd)&(0x00FF);
	msg[4] = (read.nreg>>8)&(0x0FF);
	msg[5] = (read.nreg)&(0x00FF);
	uint16_t crc = crc16(msg, 6);
	uint8_t crcLow  = crc&0xFF;   // CRC LOW
	uint8_t crcHigh = (crc>>8)&0xFF;  // CRC HIGH
	msg[6] = crcLow;
	msg[7] = crcHigh;
	USART_write_array(msg,nBytes);
	uint32_t number = read.nreg*2 + 5;
	volatile uint8_t arr[number];
	USART_read_array(arr,number);
	int val = 0;
	for(int i=0;i<read.nreg*2;i=i+2){
		msg_r[val] = (arr[3+i]<<8)|(arr[3+i+1]);
		val++;
	}
}

void msg_value(readRegister_msg read,volatile uint8_t msg_r[]){
	GPIO_init();
	USART2_init();
	USART3_init();
	uint32_t nBytes = 8;
	uint8_t msg[nBytes];
	msg[0] = read.slaveAddress;
	msg[1] = read.functionCode;
	msg[2] = (read.regAdd>>8)&(0x0FF);
	msg[3] = (read.regAdd)&(0x00FF);
	msg[4] = (read.nreg>>8)&(0x0FF);
	msg[5] = (read.nreg)&(0x00FF);
	uint16_t crc = crc16(msg, 6);
	uint8_t crcLow  = crc&0xFF;   // CRC LOW
	uint8_t crcHigh = (crc>>8)&0xFF;  // CRC HIGH
	msg[6] = crcLow;
	msg[7] = crcHigh;
	USART_write_array(msg,nBytes);
	uint32_t number = read.nreg*2 + 5;
	USART_read_array(msg_r,number);

}


void read_bit(readRegister_msg read,int data[]){
	GPIO_init();
	USART2_init();
	USART3_init();
	uint32_t nBytes = 8;
	uint8_t msg[nBytes];
	msg[0] = read.slaveAddress;
	msg[1] = read.functionCode;
	msg[2] = (read.regAdd>>8)&(0x0FF);
	msg[3] = (read.regAdd)&(0x00FF);
	msg[4] = (read.nreg>>8)&(0x0FF);
	msg[5] = (read.nreg)&(0x00FF);
	uint16_t crc = crc16(msg, 6);
	uint8_t crcLow  = crc&0xFF;   // CRC LOW
	uint8_t crcHigh = (crc>>8)&0xFF;  // CRC HIGH
	msg[6] = crcLow;
	msg[7] = crcHigh;
	USART_write_array(msg,nBytes);
	volatile uint8_t arr[100];
	USART_read_array(arr,0x01);
	USART_read_array(arr,0x01);
	USART_read_array(arr,0x01);
	uint32_t number = arr[0];
	USART_read_array(arr,number+0x02);
	int indx = 0;
	for (int j=0; j<number; j++)
	{
		for (int i=0; i<8; i++)
		{
			data[indx++] = ((arr[0+j]>>i)&1);
		}
	}

}

void write_Register(writeRegister_msg write){
	GPIO_init();
	USART2_init();
	USART3_init();
	uint32_t nBytes = 8;
	uint8_t msg[nBytes];
	msg[0] = write.slaveAddress;
	msg[1] = write.functionCode;
	msg[2] = (write.regAdd>>8)&(0x0FF);
	msg[3] = (write.regAdd)&(0x00FF);
	msg[4] = (write.data>>8)&(0x0FF);
	msg[5] = (write.data)&(0x00FF);
	uint16_t crc = crc16(msg, 6);
	uint8_t crcLow  = crc&0xFF;   // CRC LOW
	uint8_t crcHigh = (crc>>8)&0xFF;  // CRC HIGH
	msg[6] = crcLow;
	msg[7] = crcHigh;
	USART_write_array(msg,nBytes);

}

void write_coil(writeRegister_msg write){
	GPIO_init();
	USART2_init();
	USART3_init();
	uint32_t nBytes = 8;
	uint8_t msg[nBytes];
	msg[0] = write.slaveAddress;
	msg[1] = write.functionCode;
	msg[2] = (write.regAdd>>8)&(0x0FF);
	msg[3] = (write.regAdd)&(0x00FF);
	msg[4] = (write.data>>8)&(0x0FF);
	msg[5] = (write.data)&(0x00FF);
	uint16_t crc = crc16(msg, 6);
	uint8_t crcLow  = crc&0xFF;   // CRC LOW
	uint8_t crcHigh = (crc>>8)&0xFF;  // CRC HIGH
	msg[6] = crcLow;
	msg[7] = crcHigh;
	USART_write_array(msg,nBytes);

}


uint32_t mfm384_read_val(readRegister_msg read){
	GPIO_init();
	USART2_init();
	USART3_init();
	uint32_t nBytes = 8;
	uint8_t msg[nBytes];
	msg[0] = read.slaveAddress;
	msg[1] = read.functionCode;
	msg[2] = (read.regAdd>>8)&(0x0FF);
	msg[3] = (read.regAdd)&(0x00FF);
	msg[4] = (read.nreg>>8)&(0x0FF);
	msg[5] = (read.nreg)&(0x00FF);
	uint16_t crc = crc16(msg, 6);
	uint8_t crcLow  = crc&0xFF;   // CRC LOW
	uint8_t crcHigh = (crc>>8)&0xFF;  // CRC HIGH
	msg[6] = crcLow;
	msg[7] = crcHigh;
	USART_write_array(msg,nBytes);
	uint32_t number = read.nreg*2 + 5;
	volatile uint8_t msg_r[number];
	USART_read_array(msg_r,number);
	uint32_t val = (msg_r[5]<<24)|(msg_r[6]<<16)|(msg_r[3]<<8)|(msg_r[4]);
	return val;

}



