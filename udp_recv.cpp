#include <iostream>
#include <string>
#include <stdlib.h>
#include "wrapsock.h"
#include "sock_ntop.h"

using namespace std;

const std::string UPDATE = "configUpdate";

int main(int argc, char **argv)
{
	int	sockfd;
	struct sockaddr_in servaddr, cliaddr;
	const int    on = 1;
	const int    MAXLINE = 4096;

	if (argc != 2)
		err_quit("usage: udpcli <port>");

	short serv_port = atoi(argv[1]);

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
	}

	exit(0);
}
