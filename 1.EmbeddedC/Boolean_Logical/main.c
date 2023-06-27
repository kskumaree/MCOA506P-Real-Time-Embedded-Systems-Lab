#include "stm32f4xx.h"

short a,b,c,d,e;
int main(void)
{
	a=0x0F0F; b=0x0;
  c = a&b;  /* logical result c will be 0x0000 */
  d = a&&b; /* Boolean result d will be 0 (False) */
  e = a||b; /* Boolean result e will be 1 (true) */
  return 1;
}
