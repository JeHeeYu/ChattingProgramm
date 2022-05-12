#include "server.h"

struct tm m_tm;

char* startString = START_STRING;
char* endString = END_STRING;

int usersNumber = 0;
int chattingsNumber = 0;
int socketListen;

int usersList[MAX_LENGTH];
char ipList[MAX_LENGTH][20];

time_t m_timer;

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

    usersList[usersNumber] = s;
    strcpy(ipList[usersNumber], buffer);

    usersNumber++;
}

void RemoveClient(int s)
{
    close(usersList[s]);

    if(s != usersNumber - 1)
    {
        usersList[s] = usersList[usersNumber - 1];
        strcpy(ipList[s], ipList[usersNumber - 1]);
    }   

    usersNumber--;

    // 현재 시간 정보
    m_timer = time(NULL);
    m_tm = *localtime(&m_timer);
    write(1, "\033[0G", 4);
    fprintf(stderr, "\033[33m");

    printf("[%02d:%02d:%02d]", m_tm.tm_hour, m_tm.tm_min, m_tm.tm_sec);
    printf("채팅 참가자 1명 탈퇴, 현재 참가자 수 : %d\n", usersNumber);
    
    fprintf(stderr, "\033[32m");
    fprintf(stderr, "server>");
}

int TCPListen(int host, int port, int backlog)
{
    
    struct sockaddr_in socketaddr;

    /*  int socket(int domain, int type, int protocol);
*   Header :    #include <arpa/inet.h>
*   parameter : domain : 프로토콜의 정보
*               type : 소켓의 형태를 지정 (SOCK_STREAM(TCP), SOCK_DGRAM(UDP), SOCK_RAW)
*               protocol : 실제로 사용할 매개변수 지정 (IPPROTO_TCP, IPPROTO_UDP, IPPROTO_HOPOPTS(Auto))
*   returrn :  
*   Description : IPv4 or IPv6의 주소를 Binary 형태에서 사람이 알아볼 수 있는 문자열로 변환
*                 Network Adress 구조체인 src를 Charcter String으로 변환해주는 함수
*
*   AF_INET     : struct in_addr 구조체를 가리킴
*/    
    int sd = socket(AF_INET, SOCK_STREAM, 0);
}

int FindMaxNumber()
{
    int max = socketListen;
    int i;

    for(i = 0; i < usersNumber; i++)
    {
        if(usersList[i] > max)
        {
            max = usersList[i];
        }
    }

    return max;
}
