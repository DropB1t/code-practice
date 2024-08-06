#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

#include <util.h>

int main(int argc, char *argv[]) {

  if (argc == 1) {
    fprintf(stderr, "use: %s textfile [textfile]\n", argv[0]);
    return EXIT_FAILURE;
  }

  int r;
  for(int i=1; i<argc; ++i) {
    const char *filename = argv[i];
    size_t filesize =0;
    if ((r=isRegular(filename, &filesize)) == 1) {
      
      int fd;
      SYSCALL_PRINT("open", fd, open(filename, O_RDWR), "open %s", filename);
      if (fd==-1) continue;

      /* NOTAONE: se invece di MAP_SHARED si usa MAP_PRIVATE le modifiche 
       *          non vengono riportate nel file. 
       *          Per sincronizzare il contenuto della memory map area nel 
       *          file prima dalla munmap si può usare la chiamata msync.
       */ 
      char* fileptr = mmap(0, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
      close(fd);
      if (fileptr==MAP_FAILED) {
	perror("mmap");
	continue;
      }
      
      char* p = fileptr;
      for(size_t i=0;i<filesize;++i) {
	*p = toupper(*p);
	++p;
      }
      munmap(fileptr, filesize);  // implicit msync

      fprintf(stdout, "%s done\n", filename);
      fflush(stdout);
      continue;
    }
    if (r==0) 
      fprintf(stderr, "%s is not a regular file\n", filename);
    else 
      perror("isRegular");    
  }
  return 0;
}
