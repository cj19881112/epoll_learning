#include "../Reactor.h"
#include <iostream>
#include <cassert>
#include <cerrno>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>

using namespace std;

int tcp_listen(const char *ip, int port)
{

	int listenfd,connfd,n;
	struct sockaddr_in servaddr,cliaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd > 0);


	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip);
	servaddr.sin_port = htons(port);

	assert(0 == bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)));

	assert(0 == listen(listenfd,1024));

	return listenfd;
}

void setnonblocking(int sock){  
	int opts;  
	opts = fcntl(sock, F_GETFL);  
	if(opts < 0){  
		exit(1);  
	}  
	opts = opts | O_NONBLOCK;  
	if(fcntl(sock, F_SETFL, opts) < 0){  
		exit(1);  
	}  
}  

int main()
{
	int fd = tcp_listen("127.0.0.1", 10000);

	Reactor r;

	r.regist(fd, EPOLLIN, [](Reactor *r, int fd, int ev) {
		int clifd = accept(fd, 0, 0);
		assert(clifd > 0);

		setnonblocking(clifd);

		r->regist(clifd, EPOLLIN, [](Reactor *r, int fd, int ev) {
			char buf[4096] = {0, };
			int n = 0;

			do {
				n = read(fd, buf, 4096);
			} while (n < 0 && EINTR == errno);
			if (0 == n) {
				r->remove(fd);
				return ;
			}

			assert(n > 0);

			int err =  0;
			do {
				err = write(fd, buf, n);
			} while (err < 0 && EINTR == errno);
			assert(err > 0);
		});
	});

	r.react();
	return 0;
}

