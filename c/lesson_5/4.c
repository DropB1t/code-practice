#include <stdio.h>
#include <stdlib.h>

void in_arr(int*, int);
void print_arr(int*, int);
int compare(const void*,const void*);

int main() {
    int n;
    scanf("%d", &n);
    int* a = (int*)malloc(n * sizeof(int));
    in_arr(a,n);

    qsort(a,n,sizeof(int),compare);
    
    print_arr(a,n);
    free(a);
    return 0;
}

int compare(const void* a,const void* b){
    int v1=*(int*)a;
    int v2=*(int*)b;

    if(v1%2==0 && v2%2!=0)
        return -1;
    if(v1%2!=0 && v2%2==0)
        return 1;
    if(v1%2==0 && v2%2==0 && v1>v2)
        return 1;
    else if(v1%2==0 && v2%2==0 && v1<v2)
        return -1;
    else if(v1%2!=0 && v2%2!=0 && v1<v2)
        return 1;
    else if(v1%2!=0 && v2%2!=0 && v1>v2)
        return -1;

    return 0;
}

void print_arr(int* a,int n){
    printf("Sorted:\n");
    for (int i = 0; i < n; i++) {
        printf("%d\n",a[i]);
    }
    return;
}

void in_arr(int* a, int n) {
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }
    return;
}
