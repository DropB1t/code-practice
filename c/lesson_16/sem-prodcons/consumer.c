#define _GNU_SOURCE
#include <util.h>
#include <shm-buffer.h>


int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "usa: %s /name numero-messaggi-da-leggere\n", argv[0]);
    return -1;
  }
  long nmessaggi;
  if (isNumber(argv[2], &nmessaggi) != 0) {
    fprintf(stderr, "usa: %s /name nmessaggi\n", argv[0]);
    fprintf(stderr, " 'nmessaggi' deve essere un intero\n");
    return -1;
  }
  if (nmessaggi <= 0) {
    fprintf(stderr, "usa: %s /name nmessaggi\n", argv[0]);
    fprintf(stderr, " 'nmessaggi' deve essere un intero >0\n");
    return -1;
  }
  
  buffer_t *buf;
  if ((buf=buffer_open(argv[1], 20, 800)) == NULL) { // riprovo per 20 volte attendendo 800us ogni volta
    fprintf(stderr, "Non e' stato possibile aprire il buffer\n");
    return -1;
  }
  fprintf(stdout, "CONSUMER: inizio a leggere i messaggi nel buffer\n");
  data_type data;
  for(int i=0;i<nmessaggi;++i) {
    if (get(buf, &data) == -1) {
      fprintf(stderr, "Errore nella lettura dal buffer\n");
      continue;
    }
    fprintf(stdout, "%s\n", data);
  }
  fprintf(stdout, "CONSUMER: ho finito di leggere %ld messaggi dal buffer\n", nmessaggi);

  buffer_close(buf);
  return 0;
}
