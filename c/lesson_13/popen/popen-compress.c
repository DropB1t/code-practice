#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <util.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#if !defined MAX_FILENAME_LENGTH
#define MAX_FILENAME_LENGTH 255
#endif

#define CHUNKSIZE 16384

static inline void closechannel(int channel[2])
{
  close(channel[0]);
  close(channel[1]);
}

int main(int argc, char *argv[])
{
  if (argc == 1)
  {
    fprintf(stderr, "use: %s file [file]\n", argv[0]);
    return EXIT_FAILURE;
  }

  const int len = MAX_FILENAME_LENGTH + 64;

  char *buffer = malloc(CHUNKSIZE);
  if (!buffer)
  {
    perror("malloc");
    return EXIT_FAILURE;
  }

  for (int i = 1; i < argc; ++i)
  {
    const char *filename = argv[i];
    int r;
    if ((r = isRegular(filename, NULL)) == 1)
    {
      FILE *fin = fopen(filename, "r");
      if (fin == NULL)
      {
        perror("fopen");
        continue;
      }

      // Qui faccio fork e setto la pipe

      pid_t pid;
      int channel[2];
      if (pipe(channel) < 0)
      {
        perror("pipe");
        return errno;
      }

      char str[len];
      snprintf(str, len, "%s.gz", filename);

      if ((pid = fork()) == 0)
      {
        if (dup2(channel[0], STDIN_FILENO) < 0)
        {
          perror("dup2");
          closechannel(channel);
          return errno;
        }
        close(channel[1]);

        int fd = open(str, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        dup2(fd, STDOUT_FILENO); // make stdout go to file
        close(fd);

        execlp("gzip", "gzip", "-", NULL);
        perror("execlp");
        return errno;
      }
      else if (pid == -1)
      {
        perror("Fork first child");
        return errno;
      }

      if (dup2(channel[1], STDOUT_FILENO) < 0)
      {
        perror("dup2");
        closechannel(channel);
        return errno;
      }
      close(channel[0]);

      /* char str[len];
      snprintf(str, len, "gzip - -c > %s.gz", filename);
      FILE *fpipe;
      if ((fpipe = popen(str, "w")) == NULL)
      {
        perror("popen");
        continue;
      } */

      while (!feof(fin))
      {
        size_t sz = fread(buffer, 1, CHUNKSIZE, fin);
        if (sz > 0)
          /* if (fwrite(buffer, 1, sz, fpipe) != sz)
          {
            perror("fwrite");
            break;
          } */
          // Qui leggo con read dalla pipe (Oppure uso le dup)
          // write(STDOUT_FILENO,buffer,sz)
          if (write(STDOUT_FILENO, buffer, sz) != sz)
          {
            perror("fwrite");
            break;
          }
      }
      // Qui chiudo la pipe
      close(channel[1]);
      fclose(fin);
      /* if (pclose(fpipe) == -1)
      {
        perror("pclose");
      } */
      continue;
    }
    if (r == 0)
      fprintf(stderr, "%s is not a regular file\n", filename);
    else
      perror("isRegular");
  }

  free(buffer);
  return 0;
}
