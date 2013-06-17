/* File: server.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "head.h"

#define MAX_LISTEN	10

int main(int argc, char **argv)
{
	int sock_fd;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0)
	{
		prerr("socket");
		exit(1);
	}

	struct sockaddr_in sock_addr;
	int sock_addr_len;
	sock_addr.sin_family	= AF_INET;
	sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	sock_addr.sin_port	= htons(8001);
	sock_addr_len = sizeof(sock_addr);
	int ret;
	ret = bind(sock_fd, (struct sockaddr *)&sock_addr, sock_addr_len);
	if (ret < 0)
	{
		prerr("bind");
		exit(1);
	}

	int listen_fd;
	listen_fd = listen(sock_fd, MAX_LISTEN);
	if (listen_fd < 0)
	{
		prerr("listen");
		exit(1);
	}
#if 1
	int optval;
	optval = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
#ifdef SO_REUSEPORT//要注意不是每个系统都有宏定义，所以自己先判断一下，如有则用
	optval = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
#endif
#endif

	while(1) {
		printf("waiting...");
		fflush(stdout);

		struct sockaddr_in client_address;
		int len;
		int client_fd;
		len = sizeof(client_address);
		client_fd = accept(sock_fd, (struct sockaddr *)&client_address, (socklen_t*)&len);
		if (client_fd  < 0)
		{
			prerr("accept");
			exit(1);
		}
		char *p;
#if 1
		p = inet_ntoa(client_address.sin_addr);
#else
		char client_addr[INET_ADDRSTRLEN];
		p = client_addr;
		memset(client_addr, 0x00, sizeof(client_addr) / sizeof(*client_addr));
		inet_ntop(AF_INET, &client_address.sin_addr, client_addr, INET_ADDRSTRLEN);
#endif

		int rsnum;
		char recv_buf[INET_ADDRSTRLEN];
		rsnum = recv(client_fd, recv_buf, INET_ADDRSTRLEN, 0);
		if (rsnum < 0)
		{
			prerr("recv");
			exit(1);
		}
		printf("recv_buf:%s\n", recv_buf);
		printf("p:%s\n", p);

		rsnum = send(client_fd, p, INET_ADDRSTRLEN, 0);
		if (rsnum < 0)
		{
			prerr("send");
			exit(1);
		}
		close(client_fd);
	}
	return 0;
}
