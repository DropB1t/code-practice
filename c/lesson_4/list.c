#include "list.h"

#include "stdio.h"
#include "stdlib.h"

void insertOrd(List* l, double x) {
    Node *prev, *succ;
    prev = NULL;
    succ = *l;
    while (succ != NULL && succ->v < x) {
        prev = succ;
        succ = succ->next;
    }
    Node* new = (Node*)malloc(sizeof(Node));
    if (new == NULL) {
        printf("Errore di inserimento\n");
        exit(0);
    }
    new->v = x;
    new->next = succ;
    if (prev != NULL)
        prev->next = new;
    else
        *l = new;
    return;
}

void print_l(List l) {
    Node* c = l;
    printf("List: ");
    while (c != NULL) {
        printf("%f -> ", c->v);
        c = c->next;
    }
    printf("NULL\n");
    return;
}

void freeListRec(List* l){
    if(*l == NULL)
        return;
    freeListRec(&((*l)->next));
    free(*l);
    *l = NULL;
}