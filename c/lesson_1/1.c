#include <stdio.h>

#define N 10

int verifica_condizione(int[]);

int main(int argc, char const* argv[]) {
    int a[N];
    for (int i = 0; i < N; i++) {
        while (scanf("%d", &a[i]) != 1) {
            printf("Inserire un intero valido\n");
            scanf("%*[^\n]\n");
        }
    }
    printf("%d\n", verifica_condizione(a));
    return 0;
}

int verifica_condizione(int a[]) {
    for (int i = 1; i < N - 1; i++) {
        if (a[i] == a[i + 1] - a[i - 1]) {
            return i;
        }
    }
    return -1;
}
