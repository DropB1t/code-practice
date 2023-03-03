#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char const *argv[])
{
  const long N = 5000000;

  struct timeval start, stop;
  long double us;

  pthread_mutex_t lock;

  if (gettimeofday(&start, NULL) == -1)
  {
    perror("gettimeofday start");
    return EXIT_FAILURE;
  }

  for (long i = 0; i < N; i++)
  {
    pthread_mutex_lock(&lock);
    pthread_mutex_unlock(&lock);
  }

  if (gettimeofday(&stop, NULL) == -1)
  {
    perror("gettimeofday close");
    return EXIT_FAILURE;
  }

  us = (long double)(stop.tv_sec * 1000000L + stop.tv_usec) - (start.tv_sec * 1000000L + start.tv_usec);

  printf("%Lf\n", us / N);

  return EXIT_SUCCESS;
}
