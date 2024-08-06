#include "list.h"

#include "stdio.h"
#include "stdlib.h"

void in_list(List* l, int v) {
    Node* new = (Node*)malloc(sizeof(Node));
    new->v = v;
    new->next = *l;
    *l = new;
    return;
}

void insertBottom(List* l, int x) {
    Node *prev, *succ;
    prev = NULL;
    succ = *l;
    while (succ != NULL) {
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
    if (*l == NULL)
        *l = new;
    return;
}

void insertTop(List* l, int x) {
    Node* new = (Node*)malloc(sizeof(Node));
    if (new == NULL) {
        printf("Errore di inserimento\n");
        exit(0);
    }
    new->v = x;
    new->next = *l;
    *l = new;
    return;
}

void insertOrd(List* l, int x) {
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

void insertOrd_inverse(List* l, int x) {
    Node *prev, *succ;
    prev = NULL;
    succ = *l;
    while (succ != NULL && succ->v > x) {
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

boolean is_ord(List l){
    Node* c = l;

    while (c != NULL) {
        if (c->next != NULL && c->v >= c->next->v) {
            return false;
        }
        c = c->next;
    }

    return true;
}

void fill_missing(List* l){
    if(*l == NULL)
        return;
    Node *prev, *succ;
    int flag = 0;
    prev = *l;
    succ = (*l)->next;
    while (succ != NULL) {
        if((prev->v-1)>succ->v){
            Node* new = (Node*)malloc(sizeof(Node));
            if (new == NULL) {
                printf("Errore di inserimento\n");
                exit(0);
            }
            new->v = prev->v-1;
            new->next = succ;
            prev->next = new;
            flag = 1;
        }
        if(flag == 1){
            prev = prev->next;
            flag = 0;
        }else{
            prev = succ;
            succ = succ->next;
        }
    }
    return;
}

void infront_even(List* l){
    if(*l == NULL)
        return;
    if((*l)->v%2 == 0){
        Node* new = (Node*)malloc(sizeof(Node));
        if (new == NULL) {
            printf("Errore di inserimento\n");
            exit(0);
        }
        Node* mem = *l;
        new->v = -1;
        new->next = *l;
        *l = new;
        return infront_even(&mem->next);
    }
    return infront_even(&(*l)->next);
}

void in_after_four(List* l, int v) {
    Node *c = *l, *prev = NULL;
    for (int i = 0; i < 4; i++) {
        if (c == NULL) {
            break;
        }
        prev = c;
        c = c->next;
    }
    Node* new = (Node*)malloc(sizeof(Node));
    if (new == NULL) {
        printf("Errore di inserimento\n");
        exit(0);
    }
    new->v = v;
    new->next = c;
    if (prev != NULL) {
        prev->next = new;
    } else {
        *l = new;
    }
    return;
}

void del_val(List* l, int v) {
    Node* curr = *l;
    Node* prev = NULL;

    while (curr != NULL && curr->v != v) {
        prev = curr;
        curr = curr->next;
    }
    if (curr != NULL && curr->v == v) {
        curr = curr->next;
        if (prev != NULL) {
            prev->next = curr;
        } else {
            *l = curr;
        }
    }
    return;
}

void del_n(List* l, int n) {
    for (int i = 0; i < n; i++) {
        if (*l == NULL) {
            return;
        }
        Node* c = *l;
        *l = (*l)->next;
        free(c);
    }
}

void del_multiple(List* l, int num){
    Node* curr = *l;
    Node* prev = NULL;

    while (curr != NULL) {
        if(curr->v % num == 0){
            curr = curr->next;
            if (prev != NULL) {
                prev->next = curr;
            } else {
                *l = curr;
            }
            continue;
        }
        prev = curr;
        curr = curr->next;
    }
    return;
}

int check_val(List l, int v) {
    Node* c = l;
    while (c != NULL) {
        if (c->v == v) {
            return 1;
        }
        c = c->next;
    }
    return -1;
}

void intersec(List l1, List l2, List* l) {
    if (lenght(l1) < lenght(l2)) {
        Node* temp = l1;
        l1 = l2;
        l2 = temp;
    }
    Node* c = l1;

    while (c != NULL) {
        if (check_val(l2, c->v) == 1 && check_val(*l, c->v) == -1) {
            insertOrd(&(*l), c->v);
        }
        c = c->next;
    }
    return;
}

void join_l(List l1,List l2,List* l3){
    Node* c1 = l1;
    while (c1 != NULL) {
        insertOrd(&(*l3), c1->v);
        c1 = c1->next;
    }
    free(c1);
    Node* c2 = l2;
    while (c2 != NULL) {
        insertOrd(&(*l3), c2->v);
        c2 = c2->next;
    }
    free(c2);
    return;
}

int lenght(List l) {
    Node* c = l;
    int cout = 0;
    while (c != NULL) {
        cout++;
        c = c->next;
    }
    return cout;
}

int calcoloSomma(List l) {
    Node* c = l;
    int sum = 0;
    while (c != NULL) {
        sum += c->v;
        c = c->next;
    }
    return sum;
}

int calcoloNElementi(List l) {
    Node* c = l;
    int count = 0, sumforth = calcoloSomma(l) / 4;
    while (c != NULL) {
        if (c->v > sumforth) {
            count++;
        }
        c = c->next;
    }
    return count;
}

int prod_between(List l, int N, int M) {
    if(l == NULL)
        return -1;
    Node* c = l;
    int prod = 1;
    while (c != NULL) {
        if (c->v > N && c->v < M) {
            prod *= c->v;
        }
        c = c->next;
    }
    return prod;
}

int even_v(List l){
    if(l == NULL)
        return -1;
    if(l->v%2 == 0)
        return l->v;
    even_v(l->next);
}

int odd_v(List l){
    if(l == NULL)
        return -1;
    if(l->v%2 != 0)
        return l->v;
    even_v(l->next);
}

void print_l(List l) {
    Node* c = l;
    printf("List: ");
    while (c != NULL) {
        printf("%d -> ", c->v);
        c = c->next;
    }
    printf("NULL\n");
    return;
}

void print_rawL(List l) {
    Node* c = l;
    while (c != NULL) {
        printf("%d\n", c->v);
        c = c->next;
    }
    return;
}

void print_rec(List* l){
    if(*l == NULL){
        printf("NULL\n");
        return;
    }
    printf("%d->",(*l)->v);
    print_rec(&(*l)->next);
    return;
}

void print_inverse_rec(List* l){
    if(*l == NULL){
        return;
    }
    print_inverse_rec(&(*l)->next);
    printf("%d->",(*l)->v);
    return;
}

void freeListRec(List* l) {
    if (*l == NULL)
        return;
    freeListRec(&((*l)->next));
    free(*l);
    *l = NULL;
}