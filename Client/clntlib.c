#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <locale.h>
#include <ncursesw/curses.h>
#include "clnt.h"

char name[NAME_SIZE] = "[NONE]";
char msg[BUF_SIZE];
WINDOW * wWindow;
WINDOW * pWindow;

int set_up(char * n){
	int my_sock;
	struct sockaddr_in servAddr;
	
	sprintf(name, "[%s]", n);
	my_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(SERVADDR);
	servAddr.sin_port = htons(SERVPORT);

	if(connect(my_sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1){
		perror("connect");
		exit(1);
	}

	setlocale(LC_CTYPE, "ko_KR.utf-8");

	initscr();
	clear();

	wWindow = newwin(5, COLS-10, LINES-5, 5);
	pWindow = newwin(LINES-12, COLS-5, 3, 3);

	move(LINES-7,0);
	for(int i=0; i<COLS; i++)
		printw("%c", '-');
	move(LINES-8,0);
	printw("%s\n", "quit 또는 exit 입력 시 프로그램 종료");
	scrollok(pWindow, TRUE);
	scrollok(wWindow, TRUE);

	refresh();

	return my_sock;
}

void ClientProcessing(int my_sock){
	pthread_t t_send, t_recv;

	pthread_create(&t_send, NULL, SendMessage, (void*)&my_sock);
	pthread_create(&t_recv, NULL, RecvMessage, (void*)&my_sock);
	pthread_join(t_send, NULL);
	pthread_join(t_recv, NULL);

}

void * SendMessage(void * arg){
	int my_sock = *((int*)arg);
	char message[NAME_SIZE+BUF_SIZE];
	char msg[BUF_SIZE];
	while(1){
		wgetstr(wWindow, msg);
		werase(wWindow);
		wrefresh(wWindow);
		if(strlen(msg)==0)
			continue;
		if(!strcmp(msg,"quit") || !strcmp(msg, "exit")){
			close(my_sock);
			endwin();
			exit(0);
		}
		sprintf(message, "%s %s", name, msg);
		write(my_sock, message, strlen(message));
	}
	return NULL;
}

void * RecvMessage(void * arg){
	int my_sock= *((int*)arg);
	char message[NAME_SIZE + BUF_SIZE];
	int len;
	while(1){
		len = read(my_sock, message, NAME_SIZE+BUF_SIZE-1);
		if(len == -1)
			return (void*)-1;
		message[len] = 0;
		wprintw(pWindow, "%s\n", message);
		wrefresh(pWindow);
	}
	return NULL;
}

void TurnOffClientProgram(void){
	endwin();
	return;
}
