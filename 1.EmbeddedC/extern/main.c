#include "stm32f4xx.h"
int myglobal;
int foo();
int fool();
int main(void) {
int y;
y = foo();
y = foo();
y = foo();
fool();
while(1);
}
int foo() {

static int x = 5;
x = x + 1;
myglobal=myglobal+1;
return(x);
}
