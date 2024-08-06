// in questa versione bc rimane in esecuzione e non viene
// lanciato ogni volta
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/wait.h>

#include <util.h>

#define MAX_LINE_SIZE 1024


int main(int argc, char *argv[]) {
  const char exit_command1[] = "exit";
  const char exit_command2[] = "quit";
  char result[BUFSIZE];
  int tobc[2];
  int frombc[2];

  int r;
  SYSCALL_EXIT(pipe, r, pipe(tobc), "pipe1");
  SYSCALL_EXIT(pipe, r, pipe(frombc), "pipe2");
  
  if (fork() == 0) {
    
    // chiudo i descrittori che non uso
    SYSCALL_EXIT(close, r, close(tobc[1]), "close");  
    SYSCALL_EXIT(close, r, close(frombc[0]), "close");
    
    
    SYSCALL_EXIT(dup2, r, dup2(tobc[0],0),   "dup2 child", "stdin");
    // redirigo sia stdout che stderr sulla stessa pipe
    SYSCALL_EXIT(dup2, r, dup2(frombc[1],1), "dup2 child", "stdout");
    SYSCALL_EXIT(dup2, r, dup2(frombc[1],2), "dup2 child", "stderr");
    
    
    execl("/usr/bin/bc", "bc", "-lq", NULL);
    perror("execl");
    return EXIT_FAILURE;
  }
  // chiudo i descrittori che non uso
  SYSCALL_EXIT(close, r, close(tobc[0]), "close");
  SYSCALL_EXIT(close, r, close(frombc[1]), "close");

  while(1) {      
    char line[MAX_LINE_SIZE];
    
    memset(line, '0', MAX_LINE_SIZE);
    if (fgets(line, MAX_LINE_SIZE, stdin) == NULL ) {
      perror("fgets");
      return -1;
    }
    if (strncmp(line, exit_command1, strlen(exit_command1)) == 0)  break;
    if (strncmp(line, exit_command2, strlen(exit_command2)) == 0)  break;

    int n;
    SYSCALL_EXIT(write, n, write(tobc[1], (char*)line,strlen(line)), "write");
    SYSCALL_EXIT(read, n, read(frombc[0], result, BUFSIZE), "readn");  // leggo il risultato o l'errore

    result[n] = '\0';
    
    fprintf(stdout, "Operazione: %s", line);
    fprintf(stdout, "Risultato : %s\n", result);
  }

  SYSCALL_EXIT(close, r, close(tobc[1]), "close");  // si chiude lo standard input di bc cosi' da farlo terminare
  SYSCALL_EXIT(wait, r, wait(NULL), "wait");
  
  return 0;
}
