#include <stdio.h>
#include <stdlib.h>

int** in_m(int);
void restore_d(int**,int);
void print_matrix(int**,int);

int main() {
    int R;
    scanf("%d", &R);

    int** m = in_m(R);
    restore_d(m,R);

    print_matrix(m,R);

    return 0;
}

int** in_m(int R){
    int** m = (int**)malloc(R * sizeof(int*));
    for (int i = 0; i < R; i++) {
        m[i] = (int*)calloc(R, sizeof(int));
        for (int j = 0; j < R; j++) {
            scanf("%d", &m[i][j]);
        }
    }
    return m;
}

void restore_d(int** m,int R){
    int i=0;
    while(i<R){
        m[i][i]=0;
        m[i][R-1-i]=0;
        i++;
    }
    return;
}

void print_matrix(int**m,int R){
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < R; j++) {
            printf("%d ",m[i][j]);
        }
        printf("\n");
    }
    return;
}