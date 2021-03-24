#include "net.h"

int main()
{
	int sockfd,client_fd;
	struct sockaddr_in my_addr;
	struct sockaddr_in remote_addr;
	int sin_size;
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket create error!");
		exit(1);
	}
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(SERVPORT);
	my_addr.sin_addr.s_addr=INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);
	if(bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr))==-1)
	{
		perror("bind failed!");
		exit(1);
	}
	if(listen(sockfd,BACKLOG)==-1)
	{
		perror("listen failed!");
		exit(1);
	}
	while(1)
	{
		sin_size=sizeof(struct sockaddr_in);
		if((client_fd=accept(sockfd,(struct sockaddr*)&remote_addr,&sin_size))==-1)
		{
			perror("accept error!");
			continue;
		}
		printf("accept a connection from %s\n ",inet_ntoa(remote_addr.sin_addr));
		if(!fork())
		{
			if(send(client_fd,"link successful\n",26,0)==-1)
			perror("send error!");
			exit(0);
		}
		printf("%s",remote_addr.sin_zero);
		close(client_fd);
	}	
}
