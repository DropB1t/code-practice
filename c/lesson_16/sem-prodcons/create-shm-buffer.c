#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>


#include "shm-buffer.h"


int main(int argc, char *argv[]) {
  if (argc != 2 && argc != 3) {
    fprintf(stderr, "usage: %s name [destroy]\n", argv[0]);
    return -1;
  }

  if (argc == 3) {
    shm_unlink(argv[1]);
    //sem_unlink(STARTUP_SEM_NAME);
    printf("removed POSIX shared buffer with name %s\n", argv[1]);
    return 0;
  }
  
  int fd = shm_open(argv[1], O_CREAT | O_EXCL | O_RDWR, 0600);
  if (fd<0) {
    perror("shm_open");
    return -1;
  }

  // set the size of the shared memory segment
  if (ftruncate(fd, sizeof(buffer_t)) == -1) {
    perror("ftruncate");
    return -1;
  }
  
  buffer_t *buf = mmap(0, sizeof(buffer_t), PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (!buf){
    perror("mmap");
    return -1;
  }
  if (buffer_init(buf) == -1) {
    fprintf(stderr, "Inizializzazione del buffer fallita!\n");
    return -1;
  }
  close(fd);
  munmap(buf, sizeof(buffer_t));

  printf("created a POSIX shared buffer of %d entries and with name %s\n", NBUFF, argv[1]);
  return 0;
}
