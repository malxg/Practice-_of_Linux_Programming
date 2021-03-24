#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>
#include <unistd.h>
/*
#include <sys/msg.h>

//创建或打开消息队列：成功返回队列ID，失败返回-1
int msgget(key_t key, int flag);

//添加消息：成功返回0，失败返回-1
int msgsnd(int msqid, const void *ptr, size_t size, int flag);

//读取消息：成功返回消息数据的长度，失败返回-1
int msgrcv(int msqid, void *ptr, size_t size, long type,int flag);

//控制消息队列：成功返回0，失败返回-1
int msgctl(int msqid, int cmd, struct msqid_ds *buf);
*/

#define IPC_KEY 0x12345678 
#define TYPE_S 1
#define TYPE_C 2

typedef struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[1024];    /* message data */
}msgbuf;

struct msg
{
	struct msg *next;
	int num;
};

struct msg *head;
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *p)
{
	struct msg *mp;
	int i;
	for(i=0;i<20;++i)
	{
		mp = malloc(sizeof(struct msg));
		mp->num = rand() % 1000+1;
		printf("p is %d\n",mp->num);
		pthread_mutex_lock(&lock);
		mp->next=head;
		head=mp;
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&has_product);
		sleep(rand()%5);
	}
}

void *consumer(void *p)
{
	struct msg *mp;
	int i;
	for(i=0;i<20;++i)
	{
		pthread_mutex_lock(&lock);
		while(head==NULL)
		pthread_cond_wait(&has_product,&lock);
		mp=head;
		head=mp->next;
		pthread_mutex_unlock(&lock);
		printf("C is %d\n",mp->num);
		free(mp);
		sleep(rand()%5);
	}
}

int main()
{
    int msgid=-1,flag=1;//key_t ftok(const char *pathname, int proj_id);
    struct msgbuf buf;
    pthread_t pt,ct;
    srand(time(NULL));
    
    msgid=msgget(IPC_KEY,IPC_CREAT | 0664);//创建一个消息队列int msgget ( key_t key, int msgflg ) 
    if(msgid<0){			    //是否创建失败
        perror("msgget error");
        return -1;
    }
    
    while(1)				   //读取消息
    {
	msgbuf buf;		           //定义一个buf结构体

	msgrcv(msgid,&buf,1024,TYPE_C,0); //读取消息长度为1024 int msgrcv ( int msqid, struct msgbuf *msgp, int msgsz, long
	
	printf("The message you receved(client say):%s\n",buf.mtext);
	
	if(0 == strcmp(buf.mtext,"create_producer"))
	{
		char user[1024];
		pthread_create(&pt,NULL,producer,NULL);
		pthread_join(pt,NULL);
		
		sprintf(user,"%d",(unsigned int)pthread_self());
		
		memset(&buf,0x00,sizeof(struct msgbuf));//发送新建 生产者 线程号数据
		buf.mtype=TYPE_S;
		strcat(buf.mtext,user);
		msgsnd(msgid,&buf,1024,0);
		
		break;
	}
	    
	else if(0 == strcmp(buf.mtext,"create_consumer"))
	{
		char user[1024];
		pthread_create(&ct,NULL,consumer,NULL);
		pthread_join(ct,NULL);	
		
		sprintf(user,"%d",(unsigned int)pthread_self());
		
		memset(&buf,0x00,sizeof(struct msgbuf));//发送新建 消费者 线程号数据
		buf.mtype=TYPE_S;
		strcat(buf.mtext,user);
		msgsnd(msgid,&buf,1024,0);
		
		break;
	}
	    
	else if(0 == strcmp(buf.mtext,"delete"))
	{
		char t[1024];
		strcpy(t,buf.mtext+6);
	    	int p=atoi(t); //returns 线程号
		int k=pthread_key_delete(p);	    //删除线程号
		if(k==0)
		{
			memset(&buf,0x00,sizeof(struct msgbuf));//发送数据
			buf.mtype=TYPE_S;
			strcpy(buf.mtext,"OK");
			msgsnd(msgid,&buf,1024,0);	
		}
		else 
		{
			memset(&buf,0x00,sizeof(struct msgbuf));//发送数据
			buf.mtype=TYPE_S;
			strcpy(buf.mtext,"Failed");
			msgsnd(msgid,&buf,1024,0);
		}
		
		break;
	}
	    
	else if(0 == strcmp(buf.mtext,"list prodicer"))
	{
		printf("主动断开,通信结束!\n");
		break;
	}
	    
	else if(0 == strcmp(buf.mtext,"list consumer"))
	{
		printf("主动断开,通信结束!\n");
		break;
	}
	    
	else if(0 == strcmp(buf.mtext,"exit"))
	{
		pthread_exit(0);
		break;
	}
	    
	else
	{
		printf("Unknown Command\n");
		break;
	}
	
    }
    msgctl(msgid,IPC_RMID,NULL);		//将消息队列设为空int msgctl ( int msgqid, int cmd, struct msqid_ds *buf )
    return 0;
}
