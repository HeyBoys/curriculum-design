#include<sys/epoll.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include"ProcessPond.h"
ProcessPond::ProcessPond()
{

}
ProcessPond::~ProcessPond()
{

}
int  ProcessPond::TryToSendMessage(const char * const buffer,int length)
{
	this->sort();
	int i,flag;
	for(i=0;i<5;i++)
	{
		flag = member[i].TryToSendMessage(buffer,length);
		if(flag != -1 )break;
	}
	return flag;
	
}
void ProcessPond::sort()
{
	//选择排序
	int i,j;
	int max ,maxid;
	for(i=4;i>=0;i--)
	{
		max = -1;
		for(j=0;j<=i;j++)
		{
			if(member[j].load > max)
				{
					max = member[j].load;
					maxid = j;
				}
		}
		if(maxid != i)
			swap(i,maxid);
	}
}
inline void ProcessPond::swap(int a,int b)
{
	ProcessMember t(0,0,0);
	t = member[a];
	member[a] = member[b];
	member[b] = t;
}
void ProcessPond::PipeEpollEventsRegister(int epoll_fd)
{
	int i;
	struct epoll_event ev;
	int fd1,fd2;
	for(i=0;i<5;i++)	
	{
		fd1 = member[i].get_fd_read();
		fd2 = member[i].get_fd_write();

		ev.events = EPOLLIN;
		ev.data.fd = fd1;
		if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd1,&ev) == -1)
		{
			printf("events add error\n");
			exit(1);
		}
		ev.events = EPOLLOUT;
		ev.data.fd = fd2;
		if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd2,&ev) == -1)
		{
			printf("events add error\n");
			exit(1);
		}
	}
}
void ProcessPond::ClearProcessLoad(int fd)
{
	char buffer;
	for(int i = 0;i<5;i++)
	{
		if(fd == member[i].get_fd_read())
		{
			read(fd,&buffer,1);
			member[i].load--;
			
		}
	}
}
