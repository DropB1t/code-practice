#include <stdio.h>

int main(int argc, char const *argv[]) {

    FILE* fin  = fopen("test.txt","a");
    char* testo="\n";
    if(fin){
        fputs(testo,fin);
        printf("%s  %d\n",testo,feof(fin));
        fclose(fin);
    }else
        printf("Error");

    return 0;
}
