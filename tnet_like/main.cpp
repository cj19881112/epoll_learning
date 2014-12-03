#include "Channel.h"
#include "Socket.h"
#include "Mutex.h"
#include "Condition.h"

int main()
{
	Channel channel;
	Socket socket;


	Mutex mutex;
	Condition condition;

	{
		MutexGuard guard(mutex);
		condition.wait(mutex);
		//do_something();
	}



	return 0;
}

