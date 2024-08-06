#include <stdio.h>

#include "list.h"

void insert_l(List*);

int main() {
    List l = NULL;

    insert_l(&l);

    if(is_ord(l) == true)
        printf("True\n");
    else
        printf("False\n");

    //print_l(l);
    freeListRec(&l);
    return 0;
}

void insert_l(List* l) {
    int n;
    scanf("%d", &n);
    while (n >= 0) {
        insertBottom(&(*l), n);
        scanf("%d", &n);
    }
    return;
}
