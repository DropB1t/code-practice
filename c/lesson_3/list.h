struct N {
    int v;
    struct N* next;
};

typedef struct N Node;
typedef Node* List;
typedef enum { false,
               true } boolean;

void in_list(List*, int);
void in_after_four(List*, int);
void insertOrd(List*, int);
void insertOrd_inverse(List*, int);
void insertTop(List*, int);
void insertBottom(List*, int);

boolean is_ord(List);

int lenght(List);
int check_val(List, int);
void del_val(List*, int);
void del_n(List*, int);
void del_multiple(List*,int);
void intersec(List, List, List*);
int calcoloSomma(List);
int calcoloNElementi(List);
int prod_between(List, int, int);
void join_l(List, List, List*);
void fill_missing(List*);
void infront_even(List*);
int even_v(List);
int odd_v(List);


void print_l(List);
void print_rawL(List);
void print_rec(List*);
void print_inverse_rec(List*);

void freeListRec(List*);