#define BUF_SIZE 1024
#define NAME_SIZE 20
#define SERVPORT 9090
#define SERVADDR "14.46.254.107"

void * SendMessage(void * arg);
void * RecvMessage(void * arg);
int set_up(char * name);
void ClientProcessing(int sock);
void TurnOffClientProgram(void);
