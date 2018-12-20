#include <stdio.h>
#include <stdlib.h>
#include "clnt.h"

int main(int argc, char * argv[]){
	int sock;
	if(argc != 2){
		fprintf(stderr, "usage : %s Name\n", argv[0]);
		exit(1);
	}

	sock = set_up(argv[1]);
	ClientProcessing(sock);

	TurnOffClientProgram();

	return 0;
}
