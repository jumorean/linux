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
    int running =1 ;
    void * shared_memory = (void *)0;
    struct shared_use_st * shared_stuff;

    char buffer[BUFSIZ];
    int shmid;


	/*
	 * 获取共享内存的id，相同的key指向相同的共享内存
	 * 问题：如果这里共享内存的大小设置和另一个程序的设置不同会有什么后果？
	 * 答案：如果两个程序设置得不一样，且先调用shmget函数的程序设置的内存大小更大，则两者能够相安无事
	 * 如果先调用的程序设置得更小，则后调用的程序会发生段错误，因为已经给key分配了一定大小的内存空间，
	 * 再来分配更大的空间就会访问无效内存，也就是说多出来的内存是无效的
	 * 如果一个程序调用了shmget函数但是退出前没有删除共享内存则，共享内存的大小依然保持，直到某个程序
	 * */
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st)*2, 0666|IPC_CREAT);

    shared_memory = shmat(shmid, (void *)0, 0);

    shared_stuff = shared_memory;


    while (running)
    {
        while(shared_stuff->written_by_you == 1)
        {
            sleep(1);
            printf("1s pass\n");
        }

        printf("enter some text: ");
        fgets(buffer, BUFSIZ, stdin);

        strncpy(shared_stuff->some_text, buffer, TEXT_SZ);

        shared_stuff->written_by_you = 1;

        if(strncmp(buffer, "end", 3) == 0)
        {
            running = 0;
        }
    }


    /*
     * 这里只是将指针和共享内存解除关系了
     * 指针不再指向内存块，删除内存块只能在某一个程序调用，不能删除多次
     * */
    if(shmdt(shared_memory) == -1)
    {
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}



