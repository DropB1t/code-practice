#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>

#include <util.h>

// globale, e' possibile dichiararla come variabile locale del thread
// main e passarla ai thread come argomento
pthread_barrier_t barrier;

void *threadFunction(void *arg) {
  long myid = (long)arg;
  usleep(myid*500000);
  
  fprintf(stdout, "Ciao sono il thread %ld [%ld]\n", myid, pthread_self());

  pthread_barrier_wait(&barrier);   // aspetto finche' non siamo tutti....

  fprintf(stdout, "thread %ld go\n", myid);
  
  return NULL;
}


int main(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "usa: %s nthreads\n", argv[0]);
    return -1;
  }

  long nthreads;
  if (isNumber(argv[1], &nthreads) != 0) {
    fprintf(stderr, "%s non e' un numero valido\n", argv[1]);
    return -1;
  }

  // inizializzo la barriera con il numero di thread che si devono
  // sincronizzare (tutti i thread + il thread main)
  pthread_barrier_init(&barrier, NULL, nthreads+1);
  
  pthread_t threadid[nthreads];
  for(long i=0;i<nthreads;++i) {
    if (pthread_create(&threadid[i], NULL, threadFunction, (void*)i) != 0) {
      perror("pthread_create");
      return -1;
    }
  }

  fprintf(stdout, "thread main si blocca sulla barrier\n");
  pthread_barrier_wait(&barrier);   // aspetto finche' non siamo tutti....

  fprintf(stdout, "thread main go\n");
  
  for(long i=0;i<nthreads;++i) {
    pthread_join(threadid[i], NULL);
  }

  // libero la risorsa barriera 
  pthread_barrier_destroy(&barrier);
  
  
  return 0;
}
