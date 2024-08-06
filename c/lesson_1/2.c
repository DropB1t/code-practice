#include <stdio.h>

#define N 10

int min_azzera(int[]);
void print(int[]);

int main(int argc, char const* argv[]) {
    int a[N];
    for (int i = 0; i < N; i++) {
        while (scanf("%d", &a[i]) != 1) {
            printf("Inserire un intero valido\n");
            scanf("%*[^\n]\n");
        }
    }
    int min = min_azzera(a);
    print(a);
    printf("%d",min);
    return 0;
}

void print(int a[]) {
    for (int i = 0; i < N; i++) {
        printf("%d\n", a[i]);
    }
}

int min_azzera(int a[]) {
    int min = a[0];
    for (int i = 1; i < N; i++) {
        if (a[i] < min) {
            min = a[i];
        }
        if (i % 2 != 0) {
            a[i] = 0;
        }
    }
    return min;
}
