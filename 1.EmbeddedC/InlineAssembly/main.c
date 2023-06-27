#include "stm32f4xx.h"
int main(void) {
  __asm volatile(
		"MOVS R0, #1\n\t"
		"MOVS R1, #2\n\t"
        "LDR R6, =#0x20001008\n\t" /* Load address 0x20001008 to R3 */
        "ADD R0, R1\n\t"           /* Add R0 to R1, save to R0 */
        "STR R0, [R6]\n\t"         /* Store R0 to the address pointing by R3 */
		"LDR R8, [R6]\n\t"
  );
}
