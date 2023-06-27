#include "stm32f4xx.h"
uint32_t combine(uint8_t ,uint8_t );   /* a regular global variable*/
void main(void){
	uint32_t a;
	uint8_t c=1,d=2;
	a=combine(1,2);
}


uint32_t combine(uint8_t msb,uint8_t lsb)
{
return msb<<8 + lsb;
}
