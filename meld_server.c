 /* 
 *  A server using select()
 *  by Martin Broadhurst (www.martinbroadhurst.com)
 */

#include <stdio.h>
#include <string.h> /* memset() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "csapp.h"

static int port=5000;  /* Port to listen on */
#define BACKLOG     10  /* Passed to listen() */


void fork_clients(int num){

	char* port="5000";
	char countbuf[MAXLINE];
	char* path[3];
	path[0]=port;
	path[2]=NULL;
	
	int i=0;
	for(i=0; i<num; i++){
		sprintf(countbuf,"%d",i);
		path[1]=countbuf;
		if(fork()==0){
		execve("blocks",path,NULL);
		}
	printf("%d",i);
	} 
printf("Forked clients...\n");
}


void handle(int newsock, fd_set *set)
{
	rio_t rio_read;
    /* send(), recv(), close() */
    /* Call FD_CLR(newsock, set) on disconnection */
	char buf[MAXLINE];
	int bytecount;

	Rio_readinitb(&rio_read, newsock);
	bytecount=Rio_readlineb(&rio_read,buf,MAXLINE);
	printf("Server:%s\n",buf);

	sprintf(buf, "Ack.\n");
	Rio_writen(newsock,buf,strlen(buf)+1);
	FD_CLR(newsock,set);
	Close(newsock);
}

int main(void)
{
    int sock;
    fd_set socks;
    fd_set readsocks;
    int maxsock;
    int reuseaddr = 1; /* True */
    struct addrinfo hints, *res;
	int i=0;
    /* Get the address info */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
#if 0
if (getaddrinfo(NULL, PORT, &hints, &res) != 0) {
        perror("getaddrinfo");
        return 1;
    }

    /* Create the socket */
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    /* Enable the socket to reuse the address */
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("setsockopt");
        return 1;
    }

    /* Bind to the address */
    if (bind(sock, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        return 1;
    }

    freeaddrinfo(res);

    /* Listen */
    if (listen(sock, BACKLOG) == -1) {
        perror("listen");
        return 1;
    }
#endif

sock=Open_listenfd(port);

	printf("Listening on port:%d\n",port);
    /* Set up the fd_set */
    FD_ZERO(&socks);
    FD_SET(sock, &socks);
    maxsock = sock;
	
fork_clients(2);	
	int switch1=1;

	/* Main loop */
    while (1) {
        unsigned int s;
        readsocks = socks;
        if (select(maxsock + 1, &readsocks, NULL, NULL, NULL) == -1) {
            perror("select");
            return 1;
        }
		printf("Finished Select, s=%d\n",maxsock);
        for (s = 0; s <= maxsock; s++) {
			printf("Inside the for loop.\n");
            if (FD_ISSET(s, &readsocks)) {
                printf("socket %d was ready\n", s);
                if (s == sock) {
                    /* New connection */
                    int newsock;
                    struct sockaddr_in their_addr;
                    unsigned int size = sizeof(struct sockaddr_in);
                    newsock = accept(sock, (struct sockaddr*)&their_addr, &size);
                   	printf("Accepted a connection.\n");
				   	if (newsock == -1) {
                        perror("accept");
                    }
                    else {
                        printf("Got a connection from %s on port %d\n", inet_ntoa(their_addr.sin_addr), htons(their_addr.sin_port));
			if(fork()==0){
			handle(newsock,&socks);
			}
				                        
			FD_SET(newsock, &socks);
                        if (newsock > maxsock) {
                            maxsock = newsock;
                        }
                    }
                }

            }
        }
//	if(switch1){
//		fork_clients(2);
//		switch1=0;
//	}
    }

    close(sock);

    return 0;
}


