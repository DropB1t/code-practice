#include <stdio.h>
#include <stdlib.h>

#include "list.h"

int main() {
    int n;
    scanf("%d", &n);
    List l = NULL;
    while (n != 0) {
        
        if(n >= 0 && n%2==0){
            insertTop(&l,n);
        }else if(n >= 0 && n%2!=0){
            insertBottom(&l,n);
        }else if(n<0){
            del_val(&l,(-n));
            
        }
        scanf("%d", &n);
    }
    print_l(l);
    freeListRec(&l);
    return 0;
}
