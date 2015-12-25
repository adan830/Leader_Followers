#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

void *Work(void *arg)
{
	int listenSock = (int)arg;
	int client;
	struct sockaddr_in remote;
	socklen_t len;
	
	client = accept(listenSock,&remote,&len);

	ThreadPool_Add_Work( ThreadPool, Work, listenSock );

	Do_Job(client);
}

int main(int argc, char *argv[])
{
	int listenSock;

	listenSock = GetListenSocket();
	listen(listenSock,10);
	
	Work(listenSock);

	return 0;
}
