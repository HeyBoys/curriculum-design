#include<queue>
#include<sys/epoll.h>
#include"ProcessPond.h"

#define PORT_NUMBER  4000
class UdpServer
{
	private:
		ProcessPond * pond;
		int epoll_fd_;
		int socket_fd_;
		struct epoll_event ev[100];
		std::queue<char *> WaitingToWorkQueue ;
		bool PassiveUdp();
		void DoNetwork();
	public:
		UdpServer();
		~UdpServer();
		void run();	
};
