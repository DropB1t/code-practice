#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
    int channel1[2], channel2[2];
    if (pipe(channel1) < 0)
    {
        perror("pipe");
        return errno;
    }
    if (pipe(channel2) < 0)
    {
        perror("pipe");
        return errno;
    }
    pid_t pid1, pid2, pid3;
    int i = 1;
    while (i <= 2)
    {
        switch (i)
        {
        case 1:
        {
            if ((pid1 = fork()) == 0)
            {
                if (dup2(channel1[1], STDOUT_FILENO) < 0)
                {
                    perror("dup2");
                    closechannel(channel1);
                    return errno;
                }
                close(channel1[0]);
                execlp("cat", "cat", "/etc/passwd", NULL);
                perror("execlp");
                return errno;
            }
            else if (pid1 == -1)
            {
                perror("Fork first child");
                return errno;
            }
        }
        case 2:
        {
            if (dup2(channel1[0], STDIN_FILENO) < 0)
            {
                perror("dup2");
                closechannel(channel2);
                return errno;
            }
            close(channel1[1]);
            if ((pid2 = fork()) == 0)
            {
                if (dup2(channel2[1], STDOUT_FILENO) < 0)
                {
                    perror("dup2");
                    closechannel(channel2);
                    return errno;
                }
                close(channel2[0]);
                execlp("cut", "cut", "-d", ":", "-f", "1", NULL);
                perror("execlp");
                return errno;
            }
            else if (pid2 == -1)
            {
                perror("Fork second child");
                return errno;
            }
        }
        case 3:
        {
            if ((pid3 = fork()) == 0)
            {
                if (dup2(channel2[0], STDIN_FILENO) < 0)
                {
                    perror("dup2");
                    closechannel(channel2);
                    return errno;
                }
                close(channel2[1]);
                execlp("sort", "sort", NULL);
                perror("execlp");
                return errno;
            }
            else if (pid3 == -1)
            {
                perror("Fork third child");
                return errno;
            }
        }
        default:;
        }
        i += 1;
    }

    closechannel(channel1);
    closechannel(channel2);
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
    if (waitpid(pid3, NULL, 0) < 0)
    {
        perror("waitpid pid1");
        return -1;
    }
    return 0;
}