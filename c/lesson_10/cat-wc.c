// esegue usando una pipe come canale di comunicazione l'equivalente
// della pipe shell:  cat <filename> | wc -w 

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

static inline void closechannel(int channel[2]) {
  close(channel[0]);
  close(channel[1]);
}
int main(int argc, char* argv[]) {
  if (argc!= 2) {
    fprintf(stderr, "usa: %s <nome-file-testuale>\n", argv[0]);
    return -1;
  }
    
  int channel[2];    
  if (pipe(channel)<0) {
    perror("pipe");
    return errno;
  }
  pid_t pid1, pid2;
  switch(pid1=fork()) {
  case 0: { // primo figlio, eseguira' 'cat <nome-file>'
    if (dup2(channel[1], 1)<0) {
      perror("dup2");
      closechannel(channel);
      return errno;
    }
    // chiudere non e' stettamente necessario qui
    // ma e' comunque una buona pratica da seguire:
    // chiudere tutti i descrittori che non si usano
    closechannel(channel);
    
    execlp("cat","cat", argv[1], NULL);
    perror("execlp");
    return errno;	
  };
  case -1: {
    perror("forking primo figlio");
    return errno;
  } 
  default:;
  }
  switch(pid2=fork()) {
  case 0: {  // secondo figlio eseguira' 'wc -w'
    if (dup2(channel[0], 0)<0) {
      perror("dup2");
      closechannel(channel);
      return errno;
    }
    // qui e' fondamentale chiudere channel[1] altrimenti
    // l'EOF da parte del primo figlio non viene rilevato
    // da wc
    closechannel(channel);
    
    execlp("wc", "wc", "-w", NULL);
    perror("execlp");
    return errno;
  } 
  case -1: {
    perror("forking secondo figlio");
    return errno;
  } 
  default:;
  }
  // anche qui' e' fondamentale chiudere channel[1]
  closechannel(channel);
  
  // attendo la terminazione dei processi figli
  if (waitpid(pid1,NULL,0)<0) { perror("waitpid pid1"); return -1;}
  if (waitpid(pid2,NULL,0)<0) { perror("waitpid pid2"); return -1;}
  return 0;
}
