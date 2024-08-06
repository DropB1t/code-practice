#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 1000
#define NMENU 4

void insert(char*);
void en2piglatin(char*);
void fullapper(char*);
void quit(char*);

int main() {
    int opt;
    char* s = (char*)malloc(N * sizeof(char));
    s[0] = 0;
    void (*menu[NMENU])(char* s) = {insert,fullapper, en2piglatin, quit};

    while (1) {
        printf("Scegliere l'operazione:");
        while (scanf("%d", &opt) != 1 || opt > 4 || opt < 1){
            printf("Scegliere l'operazione:");
            scanf("%*[^\n]\n");
        }
        scanf("%*c");
        (*menu[opt - 1])(s);
    }

    return 0;
}

void insert(char* s) {
    printf("Inserire stringa:");
    scanf("%[^\n]", s);
    s = (char*)realloc(s, strlen(s) * sizeof(char));
    //printf("%s - %d", s, strlen(s));
    return;
}

void fullapper(char* s) {
    int len = strlen(s);
    if (len == 0) {
        return;
    }
    //printf("%s - %d\n",s,len);
    char* u = (char*)malloc(2*len * sizeof(char));
    u[0]='\0';
    for (int i = 0; i < len; i++) {
        u[i] = (char)toupper(s[i]);
    }
    u[len]='\0';
    printf("%s\n", u);
    free(u);
    return;
}

void en2piglatin(char* s) {
    int len = strlen(s);
    if (len == 0) {
        return;
    }
    char* copy = (char*)malloc(len * sizeof(char));
    copy[0]='\0';
    char* result = (char*)malloc(N * sizeof(char));
    result[0]='\0';
    copy = strcpy(copy, s);

    /* while ((tok = strtok(NULL, " ")) != NULL) {
    } */
    char* tok = strtok(copy, " ");
    do {
        if (strlen(tok) == 0) {
            continue;
        }
        char* w = (char*)malloc(2*strlen(tok)*sizeof(char));
        w[0]='\0';
        strcpy(w,tok);
        int up = isupper(w[0]);
        switch (tolower(w[0])) {
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
                strcat(w,"way ");
                break;
            default:
                do {
                    char ch = (char)tolower(w[0]);
                    strncat(w,&ch,1);
                    w = &w[1];
                } while (w[0]!='\0'&&w[0]!='a'&&w[0]!='e'&&w[0]!='i'&&w[0]!='o'&&w[0]!='u'&&w[0]!='y');
                strcat(w,"ay ");
                break;
        }
        if(up!=0)
            w[0]=toupper(w[0]);
        strcat(result,w);
        
    } while ((tok = strtok(NULL, " ")) != NULL);
    result[strlen(result)-1]='\0';
    printf("%s\n", result);
    free(copy);
    free(result);
    return;
}

void quit(char* s) {
    printf("Ciao!\n");
    exit(0);
}
