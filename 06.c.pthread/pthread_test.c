#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include <unistd.h>
void * my_print(void * arg)
{
	int i;
	int counter=0;
	for(i=0;i<10;i++)
	{
		printf("%c%d ", (char *)arg,counter++);
		fflush(stdout);
		sleep(1);
	}
	return NULL;
}
int main()
{
	pthread_t tid1,tid2;
	pthread_create(&tid1,NULL,my_print,(void *)'#');
	pthread_create(&tid2,NULL,my_print,(void *)'.');
	
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	
	return 0;
}
