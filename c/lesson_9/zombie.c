#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{

    if(argc != 2)
    {
        fprintf(stderr, "Use %s N\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    for (size_t i = 0; i < atoi(argv[1]); i++)
    {
        pid_t pid = fork();
        if (pid == 0) // Child process
        {
            exit(EXIT_SUCCESS);
        }
    }

    //Father process
    sleep(30);
    return 0;
}
