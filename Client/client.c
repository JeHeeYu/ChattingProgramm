#include "client.h"

int main(int argc, char* argv[])
{
    char bufferName[MAX_LENGTH];
    char bufferMessage[MAX_LINE];

    char bufferAll[MAX_LINE + MAX_LENGTH];

    int maxSocket;
    int s;
    int nameLength;

    fd_set readFd;

    time_t m_timer;
    struct tm m_tm;

    if(argc != 4)
    {
        printf("사용법 : %s server_ip port name \n", argv[0]);
        exit(0);
    }

    s = TCPConnect(AF_INET, argv[1], atoi(argv[2]));
    if(s == -1)
    {
        ErrorHandler("TCP Connect is Fail");
    }

    puts("서버 접속");

    maxSocket = s + 1;
    FD_ZERO(&readFd);

    while(1)
    {
        FD_SET(0, &readFd);
        FD_SET(s, &readFd);

        if(select(maxSocket, &readFd, NULL, NULL, NULL) < 0)
        {
            ErrorHandler("Select is Fail");
        }

        if(FD_ISSET(s, &readFd))
        {
            int nbyte;
            if((nbyte == recv(s, bufferMessage, MAX_LINE, 0)) > 0)
            {
                bufferMessage[nbyte] = 0;
                write(1, "\033[0G", 4);
                printf("%s", bufferMessage);
                fprintf(stderr, "\033[1;32m");
                fprintf(stderr, "%s>", argv[3]);

            }
        }

        if(FD_ISSET(0, &readFd))
        {
            if(fgets(bufferMessage, MAX_LINE, stdin))
            {
                fprintf(stderr, "\033[1;33m");
                fprintf(stderr, "\033[1A");

                m_timer = time(NULL);
                m_tm = *localtime(&m_timer);

                sprintf(bufferAll, "[%02d:%02d:%02d]%s>%s", m_tm.tm_hour, m_tm.tm_min, m_tm.tm_sec, argv[3], bufferMessage);

                if(send(s, bufferAll, strlen(bufferAll), 0) < 0)
                {
                    puts("Error : Write error on Socket");
                }

                if(strstr(bufferMessage, END_STRING) != NULL)
                {
                    puts("Bye");
                    close(s);
                    exit(0);
                }
            }
        }
    }
}

int TCPConnect(int af, char* serverip, unsigned short port)
{
    struct sockaddr_in addr;

    int s;

    if((s = socket(af, SOCK_STREAM, 0)) < 0)
    {
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = af;
    inet_pton(AF_INET, serverip, &addr.sin_addr);
    addr.sin_port = htons(port);

    if(connect(s, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        return -1;
    }

    return s;
}

void ErrorHandler(char* msg)
{
    perror(msg);
    exit(1);
}
