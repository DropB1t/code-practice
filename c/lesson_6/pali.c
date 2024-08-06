#include <stdio.h>
#include <string.h>

#define N 3

int pali();

int main() {
    /*char* s;
    scanf("%s", s); */

    if (pali())
        printf("palindrome\n");
    else
        printf("non palindrome\n");

    return 0;
}

int pali() {
    char c1,c2;
    int res;
    scanf("%c",&c1);
    if(c1 == '*')
        return 1;
    else{
        res = pali();
        scanf("%c",&c2);
        if(c1!=c2)
            return 0;
        else
            return res;
    }
    return 1;
}