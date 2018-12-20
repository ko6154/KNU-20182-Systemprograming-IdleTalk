#include <stdio.h>
#include "serv.h"

int main(int argc, char * argv[]){
	struct sockaddr_in servAddr;
	int server_socket;

	server_socket = set_up(&servAddr);

	ServerProcessing(server_socket);

	return 0;
}
