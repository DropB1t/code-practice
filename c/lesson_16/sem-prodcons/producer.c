#define _GNU_SOURCE

#include <util.h>
#include <shm-buffer.h>


int main(int argc, char *argv[]) {
  if (argc == 1) {
    fprintf(stderr, "usage: %s /name [msg1 msg2 ...]\n", argv[0]);
    return -1;
  }

  buffer_t *buf;
  if ((buf=buffer_create(argv[1])) == NULL) {
    fprintf(stderr, "Errore nella creazione del buffer condiviso\n");
    return -1;
  }
  if (argc > 2) {
    for(int i=2;i<argc;++i) {
      if (put(buf, argv[i]) == -1)
	fprintf(stderr, "Errore nell'inserimento nel buffer del messaggio %s\n", argv[i]); 
    }    
  } else {  
    data_type msg;
    for(;;) {
      memset(msg, '0', MAX_MSG_LENGTH);
      fprintf(stdout, "Inserisci un messaggio (o 'exit'): ");
      fflush(stdout);
      
      if (fgets(msg, MAX_MSG_LENGTH, stdin) == NULL ) {
	perror("fgets");
	break;
      }
      if (strncmp(msg, "exit", strlen("exit")) == 0) {
	DBG("CLIENT closing\n");
	break;
      }

      // tolgo lo '\n' finame
      msg[strlen(msg)-1] = '\0';
      
      if (put(buf, msg) == -1)
	fprintf(stderr, "Errore nell'inserimento nel buffer del messaggio %s\n", msg); 
    }
  }
  
  buffer_close(buf);

  return 0;
}
