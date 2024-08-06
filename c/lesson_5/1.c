#include <stdio.h>
#include <string.h>

#define N 3

int pali(char*, char*);

int main() {
    int r[3];
    for (int i = 0; i < N; i++) {
        char* s;
        scanf("%s", s);

        char* s1 = strtok(s, "*");
        char* s2 = strtok(NULL, "*");
        r[i] = pali(s1, s2);
    }
    for (int i = 0; i < N; i++) {
        if (r[i] == 1)
            printf("palindrome\n");
        else
            printf("non palindrome\n");
    }

    return 0;
}

int pali(char* s1, char* s2) {
    if (s1 == NULL || s2 == NULL)
        return 1;
    int n1 = strlen(s1);
    int n2 = strlen(s2);
    if (n1 != n2)
        return 0;
    for (int i = 0; i < n1; i++) {
        if (s1[i] != s2[n1 - 1 - i])
            return 0;
    }
    return 1;
}
