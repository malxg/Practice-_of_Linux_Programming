#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>
sem_t mysem,mysem_space;
int buf[10];
int idx;

void *producer(void *arg)
{
	for(int i=0;i<10;i++)
	{
		sem_wait(&mysem_space);
		idx++;
		if(i==9)
		{
			buf[idx]=0xab;	
		}
		else
		{
			buf[idx] = rand();
		}
		sem_post(&mysem);
		printf("product:%d\n",buf[idx]);
		sleep(1);	
	}
	return NULL;
}

void *consumer(void *arg)
{
	while(1)
	{
		sem_wait(&mysem);
		printf("consum:%d\n",buf[idx]);
		if(buf[idx]==0xab)
		{
			break;
		}
		idx--;
		sem_post(&mysem_space);
		sleep(1);
	}	
	return NULL;
}

int main()
{
	pthread_t pid1,pid2;
	sem_init(&mysem,0,0);
	sem_init(&mysem_space,0,10);
	
	pthread_create(&pid1,NULL,producer,NULL);
	pthread_create(&pid2,NULL,consumer,NULL);

	pthread_join(pid1,NULL);
	pthread_join(pid2,NULL);
	
	sem_destroy(&mysem);

	return 0;
}
