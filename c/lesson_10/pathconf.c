#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>


int main() {
    int p[2];

    if (pipe(p)<0) {
        perror("pipe");
        return -1;
    }
    long v;
    errno=0;
    if ((v=fpathconf(p[0], _PC_PIPE_BUF)) == -1) {
	if (errno == 0) printf("non determinato\n");
	else {
	    perror("fpathconf");    
	    return -1;
	}
    }
    printf("N. di bytes che posso scrivere atomicamente: %ld\n", v);
    printf("secondo lo standard POSIX (_POSIX_PIPE_BUF): %d\n", _POSIX_PIPE_BUF);
    
    printf("Questa è la capacità della pipe: %d\n", fcntl(p[0], F_GETPIPE_SZ));
}
	    
	    
    
