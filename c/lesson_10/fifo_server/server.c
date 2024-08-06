/* Author: Massimo Torquati <torquati@di.unipi.it>
 * 
 * Per il corso di Laboratorio di SO, Universita' di Pisa.
 * Esempio di utilizzo delle pipe con nome per implementare
 * un protocollo client-server.
 *
 */ 
#define _POSIX_C_SOURCE  200112L
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "msg.h"

#define MAXARGS 100   // massimo numero di argomenti di un comando

// costruisce un vettore di argomenti da una stringa
static inline void buildargs(char *line, char * args[]) {
  int i = 0;
  args[i++] = strtok(line," \n");
  do {
    args[i] = strtok(NULL," \n");
  } while(args[i++]!=NULL);
}

int main() {    
    struct message msg;
    static char buffer[PIPE_BUF];
    
    /*creating the PUBLIC fifo*/
    if ((mkfifo(PUBLIC, 0666) == -1) && errno!=EEXIST) {
	perror("mkfifo public fifo");
	exit(errno);
    }
    
    int publicfifo;
    // apertura in sola lettura per ricevere i comandi dai clients, aspetto che
    // almeno uno dei client apra la fifo in scrittura
    SYSCALL(publicfifo, open(PUBLIC, O_RDONLY), "open public fifo (read)");
    // apro la fifo in scrittura per evitare di ricevere EOF sulla fifo pubblica
    // che rimane sempre aperta
    int notused;
    SYSCALL(notused, open(PUBLIC, O_WRONLY|O_NONBLOCK), "open public fifo (write)");
    
    while(1) {
	int n, done;
	SYSCALL(n, read(publicfifo, &msg, sizeof(msg)), "read public fifo");
	if (n == 0) break; // ho letto EOF

	n=0, done=0;
	do {
	    int privatefifo;
	    // attendo che il client si connetta riprovando un po' di volte
	    if ((privatefifo = open(msg.fifo_name, O_WRONLY|O_NONBLOCK)) == -1) {
		struct timespec ts={0,50000000};
		nanosleep(&ts,NULL);
	    } else {
		int channel[2];
		int r;
		SYSCALL(r, pipe(channel), "pipe");
		
		char *args[MAXARGS];
		buildargs(msg.cmd_line, args);

		if (fork() == 0) { // figlio
		    SYSCALL(r, dup2(channel[1],1), "dup2 writer");
		    SYSCALL(r, close(channel[0]), "close reader");
		    SYSCALL(r, close(channel[1]), "close writer");

		    execvp(args[0], &args[0]);
		    fprintf(stderr, "execvp fallita");
		    exit(EXIT_FAILURE);
		} // padre

		SYSCALL(r, close(channel[1]), "close writer");
		SYSCALL(r, write(privatefifo,"\n",1), "write private fifo");
		do {
		    SYSCALL(n, read(channel[0], buffer, PIPE_BUF), "read channel[0]");
		    SYSCALL(r, write(privatefifo,buffer,n) && errno!=EINTR, "write private fifo");
		    memset(buffer, 0x0, PIPE_BUF);
		} while(n>0);
		
		SYSCALL(r, close(channel[0]), "close reader");
		SYSCALL(r, close(privatefifo), "close private fifo"); // invio EOF
		done = 1;
	    }
	}while(!done && n++ < 10);
	if (!done) {
	    fprintf(stderr, "Il client non ha inviato il comando, condizione di fallimento\n");
	    exit(EXIT_FAILURE);
	}	
    }
    return 0;
}
