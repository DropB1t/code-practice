#include <stdio.h>

void primoultimopari(int[], int, int*, int*);

int main() {
    int n = 7, primaocc, ultimaocc;
    int arr[n];

    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    primoultimopari(arr, n, &primaocc, &ultimaocc);

    printf("%d %d\n", primaocc, ultimaocc);
    return 0;
}

void primoultimopari(int arr[], int dim, int* primaocc, int* ultimaocc) {
    *primaocc = -1;
    *ultimaocc = -1;

    for (int i = 0; i < dim; i++) {
        if (arr[i] % 2 == 0 && *primaocc == -1) {
            *primaocc = i;
            *ultimaocc = i;
        } else if (arr[i] % 2 == 0 && *primaocc != -1) {
            *ultimaocc = i;
        }
    }

    return;
}
