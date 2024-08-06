#include <stdio.h>

#include "list.h"

void insert_l(List*);

int main() {
    List l = NULL;

    insert_l(&l);
    printf("Stampa l'elenco in ordine inverso di immissione (NULL se vuota):\n");
    print_inverse_rec(&l);
    printf("NULL\n");

    int num;
    printf("Inserisci il numero richiesto:\n");
    scanf("%d", &num);
    del_multiple(&l,num);

    printf("Stampa l'elenco in ordine di immissione (NULL se vuota):\n");
    print_rec(&l);

    printf("Dispari:%d\n",odd_v(l));
    printf("Pari:%d\n",even_v(l));

    //print_l(l);
    freeListRec(&l);
    return 0;
}

void insert_l(List* l) {
    int n;
    do {
        while (scanf("%d", &n) != 1 || n < 0) {
            printf("Inserisci intero positivo o 0 per uscire\n");
            scanf("%*[^\n]\n");
        }
        if(n != 0)
            insertBottom(&(*l), n);
    } while (n != 0);
    return;
}