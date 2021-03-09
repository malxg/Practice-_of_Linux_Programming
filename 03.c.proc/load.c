#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main()
{
	pid_t pid;
	printf("I am load:%d\n",getpid());
	pid = fork();
	if(pid==0)
	{
		execl("/home/lichaofan/桌面/pro/03.c.proc/proc","proc",NULL);  //被proc替换
	}
	else
	{
		printf("I am load,I finished\n");
                wait(NULL);
	}
	return 0;
}
