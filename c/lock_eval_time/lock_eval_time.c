#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

long delta_time(struct timeval *now, struct timeval *before)
{
	time_t delta_seconds;
	time_t delta_microseconds;

	delta_seconds = now->tv_sec - before->tv_sec;
	delta_microseconds = now->tv_usec - before->tv_usec;

	if (delta_microseconds < 0)
	{
		delta_microseconds += 1000000;
		--delta_seconds;
	}

	return ((delta_seconds * 1000000) + delta_microseconds);
}

int main(int argc, char const *argv[])
{
	const long N = 50000000;

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

	us = (long double)delta_time(&stop,&start);

	printf("\nWhole for loop: %.0Lf us\n", us);
	printf("Each lock: %Lf us\n", us / N);

	return EXIT_SUCCESS;
}
