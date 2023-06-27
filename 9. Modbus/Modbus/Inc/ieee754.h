/*
 * ieee754.h
 *
 *  Created on: 05-Mar-2023
 *      Author: user
 */

#ifndef IEEE754_H_
#define IEEE754_H_



unsigned int convertToInt(unsigned int arr[],int low, int high);
void int_to_bin(int n,unsigned int val[]);

float ieee754_to_float(int value);
#endif /* IEEE754_H_ */
