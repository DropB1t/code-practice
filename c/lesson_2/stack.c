#include <stdio.h>

#define N 100

typedef struct{
    int elements[N];
    int n;
} Stack;

void push(int, Stack *);
int pop(Stack*);
int top(Stack);
void empty(Stack*);

void push(int v, Stack* s){
    s->elements[s->n] = v;
    s->n++;
    return;
}

int pop(Stack* s){
    int e = s->elements[s->n-1];
    s->n--;
    return e;
}

int top(Stack s){
    return s.elements[s.n-1];
}

void empty(Stack* s){
    s->n = 0;
}

void print(Stack s){
    printf("Stack with %d elements\n",s.n);
    if(s.n == 0){
        return;
    }
    for(int i=0; i<s.n; i++){
        printf("[%d]->%d ",i, s.elements[i]);
    }
    printf("\n");
}