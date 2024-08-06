struct N{
  double v;
  struct N * next;
};
typedef struct N NodeS;
typedef NodeS * Stack; 


void push(Stack* , double );
double pop (Stack* );
double top (Stack );
void freeS(Stack* );
void print(Stack);
