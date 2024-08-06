#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define IP "10.101.34.121"
#define PORT 7777

#define N 100

int main(){

	char buff[N];
	int sSk, sd,n, maxS,client;
	struct sockaddr_in server;

	fd_set sockets,readSockets;

	server.sin_family=AF_INET;
	server.sin_port=htons(PORT);
	server.sin_addr.s_addr=inet_addr(IP);
	
	sSk=socket(AF_INET,SOCK_STREAM,0);
	bind(sSk,(struct sockaddr *) &server, sizeof(server));
	listen(sSk,SOMAXCONN);

	FD_ZERO(&sockets);
	FD_SET(sSk,&sockets);
	maxS=sSk;

	while(1){
		readSockets=sockets;
		if((n=select(maxS+1,&readSockets,NULL,NULL,NULL))==-1){
				perror("select");
				continue;
			}
		fprintf(stderr," disponibili: %d \n",n);
		for (sd=0;sd<maxS+1;sd++){
			if(FD_ISSET(sd,&readSockets)){
			if(sd==sSk){
				client=accept(sSk,NULL,0);
				if(client==-1){perror("accept"); exit(EXIT_FAILURE);}
	
				fprintf(stderr,"client %d\n",client);
				FD_SET(client,&sockets);
				if(maxS<client)
					maxS=client;	
			} else {
				fprintf(stderr,"cliente manda un messaggio\n");

				n=read(sd,buff,N);
				if(n>0){
					buff[n]='\0';
					printf("Server ha ricevuto %s \n",buff);
					for (client=0;client<=maxS;client++){
						if(client!=sd && client!=sSk)
							write(client, buff,n);
					} 
				}
				if(n==0){
					FD_CLR(sd,&sockets);
					while(!FD_ISSET(maxS,&sockets))
						maxS--;
					close(sd);
				}

			}
		}
			
		}

	}
	
	close(sSk);
	
	return 0;

}