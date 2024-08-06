#include "stdio.h"

void stampa_cifre(int);
void stampa_cifre_inverso(int);

int main() {
    int n;

    while (scanf("%d", &n) != 1 || n < 0) {
        printf("Errore. Inserisci un numero intero positivo\n");
        scanf("%*[^\n]\n");
    }

    stampa_cifre(n);
    stampa_cifre_inverso(n);

    return 0;
}

void stampa_cifre(int n) {
    if(n == 0){
        return;
    }
    printf("%i\n",n%10);
    return stampa_cifre(n/10);
}

void stampa_cifre_inverso(int n) {
    if(n == 0){
        return;
    }
    stampa_cifre_inverso(n/10);
    printf("%i\n",n%10);
    return;
}