#include<stdio.h>
#include<unistd.h>
int main()
{
	char *args[]={"/usr/bin/vim",NULL};
	printf("系统分配的进程号(PID)是:%d\n",getpid());
	if(execve("/usr/bin/vim",args,NULL)<0)
	{
		perror("用execve创建进程出错");
	}
	return 1;
}
