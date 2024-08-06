#include <signal.h>
#include <stdio.h>
#include <unistd.h> 

int main(void)
{
    alarm(3);
    printf("Inizio il ciclo infinito ...\n") ;
    while (1) ;
    printf("Pippo\n") ;
    return 0 ;
}