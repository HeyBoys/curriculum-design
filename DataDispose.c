#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<hiredis/hiredis.h>
int main()
{
	int fd,i;
	
	char lengthbuffer[4];
	char redis[1024]={"set "};
	int length;
 	
	fd = open("../NFS_dir/log.txt",O_WRONLY | O_CREAT | O_APPEND,S_IRWXU | S_IRWXG | S_IRWXO);
	while(1)
	{
		length = 0;
		read(0,lengthbuffer,4);
		
		for(i=0;i<4;i++)
		{
			if(lengthbuffer[i] >= '0' && lengthbuffer[i] <= '9')
			{
				length *= 10;
				length += (lengthbuffer[i] - '0');
			}
			else break;
		}

		read(0,redis+4,length);
		

		redisContext *conn =redisConnect("127.0.0.1",6379);
		if(conn->err )
			
		{
			printf("connect error\n");
			return 0;
		}
		redisReply  *reply = (redisReply *)redisCommand(conn,redis);
		if(reply == NULL)
		{
			printf("command 1 error\n");
			return 0;
		}
	
		freeReplyObject(reply);
	
		write(fd,redis+4,length);
		write(1,redis,1);
	}	
}
