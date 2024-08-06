#include <stdio.h>
#include <stdlib.h>

int* in_arr(int);
int* merge(int*, int*, int, int);
void print_arr(int*, int);
int sorted(int*, int);

int main() {
    int n1, n2;

    scanf("%d", &n1);
    int* a = in_arr(n1);

    scanf("%d", &n2);
    int* b = in_arr(n2);

    int* c = merge(a, b, n1, n2);

    printf("Array A:\n");
    print_arr(a, n1);
    printf("Array B:\n");
    print_arr(b, n2);
    printf("Array risultato C\n");
    print_arr(c, (n1 + n2));

    printf("Ordinato: %d\n", sorted(c, (n1 + n2)));

    return 0;
}

int* in_arr(int n) {
    int* arr = (int*)calloc(n,sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    return arr;
}

int* merge(int* a, int* b, int n1, int n2) {
    int* c = (int*)calloc((n1 + n2),sizeof(int));
    int i =0, j=0,k=0;
    while(i<n1 && j<n2){
        if (a[i] <= b[j]){
            c[k] = a[i];
            i++;
            k++;
        } else{
            c[k] = b[j];
            j++;
            k++;
        }
    }
    while(i<n1){
        c[k] = a[i];
        i++;
        k++;
    }
    while(j<n2){
        c[k] = b[j];
        j++;
        k++;
    }
    return c;
}

int sorted(int* a, int n) {
    if (n <= 1)
        return 1;
    for (int i = 0; i < n - 1; i++) {
        if (a[i] > a[i + 1])
            return 0;
    }
    return 1;
}

void print_arr(int* a, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    return;
}