#define N 100

typedef struct{
    int elements[100];
    int n;
} Stack;

void push(int, Stack*);
int pop(Stack*);
int top(Stack);
void empty(Stack*);
void print(Stack s);