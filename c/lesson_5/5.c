#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char s[100];
    int len;
} string;

int stringCmp(const void*, const void*);
void in_arr(string*, int);
void print_str(string*,int);

int main() {
    int n;
    scanf("%d", &n);

    string* arr = (string*)malloc(n * sizeof(string));
    in_arr(arr,n);

    qsort(arr,n,sizeof(string),stringCmp);

    print_str(arr,n);
    return 0;
}

int stringCmp(const void* s1, const void* s2) {
    string* a = (string*)s1;
    string* b = (string*)s2;
    
    if(a->len > b->len)
        return 1;
    else if(a->len < b->len)
        return -1;
    return strcmp(a->s, b->s);
}

void in_arr(string* arr, int n) {
    for (int i = 0; i < n; i++) {
        scanf("%s",arr[i].s);
        arr[i].len = strlen(arr[i].s);
    }
    return;
}

void print_str(string* arr,int n){
    for (int i = 0; i < n; i++) {
        printf("%s\n",arr[i].s);
        //printf("%s\t%d\n",arr[i].s,arr[i].len);
    }
    return;
}
