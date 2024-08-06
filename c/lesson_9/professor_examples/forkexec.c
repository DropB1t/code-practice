#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(int argc, char* argv[]) {

  if (argc != 2) {
    fprintf(stderr, "usa %s nomedir\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const char* dirname = argv[1];
  struct stat statbuf;

  if (stat(dirname, &statbuf) == -1) {
    perror("stat");
    fprintf(stderr, "errore nella stat per il file %s\n", dirname);
    exit(EXIT_FAILURE);
  }

  if (!S_ISDIR(statbuf.st_mode)) {
    fprintf(stderr, "%s non e' una directory\n", dirname);
    exit(EXIT_FAILURE);
  }
  
  pid_t pid = fork();
  if (pid == 0) { // processo figlio
    execlp("ls", "ls", "-al", dirname, (char*)NULL);
    perror("execlp");
    exit(EXIT_FAILURE);
  }
  // processo padre

  int status;
  if (waitpid(pid, &status, 0) == -1) {
    perror("waitpid");
    exit(EXIT_FAILURE);    
  }

  if (WIFEXITED(status) ) {
    // terminato con una exit
    fprintf(stdout, "Il processo figlio di pid %d terminato con exit(%d)\n",
	    pid, WEXITSTATUS(status));
  } else {
    fprintf(stdout, "Il processo figlio di pid %d terminato NON con una exit\n",
	    pid);
  }

  return 0;   // exit(EXIT_SUCCESS);
      
}