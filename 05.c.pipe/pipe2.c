#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	pid_t result;
	int r_num;
	int pipe_fd[2];
	char buf_r[100];
	memset(buf_r,0,sizeof(buf_r));
	p=open("/home/linux/桌面/proc/05.c.pipe/pipe2.c",O_WRONLY);
	if(pipe(pipe_fd)<0)
	{
		printf("fail,error");
		return -1;
	}
	result=fork();
	if(result<0)
	{
		perror("children fail,error");
		exit(0);
	}	
	else if(result==0)
	{
		close(pipe_fd[1]);
		if((r_num=read(pipe_fd[0],buf_r,100))>0)
		{
			printf("%d,%s\n",r_num,buf_r);
		}
		close(pipe_fd[0]);
		exit(0);
	}
	else
	{
		close(pipe_fd[0]);
                if(write(pipe_fd[1],"first a string",10)!=-1)
                {
                        printf("first write to pipe of parent!\n");
                }
		
		if(write(pipe_fd[1],"Second, a string",10)!=-1)
                {
                        printf("Second, write to pipe of parent!\n");
                }
                close(pipe_fd[1]);
		waitpid(result,NULL,0);
                exit(0);

	}
	return 0;
}
