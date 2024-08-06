/*
 * Schema logico del programma.
 * Il client invia file al server che li comprime utilizzando gzip.
 * I dati del file (ed il nome del file) sono inviati al server
 * tramite un segmento di memoria condivisa.
 *
 *                         SHM segment
 *                       ----------------
 *         ------------ |  filename e    | <----------
 *        |             |  dati del file |            |
 *        |              ----------------             |
 *        |        |                         |        |
 *        v        | < ---- toserver ------- |        |
 *      server     |                         |      client
 *    consumatore  | ----- fromserver -----> |    Produttore
 *                 |                         |
 *
 * Le pipe sono utilizzate prevalentemente per sincronizzare client e server.
 * Nella pipe 'toserver' il client inizia una "transazione" inserendo la size del
 * file che intende inviare al server, quindi scrive nel segmento condiviso
 * il nome del file (stringa di lunghezza fissa) ed i dati del file.
 * Il client si mette in attesa di ricevere un 'ack' dal server che notifica
 * il completamento della transazione.
 *
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

#include <util.h>

static int mapfile_and_copyto(const char filename[], int filesize, char *destptr)
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

void client(char *shmptr, const size_t size, int toserver, int fromserver)
{
  const char exit_command1[] = "exit";
  char filename[MAX_FILENAME_LENGTH];
  char b = '\0';
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
    if (filesize > size)
    {
      fprintf(stderr, "il file %s e' troppo grande, il limite e' %ld byte\n", filename, size);
      continue;
    }

    memcpy(shmptr, filename, MAX_FILENAME_LENGTH); // copio il nome del file

    // copio il file nel segmento condiviso
    int r = mapfile_and_copyto(filename, filesize, shmptr + MAX_FILENAME_LENGTH);
    if (r == -1)
      continue;

    SYSCALL_PRINT("write", r, write(toserver, &filesize, sizeof(filesize)), "write toserver");
    if (r == -1)
      break;
    // sync with server
    SYSCALL_PRINT("read", r, read(fromserver, &b, sizeof(char)), "read fromserver");
    if (r <= 0)
      break;
  }
}

void server(char *shmptr, const size_t size, int fromclient, int toclient)
{
  char filename[MAX_FILENAME_LENGTH];
  const int len = MAX_FILENAME_LENGTH + 64;
  size_t filesize;
  const char b = '\0';
  int r;
  for (;;)
  {
    memset(filename, '0', MAX_FILENAME_LENGTH);
    SYSCALL_PRINT("read", r, read(fromclient, &filesize, sizeof(filesize)), "read fromclient");
    if (r <= 0)
    {
      DBG("SERVER closing\n");
      break;
    }
    assert(filesize <= size); // sanity check. NOTA: compilando con -DNDEBUG tutte le assert vengono tolte

    memcpy(filename, shmptr, MAX_FILENAME_LENGTH); // copio il nome del file

    DBG("SERVER received data from client %s (%ld)\n", filename, filesize);

    // ------ comprimiamo il file con gzip usando popen
    char str[len];
    snprintf(str, len, "gzip - -c > %s.gz", filename);
    FILE *fpipe;
    if ((fpipe = popen(str, "w")) == NULL)
      perror("popen");
    if (fwrite(shmptr + MAX_FILENAME_LENGTH, 1, filesize, fpipe) != filesize)
      perror("fwrite");
    if (pclose(fpipe) == -1)
      perror("pclose");
    // -------------------------------------------------

    // sync with client
    SYSCALL_PRINT("write", r, write(toclient, &b, sizeof(b)), "write toclient");
    if (r == -1)
      break;
  }
}

int main(int argc, char *argv[])
{
  const char SHMNAME[] = "/shmem-prodcons";
  const size_t size = 1048576; // 1MB
  char *buffer = NULL;
  int fd, unused;
  int toserver[2];
  int fromserver[2];

  SYSCALL_EXIT("pipe", unused, pipe(toserver), "pipe toserver");
  SYSCALL_EXIT("pipe", unused, pipe(fromserver), "pipe fromserver");

  SYSCALL_EXIT("shm_open", fd, shm_open(SHMNAME, O_CREAT | O_TRUNC | O_RDWR, 0600), "shm_open");
  SYSCALL_EXIT("ftruncate", unused, ftruncate(fd, MAX_FILENAME_LENGTH + size), "ftruncate");

  buffer = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);
  if (buffer == MAP_FAILED)
  {
    perror("mmap");
    return EXIT_FAILURE;
  }

  pid_t pid;
  SYSCALL_EXIT("fork", pid, fork(), "fork");

  if (pid == 0)
  { // client
    close(toserver[0]);
    close(fromserver[1]);
    client(buffer, size, toserver[1], fromserver[0]);
    close(toserver[1]);
    close(fromserver[0]);
    DBG("CLIENT exiting %d\n", getpid());
    return EXIT_SUCCESS;
  }
  // server
  close(toserver[1]);
  close(fromserver[0]);
  server(buffer, size, toserver[0], fromserver[1]);
  close(toserver[0]);
  close(fromserver[1]);

  SYSCALL_EXIT("waitpid", unused, waitpid(pid, NULL, 0), "waitpid");
  SYSCALL_EXIT("shm_unlink", unused, shm_unlink(SHMNAME), "shm_unlink");
  return EXIT_SUCCESS;
}
