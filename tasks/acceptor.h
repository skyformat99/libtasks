#ifndef _TASKS_ACCEPTOR_H_
#define _TASKS_ACCEPTOR_H_

#include <cassert>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <vector>

#include <tasks/io_task.h>
#include <tasks/dispatcher.h>
#include <tasks/logging.h>

namespace tasks {

	// Takes a handler class as argument that needs to take the client socket in its constructor.
	// See echo_server example.
	template<class T>
	class acceptor : public io_task {
	public:
		acceptor(int port) : io_task(-1, EV_READ) {
			// Create a non-blocking master socket.
			tdbg("acceptor: listening on port " << port << std::endl);
			int on = 1;
			int ret = 0;
			int fd = socket(PF_INET, SOCK_STREAM, 0);
			assert(fd > 0);
			assert(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on)) == 0);
			assert(fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK) == 0);
			struct sockaddr_in addr = {0};
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = INADDR_ANY;
			addr.sin_port = htons(port);
			assert(bind(fd, (struct sockaddr *) &addr, sizeof(addr)) == 0);
			assert(listen(fd, 128) == 0);
			set_fd(fd);
		}
		
		~acceptor() {
			shutdown(get_fd(), SHUT_RDWR);
		}

		bool handle_event(worker* worker, int revents)  {
			struct sockaddr_in addr;
			socklen_t len = sizeof(addr);
			int client = accept(get_fd(), (struct sockaddr *) &addr, &len);
			if (client < 0) {
				terr("acceptor: accept failed with errno " << errno << std::endl);
				return false;
			} else {
				tdbg("acceptor: new client fd " << client << std::endl);
				T* task = new T(client);
				add_io_task(worker, task);
			}
			return true;
		}
	};

} // tasks

#endif // _TASKS_ACCEPTOR_H_
