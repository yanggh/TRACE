#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include "wrapsock.h"
#include "sock_ntop.h"
#include "Udp_recv.h"
using namespace std;

int flag_1 = 0;
int flag_2 = 0;

const std::string UPDATE{"configUpdate"};
#define  UDP_SERVER_IP  "127.0.0.1"

int Send_msg(int serv_port)
{
    printf("Send_msg\n");
    int sockfd, len = 0;
    struct sockaddr_in addr;
    int  addr_len = sizeof(struct sockaddr_in);
    char buffer[256];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        exit(0);
    }

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(serv_port);
    addr.sin_addr.s_addr = inet_addr(UDP_SERVER_IP);

    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
        len = sprintf(buffer, "%d:%d", flag_1, flag_2);
        sendto(sockfd, buffer, len, 0, (struct sockaddr *)&addr, addr_len);
        flag_1 = 0;
        flag_2 = 0;
        sleep(5);
    }

}

int Recv_msg(int serv_port)
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    const int    on = 1;
    const int    MAXLINE = 4096;

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(serv_port);

    Bind(sockfd, (SA *) &servaddr, sizeof(servaddr));

    char mesg[MAXLINE];
    for ( ; ; ) //循环读取数据
    {
        int n;
        socklen_t len = sizeof (cliaddr);
        n = Recvfrom(sockfd, mesg, MAXLINE, 0, (SA *) &cliaddr, &len);
        if(UPDATE == std::string(mesg, n))
        {
            cout << "UPDATE" << endl;
            Sendto(sockfd, "ok!", strlen("ok!"), 0, (struct sockaddr*)&cliaddr, len);
        }
        else
        {
            system(mesg);
            cout << string(mesg, n) << endl;
            Sendto(sockfd, "ok!", strlen("ok!"), 0, (struct sockaddr*)&cliaddr, len);

        }
    }

    exit(0);
}
