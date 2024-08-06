#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define EC(sc,m) if(sc==-1){perror(m); exit(EXIT_FAILURE);}

volatile sig_atomic_t countINT=0;
volatile sig_atomic_t countTSTP=0;
volatile sig_atomic_t exitYes=1;


void handlerINT(int s){
	countINT++;	
}

void handlerTSTP(int s){
	char c;
	int r;
	printf("Ho ricevuto %d SIGINT\n",countINT);////NOT SIGNAL SAFE!!!!
	
	countTSTP++;
	
	if(countTSTP>=3){
		write(1,"Usciamo?y/n\n",13);

		exitYes=1;
		alarm(10);
		while((r=read(0,&c,sizeof(char)))==-1 && errno==EINTR){
			write(1,"Usciamo?y/n\n",13);
		}
				
		if(r==-1)
			_exit(EXIT_FAILURE);

		if(c=='y')
			_exit(EXIT_SUCCESS);
		else
			exitYes=0;

	}

}

void handlerALRM(int s){
		if(exitYes)
			_exit(EXIT_SUCCESS);
}

int main(){

	//installo dei handler SIGINT, SIGTSTP, SIGALRM
	struct sigaction s;
	sigset_t set;

	EC(sigfillset(&set),"fillset");
	EC(pthread_sigmask(SIG_SETMASK,&set,NULL),"sigmask");
	
	EC(sigemptyset(&set),"emptyset");
	sigaddset(&set,SIGINT);
	sigaddset(&set,SIGTSTP);

	EC(sigaction(SIGINT,NULL,&s),"sigaction1");
	
	s.sa_mask=set;
	s.sa_handler=handlerALRM;
	EC(sigaction(SIGALRM,&s,NULL),"sigaction4");

	s.sa_handler=handlerINT;
	EC(sigaction(SIGINT,&s,NULL),"sigaction2");

	s.sa_handler=handlerTSTP;
	EC(sigaction(SIGTSTP,&s,NULL),"sigaction3");

	EC(sigemptyset(&set),"emptyset");

	EC(pthread_sigmask(SIG_SETMASK,&set,NULL),"sigmask");

	//pause per aspettare segnali
	while(1){
		pause();
	}

	return 0;
}