#include <stdio.h>
#include "stdlib.h"

struct N{
    int v;
    struct N* sx;
    struct N* dx;
};

typedef struct N Node;
typedef Node* ABR;

void in_abr(ABR* abr, int v){
    if((*abr)==NULL){
        Node* new = (Node*)malloc(sizeof(Node));
        new->v = v;
        new->sx = NULL;
        new->dx = NULL;
        *abr = new;
        return;
    }
    if((*abr)->v > v)
        in_abr(&((*abr)->sx),v);
    else
        in_abr(&((*abr)->dx),v);
    return;
}

void print_ant(ABR abr){
    if((abr)==NULL)
        return;
    printf(" (%d ",abr->v);
    print_ant(abr->sx);
    print_ant(abr->dx);
    printf(") ");
}

int main(){
    
    ABR abr = NULL;
    in_abr(&abr,10);
    in_abr(&abr,5);
    in_abr(&abr,12);
    in_abr(&abr,1);
    in_abr(&abr,2);
    in_abr(&abr,4);
    in_abr(&abr,20);
    in_abr(&abr,8);
    in_abr(&abr,14);
    in_abr(&abr,16);

    print_ant(abr);

   return 0;
}