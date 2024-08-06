/*
 * Utilizzo di pipe fork dup2. Versione con 2 pipe.
 *
 * Programma che implementa l'equivalente dei seguenti comandi Bash:
 *
 *  echo "Ciao mondo ... Passo e chiudo !"   |    tr ' ' '\n'     |        LC_ALL=C sort
 *                                       canale1[0]            canale2[0]
 *                                       canale1[1]            canale2[1]
 *
 *            main                              figlio1         figlio2
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

int main(int argc, char *argv[])
{
  int canale1[2],r;
  SYSCALL_EXIT("pipe", r, pipe(canale1), "pipe1");
  pid_t pid1, pid2, pid3;

  if ((pid1 = fork()) == 0)
  {

    int canale2[2];
    SYSCALL_EXIT("pipe", r, pipe(canale2), "pipe2");

    if ((pid2 = fork()) == 0)
    {
      SYSCALL_EXIT("dup2 1", r, dup2(canale1[0], STDIN_FILENO), "dup2 figlio1 1");
      SYSCALL_EXIT("dup2 2", r, dup2(canale2[1], STDOUT_FILENO), "dup2 figlio1 2");
      close(canale1[1]);
      close(canale2[0]);
      execlp("tr", "tr", " ", "\n", (char *)NULL);
      perror("execlp");
      exit(errno);
    }
    SYSCALL_EXIT("dup2 3", r, dup2(canale2[0], STDIN_FILENO), "dup2 figlio2");
    if ((pid3 = fork()) == 0)
    {
      
      close(canale2[1]);
      close(canale1[0]);
      close(canale1[1]);
      
      char *path = getenv("PATH");
      char envpath[strlen("PATH=") + strlen(path) + 1];
      char envlcall[] = "LC_ALL=C";
      snprintf(envpath, sizeof(envpath), "PATH=%s", path);
      char *envp[] = {envpath, envlcall, NULL};
      char *cmd[] = {"/usr/bin/sort", NULL};

      execve(cmd[0], cmd, envp);
      perror("execve");
      exit(errno);
    }
    close(canale1[0]);
    close(canale1[1]);
    close(canale2[0]);
    close(canale2[1]);
    SYSCALL_EXIT("waitpid", r, waitpid(pid2, NULL, 0), "waitpid1");
    SYSCALL_EXIT("waitpid", r, waitpid(pid3, NULL, 0), "waitpid1");
  }

  SYSCALL_EXIT("dup2 4", r, dup2(canale1[1], STDOUT_FILENO), "dup2");

  // chiudo tutti i descrittori che non uso
  close(canale1[0]);
  //close(canale1[1]);

  printf("Ciao mondo ... Passo e chiudo !");
  fflush(stdout);

  // DEVO chiudere l'output prima di attendere la terminazione
  close(1);

  // attendo la terminazione dei processi figli
  SYSCALL_EXIT("waitpid", r, waitpid(pid1, NULL, 0), "waitpid1");
  return 0;
}
