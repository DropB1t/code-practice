#include <stdio.h>

#include "list.h"

void insert_l(List*);

int main() {
    int n;
    List l = NULL;
    insert_l(&l);
    scanf("%d", &n);

    del_n(&l, n);

    print_l(l);

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
