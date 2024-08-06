#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "utils/includes/util.h"
#include "unboundedqueue.h"

#define EOS (void *)0x1
#define MAX_LINE_SIZE 1024

typedef struct
{
    int *semi_sums;
    int len;
    int last;
    sem_t sem;
    Queue_t *q;
} th_struct_t;

static void *Worker(void *arg)
{
    th_struct_t *t_struct = arg;
    while (1)
    {
        char *filename = NULL;
        filename = pop(t_struct->q);
        if (filename == EOS)
        {
            break;
        }
        FILE *in = NULL;
        if ((in = fopen((const char *)filename, "r")) == NULL)
        {
            perror("fopen");
            return NULL;
        }

        int sum[2];
        int i = 0;

        char *line = NULL;
        line = malloc(MAX_LINE_SIZE);
        while (line && fgets(line, MAX_LINE_SIZE, in) != NULL)
        {
            line[strlen(line) - 1] = '\0';
            if (strlen(line) == 0)
                continue;
            char *tmpstr;
            char *token = strtok_r(line, ",", &tmpstr);
            while (token)
            {
                long val = 0;
                if (isNumber(token, &val) != 0)
                {
                    perror("NaN");
                }
                sum[i % 2] += (int)val;
                i++;
                token = strtok_r(NULL, ",", &tmpstr);
            }
            free(line);
            i = 0;
            line = malloc(MAX_LINE_SIZE);
        }

        P(&t_struct->sem);
        t_struct->semi_sums[t_struct->last] = sum[0];
        t_struct->last++;
        t_struct->semi_sums[t_struct->last] = sum[1];
        t_struct->last++;
        V(&t_struct->sem);

        if (line)
            free(line);
        free(filename);
        fclose(in);
    }
    push(t_struct->q, EOS);
    return NULL;
}

int main(int argc, char const *argv[])
{

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <N> filename ...\n", argv[0]);
        return -1;
    }

    long N = 10;
    if (isNumber(argv[1], &N) != 0)
    {
        perror("NaN");
    }

    th_struct_t *th_struct = malloc(sizeof(th_struct_t));
    assert(th_struct);

    th_struct->len = (argc - 2) * 2;
    th_struct->last = 0;
    th_struct->semi_sums = malloc(sizeof(int) * th_struct->len);
    assert(th_struct->semi_sums);

    int r;
    SYSCALL_EXIT("sem_init", r, sem_init(&th_struct->sem, 1, 1), "sem_init sem");

    th_struct->q = initQueue();
    assert(th_struct->q);

    pthread_t th[N];
    for (size_t i = 0; i < N; i++)
    {
        if (pthread_create(&th[i], NULL, Worker, th_struct) != 0)
        {
            fprintf(stderr, "pthread_create failed (Worker n.%ld)\n", i);
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i = 2; i < argc; i++)
    {
        const char *filename = argv[i];
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
        char *tout = strdup(filename);
        push(th_struct->q, tout);
    }
    push(th_struct->q, EOS);

    for (size_t i = 0; i < N; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            fprintf(stderr, "pthread_join failed (Worker n.%ld)\n", i);
            return (EXIT_FAILURE);
        }
    }

    int col1 = 0, col2 = 0;
    P(&th_struct->sem);
    for (size_t i = 0; i < th_struct->len - 1; i = i + 2)
    {
        col1 += th_struct->semi_sums[i];
        col2 += th_struct->semi_sums[i + 1];
    }
    V(&th_struct->sem);

    printf("Column 1 -> %d \n", col1);
    printf("Column 2 -> %d \n", col2);

    free(th_struct->semi_sums);
    deleteQueue(th_struct->q);
    free(th_struct);

    return 0;
}
