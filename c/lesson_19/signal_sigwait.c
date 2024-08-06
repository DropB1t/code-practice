#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define EC(sc,m) if(sc==-1){perror(m); exit(EXIT_FAILURE);}

int countINT=0;
int countTSTP=0;
volatile sig_atomic_t exitYes=1;


void handlerINT(){
	countINT++;	
}

void handlerTSTP(){
	char c;
	int r;
	printf("Ho ricevuto %d SIGINT\n",countINT);
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
	int sign;

	EC(sigfillset(&set),"fillset");
	EC(pthread_sigmask(SIG_SETMASK,&set,NULL),"sigmask");

	EC(sigaction(SIGALRM,NULL,&s),"sigaction1");
	s.sa_handler=handlerALRM;
	EC(sigaction(SIGALRM,&s,NULL),"sigaction2");


	EC(sigemptyset(&set),"emptyset");
	sigaddset(&set,SIGINT);
	sigaddset(&set,SIGTSTP);

	EC(pthread_sigmask(SIG_SETMASK,&set,NULL),"sigmask");

	sigaddset(&set,SIGALRM);


	while(1){
		sigwait(&set,&sign);
	
		switch(sign){
			case SIGINT:
				handlerINT();
				break;
			case SIGTSTP:
				handlerTSTP();
				break;
			case SIGALRM:
			 	break;
		}
	}
	return 0;
}