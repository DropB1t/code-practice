// Yuriy Rymarchuk 614484

#define _GNU_SOURCE
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <dirent.h>

#include "merge.h"          //optional
#include "unboundedqueue.h" //optional
#include "util.h"

#define EOS (void *)0x1
#define MIN_N_THREADS 1
#define NUM_FILES 100

typedef struct
{
    char *dirname;
    int N;
    Queue_t *ptrs;
    Queue_t *q;
} th_struct_t;

int cmpfunc(const void *a, const void *b)
{
    if (*(int *)a - *(int *)b < 0)
        return -1;
    if (*(int *)a - *(long *)b > 0)
        return 1;
    return 0;
}

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
        char *f_path = strcat(t_struct->dirname, "/");
        f_path = strcat(f_path, filename);

        int r;
        size_t filesize;
        if ((r = isRegular(f_path, &filesize)) == 1)
        {
            int fd;
            SYSCALL_EXIT("open", fd, open(f_path, O_RDONLY), "open - mapfile_and_copyto");
            long *arr = malloc(sizeof(long) * t_struct->N);
            arr = mmap(0, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
            if (arr == MAP_FAILED)
            {
                perror("mmap");
                return NULL;
            }
            close(fd);

            long *arr_ptr = malloc(sizeof(long) * t_struct->N);
            memcpy(arr_ptr, arr, sizeof(long) * t_struct->N);
            SYSCALL_EXIT("munmap", fd, munmap(arr, filesize), "munmap - mapfile_and_copyto");

            qsort(arr_ptr, t_struct->N, sizeof(long), cmpfunc);

            for (size_t i = 0; i < t_struct->N; i++)
            {
                printf("->%ld\n", arr_ptr[i]);
            }
            // push(t_struct->ptrs, "Ciao");
            // printf("Return -> %d",push(t_struct->ptrs, arr_ptr));
        }
        if (r == 0)
            fprintf(stderr, "%s is not a regular file\n", f_path);
        /* if(f_path)
            free(f_path); */
    }

    push(t_struct->q, EOS);
    return NULL;
}

int main(int argc, char const *argv[])
{

    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <T Threads> <N dim file> <dir>\n", argv[0]);
        return -1;
    }

    long T = MIN_N_THREADS;
    if (isNumber(argv[1], &T) != 0)
    {
        perror("NaN");
    }

    long N = 0;
    if (isNumber(argv[2], &N) != 0)
    {
        perror("NaN");
    }

    struct stat statbuf;
    const char *dirname = argv[3];

    if ((stat(dirname, &statbuf) == -1))
    {
        fprintf(stderr,
                "Cannot access directory %s\n",
                argv[1]);
        return EXIT_FAILURE;
    }

    if (!S_ISDIR(statbuf.st_mode))
    {
        fprintf(stderr, "%s is not a directory\n", dirname);
        return EXIT_FAILURE;
    }

    th_struct_t *th_struct = malloc(sizeof(th_struct_t));
    assert(th_struct);

    th_struct->dirname = strdup(dirname);
    th_struct->N = N;

    th_struct->q = initQueue();
    assert(th_struct->q);

    th_struct->ptrs = initQueue();
    assert(th_struct->ptrs);

    pthread_t th[T];
    for (size_t i = 0; i < T; i++)
    {
        if (pthread_create(&th[i], NULL, Worker, th_struct) != 0)
        {
            fprintf(stderr, "pthread_create failed (Worker n.%ld)\n", i);
            exit(EXIT_FAILURE);
        }
    }

    if (chdir(dirname) == -1)
    {
        fprintf(stderr, "Cannot enter into %s directory: %s\n", dirname, strerror(errno));
        exit(EXIT_FAILURE);
    }
    DIR *dir;
    if ((dir = opendir(".")) == NULL)
    {
        fprintf(stderr, "Cannot open directory '%s': %s\n",
                dirname, strerror(errno));
        exit(EXIT_FAILURE);
    }
    struct dirent *dp;
    while ((errno = 0, dp = readdir(dir)) != NULL)
    {
        if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
        {
            continue;
        }
        char *tout = strdup(dp->d_name);
        push(th_struct->q, tout);
    }
    if (errno != 0)
    {
        perror("readdir");
    }
    if (chdir("..") == -1)
    {
        fprintf(stderr, "Unable to trace back to parent directory\n");
        exit(EXIT_FAILURE);
    }
    if (closedir(dir))
    {
        fprintf(stderr, "Could not close '%s': %s\n",
                dirname, strerror(errno));
        exit(EXIT_FAILURE);
    }
    free(dp);
    push(th_struct->q, EOS);

    for (size_t i = 0; i < T; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            fprintf(stderr, "pthread_join failed (Worker n.%ld)\n", i);
            return (EXIT_FAILURE);
        }
    }
    // deleteQueue(th_struct->q);
    // deleteQueue(th_struct->ptrs);

    long *merged = NULL;
    int len = 0;
    // printf("Qui %ld\n", length(th_struct->ptrs));
    for (size_t i = 0; i < 0; i++)
    {
        len = th_struct->N * i;
        long *arr_ptr = (long *)pop(th_struct->ptrs);
        merged = mergeAndFree(merged, len, arr_ptr, th_struct->N);
    }

    for (size_t i = 0; i < len; i++)
    {
        printf("->%ld\n", merged[i]);
    }

    return 0;
}
