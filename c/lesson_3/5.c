#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void insert_l(List*);

int main(){

    List l1 = NULL;
    List l2 = NULL;
    List l3 = NULL;

    insert_l(&l1);
    insert_l(&l2);
    intersec(l1,l2,&l3);

    //print_l(l1);
    //print_l(l2);
    print_l(l3);
    freeListRec(&l1);
    freeListRec(&l2);
    freeListRec(&l3);
    return 0;
}

void insert_l(List* l){
    int n;
    scanf("%d", &n);
    while (n > 0) {
        insertOrd(&(*l),n);
        scanf("%d", &n);
    }
    return;
}
