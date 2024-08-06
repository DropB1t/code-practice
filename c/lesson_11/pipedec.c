#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

static inline void closechannel(int channel[2])
{
    close(channel[0]);
    close(channel[1]);
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Use %s <int num>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int c1[2], c2[2];
    pid_t pid1, pid2;

    if (pipe(c1) < 0)
    {
        perror("pipe");
        return errno;
    }
    if (pipe(c2) < 0)
    {
        perror("pipe");
        return errno;
    }

    if ((pid1 = fork()) == 0)
    {
        if (dup2(c1[1], STDOUT_FILENO) < 0)
        {
            perror("dup2");
            closechannel(c1);
            return errno;
        }
        close(c1[0]);
        if (dup2(c2[0], STDIN_FILENO) < 0)
        {
            perror("dup2");
            closechannel(c2);
            return errno;
        }
        close(c2[1]);

        execl("/home/dropbit/Dev/C-Exercises/L34/dec", "dec", argv[1], NULL);
        perror("execlp");
        return errno;
    }
    else if (pid1 == -1)
    {
        perror("Fork first child");
        return errno;
    }

    if ((pid2 = fork()) == 0)
    {
        if (dup2(c1[0], STDIN_FILENO) < 0)
        {
            perror("dup2");
            closechannel(c1);
            return errno;
        }
        close(c1[1]);
        if (dup2(c2[1], STDOUT_FILENO) < 0)
        {
            perror("dup2");
            closechannel(c2);
            return errno;
        }
        close(c2[0]);

        execl("/home/dropbit/Dev/C-Exercises/L34/dec", "dec", NULL);
        perror("execlp");
        return errno;
    }
    else if (pid2 == -1)
    {
        perror("Fork first child");
        return errno;
    }

    closechannel(c1);
    closechannel(c2);
    if (waitpid(pid1, NULL, 0) < 0)
    {
        perror("waitpid pid1");
        return -1;
    }
    if (waitpid(pid2, NULL, 0) < 0)
    {
        perror("waitpid pid1");
        return -1;
    }
    return 0;
}
