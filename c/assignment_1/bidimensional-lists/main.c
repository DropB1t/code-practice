/*
Implementare una libreria per lavorare con liste bidimensionali utilizzate per tenere traccia
di versioni di file. Ogni file è definito dal suo path assoluto, più una lista di versioni.
Per ogni versione abbiamo un ID numerico e una timestamp con la data della versione. 

Scrivere una struttura di dati FileList che utilizza liste concatenate per mantenere le versioni,
e per mantenere la lista stessa di file. 

Implementare le seguenti funzioni:

int addFile(FileList* fl ,const char* filename) - aggiunge un file nuovo alla lista (in testa)
e restituisce 0 in caso di successo, 1 in caso di errore. La lista di versioni del nuovo file è vuota.
Se il file esiste già non aggiunge niente e restituisce 1. 

int addVersion(FileList* fl, const char* filename, int versionID, time_t timestamp) - aggiunge
una nuova versione alla lista di versione del file con il nome filename. Se il file non esiste
nella lista, lo aggiunge. La funzione restituisce 0 in caso di successo, 1 in caso di errore.

int removeFile(FileList* fl ,const char* filename) - rimuove un file con tutte le sue versioni
dalla lista. Se il file non esiste restituisce 1, altrimenti 0.

int removeVersion (FileList* fl, const char* filename, int versionID)  - rimuove una versione
da un file nella lista. Se il file non esiste, restituisce 1, se la versione non esiste, restituisce 2,
altrimenti 0.

VersionList getHist(FileList fl, const char* filename) - restituisce la lista di versioni del
file passato come parametro. Il tipo di return VersionList è lo stesso della struttura di dati
utilizzata per mantenere la lista di versioni.

void freeFileList(FileList* fl) - libera la memoria per la lista FileList. Dopo la chiamata
il puntatore fl deve essere NULL.

FileList loadFileList(const char* file) - legge una lista da un file testuale. Ogni file e le sue
versioni sono rappresentate su una riga
nel formato “nome file: versione, timestamp; versione, timestamp; … versione, timestamp”
Restituisce  la lista di file, NULL in caso di errore.

int saveFileList(FileList f, const char* file) - salva una lista di file in un file
testuale, nel formatto descritto sopra. La funzione restituisce 0 in caso di successo, 1 in caso di errore.
 
Non editare la funzione main e i file di input e output che trovate su REPL.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct v{
    int ID;
    struct v* next;
    time_t data;

};
typedef struct v version;
typedef version* VersionList;


struct f{
    char* filename;
    struct f* next;
    VersionList v;
};
typedef struct f flist;
typedef flist* FileList;

int addFile_tail(FileList* fl ,const char* filename);
int addFile(FileList* fl ,const char* filename);
void free_v(VersionList* v);
int removeFile(FileList* fl ,const char* filename);
int removeVersion(FileList* fl, const char* filename, int versionID);
VersionList getHist(FileList fl, const char* filename);
FileList loadFileList(const char* file);
void freeFileList(FileList* fl);
int addVersion(FileList* fl, const char* filename, int versionID, time_t timestamp);
int saveFileList(FileList f, const char* file);
void v_toString(VersionList* v,char* str);
void rec_save(FileList tmp,FILE* fl,int n);

int main(void) {
	char fname[100],command[200];
	scanf("%s",fname);

    FileList f= loadFileList(fname);
    VersionList h = NULL;
	if(f){
		int option;
		scanf("%d",&option);
		switch(option){
			case 1: 
				saveFileList(f,"realout1.txt");
				sprintf(command,"diff %s %s","out1.txt","realout1.txt");
  			    system(command);
				break;
		
			case 2:
				addFile(&f,"/home/user/f9.txt");
				saveFileList(f,"realout2.txt");
				sprintf(command,"diff %s %s","out2.txt","realout2.txt");
  			    system(command);
				break;

			case 3:
				addVersion(&f,"/home/user/f9.txt",1,1637507877);
				saveFileList(f,"realout3.txt");
				sprintf(command,"diff %s %s","out3.txt","realout3.txt");
  			    system(command);
				break;

			case 4:
				addVersion(&f,"/home/user/f2.txt",2,1637507890);
				saveFileList(f,"realout4.txt");
				sprintf(command,"diff %s %s","out4.txt","realout4.txt");
  			    system(command);
				break;

			case 5:
				removeFile(&f,"/home/user/f2.txt");
				saveFileList(f,"realout5.txt");
				sprintf(command,"diff %s %s","out5.txt","realout5.txt");
  			    system(command);
				break;

			case 6:
				removeVersion(&f,"/home/user/t3.txt",2);
				saveFileList(f,"realout6.txt");
				sprintf(command,"diff %s %s","out6.txt","realout6.txt");
  			    system(command);
				break;

			case 7:
				h=getHist(f,"/home/user/t3.txt");
				assert(h!=NULL);
				freeFileList(&f);
				assert(f==NULL);
		}
		printf("Done.\n");
	}
	else{
		printf("Errore.\n");
	}
}

int addFile_tail(FileList* fl ,const char* filename){
    flist *tmp = *fl;
    flist *pred = NULL;
    int exist = 0;
    while (tmp != NULL)
    {
        if(strcmp(tmp->filename,filename) == 0){
            exist = 1;
            break;
        }
        pred = tmp;
        tmp = tmp->next;
    }
    if(exist)
        return 1;
    
    flist* f = malloc(sizeof(flist));
    f->filename = malloc(strlen(filename)+1);
    strcpy(f->filename,filename);
    f->v = NULL;
    f->next = NULL;
    if(pred)
        pred->next = f;
    else
        *fl = f;
    return 0;
}

int addFile(FileList* fl ,const char* filename){
    flist *tmp = *fl;
    int exist = 0;
    while (tmp != NULL)
    {
        if(strcmp(tmp->filename,filename) == 0){
            exist = 1;
            break;
        }
        tmp = tmp->next;
    }
    if(exist)
        return 1;
    
    flist* f = malloc(sizeof(flist));
    f->filename = malloc(strlen(filename));
    strcpy(f->filename,filename);
    f->v = NULL;
    f->next = *fl;
    *fl = f;
    return 0;
}

int addVersion(FileList* fl, const char* filename, int versionID, time_t timestamp){
    flist * tmp = *fl;
    int exist = 0;
    while (tmp != NULL)
    {
        if(strcmp(tmp->filename,filename) == 0){
            exist = 1;
            break;
        }
        tmp = tmp->next;
    }
    if(!exist){
        addFile(fl,filename);
        addVersion(fl,filename,versionID,timestamp);
        return 0;
    }

    version* v = malloc(sizeof(version));
    v->data = timestamp;
    v->ID = versionID;
    v->next = NULL;
    if(tmp->v != NULL){
        version* tail = tmp->v;
        while (tail->next != NULL){ tail = tail->next; }
        tail->next = v;
    }else{
        tmp->v = v;
    }
    return 0;
}

int removeFile(FileList* fl ,const char* filename){
    flist * tmp = *fl;
    flist * pred = NULL;
    int exist = 0;
    while (tmp != NULL)
    {
        if(strcmp(tmp->filename,filename) == 0){
            exist = 1;
            break;
        }
        pred = tmp;
        tmp = tmp->next;
    }
    if(!exist)
        return 1;
    if(pred){
        pred->next = tmp->next;
    }
    free(tmp->filename);
    free_v(&tmp->v);
    tmp->next = NULL;
    free(tmp);
    return 0;
}

int removeVersion(FileList* fl, const char* filename, int versionID){
    flist * tmp = *fl;
    int exist = 0;
    while (tmp != NULL)
    {
        if(strcmp(tmp->filename,filename) == 0){
            exist = 1;
            break;
        }
        tmp = tmp->next;
    }
    if(!exist)
        return 1;
    exist = 0;
    version* v = tmp->v;
    version* pred = NULL;
    while (v != NULL)
    {
        if(v->ID == versionID){
            exist = 1;
            break;
        }
        pred = v;
        v = v->next;
    }
    if(!exist)
        return 2;
    if(pred){
        pred->next = v->next;
    }
    v->next = NULL;
    free(v);
    return 0;
}

VersionList getHist(FileList fl, const char* filename){
    flist * tmp = fl;
    int exist = 0;
    while (tmp != NULL)
    {
        if(strcmp(tmp->filename,filename) == 0){
            exist = 1;
            break;
        }
        tmp = tmp->next;
    }
    if(!exist)
        return NULL;
    return tmp->v;
}

void freeFileList(FileList* fl){
    if(*fl == NULL)
        return;
    freeFileList(&((*fl)->next));
    removeFile(fl,(*fl)->filename);
    *fl = NULL;
    return;
}

FileList loadFileList(const char* file){
    FILE* fl = fopen(file,"r");
    if (!fl)
        return NULL;
    FileList f = NULL;
    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, fl) != -1){
        char *token;
        token = strtok(line, ":");
        if(token != NULL){
            addFile_tail(&f,token);
        }else{ return NULL; }
        while( (token = strtok(NULL, ";")) != NULL ) {
            if(!token){ return NULL; }
            int id;
            time_t data;

            char* res = malloc(2);
            strncpy(res, &token[0], 1);
            id = atoi(res);

            res = realloc(res,11);
            strncpy(res, &token[2], 10);
            data = (time_t)atoi(res);
            
            flist * tmp = f;
            while (tmp->next != NULL){ tmp = tmp->next; }
            addVersion(&f,tmp->filename,id,data);
            free(res);
        }
        free(token);
    }

    fclose(fl);
    free(line);
    
    return f;
}

int saveFileList(FileList f, const char* file){
    if(!f || !file){  return 1;}
    FILE* fl = fopen(file,"w");
    if (!fl)
        return 1;
    
    flist * tmp = f;
    rec_save(tmp,fl,0);
    fclose(fl);
    return 0;
}

void rec_save(FileList tmp,FILE* fl,int n){
    if(tmp == NULL)
        return;
    char line[10000];
    sprintf(line,"%s:",tmp->filename);
    v_toString(&(tmp->v),line);
    if(tmp->v != NULL)
        line[strlen(line)-1] = '\0';
    if(tmp->next != NULL){
        strcat(line,"\n");
    }
    fputs(line,fl);
    rec_save(tmp->next,fl,n+1);
    return;
}

void v_toString(VersionList* v,char* str){
    if(*v == NULL)
        return;
    char* info = malloc((*v)->ID+(int)((*v)->data)+3);
    sprintf(info,"%d,%d;",(*v)->ID,(int)((*v)->data));
    strcat(str,info);
    v_toString(&((*v)->next),str);
    return;
}

void free_v(VersionList* v){
    if(*v == NULL)
        return;
    free_v(&((*v)->next));
    free(*v);
    *v = NULL;
    return;
}