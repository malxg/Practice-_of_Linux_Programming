//procon
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
sem_t mysem;
struct msg
{
	struct msg *next;
	int num;
};
struct msg *head;
pthread_t tid_p[1024],tid_c[1024];
int P=0,C=0;
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *p)
{
	tid_p[P]=pthread_self();
	struct msg *mp;
	int i;
	for(i=0;i<1;++i)
	{
		mp = malloc(sizeof(struct msg));
		mp->num = rand() % 1000+1;
		printf("producer is %d,%lu\n",mp->num,tid_p[P]);
		pthread_mutex_lock(&lock);
		mp->next=head;
		head=mp;
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&has_product);
		sleep(1);
	}
	P++;
}

void *consumer(void *p)
{
	tid_c[C]=pthread_self(); //获取自己线程
	struct msg *mp;
	int i;
	for(i=0;i<1;++i)
	{
		pthread_mutex_lock(&lock);
		while(head==NULL)
		pthread_cond_wait(&has_product,&lock);
		mp=head;
		head=mp->next;
		pthread_mutex_unlock(&lock);
		printf("Consumer is %d,%lu\n",mp->num,tid_c[C]);
		free(mp);
		sleep(1);
	}
	C++;
}

int main()
{
	int i,rfd,wfd,len=0,fd_in,fd1,fd2;
	char str[1024];
	pthread_t tid1,tid2;
	sem_init(&mysem,0,0);
	mkfifo("fifo1",S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	mkfifo("fifo2",S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	rfd=open("fifo1",O_RDONLY);
	wfd=open("fifo2",O_WRONLY);
	if(rfd<0||wfd<0)return 0;

	fflush(stdout);
	while(1)
	{
		
		memset(str,0,sizeof(str));
		if(read(rfd,str,sizeof(str)))
		{
			if(strcmp(str,"create producer\n")==0)
			{
				pthread_t tid1;
				pthread_create(&tid1,NULL,producer,NULL);
				char wstr1[1024]={0};
				sprintf(wstr1,"%lu",tid_p[P]);
				write(wfd,wstr1,strlen(wstr1));
				pthread_join(tid1,NULL);
			}
			else if(strcmp(str,"create consumer\n")==0)
			{
				pthread_t tid2;
				pthread_create(&tid2,NULL,consumer,NULL);
				char wstr2[1024]={0};
				sprintf(wstr2,"%lu",tid_c[C]);
				write(wfd,wstr2,strlen(wstr2));
				pthread_join(tid2,NULL);	
			}
			else if(strstr(str,"delete")!=NULL)
			{
				char tid[1024];
				int k;
				strcpy(tid,str+6);
				pthread_t tid3;
				tid3 = atoi(tid);
				k=pthread_key_delete(tid3);
				char wstr3[1024]="OK";
				char wstr4[1024]="failed";
				if(k==0)
				{
					write(wfd,wstr3,strlen(wstr3));
				}
				else
				{
					write(wfd,wstr4,strlen(wstr4));
				}	
			}
			else if(strcmp(str,"list producer\n")==0)
			{
				char wstr5[1024]={0};
				for(int i=0;i<P;i++)
				{
						memset(wstr5,0,sizeof(wstr5));
						sprintf(wstr5,"%lu",tid_p[i]);//lu unsigned int
						strcat(wstr5,"  ");
						write(wfd,wstr5,strlen(wstr5));
				}
			}
			else if(strcmp(str,"list consumer\n")==0)
			{
				char wstr6[1024]={0};
				for(int j=0;j<C;j++)
				{
						memset(wstr6,0,sizeof(wstr6));
						sprintf(wstr6,"%lu",tid_c[j]);//lu unsigned int
						strcat(wstr6,"  ");
						write(wfd,wstr6,strlen(wstr6));
						
				}
			}
			else if(strcmp(str,"exit\n")==0)
			{
				exit(0);
			}
			else 
			{
				char wstr7[1024]="Unknown Command";
				write(wfd,wstr7,strlen(wstr7));
				memset(wstr7,0,sizeof(wstr7));
			}
		}
	}
	close(rfd);
	return 0;
}
