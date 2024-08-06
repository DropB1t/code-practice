#include <stdio.h>

#include "list.h"

void insert_l(List*);

int main() {
    int N, M;
    List l = NULL;
    scanf("%d", &N);
    scanf("%d", &M);
    insert_l(&l);

    printf("%d\n",prod_between(l,N,M));

    //print_l(l);

    return 0;
}

void insert_l(List* l) {
    int n;
    scanf("%d", &n);
    while (n >= 0) {
        insertOrd(&(*l), n);
        scanf("%d", &n);
    }
    return;
}