#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <uv.h>

int main(int argc, char ** argv)
{

	if(argc != 2)
	{
		fprintf(stderr, "输入要发送的字符");
		exit(EXIT_FAILURE);
	}

	/*
	 * 创建套接字，指定属性没有绑定路径或者IP端口
	 * */
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		fprintf(stderr, "123");
		exit(EXIT_FAILURE);
	}



	/*
	 * 设置自己（客户端）的地址
	 * */
	struct sockaddr_in my_addr;
	my_addr.sin_port = htons(8889);
	my_addr.sin_family = AF_INET;
	in_addr_t ip;
	inet_pton(AF_INET, "127.0.0.1", &ip);
	my_addr.sin_addr.s_addr = ip;
	bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr));


	/*
	 * 设置服务器的地址
	 * */
	struct sockaddr_in server_addr;
	server_addr.sin_port = htons(8888);
	server_addr.sin_family = AF_INET;
	in_addr_t server_ip;
	inet_pton(AF_INET, "127.0.0.1", &server_ip);
	server_addr.sin_addr.s_addr = server_ip;



	// 将连接到服务器描述符为sockfd的套接字连接到地址为server_address的服务器套接字
	if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		// 如果服务器没启动，或者地址错误，则找不到。
		fprintf(stderr, "连接失败，退出\n");
		exit(EXIT_FAILURE);
	}

	// 往套接字写数据，写一个字符，与读写文件一致
	char recv_buffer[100];
	memset(recv_buffer, '\0', 100);

	//
	printf("--------------start-------------\n");
	write(sockfd, argv[1], strlen(argv[1]));
	// lseek(sockfd, )

	// 读一个字符
	read(sockfd, recv_buffer, 1024);
	printf("ch = %s\n", recv_buffer);

	// 关闭资源
	close(sockfd);
	printf("--------------end-------------\n");
	return 0;
}