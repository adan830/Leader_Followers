#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#include "threadpool.h"

struct threadpool *gThreadPool;

int get_listen_socket()
{
	int sock;
	struct sockaddr_in addr;
	sock = socket(AF_INET,SOCK_STREAM,0);

	if(0 >= sock)
	{
		perror("socket error!");
		exit(0);
	}

	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(51005);
	addr.sin_family = AF_INET;

	bind(sock,(struct sockaddr *)&addr,sizeof(struct sockaddr));
	listen(sock,10);

	return sock;
}

void do_job(client)
{
	char buf[20] = "hello!";
	send(client,buf,strlen(buf),0);
	close(client);
}

void *work(void *arg)
{
	int *listensock = (int *)arg;
	int client;
	struct sockaddr_in remote;
	socklen_t len;
	
	client = accept(*listensock,(struct sockaddr *)&remote,&len);

	threadpool_add_job( gThreadPool, work, (void *)listensock );

	do_job(client);
}

int main(int argc, char *argv[])
{
	int *listensock;

	listensock = (int *)malloc(sizeof(int));
	*listensock = get_listen_socket();
	gThreadPool = threadpool_init(6,20);
	
	//work(listensock);
	threadpool_add_job( gThreadPool, work, (void *)listensock );

	while(1)
	{
		sleep(10);
	}

	return 0;
}
