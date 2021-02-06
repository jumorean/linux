
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>


int main(int argc, char ** argv)
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;

	printf("%d\n", unlink("server_socket"));


	/*
	 * 创建socket，AF_UNIX为套接字的域或者叫地址族（Address Family）
	 * SOCK_STREAM为套接字的类型（流型），0表示默认传输协议
	 * 定义了这个套接字的一些属性，但是没有指定地址，比如文件名或者IP和端口
	 * */
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

	// 设置套接字的地址，地址类型由套接字的域决定，
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "server_socket");
	server_len = sizeof(server_address);


	// 将套接字绑定到上面指定的地址里面，方便客户端连接
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);


	// 创建套接字队列
	listen(server_sockfd, 5);

	while(1)
	{
		printf("--------start-------\n");
		char ch;


		client_len = sizeof(client_address);

		/*
		 * 等待某个套接字连接（connect()函数），如果没有则阻塞
		 * 这里的addr参数并不是指定对方的地址，
		 * 而是预留的地址结构体用来存储前来连接的套接字的地址
		 * 函数调用完后可以通过addr指针查看连接的套接字的地址
		 * 如果为空则说明不关心对方的地址
		 * */
		client_sockfd = accept(server_sockfd, NULL, 0);

		// printf("domain = %d\n", client_address.sun_family);
		// printf("client_address = %s\n", client_address.sun_path);

		/*
		 * 读取套接字的内容，这里设置的nbytes为最大数量
		 * */
		read(client_sockfd, &ch, 1);
		printf("server ch = %c\n", ch);

		// ch = 'a';

		write(client_sockfd, &ch, 1);

		close(client_sockfd);

		printf("--------end-------\n");
	}

}



