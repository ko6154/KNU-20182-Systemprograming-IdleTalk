#include <netinet/in.h>

#define BUF_SIZE 1024
#define MAX_CLNT 256
#define SERVPORT 9090

void * handle_clnt(void * arg);
void send_msg(char * msg, int len);
int set_up(struct sockaddr_in * servAddr);
void ServerProcessing(int servSock);
