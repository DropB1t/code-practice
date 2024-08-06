/*
Scrivere una funzione in C 

int copyFile(const char* source, const char* dest)

che copia i contenuti del file "source" nel file "dest".
La funzione utilizza un buffer di dimensione fissa (una costante) per processare i contenuti.
La funzione restituisce 0 se la copia è stata fatta con successo, 1 in caso di errore.
Non cambiare la funzione "main" e i due file che trovate su REPL, servono per testare la funzione.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_L 10000

int copyFile(const char* source, const char* dest);

int main()
{
    char f1[100], f2[100];
    char command[206];
    scanf("%s %s", f1, f2);
    int res = copyFile(f1, f2);
    if (!res) {
        sprintf(command, "diff %s %s", f1, f2);
        system(command);
        printf("\nEnd diff.");
    } else {
        printf("Copy failed.\n");
    }
    return 0;
}

int copyFile(const char* source, const char* dest)
{
    FILE *fs;
	char *buffer;
	unsigned long fileLen;

	//Open file
	fs = fopen(source, "rb");
	if (!fs)
	{
		fprintf(stderr, "Unable to open file %s", source);
		return 1;
	}
	
	//Get file length
	fseek(fs, 0, SEEK_END);
	fileLen=ftell(fs);
	fseek(fs, 0, SEEK_SET);

	//Allocate memory
	buffer=(char *)malloc(fileLen+1);
	if (!buffer)
	{
		fprintf(stderr, "Memory error!");
        fclose(fs);
		return 1;
	}

	fread(buffer, fileLen, 1, fs);
	fclose(fs);
    FILE *fd = fopen(dest, "wb");
    if (!fd)
	{
		fprintf(stderr, "Unable to open file %s", source);
		return 1;
	}
    fwrite(buffer,1,fileLen,fd);
    fclose(fd);
    return 0;
}