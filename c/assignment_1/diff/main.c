/*
Scrivere una funzione in C 
int diff(const char* f1, const char* f2) 
che trova le differenze tra i due file ricevuti come parametri.
La funzione considera i file come insiemi di righe (due insiemi F1, F2), e stampa
prima le righe del primo file che non ci sono nel secondo file (l’insieme differenza F1-F2),
e poi le righe del secondo file che non ci sono nel primo file (l’insieme differenza F2-F1).
Le righe vanno stampate nel formato “numero di riga: contenuto della riga” in ordine
crescente del numero di riga. La funzione restituisce 0 se i file sono identici, 1 se sono
diversi, 2 se si è incontrato un errore. Implementare le eventuali strutture di dati e
funzioni necessarie per ottenere la funzionalità richiesta. Non cambiare la funzione main
e i file di input che trovate su REPL.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE * f_open(const char* f);
int diff(const char* f1, const char* f2);
int count_lines(const char* f);
char ** fill_arr(char** ln,FILE *fl);
int check_diff(char ** a, char** b, int n1, int n2);
void free_arr(char** arr, int n);

int main(void) {
  char f1[100],f2[100];
	scanf("%s %s",f1,f2);
	int res=diff(f1,f2);//diff stampa differenze se ci sono, altrimenti niente
    if(res==0){
		printf("File identici.\n");
	} 
	if(res==2){
		printf("Errore.\n");
	}
	return 0;
}

int diff(const char* f1, const char* f2){
    
    FILE *fl1 = f_open(f1), *fl2 = f_open(f2);
    if(!fl1 || !fl2)
        return 2;
    int n1 = count_lines(f1);
    int n2 = count_lines(f2);

    char** lines1 = (char**)malloc(n1*sizeof(char*));
    char** lines2 = (char**)malloc(n2*sizeof(char*));

    lines1 = fill_arr(lines1,fl1);
    lines2 = fill_arr(lines2,fl2);

    fclose(fl1);
    fclose(fl2);

    int res1 = check_diff(lines1,lines2,n1,n2);
    int res2 = check_diff(lines2,lines1,n2,n1);

    free_arr(lines1,n1);
    free_arr(lines2,n2);

    if(res1 || res2){
        return 1;
    }

    return 0;
}

int check_diff(char ** a, char** b, int n1, int n2){
    int res = 0;
    int check = 0;
    for (size_t i = 0; i < n1; i++)
    {
        check = 0;
        for (size_t j = 0; j < n2; j++)
        {
            if (strcmp(a[i],b[j]) == 0){
                check = 1;
            }
        }
        if(!check){
            printf("%ld: %s",i+1,a[i]);
            res = 1;
        }
    }
    return res;
}

char ** fill_arr(char** ln,FILE *fl){
    char * line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    int i = 0;
    while (read >= 0) {
        read = getline(&line, &len, fl);
        ln[i] = strdup(line);
        i++;
    }
    return ln;
}

int count_lines(const char* f){
    FILE *fl = f_open(f);
    int lines = 0,ch;
    while(!feof(fl))
    {
        ch = fgetc(fl);
        if(ch == '\n')
            lines++;
    }
    fclose(fl);
    return lines;
}

FILE * f_open(const char* f){
    FILE *fl;
    fl = fopen(f, "r");
	if (!fl)
        return NULL;
    return fl;
}

void free_arr(char** arr, int n){
    for (int i = 0; i < n; i++)
    {
        free(arr[i]);
    }
    return;
}