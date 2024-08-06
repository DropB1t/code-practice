#include "stdio.h"

void print_product(int[][100], int, int);
void pip(int[][100], int, int);

int main(int argc, char const *argv[]) {
    int r, c;
    scanf("%d", &r);
    scanf("%d", &c);
    int a[100][100];
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    pip(a, r, c);
    printf("\n");
    print_product(a, r, c);
    return 0;
}

void print_product(int a[][100], int r, int c) {
    int sum;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < r; j++) {
            sum = 0;
            for (int k = 0; k < c; k++) {
                sum += a[i][k] * a[j][k];
            }
            printf("%d ", sum);
        }
        printf("\n");
    }
    return;
}

void pip(int a[][100], int r, int c) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    return;
}