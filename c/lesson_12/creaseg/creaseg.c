#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>


int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "usa: %s <shm name> <shm size>\n", argv[0]);
    return -1;
  }
  
  
  int fd = shm_open(argv[1], O_CREAT | O_EXCL | O_RDWR, S_IRUSR|S_IWUSR);
  if (fd<0) {
    perror("shm_open");
    return -1;
  }

  int size = atoi(argv[2]);
  
  // set the size of the shared memory segment
  if (ftruncate(fd, size) == -1) {
    perror("ftruncate");
    return -1;
  }
  
  int *data = (int*)mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if (!data){
    perror("mmap");
    return -1;
  }
  close(fd);
  return 0;
}
