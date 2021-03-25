#include "Unet.h"


int port=10086;
#define SERVPORT 10086
#define BACKLOG 10
int main()
{
	int sockfd,len,z;
	char buf[MAXDATASIZE];
	struct sockaddr_in adr_inet;
	struct sockaddr_in adr_clnt;
	printf("wait client\n");

	adr_inet.sin_family=AF_INET;
	adr_inet.sin_port=htons(port);
	adr_inet.sin_addr.s_addr=htonl(INADDR_ANY);
	bzero(&(adr_inet.sin_zero),8);
	len=sizeof(adr_clnt);
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd==-1)
	{
		perror("socket error!");
		exit(1);
	}
	
	z=bind(sockfd,(struct sockaddr *)&adr_inet,sizeof(adr_inet));
	if(z==-1)
	{
		perror("bind errro!");
		exit(1);
	}
	
	while(1)
	{
		z=recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&adr_clnt,&len);
		if(z<0)
		{
			perror("recvfrom error!");
			exit(1);
		}
		buf[z]='\0';
		printf("recive:%s",buf);
		
	}	
	close(sockfd);
	exit(0);
}
