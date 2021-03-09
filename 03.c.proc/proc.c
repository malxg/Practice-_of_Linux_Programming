#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
	pid_t i,j;
	printf("I am parent:%d\n",getpid());
	i=fork();
	if(i != 0)
	{
		printf("I am parent:%d my parent:%d\n",getpid(),getppid());
		/*for (j=0;j<100;j++)
		{
			printf("*");
			fflush(stdout);
			sleep(1);
		}*/
		wait(NULL);
	}
	else
	{
		printf("I am child:%d my parent:%d\n",getpid(),getppid());
		for (j=0;j<100;j++)
                {
                        printf(".");
			fflush(stdout);
                        sleep(1);
                }

	}
	//printf("my PID:%d i:%d\n",getpid(),i);
	return 0;
}
