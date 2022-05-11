#include "server.h"

char* startString = START_STRING;
char* endString = END_STRING;

int usersNumber = 0;
int chattingsNumber = 0;

int usersList[MAX_LENGTH];
char ipList[MAX_LENGTH][20];

// 채팅방의 명령어를 처리하는 메인 스레드
void* MainThread(void* args)
{
    int i;

    printf("명령어 목록");

    while(1)
    {
        char buffer[MAX_LINE + 1];

        fprintf(stderr, "\033[1;32m");
        printf("server>");
        fgets(buffer, MAX_LINE, stdin);

        if(!strcmp(buffer, "\n"))
        {
            continue;
        }
        else if(!strcmp(buffer, "help\n"))
        {
            printf("help, userNumber, chattingNumber, ipList\n");
        }
        else if(!strcmp(buffer, "userNumber"))
        {
            printf("Number Of Users = %d\n", usersNumber);
        }
        else if(!strcmp(buffer, "chattingNumber\n"))
        {
            printf("Number Of Chatting = %d\n", chattingsNumber);
        }
        else
        {
            printf("is Null!\n");
        }
    }
}

void AddClient(int s, struct sockaddr_in *newcliaddr)
{
    char buffer[20];

/*  inet_ntop 
*   Header :    #include <arpa/inet.h>
*   Prototype : const char* inet_ntop(int af, const void* src, char* dst, socklen_t size);
*   Description : IPv4 or IPv6의 주소를 Binary 형태에서 사람이 알아볼 수 있는 문자열로 변환
*                 Network Adress 구조체인 src를 Charcter String으로 변환해주는 함수
*
*   AF_INET     : struct in_addr 구조체를 가리킴
*/
    inet_ntop(AF_INET, &newcliaddr->sin_addr, buffer, sizeof(buffer));
    write(1, "\033[0G", 4);
    fprintf(stderr, "\033[33m"); // 글자색 노란색 변경
    printf("new Client : %s\n", buffer);

}
