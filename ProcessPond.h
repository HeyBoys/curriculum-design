#include"ProcessMember.h"
class ProcessPond
{
	private:
		ProcessMember member[5];
		void sort();
		inline void swap(int a,int b);
	
	public:
		ProcessPond();
		~ProcessPond();	
		int TryToSendMessage(const char * const buffer,int length);
		void PipeEpollEventsRegister(int epoll_fd);
		void ClearProcessLoad(int fd);	
};
