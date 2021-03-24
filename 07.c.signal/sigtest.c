#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h> 

void func_ctrl_z();


int main()
{
	int counter=0;
	pid_t pid;
	(void) signal(SIGTSTP,func_ctrl_z);
	(void) signal(SIGTSTP,SIG_IGN);//ignore SIGTSTP
	(void) signal(SIGTSTP,SIG_DFL);//ignore SIGTSTP default proc


	pid=fork();
	
	if(pid<0)
	{
		printf("fork failed.\n");
		exit(-1);
	}
	
	else if(pid==0)
	{
		while(1)
		{
			counter++;
			printf("running %ds\n",counter);
			sleep(1);
			
		}
		exit(0);
	}

	else
	{
		sleep(10);
		kill(pid,SIGKILL);
	}
	return 0;
}
void func_ctrl_z()
{
	printf("I don't want hangup.\n");
	(void) signal(SIGINT,SIG_DFL);
	return;
}
