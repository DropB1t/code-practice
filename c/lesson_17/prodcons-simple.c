//
// Semplice produttore-consumatore con buffer di una sola posizione.
// Versione con 2 variabili di condizione
//
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <util.h>

#define MAX_STRING_LEN   1024



// risorsa condivisa (buffer di una sola posizione, memorizza una stringa)
static char buffer[MAX_STRING_LEN];
static int  buffer_capacity = 1;  // capacita' totale del buffer
static int  buffer_len=0;         // lunghezza corrente del buffer 

// variabile di lock e di condizione per la sincronizzazione
// sul buffer condiviso
static pthread_mutex_t mutex       = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cond_empty  = PTHREAD_COND_INITIALIZER;
static pthread_cond_t  cond_full   = PTHREAD_COND_INITIALIZER;


// produttore
void *Producer(void *notused) {
  char localbuffer[MAX_STRING_LEN];
  
  for(;;) {

    /* ---------------------- */
    fprintf(stdout, "Inserisci una stringa (o 'exit'): ");
    fflush(stdout);
    if (fgets(localbuffer, MAX_STRING_LEN, stdin) == NULL ) {
      perror("fgets");
      return NULL;
    }
    if (strncmp(localbuffer, "exit", strlen("exit")) == 0) {      
      break;
    }    
    localbuffer[strlen(localbuffer)-1]='\0';  // tolgo '\n'
    
    /* -------------------- */

    LOCK(&mutex);
    while(buffer_len>=buffer_capacity)
      WAIT(&cond_full,&mutex);
    
    for(int i=0;i<strlen(localbuffer); ++i)
      buffer[i] = localbuffer[i];    
    buffer_len = 1;
    SIGNAL(&cond_empty);
    UNLOCK(&mutex);
  }
  
  // produco un valore speciale per provocare la terminazione
  // del consumatore
  LOCK(&mutex);
  while(buffer_len>=buffer_capacity)
    WAIT(&cond_full,&mutex);
  
  strncpy(buffer, "\n", 2);  // suppongo che '\n' non sia mai nel buffer
  buffer_len = 1;
  
  SIGNAL(&cond_empty);
  UNLOCK(&mutex);

  DBG("Producer [%ld] closing\n", pthread_self());  
  return NULL;
}

// consumatore 
void *Consumer(void *notused) {
  char mybuffer[MAX_STRING_LEN]; 
  
  while(1) {
    LOCK(&mutex);
    while(buffer_len == 0) 
      WAIT(&cond_empty,&mutex);
    
    if (buffer[0] == '\n') { // il produttore è terminato
      UNLOCK(&mutex);
      break;
    }

    strncpy(mybuffer, buffer, MAX_STRING_LEN);
    buffer_len = 0;
    
    SIGNAL(&cond_full);
    UNLOCK(&mutex);

    fprintf(stdout, "Consumer, ricevuta stringa: %s\n", mybuffer);
  }
  DBG("Consumer [%ld] closing\n", pthread_self());    
  return NULL;
}

int main() {
  pthread_t thconsumer, thproducer;
  
  if (pthread_create(&thconsumer, NULL, Consumer, NULL) != 0) {
    fprintf(stderr, "pthread_create failed (Consumer)\n");
    return (EXIT_FAILURE);
  }
  if (pthread_create(&thproducer, NULL, Producer, NULL) != 0) {
    fprintf(stderr, "pthread_create failed (Producer)\n");
    return (EXIT_FAILURE);
  }    
  if (pthread_join(thproducer, NULL) != 0) {
    fprintf(stderr, "pthread_join failed (Producer)\n");
    return (EXIT_FAILURE);
  }
  if (pthread_join(thconsumer, NULL) != 0) {
    fprintf(stderr, "pthread_join failed (Consumer)\n");
    return (EXIT_FAILURE);
  }
  return 0;
}
