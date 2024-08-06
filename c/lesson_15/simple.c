#include <stdio.h>
#include <pthread.h>

#include <util.h>

void *threadFunction(void *arg) {
#if 0
  long myid = *(long*)arg;
#endif  
  long myid = (long)arg;

  printf("Thread %ld [%ld] partito\n", myid, pthread_self());

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

  pthread_t threadID[nthreads];
  for(long i=0;i<nthreads; ++i) {
#if 0    
    long *j = malloc(sizeof(long));
    assert(j != NULL);
    *j = i;
#endif     
    if (pthread_create(&threadID[i], NULL, threadFunction, (void*)i ) !=0) {
      perror("pthread_create");
      return -1;
    }
  }

  for(long i=0;i<nthreads; ++i) {
    if (pthread_join(threadID[i], NULL) !=0) {
      perror("pthread_join");
      return -1;
    }
  }
  return 0;
}
