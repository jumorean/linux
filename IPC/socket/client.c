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

int main(int argc, char ** argv)
{

	if(argc != 2)
	{
		fprintf(stderr, "输入要发送的字符");
		exit(EXIT_FAILURE);
	}

	// if(strlen(argv[1]) != 1)
	// {
	//     fprintf(stderr, "只需要一个字符");
	//     exit(EXIT_FAILURE);
	// }

	/*
	 * 创建套接字，指定属性没有绑定路径或者IP端口
	 * */
	int sockfd;
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		fprintf(stderr, "123");
		exit(EXIT_FAILURE);
	}

	/*
	 * 设置服务器套接字的地
	 * 注意：这里的地址如果是相对路径。不加绝对路径则和执行程序时的pwd值一致。
	 * 例如在文件夹/home/mi/CLionProjects/untitled/cmake-build-debug目录下执行./socket/client命令
	 * 则"socket/server_socket" 这个地址指的是
	 * /home/mi/CLionProjects/untitled/cmake-build-debug/socket/server_socket的套接字，如果
	 * 程序运行后会在路径下生成一个名为server_socket的文件
	 * 这里如果设置成绝对路径则不用考虑相对路径和程序执行时的运行路径问题。
	 * */
	struct sockaddr_un server_address;
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "/home/mi/CLionProjects/untitled/cmake-build-debug/socket/server_socket");



#define SPEC_ADDR

#ifdef SPEC_ADDR
	/*
	* 设置自身的套接字地址
	* 如果不设置地址，则不会在当前目录下生成套接字文件，
	* 由于没有指定目录更不谈在指定目录下生成套接字文件。
	* */
	struct sockaddr_un client_address;
	client_address.sun_family = AF_UNIX;
	strcpy(client_address.sun_path, "client_socket");

	// 将指定的套接字求连接指定地址的套接字, 也可以不指定本地socket的地址，相当于匿名。
	bind(sockfd, (struct sockaddr *)&client_address, sizeof(client_address));
#endif


	// 将连接到服务器描述符为sockfd的套接字连接到地址为server_address的服务器套接字
	if(connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
		// 如果服务器没启动，或者地址错误，则找不到。
		fprintf(stderr, "连接失败，退出\n");
		exit(EXIT_FAILURE);
	}

	// 往套接字写数据，写一个字符，与读写文件一致
	char send_buffer[100] = "abcd";
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