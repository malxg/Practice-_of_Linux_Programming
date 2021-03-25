#include "Unet.h"
#define SERVPORT 10086
#define BACKLOG 10
int main(int argc,char *argv[])
{
	int sockfd,i=0,z;
	char buf[80],str1[180];
	struct hostent *host;
	struct sockaddr_in adr_server;
	if((host=gethostbyname(argv[1]))==NULL)
	{
		perror("gethostbyname error!");
		exit(1);
	}
	printf("Link server port...\n");

	adr_server.sin_family=AF_INET;
	adr_server.sin_port=htons(SERVPORT);
	adr_server.sin_addr=*((struct in_addr *)host->h_addr);
	bzero(&(adr_server.sin_zero),8);
	
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd==-1)
	{
		perror("socket error!");
		exit(1);
	}
	printf("send file....\n");
	
	while(1)
	{
		fgets(buf,sizeof(buf),stdin);
		z=sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&adr_server,sizeof(adr_server));
		if(z<0)
		{
			perror("sendto error!");
			exit(1);
		}
		memset(buf,0,sizeof(buf));
	}		
	close(sockfd);
	exit(0);	
}
