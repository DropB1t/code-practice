//
// shm_sender/shm_receiver
//
// Esempio base di utilizzo di un segmento di memoria condivisa tra 2 processi utilizzando
// l'interfaccia POSIX.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define NAME    "/shmem-example"
#define NUM     10
#define SIZE    (NUM * sizeof(int))


int main() {
  // the shared-memory segment must be already created
  int fd = shm_open(NAME, O_RDONLY, 0);
  if (fd<0) {
    perror("shm_open");
    return -1;
  }
  // map the file associated to the shared-memory segment into the address
  // space of the caller. NOTE: the MAP_SHARED flag !!
  int *data = (int*)mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
  if (!data){
    perror("mmap");
    return -1;
  }
  printf("receiver mapped the segment at address %p\n", data);

  // read and print the shared data
  for(int i=0;i<NUM;++i)
    printf("data[%d]=%d\n", i, data[i]);

  // unmap the shared-memory segment from the address space 
  munmap(data, SIZE);
  close(fd);

  // IMPORTANT: if the segment is not removed it remains allocated,
  //            see /dev/shm system directory
  shm_unlink(NAME);    
  return 0;
}
