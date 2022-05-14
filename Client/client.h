#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_LINE    1000
#define MAX_LENGTH  20

char* END_STRING = "exit";

int TCPConnect(int af, char* serverip, unsigned short port);
void ErrorHandler(char* msg);
