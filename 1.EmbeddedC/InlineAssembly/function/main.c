#include "stm32f4xx.h"

int sum4(int a, int b, int c, int d)
{
//arguments stored in r0, rl, r2, r3
	__asm volatile(
			"PUSH {r4, lr}\n\t" //preserve environment i.n stack
			"MOV r4, r0\n\t" // r0 = 1st argument
			"ADD r4, r4, r1\n\t" //r1 = 211d argument
			"ADD r4, r4, r2\n\t" //r2 = 3rd argument
			"ADD r0, r4, r3\n\t" //r3 = 4th argument, re = return
			"POP {r4, PC}\n\t"
);
}
int main (void)
{
int s = sum4(1, 2, 3, 4);
while(1);
}

