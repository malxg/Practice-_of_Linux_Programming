#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<pthread.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/wait.h>
#include <arpa/inet.h>
#define SERVPORT 12306
#define BACKLOG 10
#define MAXDATASIZE 100
