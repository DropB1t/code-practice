//
// Semplice produttore-consumatore con buffer di una sola posizione.
// Versione con 1 sola variabile di condizione
//
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <util.h>

#define MAX_STRING_LEN 1024

typedef struct
{
	// risorsa condivisa (buffer di una sola posizione, memorizza una stringa)
	char buffer[MAX_STRING_LEN];
	char bufempty; // flag che indica se il buffer e' vuoto o meno

	// variabile di lock e di condizione per la sincronizzazione
	// sul buffer condiviso, inizializzate nel main
	pthread_mutex_t mutex;
	pthread_cond_t cond;
} buffer_t;

//
// get_buffer1(buffer_t *buf, char **localbuffer);
// put_buffer1(buffer_t *buf, char *localbuffer);
//

// produttore
void *Producer(void *arg)
{

	buffer_t *b = (buffer_t *)arg;
	char localbuffer[MAX_STRING_LEN];
	assert(b);

	for (;;)
	{

		fprintf(stdout, "Inserisci una stringa (o 'exit'): ");
		fflush(stdout);
		if (fgets(localbuffer, MAX_STRING_LEN, stdin) == NULL)
		{
			perror("fgets");
			return NULL;
		}
		localbuffer[strlen(localbuffer) - 1] = '\0';

		if (strncmp(localbuffer, "exit", strlen("exit")) == 0)
		{
			break;
		}

		//Sezione critica:
		LOCK(&b->mutex);
		while (!b->bufempty)
			WAIT(&b->cond, &b->mutex);

		strncpy(b->buffer, localbuffer, MAX_STRING_LEN);

		// b->buffer[strlen(b->buffer) - 1] = '\0'; // tolgo '\n'
		b->bufempty = 0;

		SIGNAL(&b->cond);
		UNLOCK(&b->mutex);
	}

	// produco un valore speciale per provocare la terminazione
	// del consumatore
	LOCK(&b->mutex);
	while (!b->bufempty)
		WAIT(&b->cond, &b->mutex);

	strncpy(b->buffer, "\n", 2); // suppongo che '\n' non sia mai nel buffer
	b->bufempty = 0;

	SIGNAL(&b->cond);
	UNLOCK(&b->mutex);

	DBG("Producer [%ld] closing\n", pthread_self());
	return NULL;
}

// consumatore
void *Consumer(void *arg)
{
	buffer_t *b = (buffer_t *)arg;
	assert(b);

	char mybuffer[MAX_STRING_LEN];
	mybuffer[0] = '\0';

	while (1)
	{
		LOCK(&b->mutex);
		while (b->bufempty)
			WAIT(&b->cond, &b->mutex);

		if (b->buffer[0] == '\n')
		{ // il produttore è terminato
			UNLOCK(&b->mutex);
			break;
		}

		strncpy(mybuffer, b->buffer, MAX_STRING_LEN);

		b->bufempty = 1;
		SIGNAL(&b->cond);
		UNLOCK(&b->mutex);

		fprintf(stdout, "Consumer, ricevuta stringa: %s\n", mybuffer);
		fflush(stdout);
	}

	DBG("Consumer [%ld] closing\n", pthread_self());
	return NULL;
}

int main()
{
	pthread_t thconsumer, thproducer;

	buffer_t *b;
	b = malloc(sizeof(buffer_t));
	assert(b);

	b->bufempty = 1;
	b->buffer[0] = '\0';

	if (pthread_mutex_init(&b->mutex, NULL) != 0)
	{
		fprintf(stderr, "pthred_mutex_init failed\n");
		return (EXIT_FAILURE);
	}
	if (pthread_cond_init(&b->cond, NULL) != 0)
	{
		fprintf(stderr, "pthread_cond_init failed\n");
		return (EXIT_FAILURE);
	}

	if (pthread_create(&thconsumer, NULL, Consumer, b) != 0)
	{
		fprintf(stderr, "pthread_create failed (Consumer)\n");
		return (EXIT_FAILURE);
	}
	if (pthread_create(&thproducer, NULL, Producer, b) != 0)
	{
		fprintf(stderr, "pthread_create failed (Producer)\n");
		return (EXIT_FAILURE);
	}
	if (pthread_join(thproducer, NULL) != 0)
	{
		fprintf(stderr, "pthread_join failed (Producer)\n");
		return (EXIT_FAILURE);
	}
	if (pthread_join(thconsumer, NULL) != 0)
	{
		fprintf(stderr, "pthread_join failed (Consumer)\n");
		return (EXIT_FAILURE);
	}

	pthread_cond_destroy(&b->cond);
	pthread_mutex_destroy(&b->mutex);

	free(b);

	return 0;
}
