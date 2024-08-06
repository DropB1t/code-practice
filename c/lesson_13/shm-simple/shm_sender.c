//
// shm_sender/shm_receiver
//
// Esempio base di utilizzo di un segmento di memoria condivisa tra 2 processi utilizzando
// l'interfaccia POSIX.
//
//

#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>


#define NAME    "/shmem-example"
#define NUM     10
#define SIZE    (NUM * sizeof(int))

                                                        
int main() {                                          
  int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);   // rw- --- --- 
  if (fd<0) {
    perror("shm_open");
    return -1;
  }
  
  // set the size of the shared memory segment
  if (ftruncate(fd, SIZE) == -1) {
    perror("ftruncate");
    return -1;
  }
  
  // map the file associated to the shared-memory segment into the address
  // space of the caller. NOTE: the MAP_SHARED flag !!
  int *data = (int*)mmap(0, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if (!data){
    perror("mmap");
    return -1;
  }
  printf("sender mapped the segment at address %p\n", data);

  // write something into the shared memory region
  for(int i=0;i<NUM;++i)
    data[i]=i;

  // unmap the shared-memory segment from the address space 
  munmap(data, SIZE);
  close(fd);
  return 0;
}
