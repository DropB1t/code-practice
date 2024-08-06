#include "stdio.h"

void ordered_swap(int*, int*, int*);
void swap(int*, int*);

int main() {
    int a, b, c;
    scanf("%d", &a);
    scanf("%d", &b);
    scanf("%d", &c);
    ordered_swap(&a, &b, &c);
    printf("%d\n", a);
    printf("%d\n", b);
    printf("%d\n", c);
    return 0;
}

void ordered_swap(int* a, int* b, int* c) {
    if (*a > *b) {
        swap(a, b);
        if (*a > *c) {
            swap(a, c);
        }
    } else {
        if (*a > *c) {
            swap(a, c);
        }
        if (*b > *c) {
            swap(b, c);
        }
    }
    if (*b > *c) {
        swap(b, c);
    }
    return;
}

void swap(int* a, int* b) {
    int swap;
    swap = *a;
    *a = *b;
    *b = swap;
    return;
}