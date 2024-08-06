#include <stdio.h>
#include <stdlib.h>

enum symbols { o = 111,
               x = 120 };

void check_winner(char**, int, int);
void free_m(char**, int);

int main() {
    int r = 3, c = 3;
    char** m = (char**)malloc(r * sizeof(char*));
    if (m == NULL) {
        printf("Errore di memoria\n");
        exit(420);
    }
    for (int i = 0; i < r; i++) {
        m[i] = (char*)calloc(c, sizeof(char));
        for (int j = 0; j < c; j++) {
            printf("m[%d][%d]:",i,j);
            scanf(" %c", &m[i][j]);
        }
    }

    check_winner(m, r, c);
    free_m(m, r);
    return 0;
}

void check_winner(char** m, int r, int c) {
    char* result = "Pareggio\n";
    int sum = 0;
    for (int i = 0; i < r; i++) {
        sum += m[i][0] + m[i][1] + m[i][2];
        if (sum == 3 * o) {
            result = "Vince o\n";
        }
        if (sum == 3 * x) {
            result = "Vince x\n";
        }
        sum = 0;
    }
    for (int i = 0; i < r; i++) {
        sum += m[0][i] + m[1][i] + m[2][i];
        if (sum == 3 * o) {
            result = "Vince o\n";
        }
        if (sum == 3 * x) {
            result = "Vince x\n";
        }
        sum = 0;
    }
    sum += m[0][0] + m[1][1] + m[2][2];
    if (sum == 3 * o) {
        result = "Vince o\n";
    }
    if (sum == 3 * x) {
        result = "Vince x\n";
    }
    sum = 0;
    sum += m[0][2] + m[1][1] + m[2][0];
    if (sum == 3 * o) {
        result = "Vince o\n";
    }
    if (sum == 3 * x) {
        result = "Vince x\n";
    }
    sum = 0;
    printf("%s", result);
    return;
}

void free_m(char** m, int r) {
    for (int i = 0; i < r; i++) {
        free(m[i]);
    }
    free(m);
    return;
}