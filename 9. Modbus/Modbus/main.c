#include "stm32f4xx.h"
#include "usart.h"
#include "modbus_crc.h"
#include <stdint.h>
#include <stdio.h>
#include "modbus_function.h"
#define ARM_MATH_CM4
volatile uint8_t msg_r[1000];
int data[1000];
int main(void){

	message_read.slaveAddress = 0x01;
	message_read.functionCode = 0x04;
	message_read.regAdd = 0x0C;
	message_read.nreg = 0x002;
	//read_bit(message_read,data);
	uint32_t number = message_read.nreg*2 + 5;
	//read_Register(message_read,msg_r);
	msg_value(message_read,msg_r);
	for(int i =0;i<number;i++){
		printf("the val is %d\n",msg_r[i]);
	}

}
