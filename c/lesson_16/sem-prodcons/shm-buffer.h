#if !defined(SHM_BUFFER_H)
#define SHM_BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>


#define NBUFF 10
#define MAX_MSG_LENGTH 1024

// tipo memorizzato nel buffer
typedef char data_type[MAX_MSG_LENGTH];

typedef struct  {
  sem_t empty;   // semaforo usato per il controllo buffer vuoto
  sem_t full;    // semaforo usato per il controllo buffer pieno
  sem_t mutex;   // semaforo usato per la mutua esclusione
  
  int head;      // "puntatore" all'elemento di testa
  int tail;      // "puntatore" all'elemento di coda (prima posizione libera nel buffer)

  data_type databuf[NBUFF];  // buffer di capacità NBUFF gestito in modo circolare
} buffer_t;


// apre il buffer, se non esiste riprova per maxretry volte
// attendendo delay_ms millisecondi ogni volta
static inline buffer_t *buffer_open(const char *name, int maxretry,
				    int delay_ms) {

  const struct timespec ts = {delay_ms/1000, (delay_ms%1000)*1000000};
  int retry=0;
  int fd;
  
  do {
    fd = shm_open(name, O_RDWR, 0);
    if (fd<0) {
      if (errno == ENOENT) {
	nanosleep(&ts, NULL);
	if (++retry > maxretry) return NULL;
	printf("retrying ....\n");
	continue;
      }
      perror("shm_open");
      return NULL;
    } else break;
  } while(1);
  
  buffer_t *buf = mmap(0, sizeof(buffer_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);
  if (buf == MAP_FAILED){
    perror("mmap");
    return NULL;
  }
  return buf;
}

static inline void buffer_close(buffer_t *buf) {
  munmap(buf, sizeof(*buf));
}

// crea ed apre il segmento condiviso per il buffer
static inline buffer_t *buffer_create(const char *name) {
  int fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600);
  if (fd<0) {
    // se il nome esiste, suppongo sia stato gia' 
    // creato ed inizializzato da un altro produttore
    if (errno != EEXIST) {
      perror("shm_open");
      return NULL;
    }
    return buffer_open(name, 0, 0);
  } else {
    // set the size of the shared memory segment
    if (ftruncate(fd, sizeof(buffer_t)) == -1) {
      perror("ftruncate");
      close(fd);
      return NULL;
    }
  }
  buffer_t *buf = mmap(0, sizeof(buffer_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);
  if (buf == MAP_FAILED){ // memory mapping fallito
    perror("mmap");
    return NULL;
  }
  // inizializziamo gli unnamed semaphores nel segmento condiviso settando pshared = 1 
  if (sem_init(&buf->full, 1, NBUFF)<0) { 
    perror("sem_init full");
    return NULL;
  }
  if (sem_init(&buf->empty, 1, 0)<0) {
    perror("sem_init empty");
    return NULL;
  }
  if (sem_init(&buf->mutex, 1, 1)<0) {
    perror("sem_init mutex");
    return NULL;
  }
  // inizializzazione dei "puntatori"
  buf->head=0;
  buf->tail=0;

  return buf;
}

//
// funzione che inserisce nel buffer l'elemento 'data'
// se il buffer e' pieno, sospende il chiamante sul semaforo full
//
static inline int put(buffer_t *buf, const data_type data) {
  if (!buf) {
    errno=EINVAL;
    return -1;
  }
  
  P(&buf->full);
  P(&buf->mutex);
  
  memcpy(buf->databuf[buf->tail], data, sizeof(data_type));

  buf->tail = (buf->tail + 1) % NBUFF;
  V(&buf->mutex);
  V(&buf->empty);
  return 0;
}
//
// funzione che estrae un elemento dal buffer memorizzandolo in 'data'
// se il buffer e' vuoto, sospende il chiamante sul semaforo empty
//
static inline int get(buffer_t *buf, data_type *data) {
  if (!buf || !data) {
    errno=EINVAL;
    return -1;
  }

  P(&buf->empty);
  P(&buf->mutex);
  
  memcpy(*data, buf->databuf[buf->head], sizeof(data_type));

  buf->head = (buf->head + 1) % NBUFF;
  V(&buf->mutex);
  V(&buf->full);
  return 0;
}

#endif /* SHM_BUFFER_H */
