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

#define MAX_LENGTH      1024
#define MAX_LINE        511
#define START_STRING    "Start"
#define END_STRING      "End"

void AddClient(int s, struct sockaddr_in *newcliaddr);
void RemoveClient(int s);
void* MainThread(void* args);
void ErrorHandler(char* msg);
int TCPListen(int host, int port, int backlog);
int FindMaxNumber();
