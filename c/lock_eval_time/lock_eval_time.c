#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

#define ITERATIONS 5000000
#define N_THREAD 50

int resource;
pthread_mutex_t lock;

void check(int test, const char *message, ...)
{
	if (test)
	{
		va_list args;
		va_start(args, message);
		vfprintf(stderr, message, args);
		va_end(args);
		fprintf(stderr, "\n");
		exit(EXIT_FAILURE);
	}
}

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

void *Worker(void *arg)
{

	struct timeval start, stop;

	if (gettimeofday(&start, NULL) == -1)
	{
		perror("gettimeofday start");
		return 0;
	}

	for (long i = 0; i < ITERATIONS; i++)
	{
		pthread_mutex_lock(&lock);
		resource++;
		pthread_mutex_unlock(&lock);
	}

	if (gettimeofday(&stop, NULL) == -1)
	{
		perror("gettimeofday close");
		return 0;
	}

	double *us = malloc(sizeof(double));
	*us = (double)delta_time(&stop, &start);

	return (void *)us;
}

int main(int argc, char const *argv[])
{
	int err;
	pthread_t th[N_THREAD];
	long res[N_THREAD];

	for (int i = 0; i < N_THREAD; i++)
	{
		err = pthread_create(&th[i], NULL, Worker, NULL);
		check(err != 0, "pthread_create failed (Worker n.%ld): %s", i, strerror(err));
	}

	double *us;
	double sum = 0.0;
	for (int i = 0; i < N_THREAD; i++)
	{
		err = pthread_join(th[i], (void **)&us);
		check(err != 0, "pthread_join failed (Worker n.%ld): %s", i, strerror(err));

		printf("\nThread n.%d\n", i + 1);
		printf("For loop: %.5f s [%.0f ms] [%.0f us]\n", *us / 1e+6, *us / 1000, *us);
		printf("For each lock: %f us\n", *us / ITERATIONS);

		sum += (*us / ITERATIONS);
		free(us);
	}

	sum = sum / N_THREAD;
	printf("\nAvg lock time [plus int increament]: %f\n", sum);

	return EXIT_SUCCESS;
}
