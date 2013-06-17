#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int fd;
	int address_len;
	struct sockaddr_in address;
	int listenfd;

	//建立套接口
	fd = socket(AF_INET, SOCK_STREAM, 0);

	//绑定地址和端口
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(8001);
	address_len = sizeof(address);
	bind(fd, (struct sockaddr *)&address, address_len);

	//建立套接口队列
	listenfd = listen(fd, 64);
#if 1
	int optval;

	optval = 1;

	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

#ifdef SO_REUSEPORT//要注意不是每个系统都有宏定义，所以自己先判断一下，如有则用
	optval = 1;

	setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
#endif

#endif

	char client_addr[] = "xxx.xxx.xxx.xxx\0";
	char *p = NULL;
	while(1) {
		struct sockaddr_in client_address;
		int len;
		int client_sockfd;
		char *data = "Server to Client String!\n";
		char data2[100];
		pid_t pid;

		printf("waiting...");
		fflush(stdout);
		//等待
		len = sizeof(client_address);
		client_sockfd = accept(fd, (struct sockaddr *)&client_address, &len);
		if (client_sockfd  < 0)
			printf("server accept filad\n");
		memset(client_addr, 0x00, sizeof(client_addr) / sizeof(*client_addr));
		inet_ntop(AF_INET, &client_address.sin_addr, client_addr, INET_ADDRSTRLEN);
		p = inet_ntoa(client_address.sin_addr);
		printf("client_addr:%s\np:%s\n",client_addr, p);



		//建立子进程
		if( (pid = fork()) == 0){	//child
			close(listenfd);
			//接收数据
			bzero((void *)data2, 100);
			read(client_sockfd, (void *)data2, 100);
			printf("server read line:%s", data2);
			//发送数据
			write(client_sockfd,(void *)p, INET_ADDRSTRLEN);
			printf("server send line:%s", data);

			//关闭联接并且退出
			close(client_sockfd);
			exit(0);

		}
		close(client_sockfd);		//parent
	}
}

