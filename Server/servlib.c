#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "serv.h"

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

int set_up(struct sockaddr_in * servAddr){
	int servSock;
	pthread_mutex_init(&mutx, NULL);
	servSock = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(servAddr, 0, sizeof(*servAddr));
	servAddr->sin_family = AF_INET;
	servAddr->sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr->sin_port = htons(SERVPORT);

	if (bind(servSock, (struct sockaddr*)servAddr, sizeof(*servAddr)) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(servSock, 100) == -1) {
		perror("listen");
		exit(1);
	}

	return servSock;
}

void ServerProcessing(int servSock){
	int clntSock;
	struct sockaddr_in clntAddr;
	int clntSz;
	pthread_t t1;

	while(1){
		clntSz = sizeof(clntAddr);
		clntSock = accept(servSock, (struct sockaddr*)&clntAddr, &clntSz);

		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt++] = clntSock;
		pthread_mutex_unlock(&mutx);

		pthread_create(&t1, NULL, handle_clnt, (void*)&clntSock);
		pthread_detach(t1);
	}
	close(servSock);
}

void * handle_clnt(void * clntSock){
	int clnt_socket = *((int*)clntSock);
	int len = 0;
	char msg[BUF_SIZE];

	while((len = read(clnt_socket, msg, sizeof(msg))) !=0)
		send_msg(msg, len);

	pthread_mutex_lock(&mutx);
	for(int i=0; i<clnt_cnt; i++){
		if(clnt_socket == clnt_socks[i]){
			while(i++ < clnt_cnt-1)
				clnt_socks[i] = clnt_socks[i+1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_socket);
	return NULL;
}

void send_msg(char * msg, int len){
	int i;
	pthread_mutex_lock(&mutx);
	for(i=0; i<clnt_cnt; i++)
		write(clnt_socks[i], msg, len);
	pthread_mutex_unlock(&mutx);
}
