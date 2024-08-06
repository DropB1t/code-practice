#if !defined(_SEM_BARRIER_H)
#define _SEM_BARRIER_H

#include <util.h>

typedef struct {
  sem_t barmutex;
  sem_t barrier;
  int   bar_counter;
} mybarrier_t;


// Attenzione: non eseguita in mutua esclusione!
static inline int barrier_init(mybarrier_t *b, int n) {
  b->bar_counter = n;
  if (sem_init(&b->barmutex, 0, 1) == -1) {
    perror("sem_init barmutex");
    return -1;
  }
  if (sem_init(&b->barrier, 0, 0) == -1) {
    perror("sem_init barrier");
    return -1;
  }
  return 0;
}
// Attenzione: non eseguita in mutua esclusione!
static inline void barrier_reset(mybarrier_t *b, int n) { b->bar_counter = n; }
// Attenzione: non eseguita in mutua esclusione!
static inline int barrier_destroy(mybarrier_t *b) {
  b->bar_counter = -1;
  if (sem_destroy(&b->barmutex) == -1) {
    perror("sem_destroy barmutex");
    return -1;
  }
  if (sem_destroy(&b->barrier) == -1) {
    perror("sem_destroy barrier");
    return -1;
  }
  return 0;
}

// mancano i controlli su P e V
static inline int barrier_wait(mybarrier_t *b) {
  P(&b->barmutex);
  if (--b->bar_counter== 0) V(&b->barrier);
  V(&b->barmutex);

  P(&b->barrier);
  V(&b->barrier);
  return 0;
}



#endif /* _SEM_BARRIER_H */
