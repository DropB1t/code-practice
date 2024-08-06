// Yuriy Rymarchuk 614484

#define _GNU_SOURCE
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/mman.h>

#include "util.h"

#define MAX_FILE_SIZE 3145728 //1048576 1MB - 3145728 3M - 1024 1KB

typedef struct
{
    sem_t sem;
    char data[MAX_FILE_SIZE];
} shmsegment_t;

void writer_proc(shmsegment_t *shmptr, int pipe2[2])
{
    close(pipe2[1]);
    while (1)
    {
        
        long result;
        if ((read(pipe2[0], &result, sizeof(long))) == -1)
        {
            perror("read");
        }

        if (result == -2)
            break;

        DBG("SERVER received data from Worker (%ld)\n", result);
        printf("%ld\n", result);

        V(&shmptr->sem);
    }
    // ricevo -2 termino il processo
    close(pipe2[0]);
    DBG("SERVER terminating2\n");
}

void worker_proc(shmsegment_t *shmptr, int pipe1[2], int pipe2[2])
{
    // Chiudo Output della pipe 1 e Input della pipe2
    close(pipe1[1]);
    close(pipe2[0]);

    while (1)
    {
        size_t filesize;
        if ((read(pipe1[0], &filesize, sizeof(size_t))) == -1)
        {
            perror("read");
        }

        if (filesize == -2)
            break;

        DBG("SERVER received data from Reader (%ld)\n", filesize);
        // Qui processo file nella sh_mem
        int n = (int)(filesize / 8);
        long *file = malloc(sizeof(long) * n);
        memcpy(file, shmptr->data, filesize);
        long result = 0;
        for (size_t i = 0; i < n; i++)
        {
            result += (i * file[i]);
        }

        // Qui faccio la write sulla pipe2 del result;
        if ((write(pipe2[1], &result, sizeof(long))) == -1)
        {
            perror("Write Pipe2");
        }
    }
    // qui invio -1 alla pipe 2 per terminare Writer e chiudo la pipe
    long exit_p = -2;
    if ((write(pipe2[1], &exit_p, sizeof(long))) == -1)
    {
        perror("Write Pipe2");
    }
    close(pipe1[0]);
    close(pipe2[1]);
    DBG("WORKER terminating\n");
}

int main(int argc, char const *argv[])
{

    if (argc == 1)
    {
        fprintf(stderr, "use: %s file [file]\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char SHMNAME[] = "/shmem-yr";
    int r, fd;

    SYSCALL_EXIT("shm_open", fd, shm_open(SHMNAME, O_CREAT | O_TRUNC | O_RDWR, 0600), "shm_open");
    SYSCALL_EXIT("ftruncate", r, ftruncate(fd, sizeof(shmsegment_t)), "ftruncate");
    shmsegment_t *shmptr = mmap(0, sizeof(shmsegment_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    if (shmptr == MAP_FAILED)
    {
        perror("mmap");
        return EXIT_FAILURE;
    }

    SYSCALL_EXIT("sem_init", r, sem_init(&shmptr->sem, 1, 1), "sem_init sem");

    int pipe1[2];
    if (pipe(pipe1) < 0)
    {
        perror("pipe1");
        return errno;
    }

    int pipe2[2];
    if (pipe(pipe2) < 0)
    {
        perror("pipe2");
        return errno;
    }

    pid_t worker, writer;

    SYSCALL_EXIT("fork", worker, fork(), "fork");

    if (worker == 0)
    {
        worker_proc(shmptr, pipe1, pipe2);
        DBG("CLIENT exiting %d\n", getpid());
        return EXIT_SUCCESS;
    }

    SYSCALL_EXIT("fork", writer, fork(), "fork");

    if (writer == 0)
    {
        writer_proc(shmptr, pipe2);
        DBG("CLIENT exiting %d\n", getpid());
        return EXIT_SUCCESS;
    }

    close(pipe1[0]);
    close(pipe2[0]);
    close(pipe2[1]);

    for (int i = 1; i < argc; ++i)
    {
        P(&shmptr->sem);
        const char *filename = argv[i];
        int r;
        size_t filesize;
        if ((r = isRegular(filename, &filesize)) == 1)
        {
            if (filesize > MAX_FILE_SIZE)
            {
                fprintf(stderr, "Il file %s e' troppo grande, il limite e' %d byte\n", filename, MAX_FILE_SIZE);
                V(&shmptr->sem);
                continue;
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

            memcpy(shmptr->data, fileptr, filesize);
            SYSCALL_RETURN("munmap", fd, munmap(fileptr, filesize), "munmap - mapfile_and_copyto");
            if ((write(pipe1[1], &filesize, sizeof(size_t))) == -1)
            {
                perror("Write Pipe1");
            }
        }
        if (r == 0)
            fprintf(stderr, "%s is not a regular file\n", filename);
    }
    P(&shmptr->sem);
    long exit_p = -2;
    if ((write(pipe1[1], &exit_p, sizeof(long))) == -1)
    {
        perror("Write Pipe2");
    }
    close(pipe1[1]);

    DBG("Reader terminating\n");
    SYSCALL_EXIT("waitpid", r, waitpid(worker, NULL, 0), "waitpid");
    SYSCALL_EXIT("waitpid", r, waitpid(writer, NULL, 0), "waitpid");
	SYSCALL_EXIT("shm_unlink", r, shm_unlink(SHMNAME), "shm_unlink");
	return EXIT_SUCCESS;
}
