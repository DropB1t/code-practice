#include <stdio.h>
#include "stack.h"


int main(){

    Stack s;
    empty(&s);
    print(s);
    push(10,&s);
    print(s);

    return 0;
}