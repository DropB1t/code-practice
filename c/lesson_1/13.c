#include "stdio.h"

int Pot2(int);

int main() {
    int n;

    while (scanf("%d", &n) != 1 || n < 0) {
        printf("Errore. Inserisci un numero intero positivo\n");
        scanf("%*[^\n]\n");
    }

    printf("%d\n",Pot2(n));

    return 0;
}

int Pot2(int n) {
    if (n == 1) {
        return 2;
    }
    return 2 * Pot2(n - 1);
}