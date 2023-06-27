#include <stdio.h>
#include <math.h>
#include "ieee754.h"

typedef union{
	float f;
	struct
	{
		unsigned int mantissa :23;
		unsigned int exponent :8;
		unsigned int sign :1;
	}raw;
} myFloat;


//function to convert a binary array to corresponding integer
unsigned int convertToInt(unsigned int arr[],int low, int high){
	unsigned int f = 0,i;
	for(i=high;i >= low; i--){
		f = f + arr[i]*pow(2,high - i);
	}
	return f;
}

void int_to_bin(int n,unsigned int val[]){
	int i,a[32];
	for(i=0;n!=0;i++){
		a[i] = n%2;
		n = n/2;
	}
	n = i;

	for(int i=0;i<n;i++){
		val[32-i-1] = a[i];
		//printf("(%d,%d)",a[i],val[i]);
	}

}


float ieee754_to_float(int value){
	unsigned int a[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int_to_bin(value,a);
	myFloat var;
	unsigned f = convertToInt(a,9,31);
	var.raw.mantissa = f;

	f = convertToInt(a,1,8);
	var.raw.exponent = f;

	var.raw.sign = a[0];

	return var.f;
}
