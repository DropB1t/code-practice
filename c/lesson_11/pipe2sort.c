/*
 * Utilizzo di pipe fork dup. Versione base.
 *
 * Programma che produce in output l'equivalente dei seguenti comandi Bash:
 *   
 *   echo "Ciao mondo ... Passo e chiudo !" | tr ' ' '\n' | LC_ALL=C sort
 *
 * viene lanciato 1 processo figlio che esegue 'sort'.
 * Il processo main esegue le write del messaggio sullo standard output.
 *
 * Output atteso:
 * !
 * ...
 * Ciao
 * Passo
 * chiudo
 * e
 * mondo
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#include <util.h>

int main(int argc, char *argv[]) {    
    int canale[2];

    int r;
    SYSCALL_EXIT("pipe", r, pipe(canale), "pipe");
    pid_t pid;
    SYSCALL_EXIT("fork", pid, fork(), "fork");    
    if (pid == 0) {
      close(0);
      SYSCALL_EXIT("dup", r,dup(canale[0]),"dup figlio");
      // Le due chiamate precedenti possono essere
      // sostituite dalla chiamate seguente:
      // dup2(canale[0], 0);
      

      // chiudo tutti i descrittori che non uso prima di chiamare la exec
      close(canale[1]);	
      close(canale[0]);
      
      // utilizzo la chiamata di sistema execve specificando le
      // variabili di ambente del processo sort
      char *path = getenv("PATH");
      char envpath[strlen("PATH=")+strlen(path)+1];
      char envlcall[] = "LC_ALL=C";
      snprintf(envpath, sizeof(envpath), "PATH=%s",path); 
      char *envp[] = { envpath, envlcall, NULL};
      char *argv[]  = { "/usr/bin/sort", NULL };
      
      execve(argv[0], argv, envp);
      perror("execve");
      exit(errno);
    }
    
    close(1);
    SYSCALL_EXIT("dup", r,dup(canale[1]),"dup padre");
    
    // chiudo tutti i descrittori che non uso
    close(canale[1]);	
    close(canale[0]);


    
    // scrivo nella pipe
    SYSCALL_EXIT("write", r, write(1, "mondo\n", 7), "write1");
    SYSCALL_EXIT("write", r, write(1, "Ciao\n", 6), "write2");
    SYSCALL_EXIT("write", r, write(1, "!\n", 3), "write3");
    SYSCALL_EXIT("write", r, write(1, "Passo\n", 7), "write4");
    SYSCALL_EXIT("write", r, write(1, "e\n", 3), "write5");
    SYSCALL_EXIT("write", r, write(1, "chiudo\n", 8), "write6");
    SYSCALL_EXIT("write", r, write(1, "...", 4), "write7");
	    
    // DEVO chiudere l'output prima di attendere la terminazione
    close(1);

    // attendo la terminazione del processo figlio
    int status;
    SYSCALL_EXIT("waitpid", r, waitpid(pid, &status, 0), "waitpid");
    return 0;
}
