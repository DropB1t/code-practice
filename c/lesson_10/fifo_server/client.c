/* Author: Massimo Torquati <torquati@di.unipi.it>
 * 
 * Per il corso di Laboratorio di SO, Universita' di Pisa.
 * Esempio di utilizzo delle pipe con nome per implementare
 * un protocollo client-server.
 *
 */ 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "msg.h"

// nome della fifo privata
char fifo_name[F_SIZE];
void cleanup() {
    int n;
    SYSCALL(n, unlink(fifo_name), "cleanup: unlink private fifo");
}

int main() {
    static char buffer[PIPE_BUF];
    struct message msg;
    int publicfifo, privatefifo, n;

    // fifo privata per ricevere le risposte dal server 
    snprintf(fifo_name, F_SIZE, "/tmp/fifo%d", getpid());
    SYSCALL(n, mkfifo(fifo_name, 0666), "mkfifo");

    // qualora qualcosa vada storto
    if (atexit(cleanup) != 0) {
	fprintf(stderr, "ERRORE in atexit\n");
	exit(EXIT_FAILURE);
    }

    // apro la fifo pubblica in sola scrittura
    SYSCALL(publicfifo, open(PUBLIC,O_WRONLY), "open public fifo");
    
    while(1) {
	SYSCALL(n,write(1, "\n cmd>", 6), "write 1");

	// resetto il messaggio
	memset(msg.fifo_name, 0x0, F_SIZE);
	memset(msg.cmd_line, 0x0, B_SIZE);
	// leggo il comando da inviare al server
	SYSCALL(n,read(0, msg.cmd_line, B_SIZE), "read 0");
	
	// devo uscire ?
	if(strncmp("quit", msg.cmd_line, n-1) == 0)  break;

	strncpy(msg.fifo_name,fifo_name, strlen(fifo_name)+1);

	// mando la richiesta al server
	SYSCALL(n, write(publicfifo, &msg, sizeof(msg)), "write public fifo");

	// opro la fifo privata in sola lettura aspettando che il server la apra in scrittura
	SYSCALL(privatefifo, open(msg.fifo_name, O_RDONLY), "open private fifo");
	
	do {
	    memset(buffer, 0x0, PIPE_BUF);
	    SYSCALL(n ,read(privatefifo, buffer, PIPE_BUF), "read private fifo");
	    fprintf(stdout, "%s", buffer);
	} while(n>0);
	SYSCALL(n, close(privatefifo), "close private fifo");
    }
    SYSCALL(n, close(publicfifo), "close public fifo");    
    return 0;
}
