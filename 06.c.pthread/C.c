#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define IPC_KEY 0x12345678 
#define TYPE_S 1
#define TYPE_C 2

struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[1024];    /* message data */
};

int main()
{
    int msgid=-1,flag=1;//1.创建消息队列   
    pid_t pid=getpid();
    char user[1024];
    
    msgid=msgget(IPC_KEY,IPC_CREAT | 0664);//msgget()函数被用来创建新的消息队列或获取已有的消息队列
    
    if(msgid<0)
    {
	    perror("msgget error");
	    return -1;
    }
    
    while(1)
    {
	    struct msgbuf buf;
	    
	    memset(&buf,0x00,sizeof(struct msgbuf));
	    buf.mtype=TYPE_C;

	    scanf("%s",buf.mtext);
	    msgsnd(msgid,&buf,1024,0);//msgsnd()函数是用来向消息队列发送消息的
	     
    
	    msgrcv(msgid,&buf,1024,TYPE_S,0);//msgrcv()函数被用来从消息队列中取出消息。
	    printf("The message you receved(server say):%s\n",buf.mtext);

    }
    msgctl(msgid,IPC_RMID,NULL);
    return 0;
}


