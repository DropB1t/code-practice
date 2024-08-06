/*
 * Altra versione del programma shm-prodcons.c in cui si utilizzavano
 * due pipe senza nome per la sincronizzazione tra client e server.
 *
 * In questa versione (il client invia file al server da comprimere via gzip),
 * sia i dati del file, che il nome che la size sono inviati al server
 * tramite un segmento di memoria condivisa (SHM).
 * La sincronizzazione avviene invece con un semaforo POSIX (sem) anch'esso
 * contenuto nel segmento condiviso.
 *
 *                         SHM segment
 *                       ----------------
 *                      |    sem1, sem2  |
 *         ------------ |    filename    | <----------
 *        |             |    filesize    |            |
 *        |             |  dati del file |            |
 *        |              ----------------             |
 *        |                                           |
 *        v                                           |
 *      server                                     client
 *    Consumatore                                 Produttore
 *
 * La terminazione viene gestite inviando al server una filesize minore di 0.
 *
 */
#define _GNU_SOURCE
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <semaphore.h>

#include "utils/includes/util.h"

#define MAX_FILE_SIZE 1048576 /* 1M */

// rappresentazione del segmento condiviso
typedef struct
{
  sem_t sem1;                         // semaforo utilizzato per la sincronizzazione client -> server
  sem_t sem2;                         // semaforo utilizzato per la sincronizzazione server -> client
  char filename[MAX_FILENAME_LENGTH]; // nome del file
  size_t filesize;                    // dimensione del file
  char data[MAX_FILE_SIZE];           // dati del file
} shmsegment_t;

static int mapfile_and_copyto(const char filename[], size_t filesize, char *destptr)
{
  if (destptr == NULL)
  {
    DBG("wrong destptr parameter in mapfile_and_copyto\n");
    errno = EINVAL;
    perror("mapfile_and_copyto");
    return -1;
  }
  int fd;
  SYSCALL_RETURN("open", fd, open(filename, O_RDONLY), "open - mapfile_and_copyto");

  char *fileptr = mmap(0, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
  if (fileptr == MAP_FAILED)
  {
    perror("mmap");
    return -1;
  }

  close(fd);
  memcpy(destptr, fileptr, filesize);
  SYSCALL_RETURN("munmap", fd, munmap(fileptr, filesize), "munmap - mapfile_and_copyto");
  return 0;
}

void client(shmsegment_t *shmptr)
{
  const char exit_command1[] = "exit";
  char filename[MAX_FILENAME_LENGTH];

  for (;;)
  {
    memset(filename, '0', MAX_FILENAME_LENGTH);

    fprintf(stdout, "Inserisci un file path (o 'exit'): ");
    fflush(stdout);

    if (fgets(filename, MAX_FILENAME_LENGTH, stdin) == NULL)
    {
      perror("fgets");
      return;
    }
    if (strncmp(filename, exit_command1, strlen(exit_command1)) == 0)
    {
      DBG("CLIENT closing\n");
      break;
    }
    filename[strlen(filename) - 1] = '\0'; // tolgo '\n'

    size_t filesize;
    errno = 0;
    if (isRegular(filename, &filesize) != 1)
    {
      if (errno == 0)
      {
        fprintf(stderr, "%s non e' un file regolare\n", filename);
        continue;
      }
      perror("isRegular");
      continue;
    }
    if (filesize > MAX_FILE_SIZE)
    {
      fprintf(stderr, "il file %s e' troppo grande, il limite e' %d byte\n", filename, MAX_FILE_SIZE);
      continue;
    }

    P(&shmptr->sem1); // aspetto che il server abbia finito

    strncpy(shmptr->filename, filename, MAX_FILENAME_LENGTH); // copio il nome del file
    shmptr->filesize = filesize;

    // copio il file nel segmento condiviso
    int r = mapfile_and_copyto(filename, filesize, shmptr->data);
    if (r == -1)
      continue;

    V(&shmptr->sem2); // dico al server che puo' partire
  }

  // sincronizzazione di terminazione
  P(&shmptr->sem1);
  shmptr->filesize = -1;
  V(&shmptr->sem2);
}

void server(shmsegment_t *shmptr)
{
  const int len = MAX_FILENAME_LENGTH + 64;

  V(&shmptr->sem1); // dico al client che puo' partire

  for (;;)
  {
    P(&shmptr->sem2); // attendo il segnale di partenza dal client
    if (shmptr->filesize == -1)
      break;

    DBG("SERVER received data from client %s (%ld)\n", shmptr->filename, shmptr->filesize);

    // ------ comprimiamo il file con gzip usando popen
    char str[len];
    snprintf(str, len, "gzip - -c > %s.gz", shmptr->filename);
    FILE *fpipe;
    if ((fpipe = popen(str, "w")) == NULL)
      perror("popen");
    if (fwrite(shmptr->data, 1, shmptr->filesize, fpipe) != shmptr->filesize)
      perror("fwrite");
    if (pclose(fpipe) == -1)
      perror("pclose");
    // -------------------------------------------------

    // sync with client
    V(&shmptr->sem1); // dico al client che ho finito
  }

  DBG("SERVER terminating2\n");
}

int main(int argc, char *argv[])
{
  const char SHMNAME[] = "/shmem-prodcons";
  int r, fd;

  SYSCALL_EXIT("shm_open", fd, shm_open(SHMNAME, O_CREAT | O_TRUNC | O_RDWR, 0600), "shm_open");
  SYSCALL_EXIT("ftruncate", r, ftruncate(fd, sizeof(shmsegment_t)), "ftruncate");

  shmsegment_t *shmptr = mmap(0, sizeof(shmsegment_t),
                              PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);
  if (shmptr == MAP_FAILED)
  {
    perror("mmap");
    return EXIT_FAILURE;
  }
  // inizializzo i semafori in modo che siano condivisi da tutti i processi
  SYSCALL_EXIT("sem_init", r, sem_init(&shmptr->sem1, 1, 0), "sem_init sem1");
  SYSCALL_EXIT("sem_init", r, sem_init(&shmptr->sem2, 1, 0), "sem_init sem2");
  pid_t pid;
  SYSCALL_EXIT("fork", pid, fork(), "fork");

  if (pid == 0)
  { // client
    client(shmptr);
    DBG("CLIENT exiting %d\n", getpid());
    return EXIT_SUCCESS;
  }
  // server
  server(shmptr);

  SYSCALL_EXIT("waitpid", r, waitpid(pid, NULL, 0), "waitpid");
  SYSCALL_EXIT("shm_unlink", r, shm_unlink(SHMNAME), "shm_unlink");
  return EXIT_SUCCESS;
}
