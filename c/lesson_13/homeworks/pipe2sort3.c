/*
 * Utilizzo di pipe fork dup2. Versione in cui la seconda pipe e' creata
 * dal primo processo figlio e non dal padre. 
 * Attenzione: il padre aspetta solo la terminazione del processo figlio 
 * (il processo che eseguira' 'tr') mentre il processo figlio forka altri 
 * due processi uno che esegue 'tr' ed uno che esegue 'sort'. In questo modo
 * il processo figlio aspetta la terminazione dei due processi.
 *
 *
 * Programma che implementa l'equivalente dei seguenti comandi Bash:
 *   
 *  echo "Ciao mondo ... Passo e chiudo !"   |    tr ' ' '\n'     |        LC_ALL=C sort
 *                                       canale1[0]            canale2[0]
 *                                       canale1[1]            canale2[1]
 * 
 *            main                                 figlio1                      figlio2 
 * 
 * vengono lanciati 2 processi figli, uno esegue 'tr' l'altro 'sort'.
 * Il processo main esegue la print del messaggio sullo standard 
 * output.
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
    int canale1[2];

    int r;
    SYSCALL_EXIT("pipe", r, pipe(canale1), "pipe1");

    pid_t pid1;
    if ((pid1=fork())==0) {  // figlio1 
      SYSCALL_EXIT("dup2", r,dup2(canale1[0], 0),"dup2 figlio1 1");

      int canale2[2];
      SYSCALL_EXIT("pipe", r, pipe(canale2), "pipe2");

      pid_t pid2;
      if ((pid2=fork())==0) {  // figlio2 
	SYSCALL_EXIT("dup2", r,dup2(canale2[0], 0),"dup2 figlio2");
	
	// chiudo tutti i descrittori che non uso
	close(canale2[1]);
	close(canale1[0]);  
	close(canale1[1]);

	

	// utilizzo la chiamata di sistema execve specificando le
	// variabili di ambente del processo sort
	char *path = getenv("PATH");
	char envpath[strlen("PATH=")+strlen(path)+1];
	char envlcall[] = "LC_ALL=C";
	snprintf(envpath, sizeof(envpath), "PATH=%s",path); 
	char *envp[] = { envpath, envlcall, NULL};
	char *cmd[]  = { "/usr/bin/sort", NULL };

	execve(cmd[0], cmd, envp);
	perror("execve");
	exit(errno);            
      }  

      SYSCALL_EXIT("dup2", r,dup2(canale2[1], 1),"dup2 figlio1 2");      
      // chiudo tutti i descrittori che non uso
      close(canale1[0]);
      close(canale1[1]);	
      close(canale2[0]);
      close(canale2[1]);

      // creo un ulteriore figlio che esegue 'tr'
      pid_t pid3;
      if ((pid3=fork()) == 0) {      
	execlp("tr", "tr", " ", "\n", (char*)NULL);
	perror("execlp");
	exit(errno);
      }
      // chiudo i descrittori che non uso
      close(0);
      close(1);

      // attendo la terminazione dei processi figli
      SYSCALL_EXIT("waitpid", r, waitpid(pid2, NULL, 0), "waitpid2");
      SYSCALL_EXIT("waitpid", r, waitpid(pid3, NULL, 0), "waitpid3");
      exit(EXIT_SUCCESS);
    }

    SYSCALL_EXIT("dup2", r,dup2(canale1[1], 1),"dup2");

    // chiudo tutti i descrittori che non uso
    close(canale1[0]);
    close(canale1[1]);	

    printf("Ciao mondo ... Passo e chiudo !");
    fflush(stdout);

    // DEVO chiudere l'output prima di attendere la terminazione
    close(1);

    // attendo la terminazione del processo figlio
    SYSCALL_EXIT("waitpid", r, waitpid(pid1, NULL, 0), "waitpid1");
    return 0;
}
