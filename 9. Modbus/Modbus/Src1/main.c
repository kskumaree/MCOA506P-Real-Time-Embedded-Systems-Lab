#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "modbus_function.h"
#include "ieee754.h"
#define ARM_MATH_CM4
uint8_t msg_r[1000];
int data[1000];
int main(void){
while(1){
	message_read.slaveAddress = 0x02;
	message_read.functionCode = 0x04;
	message_read.regAdd = 0x00;
	message_read.nreg = 0x002;
	uint32_t number = message_read.nreg*2 + 5;
	msg_value(message_read,msg_r);
	uint32_t val = (msg_r[5]<<24)|(msg_r[6]<<16)|(msg_r[3]<<8)|(msg_r[4]);
	float value = ieee754_to_float(val);
	printf("the voltage is %f\n",value);

	msDelay(2000);


	message_read.slaveAddress = 0x01;
	message_read.functionCode = 0x04;
	message_read.regAdd = 0x0c;
	message_read.nreg = 0x002;
	uint32_t number1 = message_read.nreg*2 + 5;
	msg_value(message_read,msg_r);
	uint32_t val1 = (msg_r[5]<<24)|(msg_r[6]<<16)|(msg_r[3]<<8)|(msg_r[4]);
	float value1 = ieee754_to_float(val1);
	printf("the frequency is %f\n",value1);

}

}
