#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "list.h"

int main() {

    Stack s = NULL;
    print(s);
    push(&s, 5.4);
    print(s);
    push(&s, 420);
    print(s);

    printf("Elemento in cima è %f\n", top(s));
    print(s);

    printf("Elemento in cima è %f\n", pop(&s));
    print(s);

    List l = NULL;

    insertOrd(&l,69);
    insertOrd(&l,10);
    insertOrd(&l,5);
    insertOrd(&l,100);
    insertOrd(&l,80);
    insertOrd(&l,420);
    print_l(l);
    freeListRec(&l);
    print_l(l);
    return 0;
}