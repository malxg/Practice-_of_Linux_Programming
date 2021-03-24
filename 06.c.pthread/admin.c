//admin
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/select.h>
#include<errno.h>
#include<semaphore.h>
int main()
{
	char w[1024];
	char r[1024];
	int wfd,rfd;
	mkfifo("fifo1",S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	mkfifo("fifo2",S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	wfd=open("fifo1",O_WRONLY);
	rfd=open("fifo2",O_RDONLY);
	while(1)
	{
		memset(w,0,sizeof(w));
		fgets(w,sizeof(w),stdin);
		write(wfd,w,strlen(w));
		read(rfd,r,sizeof(r));
		printf("%s\n",r);

		if(strcmp(w,"exit\n")==0)
		{
			exit(0);
		}
		memset(r,0,sizeof(r));
	}
	unlink("fifio1");
	unlink("fifio2");
	return 0;
}
