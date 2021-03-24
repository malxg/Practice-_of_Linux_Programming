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
	int i=0;
	sigset_t set,pendset;
	struct sigaction action;
	if(sigemptyset(&set)<0)
	{	
		perror("init signal error");
	}
	
	if(sigaddset(&set,SIGINT)<0)
	{	
		perror("add signal error");
	}
	
	if(sigprocmask(SIG_BLOCK,&set,NULL)<0)
	{	
		perror("Block add  mask signal error");
	}
	else
	{
		for(i=0;i<5;i++)
	{
			printf("according to the words,express signal block of process!\n");
			sleep(2);
		}
	}	
	if(sigprocmask(SIG_UNBLOCK,&set,NULL)<0)
	{	
		perror("delete signal error in signal block set");
	}

	return 0;
}
void func_ctrl_z()
{
	printf("I don't want hangup.\n");
	
	(void) signal(SIGINT,SIG_DFL);
	return;
}
