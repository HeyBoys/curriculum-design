#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include"ProcessMember.h"

int ProcessMember::TryToSendMessage(const char * const  buffer,int length)
{
	char buffer2[1024] = {'\0'};
	sprintf(buffer2,"%d",length);
	strcpy(buffer2+4,buffer);
	return write(fd_write_,buffer2,length+4);
}
ProcessMember::ProcessMember()
{
	int fd1[2],fd2[2];
	pipe(fd1);
	pipe(fd2);
	int flag;
	flag = fcntl(fd1[1],F_GETFL,0);
	if(fcntl(fd1[1],F_SETFL,flag|O_NONBLOCK)<0)
	{
		perror("fcntl");
		exit(1);
	}
	if(fork() == 0)
	{
		close(fd1[1]);
		close(fd2[0]);
		dup2(fd1[0],0);
		dup2(fd2[1],1);
		if(execl("./do","do",(char *)0) < 0)
		{
			printf("exec error\n");
			exit(1);
		}
		
	}
	close(fd1[0]);
	close(fd2[1]);

	fd_read_ = fd2[0];
	fd_write_ = fd1[1];
	load = 0;

}
//重载的带参数的构造函数 用于swap时构造一个空的对象
ProcessMember::ProcessMember(int fd1,int fd2,int newload):fd_read_(fd1),fd_write_(fd2),load(newload)
{

}
ProcessMember::~ProcessMember()
{
}
int ProcessMember::get_fd_read()
{
	return fd_read_;
}

int ProcessMember::get_fd_write()
{
	return fd_write_;
}
