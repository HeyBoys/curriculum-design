#include<sys/epoll.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<netinet/in.h>
#include"UdpServer.h"

UdpServer::UdpServer()
{
	//注册epoll
	epoll_fd_ = epoll_create(256);

	//创建udp套接字并绑定端口
	if(!PassiveUdp())
	{
		printf("Udp error\n");
		exit(1);
	}

	//将udp套接字加入到epoll事件中
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = socket_fd_;


	if(epoll_ctl(epoll_fd_,EPOLL_CTL_ADD,socket_fd_,&ev) == -1)
	{
		printf("event add error\n");
		exit(1);
	}
	
	//创建进程池
	pond = new ProcessPond();

	//将进程池中需要监听的读写管道加入到epoll监听事件中
	pond->PipeEpollEventsRegister(epoll_fd_);

}
UdpServer::~UdpServer()
{
}

void UdpServer::run()
{
	int n;
	
	while(1)	
	{
		n = epoll_wait(epoll_fd_,ev,100,0);
		for(int i = 0;i<n;i++)
		{
			if(ev[i].data.fd == socket_fd_)//UDP网络套接字可读
			{
				DoNetwork();//做网络读取，尝试发送
			}
			else if(ev[i].events == EPOLLOUT)//写管道可写
				
				{
					//遍历未发送队列,发送消息	
					while( !WaitingToWorkQueue.empty())
					{
						if(pond->TryToSendMessage(WaitingToWorkQueue.front(),strlen(WaitingToWorkQueue.front())) != -1)

						WaitingToWorkQueue.pop();
						else break;
					}
				}
				else //是子进程发送数据，表明做完一次工作
				{
					pond->ClearProcessLoad(ev[i].data.fd);//该进程负载-1
				}
		}
	}
}
bool UdpServer::PassiveUdp()
{
	struct sockaddr_in server_addr;
	
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port =	htons(PORT_NUMBER);
	
	
	socket_fd_ = socket(AF_INET,SOCK_DGRAM,0);
    if (socket_fd_ < 0)
	{
	   printf("can't create socket: \n" );
	   exit(1);
	}
        /* Bind the socket */
    if (bind(socket_fd_, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("can't bind to  port: \n");
		exit(1);
    
	}
    
	return true;
}
void UdpServer::DoNetwork()
{
	int length;
	char buffer[1024];
	length = recvfrom(socket_fd_,buffer,1024,0,NULL,NULL);
	buffer[length] = '\0';
	if(pond->TryToSendMessage(buffer,length) == -1)
	{
		//等待队列添加成员
		char* point = new char[1024];
		strcpy(point,buffer);
		
		WaitingToWorkQueue.push(point);
	}
}
