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
        fprintf(stderr, "Use %s dirname\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* dirname = argv[1];

    struct stat statbuf;

    if(stat(dirname,&statbuf) == -1)
    {
        perror("stat");
        fprintf(stderr,"Error in the file's stat %s\n",dirname);
        exit(EXIT_FAILURE);
    }

    if (!S_ISDIR(statbuf.st_mode))
    {
        fprintf(stderr,"%s is not a directory\n",dirname);
    }

    pid_t pid = fork();

    if (pid == 0) // Child process
    {
        execlp("ls", "ls", "-al", dirname, (char*)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    //Father process
    int status;
    if (waitpid(pid,&status,0) == -1)
    {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }
    
    if (WIFEXITED(status))//Terminated with an exit
    {
        fprintf(stdout,"Child process with pid %d terminated with exit(%d)\n",pid,WEXITSTATUS(status));
    }else {
        fprintf(stdout,"Child process with pid %d terminated without regular exit\n",pid); 
    }
    

    return 0;
}
