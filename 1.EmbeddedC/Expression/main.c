#include "stm32f4xx.h"

int y;       // pointer to 32-bit data
int x=2;      // 32-bit

int main(void){
	y = 254*(x/100);
}
