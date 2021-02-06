#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>

#include "shm_com.h"

int main()
{






	/*
	 * rand函数产生的随机数是一个固定序列，
	 * 这个序列是由递推公式生成的，也就是说有一个递推函数，R(n+1) = f{R(n)};
	 * srand可以设置R(0)的值，这样就能改变这个序列，使其不固定，这里设置为程序的pid
	 * */
	srand((unsigned int)getpid());


	/*
	 * key:1234 用来标记这块内存，类似文件的路径或者文件描述符，
	 * 只不过这里是自己设定的一个数
	 * size 以字节为单位的内存大小，这里等于struct shared_use_st 的大小
	 * shmflg：权限标志,这里0666 | IPC_CREAT == 01666（八进制）
	 * IPC_CREAT = 01000 = 0b 001 000 000
	 *
	 * 返回共享内存标识符 shmid，非负整数，如果失败则等于-1
	 * 	这个函数已经创建了一块共享内存，但是其地址是隐藏的
	 * */
	int shmid;
	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);

	if (shmid == -1) // 判断是否出错
	{
		fprintf(stderr, "shmget() failed\n");
		exit(EXIT_FAILURE);
	}

	/*
	 * 通过shmat函数定位shmid对应的共享内存的地址
	 * shmid为shmget得到的共享内存的标识符
	 * 第二个参数，shmaddr：通常是空指针，目前不深究
	 * 第三个参数
	 * 常规用法后两个参数设为零，暂不追究
	 * */
	void* shared_memory = NULL; // 先初始化为零
	shared_memory = shmat(shmid, (void*)0, 0);
	if (shared_memory == (void*)(-1))
	{
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}


	/*
	 * 将结构体指针指向这块内存
	 * */
	struct shared_use_st* shared_stuff = (struct shared_use_st*)shared_memory;


	shared_stuff->written_by_you = 0;


	int running = 1;
	while (running)
	{
		if (shared_stuff->written_by_you)
		{
			puts("You wrote: ");
			puts(shared_stuff->some_text);
			shared_stuff->written_by_you = 0;
			if (strncmp(shared_stuff->some_text, "end", 3) == 0)
			{
				running = 0;
			}
		}
	}


	/*
	 * 将内存地址和该共享内存空间分离，
	 * 并没有改变指针的值，而是指针不再指向共享内存，
	 * 如果这时操作指针指向的内存则会触发段错误（核心已转储）
	 * */
	if (shmdt(shared_memory) == -1)
	{
		exit(EXIT_FAILURE);
	}


	/*
	 * 共享内存控制函数，需要传入共享内存的id和指令，
	 * 例如IPC_RMID：删除共享内存，这个指令buf参数不起作用，不需要
	 * */
	if (shmctl(shmid, IPC_RMID, NULL) == -1)
	{
		exit(EXIT_FAILURE);
	}

	return 0;
}
