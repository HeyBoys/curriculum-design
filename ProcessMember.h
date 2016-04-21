
class ProcessMember
{
	private:
			int fd_read_,fd_write_;


	public:		
			int load;
			ProcessMember();
			~ProcessMember();
			ProcessMember(int fd1,int fd2,int newload);
			int TryToSendMessage(const char * const buffer,int length);
			int get_fd_read();
			int get_fd_write();
};
