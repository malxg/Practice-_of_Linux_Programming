#include "Unet.h"
int port=8888;
int main()
{
	int sockfd,i=0,z;
	char buf[80],str1[180];
	struct sockaddr_in adr_server;
	FILE *fp;
	printf("open file.....\n");
	
	fp=fopen("liu","r");
	if(fp==NULL)
	{
		perror("open error!\n");
		exit(1);
	}
	printf("Link server port...\n");

	adr_server.sin_family=AF_INET;
	adr_server.sin_port=htons(port);
	adr_server.sin_addr.s_addr=htonl(INADDR_ANY);
	bzero(&(adr_server.sin_zero),8);
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd==-1)
	{
		perror("socket error!");
		exit(1);
	}
	printf("send file....\n");

	for(i=0;i<3;i++)
	{
		fgets(str1,80,fp);
		printf("%d:%s",i,str1);
		sprintf(buf,"%d:%s",i,str1);
		z=sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&adr_server,sizeof(adr_server));
		if(z<0)
		{
			perror("recvform error!");
			exit(1);
		}
	}
	printf("send....\n");
	sprintf(buf,"stop\n");
	z=sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&adr_server,sizeof(adr_server));
	if(z<0)
	{
		perror("sendto error!");
		exit(1);
	}
	fclose(fp);
	close(sockfd);
	exit(0);	
}
