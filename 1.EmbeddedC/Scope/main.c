#include "stm32f4xx.h"
int myglobal;
void sub();
int foo();
int fool();
int main(void) {
int y;
y = foo();
y = foo();
y = foo();
fool();
sub();
while(1);
}
int foo() {

static int x = 5;
x = x + 1;
myglobal=myglobal+1;
return(x);
}

unsigned char x,z;   /* a regular global variable*/
void sub(void){
    x=1;
    {   unsigned char x;   /* a local variable*/
        x=2;
        {   unsigned char x;  /* a local variable*/
            x=3;
            z=x;}
        z=x;}
    z=x;}

