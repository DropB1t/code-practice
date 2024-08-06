/* Author: Massimo Torquati <torquati@di.unipi.it>
 * 
 * Per il corso di Laboratorio di SO, Universita' di Pisa.
 * Esempio di utilizzo delle pipe con nome per implementare
 * un protocollo client-server.
 *
 */ 
#if !defined(MSG_H)
#define MSG_H

#include <linux/limits.h>  /* Linux specific definisce PIPE_BUF */


#define F_SIZE  256               /* size massima del nome della FIFO del client */ 
#define B_SIZE  (PIPE_BUF-F_SIZE) /* size massima dei dati scambiati in modo atomico */

// un po' brutale .... non deve essere utilizzata in funzioni di libreria
// l'errore va fatto "galleggiare" al chiamante
#define SYSCALL(r,c,e) \
    if((r=c)==-1) { perror(e);exit(errno); }

// FIFO pubblica per inviare le richieste al server
const char *PUBLIC = "/tmp/PUBLIC";

/* 
 * In un singolo messaggio vogliamo inviare sia il comando da eseguire 
 * che il nome della fifo su cui vogliamo ottenere il risultato.
 */
struct message {
    char fifo_name[F_SIZE];
    char  cmd_line[B_SIZE];
};

#endif /* MSG_H */
