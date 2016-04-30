#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
int main()
{
	int fd;
	fd = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(4000);
	
	char buffer[1024]={'\0'};
	
	struct hostent *phe;
	
    if ( phe = gethostbyname("localhost") )
        memcpy(&servaddr.sin_addr, phe->h_addr, phe->h_length);
	if(connect(fd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
		printf("connet error\n");

	int i;
	for(i=1;i<=10;i++)
		{
			memset(buffer,0,sizeof(buffer));
			sprintf(buffer,"localhost_13:3%d NameError",i);
			sendto(fd,(void *)buffer,strlen(buffer),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
		}

}
