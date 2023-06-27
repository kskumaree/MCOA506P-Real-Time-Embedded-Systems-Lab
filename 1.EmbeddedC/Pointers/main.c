#include "stm32f4xx.h"

long *pt;       // pointer to 32-bit data
long data;      // 32-bit
long buffer[4]; // array of 4 32-bit numbers
int main(void){
  pt = &buffer[1];
  *pt = 1234;
  data = *pt;
  return 1;
}
