#include "server.h"

struct tm m_tm;

char* startString = START_STRING;
char* endString = END_STRING;

int usersNumber = 0;
int chattingsNumber = 0;

int usersList[MAX_LENGTH];
char ipList[MAX_LENGTH][20];

int socketListen;
int maxSocket;

time_t m_timer;

int main(int argc, char *argv[])
{
    struct sockaddr_in clientaddr;

    char buffer[MAX_LINE + 1];

    int socketListen, socketAccept, addrlength;

    int i, j, nbyte;
    
    fd_set readFd;
    pthread_t mainThread;

    if(argc != 2)
    {
        printf("사용법 : %s port\n", argv[0]);
        exit(0);
    }

    // TCP 함수 대기
    socketListen = TCPListen(INADDR_ANY, atoi(argv[1]), 5);

    // 메인 스레드 생성
    pthread_create(&mainThread, NULL, MainThread, (void *)NULL);

    while(1)
    {
        FD_ZERO(&readFd);
        FD_SET(socketListen, &readFd);

        for(i = 0; i < usersNumber; i++)
        {
            FD_SET(usersList[i], &readFd);
        }
    }

    if(select(maxSocket, &readFd, NULL, NULL, NULL) < 0)
    {
        ErrorHandler("Select is Fail!");
    }

    if(FD_ISSET(socketListen, &readFd))
    {
        socketAccept = accept(socketListen, (struct sockaddr*)&clientaddr, &addrlength);

        if(socketAccept == -1)
        {
            ErrorHandler("Accept is Fail");
        }

        AddClient(socketAccept, &clientaddr);
        send(socketAccept, START_STRING, strlen(START_STRING), 0);

        m_timer = time(NULL);
        m_tm = *localtime(&m_timer);

        write(1, "\033[0G", 4);

        printf("[%02d:%02d:%02d]", m_tm.tm_hour, m_tm.tm_min, m_tm.tm_sec);
        fprintf(stderr, "\033[33m");
        printf("사용자 1명 추가, 현재 참가자 수 : %d\n", usersNumber);
        fprintf(stderr, "\033[32m");
        fprintf(stderr, "server>");
    }

    for(i = 0; i < usersNumber; i++)
    {
        if(FD_ISSET(usersList[i], &readFd))
        {
            chattingsNumber++;
            nbyte = recv(usersList[i], buffer, MAX_LINE, 0);
            if(nbyte <= 0)
            {
                RemoveClient(i);
                continue;
            }

            buffer[nbyte] = 0;

            if(strstr(buffer, END_STRING) != NULL)
            {
                RemoveClient(i);
                continue;
            }

            for(j = 0; j < usersNumber; j++)
            {
                send(usersList[i], buffer, nbyte, 0);
            }

            printf("\033[0G");		
            fprintf(stderr, "\033[97m");
            printf("%s", buffer);		
            fprintf(stderr, "\033[32m");
            fprintf(stderr, "server>");
            
        }
    }

    return 0;
}

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
*   returrn :  ㄹ
*   Description : IPv4 or IPv6의 주소를 Binary 형태에서 사람이 알아볼 수 있는 문자열로 변환
*                 Network Adress 구조체인 src를 Charcter String으로 변환해주는 함수
*
*   AF_INET     : struct in_addr 구조체를 가리킴
*/    
    int sd = socket(AF_INET, SOCK_STREAM, 0);

    if(sd == 1)
    {
        perror("Socket Fail!");
        exit(1);
    }


/*  void memset(void* ptr, int value, size_t num);
*   Header :    #include <string.h>
*   parameter : ptr : 메모리의 크기를 변경할 포인터
*               value : 메모리를 초기화할 값
*               size : 초기화 후 크기 반환 값
*   returrn :  성공 시 ptr을 반환, 실패 시 NULL 반환
*   Description : 메모리의 값을 원하는 크기만큼 특정 값으로 세팅할 수 있는 함수
*/  
    memset(&socketaddr, 0, sizeof(socketaddr));
    socketaddr.sin_family = AF_INET;
    socketaddr.sin_addr.s_addr = htonl(host);
    socketaddr.sin_port = htons(port);
    
    
/*  int bind(int sockfd, struct sockaddr* myaddr, socklen_t addrlen);
*   Header :    #include <sys/types.h>, <sys/socket.h>
*   parameter : sockfd : 소켓 디스크립터
*               struct sockaddr* addr : 주소 정보
*               socklen_t addrlen : 구조체의 크기
*   returrn :  성공 시 0, 실패 시 1 반환
*   Description : 소켓에 IP 주소와 포트번호 지정 함수
*/
    if(bind(sd, (struct sockaddr *)&socketaddr, sizeof(socketaddr)) < 0)
    {
        perror("Bind Fail!");
    }

/*  int listen(int sock, int backlog);
*   Header :    #include <sys/types.h>, <sys/socket.h>
*   parameter : sock : 소켓 식별자 또는 소켓 디스크립터
*               backlog : 연결 요청을 대기시킬 공간 설정(백로그 큐)
*   returrn :  성공 시 0, 실패 시 1 반환
*   Description : 소켓 연결 요청을 대기하는 함수
*/
    listen(sd, backlog);

    return sd;
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

void ErrorHandler(char* msg)
{
    perror(msg);
    exit(1);
}
