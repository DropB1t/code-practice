struct N {
    double v;
    struct N* next;
};

typedef struct N Node;
typedef Node* Stack;

void push(Stack*, double);
double pop(Stack*);
double top(Stack);

void freeS(Stack*);
void print(Stack);