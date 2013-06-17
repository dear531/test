/* File: client.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int main(int argc, char **argv)
{
	int fd;
	struct sockaddr_in address;
	int address_len;
	int rtval;
	char *data = "Client to Server string!\n";
	char data2[100];
	int len;
	//建立套接口
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		printf("client socket failed\n");
	//联接
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("192.168.1.110");
	address.sin_port = htons(8001);
	address_len = sizeof(address);
	rtval = connect(fd, (struct sockaddr *)&address, address_len);
	if(rtval == -1)
	{
		printf("client connect failed\n");
		exit(1);
	}
	//发送数据
	write(fd, (void *)data, strlen(data));
	printf("sent line:%s", data);

	//接收数据
	len = read(fd, (void *)data2, 100);
	printf("readline:%s\t", data2);
	printf("client exit.\n");
	//关闭
	close(fd);
}
