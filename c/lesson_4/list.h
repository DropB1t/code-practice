struct N {
    double v;
    struct N* next;
};

typedef struct N Node;
typedef Node* List;

void insertOrd(List*, double);
void print_l(List);
void freeListRec(List*);