#include "stdio.h"

typedef enum { false, true } bool;

int main() {
    int n, prec = -1;
    bool flag = true;
    while (n >= 0) {
        while (scanf("%d", &n) != 1) {
            printf("Errore. Inserire un numero intero positivo oppure un numero intero negativo per terminare\n");
            scanf("%*[^\n]\n");
        }
        if (n < 0) {
            if (flag == true) {
                printf("Sequenza in ordine strettamente crescente\n");
            } else {
                printf("Sequenza non in ordine strettamente crescente\n");
            }
            return 0;
        }
        if (n <= prec) {
            flag = false;
        }
        prec = n;
    }
    return 0;
}
