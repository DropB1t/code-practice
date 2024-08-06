#include <stdio.h>
#include <stdlib.h>

void in_matrix(int**, int, int);
void sort_col(int**, int, int);
void print_matrix(int**, int, int);

int main() {
    int r, c;
    scanf("%d %d", &r, &c);

    int** m = (int**)malloc(r * sizeof(int*));
    in_matrix(m, r, c);

    sort_col(m,r,c);

    print_matrix(m,r,c);

    return 0;
}

void sort_col(int** m, int r, int c) {
    int k = 0;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < r - i - 1; j++) {
            while(m[j][k]==m[j+1][k] && k<c)
                k++;
            if (m[j][k]>m[j+1][k]){
                int* temp = m[j];
                m[j] = m[j+1];
                m[j+1]=temp;
                k=0;
            }
        }
    }

    return;
}

void print_matrix(int** m, int r, int c) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
    return;
}

void in_matrix(int** m, int r, int c) {
    for (int i = 0; i < r; i++) {
        m[i] = (int*)malloc(c * sizeof(int));
        for (int j = 0; j < c; j++) {
            scanf("%d", &m[i][j]);
        }
    }
    return;
}
