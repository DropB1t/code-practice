#include <stdio.h>

#include "list.h"

void insert_l(List*);

int main() {
    List l = NULL;

    insert_l(&l);

    infront_even(&l);

    print_l(l);
    freeListRec(&l);
    return 0;
}

void insert_l(List* l) {
    int n;
    scanf("%d", &n);
    while (n >= 0) {
        in_list(&(*l), n);
        scanf("%d", &n);
    }
    return;
}
