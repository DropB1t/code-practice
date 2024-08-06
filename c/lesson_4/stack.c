#include "stack.h"

#include "stdio.h"
#include "stdlib.h"

void print(Stack s) {
    Node* c = s;
    printf("Stack:");
    while (c != NULL) {
        printf("%f -> ", c->v);
        c = c->next;
    }
    printf("NULL\n");
    return;
}

void push(Stack* s, double x) {
    Node* new = (Node*)malloc(sizeof(Node));
    if (new == NULL) {
        printf("Errore di inserimento\n");
        exit(0);
    }
    new->v = x;
    new->next = *s;
    *s = new;
    return;
}

double top(Stack s) {
    if (s != NULL) {
        return s->v;
    }
    printf("La pila è vuota\n");
    return 0;
}

double pop(Stack* s) {
    if (*s == NULL) {
        printf("Non ci sono elementi da restituire\n");
        return 0;
    }
    Node* temp = *s;
    double ris = temp->v;
    *s = (*s)->next;
    free(temp);
    return ris;
}

void freeS(Stack* s) {
}