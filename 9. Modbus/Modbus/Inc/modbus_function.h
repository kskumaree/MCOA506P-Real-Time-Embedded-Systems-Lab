/*
 * modbus_function.h
 *
 *  Created on: Feb 18, 2023
 *      Author: user
 */

#ifndef MODBUS_FUNCTION_H_
#define MODBUS_FUNCTION_H_

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

void read_Register(readRegister_msg,volatile uint16_t msg_r[]);  //fc = 3
void read_bit(readRegister_msg read,int data[]); //fc = 1
void write_Register(writeRegister_msg write); //fc = 6
void write_coil(writeRegister_msg write); //fc = 5
uint32_t mfm384_read_val(readRegister_msg read);
void msg_value(readRegister_msg read,uint8_t msg_r[]);

readRegister_msg message_read;
writeRegister_msg message_write;
#endif /* MODBUS_FUNCTION_H_ */
