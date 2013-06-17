/* File: client.c */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "head.h"

int main(int argc, char **argv)
{
	int sock_fd;
	struct sockaddr_in client_addr;
	int client_addr_len;
	char recv_buff[INET_ADDRSTRLEN];
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0)
	{
		prerr("socket");
		exit(1);
	}
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	client_addr.sin_port = htons(8001);
	client_addr_len = sizeof(client_addr);

	int rtval;
	rtval = connect(sock_fd, (struct sockaddr *)&client_addr, client_addr_len);
	if(rtval == -1)
	{
		prerr("connect");
		exit(1);
	}
	int rsnum;

	char *send_buf = "Client to Server string!\n";
	rsnum = send(sock_fd, (void *)send_buf, strlen(send_buf), 0);
	if (rsnum < 0)
	{
		prerr("send");
		exit(1);
	}
	printf("sent line:%s", send_buf);

	rsnum  = recv(sock_fd, (void *)recv_buff, INET_ADDRSTRLEN, 0);
	if (rsnum < 0)
	{
		prerr("recv");
		exit(1);
	}

	printf("readline:%s\t", recv_buff);
	
	printf("client exit.\n");

	close(sock_fd);
	return 0;
}
