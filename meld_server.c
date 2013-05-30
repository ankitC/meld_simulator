#include "csapp.h"

int main(int argc, char **argv)
{
	int listenfd, *clientfd, port;
	struct sockaddr_in clientaddr;
	unsigned int clientlen;
	fd_set active_fd_set, read_fd_set;
	pthread_t tid;
	int i;

	/*Handling the SIGPIPE signal*/
	Signal(SIGPIPE, SIG_IGN);
	/*Initialize the file desc set*/

	port=5000; 
	listenfd=Open_listenfd(port);

	FD_ZERO (&active_fd_set);
	FD_SET (listenfd, &active_fd_set);


	if(fork ()==0){
		sleep(2);
		Execve("client_vm",5000);
	}
	else{
		while(1){
			 read_fd_set = active_fd_set;
			 	if (select (FD_SETSIZE, &read_fd_set, NULL, NULL,NULL) < 0)
					{
						perror("select");
						exit(EXIT_FAILURE);
					}
					/* Service all the sockets with input pending. */
				for (i = 0; i < FD_SETSIZE; ++i)
					if (FD_ISSET (i,&read_fd_set))
					{
						
						
								/* Connection request on original socket. */
								clientlen=sizeof(clientaddr);
								clientfd=Calloc(1,sizeof(int));
								*clientfd=Accept(listenfd,(SA*)&clientaddr,&clientlen);
							
							fprintf	(stderr, "Server:connect from host %s, %port %%hd.\n",
									 inet_ntoa (clientname.sin_addr),
									 ntohs (clientname.sin_port));
								FD_SET(clientfd, &active_fd_set);
						
					}

		}


	/*while(1){

		clientlen=sizeof(clientaddr);
		clientfd=Calloc(1,sizeof(int));
		*clientfd=Accept(listenfd,
				(SA*)
				&clientaddr,
				&clientlen);
		if(*clientfd<0){
			continue;
		}
		Pthread_create(&tid,
				NULL,
				connection,
				clientfd);
	}*/
