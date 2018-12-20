all : server client
server : server.c servlib.c serv.h
	cc -o server server.c serv.h servlib.c -lpthread

client : client.c clntlib.c clnt.h
	cc -o client client.c clnt.h clntlib.c -lpthread -lncursesw

form : formWin.c
	gcc -o form formWin.c -lcurses -lform
